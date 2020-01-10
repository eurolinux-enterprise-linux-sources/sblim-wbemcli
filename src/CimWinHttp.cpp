/*
 * $Id: CimWinHttp.cpp,v 1.5 2008/12/12 00:51:59 tyreld Exp $
 *
 * CimWinHttp.cpp
 *
 * (C) Copyright IBM Corp. 2004, 2008
 *
 * THIS FILE IS PROVIDED UNDER THE TERMS OF THE ECLIPSE PUBLIC LICENSE
 * ("AGREEMENT"). ANY USE, REPRODUCTION OR DISTRIBUTION OF THIS FILE
 * CONSTITUTES RECIPIENTS ACCEPTANCE OF THE AGREEMENT.
 *
 * You can obtain a current copy of the Eclipse Public License from
 * http://www.opensource.org/licenses/eclipse-1.0.php
 *
 * Author:       Adrian Schuur <schuur@de.ibm.com>
 * Contributors: Viktor Mihajlovski <mihajlov@de.ibm.com>
 *               Markus Mueller <markus_mueller@de.ibm.com>
 *               Steve Shepherd <steve.shepherd@configuresoft.com>
 *               Heidi Neumann  <heidineu@de.ibm.com>
 *
 * Description: Line command interface to DMTF conforming WBEM servers
*/
#include "stdafx.h"
#include "CimWinHttp.h"

////////////////////////////////////////////////////////////////////////
//
//  CimomWinHttp::CimomWinHttp( )
//  CimomWinHttp::~CimomWinHttp( )
//
//  Default constructor & destructor.
//
////////////////////////////////////////////////////////////////////////
extern int useNl;

CimomWinHttp::CimomWinHttp()
{
	m_hSession = NULL;
	m_hConnect = NULL;
	m_hRequest = NULL;
	m_lpPayload = NULL;
	m_dwPayloadSize = 0;
}

CimomWinHttp::~CimomWinHttp()
{
	if (m_lpPayload) delete m_lpPayload;
	if (m_hRequest)	::WinHttpCloseHandle(m_hRequest);
	if (m_hConnect)	::WinHttpCloseHandle(m_hConnect);
	if (m_hSession)	::WinHttpCloseHandle(m_hSession);
}

////////////////////////////////////////////////////////////////////////
//
//  CimomWinHttp::SetTimeouts( )
//
//  Change the timeouts for HTTP messages.
//
////////////////////////////////////////////////////////////////////////
bool CimomWinHttp::SetTimeouts(int Connect, int Send, int Receive)
{
	bool bReturn = false;

	if (m_hSession)
	{
		if (WinHttpSetTimeouts(m_hSession, 0, Connect, Send, Receive))
		{
			bReturn = true;
		}
	}

	return bReturn;
}

////////////////////////////////////////////////////////////////////////
//
//  CimomWinHttp::addPayload( )
//
//  Add the message payload to the HTTP message.
//
////////////////////////////////////////////////////////////////////////
void CimomWinHttp::addPayload(char *pl)
{
	addPayload(pl, strlen(pl));
}

void CimomWinHttp::addPayload(LPCVOID lpPayload, DWORD dwSize)
{
	if (m_lpPayload) delete m_lpPayload;
	m_lpPayload = new char[dwSize];
	m_dwPayloadSize = dwSize;
	if (m_lpPayload)
	{
		memcpy(m_lpPayload, lpPayload, dwSize);
	}
	else
	{
		throw("addPayload failed: could not allocate a buffer.");
	}
}

