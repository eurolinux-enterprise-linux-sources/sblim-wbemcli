/*
 * $Id: Cimwww.cpp,v 1.11 2008/12/12 00:52:00 tyreld Exp $
 *
 * CimWww.cpp
 *
 * (C) Copyright IBM Corp. 2002, 2008
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
 *               David Eger <dteger@us.ibm.com>
 *
 * Description: Line command interface to DMTF conforming WBEM servers
*/

#include "Cimwww.h"

extern int useNl;

PRIVATE int printer (const char * fmt, va_list pArgs)
{
  return 0;
   //return (vfprintf(stdout, fmt, pArgs));
}

PRIVATE int tracer (const char * fmt, va_list pArgs)
{
  return 0;
  //return (vfprintf(stderr, fmt, pArgs));
}

PRIVATE int term_handler (HTRequest * request, HTResponse * response,
			  void * param, int status)
{
    HTList      * error_list = NULL;
    HTAssocList * headers = NULL;
    HTAssoc     * header = NULL;
    string      * buf = (string*)param;
    char *        errorstring = NULL;

    if (request) {
      error_list = HTRequest_error (request);
      HTError_setShow(HT_ERR_SHOW_DEBUG);

      errorstring=HTDialog_errorMessage(request,HT_A_MESSAGE,
					0,NULL,error_list);
    }

    if (response && errorstring) {
      char *lf="";
      (*buf)=errorstring;
      headers = HTResponse_header(response);
      while ((header=(HTAssoc*)HTAssocList_nextObject(headers))) {
	(*buf)+=string(lf)+string("* ")+
	       HTAssoc_name(header)+": "+HTAssoc_value(header);
	 lf="\n";
      }
    }
    if (errorstring) free(errorstring);
    /* We are done with this request */
    HTRequest_delete(request);
    HTEventList_stopLoop();
    return 0;
}

CimomWww::CimomWww()
 // : errorbuf(500)
{
 // HTAlertCallback * cbf;
  HTProfile_newNoCacheClient("libwww-wbemcli", "1.0");
  HTPrint_setCallback(&printer);
  HTTrace_setCallback(&tracer);
  HTNet_addAfter(&term_handler, NULL, &errorbuf, HT_ALL, HT_FILTER_LAST);
   //HTSetTraceMessageMask("p");
}

CimomWww::~CimomWww()
{
  HTProfile_delete();
}

void CimomWww::genRequest(URL &url, const char *op, bool cls, bool keys)
{
   if (!supportsSSL() && url.scheme == "https")
      throw HttpException("CimomWww does not support https urls.");
   string sb,uri;
   char clearbuf[90];
   char secretbuf[120];
   char authbuf[130];

   uri="http://"+url.host+":";
   uri+=url.port+"/cimom";
   url.ns.toStringBuffer(sb,"%2F");
   
   if (cls) {
      sb = sb + "%3A" + url.cName;
      if (keys)
      {
         char sep = '.';
	 int t=useNl;
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

   request=HTRequest_new();
   HTRequest_setOutputFormat(request, HTAtom_for("application/xml"));
   HTRequest_setOutputStream(request,HTStreamToChunk(request,&chunk,0));
   HTRequest_addExtraHeader(request,"CIMProtocolVersion","1.0");
   HTRequest_addExtraHeader(request,"CIMOperation","MethodCall");
   HTRequest_addExtraHeader(request,"CIMMethod",op);
   HTRequest_addExtraHeader(request,"CIMObject",(char*)sb.c_str());

   /* This is more or less a hack to enable authentication */
   /* We can't use the libwww filters as SNIA is generating invalid realms */
   if (url.user.size()) {
     strcpy(clearbuf,url.user.c_str());
     strcat(clearbuf,":");
     strcat(clearbuf,url.password.c_str());
     HTUU_encode((unsigned char *)clearbuf,strlen(clearbuf),secretbuf);
     strcpy(authbuf,"Basic ");
     strcat(authbuf,secretbuf);
     HTRequest_addCredentials(request,"Authorization",authbuf);
   }
   anchor = HTAnchor_findAddress(uri.c_str());
}


string CimomWww::getResponse()
{
   char *rv=NULL;

   if (!HTPostAnchor(parent,anchor,request))
     throw new HttpException("Unable to process request");

   HTEventList_loop(request);
   rv = HTChunk_toCString(chunk);

   if (dumpXml) {
      if (rv) cerr<<"From server: "<<rv<<endl;
      else if (errorbuf.size()>0) cerr<<"From server: "<<errorbuf<<endl;
      else cerr<<"From server: NO DATA RECEIVED"<<endl;
   }

   if (rv == NULL || strlen(rv) == 0 && errorbuf.size()>0) {
      if (rv) free(rv);
      throw HttpException(errorbuf);
   }

   string rvstr=string(rv);
   free(rv);
   return rvstr;
}

void CimomWww::addPayload(const string &pl)
{
   parent = HTTmpAnchor(NULL);
   if (dumpXml) cerr<<"To server: "<<pl.c_str()<<endl;
   HTAnchor_setDocument(parent,(void*)pl.c_str());
   HTAnchor_setFormat(parent, HTAtom_for("application/xml"));
   HTAnchor_setCharset(parent, HTAtom_for("utf-8"));
   HTAnchor_setLength(parent, strlen(pl.c_str()));
}

void CimomWww::setClientCertificates(const char * cacert, int noverify,
				     const char * certificate,
				     const char * key) 
{
   if (!supportsSSL())
      throw HttpException("CimomWww does not support https urls.");
}
