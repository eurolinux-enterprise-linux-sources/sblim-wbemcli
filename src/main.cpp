/*
 * $Id: main.cpp,v 1.34 2009/06/24 17:44:48 tyreld Exp $
 *
 * main.cpp
 *
 * (C) Copyright IBM Corp. 2002, 2004, 2008
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
 *               Philip K. Warren <pkw@us.ibm.com>
 *               Tyrel Datwyler <tyreld@us.ibm.com>
 *
 * Description: Line command interface to DMTF conforming WBEM servers
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#if HAVE_STRING_H
#include <string.h>
#endif

#include "CimXml.h"

#ifdef HAVE_LIBWWW
#include "Cimwww.h"
#elif HAVE_LIBCURL
#include "CimCurl.h"
#elif HAVE_LIBGHTTP
#include "Cimghttp.h"
#elif WIN32
#include "CimWinHttp.h"
#else
#error "No HTTP Library Configured"
#endif

#ifndef WIN32
#include <ctype.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#endif

using namespace std;

extern int useNl;
extern int noVal;
extern int addType;
extern int openwbem;
extern int dlmRefs;
extern int reqChunking;



static void usage(int indir, int op, char *cmd)
{
   cerr<<"usage:";
   cerr<<"\twbemcli gc|gcd|dc|ec|ecn|gi|ci|mi|di|ei|ein|ain|ai|rin|ri|gp|sp|cm|cmx\n"
         "\t        [-nl] [-dx] [-t] [-cte] [-h] [-v]\n"
         "\t        [-noverify] [-cacert <file>] [-clientcert <file>] [-clientkey <file>]\n"
         "\t        [-ac <assocClass>] [-arc <resultClass>] [-ar <role>] [-arr <resultRole>]\n"
         "\t        objectPath [prop=value,[...]] [props[,...]]\n"<<endl;
   cerr<<"\texample: wbemcli gi 'http://localhost:5988/root/cimv2:rpm_package.name=\"glibc\"'\n"<<endl;
   cerr<<"\tUse man wbemcli for details"<<endl;

}

static int hexValue(char v)
{
   if ((v-'0')<10) return v-'0';
   if (v>='a' && v<='f') return v-'a'+10;
   if (v>='A' && v<='F') return v-'A'+10;
   else return -1;
}

static void cleanUpMsg(string &msg)
{
   for (int m=msg.size(),i=0; i<m; i++) {
      if (msg.at(i)=='\n') {
         if (i<m-1 && msg.at(i+1)!='*') {
	    if (msg.at(i+1)=='\n') { msg.erase(i,1); i--; m--; continue; }
	    msg.insert(i+1,"* ");
	    m+=2;
	    i+=2;
	    continue;
	 }
      }
      else if (msg.at(i)=='%') {
         char c[2]=" ";
         if (i<m-2) {
	    if ((c[0]=hexValue(msg.at(i+1))*16)>=0) {
	       if (hexValue(msg.at(i+2))>=0) {
	          c[0]|=hexValue(msg.at(i+2));
	          msg.erase(i,3);
	          msg.insert(i,c);
	          m-=2;
	          continue;
	       }
	    }
	 }
      }
   }
}


int main (int argc, char *argv[]) {
   int pos, retval = 0;
   char *cmd, *params, *opStr=NULL,*urlStr=NULL,*xtraStr=NULL;
   char *assocClass=NULL,*resultClass=NULL,*role=NULL,*resultRole=NULL;
   const char *cacert=CACERT, *clientcert=NULL, *clientkey=NULL;
   int n=1,err=0,op=0,cmdln,help=0,indir=0, noverify=0;
   ClassXml *cls=0;
   InstanceXml *inst=0;
   InstanceNameXml *in=0;
   InstanceNamesXml *ina=0;
   NamedInstancesXml *nia=0;
   ObjectsWithPathXml *owp=0;
   ValueXml *va=0;
   // InstancesXml *ia; // - not yet used
   ClassNamesXml *cna=0;
   ClassesXml *ca=0;
   MethodresponseXml *mrsp=0;
   string sb;
#ifdef HAVE_LIBWWW
   CimomWww cimom;
#elif HAVE_LIBCURL
   // This needs to be called once before any curl functions are called
   curl_global_init(CURL_GLOBAL_ALL);
   CimomCurl cimom;
#elif HAVE_LIBGHTTP
   CimomGhttp cimom;
#else
   CimomWinHttp cimom;
#endif

   cmd=argv[0];
   cmdln=strlen(cmd);
   n=1;

#ifdef DBGX
   cerr << "Invoked as: " << cmd << "\n";
#endif
 
   cimom.mErrorData.mStatusCode=0;
   
   indir=1;
   if (strcmp(cmd+cmdln-6,"wbemgi")==0) 	op=GetInstance;
   else if (strcmp(cmd+cmdln-7,"wbemein")==0) 	op=EnumInstanceNames;
   else if (strcmp(cmd+cmdln-6,"wbemei")==0) 	op=EnumInstances;
   else if (strcmp(cmd+cmdln-6,"wbemci")==0) 	op=CreateInstance;
   else if (strcmp(cmd+cmdln-6,"wbemmi")==0) 	op=ModifyInstance;
   else if (strcmp(cmd+cmdln-6,"wbemdi")==0) 	op=DeleteInstance;
   else if (strcmp(cmd+cmdln-6,"wbemgc")==0) 	op=GetClass;
   else if (strcmp(cmd+cmdln-6,"wbemec")==0) 	op=EnumClasses;
   else if (strcmp(cmd+cmdln-7,"wbemecn")==0) 	op=EnumClassNames;
   else if (strcmp(cmd+cmdln-6,"wbemdc")==0) 	op=DeleteClass;
   else if (strcmp(cmd+cmdln-7,"wbemgcd")==0) 	op=GetClassDef;
   /* ----------- Associator Provider Interface ---------------- */
   else if (strcmp(cmd+cmdln-7,"wbemain")==0) 	op=AssociatorNames;
   else if (strcmp(cmd+cmdln-6,"wbemai")==0) 	op=Associators;
   else if (strcmp(cmd+cmdln-7,"wbemrin")==0) 	op=ReferenceNames;
   else if (strcmp(cmd+cmdln-6,"wbemri")==0) 	op=References;
   /* -------- end of Associator Provider Interface ------------ */
   else if (strcmp(cmd+cmdln-6,"wbemgp")==0) 	op=GetProperty;
   else if (strcmp(cmd+cmdln-6,"wbemsp")==0) 	op=SetProperty;
   else if (strcmp(cmd+cmdln-6,"wbemcm")==0) 	op=CallMethod;
   else if (strcmp(cmd+cmdln-7,"wbemcmx")==0) 	op=CallMethodX;
   else n=op=indir=0;

   try {
     if (argc==1) {
       help=1; // force usage directions if no args
     }
      for (argv++; *argv; argv++) {
         if (*argv[0]=='-') {
           if (strcmp(*argv,"-nl")==0) useNl=1;
           else if (strcmp(*argv,"-noverify")==0) noverify=1;
           else if (strcmp(*argv,"-cacert")==0) cacert=*(++argv);
           else if (strcmp(*argv,"-clientcert")==0) clientcert=*(++argv);
           else if (strcmp(*argv,"-clientkey")==0) clientkey=*(++argv);
           else if (strcmp(*argv,"-dx")==0) dumpXml=1;
           else if (strcmp(*argv,"-cte")==0) reqChunking=1;
           else if (strcmp(*argv,"-t")==0) addType=1;
           else if (strcmp(*argv,"-dr")==0) dlmRefs=1;
           else if (strcmp(*argv,"-ow")==0) openwbem=1;
           else if (strcmp(*argv,"-h")==0) help=1;
	   else if (strcmp(*argv,"-ac")==0) assocClass=*(++argv);
 	   else if (strcmp(*argv,"-arc")==0) resultClass=*(++argv);
 	   else if (strcmp(*argv,"-ar")==0) role=*(++argv);
 	   else if (strcmp(*argv,"-arr")==0) resultRole=*(++argv);
           else if (strcmp(*argv,"-v")==0) {
              cerr<<"* wbemcli Version " PACKAGE_VERSION ", $Date: 2009/06/24 17:44:48 $" <<endl;
              goto fini;
           }
           else err=1;
         }
         else {
            if (n==0) opStr=*argv;
            else if (n==1) urlStr=strdup(*argv);
            else if (n==2) xtraStr=*argv;
            else err=1; //throw CmdException("Invalid command invocation");
            n++;
         }
      }


      if (help) {
         usage(indir,op,cmd);
         retval = 0;
	 goto fini;
      }

      if (err) {
         cerr<<cmd<<": Invalid invocation syntax"<<endl;
         usage(indir,op,cmd);
         retval = 16;
	 goto fini;
      }

      if (op==0 && opStr==NULL) throw CmdException("No operation specified");
      if (urlStr==NULL) throw CmdException("No URL specified");

      if (op==0) {
         if (strcmp(opStr,"gi")==0)		op=GetInstance;
         else if (strcmp(opStr,"ein")==0) 	op=EnumInstanceNames;
         else if (strcmp(opStr,"ei")==0) 	op=EnumInstances;

         else if (strcmp(opStr,"ci")==0) 	op=CreateInstance;
         else if (strcmp(opStr,"mi")==0) 	op=ModifyInstance;
         else if (strcmp(opStr,"di")==0) 	op=DeleteInstance;

         else if (strcmp(opStr,"gc")==0) 	op=GetClass;
         else if (strcmp(opStr,"ec")==0) 	op=EnumClasses;
         else if (strcmp(opStr,"ecn")==0) 	op=EnumClassNames;
         else if (strcmp(opStr,"dc")==0) 	op=DeleteClass;
         else if (strcmp(opStr,"gcd")==0) 	op=GetClassDef;

	 /* ---------- Associator Provider Interface ------------ */
         else if (strcmp(opStr,"ain")==0) 	op=AssociatorNames;
         else if (strcmp(opStr,"ai")==0) 	op=Associators;
         else if (strcmp(opStr,"rin")==0) 	op=ReferenceNames;
         else if (strcmp(opStr,"ri")==0) 	op=References;
	 /* -------- end of Associator Provider Interface ------- */

         else if (strcmp(opStr,"gp")==0) 	op=GetProperty;
         else if (strcmp(opStr,"sp")==0) 	op=SetProperty;
         else if (strcmp(opStr,"cm")==0) 	op=CallMethod;
         else if (strcmp(opStr,"cmx")==0) 	op=CallMethodX;

         else throw CmdException("Invalid operation");
      }

      URL url(urlStr);
      if ((op!=CreateInstance &&
           op!=GetInstance &&
           op!=GetClass &&
           op!=GetProperty &&
           op!=SetProperty &&
           op!=EnumInstances &&
           op!=Associators &&
           op!=References &&
           op!=CallMethod &&
           op!=CallMethodX &&
	   op!=ModifyInstance) && xtraStr)
         throw CmdException("Invalid command invocation 1");
      if ((op==CreateInstance ||
           op==GetProperty ||
           op==SetProperty ||
           op==CallMethod ||
           op==ModifyInstance) && !xtraStr)
         throw CmdException("Invalid command invocation 2");

      
      if (url.scheme == "https") {
	cimom.setClientCertificates(cacert,noverify,clientcert,clientkey);
      }
      switch (op) {

      case GetInstance:
         if (url.cName == "")
           throw CmdException("Class name not specified");
         if (url.keys.size() == 0)
           throw CmdException("Keys not specified");
	 if (ASSOC_PARMS)
           throw CmdException("Invalid command invocation");
         if ((inst=cimom.getInstance(url,xtraStr))) {
            url.toStringBufferGI(sb);
            if (useNl) sb += "\n";
            else sb += " ";
	    inst->propagateKeyFlags(url.keys);
            inst->toStringBuffer(sb);
            cout << sb << endl;
         }
         break;

      case EnumInstanceNames:
         if (url.cName == "")
           throw CmdException("Class name not specified");
	 if (ASSOC_PARMS)
           throw CmdException("Invalid command invocation");
         if ((ina=cimom.enumInstanceNames(url))) {
            for (int i=0,s=ina->size(); i<s; i++) {
               sb.erase();
               InstanceNameXml& n=(*ina)[i];
               url.toStringBuffer(sb,1);
               n.toStringBuffer(sb,":");
               cout << sb << endl;
            }
         }
         break;

      case EnumInstances:
         if (url.cName == "")
           throw CmdException("Class name not specified");
	 if (ASSOC_PARMS)
           throw CmdException("Invalid command invocation");
         if ((nia=cimom.enumInstances(url,xtraStr))) {
            for (int i=0,s=nia->size(); i<s; i++) {
               sb.erase();
               InstanceXml in=nia->getInst(i);
               InstanceNameXml n=nia->getName(i);
               url.toStringBuffer(sb,1);
               n.toStringBuffer(sb,":");
	       in.propagateKeyFlags(n.keys);
               if (useNl) sb += "\n";
               else sb += " ";
               in.toStringBuffer(sb);
               cout << sb << endl;
            }
         }
         break;

      case CreateInstance:
         if (url.cName == "")
           throw CmdException("Class name not specified");
         if (url.keys.size() == 0)
           throw CmdException("Keys not specified");
	 if (ASSOC_PARMS)
           throw CmdException("Invalid command invocation");
         if ((cls=cimom.getClass(url))) {
            if ((in=cimom.createInstance(url,cls,xtraStr))) {
               url.toStringBuffer(sb,1);
               in->toStringBuffer(sb,":");
               cout << sb << endl;
            }
         }
         break;

      case ModifyInstance:
         if (url.cName == "")
           throw CmdException("Class name not specified");
         if (url.keys.size() == 0)
           throw CmdException("Keys not specified");
	 if (ASSOC_PARMS)
           throw CmdException("Invalid command invocation");
         if ((inst=cimom.getInstance(url)))
            cimom.modifyInstance(url,inst,xtraStr);
         break;

      case DeleteInstance:
         if (url.cName == "")
           throw CmdException("Class name not specified");
         if (url.keys.size() == 0)
           throw CmdException("Keys not specified");
         cimom.deleteInstance(url);
         break;

      case GetClass:
         if (url.cName == "")
           throw CmdException("Class name not specified");
	 if (ASSOC_PARMS)
           throw CmdException("Invalid command invocation");
         noVal=1;
         if ((cls=cimom.getClass(url,xtraStr))) {
            url.toStringBuffer(sb,0);
            if (useNl) sb += "\n";
            else sb += " ";
            cls->toStringBuffer(sb);
            cout << sb << endl;
         }
         break;

      case GetClassDef:
         if (url.cName == "")
           throw CmdException("Class name not specified");
         if (ASSOC_PARMS)
           throw CmdException("Invalid command invocation");
         noVal=1;
         sb = cimom.getClass2str(&url, NULL, false, false);
         sb.erase(0, sb.find("<IRETURNVALUE>") + 14);
         pos = sb.find("</IRETURNVALUE>");
         sb.erase(pos, sb.size() - pos);
         cout << sb << endl;
         break;

      case EnumClassNames:
	 if (ASSOC_PARMS)
           throw CmdException("Invalid command invocation");
//         try {
         if ((cna=cimom.enumClassNames(url))) {
            for (int i=0,s=cna->size(); i<s; i++) {
               sb.erase();
               ClassNameXml *n=cna->get(i);
               url.toStringBuffer(sb,1);
               n->toStringBuffer(sb,":");
               cout << sb << endl;
            }
         }
 /*        }
         catch (...) {
            string resp = cimom.enumClassesSMC(&url);
            if (resp.size() > 0) {
               char *response = (char *) malloc(resp.size() + 1);
               memcpy(response, resp.c_str(), resp.size());
               char *line = strtok(response, "\n");
               for (  ; line != NULL ; line = strtok(NULL, "\n") ) {
                  if (strstr(line, "<CLASS ")) {
                     char *name = strstr(line, "NAME=\"");
                     if (name) {
                        name += strlen("NAME=\"");
                        if (strchr(name, '"'))  *strchr(name, '"') = '\0';
                        sb.erase();
                        url.toStringBuffer(sb,1);
                        cout << sb << ":" << name << endl;
                     }
                  }
               }
               free(response);
            }
         } */
         break;

      case EnumClasses:
	 if (ASSOC_PARMS)
           throw CmdException("Invalid command invocation");
         noVal=1;
         if ((ca=cimom.enumClasses(url))) {
            for (int i=0,s=ca->size(); i<s; i++) {
               sb.erase();
               ClassXml *n=ca->get(i);
               url.toStringBuffer(sb,2);
               if (useNl) sb = sb + ":" + n->clsName + " : " + n->superClass + "\n";
               else sb = sb + ":" + n->clsName + " ";
               n->toStringBuffer(sb,"");
               cout << sb << endl;
            }
         }
         break;

      case DeleteClass:
         if (url.cName == "")
           throw CmdException("Class name not specified");
	 if (ASSOC_PARMS)
           throw CmdException("Invalid command invocation");
         cimom.deleteClass(url);
         break;

	 /* ------------- Associator Provider Interface ---------------- */

      case AssociatorNames:
         if (url.cName == "")
           throw CmdException("Class name not specified");
	 //	 if (BAD_ASSOC_NAMES_PARMS)
	 //           throw CmdException("Invalid command invocation");
         if ((ina=cimom.associatorNames(url,assocClass,resultClass,role,resultRole))) {
            for (int i=0,s=ina->size(); i<s; i++) {
               sb.erase();
               InstanceNameXml& n=(*ina)[i];
               // url.toStringBuffer(sb,1);
               n.toStringBuffer(sb,":");
               cout << sb << endl;
            }
         }
         break;

      case Associators:
         if (url.cName == "")
           throw CmdException("Class name not specified");
         if ((owp=cimom.associators(url,assocClass,resultClass,role,resultRole,xtraStr))) {
            for (int i=0,s=owp->size(); i<s; i++) {
               sb.erase();
               InstanceXml in=owp->getInst(i);
               InstancePathXml n=owp->getName(i);
               // url.toStringBuffer(sb,1);
               n.toStringBuffer(sb,":");
	            in.propagateKeyFlags(n.name.keys);
               if (useNl) sb += "\n";
               else sb += " ";
               in.toStringBuffer(sb);
               cout << sb << endl;
            }
         }
         break;

      case ReferenceNames:
         if (url.cName == "")
           throw CmdException("Class name not specified");
	 if (BAD_REFERENCES_PARMS)
           throw CmdException("Invalid command invocation");
         if ((ina=cimom.referenceNames(url,resultClass,role))) {
            for (int i=0,s=ina->size(); i<s; i++) {
               sb.erase();
               InstanceNameXml& n=(*ina)[i];
               //url.toStringBuffer(sb,1);
               n.toStringBuffer(sb,":");
               cout << sb << endl;
            }
         }
         break;

      case References:
         if (url.cName == "")
           throw CmdException("Class name not specified");
	 if (BAD_REFERENCES_PARMS)
           throw CmdException("Invalid command invocation");
         if ((owp=cimom.references(url,resultClass,role,xtraStr))) {
            for (int i=0,s=owp->size(); i<s; i++) {
               sb.erase();
               InstanceXml in=owp->getInst(i);
               InstancePathXml n=owp->getName(i);
               //url.toStringBuffer(sb,1);
               n.toStringBuffer(sb,":");
	       in.propagateKeyFlags(n.name.keys);
               if (useNl) sb += "\n";
               else sb += " ";
               in.toStringBuffer(sb);
               cout << sb << endl;
            }
         }
         break;

	 /* ---------- end of Associator Provider Interface ------------ */

      case GetProperty:
         if (url.cName == "")
           throw CmdException("Class name not specified");
         if (url.keys.size() == 0)
           throw CmdException("Keys not specified");
	 if (ASSOC_PARMS)
           throw CmdException("Invalid command invocation");
	 try {
            if ((va=cimom.getProperty(url,xtraStr))) {
               va->toStringBuffer(sb,"");
               cout << sb << endl;
            }
	 }
	 catch (EmptyValue e) {
	    cout<<endl;
	 }
	 break;

      case SetProperty:
         if (url.cName == "")
           throw CmdException("Class name not specified");
         if (url.keys.size() == 0)
           throw CmdException("Keys not specified");
	 if (ASSOC_PARMS)
           throw CmdException("Invalid command invocation");
         if ((cls=cimom.getClass(url))) {
            cimom.setProperty(url,cls,xtraStr);
	 }
	 break;

      case CallMethodX:
         if (url.cName == "")
           throw CmdException("Class name not specified");
//         if (url.keys.size() == 0)
//           throw CmdException("Keys not specified");
         if (ASSOC_PARMS)
           throw CmdException("Invalid command invocation");
         if ((params = strchr(xtraStr, '.')))
           *(params++) = '\0';

         sb = cimom.callMethod2str(&url, xtraStr, params);
         sb.erase(0, sb.find("<SIMPLERSP>") + 11);
         pos = sb.find("</SIMPLERSP>");
         sb.erase(pos, sb.size() - pos);
         cout << sb << endl;
         break;

       case CallMethod:
         if (url.cName == "")
           throw CmdException("Class name not specified");
         if (ASSOC_PARMS)
           throw CmdException("Invalid command invocation");
         if ((params = strchr(xtraStr, '.')))
           *(params++) = '\0';

         if ((mrsp=cimom.callMethod(url,xtraStr,params))) {
            url.toStringBuffer(sb,0);
	    sb += " ";
	    mrsp->toStringBuffer(sb);
            cout << sb << endl;
         }
         break;

     default:
         throw CmdException("Command not (yet) supported");
      }
   }

   catch (NoReturnValue e) {
     //   cout<<endl;
   }
   catch (XmlException e) {
     cleanUpMsg(e.msg);
      cerr<<"*"<<endl
          <<"* "<<cmd<<": "<<e.msg<<endl
          <<"*"<<endl;
      retval = 16;
      goto fini;
   } catch(...) {
     cerr<<"*"<<endl
         <<"* what happened? " <<endl
	 <<"*"<<endl;
     retval = 16;
     goto fini;
   }

fini:
   free(urlStr);

   delete cls;
   delete inst;
   delete in;
   delete ina;
   delete nia;
   delete owp;
   delete va;
   // delete ia; // - not yet used
   delete cna;
   delete ca;
   delete mrsp;
#ifdef HAVE_LIBCURL
   // These needs to be called once
   curl_global_cleanup();
#endif
   return 0;
}