////////////////////////////////////////////////////////////////////////
//
//  CimomWinHttp::genRequest( )
//
//  Create the HTTP message.
//
////////////////////////////////////////////////////////////////////////
void CimomWinHttp::genRequest(URL &url, const char *op, bool cls, bool keys)
{
	if (!supportsSSL() && url.scheme == "https")
		throw HttpException("WinHttp does not support https urls.");
	
	DWORD dwLen, dwMBLen;
	WCHAR pszSingle[512];
	BOOL  bResult = FALSE;
	LPCWSTR sHeaders =	L"Content-type: application/xml; charset=\"utf-8\"\r\n"
						L"Connection: Keep-Alive, TE\r\n"
						L"CIMProtocolVersion: 1.0\r\n"
						L"CIMOperation: MethodCall\r\n";

	m_hSession = ::WinHttpOpen(L"EcmXpdTool", WINHTTP_ACCESS_TYPE_NO_PROXY,
						WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);

	if (!m_hSession)
	{
		throw("::WinHttpOpen failed");
	}


	INTERNET_PORT iPort;

	if (sscanf(url.port, "%hui", &iPort) != 1)
	{
		iPort = 5988;
	}

	dwLen = strlen(url.host) + 1;
	wchar_t *wpHost = static_cast<wchar_t*>(malloc((dwLen+1)*sizeof(wchar_t)));
	if (!wpHost)
	{
		throw("Insufficient memory available");
	}
	dwMBLen = MultiByteToWideChar(CP_UTF8, 0, url.host, dwLen, wpHost, dwLen);
	m_hConnect = ::WinHttpConnect(m_hSession, wpHost, iPort, 0);
	free(wpHost);

	if (!m_hConnect || dwMBLen == 0)
	{
		throw("::WinHttpConnect failed");
	}

	// Create an HTTP Request handle.
	m_hRequest = ::WinHttpOpenRequest(m_hConnect, L"POST", L"/cimom",
									   NULL, WINHTTP_NO_REFERER,
									   WINHTTP_DEFAULT_ACCEPT_TYPES, 0);
	if (!m_hRequest)
	{
		throw("::WinHttpOpenRequest failed");
	}

	bResult = ::WinHttpAddRequestHeaders(m_hRequest, sHeaders,
									ULONG_MAX, WINHTTP_ADDREQ_FLAG_ADD);


	dwLen = strlen(op) + 1;
	wchar_t *wpCimOp = static_cast<wchar_t*>(malloc((dwLen+1)*sizeof(wchar_t)));
	if (!wpCimOp)
	{
		throw("Insufficient memory available");
	}
	dwMBLen = MultiByteToWideChar(CP_UTF8, 0, op, dwLen, wpCimOp, dwLen);

	memset(pszSingle, '\0', sizeof(pszSingle));
	wcscat(pszSingle, L"CIMMethod: ");
	wcscat(pszSingle, wpCimOp);
	wcscat(pszSingle, L"\r\n");
	free(wpCimOp);

	bResult = ::WinHttpAddRequestHeaders(m_hRequest, pszSingle,
									ULONG_MAX, WINHTTP_ADDREQ_FLAG_ADD);

	string sb;
	url.ns.toStringBuffer(sb,"%2F");
 	if (cls)
	{
		sb = sb + "%3A" + url.cName;
		if (keys)
		{
			char sep = '.';
			int t=useNl;
			useNl=0;
			for ( unsigned i = 0 ; i < url.keys.size() ; i++ )
			{
				string sk;
				url.keys[i].toStringBuffer(sk, "");
				sb = sb + sep + sk;
				sep = ',';
			}
			useNl=t;
		}
	}
	dwLen = strlen(sb.c_str()) + 1;
	wchar_t *wpNS = static_cast<wchar_t*>(malloc((dwLen+1)*sizeof(wchar_t)));
	if (!wpNS)
	{
		throw("Insufficient memory available");
	}
	dwMBLen = MultiByteToWideChar(CP_UTF8, 0, sb.c_str(), dwLen, wpNS, dwLen);

	memset(pszSingle, '\0', sizeof(pszSingle));
	wcscat(pszSingle, L"CIMObject: ");
	wcscat(pszSingle, wpNS);
	wcscat(pszSingle, L"\r\n");
	free(wpNS);

	bResult = ::WinHttpAddRequestHeaders(m_hRequest, pszSingle,
									ULONG_MAX, WINHTTP_ADDREQ_FLAG_ADD);

	dwLen = strlen(url.password) + 1;
	wchar_t *wpPswd = static_cast<wchar_t*>(malloc((dwLen+1)*sizeof(wchar_t)));
	if (!wpPswd)
	{
		throw("Insufficient memory available");
	}
	dwMBLen = MultiByteToWideChar(CP_UTF8, 0, url.password, dwLen, wpPswd, dwLen);

	dwLen = strlen(url.user) + 1;
	wchar_t *wpUser = static_cast<wchar_t*>(malloc((dwLen+1)*sizeof(wchar_t)));
	if (!wpUser)
	{
		throw("Insufficient memory available");
	}
	dwMBLen = MultiByteToWideChar(CP_UTF8, 0, url.user, dwLen, wpUser, dwLen);

	bResult = WinHttpSetCredentials(m_hRequest, WINHTTP_AUTH_TARGET_SERVER,
									WINHTTP_AUTH_SCHEME_BASIC,
									wpUser, wpPswd, NULL);
	free(wpPswd);
	free(wpUser);
}

