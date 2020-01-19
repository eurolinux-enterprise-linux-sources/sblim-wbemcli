/*
 * $Id: CimXml.h,v 1.38 2011/06/22 05:33:02 tyreld Exp $
 *
 * CimXml.h
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
 *
 * Description: Line command interface to DMTF conforming WBEM servers
*/


#ifndef CIMXML_H
#define CIMXML_H

#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>
#include <iostream>

using namespace std;

extern int dumpXml;
extern char *strndup(char*,int);

#define ASSOC_PARMS ((((long)assocClass)|((long)resultClass)|((long)role)|((long)resultRole))!=0)
//#define BAD_ASSOC_NAMES_PARMS ((((long)role)|((long)resultRole))!=0)
#define BAD_REFERENCES_PARMS ((((long)assocClass)|((long)resultRole))!=0)

typedef struct cimErrorData {
    string mStatusDescription;
    int mStatusCode;
    string mError;
} CimErrorData;

class StringArray : public vector<string> {
  typedef vector<string>::size_type size_type;
  public:
   StringArray() : vector<string>() {}
   StringArray(int n) : vector<string>(n) {}
   StringArray(int n, const string& s) : vector<string>(n,s) {}
   void add(const string& t) { push_back(t); }
   void toStringBuffer(string &sb, const char *sep=" ",const char *q="") {
     for (size_type n=0; n<size(); n++) {
       sb = sb + (*this).at(n) + q;
       if (n!=size()-1) sb += sep;
     }}
#if defined(GCC_VERSION) && GCC_VERSION < 3000
   string& at(int i) {
     return (*this)[i];
   }
#endif
};

struct XmlElement {
  StringArray attr;
  XmlElement() : attr(0) {};
  XmlElement(int n) : attr(n) {}
  XmlElement(int n, string foo) : attr(n, foo) {}
};

struct XmlAttr {
   char eTagFound;
   StringArray attr;
};


class XmlBuffer {
   char *base;
   char *last;
   XmlBuffer(const XmlBuffer& x);
  public:
   char *cur;
   XmlBuffer(string s);
   ~XmlBuffer() { free(base); }
   void skipWS();
   int getChar(const char c);
   int getChar(const char *s);
   int getWord(const char *, int xCase=1);
   char *nextTag();
   int nextEquals(const char*,const char*);
   int xmlHdr();
   int tagEquals(const char*);
   int endTag(const char*, const XmlAttr *a=NULL);
   int attrsOk(const XmlElement&,XmlAttr&,const char* tag);
   string getValue(const char *v=NULL);
   const char *getContent(const XmlAttr*);
   int skipElement(const char*);
   int testAttr(const char*,const char*,int);
};

class DoesNotExist {
  public:
  string desc; 
  DoesNotExist(string str) : desc(str) {}
};

template<class T> class Option {
#define WHICHCLASS(t) "Object of type "#t "not found..."
  bool e;
  T t;
 public:
  Option() : e(false) {}
  Option(const T& s) : e(true), t(s) {}
  bool exists() {return e;}
  T& val() {
    if(e) return t; 
    else throw DoesNotExist(WHICHCLASS(T));
  }
};

template<class T> class ArrayXml : public vector<T> {
  typedef typename vector<T>::size_type size_type;
  public:
   ArrayXml() {}
   ArrayXml<T> * clone() const { return new ArrayXml<T>(*this);}
   void add(const T& t) { push_back(t); }
#if !defined(GCC_VERSION) || GCC_VERSION >= 3000
   T& operator[] (size_type n) {return at(n);}
   const T& operator[] (size_type n) const {return at(n);}
#endif
   T *get(int n)    { return &(*this)[n]; }
   void toStringBuffer(string &sb, const char *sep=" ",const char *q="") {
     for (size_type n=0; n<this->size(); n++) {
       (*this).at(n).toStringBuffer(sb,q);
       if (n!=this->size()-1) sb += sep;
     }}
#if defined(GCC_VERSION) && GCC_VERSION < 3000
   T& at(int i) {
     return (*this)[i];
   }
#endif
};

template<class T>
class ArrayIterator  {
public:
  int it;
  ArrayXml<T> * c;
  ArrayIterator() : it(0), c(0) {}
  ~ArrayIterator() {}
  ArrayIterator(const ArrayIterator&otit) : it(otit.it), c(otit.c) {}
  ArrayIterator(int anit, ArrayXml<T> * cont) 
                    : it(anit), c(cont) {}
  ArrayIterator __iter__() {return *this;}
  T * next() { return &((*c).at(it++)); }
};

