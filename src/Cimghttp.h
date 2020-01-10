/*
 * $Id: Cimghttp.h,v 1.7 2008/12/12 00:52:00 tyreld Exp $
 *
 * CimGhttp.h
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


#ifndef CIMGHTTP_H
#define CIMGHTTP_H

#include "CimXml.h"
#include <ghttp.h>

class CimomGhttp : public Cimom {
  private:
   ghttp_request *request;
   void genRequest(URL &url, const char *op, bool cls=false, bool keys=false);
   void addPayload(const string& pl);
   string getResponse();
  public:
   CimomGhttp();
   ~CimomGhttp() { if (newstr) free(newstr); }
   void setClientCertificates(const char * cacert,
			      int noverify,
			      const char * clientcert,
			      const char * clientkey);
};

#endif	/* CIMGHTTP_H */
