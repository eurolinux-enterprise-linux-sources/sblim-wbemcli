/*
 * $Id: CimCurl.cpp,v 1.13 2009/03/04 19:10:54 tyreld Exp $
 *
 * CimCurl.cpp
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
 * Author:       Philip K. Warren <pkw@us.ibm.com>
 *
 * Contributors: Viktor Mihajlovski <mihajlov@de.ibm.com>
 *
 * Description: Line command interface to DMTF conforming WBEM servers
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#if HAVE_STRING_H
#include <string.h>
#endif

#include "CimCurl.h"

extern int useNl;
extern int dumpXml;
extern int reqChunking;

// These are the constant headers added to all requests
static const char *headers[] = {
    "Content-Type: application/xml; charset=\"utf-8\"",
    "Connection: Keep-Alive, TE",
    "CIMProtocolVersion: 1.0",
    "CIMOperation: MethodCall",
    "Accept:",
    "Expect:",
};
#define NUM_HEADERS ((sizeof(headers))/(sizeof(headers[0])))

#if LIBCURL_VERSION_NUM >= 0x070a00
static const curl_version_info_data *version = curl_version_info(CURLVERSION_NOW);
#endif

//
// NOTE:
//
// All strings passed to libcurl are not copied so they must have a lifetime
// at least as long as the last function call made on the curl handle. The
// only exception to this seems to be strings passed to the curl_slist_append
// function which are copied.
//

CimomCurl::CimomCurl() 
{
    mHandle = curl_easy_init();
    mBody = "";
    mHeaders = NULL;
    mResponse.str("");
    
    mErrorData.mStatusCode = 0;
    mErrorData.mStatusDescription= "";
    mErrorData.mError= "";
}

CimomCurl::~CimomCurl()
{
    if (mHandle)
        curl_easy_cleanup(mHandle);
    if (mHeaders)
        curl_slist_free_all(mHeaders);
}

bool CimomCurl::supportsSSL()
{
#if LIBCURL_VERSION_NUM >= 0x070a00
    if (version && (version->features & CURL_VERSION_SSL))
        return true;
    
    return false;
#else
    // Assume we support SSL if we don't have the curl_version_info API
    return true;
#endif
}

void CimomCurl::initializeHeaders()
{
    // Free any pre-existing headers
    if (mHeaders) {
        curl_slist_free_all(mHeaders);
        mHeaders = NULL;
    }

    // Add all of the common headers
    unsigned int i;
    for (i = 0; i < NUM_HEADERS; i++)
        mHeaders = curl_slist_append(mHeaders, headers[i]);
    
    if (reqChunking)    
       mHeaders = curl_slist_append(mHeaders, "TE: trailers");
}

static size_t writeCb(void *ptr, size_t size, size_t nmemb, void *stream)
{
    stringstream *str = (stringstream*)stream;
    int length = size * nmemb;
    (*str).write((char *)ptr, length);
    return length;
}

static size_t headerCb(void *ptr, size_t size, size_t nmemb, void *stream)
{
    CimErrorData *ed = (CimErrorData*)stream;
    int length = size * nmemb;
    char *cln,*crp;
    
    if ((cln=strchr((char*)ptr,':'))) {
       crp=strchr((char*)ptr,'\r');
       if (crp) *crp=0;
       
       if (dumpXml) cerr<<"From server: "<<(char*)ptr<<endl;
       
       if (strncasecmp(cln-20,"CIMStatusDescription",20)==0) 
          ed->mStatusDescription=cln+2;
       else if (strncasecmp(cln-13,"CIMStatusCode",13)==0)
          ed->mStatusCode=atoi(cln+2);
       else if (strncasecmp(cln-13,"CIMError",8)==0)
          ed->mError=atoi(cln+2);
       if (crp) *crp='\n';
    }
    return length;
}

void CimomCurl::genRequest(URL &url, const char *op, bool cls, bool keys)
{
    if (!mHandle)
        throw HttpException("Unable to initialize curl interface.");
    
    if (!supportsSSL() && url.scheme == "https")
        throw HttpException("this curl library does not support https urls.");

    CURLcode rv;
    string sb;
    
    mUri = url.scheme + "://" + url.host + ":" + url.port + "/cimom";
    url.ns.toStringBuffer(sb,"%2F");

    /* Initialize curl with the url */
    rv = curl_easy_setopt(mHandle, CURLOPT_URL, mUri.c_str());

    /* Disable progress output */
    rv = curl_easy_setopt(mHandle, CURLOPT_NOPROGRESS, 1);

    /* This will be a HTTP post */
    rv = curl_easy_setopt(mHandle, CURLOPT_POST, 1);

    /* Disable SSL host verification */
    rv = curl_easy_setopt(mHandle, CURLOPT_SSL_VERIFYHOST, 0);
    //    rv = curl_easy_setopt(mHandle, CURLOPT_SSL_VERIFYPEER, 0);
    
    /* Force using SSL V3 */
    rv = curl_easy_setopt(mHandle, CURLOPT_SSLVERSION, 3);    

    /* Set username and password */
    if (url.user.length() > 0 && url.password.length() > 0) {
        mUserPass = url.user + ":" + url.password;
        rv = curl_easy_setopt(mHandle, CURLOPT_USERPWD, mUserPass.c_str());
    }

    /* Prompt for password */
    if (url.user.length() > 0 && !url.password.length() > 0) {
       url.password = getpass ("Enter password:");
       mUserPass = url.user + ":" + url.password;
       rv = curl_easy_setopt(mHandle, CURLOPT_USERPWD, mUserPass.c_str());
    }
    
    if (cls)
    {
        sb = sb + "%3A" + url.cName;
        if (keys)
        {
            char sep = '.';
            int t = useNl;
            useNl=0;
            for (unsigned i = 0 ; i < url.keys.size() ; i++ ) {
                string sk;
                url.keys[i].toStringBuffer(sk, "");
                sb = sb + sep + sk;
                sep = ',';
            }
            useNl=t;
        }
    }
    
    // Initialize default headers
    initializeHeaders();

    // Add CIMMethod header
    string method = "CIMMethod: ";
    method += op;
    mHeaders = curl_slist_append(mHeaders, method.c_str());

    // Add CIMObject header
    string object = "CIMObject: ";
    object += sb;
    mHeaders = curl_slist_append(mHeaders, object.c_str());

    // Set all of the headers for the request
    rv = curl_easy_setopt(mHandle, CURLOPT_HTTPHEADER, mHeaders);

    // Set up the callbacks to store the response
    rv = curl_easy_setopt(mHandle, CURLOPT_WRITEFUNCTION, writeCb);

    // Use CURLOPT_FILE instead of CURLOPT_WRITEDATA - more portable
    rv = curl_easy_setopt(mHandle, CURLOPT_FILE, &mResponse);
    
    // Fail if we receive an error (HTTP response code >= 300)
    rv = curl_easy_setopt(mHandle, CURLOPT_FAILONERROR, 1);

    char * curldebug = getenv("CURLDEBUG");
    if (curldebug && strcasecmp(curldebug,"false")) { 
      // Turn this on to enable debugging
      rv = curl_easy_setopt(mHandle, CURLOPT_VERBOSE, 1);
    }
    
    rv = curl_easy_setopt(mHandle, CURLOPT_WRITEHEADER, &mErrorData);
    rv = curl_easy_setopt(mHandle, CURLOPT_HEADERFUNCTION, headerCb);
}