#define LocalInstancePathXml LocalInstancepathValueXml
#define InstancePathXml InstancepathValueXml

class ClassNameXml;
class MethodXml;
class QualifierXml;

class ValueXml;
class PropertyXml;
class ParameterXml;
class InstanceNameXml;
class InstancePathXml;
class InstanceXml;
class KeyBindingXml;
class ClassXml;
class NamedInstanceXml;
class ObjectWithPathXml;
class NameSpaceXml;
class URL;
class AValue;
class ParamvalueXml;

class QualifierXml {
  public:
   string str;
   int key;
   QualifierXml(XmlBuffer&);
};

typedef ArrayXml<QualifierXml>     QualifierArray;
typedef ArrayXml<PropertyXml>      PropertyArray;
typedef ArrayXml<ParameterXml>     ParameterArray;
typedef ArrayXml<MethodXml>        MethodArray;
typedef ArrayXml<ValueXml>         ValueArray;
typedef ArrayXml<InstanceNameXml>  InstanceNameArray;
typedef ArrayXml<InstancePathXml>  InstancePathArray;
typedef ArrayXml<InstanceXml>      InstanceArray;
typedef ArrayXml<KeyBindingXml>    KeyBindingArray;
typedef ArrayXml<ClassNameXml>     ClassNameArray;
typedef ArrayXml<ClassXml>         ClassArray;
typedef ArrayXml<void>             VoidArray;
typedef ArrayXml<ObjectWithPathXml> ObjectWithPathArray;
typedef ArrayXml<NamedInstanceXml> NamedInstanceArray;
typedef ArrayXml<NameSpaceXml>     NameSpaceArray;
typedef ArrayXml<AValue>           AValueArray;
typedef ArrayXml<ParamvalueXml>    ParamvalueArray;


class AValue {
  public:
   string attr, val;
   StringArray vals;
   AValue(const char *a, const char *v) { attr=a; val=v; }
   void add(const char *v) {
     if(vals.size()==0)
       vals.push_back(string(val));
     val="";
     vals.push_back(string(v)); }
};

class AVPs {
   AValueArray avals;
   char type[256];
   char *all;
   void parseRefList(char *name, char *s, bool isArray=false);
  public:
   AVPs(char*);
   AVPs(char*,PropertyArray&);
   AVPs(char*,ParameterArray&);
   ~AVPs() { if(all) free(all); }
   static AVPs *makeAVPs(char *s, unsigned int n);
   AValue* get(unsigned int n);
   AValue* get(const char *a);
   int check(const char *a);
   Option<string> notGotten();
   int size() { return avals.size(); }
};

class AList {
  public:
   AList(char*);
   ~AList();
  char **attrv;
   char *attrl;
   int attrs;
};

class XmlException {
  protected:
   void set(const string& t, const string& m) { msg=t+m; }
   void set(const string& t, const string& c, const string& m) { msg = t+"("+c+") "+m; }
  public:
   string msg;
   const string& getMsg() { return msg; }
};

class URLException : public XmlException {
  public:
   URLException(const string& m) { set("Url Exception: ",m); }
};

class ParseException : public XmlException {
  public:
   ParseException(const string& m) { set("Parse Exception: ",m); }
};

class EmptyValue : public XmlException {
};

class NoReturnValue : public XmlException {
};

class HttpException : public XmlException {
  public:
   HttpException(const string& m) { set("Http Exception: ",m); }
};

class CmdException : public XmlException {
  public:
   CmdException(const string& m) { set("Cmd Exception: ",m); }
};

class ErrorXml : public XmlException {
   static XmlElement elm;
   static bool initialized;
   void initialize();
  public:
   string desc, code;
   ErrorXml(XmlBuffer&);
   ErrorXml(int code, string &desc);
};


class KeyValueXml {
   static XmlElement elm;
   static bool initialized;
  public:
   string valType, val;
   KeyValueXml(XmlBuffer&);
   KeyValueXml(const char*);
   KeyValueXml() {}
   KeyValueXml& operator=(const KeyValueXml& src) {if(this!=&src) {valType=src.valType; val = src.val;} return *this;}
   void toStringBuffer(string&,const char*);
};