////////////////////////////////////////////////////////////////////////
//
//  CimomWinHttp::getResponse( )
//
//  Send the message and get the response.
//
////////////////////////////////////////////////////////////////////////
#ifdef OLD
string CimomWinHttp::getResponse()
{
	BOOL bResult;
	LPSTR pszOutBuffer = NULL;

    if (m_hRequest)
	{
        bResult = WinHttpSendRequest(m_hRequest,
                                       WINHTTP_NO_ADDITIONAL_HEADERS, 0,
                                       WINHTTP_NO_REQUEST_DATA, 0,
                                       m_dwPayloadSize, 0);

		// Write data to the server.
		if (bResult)
		{
			DWORD dwBytesWritten = 0;
			bResult = WinHttpWriteData(m_hRequest, m_lpPayload,
										m_dwPayloadSize, &dwBytesWritten);
		}

		// End the request.
		if (bResult)
		{
			bResult = WinHttpReceiveResponse(m_hRequest, NULL);
		}

		// Keep checking for data until there is nothing left.
		if (bResult)
		{
			DWORD dwContentLength = 0;
			DWORD dwCLSize = sizeof(dwContentLength);

			if (!WinHttpQueryHeaders(m_hRequest,
					WINHTTP_QUERY_CONTENT_LENGTH | WINHTTP_QUERY_FLAG_NUMBER,
					WINHTTP_HEADER_NAME_BY_INDEX, &dwContentLength, &dwCLSize,
					WINHTTP_NO_HEADER_INDEX))
			{
				DWORD dwLastError = ::GetLastError();
				LPVOID lpvMessageBuffer = NULL;
				DWORD dwFlags = FORMAT_MESSAGE_ALLOCATE_BUFFER |
								FORMAT_MESSAGE_FROM_HMODULE |
								FORMAT_MESSAGE_FROM_SYSTEM;

				if (::FormatMessage(dwFlags, m_hRequest, dwLastError, 0,
									(LPTSTR)&lpvMessageBuffer, 10, NULL) > 0)
				{
					throw((char *)lpvMessageBuffer);
				}
				else
				{
					throw("WinHttpQueryHeaders() failed");
				}
			}

			if (dwContentLength > 0)
			{
				DWORD dwSize = 0;
				DWORD dwTotal = 0;

				// Allocate space for the buffer.
				pszOutBuffer = new char[dwContentLength+1];
				if (!pszOutBuffer)
				{
					throw("Out of memory");
				}
				else
				{
					::ZeroMemory(pszOutBuffer, dwContentLength+1);
					do
					{
						// Check for available data.
						dwSize = 0;
						if (!WinHttpQueryDataAvailable(m_hRequest, &dwSize))
						{
							throw("WinHttpQueryDataAvailable failed");
						}

						// Read the Data.
						DWORD dwDownloaded = 0;
						if (!WinHttpReadData(m_hRequest,
												(LPVOID)(pszOutBuffer+dwTotal),
												dwSize, &dwDownloaded))
						{
							throw("WinHttpReadData failed");
						}
						else
						{
							dwTotal += dwSize;
						}

					} while (dwSize>0);

					// NULL terminate
					pszOutBuffer[dwTotal] = '\0';
				}
			}
		}

		// Report any errors.
		if (!bResult)
		{
			throw("GetResponse failed");
		}
	}
	else
	{
		throw("GetResponse failed: Request object not prepared.");
	}

   return (char *)(pszOutBuffer ? pszOutBuffer : "");
}
#endif	/* OLD */

