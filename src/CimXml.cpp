/*
 * $Id: CimXml.cpp,v 1.68 2009/06/24 17:55:53 tyreld Exp $
 *
 * CimXml.cpp
 *
 * © Copyright IBM Corp. 2002, 2004, 2006, 2007, 2008
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
 *               Heidi Eckhart  <heidineu@de.ibm.com>
 *               David Eger <dteger@us.ibm.com>
 *               Tyrel Datwyler <tyreld@us.ibm.com>
 *               Chris Buccella <buccella@linux.vnet.ibm.com>
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
#include <iostream>
#include <cstdlib>
#include <cstdio>
#ifdef WIN32
#include <malloc.h>
#define strcasecmp	stricmp
#define strncasecmp	strnicmp
#endif

#if defined(HPUX) || defined(__sun)
#include <alloca.h>
#endif

using namespace std;

int DBGXB=0;
int DBGX=0;

int useNl=0;		/* Append a newline after every attribute */
int noVal=0;
int dumpXml=0;		/* Output the HTTP messages to/from CIMOM */
int noQuotes=0;
int dlmRefs=0;		/* delimit refs with { } ppairs */
int addType=0;		/* Append array ([]) and reference (&) indicators */
int openwbem=0;
int reqChunking=0;

/*-------------------------------------------------------*/
/*   XmlBuffer support                                   */
/*-------------------------------------------------------*/

XmlBuffer::XmlBuffer(string s) {
   base=cur=strdup(s.c_str());
   last=cur+strlen(cur);
}

XmlBuffer::XmlBuffer(const XmlBuffer & x) {
 throw ParseException("I told you not to!");
}

void XmlBuffer::skipWS() {
#ifndef WIN32
   if (DBGXB)
      cerr<<"--- XmlBuffer::skipWS()"<<endl;
#endif
   static int c=0;
   c++;
   while (*cur<=' ' && last>cur) cur++;
}

int XmlBuffer::getChar(const char *s) {
#ifndef WIN32
   if (DBGXB)
      cerr<<"--- XmlBuffer::getChar(\""<<s<<"\")"<<endl;
#endif
   int l=strlen(s);
   if (DBGXB) {
      char * x = (char *)malloc(81);
      strncpy(x,cur,80);
      x[80]='\0';
      fprintf(stderr,"gotChar: %s\n",x);
      free(x);
   }
   if (strncmp(cur,s,l)==0) {
      cur+=l;
      return 1;
   }
   return 0;
}

int XmlBuffer::getChar(const char c) {
#ifndef WIN32
   if (DBGXB)
      cerr<<"--- XmlBuffer::getChar('"<<c<<"'): "<<*cur<<endl;
#endif
   if (*cur++==c) return *(cur-1);
   cur--;
   return 0;
}

char *XmlBuffer::nextTag() {
#ifndef WIN32
   if (DBGXB)
      cerr<<"--- XmlBuffer::nextTag(): "<<endl;
#endif
   skipWS();
   if (*cur=='<') return cur+1;
   return NULL;
}

int XmlBuffer::nextEquals(const char *n, const char *t) {
   int l=strlen(t);
   if (n) if (strncmp(n,t,l)==0) {
      if (!isalnum(*(n+l))) {
         return 1;
      }
   }
   return 0;
}

int XmlBuffer::xmlHdr() {
   XmlElement elm(2);
   elm.attr[0] = string("version");
   elm.attr[1] = string("encoding");
   StringArray vals(2);

   XmlAttr attr;
   attr.eTagFound = 0;
   attr.attr=vals;

   skipWS();
   if (getChar("<?xml")) {
     if (attrsOk(elm,attr,"?xml")) return 1;
   }
   return 0;
}

int XmlBuffer::skipElement(const char *t) {
#ifndef WIN32
   if (DBGXB)
      cerr<<"--- XmlBuffer::skipElement(\""<<t<<"\")"<<endl;
#endif
   char eTag[256]="</";
   strcat(eTag,t);
   strcat(eTag,">");
   char *e=strstr(cur,eTag);
   if (e) {
      cur=e+strlen(eTag);
      return 1;
   }
   return 0;
}

int XmlBuffer::testAttr(const char *w, const char *val, int xCase) {
   char *end=strchr(cur,'>'),*tcur=cur,dlm,*a=NULL;
   int l=strlen(val),rv=0;

   *end=0;
   a=strstr(cur,w);
   if (a) {
     cur=a+strlen(w);
     skipWS();
     dlm=*(cur);
     if ((xCase && strncmp(cur+1,val,l)==0) || (xCase==0 && strncasecmp(cur+1,val,l)==0)) {
       if (*(cur+l+1)==dlm) rv=1;
     }
   }
   *end='>';
   cur=tcur;
   return rv;
}

int XmlBuffer::getWord(const char *w, int xCase) {
#ifndef WIN32
   if (DBGXB)
     cerr<<"--- XmlBuffer::getWord(\""<<w<<"\") ";
#endif
   int l=strlen(w);
   if ((xCase && strncmp(cur,w,l)==0) || (xCase==0 && strncasecmp(cur,w,l)==0)) {
      if (!isalnum(*(cur+l))) {
         cur+=l;
         return 1;
      }
   }
   return 0;
}

int XmlBuffer::tagEquals(const char *t) {
#ifndef WIN32
   if (DBGXB)
      cerr<<"--- XmlBuffer::tagEquals(\""<<t<<"\")"<<endl;
#endif
   char *start=cur;
   skipWS();
   if (getChar('<')) {
      skipWS();
      if (getWord(t)) return 1;
   }
   else {
   }
   cur=start;
   return 0;
}

int XmlBuffer::endTag(const char *t, const XmlAttr *a) {
#ifndef WIN32
   if (DBGXB)
      cerr<<"--- XmlBuffer::endTag(\""<<t<<"\")"<<endl;
#endif
   char *start=cur;
   if (a && a->eTagFound) return 1;
   skipWS();
   if (getChar("</")) {
      skipWS();
      if (getWord(t)) {
         skipWS();
         if (getChar('>')) return 1;
      }
   }
   cur=start;
   return 0;
}

int XmlBuffer::attrsOk(const XmlElement &e, XmlAttr &r, const char *tag) {
   unsigned int n;
   const StringArray &a = e.attr;	// input list
   StringArray &v = r.attr;	// output list
   char *ptr, wa[32];
   char msg1[]={"Unknown attribute in list for "};
   char msg2[]={"Bad attribute list for "};
   char word[11];

#ifndef WIN32
   if (DBGXB)
      cerr<<"--- XmlBuffer::attrsOk() "<<tag<<endl;
#endif

   word[0]='\0';

   for (n=0; n < a.size(); n++) wa[n]=0;

   r.eTagFound=0;
   for (skipWS(); isalpha(*cur); skipWS()) {
      for (n=0; n < a.size(); n++) {
         if (wa[n]==1) continue;
         if (getWord(a[n].c_str(),0)) {
            if (!isalnum(*cur)) {
	       skipWS();
	       if (getChar('=')) {
                  v[n]=getValue(a[n].c_str());
                  wa[n]=1;
                  goto ok;
               }
               else throw ParseException("'=' expected in attribute list");
            }
         }
      }
      strncpy(word,cur,10);
      word[10]=0;
      ptr=(char*)alloca(strlen(tag)+strlen(msg1)+8+20);
      strcpy(ptr,msg1);
      strcat(ptr,tag);
      strcat(ptr," (");
      strcat(ptr,word);
      strcat(ptr,")");
      throw ParseException(ptr);
    ok: ;
   }

   if (getChar("/>")) {
      r.eTagFound=1;
      return 1;
   }

   else if (getChar('>')) return 1;

   else if (getChar("?>") && strcmp(tag,"?xml")==0) {
      r.eTagFound=1;
      return 1;
   }

   ptr=(char*)alloca(strlen(tag)+strlen(msg2)+8);
   strcpy(ptr,msg2);
   strcat(ptr,tag);
   strcat(ptr,": ");
   strcat(ptr,word);
   throw ParseException(ptr);
}

/* Is this Broken?  I guess we don't allow escaping the quotes */
string XmlBuffer::getValue(const char *v) {
#ifndef WIN32
   if (DBGXB)
      cerr<<"--- XmlBuffer::getValue(\""<<v<<"\")"<<endl;
#endif
   skipWS();
   char dlm=0;
   char *start=NULL;
   if ((dlm=getChar('"')) || (dlm=getChar('\''))) {
      start=cur;
      while (*cur && *cur!=dlm) { cur++; }
      *cur=0;
      cur++;
      return string(start);
   }
   return NULL;
}

const char *XmlBuffer::getContent(const XmlAttr *a) {
#ifndef WIN32
   if (DBGXB)
      cerr<<"--- XmlBuffer::getContent()"<<endl;
#endif
   char *start=cur;
   if (a && a->eTagFound) return NULL;
   for (; *cur!='<' && cur<last; cur++);
   if (start==cur) return "";
   memmove(start-2,start,cur-start);
   *(cur-2)=0;
   return start-2;
}


/*-------------------------------------------------------*/
/*   AVPs support                                        */
/*-------------------------------------------------------*/
AVPs *AVPs::makeAVPs(char *s, unsigned int n)
{
   if (s==NULL) return NULL;
   AVPs *a=new AVPs(s);
   if (a->avals.size()==n) return a;
   delete a;
   return NULL;
}

AVPs::AVPs(char *s, PropertyArray &prop)
{
   char *v,*n, *u, *t;
   int i,k,j,l;
   bool name = false;
   bool addAValue = false;
   AValue av(s,"");

   /* Syntax of s:
      <name>[=<value>[,<value>,...],<name>...]
      Syntax of prop:
      contains the properties needed to correctly parse s
    */

   if (*s) {
     for (v=strchr(s,'='); *s; v=strpbrk(s,"=,")) {

       /* check if s is a <value> and add it to the current AValue */
       if (v==NULL || *v==',') {
	 if (v) *v=0;
	 /* check if s is a <name> */
	 name=false;
	 for (i=0,k=prop.size(); i<k; i++) {
	   PropertyXml &p=prop[i];
	   if (strcasecmp(p.name.c_str(),s)==0) {
	     name=true;
	     break;
	   }
	 }
	 if (!name) {
	   av.add(s);
	   s=v+1;
	 }
	 if (v) continue;
       }

       if(addAValue) {
	 avals.add(av);
	 addAValue=false;
       }
       
       /* get <name> */
       if(v) {
 	 *v=0;
	 v++;
	 n=strchr(v,',');
	 /* check if property is a reference */
	 for (i=0,k=prop.size(); i<k; i++) {
	   PropertyXml &p=prop[i];
	   if (strcasecmp(p.name.c_str(),s)==0) {
	     if(p.pType==TypePropertyRef) {
	       name=false;
	       if(n) {
		 for (u=n+1; ; n=strchr(u,',')) {
		   if(n==NULL) break;
		   t=strchr(u,'=');
		   for (j=0,l=prop.size(); j<l; j++) {
		     PropertyXml &p=prop[j];
		     if (strncasecmp(p.name.c_str(),u,
				     strlen(u)-strlen(t))==0) {
		       name=true;
		       n=u-1;
		       break;
		     }
		   }
		   if(name) break;
		   if(n) u=n+1;
		 }
	       }
	     }
	     break;
	   }
	 }
	 if(n) *n=0;
	 av=AValue(s,v);
 	 addAValue=true;
	 if(n) s=n+1;
       }
       /* end of property list */
       else break;
     }
   }
   all=(char*)malloc(avals.size());
   memset(all,0,avals.size());
}

AVPs::AVPs(char *s, ParameterArray &param)
{
   char *n, *t, *u, *v, *w;
   int i,k,j,l;
   bool name = false;
   bool addAValue = false;
   bool isRef = false;
   bool isArray = false;
   AValue av(s,"");

   /* Syntax of s:
      <name>[=<value>[,<value>,...],<name>...]
      Syntax of param:
      contains the parameters needed to correctly parse s
    */

   if (*s) {
     for (v=strchr(s,'='); *s; v=strpbrk(s,"=,")) {

       /* seems to be an array - check if s is a <value>
	  and add it to the current AValue
       */
       if (v==NULL || *v==',') {
	 if (v) *v=0;
	 /* check if s is a <name> */
	 name=false;
	 for (i=0,k=param.size(); i<k; i++) {
	   ParameterXml &p=param[i];
	   if (strcasecmp(p.name.c_str(),s)==0) {
	     name=true;
	     break;
	   }
	 }
	 if (!name) {
	   av.add(s);
	   s=v+1;
	 }
	 if(v) continue;
       }

       if(addAValue) {
	 avals.add(av);
	 addAValue=false;
       }
       
       /* get <name> */
       if(v) {
	 *v=0;
	 v++;
	 n=strchr(v,',');

	 /* check if parameter is a reference/reference array */
	 for (i=0,k=param.size(); i<k; i++) {
	   ParameterXml &p=param[i];
	   if (strcasecmp(p.name.c_str(),s)==0) {
	     if(p.pType==TypeParameterRefArray ||
		p.pType==TypeParameterRef) {

	       isRef=true;
	       if(p.pType==TypeParameterRefArray) isArray=true;
	       
	       name=false;
	       if(n) {
		 for (u=n+1; ; n=strchr(u,',')) {
		   if(n==NULL) break;
		   t=strchr(u,'=');
		   for (j=0,l=param.size(); j<l; j++) {
		     ParameterXml &p=param[j];
		     if (strncasecmp(p.name.c_str(),u,
				     strlen(u)-strlen(t))==0) {
		       name=true;
		       n=u-1;
		       break;
		     }
		   }
		   if(name) {
		     if(n) *n=0;
		     break;
		   }
		   if(n) u=n+1;
		 }
	       }
	       parseRefList(s,v,isArray);
	       if(n) s=n+1;
	     }
	     break;
	   }
	 }
	 if(!isRef) {
	   if(n) *n=0;
	   av=AValue(s,v);
	   addAValue=true;
	   if(n) s=n+1;
	 }
	 isRef=false;
       }
       /* end of parameter list */
       else break;
     }
   }
   all=(char*)malloc(avals.size());
   memset(all,0,avals.size());
}

void AVPs::parseRefList(char *name, char *s, bool isArray)
{ 
  AValue av(name,"");
  char *u, *v, *n;
  bool addValue=false;

  if(!s) return;

  if(!isArray)
    av=AValue(name,s);
  else {
    v=strchr(s,'.');
    u=v+1;
    for(v=strchr(u,'.');;v=strchr(u,'.')) {
      if(!v) {
	if(!addValue) av=AValue(name,s);
	else av.add(s);
	break;
      }
      for(n=v; *n!=','; n--) {}
      *n=0;
      if(!addValue) {
	av=AValue(name,s);
	addValue=true;
      }
      else av.add(s);
      s=n+1;
      u=v+1;
    }
  }
  avals.add(av);
}

AVPs::AVPs(char *s)
{
   char *v,*n;
   int i=0;
   AValue av(s,"");

   if (*s) {
     for (i=0,v=strchr(s,'='); *s; v=strpbrk(s,"=,"),i++) {
      if (v==NULL || *v==',') {
         if (v) *v=0;
         av.add(s);
         s=v+1;
         if (v) continue;
	 break;
      }
      n=strchr(v,',');
      *v=0;
      if (n) {
         *n=0;
         avals.add(av=AValue(s,v+1));
         s=n+1;
      }
      else {
         avals.add(av=AValue(s,v+1));
         break;
      }
     }
   }
   all=(char*)malloc(avals.size());
   memset(all,0,avals.size());
}

AValue* AVPs::get(const char *a)
{
  for (int i=0,s=avals.size(); i<s; i++) {
      if (strcasecmp(avals[i].attr.c_str(),a)==0) {
	all[i]=1;
	return &avals[i];
      }
	}
   return NULL;
}

AValue* AVPs::get(unsigned int n)
{
   if (n<avals.size()) return &avals[n];
   return NULL;
}

int AVPs::check(const char *a)
{
   for (int i=0,s=avals.size(); i<s; i++)
      if (strcasecmp(avals[i].attr.c_str(),a)==0) return 1;
   return 0;
}

Option<string> AVPs::notGotten()
{
  bool exists=false;
   string sb;
   for (int i=0,s=avals.size(); i<s; i++) {
      if (all[i]==0) {
          if (!exists) {exists = true;}
          else {sb +=", ";}
          sb += avals[i].attr;
      }
   }
   return exists ? Option<string>(sb) : Option<string>();
}

AList::AList(char *s)
{
   int i=-1;
   char *ss=s,*v;
   attrs=0;

   if (*s) {
      for (i=0,v=strchr(s,','); *s; v=strpbrk(s,","),i++) {
          s=v+1;
          if (v) continue;
          break;
       }
       attrs=i+1;
       attrv=(char**)malloc(sizeof(char*)*i+1);
       s=attrl=strdup(ss);
       for (i=0,v=strchr(s,','); *s; v=strpbrk(s,","),i++) {
          attrv[i]=s;
          if (v) *v=0;
          s=v+1;
          if (v) continue;
          break;
       }
    }
}

AList::~AList()
{
   if (attrs) {
      free(attrl);
      free(attrv);
   }
}

/*-------------------------------------------------------*/
/*   CimXml support                                      */
/*-------------------------------------------------------*/

XmlElement CimXml::elm;
bool CimXml::initialized(false);

CimXml::CimXml(string x, int noIRET) : xb(0), msg(0)
{
  if(!initialized) {
    elm.attr = StringArray(2);
    elm.attr[0] = string("CIMVERSION"); 
    elm.attr[1] = string("DTDVERSION");
    initialized = true;
  }
#ifndef WIN32
   if (DBGX)
      cerr<<"--- CimXml::CimXml("<<x<<")"<<endl;
#endif
   XmlAttr attr;
   attr.eTagFound = 0;
   attr.attr=StringArray(3);
   xb=new XmlBuffer(x);

   char *tag;
   if (xb->xmlHdr()==0) {
      // <?xml version="1.0" encoding="utf-8" ?>
      throw ParseException("Expecting ?xml element");
   }

   if (xb->tagEquals("CIM")) {
     if (xb->attrsOk(elm,attr,"CIM")) {
        cimVersion=attr.attr[0];
	dtdVersion=attr.attr[1];
        tag=xb->nextTag();
        if (xb->nextEquals(tag,"MESSAGE")) {
           msg=new MessageXml(*xb, noIRET);
           if (xb->endTag("CIM")) return;
           else throw ParseException("Expecting </CIM> tag");
        }
        else throw ParseException("Expecting MESSAGE element");
     }
     else throw ParseException("Bad CIM attributes");
   }
   else throw ParseException("Expecting CIM element");
}



/*-------------------------------------------------------*/
/*   MessageXml support                                  */
/*-------------------------------------------------------*/


