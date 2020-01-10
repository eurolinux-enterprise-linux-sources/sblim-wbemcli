/*
 * $Id: CimWinHttp.h,v 1.4 2008/12/12 00:51:59 tyreld Exp $
 *
 * CimWinHttp.h
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


#ifndef CIMWINHTTP_H
#define CIMWINHTTP_H

#include <winhttp.h>
#include "CimXml.h"


class CimomWinHttp : public Cimom
{
  private:
   virtual void addPayload(char *pl);

   virtual void genRequest(URL &url, const char *op, bool cls=false, bool keys=false);
   virtual void addPayload(LPCVOID lpPayload, DWORD dwSIze);
   virtual string getResponse();

  public:
   CimomWinHttp::CimomWinHttp();
   CimomWinHttp::~CimomWinHttp();

   // Timeouts are in milli-seconds (these are the defaults)
   virtual bool SetTimeouts(int Connect = 60000,
							int Send = 30000,
							int Receive = 30000);

   void setClientCertificates(const char * cacert,
			      int noverify,
			      const char * clientcert,
			      const char * clientkey);
  private:
	HINTERNET			m_hSession;
	HINTERNET			m_hConnect;
	HINTERNET			m_hRequest;
	LPVOID				m_lpPayload;
	DWORD				m_dwPayloadSize;

};

#endif	/* CIMWINHTTP_H */