////////////////////////////////////////////////////////////////////////
//
//  CimomWinHttp::getResponse( )
//
//  Send the message and get the response.
//
////////////////////////////////////////////////////////////////////////
string CimomWinHttp::getResponse()
{
	BOOL bResult;
    LPSTR pszOutBuffer = NULL;

	if (m_hRequest)
	{
        bResult = WinHttpSendRequest(m_hRequest,
                                       WINHTTP_NO_ADDITIONAL_HEADERS, 0,
                                       WINHTTP_NO_REQUEST_DATA, 0,
                                       m_dwPayloadSize, 0);

		// Write data to the server.
		if (bResult)
		{
			DWORD dwBytesWritten = 0;
			bResult = WinHttpWriteData(m_hRequest, m_lpPayload,
										m_dwPayloadSize, &dwBytesWritten);
		}

		// End the request.
		if (bResult)
		{
			bResult = WinHttpReceiveResponse(m_hRequest, NULL);
		}

		// Keep checking for data until there is nothing left.
		if (bResult)
		{
			DWORD dwSize = 0;
			DWORD dwTotal = 0;

			do 
			{
				// Check for available data.
				dwSize = 0;
				if (!WinHttpQueryDataAvailable(m_hRequest, &dwSize))
				{
					throw("WinHttpQueryDataAvailable failed");
				}

				if (dwSize <= 0)
				{
					continue;
				}

				// Allocate space for the buffer.
				LPSTR pszNewBuffer = new char[dwSize + 1];
				if (!pszNewBuffer)
				{
					throw("Out of memory");
				}
				else
				{
					DWORD dwDownloaded = 0;

					// Read the Data.
					::ZeroMemory(pszNewBuffer, dwSize + 1);

					if (!WinHttpReadData(m_hRequest, (LPVOID)pszNewBuffer, 
										  dwSize, &dwDownloaded))
					{
						throw("WinHttpReadData failed");
					}

					if (!pszOutBuffer)
					{
						pszOutBuffer = pszNewBuffer;
					}
					else
					{
						LPSTR pszTmpBuffer = new char[dwTotal + dwSize + 1];
						if (!pszTmpBuffer)
						{
							throw("Out of memory");
						}

						::CopyMemory(pszTmpBuffer, pszOutBuffer, dwTotal);
						::CopyMemory(pszTmpBuffer + dwTotal, pszNewBuffer, dwSize);

						delete [] pszOutBuffer;
						delete [] pszNewBuffer;

						pszOutBuffer = pszTmpBuffer;
					}
					dwTotal += dwSize;
				}
			} while (dwSize > 0);
		}

		// Report any errors.
		if (!bResult)
		{
			throw("GetResponse failed");
		}
	}
	else
	{
		throw("GetResponse failed: Request object not prepared.");
	}

   return (char *)(pszOutBuffer ? pszOutBuffer : "");
}

void CimomWinHttp::setClientCertificates(const char * cacert, int noverify,
					 const char * certificate,
					 const char * key) 
{
   if (!supportsSSL())
      throw HttpException("CimWinHttp does not support https urls.");
}