XmlElement MessageXml::elm(2);
bool MessageXml::initialized(false);

MessageXml::MessageXml(XmlBuffer &xb, int noIRET) : srq(0)
{
  if(!initialized){
    elm.attr = StringArray(2);
    elm.attr[0] = "ID";
    elm.attr[1] = "PROTOCOLVERSION";
    initialized = true;
  }
#ifndef WIN32
   if (DBGX)
      cerr<<"--- MessageXml::MessageXml("<<xb.cur<<")"<<endl;
#endif
   XmlAttr attr;
   attr.eTagFound = 0;
   attr.attr=StringArray(3);
   char *tag;
   if (xb.tagEquals("MESSAGE")) {
     if (xb.attrsOk(elm,attr,"MESSAGE")) {
        id=attr.attr[0];
        protocol=attr.attr[1];
        tag=xb.nextTag();
        if (xb.nextEquals(tag,"SIMPLERSP")) {
           srq=new SimplerspXml(xb, noIRET);
           if (xb.endTag("MESSAGE")) return;
           else throw ParseException("Expecting </MESSAGE> tag");
        }
        else throw ParseException("Expecting SIMPLERSP element");
     }
     else throw ParseException("Bad MESSAGE attributes");
   }
   else throw ParseException("Expecting MESSAGE element");
}

MessageXml::~MessageXml() { delete srq; }


/*-------------------------------------------------------*/
/*   SimplerspXml support                                */
/*-------------------------------------------------------*/

XmlElement SimplerspXml::elm(0);

SimplerspXml::SimplerspXml(XmlBuffer &xb, int noIRET) : irsp(0)
{
#ifndef WIN32
   if (DBGX)
      cerr<<"--- SimplerspXml::SimplerspXml("<<xb.cur<<")"<<endl;
#endif
   XmlAttr attr;
   attr.eTagFound = 0;
   attr.attr=StringArray(1);
   char *tag;
   if (xb.tagEquals("SIMPLERSP")) {
     if (xb.attrsOk(elm,attr,"SIMPLERSP")) {
        tag=xb.nextTag();
        if (xb.nextEquals(tag,"IMETHODRESPONSE")) {
           if (openwbem)
              irsp=new IMethodresponseXml(xb);
           else
              irsp=new IMethodresponseXml(xb, noIRET);

           if (xb.endTag("SIMPLERSP")) return;
           else throw ParseException("Expecting </SIMPLERSP> tag");
        }
        else if (xb.nextEquals(tag,"METHODRESPONSE")) {
           if (openwbem)
              irsp=new MethodresponseXml(xb);
           else
              irsp=new MethodresponseXml(xb, noIRET);

           if (xb.endTag("SIMPLERSP")) return;
           else throw ParseException("Expecting </SIMPLERSP> tag");
        }
        else throw ParseException("Expecting IMETHODRESPONSE or METHODRESPONSE element");
     }
     else throw ParseException("Bad SIMPLERSP attributes");
   }
   else throw ParseException("Expecting SIMPLERSP element");
}

SimplerspXml::~SimplerspXml() { delete irsp; }


/*-------------------------------------------------------*/
/*   IMethodresponseXml support                          */
/*-------------------------------------------------------*/

XmlElement IMethodresponseXml::elm(1,string("NAME"));

IMethodresponseXml::IMethodresponseXml(XmlBuffer &xb, int noIRET) : irv(0), err(0)
{
#ifndef WIN32
   if (DBGX)
      cerr<<"--- IMethodresponseXml::IMethodresponseXml("<<xb.cur<<")"<<endl;
#endif
   XmlAttr attr;
   attr.eTagFound = 0;
   attr.attr=StringArray(2);
   char *tag;

   err=NULL;
   if (xb.tagEquals("IMETHODRESPONSE")) {
     if (xb.attrsOk(elm,attr,"IMETHODRESPONSE")) {
        name=attr.attr[0];
        tag=xb.nextTag();
        if (xb.nextEquals(tag,"IRETURNVALUE")) {
           irv=new IReturnvalueXml(xb);
	   if (irv->getReturnData() == NULL) {
	       /* OpenWBEM support */
	       throw NoReturnValue();
	       delete irv;
	       irv=NULL;
	   }
           if (xb.endTag("IMETHODRESPONSE")) return;
           else throw ParseException("Expecting </IMETHODRESPONSE> tag(2)");
        }
        else if (xb.nextEquals(tag,"ERROR")) {
           err=new ErrorXml(xb);
           if (xb.endTag("IMETHODRESPONSE")) return;
           else throw ParseException("Expecting </IMETHODRESPONSE> tag(3)");
        }
/*	else if (noIRET) {
        // BMMU
           if (xb.endTag("IMETHODRESPONSE")) {
              irv=NULL;
              return;
           }
           else throw ParseException("Expecting </IMETHODRESPONSE> tag(1)");
	}
*/	else if (xb.endTag("IMETHODRESPONSE")) {
           throw NoReturnValue();
           irv=NULL;
           return;
        }
/*        else throw ParseException("Expecting </IMETHODRESPONSE> tag(1)");
	}
        else throw ParseException("Expecting IRETURNVALUE or ERROR element");
*/       else throw
           ParseException("Expecting /IMETHODRESPONSE, IRETURNVALUE or ERROR element");
    }
     else throw ParseException("Bad IMETHODRESPONSE attributes");
   }
   else throw ParseException("Expecting IMETHODRESPONSE element");
}

/*-------------------------------------------------------*/
/*   MethodresponseXml support                          */
/*-------------------------------------------------------*/

XmlElement MethodresponseXml::elm(1,string("NAME"));

MethodresponseXml::MethodresponseXml(XmlBuffer &xb, int noRET) : 
  rvl(0), pvll(0), err(0)
{
#ifndef WIN32
   if (DBGX)
      cerr<<"--- MethodresponseXml::MethodresponseXml("<<xb.cur<<")"<<endl;
#endif
   XmlAttr attr;
   attr.eTagFound = 0;
   attr.attr=StringArray(2);
   char *tag;
   type = TypeMethodResponse;

   err=NULL;
   if (xb.tagEquals("METHODRESPONSE")) {
     if (xb.attrsOk(elm,attr,"METHODRESPONSE")) {
       // step 1: return value
        name=attr.attr[0];
        tag=xb.nextTag();
        if (xb.nextEquals(tag,"RETURNVALUE")) {
           rvl=new ReturnvalueXml(xb);
        }
        else if (xb.nextEquals(tag,"ERROR")) {
           err=new ErrorXml(xb);
           if (xb.endTag("METHODRESPONSE")) return;
           else throw ParseException("Expecting </METHODRESPONSE> tag(3)");
        }
/*	else if (noRET) {
        // BMMU
           if (xb.endTag("METHODRESPONSE")) {
              rvl=NULL;
              return;
           }
           else throw ParseException("Expecting </METHODRESPONSE> tag(1)");
	}
*/	else if (xb.endTag("METHODRESPONSE")) {
           throw NoReturnValue();
           rvl=NULL;
           return;
        }
/*        else throw ParseException("Expecting </METHODRESPONSE> tag(1)");
	}
        else throw ParseException("Expecting RETURNVALUE or ERROR element");
*/       else throw
           ParseException("Expecting /METHODRESPONSE, RETURNVALUE or ERROR element");
	// step 2: paramvalues
	tag=xb.nextTag();
	pvll=new ParamvalueArray();
        while (xb.nextEquals(tag,"PARAMVALUE")) {
	  ParamvalueXml v(xb);
	  pvll->add(v);
	  tag=xb.nextTag();
	} 
	if (xb.endTag("METHODRESPONSE")) 
	  return;
	throw ParseException("Expecting </METHODRESPONSE> tag(2)");
     }
     else throw ParseException("Bad METHODRESPONSE attributes");
   }
   else throw ParseException("Expecting METHODRESPONSE element");
}

MethodresponseXml::MethodresponseXml(const MethodresponseXml& resp)
{
  type=resp.type;
  name=resp.name;
  rvl = resp.rvl ? resp.rvl->clone() : 0;
  pvll = resp.pvll ? resp.pvll->clone() : 0;
  err = resp.err ? new ErrorXml(*resp.err) : 0;
}  

void MethodresponseXml::toStringBuffer(string &sb, const char *q)
{  
  if (rvl && rvl->getReturnData()) {
    sb += name;
    sb += ": ";
    rvl->getReturnData()->toStringBuffer(sb,q);
  }
  if (useNl) sb += "\n";
  else sb += " ";
  if (pvll) {
    pvll->toStringBuffer(sb,q);
  }
}


/*-------------------------------------------------------*/
/*   IReturnvalueXml support                             */
/*-------------------------------------------------------*/

XmlElement IReturnvalueXml::elm(0);

IReturnvalueXml::IReturnvalueXml(XmlBuffer &xb) : rv(0)
{
#ifndef WIN32
   if (DBGX)
     cerr<<"--- IReturnvalueXml::IReturnvalueXml("<<xb.cur<<")"<<endl;
#endif
   XmlAttr attr;
   attr.eTagFound = 0;
   attr.attr=StringArray(1);
   RetValXml * rv2 = NULL;
   rv=NULL;
   char *tag;
   if (xb.tagEquals("IRETURNVALUE")) {
     if (xb.attrsOk(elm,attr,"IRETURNVALUE")) {
        tag=xb.nextTag();
        if (xb.nextEquals(tag,"INSTANCE")) {
           rv=new InstanceXml(xb);
           tag=xb.nextTag();
           if (xb.nextEquals(tag,"/IRETURNVALUE")) {
              if (xb.endTag("IRETURNVALUE")) return;
              throw ParseException("Expecting </IRETURNVALUE> tag");
           }
           if (xb.nextEquals(tag,"INSTANCE")) {
	     rv2=new InstancesXml(*(InstanceXml*)rv);delete(rv);rv=rv2;
              for (; xb.nextEquals(tag,"INSTANCE"); tag=xb.nextTag()) {
                 ((InstancesXml*)rv)->add(InstanceXml(xb));
              }
           }
           if (xb.endTag("IRETURNVALUE")) return;
           throw ParseException("Expecting </IRETURNVALUE> tag");
        }
        else if (xb.nextEquals(tag,"CLASS")) {
           rv=new ClassXml(xb);
           tag=xb.nextTag();
           if (xb.nextEquals(tag,"/IRETURNVALUE")) {
              if (xb.endTag("IRETURNVALUE")) return;
              throw ParseException("Expecting </IRETURNVALUE> tag");
           }
           if (xb.nextEquals(tag,"CLASS")) {
              rv2=new ClassesXml((ClassXml*)rv);delete(rv);rv=rv2;
              for (; xb.nextEquals(tag,"CLASS"); tag=xb.nextTag()) {
                 ((ClassesXml*)rv)->add(ClassXml(xb));
              }
           }
           if (xb.endTag("IRETURNVALUE")) return;
           throw ParseException("Expecting </IRETURNVALUE> tag");
        }
        else if (xb.nextEquals(tag,"INSTANCENAME")) {
           rv=new InstanceNameXml(xb);
           tag=xb.nextTag();
           if (xb.nextEquals(tag,"/IRETURNVALUE")) {
              if (xb.endTag("IRETURNVALUE")) return;
              throw ParseException("Expecting </IRETURNVALUE> tag");
           }
           if (xb.nextEquals(tag,"INSTANCENAME")) {
              rv2=new InstanceNamesXml(*(InstanceNameXml*)rv);delete(rv);rv=rv2;
              for (; xb.nextEquals(tag,"INSTANCENAME"); tag=xb.nextTag()) {
                 ((InstanceNamesXml*)rv)->add(InstanceNameXml(xb));
              }
           }
           if (xb.endTag("IRETURNVALUE")) return;
           throw ParseException("Expecting </IRETURNVALUE> tag");
        }
        else if (xb.nextEquals(tag,"OBJECTPATH")) {
           rv=new InstanceNameXml(xb,1);
           tag=xb.nextTag();
           if (xb.nextEquals(tag,"/IRETURNVALUE")) {
              if (xb.endTag("IRETURNVALUE")) return;
              throw ParseException("Expecting </IRETURNVALUE> tag");
           }
           if (xb.nextEquals(tag,"OBJECTPATH")) {
	      rv2=new InstanceNamesXml(*(InstanceNameXml*)rv);delete(rv);rv=rv2;
              for (; xb.nextEquals(tag,"OBJECTPATH"); tag=xb.nextTag()) {
		 InstanceNameXml foo(xb,1);
                 ((InstanceNamesXml*)rv)->add(foo);
              }
           }
           if (xb.endTag("IRETURNVALUE")) return;
           throw ParseException("Expecting </IRETURNVALUE> tag");
        }
        else if (xb.nextEquals(tag,"VALUE.NAMEDINSTANCE")) {
           rv=new NamedInstanceXml(xb);
           tag=xb.nextTag();
           if (xb.nextEquals(tag,"/IRETURNVALUE")) {
              if (xb.endTag("IRETURNVALUE")) return;
              throw ParseException("Expecting </IRETURNVALUE> tag");
           }
           if (xb.nextEquals(tag,"VALUE.NAMEDINSTANCE")) {
              rv2=new NamedInstancesXml((NamedInstanceXml*)rv);delete(rv);rv=rv2;
              for (; xb.nextEquals(tag,"VALUE.NAMEDINSTANCE"); tag=xb.nextTag()) {
                 NamedInstanceXml foo(xb);
                 ((NamedInstancesXml*)rv)->add(foo);
              }
           }
           if (xb.endTag("IRETURNVALUE")) return;
           throw ParseException("Expecting </IRETURNVALUE> tag");
        }
        else if (xb.nextEquals(tag,"VALUE.OBJECTWITHPATH")) {
           rv=new ObjectWithPathXml(xb);
           tag=xb.nextTag();
           if (xb.nextEquals(tag,"/IRETURNVALUE")) {
              if (xb.endTag("IRETURNVALUE")) return;
              throw ParseException("Expecting </IRETURNVALUE> tag");
           }
           if (xb.nextEquals(tag,"VALUE.OBJECTWITHPATH")) {
              rv2=new ObjectsWithPathXml(*(ObjectWithPathXml*)rv);delete(rv);rv=rv2;
              for (; xb.nextEquals(tag,"VALUE.OBJECTWITHPATH"); tag=xb.nextTag()) {
		 ObjectWithPathXml foo(xb);
                 ((ObjectsWithPathXml*)rv)->add(foo);
              }
           }
           if (xb.endTag("IRETURNVALUE")) return;
           throw ParseException("Expecting </IRETURNVALUE> tag");
        }
        else if (xb.nextEquals(tag,"CLASSNAME")) {
           rv=new ClassNameXml(xb);
           tag=xb.nextTag();
           if (xb.nextEquals(tag,"/IRETURNVALUE")) {
              if (xb.endTag("IRETURNVALUE")) return;
              throw ParseException("Expecting </IRETURNVALUE> tag");
           }
           if (xb.nextEquals(tag,"CLASSNAME")) {
              rv2=new ClassNamesXml((ClassNameXml*)rv);delete(rv);rv=rv2;
              for (; xb.nextEquals(tag,"CLASSNAME"); tag=xb.nextTag()) {
                 ((ClassNamesXml*)rv)->add(ClassNameXml(xb));
              }
           }
           if (xb.endTag("IRETURNVALUE")) return;
           throw ParseException("Expecting </IRETURNVALUE> tag");
        }
        else if (xb.nextEquals(tag,"VALUE") || xb.nextEquals(tag,"VALUE.ARRAY")) {
           rv=new ValueXml(xb);
           tag=xb.nextTag();
           if (xb.endTag("IRETURNVALUE")) return;
           throw ParseException("Expecting </IRETURNVALUE> tag");
        }
        else if (xb.endTag("IRETURNVALUE")) return;
        throw ParseException("Expecting INSTANCE, CLASS or ... element");
     }
     else throw ParseException("Bad IRETURNVALUE attributes");
   }
   else throw ParseException("Expecting IRETURNVALUE element");
}

IReturnvalueXml::IReturnvalueXml(const IReturnvalueXml &src) : rv(0)
    { rv = src.rv->clone(); }

IReturnvalueXml& IReturnvalueXml::operator= (const IReturnvalueXml &src) 
    { if(this != &src) {rv = (src.rv) ? src.rv->clone() : 0;} return *this; }

/*-------------------------------------------------------*/
/*   ReturnvalueXml support                             */
/*-------------------------------------------------------*/

XmlElement ReturnvalueXml::elm(1,"PARAMTYPE");