class NameSpaceXml {
   static XmlElement elm;
  public:
   string name;
   NameSpaceXml() {}
   NameSpaceXml(XmlBuffer&);
   void toStringBuffer(string&,const char*);
};

class HostXml {
   static XmlElement elm;
  public:
   string name;
   HostXml() {}
   HostXml(XmlBuffer&);
};

class LocalNameSpacePathXml : public NameSpaceArray {
   static XmlElement elm;
  public:
   LocalNameSpacePathXml() {}
   LocalNameSpacePathXml(XmlBuffer&);
};

class NameSpacePathXml {
   static XmlElement elm;
  public:
   HostXml host;
   LocalNameSpacePathXml lns;
   NameSpacePathXml() {}
   NameSpacePathXml(XmlBuffer&);
   void toStringBuffer(string &sb, const char *q="");
};

class SubValueXml {
  public:
  virtual ~SubValueXml() {}
#ifndef WIN32
   virtual SubValueXml * clone()=0;
#else
   SubValueXml * clone() { return new SubValueXml(*this); }
#endif
   int sType;
#define TypeSubValueClassname 1
#define TypeSubValueLocalInstancepath 2
#define TypeSubValueInstancename 3
#define TypeSubValueInstancepath 4
#define TypeSubValueClasspath 5
#define TypeSubValueLocalClasspath 6
   virtual void toStringBuffer(string &sb, const char *q) { sb += ">>Not Supported>>"; }
};

class RetValXml {
  public:
   virtual ~RetValXml(){}
   int type;
#define TypeNull           0
#define TypeInstance       1
#define TypeInstances      2
#define TypeClass          3
#define TypeInstanceName   4
#define TypeInstanceNames  5
#define TypeClassName      6
#define TypeClassNames     7
#define TypeClasses        8
#define TypeNamedInstance  9
#define TypeNamedInstances 10
#define TypeObjectWithPath 11
#define TypeObjectsWithPath 12
#define TypeRValue         13
#define TypeMethodResponse 14
#ifndef WIN32
   virtual RetValXml * clone()=0;
#else
   virtual RetValXml * clone() { return new RetValXml(*this); }
#endif
   int getType() { return type; }
   virtual void toStringBuffer(string &sb, const char *q="") { sb += "** not implemented **"; }
};

class ValueXml : public RetValXml {
   static XmlElement elm,elmRef,elmArray;
   string val;
   SubValueXml *subVal;
   ValueArray *valArray;
   int vType;
#define TypeValue 1
#define TypeValueArray 2
#define TypeValueRef 3
#define TypeValueRefArray 4
#define TypeValueObject 5
#define TypeValueNamedInstance 6
#define TypeValueNamedObject 7
#define TypeValueObjectWithPath 8
#define TypeValueObjectWithLocalPath 9
  public:
   ValueXml() : subVal(0), valArray(0), vType(TypeValue) {}
   ~ValueXml();
   ValueXml(XmlBuffer&);
   ValueXml(const char*);
   ValueXml(const ValueXml& src);                       // copy constructor
   ValueXml& operator= (const ValueXml&); // assignment
   ValueXml * clone() { return new ValueXml(*this);}
   string getValue() { return val; }
   void toXmlBuffer(string &xb);
   virtual void toStringBuffer(string&,const char* q="");
};

class KeyBindingXml {
   static XmlElement elm;
   enum { INVALID, KEYVAL, VALREF } which;
   KeyValueXml keyVal;
   ValueXml valRef;
  public:
   string name;
   KeyBindingXml() : which(INVALID), name(0) {}
   KeyBindingXml(XmlBuffer&);
   void init(XmlBuffer&);
   KeyBindingXml(const char*,const char*);
   void toStringBuffer(string&,const char*s="");
};


class InstanceNameXml : public RetValXml {
   static XmlElement elm;
   string clsName;
   string nspace;
  public:
   KeyBindingArray keys;
   InstanceNameXml() {}
   InstanceNameXml(XmlBuffer&, char op=0);
   InstanceNameXml * clone() { return new InstanceNameXml(*this);}
   void init(XmlBuffer&, char op);
   virtual void toStringBuffer(string&,const char *q="");
};

class InstanceNamesXml : public RetValXml, public InstanceNameArray {
  public:
  InstanceNamesXml(InstanceNameXml& n) { type=TypeInstanceNames; add(n); }
  virtual InstanceNamesXml * clone() { return new InstanceNamesXml(*this);}
};


