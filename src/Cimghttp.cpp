/*
 * $Id: Cimghttp.cpp,v 1.9 2008/12/12 00:52:00 tyreld Exp $
 *
 * Cimghttp.cpp
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

#include "Cimghttp.h"

extern int useNl;

CimomGhttp::CimomGhttp() 
{
   request = 0;
   newstr=NULL;
}

void CimomGhttp::genRequest(URL &url, const char *op, bool cls, bool keys)
{
   if (!supportsSSL() && url.scheme == "https")
       throw HttpException("ghttp does not support https urls.");

   string sb, uri;

   uri = uri + "http://" + url.host + ":";
   uri = uri + url.port + "/cimom";
   url.ns.toStringBuffer(sb,"%2F");

   request=ghttp_request_new();
   // bug in libghttp.  it actually just makes a copy of uri.c_str(), should be const
   ghttp_set_uri(request,(char *)uri.c_str());
   ghttp_set_type(request,ghttp_type_post);
   ghttp_set_header(request,http_hdr_Content_Type,"application/xml; charset=\"utf-8\"");
   ghttp_set_header(request,"Connection","Keep-Alive, TE");
   ghttp_set_header(request,"CIMProtocolVersion","1.0");
   ghttp_set_header(request,"CIMOperation","MethodCall");
   ghttp_set_header(request,"CIMMethod",op);

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
   ghttp_set_header(request,"CIMObject",sb.c_str());
   ghttp_set_authinfo(request,url.user.c_str(),url.password.c_str());
}


string CimomGhttp::getResponse()
{
   string rv;
   int rc,len;

   if(ghttp_prepare(request))
     throw HttpException("Bad Request.  Probably an error in WBEMCLI");
//cerr << "URI prepared " << rc << endl;
   rc=ghttp_process(request);
   if (rc!=ghttp_done)
     throw HttpException("Unable to process request");

//cerr << "ret_code " << rc << endl;
   len=ghttp_get_body_len(request);
   if(len==0)
       throw HttpException("No data received from Server. Probably authentication error.");
       
   rv = ghttp_get_body(request);

   //ghttp_current_status_tag gcs = ghttp_get_status(request);
   //cerr << "len " << len << " and stat " << gcs.proc << ","
   //     << gcs.bytes_read << "," << gcs.bytes_total << endl;
   //cerr << " reason " << ghttp_reason_phrase(request)
   //     << "   error " << ghttp_get_error(request) << endl;
   ghttp_request_destroy(request);
   free(newstr);
   newstr=NULL;
   if (dumpXml) cerr<<"From server: "<<rv<<endl;
   return rv;
}

void CimomGhttp::addPayload(const string& pl)
{
   // char * newstr; // memory leak!
   // libghttp needs to be rewritten to make a copy or take ownership of the string
   newstr = strdup(pl.c_str());
   if (dumpXml) cerr << "To server: " << pl << endl;
   if (ghttp_set_body(request, newstr, pl.size()))
      cerr << "http request does not support this " << endl;
}

void CimomGhttp::setClientCertificates(const char * cacert, int noverify,
				       const char * certificate,
				       const char * key) 
{
   if (!supportsSSL())
      throw HttpException("CimomGhttp does not support https urls.");
}