ReturnvalueXml::ReturnvalueXml(XmlBuffer &xb) : rv(0)
{
#ifndef WIN32
   if (DBGX)
     cerr<<"--- ReturnvalueXml::ReturnvalueXml("<<xb.cur<<")"<<endl;
#endif
   XmlAttr attr;
   attr.eTagFound = 0;
   attr.attr=StringArray(1);
   RetValXml * rv2 = NULL;
   rv=NULL;
   char *tag;
   if (xb.tagEquals("RETURNVALUE")) {
     if (xb.attrsOk(elm,attr,"RETURNVALUE")) {
        tag=xb.nextTag();
        if (xb.nextEquals(tag,"INSTANCE")) {
           rv=new InstanceXml(xb);
           tag=xb.nextTag();
           if (xb.nextEquals(tag,"/RETURNVALUE")) {
              if (xb.endTag("RETURNVALUE")) return;
              throw ParseException("Expecting </RETURNVALUE> tag");
           }
           if (xb.nextEquals(tag,"INSTANCE")) {
	     rv2=new InstancesXml(*(InstanceXml*)rv);delete(rv);rv=rv2;
              for (; xb.nextEquals(tag,"INSTANCE"); tag=xb.nextTag()) {
                 ((InstancesXml*)rv)->add(InstanceXml(xb));
              }
           }
           if (xb.endTag("RETURNVALUE")) return;
           throw ParseException("Expecting </RETURNVALUE> tag");
        }
        else if (xb.nextEquals(tag,"CLASS")) {
           rv=new ClassXml(xb);
           tag=xb.nextTag();
           if (xb.nextEquals(tag,"/RETURNVALUE")) {
              if (xb.endTag("RETURNVALUE")) return;
              throw ParseException("Expecting </RETURNVALUE> tag");
           }
           if (xb.nextEquals(tag,"CLASS")) {
              rv2=new ClassesXml((ClassXml*)rv);delete(rv);rv=rv2;
              for (; xb.nextEquals(tag,"CLASS"); tag=xb.nextTag()) {
                 ((ClassesXml*)rv)->add(ClassXml(xb));
              }
           }
           if (xb.endTag("RETURNVALUE")) return;
           throw ParseException("Expecting </RETURNVALUE> tag");
        }
        else if (xb.nextEquals(tag,"INSTANCENAME")) {
           rv=new InstanceNameXml(xb);
           tag=xb.nextTag();
           if (xb.nextEquals(tag,"/RETURNVALUE")) {
              if (xb.endTag("RETURNVALUE")) return;
              throw ParseException("Expecting </RETURNVALUE> tag");
           }
           if (xb.nextEquals(tag,"INSTANCENAME")) {
              rv2=new InstanceNamesXml(*(InstanceNameXml*)rv);delete(rv);rv=rv2;
              for (; xb.nextEquals(tag,"INSTANCENAME"); tag=xb.nextTag()) {
                 ((InstanceNamesXml*)rv)->add(InstanceNameXml(xb));
              }
           }
           if (xb.endTag("RETURNVALUE")) return;
           throw ParseException("Expecting </RETURNVALUE> tag");
        }
        else if (xb.nextEquals(tag,"OBJECTPATH")) {
           rv=new InstanceNameXml(xb,1);
           tag=xb.nextTag();
           if (xb.nextEquals(tag,"/RETURNVALUE")) {
              if (xb.endTag("RETURNVALUE")) return;
              throw ParseException("Expecting </RETURNVALUE> tag");
           }
           if (xb.nextEquals(tag,"OBJECTPATH")) {
	      rv2=new InstanceNamesXml(*(InstanceNameXml*)rv);delete(rv);rv=rv2;
              for (; xb.nextEquals(tag,"OBJECTPATH"); tag=xb.nextTag()) {
		 InstanceNameXml foo(xb,1);
                 ((InstanceNamesXml*)rv)->add(foo);
              }
           }
           if (xb.endTag("RETURNVALUE")) return;
           throw ParseException("Expecting </RETURNVALUE> tag");
        }
        else if (xb.nextEquals(tag,"VALUE.NAMEDINSTANCE")) {
           rv=new NamedInstanceXml(xb);
           tag=xb.nextTag();
           if (xb.nextEquals(tag,"/RETURNVALUE")) {
              if (xb.endTag("RETURNVALUE")) return;
              throw ParseException("Expecting </RETURNVALUE> tag");
           }
           if (xb.nextEquals(tag,"VALUE.NAMEDINSTANCE")) {
              rv2=new NamedInstancesXml((NamedInstanceXml*)rv);delete(rv);rv=rv2;
              for (; xb.nextEquals(tag,"VALUE.NAMEDINSTANCE"); tag=xb.nextTag()) {
                 NamedInstanceXml foo(xb);
                 ((NamedInstancesXml*)rv)->add(foo);
              }
           }
           if (xb.endTag("RETURNVALUE")) return;
           throw ParseException("Expecting </RETURNVALUE> tag");
        }
        else if (xb.nextEquals(tag,"VALUE.OBJECTWITHPATH")) {
           rv=new ObjectWithPathXml(xb);
           tag=xb.nextTag();
           if (xb.nextEquals(tag,"/RETURNVALUE")) {
              if (xb.endTag("RETURNVALUE")) return;
              throw ParseException("Expecting </RETURNVALUE> tag");
           }
           if (xb.nextEquals(tag,"VALUE.OBJECTWITHPATH")) {
              rv2=new ObjectsWithPathXml(*(ObjectWithPathXml*)rv);delete(rv);rv=rv2;
              for (; xb.nextEquals(tag,"VALUE.OBJECTWITHPATH"); tag=xb.nextTag()) {
		 ObjectWithPathXml foo(xb);
                 ((ObjectsWithPathXml*)rv)->add(foo);
              }
           }
           if (xb.endTag("RETURNVALUE")) return;
           throw ParseException("Expecting </RETURNVALUE> tag");
        }
        else if (xb.nextEquals(tag,"CLASSNAME")) {
           rv=new ClassNameXml(xb);
           tag=xb.nextTag();
           if (xb.nextEquals(tag,"/RETURNVALUE")) {
              if (xb.endTag("RETURNVALUE")) return;
              throw ParseException("Expecting </RETURNVALUE> tag");
           }
           if (xb.nextEquals(tag,"CLASSNAME")) {
              rv2=new ClassNamesXml((ClassNameXml*)rv);delete(rv);rv=rv2;
              for (; xb.nextEquals(tag,"CLASSNAME"); tag=xb.nextTag()) {
                 ((ClassNamesXml*)rv)->add(ClassNameXml(xb));
              }
           }
           if (xb.endTag("RETURNVALUE")) return;
           throw ParseException("Expecting </RETURNVALUE> tag");
        }
        else if (xb.nextEquals(tag,"VALUE") || xb.nextEquals(tag,"VALUE.ARRAY")) {
           rv=new ValueXml(xb);
           tag=xb.nextTag();
           if (xb.endTag("RETURNVALUE")) return;
           throw ParseException("Expecting </RETURNVALUE> tag");
        }
        else if (xb.endTag("RETURNVALUE")) return;
        throw ParseException("Expecting INSTANCE, CLASS or ... element");
     }
     else throw ParseException("Bad RETURNVALUE attributes");
   }
   else throw ParseException("Expecting RETURNVALUE element");
}

ReturnvalueXml::ReturnvalueXml(const ReturnvalueXml &src) : rv(0)
    { rv = src.rv->clone(); }

ReturnvalueXml& ReturnvalueXml::operator= (const ReturnvalueXml &src) 
    { if(this != &src) {rv = (src.rv) ? src.rv->clone() : 0;} return *this; }

/*-------------------------------------------------------*/
/*   ObjectWithPathXml support                            */
/*-------------------------------------------------------*/

XmlElement ObjectWithPathXml::elm(1,string("CLASSNAME"));

ObjectWithPathXml::ObjectWithPathXml(XmlBuffer &xb) : ip(0) { init(xb); }

void ObjectWithPathXml::init(XmlBuffer &xb)
{
#ifndef WIN32
   if (DBGX)
      cerr<<"--- ObjectWithPathXml::ObjectWithPathXml("<<xb.cur<<")"<<endl;
#endif
   char *tag;
   XmlAttr attr;
   attr.eTagFound = 0;
   attr.attr=StringArray(1);
   type=TypeObjectWithPath;
   if (xb.tagEquals("VALUE.OBJECTWITHPATH")) {
     if (xb.attrsOk(elm,attr,"VALUE.OBJECTWITHPATH")) {
        tag=xb.nextTag();
        if (xb.nextEquals(tag,"INSTANCEPATH")) {
           ip=new InstancePathXml(xb);
           tag=xb.nextTag();
           if (xb.nextEquals(tag,"INSTANCE")) {
              inst=InstanceXml(xb);
              if (xb.endTag("VALUE.OBJECTWITHPATH")) return;
              else throw ParseException("Expecting /VALUE.OBJECTWITHPATH element");
           }
        }
        else if (xb.nextEquals(tag,"LOCALINSTANCEPATH")) {
           ip=new LocalInstancePathXml(xb);
           tag=xb.nextTag();
           if (xb.nextEquals(tag,"INSTANCE")) {
              inst=InstanceXml(xb);
              if (xb.endTag("VALUE.OBJECTWITHPATH")) return;
              else throw ParseException("Expecting /VALUE.OBJECTWITHPATH element");
           }
        }
        else {
           char msg[256]={"Expecting INSTANCEPATH and INSTANCE element instead of: "};
	   strncat(msg,tag,32);
	   throw ParseException(msg);
	}
     }
     else throw ParseException("Bad VALUE.OBJECTWITHPATH attributes");
   }
   else throw ParseException("Expecting VALUE.OBJECTWITHPATH element");
}

/*-------------------------------------------------------*/
/*   ObjectsWithPathXml support                            */
/*-------------------------------------------------------*/


ObjectsWithPathXml::ObjectsWithPathXml(ObjectWithPathXml& n)
{
   type=TypeObjectsWithPath;
   ia.add(n.inst);
   ipa.add(*(n.ip));
}

void ObjectsWithPathXml::add(ObjectWithPathXml& n)
{
   ia.add(n.inst);
   ipa.add(*(n.ip));
}

/*-------------------------------------------------------*/
/*   NamedInstanceXml support                            */
/*-------------------------------------------------------*/

XmlElement NamedInstanceXml::elm(1,string("CLASSNAME"));

NamedInstanceXml::NamedInstanceXml(XmlBuffer &xb) { init(xb); }

void NamedInstanceXml::init(XmlBuffer &xb)
{
#ifndef WIN32
   if (DBGX)
      cerr<<"--- NamedInstanceXml::NamedInstanceXml("<<xb.cur<<")"<<endl;
#endif
   char *tag;
   XmlAttr attr;
   attr.eTagFound = 0;
   attr.attr=StringArray(1);
   type=TypeNamedInstance;
   if (xb.tagEquals("VALUE.NAMEDINSTANCE")) {
     if (xb.attrsOk(elm,attr,"VALUE.NAMEDINSTANCE")) {
        tag=xb.nextTag();
        if (xb.nextEquals(tag,"INSTANCENAME")) {
           instname=InstanceNameXml(xb);
           tag=xb.nextTag();
           if (xb.nextEquals(tag,"INSTANCE")) {
              inst=InstanceXml(xb);
              if (xb.endTag("VALUE.NAMEDINSTANCE")) return;
              else throw ParseException("Expecting /VALUE.NAMEDINSTANCE element");
           }
        }
        else throw ParseException("Expecting INSTANCENANE and INSTANCE element");
     }
     else throw ParseException("Bad VALUE.NAMEDINSTANCE attributes");
   }
   else throw ParseException("Expecting VALUE.NAMEDINSTANCE element");
}

/*-------------------------------------------------------*/
/*   NamedInstancesXml support                            */
/*-------------------------------------------------------*/


NamedInstancesXml::NamedInstancesXml(NamedInstanceXml *n)
{
   type=TypeNamedInstances;
   ia.add(n->inst);
   ina.add(n->instname);
}

void NamedInstancesXml::add(NamedInstanceXml& n)
{
   ia.add(n.inst);
   ina.add(n.instname);
}



/*-------------------------------------------------------*/
/*   InstanceNameXml support                                 */
/*-------------------------------------------------------*/

XmlElement InstanceNameXml::elm(1,string("CLASSNAME"));

InstanceNameXml::InstanceNameXml(XmlBuffer &xb, char op) { init(xb,op); }

void InstanceNameXml::init(XmlBuffer &xb, char op)
{
#ifndef WIN32
   if (DBGX)
      cerr<<"--- InstanceNameXml::InstanceNameXml("<<xb.cur<<")"<<endl;
#endif
   char *tag;
   XmlAttr attr, oAttr;
   attr.eTagFound = 0;
   attr.attr=StringArray(2);
   oAttr.eTagFound = 0;
   oAttr.attr=StringArray(1);
   XmlElement objectPathElm(0);
   type=TypeInstanceName;

   if (op) {
      if (xb.tagEquals("OBJECTPATH")) {
         if (xb.attrsOk(objectPathElm,oAttr,"OBJECTPATH")) {
            tag=xb.nextTag();
            if (xb.nextEquals(tag,"LOCALINSTANCEPATH")) {
               LocalInstancePathXml lns=LocalInstancePathXml(xb);
	       *this=lns.name;
               if (xb.endTag("OBJECTPATH")) return;
               else throw ParseException("Expecting /OBJECTPATH element");
            }
            if (xb.nextEquals(tag,"INSTANCEPATH")) {
               InstancePathXml ins=InstancePathXml(xb);
	       *this=ins.name;
          nspace = ins.nspace;
               if (xb.endTag("OBJECTPATH")) return;
               else throw ParseException("Expecting /OBJECTPATH element");
            }
            else throw ParseException("Expecting LOCALINSTANCEPATH or INSTANCEPATH element (1)");
         }
         else throw ParseException("Bad OBJECTPATH attributes");
      }
      else throw ParseException("Expecting OBJECTPATH element");
   }

   if (xb.tagEquals("INSTANCENAME")) {
      if (xb.attrsOk(elm,attr,"INSTANCENAME")) {
        clsName=string(attr.attr[0]);
         tag=xb.nextTag();
         if (xb.nextEquals(tag,"KEYBINDING")) {
            for (; xb.nextEquals(tag,"KEYBINDING"); tag=xb.nextTag())
              keys.add(KeyBindingXml(xb));
            if (xb.endTag("INSTANCENAME")) return;
           else throw ParseException("Expecting /INSTANCENAME element");
         }
	 else if (xb.endTag("INSTANCENAME")) {
          keys.add(KeyBindingXml("*KEYBINDING","MISSING*"));
	    return;
         }
     //   else throw ParseException("Expecting KEYBINDING element");
      }
     else throw ParseException("Bad INSTANCENAME attributes");
   }
   else throw ParseException("Expecting INSTANCENAME element");
}

void InstanceNameXml::toStringBuffer(string &sb, const char *q)
{
   sb = sb + nspace + q + clsName + ".";
//   noQuotes=1;
   for (int i=0,s=keys.size(); i<s; i++) {
      keys[i].toStringBuffer(sb);
      if (i<s-1) sb+=",";
   }
   noQuotes=0;
}


/*-------------------------------------------------------*/
/*   InstanceXml support                                 */
/*-------------------------------------------------------*/

XmlElement InstanceXml::elm(1,string("CLASSNAME"));

InstanceXml::InstanceXml(XmlBuffer &xb) { init(xb); }

void InstanceXml::init(XmlBuffer &xb)
{
#ifndef WIN32
   if (DBGX)
      cerr<<"--- InstanceXml::InstanceXml("<<xb.cur<<")"<<endl;
#endif
   XmlAttr attr, oAttr;
   attr.eTagFound = 0;
   attr.attr=StringArray(2);
   char *tag;
   type=TypeInstance;
   if (xb.tagEquals("INSTANCE")) {
     if (xb.attrsOk(elm,attr,"INSTANCE")) {
        clsName=string(attr.attr[0]);
        for (;;) {
           tag=xb.nextTag();
           if (xb.nextEquals(tag,"QUALIFIER")) {
	      QualifierXml foo = QualifierXml(xb);
              addQualifier(foo);
           }
           else if (xb.nextEquals(tag,"PROPERTY")) {
	      PropertyXml foo = PropertyXml(xb);
              addProperty(foo);
           }
           else if (xb.nextEquals(tag,"/INSTANCE")) {
              if (xb.endTag("INSTANCE")) return;
           }
           else throw ParseException("Expecting /INSTANCE, QUALIFIER or PROPERTY element");
        }
     }
     else throw ParseException("Bad INSTANCE attributes");
   }
   else throw ParseException("Expecting INSTANCE element");
}

void InstanceXml::toStringBuffer(string &sb, const char*q)
{
   if (useNl) properties.toStringBuffer(sb,"",q);
   else properties.toStringBuffer(sb,",",q);
}

void InstanceXml::propagateKeyFlags(PropertyArray& k)
{
   for (int i=0,m=k.size(); i<m; i++) {
      for (int ii=0,mm=properties.size(); ii<mm; ii++)
         if (strcasecmp(k[i].name.c_str(),properties[ii].name.c_str())==0)
	    properties[ii].key=1;
   }
}

void InstanceXml::propagateKeyFlags(KeyBindingArray& k)
{
   for (int i=0,m=k.size(); i<m; i++) {
      for (int ii=0,mm=properties.size(); ii<mm; ii++)
         if (strcasecmp(k[i].name.c_str(),properties[ii].name.c_str())==0)
	    properties[ii].key=1;
   }
}

/*-------------------------------------------------------*/
/*   ClassNameXml support                                */
/*-------------------------------------------------------*/

XmlElement ClassNameXml::elm(1,string("NAME"));

ClassNameXml::ClassNameXml(XmlBuffer &xb)
{ init(xb); }

void ClassNameXml::init(XmlBuffer &xb)
{
#ifndef WIN32
   if (DBGX)
     cerr<<"--- "<<this<<" ClassNameXml::ClassNameXml("<<xb.cur<<")"<<endl;
#endif
   XmlAttr attr, oAttr;
   attr.eTagFound = 0;
   attr.attr=StringArray(2);
   type=TypeClassName;
   if (xb.tagEquals("CLASSNAME")) {
      if (xb.attrsOk(elm,attr,"CLASSNAME")) {
         name=string(attr.attr[0]);
         if (xb.endTag("CLASSNAME",&attr)) return;
         throw ParseException("Expecting </CLASSNAME> tag");
      }
      else throw ParseException("Bad CLASSNAME attributes");
   }
   else throw ParseException("Expecting CLASSNAME element");
}

void ClassNameXml::toStringBuffer(string &sb, const char *q)
{
  sb += string(q)+name;
}

/*-------------------------------------------------------*/
/*   ClassXml support                                    */
/*-------------------------------------------------------*/

XmlElement ClassXml::elm(2);
bool ClassXml::initialized(false);

ClassXml::ClassXml(XmlBuffer &xb) {init(xb);}

void ClassXml::init(XmlBuffer &xb)
{
  if(!initialized) {
    elm.attr[0] = string("NAME");
    elm.attr[1] = string("SUPERCLASS");
    initialized = true;
  }
#ifndef WIN32
   if (DBGX)
      cerr<<"--- ClassXml::ClassXml("<<xb.cur<<")"<<endl;
#endif
   XmlAttr attr, oAttr;
   attr.eTagFound = 0;
   attr.attr=StringArray(2);
   char *tag;
   type=TypeClass;
   if (xb.tagEquals("CLASS")) {
     if (xb.attrsOk(elm,attr,"CLASS")) {
        clsName=string(attr.attr[0]);
        superClass=string(attr.attr[1]);
       for (;;) {
           tag=xb.nextTag();
           if (xb.nextEquals(tag,"QUALIFIER")) {
	      QualifierXml foo = QualifierXml(xb);
              addQualifier(foo);
           }
           else if (xb.nextEquals(tag,"PROPERTY")) {
	      PropertyXml foo = PropertyXml(xb);
              addProperty(foo);
           }
           else if (xb.nextEquals(tag,"METHOD")) {
	      MethodXml foo = MethodXml(xb);
              addMethod(foo);
           }
           else if (xb.nextEquals(tag,"/CLASS")) {
              if (xb.endTag("CLASS")) return;
           }
           else throw ParseException("Expecting /CLASS, QUALIFIER, PROPERTY or METHOD element");
        }
     }
     else throw ParseException("Bad CLASS attributes");
   }
   else throw ParseException("Expecting CLASS element");
}

void ClassXml::toStringBuffer(string &sb,const char *q)
{
   if (useNl) properties.toStringBuffer(sb,"",q);
   else properties.toStringBuffer(sb,",",q);
   /*
   if (useNl) methods.toStringBuffer(sb,"",q);
   else methods.toStringBuffer(sb,",",q);
   */
}