class InstancepathValueXml : public SubValueXml {
  protected:
   static XmlElement elm;
  public:
   string nspace;
   NameSpacePathXml nsp;
   InstanceNameXml name;
   InstancepathValueXml() {}
   InstancepathValueXml(XmlBuffer&);
   InstancepathValueXml * clone() { return new InstancepathValueXml(*this); }
   void init(XmlBuffer&);
   virtual void toStringBuffer(string&,const char*);
};

/*
class InstancenameValueXml : public SubValueXml {
  public:
   string instName;
   InstancenameValueXml(XmlBuffer&);
};
*/
class InstancenameValueXml : public SubValueXml {
  protected:
   static XmlElement elm;
  public:
   InstanceNameXml name;
   InstancenameValueXml() {}
   InstancenameValueXml(XmlBuffer&);
   InstancenameValueXml * clone() { return new InstancenameValueXml(*this); }
   void init(XmlBuffer&);
   virtual void toStringBuffer(string&,const char*);
};

class ClassnameValueXml : public SubValueXml {
   static XmlElement elm;
  public:
   string clsName;
   ClassnameValueXml * clone() { return new ClassnameValueXml(*this); }
   ClassnameValueXml(XmlBuffer&);
};

class LocalInstancepathValueXml : public InstancepathValueXml {
  public:
   LocalNameSpacePathXml lnsp;
   LocalInstancepathValueXml() {}
   LocalInstancepathValueXml(XmlBuffer&);
   LocalInstancepathValueXml * clone() { return new LocalInstancepathValueXml(*this); }
   void init(XmlBuffer&);
   virtual void toStringBuffer(string&,const char*);
};

class PropertyXml {
   static XmlElement elm,elmRef,elmArray;
   static bool initialized;
   void initialize();
   void init(XmlBuffer&);
   void init(char*,int);
  public:
   string name, type, origin, propagated, refCls, arraySize;
   int pType;
   string str;
   int eprop, lprop;
#define TypeProperty 1
#define TypePropertyRef 2
#define TypePropertyArray 3
   QualifierArray qualifiers;
   Option<ValueXml> value;
   int key;
   // so we can vector-ize PropertyXml's, we need a default constructor
   PropertyXml() : pType(TypeNull) {}
   PropertyXml(XmlBuffer&);
   PropertyXml(char*,int);
   void addQualifier(QualifierXml& q) { qualifiers.add(q); }
   void toStringBuffer(string&,const char*);
   void toXmlBuffer(string&,AValue*,int vOnly=0,URL* url=NULL);
};

class ParameterXml {
   static XmlElement elm,elmRef,elmArray, elmRefArray;
   static bool initialized;
   void initialize();
   void init(XmlBuffer&);
   void refToXmlBuffer(string &xb, const char *s, URL *url);
  public:
   string name, type, refCls, arraySize;
   int pType;
   string str;
   int eprop, lprop;
#define TypeParameter 1
#define TypeParameterRef 2
#define TypeParameterArray 3
#define TypeParameterRefArray 4
   QualifierArray qualifiers;
   Option<ValueXml> value;
   int key;
   // so we can vector-ize PropertyXml's, we need a default constructor
   ParameterXml() : pType(TypeNull) {}
   ParameterXml(XmlBuffer&);
   //   ParameterXml(char*,int);
   void addQualifier(QualifierXml& q) { qualifiers.add(q); }
   void toStringBuffer(string&,const char*);
   void toXmlBuffer(string&,AValue*,int vOnly=0,URL* url=NULL);
};

class MethodXml {
   static XmlElement elm;
   static bool initialized;
   void   init(XmlBuffer&);
   QualifierArray qualifiers;
   ParameterArray parameters;
  public:
   string name;
   string str;
   MethodXml() : name("") {}
   MethodXml(XmlBuffer&);
   //   MethodXml * clone()    { return new MethodXml(*this); }
   QualifierArray &getQualifiers()        { return qualifiers; }
   ParameterArray &getParameters()        { return parameters; }
   void addQualifier(QualifierXml& q)     { qualifiers.add(q); }
   void addParameter(ParameterXml& p)     { parameters.add(p); }
   //   void toStringBuffer(string&,char*);
};

