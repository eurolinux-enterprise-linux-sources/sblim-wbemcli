/*
 * $Id: CimCurl.h,v 1.5 2008/12/12 00:51:59 tyreld Exp $
 *
 * CimCurl.h
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
 * Description: Command-line interface to DMTF conforming WBEM servers
*/


#ifndef CIMCURL_H
#define CIMCURL_H

#include "CimXml.h"
#include <curl/curl.h>
#include <sstream>


class CimomCurl : public Cimom {
 public:
    CimomCurl();
    ~CimomCurl();
   void setClientCertificates(const char * cacert,
			      int noverify,
			      const char * clientcert,
			      const char * clientkey);
    
 private:
    // The handle to the curl object
    CURL *mHandle;
    // The list of headers sent with each request
    struct curl_slist *mHeaders;
    // The body of the request
    string mBody;
    // The uri of the request
    string mUri;
    // The username/password used in authentication
    string mUserPass;
    // Used to store the HTTP response
    stringstream mResponse;

    bool supportsSSL();
    void genRequest(URL &url, const char *op, bool cls=false, bool keys=false);
    void addPayload(const string& pl);
    string getResponse();

    // Initializes the HTTP headers
    void initializeHeaders();
};

#endif	/* CIMCURL_H */