int ClassXml::keysOk(URL &url)
{
   int i,s,ii,ss;
   for (i=0,s=keys.size(); i<s; i++) {
      for (ii=0,ss=url.keys.size(); ii<ss; ii++) {
         if (strcasecmp(keys[i].name.c_str(),url.keys[ii].name.c_str())==0) goto ok;
      }
      return 0;
     ok: ;
   }
   return 1;
}

/*-------------------------------------------------------*/
/*   ParamvalueXml support                               */
/*-------------------------------------------------------*/
XmlElement ParamvalueXml::elm;
bool ParamvalueXml::initialized(false);

void ParamvalueXml::initialize() {
  elm.attr = StringArray(2);
  elm.attr[0] = string("NAME");
  elm.attr[1] = string("PARAMTYPE");
  initialized=true;
}

//remove if not necessary

ParamvalueXml::ParamvalueXml() : val(0)
{
}

ParamvalueXml::ParamvalueXml(const ParamvalueXml& paramval)
{
  name=paramval.name;
  type=paramval.type;
  val=paramval.val->clone();
}

ParamvalueXml::ParamvalueXml(XmlBuffer& xb) : val(0)
{
#ifndef WIN32
   if (DBGX)
      cerr<<"--- ParamvalueXml::ParamvalueXml("<<xb.cur<<")"<<endl;
#endif
   if (!initialized) initialize();
   XmlAttr attr;
   attr.eTagFound = 0;
   attr.attr=StringArray(2);
   char *tag;

   if (xb.tagEquals("PARAMVALUE")) {
     if (xb.attrsOk(elm,attr,"PARAMVALUE")) {
        name=attr.attr[0];
        type=attr.attr[1];
        tag=xb.nextTag();
        if (xb.nextEquals(tag,"VALUE")) {
	  val = new ValueXml(xb);
	  tag=xb.nextTag();
	  if (xb.endTag("PARAMVALUE")) return;
	  throw ParseException("Expecting </PARAMVALUE> tag");
	}
     } else {
       throw ParseException("Bad PARAMVALUE attributes");
     }
   } else {
       throw ParseException("Expecting PARAMVALUE tag");
   }
}

void ParamvalueXml::toStringBuffer(string &sb, const char *q)
{
  sb += name;
  sb += ":";
  sb += q;
  if (val) val->toStringBuffer(sb,q);
}     


/*-------------------------------------------------------*/
/*   PropertyXml support                                 */
/*-------------------------------------------------------*/

XmlElement PropertyXml::elm;
XmlElement PropertyXml::elmRef;
XmlElement PropertyXml::elmArray;
bool PropertyXml::initialized(false);

void PropertyXml::initialize() {
  elm.attr = StringArray(5);
  elm.attr[0] = string("NAME");
  elm.attr[1] = string("TYPE");
  elm.attr[2] = string("CLASSORIGIN");
  elm.attr[3] = string("PROPAGATED");
  elm.attr[4] = string("EMBEDDEDOBJECT");
  elmRef.attr = StringArray(4);
  elmRef.attr[0] = string("NAME");
  elmRef.attr[1] = string("REFERENCECLASS");
  elmRef.attr[2] = string("CLASSORIGIN");
  elmRef.attr[3] = string("PROPAGATED");
  elmArray.attr = StringArray(5);
  elmArray.attr[0] = string("NAME");
  elmArray.attr[1] = string("TYPE");
  elmArray.attr[2] = string("CLASSORIGIN");
  elmArray.attr[3] = string("PROPAGATED");
  elmArray.attr[4] = string("ARRAYSIZE");
  initialized = true;
}

PropertyXml::PropertyXml(XmlBuffer &xb) : eprop(-1), lprop(-1)
{
  init(xb);
}

void PropertyXml::init(XmlBuffer &xb) {
  if(!initialized) { initialize();}
#ifndef WIN32
   if (DBGX)
      cerr<<"--- PropertyXml::PropertyXml("<<xb.cur<<")"<<endl;
#endif
   XmlAttr attr;
   attr.eTagFound = 0;
   attr.attr=StringArray(6);
   char *tag,*start;
   key=0;

   start=xb.cur;
   if (xb.tagEquals("PROPERTY.REFERENCE")) {
     tag=xb.cur;
     pType=TypePropertyRef;
     xb.skipElement("PROPERTY.REFERENCE");
     str=string(start,xb.cur-start);
   }
   else if (xb.tagEquals("PROPERTY.ARRAY")) {
     tag=xb.cur;
     pType=TypePropertyArray;
     xb.skipElement("PROPERTY.ARRAY");
     str=string(start,xb.cur-start);
   }
   else if (xb.tagEquals("PROPERTY")) {
     tag=xb.cur;
     pType=TypeProperty;
     xb.skipElement("PROPERTY");
     str=string(start,xb.cur-start);
   }
   else throw ParseException("Expecting PROPERTY element xx");
   xb.cur=tag;

   if ((pType==TypeProperty && xb.attrsOk(elm,attr,"PROPERTY")) ||
       (pType==TypePropertyArray && xb.attrsOk(elmArray,attr,"PROPERTY.ARRAY")) ||
       (pType==TypePropertyRef && xb.attrsOk(elmRef,attr,"PROPERTY.REFERENCE"))) {

      name      = string(attr.attr[0]);
      refCls    = type = string(attr.attr[1]);
      origin    = string(attr.attr[2]);
      propagated= string(attr.attr[3]);
      arraySize = string(attr.attr[4]);

      for (;;) {
         tag=xb.nextTag();
         if (xb.nextEquals(tag,"QUALIFIER")) {
	    if (eprop==-1) { eprop = xb.cur-start; }
            QualifierXml foo=QualifierXml(xb);
            addQualifier(foo);
            if (!key) key= foo.key;
         }
         else if (xb.nextEquals(tag,"VALUE")) {
	    if (eprop==-1) { eprop = xb.cur-start; }
            value=Option<ValueXml>(ValueXml(xb));
         }
         else if (xb.nextEquals(tag,"/PROPERTY")) {
	    if (eprop==-1) { eprop = xb.cur-start; }
            lprop = xb.cur-start; //cerr<<" ### lprop: "<<lprop<<endl;
            if (xb.endTag("PROPERTY") ||
               xb.endTag("PROPERTY.ARRAY") ||
               xb.endTag("PROPERTY.REFERENCE")) {
               return;
            }
         }
         else throw ParseException("Expecting /PROPERTY, QUALIFIER or VALUE element");
      }
   }
   else throw ParseException("Bad PROPERTY attributes");
}


PropertyXml::PropertyXml(char *nvp, int ref)  : eprop(-1), lprop(-1)
{
  init(nvp, ref);
}

void PropertyXml::init(char *nvp, int ref){
  if(!initialized) { initialize();}
#ifndef WIN32
   if (DBGX)
      cerr<<"--- PropertyXml::PropertyXml("<<nvp<<","<<ref<<")"<<endl;
#endif

   char *n=nvp,*p=strchr(nvp,'=');

   if (ref) pType=TypePropertyRef;
   else pType=0;
   if (!p) throw ParseException("Invalid name-value pair");
   name=string(n,p-n);
   p++;
   value=Option<ValueXml>(ValueXml(p));
   if (strcasecmp(p,"true")==0 || strcasecmp(p,"false")==0) {
     type="boolean";
   } else if (*p=='+' || * p=='-' || isdigit(*p)) {
     type="numeric";   
     int i=1;
     while (p[i]) {
       if (!isdigit(p[i++])) {
	 type="string";
	 break;
       }
     }
   } else {
     type="string";
   }
}

void PropertyXml::toStringBuffer(string &sb, const char *q)
{
   if (useNl) sb += "-";
   sb = sb + name + q;
   if (addType && pType==TypePropertyArray) sb += "[]";
   if (addType && pType==TypePropertyRef) sb += "&";
   if (addType && key) sb += "#";

   sb += "=";
   if (!noVal && value.exists()) {
      // sb += "=";
      if (!noQuotes &&
	  type != "" &&
	  strcasecmp(type.c_str(),"string")==0)
         value.val().toStringBuffer(sb,"\"");
      else value.val().toStringBuffer(sb,"");
   }
   if (useNl) sb += "\n";
}



void PropertyXml::toXmlBuffer(string &xb, AValue *av, int vOnly, URL *url)
{
   switch (pType) {
   case TypeProperty:
      if (vOnly==0) xb += "\n<PROPERTY NAME=\""+name+"\" TYPE=\""+type+"\">";
      xb += "<VALUE>";
      if (av) {
         if (av->val[0]=='"') {
	    xb.append(av->val,1,av->val.length()-2);
	 } 
         else xb += av->val;
      }	 
      xb += "</VALUE>";
      if (vOnly==0) xb += "</PROPERTY>";
      break;
   case TypePropertyArray:
      if (vOnly==0) xb += "\n<PROPERTY.ARRAY NAME=\""+name+"\" TYPE=\""+type+"\">";
      if (av && av->val != "") { av->vals.push_back(string(av->val)); av->val=""; }
      xb += "<VALUE.ARRAY>";
      if (av && av->vals.size()) for (int i=0,s=av->vals.size(); i<s; i++) {
         xb += "<VALUE>";
         if (av->vals[i][0]=='"') {
	    xb.append(av->vals[i],1,av->vals[i].length()-2);
	 } 
         else xb += av->vals[i];
         xb += "</VALUE>";
      }
      else xb += "<VALUE></VALUE>";
      xb += "</VALUE.ARRAY>";
      if (vOnly==0) xb += "</PROPERTY.ARRAY>";
      break;
   case TypePropertyRef:
      if (vOnly==0) xb += "\n<PROPERTY.REFERENCE NAME=\""+name+"\">";
      xb += "<VALUE.REFERENCE>";
      if (av) {
	 int i,s;
	 string urlstr;
	 const char *p,*q;

	 // create full wbemcli URL
	 p=strchr(av->val.c_str(),'/');
	 if(p) q=strchr(p,':');
	 if(!p || !q) {
	   // neither hostname nor namespace are defined
	   // same hostname and namespace as in url
	   urlstr += url->scheme + "://" + url->host + ":";
	   for (i=0,s=url->ns.size(); i<s; i++)
	     urlstr += "/" + url->ns[i];
	   urlstr = urlstr + ":" + av->val.c_str();
	 }
	 else urlstr += url->scheme + "://" + av->val.c_str();
	 URL refurl = URL(urlstr.c_str());

	 xb += "<INSTANCEPATH>";
	 xb += "<NAMESPACEPATH>";
	 xb += "<HOST>" + refurl.host + "</HOST>";
	 xb += "<LOCALNAMESPACEPATH>"; 
	 for (i=0,s=refurl.ns.size(); i<s; i++)
	   xb = xb + "<NAMESPACE NAME=\"" + refurl.ns[i] + "\"></NAMESPACE>";
	 xb += "</LOCALNAMESPACEPATH>";
	 xb += "</NAMESPACEPATH>";
	 xb += "<INSTANCENAME CLASSNAME=\"" + refurl.cName + "\">";
	 refurl.toStringBuffer(xb);
	 xb += "</INSTANCENAME>";
	 xb += "</INSTANCEPATH>";
      }
      xb += "</VALUE.REFERENCE>";
      if (vOnly==0) xb += "</PROPERTY.REFERENCE>";
      break;
   }
}



/*-------------------------------------------------------*/
/*   ParameterXml support                                */
/*-------------------------------------------------------*/

XmlElement ParameterXml::elm;
XmlElement ParameterXml::elmRef;
XmlElement ParameterXml::elmArray;
XmlElement ParameterXml::elmRefArray;
bool ParameterXml::initialized(false);

void ParameterXml::initialize() {
  elm.attr = StringArray(2);
  elm.attr[0] = string("NAME");
  elm.attr[1] = string("TYPE");
  elmRef.attr = StringArray(2);
  elmRef.attr[0] = string("NAME");
  elmRef.attr[1] = string("REFERENCECLASS");
  elmArray.attr = StringArray(3);
  elmArray.attr[0] = string("NAME");
  elmArray.attr[1] = string("TYPE");
  elmArray.attr[2] = string("ARRAYSIZE");
  elmRefArray.attr = StringArray(3);
  elmRefArray.attr[0] = string("NAME");
  elmRefArray.attr[1] = string("REFERENCECLASS");
  elmRefArray.attr[2] = string("ARRAYSIZE");
  initialized = true;
}

ParameterXml::ParameterXml(XmlBuffer &xb) : eprop(-1), lprop(-1)
{
  init(xb);
}

void ParameterXml::init(XmlBuffer &xb) {
  if(!initialized) { initialize();}
#ifndef WIN32
   if (DBGX)
      cerr<<"--- ParameterXml::ParameterXml("<<xb.cur<<")"<<endl;
#endif
   XmlAttr attr;
   attr.eTagFound = 0;
   attr.attr=StringArray(3);
   char *tag,*start;
   key=0;

   start=xb.cur;
   if (xb.tagEquals("PARAMETER.REFARRAY")) {
     tag=xb.cur;
     pType=TypeParameterRefArray;
     xb.skipElement("PARAMETER.REFARRAY");
     str=string(start,xb.cur-start);
   }
   else if (xb.tagEquals("PARAMETER.REFERENCE")) {
     tag=xb.cur;
     pType=TypeParameterRef;
     xb.skipElement("PARAMETER.REFERENCE");
     str=string(start,xb.cur-start);
   }
   else if (xb.tagEquals("PARAMETER.ARRAY")) {
     tag=xb.cur;
     pType=TypeParameterArray;
     xb.skipElement("PARAMETER.ARRAY");
     str=string(start,xb.cur-start);
   }
   else if (xb.tagEquals("PARAMETER")) {
     tag=xb.cur;
     pType=TypeParameter;
     xb.skipElement("PARAMETER");
     str=string(start,xb.cur-start);
   }
   else throw ParseException("Expecting PARAMETER element xx");
   xb.cur=tag;

   if ((pType==TypeParameter && xb.attrsOk(elm,attr,"PARAMETER")) ||
       (pType==TypeParameterArray && xb.attrsOk(elmArray,attr,"PARAMETER.ARRAY")) ||
       (pType==TypeParameterRef && xb.attrsOk(elmRef,attr,"PARAMETER.REFERENCE")) ||
       (pType==TypeParameterRefArray && xb.attrsOk(elmRefArray,attr,"PARAMETER.REFARRAY"))) {

      name      = string(attr.attr[0]);
      refCls    = type = string(attr.attr[1]);
      arraySize = string(attr.attr[2]);

      for (;;) {
         tag=xb.nextTag();
         if (xb.nextEquals(tag,"QUALIFIER")) {
	    if (eprop==-1) { eprop = xb.cur-start; }
            QualifierXml foo=QualifierXml(xb);
            addQualifier(foo);
         }
         else if (xb.nextEquals(tag,"/PARAMETER")) {
	    if (eprop==-1) { eprop = xb.cur-start; }
            lprop = xb.cur-start; //cerr<<" ### lprop: "<<lprop<<endl;
            if (xb.endTag("PARAMETER") ||
               xb.endTag("PARAMETER.ARRAY") ||
               xb.endTag("PARAMETER.REFERENCE") ||
               xb.endTag("PARAMETER.REFARRAY")) {
               return;
            }
         }
         else throw ParseException("Expecting /PARAMETER or QUALIFIER element");
      }
   }
   else throw ParseException("Bad PARAMETER attributes");
}

void ParameterXml::toStringBuffer(string &sb, const char *q)
{
   if (useNl) sb += "-";
   sb = sb + name + q;
   if (addType && pType==TypeParameterArray) sb += "[]";
   if (addType && pType==TypeParameterRef) sb += "&";
   if (addType && pType==TypeParameterRefArray) sb += "#";

   if (!noVal && value.exists()) {
      sb += "=";
      if (!noQuotes &&
	  type != "" &&
	  strcasecmp(type.c_str(),"string")==0)
         value.val().toStringBuffer(sb,"\"");
      else value.val().toStringBuffer(sb,"");
   }
   if (useNl) sb += "\n";
}

void ParameterXml::toXmlBuffer(string &xb, AValue *av, int vOnly, URL *url)
{
   switch (pType) {
   case TypeParameter:
      if (vOnly==0) xb += "\n<PARAMETER NAME=\""+name+"\" TYPE=\""+type+"\">";
      xb += "<VALUE>";
      if (av) {
         if (av->val[0]=='"') {
	    xb.append(av->val,1,av->val.length()-2);
	 } 
         else xb += av->val;
      }	 
      xb += "</VALUE>";
      if (vOnly==0) xb += "</PARAMETER>";
      break;
   case TypeParameterArray:
      if (vOnly==0) xb += "\n<PARAMETER.ARRAY NAME=\""+name+"\" TYPE=\""+type+"\">";
      if (av && av->val != "") { av->vals.push_back(string(av->val)); av->val=""; }
      xb += "<VALUE.ARRAY>";
      if (av && av->vals.size()) {
	for (int i=0,s=av->vals.size(); i<s; i++) {
	  xb += "<VALUE>";
	  if (av->vals[i][0]=='"') {
	    xb.append(av->vals[i],1,av->vals[i].length()-2);
	  } 
	  else xb += av->vals[i];
	  xb += "</VALUE>";
	}
      }
      else xb += "<VALUE></VALUE>";
      xb += "</VALUE.ARRAY>";
      if (vOnly==0) xb += "</PARAMETER.ARRAY>";
      break;
   case TypeParameterRef:
      if (vOnly==0) xb += "\n<PARAMETER.REFERENCE NAME=\""+name+"\">";
      if (av) {
	refToXmlBuffer(xb, av->val.c_str(), url);
      }
      else xb += "<VALUE.REFERENCE></VALUE.REFERENCE>";
      if (vOnly==0) xb += "</PARAMETER.REFERENCE>";
      break;
   case TypeParameterRefArray:
      if (vOnly==0) xb += "\n<PARAMETER.REFARRAY NAME=\""+name+"\" TYPE=\""+type+"\">";
      if (av && av->val != "") { av->vals.push_back(string(av->val)); av->val=""; }
      xb += "<VALUE.REFARRAY>";
      if (av && av->vals.size()) {
	for (int i=0,s=av->vals.size(); i<s; i++) {
	  refToXmlBuffer(xb, av->vals[i].c_str(), url);
	}
      }
      else xb += "<VALUE.REFERENCE></VALUE.REFERENCE>";
      xb += "</VALUE.REFARRAY>";
      if (vOnly==0) xb += "</PARAMETER.REFARRAY>";
      break;
   }
}

