/*
 * $Id: SunSMC.cpp,v 1.2 2008/08/07 18:25:41 tyreld Exp $
 *
 * SunSMC.cpp
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
 * Description: The version of cimom that ships with Solaris 8 has an
 *              EnumerateClasses method instead of the (now) standard
 *              EnumerateClassNames method.
*/

#include "CimXml.h"


string Cimom::enumClassesSMC(URL *url, bool deep, bool qual)
{
   string sb;

   genRequest(*url,"EnumerateClassNames");

   sb = xmlHeader;
   sb += "<IMETHODCALL NAME=\"EnumerateClasses\"><LOCALNAMESPACEPATH>";

   for (int i=0,s=url->ns.size(); i<s; i++)
      sb += "<NAMESPACE NAME=\"" + url->ns[i] + "\"></NAMESPACE>";

   sb += "</LOCALNAMESPACEPATH>\n";
   sb += "<IPARAMVALUE NAME=\"ClassName\"><CLASSNAME NAME=\""
      + url->cName + "\"/></IPARAMVALUE>\n";

   if (deep)
      sb += "<IPARAMVALUE NAME=\"DeepInheritance\"><VALUE>TRUE</VALUE></IPARAMVALUE>\n";
   else
      sb += "<IPARAMVALUE NAME=\"DeepInheritance\"><VALUE>FALSE</VALUE></IPARAMVALUE>\n";

   if (qual)
      sb += "<IPARAMVALUE NAME=\"IncludeQualifiers\"><VALUE>TRUE</VALUE></IPARAMVALUE>\n";
   else
      sb += "<IPARAMVALUE NAME=\"IncludeQualifiers\"><VALUE>FALSE</VALUE></IPARAMVALUE>\n";

   sb += "</IMETHODCALL>";
   sb += xmlTrailer;

   addPayload(sb);
   return getResponse();
}