class ClassXml : public RetValXml {
   static XmlElement elm;
   static bool initialized;
   QualifierArray qualifiers;
   PropertyArray  keys;
   PropertyArray  properties;
   MethodArray    methods;
  public:
   string clsName;
   string superClass;
   ClassXml() : clsName(""), superClass("") {}
   ClassXml(XmlBuffer&);
   void init(XmlBuffer&);
   ClassXml * clone() { return new ClassXml(*this);}
   PropertyArray &getProperties()        { return properties;  }
   QualifierArray &getQualifiers()       { return qualifiers;  }
   PropertyArray &getKeys()              { return keys;        }
   MethodArray &getMethods()             { return methods;     }
   void addQualifier(QualifierXml& q)    { qualifiers.add(q);  }
   void addProperty(PropertyXml& p)      { properties.add(p); if (p.key) keys.add(p); }
   void addMethod(MethodXml& m)          { methods.add(m);     }
   virtual void toStringBuffer(string&,const char *q="");
   int keysOk(URL&);
};

class ClassesXml : public RetValXml, public ClassArray {
  public:
   virtual ClassesXml * clone() { return new ClassesXml(*this);}
   ClassesXml(ClassXml *n) { type=TypeClasses; add(*n); }
};

class InstanceXml : public RetValXml {
  protected:
   static XmlElement elm;
   string clsName;
   QualifierArray qualifiers;
   PropertyArray keys;
   PropertyArray properties;
   ClassXml ofClass;
  public:
   InstanceXml() {}
   InstanceXml(XmlBuffer&);
   InstanceXml * clone() { return new InstanceXml(*this);}
   ~InstanceXml() {}
   void init(XmlBuffer&);
   void propagateKeyFlags(PropertyArray& keys);
   void propagateKeyFlags(KeyBindingArray& keys);
   PropertyArray &getProperties()     { return properties; }
   QualifierArray &getQualifiers()    { return qualifiers; }
   void addQualifier(QualifierXml& q) { qualifiers.add(q); }
   void addProperty(PropertyXml& p)   { properties.add(p); }
   virtual void toStringBuffer(string&,const char *p="");
};

class NamedInstanceXml : public RetValXml {
   static XmlElement elm;
  public:
   InstanceNameXml instname;
   InstanceXml inst;
   NamedInstanceXml() {}
   NamedInstanceXml * clone() { return new NamedInstanceXml(*this);}
   NamedInstanceXml(XmlBuffer&);
   void init(XmlBuffer&);
};

class InstancesXml : public RetValXml, public InstanceArray {
  public:
   virtual InstancesXml * clone() { return new InstancesXml(*this);}
   InstancesXml(InstanceXml& n) 	{ type=TypeInstances; add(n); }
};

class ObjectWithPathXml : public RetValXml {
   static XmlElement elm;
  public:
   InstancePathXml *ip;
   InstanceXml inst;
   ObjectWithPathXml() {}
   ObjectWithPathXml(XmlBuffer&);
   void init(XmlBuffer&);
   ObjectWithPathXml * clone() { return new ObjectWithPathXml(*this);}
};

class ObjectsWithPathXml : public RetValXml {
  public:
   InstanceArray ia;
   InstancePathArray ipa;
   ObjectsWithPathXml(ObjectWithPathXml& n);
   ObjectsWithPathXml * clone() { return new ObjectsWithPathXml(*this);}
   void add(ObjectWithPathXml& n);
   InstancePathXml& getName(int n)  { return ipa.at(n); }
   InstanceXml& getInst(int n)  { return ia.at(n); }
   int size()                   { return ipa.size(); }
};


class NamedInstancesXml : public RetValXml {
  public:
   InstanceArray ia;
   InstanceNameArray ina;
   NamedInstancesXml(NamedInstanceXml *n);
   NamedInstancesXml * clone() { return new NamedInstancesXml(*this);}
   void add(NamedInstanceXml& n);
   InstanceNameXml& getName(int n)  { return ina.at(n); }
   InstanceXml& getInst(int n)  { return ia.at(n); }
   int size()                   { return ina.size(); }
};


class ClassNameXml : public RetValXml {
   static XmlElement elm;
   string name;
  public:
   ClassNameXml() {}
   ClassNameXml(XmlBuffer&);
   ClassNameXml * clone() { return new ClassNameXml(*this);}
   void init(XmlBuffer&);
   virtual void toStringBuffer(string &sb, const char *q="");
};