void ParameterXml::refToXmlBuffer(string &xb, const char *s , URL *url)
{
  int i, k;
  string urlstr;
  const char *p, *q;
  
  xb += "<VALUE.REFERENCE>";

  // create full wbemcli URL
  p=strchr(s,'/');
  if(p) q=strchr(p,':');
  if(!p || !q) {
    // neither hostname nor namespace are defined
    // same hostname and namespace as in url
    urlstr += url->scheme + "://" + url->host + ":";
    for (i=0,k=url->ns.size(); i<k; i++)
      urlstr += "/" + url->ns[i];
    urlstr = urlstr + ":" + s;
  }
  else urlstr += url->scheme + "://" + s;
  URL refurl = URL(urlstr.c_str());
	  
  xb += "<INSTANCEPATH>";
  xb += "<NAMESPACEPATH>";
  xb += "<HOST>" + refurl.host + "</HOST>";
  xb += "<LOCALNAMESPACEPATH>"; 
  for (i=0,k=refurl.ns.size(); i<k; i++)
    xb = xb + "<NAMESPACE NAME=\"" + refurl.ns[i] + "\"></NAMESPACE>";
  xb += "</LOCALNAMESPACEPATH>";
  xb += "</NAMESPACEPATH>";
  xb += "<INSTANCENAME CLASSNAME=\"" + refurl.cName + "\">";
  refurl.toStringBuffer(xb);
  xb += "</INSTANCENAME>";
  xb += "</INSTANCEPATH>";
  
  xb += "</VALUE.REFERENCE>";
}



/*-------------------------------------------------------*/
/*   QualifierXml support                                */
/*-------------------------------------------------------*/

QualifierXml::QualifierXml(XmlBuffer &xb)
{
   char * start, * end;
#ifndef WIN32
   if (DBGX)
      cerr<<"--- QualifierXml::QualifierXml("<<xb.cur<<")"<<endl;
#endif
   start=xb.cur;
   if (xb.tagEquals("QUALIFIER")) {
      key=xb.testAttr("NAME=","key",0);
      xb.skipElement("QUALIFIER");
      end=xb.cur;
      str = string(start, end-start);
   }
   else throw ParseException("Expecting QUALIFIER element");
}



/*-------------------------------------------------------*/
/*   MethodXml support                                   */
/*-------------------------------------------------------*/

XmlElement MethodXml::elm(4);
bool MethodXml::initialized(false);

MethodXml::MethodXml(XmlBuffer &xb) {init(xb);}

void MethodXml::init(XmlBuffer &xb)
{
  if(!initialized) {
    elm.attr[0] = string("NAME");
    elm.attr[1] = string("TYPE");
    elm.attr[2] = string("CLASSORIGIN");
    elm.attr[3] = string("PROPAGATED");
    initialized = true;
  }
#ifndef WIN32
   if (DBGX)
      cerr<<"--- MethodXml::MethodXml("<<xb.cur<<")"<<endl;
#endif
   XmlAttr attr;
   attr.attr=StringArray(4);
   char *tag;

   if (xb.tagEquals("METHOD")) {
     if (xb.attrsOk(elm,attr,"METHOD")) {
       name=string(attr.attr[0]);
       for (;;) {
	 tag=xb.nextTag();
	 if (xb.nextEquals(tag,"QUALIFIER")) {
	   QualifierXml foo = QualifierXml(xb);
	   addQualifier(foo);
	 }
	 else if (xb.nextEquals(tag,"PARAMETER")) {
	   ParameterXml foo = ParameterXml(xb);
	   addParameter(foo);
	 }
	 else if (xb.nextEquals(tag,"/METHOD")) {
	   if (xb.endTag("METHOD")) return;
	 }
	 else throw ParseException("Expecting /METHOD, QUALIFIER or PARAMETER element");
       }
     }
     else throw ParseException("Bad METHOD attributes");
   }
   else throw ParseException("Expecting METHOD element");
}

/*
void MethodXml::toStringBuffer(string &sb, char *q)
{
   if (useNl) sb += "-";
   sb = sb + str + q;
   if (useNl) sb += "\n";
}
*/


/*-------------------------------------------------------*/
/*   KeyBindingXml support                               */
/*-------------------------------------------------------*/

XmlElement KeyBindingXml::elm(1,string("NAME"));

KeyBindingXml::KeyBindingXml(XmlBuffer &xb) { init(xb); }

void KeyBindingXml::init(XmlBuffer &xb)
{
#ifndef WIN32
   if (DBGX)
      cerr<<"--- KeyBindingXml::KeyBindingXml("<<xb.cur<<")"<<endl;
#endif
   char *tag;
   XmlAttr attr;
   attr.eTagFound = 0;
   attr.attr=StringArray(1);
   if (xb.tagEquals("KEYBINDING")) {
      if (xb.attrsOk(elm,attr,"KEYBINDING")) {
         name=attr.attr[0];
         tag=xb.nextTag();
         if (xb.nextEquals(tag,"KEYVALUE")) {
            which = KEYVAL;
            keyVal=KeyValueXml(xb);
            if (xb.endTag("KEYBINDING")) return;
            else throw ParseException("Expecting /KEYBINDING element");
         }
         else if (xb.nextEquals(tag,"VALUE.REFERENCE")) {
            which=VALREF;
            valRef=ValueXml(xb);
            if (xb.endTag("KEYBINDING")) return;
            else throw ParseException("Expecting /KEYBINDING element");
         }
         else throw ParseException("Expecting KEYVALUE or VALUE.REFERENCE element");
      }
      else throw ParseException("Bad KEYBINDING attributes");
   }
   else throw ParseException("Expecting KEYBINDING element");
}

KeyBindingXml::KeyBindingXml(const char *n, const char *v)
{
      which  = KEYVAL;
      name   = string(n);
      keyVal = KeyValueXml(v);
}


void KeyBindingXml::toStringBuffer(string &sb, const char* s)
{
   sb += name;
   if (!noVal) {
      sb += "=";
      if(which==KEYVAL) {
	if (keyVal.valType != "" && strncasecmp(keyVal.valType.c_str(),"string",6)==0)
	  keyVal.toStringBuffer(sb,"\"");
	else keyVal.toStringBuffer(sb,"");
      } else if (which==VALREF) {
         valRef.toStringBuffer(sb,"");
      }
   }
}

/*-------------------------------------------------------*/
/*   KeyValueXml support                                 */
/*-------------------------------------------------------*/

XmlElement KeyValueXml::elm(1,string("VALUETYPE"));

KeyValueXml::KeyValueXml(XmlBuffer &xb)
{
#ifndef WIN32
   if (DBGX)
      cerr<<"--- KeyValueXml::KeyValueXml("<<xb.cur<<")"<<endl;
#endif
   XmlAttr attr;
   attr.eTagFound = 0;
   attr.attr=StringArray(2);
   if (xb.tagEquals("KEYVALUE")) {
      if (xb.attrsOk(elm,attr,"KEYVALUE")) {
         valType=string(attr.attr[0]);
         val=string(xb.getContent(&attr));
         if (xb.endTag("KEYVALUE",&attr)) return;
         throw ParseException("Expecting </KEYVALUE> tag");
      }
      else throw ParseException("Bad KEYVALUE attributes");
   }
   else throw ParseException("Expecting KEYVALUE element");
}

KeyValueXml::KeyValueXml(const char *v)
{
   valType="string";
   val=v;
}

void KeyValueXml::toStringBuffer(string &sb, const char *q)
{
   string s;
   s=Cimom::unescapeXml(val.c_str(),1); //bug??
   if (noQuotes) sb += s;
   else sb = sb + q+s+q;
}



/*-------------------------------------------------------*/
/*   NameSpaceXml support                                */
/*-------------------------------------------------------*/

XmlElement NameSpaceXml::elm(1,"NAME");

NameSpaceXml::NameSpaceXml(XmlBuffer &xb)
{
#ifndef WIN32
   if (DBGX)
      cerr<<"--- NameSpaceXml::NameSpaceXml()"<<endl;
#endif
   XmlAttr attr;
   attr.eTagFound = 0;
   attr.attr=StringArray(2);

   if (xb.tagEquals("NAMESPACE")) {
      if (xb.attrsOk(elm,attr,"NAMESPACE")) {
         name=string(attr.attr[0]);
         if (xb.endTag("NAMESPACE",&attr)) return;
         throw ParseException("Expecting </NAMESPACE> tag");
      }
      else throw ParseException("Bad NAMESPACE attributes");
   }
   else throw ParseException("Expecting NAMESPACE element");
}

void NameSpaceXml::toStringBuffer(string &sb, const char *q)
{
  throw CmdException("Internal error. never print NameSpaceXml.");
}



/*-------------------------------------------------------*/
/*   LocalNameSpacePathXml support                       */
/*-------------------------------------------------------*/

XmlElement LocalNameSpacePathXml::elm(0);

LocalNameSpacePathXml::LocalNameSpacePathXml(XmlBuffer &xb)
{
#ifndef WIN32
   if (DBGX)
      cerr<<"--- LocalNameSpacePathXml::LocalNameSpacePathXml()"<<endl;
#endif
   char *tag;
   XmlAttr attr;
   attr.eTagFound = 0;
   attr.attr=StringArray(2);

   if (xb.tagEquals("LOCALNAMESPACEPATH")) {
      if (xb.attrsOk(elm,attr,"LOCALNAMESPACEPATH")) {
         for (;;) {
            tag=xb.nextTag();
            if (xb.nextEquals(tag,"NAMESPACE"))
               add(NameSpaceXml(xb));
            else if (xb.endTag("LOCALNAMESPACEPATH",&attr)) return;
            else throw ParseException("Expecting /LOCALNAMESPACEPATH or NAMESPACE tag");
         }
      }
      else throw ParseException("Bad LOCALNAMESPACEPATH attributes");
   }
   else throw ParseException("Expecting LOCALNAMESPACEPATH element");
}

/*-------------------------------------------------------*/
/*   HostXml support                                     */
/*-------------------------------------------------------*/

XmlElement HostXml::elm(0);

HostXml::HostXml(XmlBuffer &xb)
{
#ifndef WIN32
   if (DBGX)
      cerr<<"--- HostXml::HostXml()"<<endl;
#endif
   XmlAttr attr;
   attr.eTagFound = 0;
   attr.attr=StringArray(1);

   if (xb.tagEquals("HOST")) {
      if (xb.attrsOk(elm,attr,"HOST")) {
         name=string(xb.getContent(&attr));
         if (xb.endTag("HOST",&attr)) return;
         throw ParseException("Expecting </HOST> tag");
      }
      else throw ParseException("Bad HOST attributes");
   }
   else throw ParseException("Expecting HOST element");
}



/*-------------------------------------------------------*/
/*   NameSpacePathXml support                            */
/*-------------------------------------------------------*/

XmlElement NameSpacePathXml::elm(0);

NameSpacePathXml::NameSpacePathXml(XmlBuffer &xb)
{
#ifndef WIN32
   if (DBGX)
      cerr<<"--- NameSpacePathXml::NameSpacePathXml()"<<endl;
#endif
   char *tag;
   XmlAttr attr;
   attr.eTagFound = 0;
   attr.attr=StringArray(1);

   if (xb.tagEquals("NAMESPACEPATH")) {
      if (xb.attrsOk(elm,attr,"NAMESPACEPATH")) {
         tag=xb.nextTag();
         if (xb.nextEquals(tag,"HOST")) {
            host=HostXml(xb);
            tag=xb.nextTag();
            if (xb.nextEquals(tag,"LOCALNAMESPACEPATH")) {
               lns=LocalNameSpacePathXml(xb);
               if (xb.endTag("NAMESPACEPATH",&attr)) return;
               throw ParseException("Expecting </NAMESPACEPATH> tag");
            }
            else throw ParseException("Expecting <LOCALNAMESPACEPATH> tag");
         }
         else throw ParseException("Expecting <HOST> tag");
      }
      else throw ParseException("Bad NAMESPACEPATH attributes");
   }
   else throw ParseException("Expecting NAMESPACEPATH element");
}

void NameSpacePathXml::toStringBuffer(string &sb, const char *q)
{
   sb = sb + q + host.name;
   for (int i=0, s=lns.size(); i<s; i++) {
      sb = sb + "/" + lns[i].name;
   }
}



/*-------------------------------------------------------*/
/*   LocalInstancepathValueXml support                        */
/*-------------------------------------------------------*/

LocalInstancepathValueXml::LocalInstancepathValueXml(XmlBuffer &xb) { init(xb); }

void LocalInstancepathValueXml::init(XmlBuffer &xb)
{
#ifndef WIN32
   if (DBGX)
      cerr<<"--- LocalInstancepathValueXml::LocalInstancepathValueXml()"<<endl;
#endif
   char *tag;
   XmlAttr attr;
   attr.eTagFound = 0;
   attr.attr=StringArray(1);

   if (xb.tagEquals("LOCALINSTANCEPATH")) {
      if (xb.attrsOk(elm,attr,"LOCALINSTANCEPATH")) {
         tag=xb.nextTag();
         if (xb.nextEquals(tag,"LOCALNAMESPACEPATH")) {
            lnsp=LocalNameSpacePathXml(xb);
            tag=xb.nextTag();
            if (xb.nextEquals(tag,"INSTANCENAME")) {
               name=InstanceNameXml(xb);
               if (xb.endTag("LOCALINSTANCEPATH",&attr)) return;
               throw ParseException("Expecting </LOCALINSTANCEPATH> tag");
            }
            else throw ParseException("Expecting <INSTANCENAME> tag");
         }
         else throw ParseException("Expecting <LOCALNAMESPACEPATH> tag");
      }
      else throw ParseException("Bad LOCALINSTANCEPATH attributes");
   }
   else throw ParseException("Expecting LOCALINSTANCEPATH element");
}

void LocalInstancepathValueXml::toStringBuffer(string &sb, const char *q)
{
   for (int i=0, s=lnsp.size(); i<s; i++) {
      if(i == 0) {
         sb = sb + lnsp[i].name;
      } else {
         sb = sb + "/" + lnsp[i].name;
      }
      if(i == s-1) {
         sb = sb + ":";
      }
   }
   name.toStringBuffer(sb,q);
}


/*-------------------------------------------------------*/
/*   InstancepathValueXml support                        */
/*-------------------------------------------------------*/

XmlElement InstancepathValueXml::elm(0);

InstancepathValueXml::InstancepathValueXml(XmlBuffer &xb) { init(xb); }

void InstancepathValueXml::init(XmlBuffer &xb)
{
#ifndef WIN32
   if (DBGX)
      cerr<<"--- InstancepathValueXml::InstancepathValueXml()"<<endl;
#endif
   char *tag;
   XmlAttr attr;
   attr.eTagFound = 0;
   attr.attr=StringArray(1);

   if (xb.tagEquals("INSTANCEPATH")) {
      if (xb.attrsOk(elm,attr,"INSTANCEPATH")) {
         tag=xb.nextTag();
         if (xb.nextEquals(tag,"NAMESPACEPATH")) {
            nsp=NameSpacePathXml(xb);
            tag=xb.nextTag();
            nsp.toStringBuffer(nspace);
            if (xb.nextEquals(tag,"INSTANCENAME")) {
               name=InstanceNameXml(xb);
               if (xb.endTag("INSTANCEPATH",&attr)) return;
               throw ParseException("Expecting </INSTANCEPATH> tag");
            }
            else throw ParseException("Expecting <INSTANCENAME> tag");
         }
         else throw ParseException("Expecting <NAMESPACEPATH> tag");
      }
      else throw ParseException("Bad INSTANCEPATH attributes");
   }
   else throw ParseException("Expecting INSTANCEPATH element");
}

void InstancepathValueXml::toStringBuffer(string &sb, const char *q)
{
   nsp.toStringBuffer(sb,"");
   name.toStringBuffer(sb,q);
}


/*-------------------------------------------------------*/
/*   InstancenameValueXml support                        */
/*-------------------------------------------------------*/

XmlElement InstancenameValueXml::elm(0);

InstancenameValueXml::InstancenameValueXml(XmlBuffer &xb) { init(xb); }

void InstancenameValueXml::init(XmlBuffer &xb)
{
#ifndef WIN32
   if (DBGX)
      cerr<<"--- InstancenameValueXml::InstancenameValueXml()"<<endl;
#endif
   char *tag=xb.nextTag();
   XmlAttr attr;
   attr.eTagFound = 0;
   attr.attr=StringArray(1);

   if (xb.nextEquals(tag, "INSTANCENAME")) {
      name=InstanceNameXml(xb);
   }
   else throw ParseException("Expecting INSTANCENAME element");
}

void InstancenameValueXml::toStringBuffer(string &sb, const char *q)
{
   name.toStringBuffer(sb,q);
}


/*-------------------------------------------------------*/
/*   ClassNameValueXml support                           */
/*-------------------------------------------------------*/

XmlElement ClassnameValueXml::elm(1,string("NAME"));

ClassnameValueXml::ClassnameValueXml(XmlBuffer &xb)
{
#ifndef WIN32
   if (DBGX)
      cerr<<"--- ClassnameValueXml::ClassnameValueXml()"<<endl;
#endif
   XmlAttr attr;
   attr.eTagFound = 0;
   attr.attr=StringArray(1);

   if (xb.tagEquals("CLASSNAME")) {
      if (xb.attrsOk(elm,attr,"CLASSNAME")) {
         clsName=string(attr.attr[0]);
         if (xb.endTag("CLASSNAME",&attr)) return;
         throw ParseException("Expecting </CLASSNAME> tag");
      }
      else throw ParseException("Bad CLASSNAME attributes");
   }
   else throw ParseException("Expecting CLASSNAME element");
}


/*-------------------------------------------------------*/
/*   ValueXml support                                    */
/*-------------------------------------------------------*/

XmlElement ValueXml::elm(0);
XmlElement ValueXml::elmRef(0);
XmlElement ValueXml::elmArray(0);