static string getErrorMessage(CURLcode err)
{
    string error;
#if LIBCURL_VERSION_NUM >= 0x070c00
    error = curl_easy_strerror(err);
#else
    error = "CURL error: ";
    error += err;
#endif
    return error;
}

string CimomCurl::getResponse()
{
    CURLcode rv;
    mResponse.str("");

    rv = curl_easy_perform(mHandle);
    if (rv) {
        long responseCode = -1;
        string error;
        // Use CURLINFO_HTTP_CODE instead of CURLINFO_RESPONSE_CODE
        // (more portable to older versions of curl)
        curl_easy_getinfo(mHandle, CURLINFO_HTTP_CODE, &responseCode);
        if (responseCode == 401) {
            error = (mUserPass.length() > 0) ? "Invalid username/password." :
                                               "Username/password required.";
        }
        else {
            error = getErrorMessage(rv);
        }
        throw HttpException(error);
    }

    string response = mResponse.str();
    
    if (dumpXml)
        cerr << "From server: " << response << endl;
    
    if (mErrorData.mStatusCode!=0) 
        throw ErrorXml(mErrorData.mStatusCode,mErrorData.mStatusDescription);
    
    if (response.length() == 0)
        throw HttpException("No data received from server.");
    
    return response;
}

void CimomCurl::addPayload(const string& pl)
{
    mBody = pl;
    if (dumpXml)
        cerr << "To server: " << pl << endl;

    CURLcode rv;

    rv = curl_easy_setopt(mHandle, CURLOPT_POSTFIELDS, mBody.c_str());
    if (rv)
        cerr << getErrorMessage(rv) << endl;
    rv = curl_easy_setopt(mHandle, CURLOPT_POSTFIELDSIZE, mBody.length());
    if (rv)
        cerr << getErrorMessage(rv) << endl;
}

void CimomCurl::setClientCertificates(const char * cacert, int noverify,
				      const char * certificate,
				      const char * key) 
{
   CURLcode rv;
   if (!supportsSSL())
      throw HttpException("This CimomCurl does not support https urls.");
   
   if (noverify) {
     if ((rv=curl_easy_setopt(mHandle,CURLOPT_SSL_VERIFYPEER,0))) {
       cerr << getErrorMessage(rv) << endl;
       throw HttpException("Could not disable peer verification.");
     }
   } else if (cacert) {
     if ((rv=curl_easy_setopt(mHandle,CURLOPT_SSL_VERIFYPEER,1))) {
       cerr << getErrorMessage(rv) << endl;
       throw HttpException("Could not enable peer verification.");
     }
     if ((rv=curl_easy_setopt(mHandle,CURLOPT_CAINFO,cacert))) {
       cerr << getErrorMessage(rv) << endl;
       throw HttpException("Could not load CA certificate.");
     }
   } else {
     throw HttpException("Must either specify -noverify or -cacert for https URLs.");
   }
   if (certificate && key) {
     if ((rv=curl_easy_setopt(mHandle,CURLOPT_SSLCERT,certificate))) {
       cerr << getErrorMessage(rv) << endl;
       throw HttpException("Could not load client certificate.");
     }
     if ((rv=curl_easy_setopt(mHandle,CURLOPT_SSLKEY,key))) {
       cerr << getErrorMessage(rv) << endl;
       throw HttpException("Could not load client key.");
     }
   } else if (certificate && key == NULL || certificate == NULL && key) {
     throw HttpException("Must specify both -clientcert and -clientkey or none.");
   }
}