class ClassNamesXml : public RetValXml, public ClassNameArray {
  public:
   virtual ClassNamesXml * clone() { return new ClassNamesXml(*this);}
   ClassNamesXml(ClassNameXml *n) { type=TypeClassNames; add(*n); }
};

class ParamvalueXml : public RetValXml {
  static XmlElement elm;
  static bool initialized;
  static void initialize();
  string    name;
  string    type;
  ValueXml *val;
 public:
  ParamvalueXml();
  ParamvalueXml(XmlBuffer&);
  ParamvalueXml(const ParamvalueXml&);
  ~ParamvalueXml() {delete val;}
  ParamvalueXml *clone() {return new ParamvalueXml(*this);}
  virtual void toStringBuffer(string &sb, const char *q="");
};

class IReturnvalueXml {
   static XmlElement elm;
   RetValXml *rv;
  public:
   IReturnvalueXml(XmlBuffer&);
   IReturnvalueXml(const IReturnvalueXml &src);         // copy constructor
   IReturnvalueXml& operator= (const IReturnvalueXml&); // assignment
   IReturnvalueXml * clone() { return new IReturnvalueXml(*this);}
   ~IReturnvalueXml() {delete rv;}
   RetValXml *getReturnData() {
       if (rv) return rv->clone();
       return NULL;
   }
};

class ReturnvalueXml {
   static XmlElement elm;
   RetValXml *rv;
  public:
   ReturnvalueXml(XmlBuffer&);
   ReturnvalueXml(const ReturnvalueXml &src);         // copy constructor
   ReturnvalueXml& operator= (const ReturnvalueXml&); // assignment
   ReturnvalueXml * clone() { return new ReturnvalueXml(*this);}
   ~ReturnvalueXml() {delete rv;}
   RetValXml *getReturnData() {
       if (rv) return rv->clone();
       return NULL;
   }
};

class ResponseXml {
 public:
  virtual ~ResponseXml() {};
  virtual RetValXml *getReturnData() = 0;
};

class IMethodresponseXml : public ResponseXml {
   static XmlElement elm;
   string name;
   IReturnvalueXml *irv;
   ErrorXml *err;
  public:
   IMethodresponseXml(XmlBuffer&, int noIRET=0);
   ~IMethodresponseXml() {delete irv; delete err;}
   virtual RetValXml *getReturnData() {
     if (err) throw ErrorXml(*err);
     return irv ? irv->getReturnData() : 0;
   }
};

class MethodresponseXml : public RetValXml,  public ResponseXml {
   static XmlElement elm;
   string name;
   ReturnvalueXml  *rvl;
   ParamvalueArray *pvll;
   ErrorXml *err;
  public:
   MethodresponseXml(XmlBuffer&, int noRET=0);
   MethodresponseXml(const MethodresponseXml &);
   ~MethodresponseXml() {delete rvl; delete pvll; delete err;}
   MethodresponseXml *clone() {return new MethodresponseXml(*this);};
   virtual void toStringBuffer(string&,const char *q="");
   virtual RetValXml *getReturnData() {
     if (err) throw ErrorXml(*err);
     return clone();
   }
};

class SimplerspXml {
   static XmlElement elm;
   ResponseXml *irsp;
  public:
   SimplerspXml(XmlBuffer&, int noIRET = 0);
   ~SimplerspXml();
   RetValXml *getReturnData() {
      RetValXml *rv=irsp ? irsp->getReturnData() : 0;
      if (rv) return rv;
      throw EmptyValue();
   }
};

class MessageXml {
   static XmlElement elm;
   static bool initialized;
   string id, protocol;
   SimplerspXml *srq;
  public:
   MessageXml(XmlBuffer&, int noIRET = 0);
   ~MessageXml();
   SimplerspXml *getResponse() { return srq; }
};

class CimXml {
   static XmlElement elm;
   static bool initialized;
   string cimVersion, dtdVersion;
   XmlBuffer *xb;
   MessageXml *msg;
  public:
   CimXml(string in, int noIRET = 0);
   ~CimXml() {delete xb; delete msg;}
   SimplerspXml *getResponse() { return msg->getResponse(); }
};


class URL {
  public:
   string scheme;
   string host;
   string port;
   string cName;
   string user;
   string password;
   StringArray ns;
   PropertyArray keys;
   URL(const char*);
   char *resolveIndirection(char *ind, char *u);
   int  refLookAhead(char*,char**);
   void toStringBuffer(string&);
   void toStringBuffer(string&,int nsOnly);
   void toStringBufferGI(string&);
};