ValueXml::ValueXml(XmlBuffer &xb) : subVal(0), valArray(0)
{
  valArray = new ValueArray;
#ifndef WIN32
   if (DBGX)
      cerr<<"--- ValueXml::ValueXml("<<xb.cur<<")"<<endl;
#endif
   XmlAttr attr;
   attr.eTagFound = 0;
   attr.attr=StringArray(1);
   char *tag=xb.cur;
   val="";
   type=TypeRValue;

   if (xb.tagEquals("VALUE.REFERENCE")) {
     vType=TypeValueRef;
   }
   else if (xb.tagEquals("VALUE.ARRAY")) {
     vType=TypeValueArray;
   }
   else if (xb.tagEquals("VALUE")) {
     vType=TypeValue;
   }
   else throw ParseException("Expecting VALUE element");

   if ((vType==TypeValue && xb.attrsOk(elm,attr,"VALUE")) ||
       (vType==TypeValueArray && xb.attrsOk(elmArray,attr,"VALUE.ARRAY")) ||
       (vType==TypeValueRef && xb.attrsOk(elmRef,attr,"VALUE.REFERENCE"))) {

      if (vType==TypeValueRef) {
         tag=xb.nextTag();
         if (xb.nextEquals(tag,"CLASSNAME"))
            subVal=new ClassnameValueXml(xb);
         else if (xb.nextEquals(tag,"INSTANCEPATH"))
             subVal=new InstancepathValueXml(xb);
         else if (xb.nextEquals(tag,"LOCALINSTANCEPATH"))
             subVal=new LocalInstancepathValueXml(xb);
         else if (xb.nextEquals(tag,"INSTANCENAME"))
             subVal=new InstancenameValueXml(xb);
	 else throw ParseException("Expecting CLASSPATH, LOCALCLASSPATH, CLASSNAME, INSTANCEPATH, "
             "LOCALINSTANCEPATH or INSTANCENAME element");
      }

      else if (vType==TypeValueArray) {
        for (tag=xb.nextTag(); xb.nextEquals(tag,"VALUE"); tag=xb.nextTag()) {
            valArray->add(ValueXml(xb));
         }
      }

      else val=string(xb.getContent(&attr));

      tag=xb.nextTag();
      if (xb.nextEquals(tag,"/VALUE")) {
         if (xb.endTag("VALUE") ||
             xb.endTag("VALUE.ARRAY") ||
             xb.endTag("VALUE.REFERENCE")) {
           return;
         }
      }
      else throw ParseException("Expecting /VALUE element");
   }
   else throw ParseException("Bad VALUE attributes");
}

ValueXml::ValueXml(const char *s) : subVal(0), valArray(0)
{
  valArray = new ValueArray;
#ifndef WIN32
   if (DBGX) cerr<<"--- ValueXml::ValueXml("<<s<<")"<<endl;
#endif
   if (*s=='"') val=string(s+1,(int)strlen(s)-2);
   else val=s;
   vType=TypeValue;
}


ValueXml::ValueXml(const ValueXml& src) : RetValXml(*(RetValXml *)&src), subVal(0), valArray(0)
{
  val = src.val;
  subVal = src.subVal ? src.subVal->clone() : src.subVal;
  valArray = src.valArray ? src.valArray->clone() : src.valArray;
  vType = src.vType;
}

ValueXml& ValueXml::operator= (const ValueXml& src)
{
  if(&src != this) {
    val = src.val;
    delete subVal;
    subVal = src.subVal ? src.subVal->clone() : src.subVal;
    delete valArray;
    valArray = src.valArray ? src.valArray->clone() : src.valArray;
    vType = src.vType;
  }
  return *this;
}

ValueXml::~ValueXml()
{
  delete valArray;
  delete subVal;
}

void ValueXml::toStringBuffer(string &sb, const char *q)
{
   switch (vType) {
   case TypeValue:
		{
			string s=Cimom::unescapeXml(val.c_str(),1);
      sb = sb + q+s+q;
		}
      break;
   case TypeValueArray:
		{
      for (int i=0,s=valArray->size(); i<s; i++) {
         (*valArray)[i].toStringBuffer(sb,q);
         if (i+1<s) sb += ",";
      }
		}
      break;
   case TypeValueRef:
      if (dlmRefs) sb += "{";
      subVal->toStringBuffer(sb,q);
      if (dlmRefs) sb += "}";
      break;
   }
}


/*-------------------------------------------------------*/
/*   ErrorXml support                                    */
/*-------------------------------------------------------*/

// Error string table

const char *CimErrorSymbols[] = {
  "Invalid/Unknown CIM Error Code: ",
  "CIM_ERR_FAILED: ",
  "CIM_ERR_ACCESS_DENIED: ",
  "CIM_ERR_INVALID_NAMESPACE: ",
  "CIM_ERR_INVALID_PARAMETER: ",
  "CIM_ERR_INVALID_CLASS: ",
  "CIM_ERR_NOT_FOUND: ",
  "CIM_ERR_NOT_SUPPORTED: ",
  "CIM_ERR_CLASS_HAS_CHILDREN:  ",
  "CIM_ERR_CLASS_HAS_INSTANCES: ",
  "CIM_ERR_INVALID_SUPERCLASS: ",
  "CIM_ERR_ALREADY_EXISTS: ",
  "CIM_ERR_NO_SUCH_PROPERTY: ",
  "CIM_ERR_TYPE_MISMATCH: ",
  "CIM_ERR_QUERY_LANGUAGE_NOT_SUPPORTED: ",
  "CIM_ERR_INVALID_QUERY: ",
  "CIM_ERR_METHOD_NOT_AVAILABLE: ",
  "CIM_ERR_METHOD_NOT_FOUND: ",
};

const long CIM_ERR_MAX = sizeof(CimErrorSymbols)/sizeof(char*) - 1;

XmlElement ErrorXml::elm;
bool ErrorXml::initialized(false);

void ErrorXml::initialize() {
  elm.attr = StringArray(2);
  elm.attr[0] = string("DESCRIPTION");
  elm.attr[1] = string("CODE");
  initialized = true;
}

ErrorXml::ErrorXml(XmlBuffer &xb)
{
  if(!initialized) { initialize();}
#ifndef WIN32
   if (DBGX) cerr<<"--- ErrorXml::ErrorXml("<<xb.cur<<")"<<endl;
#endif
   XmlAttr attr;
   attr.eTagFound = 0;
   attr.attr=StringArray(3);

   if (xb.tagEquals("ERROR")) {
     if (xb.attrsOk(elm,attr,"ERROR")) {
        desc=Cimom::unescapeXml(attr.attr[0].c_str());
        code=attr.attr[1];
	if (desc.substr(0,4)!="CIM_") {
	  // no error symbolic name in description - fill in from table
	  long codenum=atol(code.c_str());
	  if (codenum<0 || codenum>CIM_ERR_MAX) {
	    codenum=0;
	  }
	  desc = CimErrorSymbols[codenum] + desc;
	}
        if (xb.endTag("ERROR",&attr)) {
           set("Cim: ",code,desc);
           return;
        }
        else throw ParseException("Expecting </ERROR> tag");
     }
     else throw ParseException("Bad ERROR attributes");
   }
   else throw ParseException("Expecting ERROR element");
}

ErrorXml::ErrorXml(int c, string &d)
{
   char cde[64];
   sprintf(cde,"%d",c);
   code=cde;
   
   if (d.substr(0,4)!="CIM_") {
	  if (c<0 || c>CIM_ERR_MAX) {
	    c=0;
	  }
	  desc = CimErrorSymbols[c] + d;
	}
   set("Cim: ",code,desc);
}

/*-------------------------------------------------------*/
/*   URL support                                         */
/*-------------------------------------------------------*/

// format:  http[s]://localhost:5988/root/cimv2:rpm_package,name=glibc

char *strndup(char *f, int n) {
   char *p=(char*)malloc(n+1);
   strncpy(p,f,n);
   *(p+(n))=0;
   return p;
}

URL::URL(const char *U)
{
   int pint,indir=0;
   char *p,*q,*phelp,*np,*last,*un,*b;
   char *origu, *u;
   u = origu = strdup(U);

#ifndef WIN32
   if (DBGX) cerr<<"--- U: "<<U<<endl;
#endif

   if (!(strncasecmp(u,"http://",7)==0 || strncasecmp(u, "https://", 8)==0)) {
      for (int i=0; u[i]; i++) {
         if (u[i]=='/') {
            u[i]=0;
            u=resolveIndirection(u,u+i+1);
            origu=u;
	    if (u && (strncasecmp(u, "http://", 7) == 0 ||
		      strncasecmp(u, "https://", 8) == 0))
		indir=1;
	    break;
	 }
	 else if (!isalnum(u[i])) break;
      }
      if (indir==0)
         throw URLException("Only supporting http or https protocols");
   }

   last=u+strlen(u);
   if (strncasecmp(u, "https://", 8) == 0) {
       scheme = "https";
       u += 8;
   }
   else {
       scheme = "http";
       u += 7;
   }
   phelp=strchr(u,'@'); // potential auth token delimiter
   p=strchr(u,'.');     // potential key/host delimiter (doesn't matter)
   b=strchr(u,'[');     // likely an IPv6 host
   if (phelp && ( p==NULL || p > phelp)) {
     // contains auth token[s] -- process them
     p=strchr(u,':');
     if (b != NULL && p != NULL && (p < b)) {
       // IPv6 host; user and password
       user=string(u,p-u);
       password=string(p+1,phelp-p-1);
     }
     else if (p && phelp > p && (b == NULL)) {
       // user and password specified, not IPv6 host
       user=string(u,p-u);
       password=string(p+1,phelp-p-1);
     } 
     else if (p==NULL || ((b != NULL) && (p != NULL)) ) {
       //only user is specified
       user=string(u,phelp-u);
     }
     else if (p > phelp ) {
       //user and port specified
       user=string(u,phelp-u);
     }
     u=phelp+1;
   }
   phelp=strchr(u,'/');
   if (phelp) {
     // namespace is specified -- truncate the string and remember that
     *phelp=0;
   }
   if (b == NULL) {
        p=strchr(u,':');
   }
   else {
        p=strchr(u,']');    // make sure we don't grab inside the []
        p=strchr(p,':');
   }

   if (!p) {
     host=u; // we may do that as we are at the end of the string
     // No port specified, use default based on the scheme
     port = (scheme == "https") ? "5989" : "5988";
   } else {
     host=string(u,p-u);
     port=string(p+1); // we may do that as we are at the end of the string
   }
   if (host.size()==0) throw URLException("Could not locate hostname");
   pint=strtol(port.c_str(),&np,10);
   if (*np) throw URLException("Invalid port number");

   if (!phelp || strlen(phelp+1) == 0) {
     phelp=strdup("/root/cimv2");      // no namespace given -> use default
   }

   u=phelp+1;
   q=strchr(u,':');
   if (!q)  q=u+strlen(u);
   for (; (p=strchr(u,'/'))!=NULL && p<q; u=p+1) {
      ns.push_back(string(u,p-u));
   }
   if (*u==0 || *u==':')
     throw URLException("Invalid namespace specification");

   p=strchr(u,':');
   if (p && strlen(p)) ns.push_back(string(u,p-u));
   else {
      ns.push_back(string(u));
      goto finish_URL;
   }
   if (ns.size()==0) throw URLException("No namespace found");

   u=p+1;
   p=strchr(u,'.');
   if (!p) {
      cName=string(u);
      goto finish_URL;
   }
   cName=string(u,p-u);
   //DBGX=1;
   for (u=p+1; ; u=p+1) {
      if (refLookAhead(u,&un)) {
         char *t=strndup(u,un-u-1);
         keys.add(PropertyXml(t,1));
	 free(t);
#ifndef WIN32
	 if (DBGX) cerr<<"--- CATCHED !!! "<<strndup(u,un-u-1)<<endl;
#endif
	 u=un;
      }
      if ((q=strchr(u,'='))) {
	q++;
	if(*(q)!='\"') {
	  for (int i=0; q[i]!=0; i++) {
	    if(q[i]=='=') throw URLException("Incorrect value assigned");
	    if(q[i]==',') break;
	  }
	}
      }
      if ((p=strpbrk(u,",\""))==NULL) break;
      if (*p=='"') {
         if (*(p-1)!='=') throw URLException("Incorrectly quoted string 1");
	 p++;
         if ((p=strchr(p,'"'))==NULL) throw URLException("Unbalanced quoted string");
#ifndef WIN32
	 if (DBGX) cerr<<"-------- strndup(u,(p-u)+1): "<<strndup(u,(p-u)+1)<<endl;
#endif
	 p++;
	 if (*p!=',' && *p!=0) throw URLException("Incorrectly quoted string 2");
#ifndef WIN32
	 if (DBGX) cerr<<"--- found quoted string"<<endl;
#endif
	 if (*p==0) break;
      }
      // if ((p=strchr(u,','))==NULL) break;
#ifndef WIN32
      if (DBGX) cerr<<"--- strndup(u,p-u): "<<strndup(u,p-u)<<endl;
#endif
      char *t=strndup(u,p-u);
      keys.add(PropertyXml(t,0));
      free(t);
   }
   if (last>u) {
      char *t=strndup(u,last-u);
#ifndef WIN32
      if (DBGX) cerr<<"--- strndup(u,last-u): "<<strndup(u,last-u)<<endl;
#endif
      keys.add(PropertyXml(t,0));
      free(t);
   }
   //DBGX=0;
 finish_URL:
   free(origu);
   return;
}



int URL::refLookAhead(char *u, char **nu)
{
   int state=0;
   char *pu=NULL;
   for (int i=0; u[i]!=0; i++) {
      switch (state) {
      case 0: if (isalnum(u[i])) state=1;
              break;
      case 1: if (u[i]=='=') state=2;
              break;
      case 2: if (isalnum(u[i])) state=3;
              else return 0;
	      break;
      case 3: if (u[i]=='=') return 0;
              if (u[i]=='.') {
	         state=4;
		 pu=u+i;
              }
              break;
      case 4: if (!isalnum(u[i])) return 0;
              *nu=u+i;
#ifndef WIN32
              if (DBGX)
	         cerr<<"--- reference found"<<endl;
#endif
	      return 1;
      }
   }
   return 0;
}


char *URL::resolveIndirection(char *ind, char *u)
{
   char *fn=getenv("WBEMCLI_IND");
   if (fn==NULL) fn=strdup("wbemcli.ind");
   FILE *indf=fopen(fn,"r");
   if (!indf) throw URLException("File wbemcli.ind not found");
   size_t bl=2000;
   char *buffer=(char*)malloc(bl);
   char *origbuff=buffer;
   while(fgets(buffer,bl,indf)) {
      if (buffer[0]=='#') continue;
      if (strncmp(buffer,ind,strlen(ind))==0 && buffer[strlen(ind)]==':') {
         char *rb=(char*)malloc(strlen(buffer)+strlen(u)+16);
	 for (buffer+=strlen(ind)+1; isspace(*buffer); buffer++);
	 strcpy(rb,buffer);
         char *erb=rb+strlen(rb)-1;
	 for (; isspace(*erb); erb--) *erb=0;
	 strcat(rb,"/");
	 strcat(rb,u);
         fclose(indf);
         free(origbuff);
	 free(ind);
	 return rb;
      }
   }
   fclose(indf);
   free(origbuff);
   throw URLException("No valid indirection found");
   return NULL;
}

void URL::toStringBuffer(string &sb, int nso)
{
   sb = sb + host + ":" + port + "/";
   ns.toStringBuffer(sb,"/");
   if (nso) return;
   sb = sb + ":" + cName;
   if (nso==2) return;
   int t=useNl;
   useNl=0;
   if (keys.size()) {
      sb += ".";
      keys.toStringBuffer(sb,",");
   }
   useNl=t;
}

void URL::toStringBufferGI(string& sb)
{
	sb = sb + host + ":" + port + "/";
	ns.toStringBuffer(sb,"/");
	sb = sb + ":" + cName + ".";
	for (int i=0,s=keys.size(); i<s; i++) {
		PropertyXml p=keys[i];
        sb = sb + p.name + "=";
       	if (p.pType==TypePropertyRef) {
       		sb = sb + p.value.val().getValue() + ".";
       	} else {
       		sb = sb + "\"" + p.value.val().getValue() + "\"";
       		if (i < s-1) {
       			sb = sb + ",";
       		}
       	}
	}
}

void URL::toStringBuffer(string& sb)
{
   int state=0;

   for (int i=0,s=keys.size(); i<s; i++) {
      PropertyXml p=keys[i];
      //      cout<<endl<<"--- i: "<<i<<", state: "<<state<<endl;
      //      cout<<"--- p.name: "<<p.name<<", p.pType: "<<p.pType;
      //      cout<<", p.value.val().getValue(): "<<p.value.val().getValue()<<endl;
      switch (state) {
      case 0:
         sb = sb + "<KEYBINDING NAME=\"" + p.name + "\">";
         if (p.pType==TypePropertyRef) {
            const char *u, *nsp;
            const char *cnp;
            StringArray lns;
            string cn;

            cn = p.value.val().getValue();
            cnp = cn.c_str();
            u = strchr(cnp, ':');

            if (!u) {
                lns = ns;
            } else {
                if (*cnp == '/')
                    cnp++;
                for (; (nsp = strchr(cnp, '/')) != NULL && nsp<u; cnp = nsp+1) {
                    lns.push_back(string(cnp, nsp-cnp));
                }
                lns.push_back(string(cnp, u-cnp));

                cn = string(u+1);
            }
            
            sb += "<VALUE.REFERENCE>";
            sb += "<INSTANCEPATH>";
            sb += "<NAMESPACEPATH>"
	            "<HOST>"+host+"</HOST>"
                    "<LOCALNAMESPACEPATH>";
            for (unsigned int i=0; i<lns.size(); i++)
               sb = sb + "<NAMESPACE NAME=\"" + lns[i] + "\"></NAMESPACE>";
            sb += "</LOCALNAMESPACEPATH>"
                   "</NAMESPACEPATH>";
            sb = sb + "<INSTANCENAME CLASSNAME=\"" + cn + "\">";
            state=1;
	    continue;
         }
         else {
#ifdef OLD_PEGASUS_CONFORM
            sb = sb + "<KEYVALUE VALUETYPE=\"" + "string" + "\">" + p.value.val().getValue() + "</KEYVALUE>";
#else
            sb = sb + "<KEYVALUE VALUETYPE=\"" + p.type + "\">" + p.value.val().getValue() + "</KEYVALUE>";
#endif
            state=0;
         }
	 break;
      case 1:
         if (p.pType==TypePropertyRef) {
            sb = sb + "</INSTANCENAME>"
                      "</INSTANCEPATH>"
	              "</VALUE.REFERENCE>";
	    i--;
            state=0;
	 }
	 else {
            sb = sb + "<KEYBINDING NAME=\"" + p.name + "\">";
#ifdef OLD_PEGASUS_CONFORM
            sb = sb + "<KEYVALUE VALUETYPE=\"" + "string" + "\">" + p.value.val().getValue() + "</KEYVALUE>";
#else
            sb = sb + "<KEYVALUE VALUETYPE=\"" + p.type + "\">" + p.value.val().getValue() + "</KEYVALUE>";
#endif
	 }
	 break;
      }
      sb += "</KEYBINDING>";
   }

   if (state==1){
      sb += "</INSTANCENAME>";
      sb += "</INSTANCEPATH>";
      sb += "</VALUE.REFERENCE>";
      sb += "</KEYBINDING>";
   }
}


/*-------------------------------------------------------*/
/*   Cimom support                                       */
/*-------------------------------------------------------*/

#define emitqualorigin() \
do{   if (qual)\
      sb += "<IPARAMVALUE NAME=\"IncludeQualifiers\"><VALUE>TRUE</VALUE></IPARAMVALUE>\n";\
   else\
      sb += "<IPARAMVALUE NAME=\"IncludeQualifiers\"><VALUE>FALSE</VALUE></IPARAMVALUE>\n";\
\
   if (origin)\
      sb += "<IPARAMVALUE NAME=\"IncludeClassOrigin\"><VALUE>TRUE</VALUE></IPARAMVALUE>\n";\
   else\
      sb += "<IPARAMVALUE NAME=\"IncludeClassOrigin\"><VALUE>FALSE</VALUE></IPARAMVALUE>\n";}while(0)

#define emitlocal() \
do{ if (local)\
      sb += "<IPARAMVALUE NAME=\"LocalOnly\"><VALUE>TRUE</VALUE></IPARAMVALUE>\n";\
   else\
      sb += "<IPARAMVALUE NAME=\"LocalOnly\"><VALUE>FALSE</VALUE></IPARAMVALUE>\n";} while(0)

#define emitdeep() \
do{if (deep)\
      sb += "<IPARAMVALUE NAME=\"DeepInheritance\"><VALUE>TRUE</VALUE></IPARAMVALUE>\n";\
   else\
      sb += "<IPARAMVALUE NAME=\"DeepInheritance\"><VALUE>FALSE</VALUE></IPARAMVALUE>\n";} while(0)



char Cimom::xmlHeader[]={
   "<?xml version=\"1.0\" encoding=\"utf-8\" ?>\n"
   "<CIM CIMVERSION=\"2.0\" DTDVERSION=\"2.0\">\n"
   "<MESSAGE ID=\"4711\" PROTOCOLVERSION=\"1.0\">"
   "<SIMPLEREQ>"};

char Cimom::xmlTrailer[]={
   "</SIMPLEREQ>\n</MESSAGE></CIM>"};

ClassXml *Cimom::getClass(URL &url, char *attr, bool qual, bool origin, bool local)
{
   string resp;
   SimplerspXml *rsp;
   RetValXml *rv;

   resp=getClass2str(&url, attr, qual, origin, local);
   if ( resp != "" )
   {
      CimXml cx(resp);
      rsp=cx.getResponse();
      rv=rsp->getReturnData();

      if (rv->getType()==TypeClass) return (ClassXml*)rv;
      delete rv;
      throw ParseException("Unexpected response type");
   }
   return NULL;
}

string Cimom::getClass2str(URL *url, char *attr, bool qual, bool origin, bool local)
{
   string sb;
   string resp;   

   genRequest(*url,"GetClass");

   sb = xmlHeader;
   sb += "<IMETHODCALL NAME=\"GetClass\">\n"
          "<LOCALNAMESPACEPATH>\n";

   for (int i=0,s=url->ns.size(); i<s; i++)
      sb += "<NAMESPACE NAME=\"" + url->ns[i] + "\"/>\n";
   sb += "</LOCALNAMESPACEPATH>\n";

   sb  = sb + "<IPARAMVALUE NAME=\"ClassName\"><CLASSNAME NAME=\"" + url->cName + "\"/></IPARAMVALUE>\n";

   emitlocal();
   emitqualorigin();

   if (attr) {
      AList *al=new AList(attr);
      sb += "<IPARAMVALUE NAME=\"PropertyList\"><VALUE.ARRAY>";
      for(int i=0; i<al->attrs; i++)
           sb = sb + "<VALUE>" + al->attrv[i] + "</VALUE>";
      sb += "</VALUE.ARRAY></IPARAMVALUE>\n";
      delete al;
   }

   sb += "</IMETHODCALL>";
   sb += xmlTrailer;

   addPayload(sb);
   resp = getResponse();
   if (resp != "") {
       CimXml cx(resp);
       SimplerspXml *rsp = cx.getResponse();
       RetValXml *rv = rsp->getReturnData();
   }
   return resp;
}

int Cimom::testInst(URL &url, AValue *av)
{
   throw CmdException("create/modify instance with reference values not yet supported");
   return 0;
}

InstanceXml *Cimom::getInstance(URL &url, char *attr,
            bool qual, bool origin, bool local)
{
   string sb;
   string resp;
   SimplerspXml *rsp;
   RetValXml *rv;

   genRequest(url,"GetInstance");

   sb = xmlHeader;
   sb += "<IMETHODCALL NAME=\"GetInstance\">"
          "<LOCALNAMESPACEPATH>";
   for (int i=0,s=url.ns.size(); i<s; i++)
      sb = sb + "<NAMESPACE NAME=\"" + url.ns[i] + "\"></NAMESPACE>";
   sb += "</LOCALNAMESPACEPATH>\n";

   emitlocal();
   emitqualorigin();

   if (attr) {
      AList *al=new AList(attr);
      sb += "<IPARAMVALUE NAME=\"PropertyList\"><VALUE.ARRAY>";
      for(int i=0; i<al->attrs; i++)
           sb = sb + "<VALUE>" + al->attrv[i] + "</VALUE>";
      sb += "</VALUE.ARRAY></IPARAMVALUE>\n";
      delete al;
   }

   sb = sb + "<IPARAMVALUE NAME=\"InstanceName\">"
          "<INSTANCENAME CLASSNAME=\"" + url.cName + "\">";

   url.toStringBuffer(sb);

   sb += "</INSTANCENAME></IPARAMVALUE>\n</IMETHODCALL>";

   sb += xmlTrailer;

   addPayload(sb);
   if ((resp=getResponse()) != "")
   {
   CimXml cx(resp);
   rsp=cx.getResponse();
   rv=rsp->getReturnData();

   if (rv->getType()==TypeInstance) return (InstanceXml*)rv;
   delete rv;
   throw ParseException("Unexpected response type");
   }
   return NULL;
}


NamedInstancesXml *Cimom::enumInstances(URL &url, char *attr, bool deep, bool qual, bool origin, bool local)
{
   string resp;
   SimplerspXml *rsp;
   RetValXml *rv;

   resp=enumInstances2str(&url, attr, deep, qual, origin, local);
   if ( resp != "" )
   {
      CimXml cx(resp);
      rsp=cx.getResponse();
      rv=rsp->getReturnData();  // somehow the cloning here kills my data - clone() broken?

      if (!rv) return NULL;
      if (rv->getType()==TypeNamedInstances) return (NamedInstancesXml*)rv;
      if (rv->getType()==TypeNamedInstance)
	{
	  NamedInstancesXml * retval = new NamedInstancesXml((NamedInstanceXml*)rv);
	  delete rv; 
	  return retval;
	}
      delete rv;
      throw ParseException("Unexpected response type");
   }
   return NULL;
}


string Cimom::enumInstances2str(URL *url, char *attr, bool deep, bool qual, bool origin, bool local)
{
  string sb;

   genRequest(*url,"EnumerateInstances");

   sb = xmlHeader;
   sb += "<IMETHODCALL NAME=\"EnumerateInstances\">"
          "<LOCALNAMESPACEPATH>";
   for (int i=0,s=url->ns.size(); i<s; i++)
      sb = sb + "<NAMESPACE NAME=\"" + url->ns[i] + "\"></NAMESPACE>";
   sb += "</LOCALNAMESPACEPATH>\n";
   sb  = sb + "<IPARAMVALUE NAME=\"ClassName\"><CLASSNAME NAME=\"" + url->cName + "\"/></IPARAMVALUE>\n";

   emitdeep();
   emitlocal();
   emitqualorigin();

   if (attr) {
      AList *al=new AList(attr);
      sb += "<IPARAMVALUE NAME=\"PropertyList\"><VALUE.ARRAY>";
      for(int i=0; i<al->attrs; i++)
           sb = sb + "<VALUE>" + al->attrv[i] +"</VALUE>";
      sb += "</VALUE.ARRAY></IPARAMVALUE>\n";
      delete al;
   }

   sb += "</IMETHODCALL>";
   sb += xmlTrailer;

   addPayload(sb);
   return getResponse();
}


InstanceNamesXml *Cimom::enumInstanceNames(URL &url)
{
  string sb;
   string resp;
   SimplerspXml *rsp;
   RetValXml *rv;

   genRequest(url,"EnumerateInstanceNames");

   sb = xmlHeader;
   sb += "<IMETHODCALL NAME=\"EnumerateInstanceNames\">"
          "<LOCALNAMESPACEPATH>";
   for (int i=0,s=url.ns.size(); i<s; i++)
      sb = sb + "<NAMESPACE NAME=\"" + url.ns[i] + "\"></NAMESPACE>";
   sb += "</LOCALNAMESPACEPATH>\n";
   sb = sb + "<IPARAMVALUE NAME=\"ClassName\"><CLASSNAME NAME=\"" + url.cName 
           + "\"/></IPARAMVALUE>\n"  "</IMETHODCALL>";
   sb += xmlTrailer;

   addPayload(sb);
   if ((resp=getResponse()) != "")
   {
   CimXml cx(resp);
   rsp=cx.getResponse();
   rv=rsp->getReturnData();

   if (!rv) return NULL;
   if (rv->getType()==TypeInstanceNames) return (InstanceNamesXml*)rv;
   if (rv->getType()==TypeInstanceName) 
     {
       InstanceNamesXml * retval = new InstanceNamesXml(*(InstanceNameXml*)rv);
       delete rv; 
       return retval;
     }
   delete(rv);
   throw ParseException("Unexpected response type");
   }
   return NULL;
}


ClassesXml *Cimom::enumClasses(URL &url, bool deep, bool qual, bool origin, bool local)
{
   string sb;
   string resp;
   SimplerspXml *rsp;
   RetValXml *rv;

   genRequest(url,"EnumerateClasses");

   sb = xmlHeader;
   sb += "<IMETHODCALL NAME=\"EnumerateClasses\">"
          "<LOCALNAMESPACEPATH>";
   for (int i=0,s=url.ns.size(); i<s; i++)
      sb = sb + "<NAMESPACE NAME=\"" + url.ns[i] + "\"></NAMESPACE>";
   sb += "</LOCALNAMESPACEPATH>\n";
   if (url.cName != "")
      sb = sb + "<IPARAMVALUE NAME=\"ClassName\"><CLASSNAME NAME=\"" + url.cName + "\"/></IPARAMVALUE>\n";

   emitdeep();
   emitlocal();
   emitqualorigin();

   sb += "</IMETHODCALL>";
   sb += xmlTrailer;

   addPayload(sb);
   if ((resp=getResponse()) != "")
   {
   CimXml cx(resp);
   rsp=cx.getResponse();
   rv=rsp->getReturnData();

   if (!rv) return NULL;
   if (rv->getType()==TypeClasses) return (ClassesXml*)rv;
   if (rv->getType()==TypeClass) 
     {
       ClassesXml * retval = new ClassesXml((ClassXml*)rv);
       delete rv; 
       return retval;
     }
   delete rv;
   throw ParseException("Unexpected response type");
   }
   return NULL;
}


ClassNamesXml *Cimom::enumClassNames(URL &url, bool deep)
{
   string resp;
   SimplerspXml *rsp;
   RetValXml *rv;

   if ((resp=enumClassNames2str(&url, deep)) != "")
   {
   CimXml cx(resp);
   rsp=cx.getResponse();
   rv=rsp->getReturnData();

   if (!rv) return NULL;
   if (rv->getType()==TypeClassNames) return (ClassNamesXml*)rv;
   if (rv->getType()==TypeClassName) 
     {
       ClassNamesXml * retval = new ClassNamesXml((ClassNameXml*)rv);
       delete rv;
       return retval;
     }
   delete rv;

   throw ParseException("Unexpected response type");
   }
   return NULL;
}



string Cimom::enumClassNames2str(URL *url, bool deep)
{
   string sb;

   genRequest(*url,"EnumerateClassNames");

   sb = xmlHeader;
   sb += "<IMETHODCALL NAME=\"EnumerateClassNames\">";
   sb += "<LOCALNAMESPACEPATH>";
   for (int i=0,s=url->ns.size(); i<s; i++)
      sb = sb + "<NAMESPACE NAME=\"" + url->ns[i] + "\"></NAMESPACE>";
   sb += "</LOCALNAMESPACEPATH>\n";
   if (url->cName != "")
   {
      sb = sb + "<IPARAMVALUE NAME=\"ClassName\"><CLASSNAME NAME=\""
              + url->cName + "\"/></IPARAMVALUE>\n";
   }

   emitdeep();
   sb += "</IMETHODCALL>";
   sb += xmlTrailer;

   addPayload(sb);
   return getResponse();
}



InstanceNameXml *Cimom::createInstance(URL &url, ClassXml *cls, char *attr)
{
   int i, s;
   Option<string> sg;
   string sb;
   string resp;
   SimplerspXml *rsp;
   RetValXml *rv;
   PropertyArray &properties=cls->getProperties();
   QualifierArray &qualifiers=cls->getQualifiers();
   AVPs avp(attr,properties);

   genRequest(url,"CreateInstance");

   if (!cls->keysOk(url))
      throw CmdException("Bad key specification");

   sb = xmlHeader;
   sb += "<IMETHODCALL NAME=\"CreateInstance\">"
          "<LOCALNAMESPACEPATH>";
   for (i=0,s=url.ns.size(); i<s; i++)
      sb = sb + "<NAMESPACE NAME=\"" + url.ns[i] + "\"></NAMESPACE>";
   sb += "</LOCALNAMESPACEPATH>\n";
   sb += "<IPARAMVALUE NAME=\"NewInstance\">";
   sb = sb + "<INSTANCE CLASSNAME=\""+url.cName+"\">";

   for (i=0,s=qualifiers.size(); i<s; i++) {
      sb += qualifiers[i].str;
   }

   for (i=0,s=properties.size(); i<s; i++) {
      PropertyXml &p=properties[i];
      if (avp.check(p.name.c_str())) {
         if (p.pType==TypePropertyRef) {
	   p.toXmlBuffer(sb,avp.get(p.name.c_str()),0,&url);
	   //testInst(url,avp.get(p.name.c_str()));
         }
         else 
	   p.toXmlBuffer(sb,avp.get(p.name.c_str()));
      }
   }

   if ((sg=avp.notGotten()).exists()) {
     string err;
     err = err + "Undefined attribute(s) specified: " + sg.val();
     throw CmdException(err);
   }

   sb += "</INSTANCE>"
          "</IPARAMVALUE>\n"
         "</IMETHODCALL>";
   sb += xmlTrailer;

   addPayload(sb);
   if ((resp=getResponse()) != "")
   {
   CimXml cx(resp);
   rsp=cx.getResponse();
   rv=rsp->getReturnData();

   if (!rv) return NULL;
   if (rv->getType()==TypeInstanceName) return (InstanceNameXml*)rv;
   delete rv;
   throw ParseException("Unexpected response type");
   }
   return NULL;
}


void Cimom::modifyInstance(URL &url, InstanceXml *inst, char* attr)
{
   int i, s;
   Option<string> sg;
   string sb;
   string resp;
   SimplerspXml *rsp;
   RetValXml *rv;
   PropertyArray &properties=inst->getProperties();
   QualifierArray &qualifiers=inst->getQualifiers();
   AVPs avp(attr,properties);

   genRequest(url,"ModifyInstance");

   sb = xmlHeader;
   sb += "<IMETHODCALL NAME=\"ModifyInstance\">"
          "<LOCALNAMESPACEPATH>";
   for (i=0,s=url.ns.size(); i<s; i++)
      sb = sb + "<NAMESPACE NAME=\"" + url.ns[i] + "\"></NAMESPACE>";
   sb += "</LOCALNAMESPACEPATH>\n";
   sb += "<IPARAMVALUE NAME=\"ModifiedInstance\">";
   sb = sb + "<VALUE.NAMEDINSTANCE>"
          "<INSTANCENAME CLASSNAME=\"" + url.cName + "\">";

   url.toStringBuffer(sb);

   sb = sb + "</INSTANCENAME>"
          "<INSTANCE CLASSNAME=\"" + url.cName + "\">";

   for (i=0,s=qualifiers.size(); i<s; i++) {
      sb += qualifiers[i].str;
   }

   for (i=0,s=properties.size(); i<s; i++) {
      PropertyXml &p=properties[i];
      if (avp.check(p.name.c_str())) {
	if (p.pType==TypePropertyRef) {
	  p.toXmlBuffer(sb,avp.get(p.name.c_str()),0,&url);
	  //testInst(url,avp.get(p.name.c_str()));
	} 
	else
	  p.toXmlBuffer(sb,avp.get(p.name.c_str()));
      }
      else sb += p.str;
   }

   if ((sg=avp.notGotten()).exists()) {
     string err;
     err = err + "Undefined attribute(s) specified: " + sg.val();
     throw CmdException(err);
   }

   sb +=  "</INSTANCE>"
          "</VALUE.NAMEDINSTANCE>"
          "</IPARAMVALUE>\n"
          "</IMETHODCALL>";
   sb += xmlTrailer;

   addPayload(sb);
   if ((resp=getResponse()) != "")
   {
   CimXml cx(resp);
   rsp=cx.getResponse();
   rv=rsp->getReturnData();

   if (!rv) return;
   if (rv->getType()==TypeNull) { delete rv; return; }
   delete rv;
   throw ParseException("Unexpected response type");
   }
   return;
}


void Cimom::deleteClass(URL &url)
{
   string sb;
   string resp;
   SimplerspXml *rsp;
   RetValXml *rv;

   genRequest(url,"DeleteClass");

   sb = xmlHeader;
   sb += "<IMETHODCALL NAME=\"DeleteClass\">" "<LOCALNAMESPACEPATH>";

   for (int i=0,s=url.ns.size(); i<s; i++)
      sb = sb + "<NAMESPACE NAME=\"" + url.ns[i] + "\"></NAMESPACE>";
   sb += "</LOCALNAMESPACEPATH>\n";

   sb = sb + "<IPARAMVALUE NAME=\"ClassName\"><CLASSNAME NAME=\"" +
          url.cName + "\"/></IPARAMVALUE>\n";

   sb += "</IMETHODCALL>";
   sb += xmlTrailer;

   addPayload(sb);
   if ((resp=getResponse()) != "")
   {
   CimXml cx(resp);
   rsp=cx.getResponse();
   rv=rsp->getReturnData();

   if (!rv) return;
   if (rv->getType()==TypeNull) {delete rv; return;}
   delete rv;
   throw ParseException("Unexpected response type");
   }
   return;
}