#define INCLUDEQUAL false

class Cimom {
 private:
  static char xmlHeader[];
  static char xmlTrailer[];
 protected:
  virtual bool supportsSSL() { return false; };
  virtual void genRequest(URL&,const char*,bool cls=false,bool keys=false)=0;
  virtual void addPayload(const string& pl)=0;
  virtual string getResponse()=0;
  char *newstr;
public:
  virtual ~Cimom() {}
   ClassXml		*getClass(URL&, char *attr=NULL,
   				bool qual=true, bool origin=true, bool local=false);
   string               getClass2str(URL*, char *attr=NULL,
   				bool qual=true, bool origin=true, bool local=false);
   int			testInst(URL&,AValue*);
   InstanceXml 		*getInstance(URL&, char *attr=NULL, bool qual=INCLUDEQUAL,
                             bool origin=true, bool local=false);
   NamedInstancesXml 	*enumInstances(URL&, char *attr=NULL, bool deep=true, bool qual=INCLUDEQUAL,
                                       bool origin=true, bool local=false);
   string               enumInstances2str(URL*, char *attr=NULL, bool deep=true, bool qual=INCLUDEQUAL,
                           bool origin=true, bool local=false);
   InstanceNameXml 	*createInstance(URL&,ClassXml*,char*);
   InstanceNamesXml 	*enumInstanceNames(URL&);
   ClassesXml 		*enumClasses(URL&, bool deep=true, bool qual=INCLUDEQUAL,
                                 bool origin=true, bool local=false);
   ClassNamesXml 	*enumClassNames(URL&, bool deep=true);
   string            enumClassNames2str(URL*, bool deep=true);
   void                  modifyInstance(URL&,InstanceXml*,char*);

   /* Associator Provider Interface */
   InstanceNamesXml 	*associatorNames(URL&, char* assocClass,
                              char *resultClass, char *role, char *resultRole);
   ObjectsWithPathXml 	*associators(URL&, char *assocClass, char *resultClass,
                          char *role, char *resultRole, char *attr=NULL,
                          bool qual=INCLUDEQUAL, bool origin=true);
   string                associators2str(URL*, char* assocClass, char *resultClass,
                          char *role, char *resultRole, char *attr=NULL,
                          bool qual=INCLUDEQUAL, bool origin=true);
   InstanceNamesXml 	*referenceNames(URL&, char *resultClass, char *role);
   ObjectsWithPathXml	*references(URL&, char *resultClass, char *role, char *attr=NULL,
                                    bool qual=INCLUDEQUAL, bool origin=true);
   string               references2str(URL*, char *resultClass, char *role, char *attr=NULL,
                                    bool qual=INCLUDEQUAL, bool origin=true);
   /* end of Associator Provider Interface */

   string     callMethod2str(URL *url, char *method, char *params);
   MethodresponseXml    *callMethod(URL& url, char *method, char *params);

   /* Solaris 8 SMC Interface */
   string     enumClassesSMC(URL*, bool deep=true, bool qual=false);
#ifdef WIN32
   virtual bool SetTimeouts(int Connect = 60000,
                            int Send = 30000,
                            int Receive = 30000) {return false;}
#endif

   ValueXml		*getProperty(URL&,char*);
   void			setProperty(URL&,ClassXml*,char*);

   void                  deleteClass(URL&);
   void                  deleteInstance(URL&);
//   char			*getReferences(URL*, bool qual=false, bool origin=false);
   static string         unescapeXml(const char*,int nq=0);
   virtual void setClientCertificates(const char * cacert, 
				      int noverify,
				      const char * certificate,
				      const char * key) = 0;
   
   CimErrorData mErrorData;

};

#define GetInstance       1
#define CreateInstance    2
#define ModifyInstance    3
#define DeleteInstance    4
#define EnumInstanceNames 5
#define EnumInstances     6
#define GetClass          7
#define EnumClasses       8
#define EnumClassNames    9
#define DeleteClass      10
#define AssociatorNames  11
#define Associators      12
#define ReferenceNames   13
#define References       14
#define GetProperty      15
#define SetProperty      16
#define GetClassDef      17
#define CallMethod       18
#define CallMethodX      19


#endif	/* CIMXML_H */