void Cimom::deleteInstance(URL &url)
{
   string sb;
   string resp;
   SimplerspXml *rsp;
   RetValXml *rv;

   genRequest(url,"DeleteInstance");

   sb = xmlHeader;
   sb += "<IMETHODCALL NAME=\"DeleteInstance\">"
          "<LOCALNAMESPACEPATH>";
   for (int i=0,s=url.ns.size(); i<s; i++)
      sb = sb + "<NAMESPACE NAME=\"" + url.ns[i] + "\"></NAMESPACE>";
   sb += "</LOCALNAMESPACEPATH>\n";
   sb += "<IPARAMVALUE NAME=\"InstanceName\">";
   sb = sb + "<INSTANCENAME CLASSNAME=\"" + url.cName + "\">";

   url.toStringBuffer(sb);

   sb += "</INSTANCENAME>";

   sb += "</IPARAMVALUE>\n"
          "</IMETHODCALL>";
   sb += xmlTrailer;

   addPayload(sb);
   if ((resp=getResponse()) != "")
   {
   CimXml cx(resp, -1);
   rsp=cx.getResponse();
   rv=rsp->getReturnData();

   if (!rv) return;
   if (rv->getType()==TypeNull) { delete rv; return; }
   delete rv;
   throw ParseException("Unexpected response type");
   }
   return;
}

/* ---------------------------------------------------------------------------- */
/* ---------------------- Associator Provider Interface ----------------------- */


ObjectsWithPathXml *Cimom::associators(URL &url, char *assocClass, char *resultClass,
                                       char *role, char *resultRole, char *attr,
                                       bool qual, bool origin) {
  string resp;
  SimplerspXml *rsp;
  RetValXml *rv;

  resp = associators2str(&url, assocClass,resultClass, role, resultRole, attr, qual, origin);
  if (resp!="") {
    CimXml cx(resp);
    rsp=cx.getResponse();
    rv=rsp->getReturnData();

    if (!rv) return NULL;
    if (rv->getType()==TypeObjectsWithPath) return (ObjectsWithPathXml*)rv;
    if (rv->getType()==TypeObjectWithPath)
	{
	  ObjectsWithPathXml * retval = new ObjectsWithPathXml(*(ObjectWithPathXml*)rv);
	  delete rv; 
	  return retval;
	}
    delete rv;
    throw ParseException("Unexpected response type");
  }
  return NULL;
}

string Cimom::associators2str(URL *url, char *assocClass, char *resultClass,
                                       char *role, char *resultRole, char *attr,
                                       bool qual, bool origin)
{
   string sb;

   genRequest(*url,"Associators");

   sb = xmlHeader;
   sb += "<IMETHODCALL NAME=\"Associators\">"
          "<LOCALNAMESPACEPATH>";
   for (int i=0,s=url->ns.size(); i<s; i++)
      sb = sb + "<NAMESPACE NAME=\"" + url->ns[i] + "\"></NAMESPACE>";
   sb += "</LOCALNAMESPACEPATH>\n";

   sb = sb + "<IPARAMVALUE NAME=\"ObjectName\">"
          "<INSTANCENAME CLASSNAME=\"" + url->cName +"\">";

   url->toStringBuffer(sb);

   sb += "</INSTANCENAME></IPARAMVALUE>\n";

   if (assocClass!=NULL)
      sb = sb + "<IPARAMVALUE NAME=\"AssocClass\"><CLASSNAME NAME=\"" + assocClass +
              "\"/></IPARAMVALUE>\n";
   if (resultClass!=NULL)
      sb = sb + "<IPARAMVALUE NAME=\"ResultClass\"><CLASSNAME NAME=\"" + resultClass + 
              "\"/></IPARAMVALUE>\n";

   if (role)
      sb = sb + "<IPARAMVALUE NAME=\"Role\"><VALUE>" + role + "</VALUE></IPARAMVALUE>\n";
   if (resultRole)
      sb = sb + "<IPARAMVALUE NAME=\"ResultRole\"><VALUE>" + 
              resultRole + "</VALUE></IPARAMVALUE>\n";

   emitqualorigin();

   if (attr) {
      AList *al=new AList(attr);
      sb += "<IPARAMVALUE NAME=\"PropertyList\"><VALUE.ARRAY>";
      for(int i=0; i<al->attrs; i++)
           sb = sb + "<VALUE>" + al->attrv[i] + "</VALUE>";
      sb += "</VALUE.ARRAY></IPARAMVALUE>\n";
      delete al;
   }

   sb += "</IMETHODCALL>";
   sb += xmlTrailer;

   addPayload(sb);
   return getResponse();
}


InstanceNamesXml *Cimom::associatorNames(URL &url, char *assocClass,
                                         char *resultClass,
					 char *role, char *resultRole) {
   string sb;
   string resp;
   SimplerspXml *rsp;
   RetValXml *rv;

   genRequest(url,"AssociatorNames");

   sb = xmlHeader;
   sb += "<IMETHODCALL NAME=\"AssociatorNames\">"
          "<LOCALNAMESPACEPATH>";
   for (int i=0,s=url.ns.size(); i<s; i++)
      sb += "<NAMESPACE NAME=\"" + url.ns[i] + "\"></NAMESPACE>";
   sb += "</LOCALNAMESPACEPATH>\n";

   sb = sb + "<IPARAMVALUE NAME=\"ObjectName\">"
          "<INSTANCENAME CLASSNAME=\"" + url.cName + "\">";

   url.toStringBuffer(sb);

   sb += "</INSTANCENAME></IPARAMVALUE>\n";

   if (assocClass!=NULL)
      sb = sb + "<IPARAMVALUE NAME=\"AssocClass\"><CLASSNAME NAME=\"" + assocClass + 
              "\"/></IPARAMVALUE>\n";
   if (resultClass!=NULL)
      sb = sb + "<IPARAMVALUE NAME=\"ResultClass\"><CLASSNAME NAME=\"" + resultClass + 
              "\"/></IPARAMVALUE>\n";

   if (role!=NULL)
      sb = sb + "<IPARAMVALUE NAME=\"Role\"><VALUE>" + role + "</VALUE></IPARAMVALUE>\n";
   if (resultRole!=NULL)
      sb = sb + "<IPARAMVALUE NAME=\"ResultRole\"><VALUE>" + 
              resultRole + "</VALUE></IPARAMVALUE>\n";


   sb += "</IMETHODCALL>";
   sb += xmlTrailer;

   addPayload(sb);
   if ((resp=getResponse()) != "")
   {
   CimXml cx(resp);
   rsp=cx.getResponse();
   rv=rsp->getReturnData();

   if (!rv) return NULL;
   if (rv->getType()==TypeInstanceNames) return (InstanceNamesXml*)rv;
   if (rv->getType()==TypeInstanceName) 
	{
	  InstanceNamesXml * retval = new InstanceNamesXml(*(InstanceNameXml*)rv);
	  delete rv; 
	  return retval;
	}
   delete rv; 
   throw ParseException("Unexpected response type");
   }
   return NULL;
}


ObjectsWithPathXml *Cimom::references(URL &url,  char *resultClass, char *role, char *attr,
                                      bool qual, bool origin)
{
   string resp;
   SimplerspXml *rsp;
   RetValXml *rv;

   resp =references2str(&url, resultClass, role, attr, qual, origin);
   if (resp != "")
   {
      CimXml cx(resp);
      rsp=cx.getResponse();
      rv=rsp->getReturnData();

      if (!rv) return NULL;
      if (rv->getType()==TypeObjectsWithPath) return (ObjectsWithPathXml*)rv;
      if (rv->getType()==TypeObjectWithPath) 
	{
	  ObjectsWithPathXml * retval = new ObjectsWithPathXml(*(ObjectWithPathXml*)rv);
	  delete rv; 
	  return retval;
	}
      delete rv; 
      throw ParseException("Unexpected response type");
   }
   return NULL;
}

string Cimom::references2str(URL *url,  char *resultClass, char *role, char *attr,
                        bool qual, bool origin)
{
   string sb;

   genRequest(*url,"References");

   sb = xmlHeader;
   sb += "<IMETHODCALL NAME=\"References\">"
          "<LOCALNAMESPACEPATH>";
   for (int i=0,s=url->ns.size(); i<s; i++)
      sb += "<NAMESPACE NAME=\"" + url->ns[i] + "\"></NAMESPACE>";
   sb += "</LOCALNAMESPACEPATH>\n";

   sb = sb + "<IPARAMVALUE NAME=\"ObjectName\">"
          "<INSTANCENAME CLASSNAME=\"" + url->cName + "\">";

   url->toStringBuffer(sb);

   sb += "</INSTANCENAME></IPARAMVALUE>\n";

   if (resultClass!=NULL)
      sb = sb +"<IPARAMVALUE NAME=\"ResultClass\"><CLASSNAME NAME=\"" + resultClass + 
	"\"/></IPARAMVALUE>\n";
   if (role)
      sb = sb + "<IPARAMVALUE NAME=\"Role\"><VALUE>" + role + "</VALUE></IPARAMVALUE>\n";

   emitqualorigin();

   if (attr) {
      AList *al=new AList(attr);
      sb += "<IPARAMVALUE NAME=\"PropertyList\"><VALUE.ARRAY>";
      for(int i=0; i<al->attrs; i++)
           sb = sb + "<VALUE>" + al->attrv[i] + "</VALUE>";
      sb += "</VALUE.ARRAY></IPARAMVALUE>\n";
      delete al;
   }

   sb += "</IMETHODCALL>";
   sb += xmlTrailer;

   addPayload(sb);
   return getResponse();
}


InstanceNamesXml *Cimom::referenceNames(URL &url, char *resultClass, char *role) {
   string sb;
   string resp;
   SimplerspXml *rsp;
   RetValXml *rv;

   genRequest(url,"ReferenceNames");

   sb = xmlHeader;
   sb += "<IMETHODCALL NAME=\"ReferenceNames\">"
          "<LOCALNAMESPACEPATH>";
   for (int i=0,s=url.ns.size(); i<s; i++)
      sb = sb +"<NAMESPACE NAME=\"" + url.ns[i] + "\"></NAMESPACE>";
   sb += "</LOCALNAMESPACEPATH>\n";

   sb = sb + "<IPARAMVALUE NAME=\"ObjectName\">"
          "<INSTANCENAME CLASSNAME=\"" + url.cName + "\">";

   url.toStringBuffer(sb);

   sb += "</INSTANCENAME></IPARAMVALUE>\n";

   if (resultClass!=NULL)
      sb = sb + "<IPARAMVALUE NAME=\"ResultClass\"><CLASSNAME NAME=\"" + resultClass +
              "\"/></IPARAMVALUE>\n";
   if (role)
      sb = sb +"<IPARAMVALUE NAME=\"Role\"><VALUE>" + role + "</VALUE></IPARAMVALUE>\n";

   sb += "</IMETHODCALL>";
   sb += xmlTrailer;

   addPayload(sb);
   if ((resp=getResponse()) != "")
   {
   CimXml cx(resp);
   rsp=cx.getResponse();
   rv=rsp->getReturnData();

   if (!rv) return NULL;
   if (rv->getType()==TypeInstanceNames) return (InstanceNamesXml*)rv;
   if (rv->getType()==TypeInstanceName) 
	{
	  InstanceNamesXml * retval = new InstanceNamesXml(*(InstanceNameXml*)rv);
	  delete rv; 
	  return retval;
	}
   delete rv; 
   throw ParseException("Unexpected response type");
   }
   return NULL;
}



//   InstanceNamesXml 	*references(URL&, bool qual=true, bool origin=true);



/* ------------------ end of Associator Provider Interface -------------------- */

MethodresponseXml *Cimom::callMethod(URL &url, char *method, char *params)
{
   string resp;
   SimplerspXml *rsp;
   RetValXml *rv;

   resp=callMethod2str(&url, method, params);
   if ( resp != "" )
   {
      CimXml cx(resp);
      rsp=cx.getResponse();
      rv=rsp->getReturnData();

      if (rv->getType()==TypeMethodResponse) 
	return static_cast<MethodresponseXml*>(rv);
      delete rv;
      throw ParseException("Unexpected response type");
   }
   return NULL;
}

string Cimom::callMethod2str(URL *url, char *method, char *params)
{
   int i, s, j, t;
   string sb;
   string out;
   ClassXml *cls = NULL;
   bool instance = (url->keys.size() > 0);

   if (!(cls = getClass(*url)))
      throw CmdException("Invalid class");

   if (instance) {
      if (!cls->keysOk(*url))
         throw CmdException("Bad key specification");
   }

   genRequest(*url, method, true, instance);

   sb = xmlHeader;
   sb = sb + "<METHODCALL NAME=\"" + method + "\">";

   if (instance) {
      sb += "<LOCALINSTANCEPATH>";
   } else {
      sb += "<LOCALCLASSPATH>";
   }

   sb += "<LOCALNAMESPACEPATH>";
   for (i=0,s=url->ns.size(); i<s; i++) {
      sb = sb + "<NAMESPACE NAME=\"" + url->ns[i] + "\"/>";
   }
   sb += "</LOCALNAMESPACEPATH>";

   if (instance) {
      sb = sb + "<INSTANCENAME CLASSNAME=\"" + url->cName + "\">";
      url->toStringBuffer(sb);
      sb += "</INSTANCENAME></LOCALINSTANCEPATH>";
   } else {
      sb = sb + "<CLASSNAME NAME=\"" + url->cName + "\"/></LOCALCLASSPATH>";
   }

   if (params && cls) {
     MethodArray &methods = cls->getMethods();
     for (i=0,s=methods.size(); i<s; i++) {
       MethodXml &m = methods[i];
       if (strcasecmp(m.name.c_str(),method)==0) {
	 ParameterArray &param = m.getParameters();
	 AVPs avp(params,param);
	 for ( i = 0, s = avp.size() ; i < s ; i++ ) {
	   AValue *av = avp.get(i);
	   for (j=0,t=param.size(); j<t; j++) {
	     ParameterXml &p=param[j];
	     if (strcasecmp(p.name.c_str(),av->attr.c_str())==0) {
	       sb = sb + "<PARAMVALUE NAME=\"" + av->attr + "\">";
	       p.toXmlBuffer(sb,av,1,url);
	       sb += "</PARAMVALUE>";
	     }
	   }
	 }
	 break;
       }
     }
   }

   sb += "</METHODCALL>";
   sb += xmlTrailer;

   addPayload(sb);
   return getResponse();
}

ValueXml *Cimom::getProperty(URL &url, char *attr)
{
   string sb;
   string resp;
   SimplerspXml *rsp;
   RetValXml *rv;

   genRequest(url,"GetProperty");

   sb = xmlHeader;
   sb += "<IMETHODCALL NAME=\"GetProperty\">"
          "<LOCALNAMESPACEPATH>";

   for (int i=0,s=url.ns.size(); i<s; i++)
      sb = sb + "<NAMESPACE NAME=\"" + url.ns[i] + "\"></NAMESPACE>";
   sb += "</LOCALNAMESPACEPATH>\n";

   sb = sb + "<IPARAMVALUE NAME=\"PropertyName\"><VALUE>" + attr + "</VALUE></IPARAMVALUE>";

   sb = sb +"<IPARAMVALUE NAME=\"InstanceName\">"
          "<INSTANCENAME CLASSNAME=\"" + url.cName + "\">";

   url.toStringBuffer(sb);

   sb += "</INSTANCENAME></IPARAMVALUE>\n</IMETHODCALL>";

   sb += xmlTrailer;

   addPayload(sb);
   if ((resp=getResponse()) != "")
   {
   CimXml cx(resp);
   rsp=cx.getResponse();
   rv=rsp->getReturnData();

   if (rv->getType()==TypeRValue) return (ValueXml*)rv;
   delete rv;
   throw ParseException("Unexpected response type");
   }
   return NULL;
}

void Cimom::setProperty(URL &url, ClassXml *cls, char *attr)
{
   int i, s;
   string sb;
   string resp;
   SimplerspXml *rsp;
   RetValXml *rv;
   AVPs *avp=AVPs::makeAVPs(attr,1);
   PropertyArray &props=cls->getProperties();
   PropertyXml *prop=NULL;

   if (avp==NULL)
      throw CmdException("Invalid or missing prop-value specification");
   AValue *a=avp->get((unsigned int)0);

   for (i=0,s=props.size(); i<s; i++) {
      PropertyXml &p=props[i];
      if (strcasecmp(p.name.c_str(),a->attr.c_str())==0) {
         prop=&p;
      }
   }
   if (prop==NULL)
      throw CmdException("Unknown property");

   genRequest(url,"SetProperty");

   sb = xmlHeader;
   sb += "<IMETHODCALL NAME=\"SetProperty\">"
          "<LOCALNAMESPACEPATH>";

   for (i=0,s=url.ns.size(); i<s; i++)
      sb = sb + "<NAMESPACE NAME=\"" + url.ns[i] + "\"></NAMESPACE>";
   sb += "</LOCALNAMESPACEPATH>\n";

   sb = sb + "<IPARAMVALUE NAME=\"PropertyName\"><VALUE>" + a->attr + "</VALUE></IPARAMVALUE>\n";
   sb += "<IPARAMVALUE NAME=\"NewValue\">";
   prop->toXmlBuffer(sb,a,1);
   sb += "</IPARAMVALUE>\n";

   sb = sb + "<IPARAMVALUE NAME=\"InstanceName\">"
          "<INSTANCENAME CLASSNAME=\"" + url.cName + "\">";

   url.toStringBuffer(sb);

   sb += "</INSTANCENAME></IPARAMVALUE>\n</IMETHODCALL>";

   sb += xmlTrailer;

   addPayload(sb);
   if ((resp=getResponse()) == "")
   {
      throw ParseException("Unable to get response");
   }

   CimXml cx(resp);
   rsp = cx.getResponse();
   rv  = rsp->getReturnData();

   if (!rv) return;
   if (rv->getType()==TypeNull) {delete rv; return;}
   delete rv;
   throw ParseException("Unexpected response type");

   return;
}

/* ---------------------------------------------------------------------------- */



string Cimom::unescapeXml(const char *m, int nq)
{
#define REPL(str,chrs) if(strncasecmp(p,str,strlen(str))==0) \
 { strcpy (q,(chrs)); \
   q+=strlen(chrs); \
   p+=strlen(str); }

  const char *quotereplace = nq ? "\\\"" : "\"";
  const char *backslashreplace = nq ? "\\\\" : "\\";
  const char *p;
  char *q, *buf;
  int i = 0;

  for (p=m; *p; p++) {
      if (*p == '\\')
          i++;
  }

  buf = (char *) alloca(strlen(m) + (i * (nq ? 1 : 0)) + 1);

  for(p=m,q=buf; *p;)
    REPL("\\", backslashreplace)
    else if(*p!='&') { *q++ = *p++; }
      else {
      REPL("&gt;",">")
      else REPL("&lt;","<")
      else REPL("&quot;",quotereplace)
      else REPL("&amp;","&")
      else REPL("&apos;","'")
      else *q++ = *p++;
      }
  *q = '\0';
  return string(buf);
}
