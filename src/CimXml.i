/*
 *
 * $Id: CimXml.i,v 1.3 2008/08/07 18:25:41 tyreld Exp $
 *
 * (C) Copyright IBM Corp. 2003, 2005, 2008
 *
 * THIS FILE IS PROVIDED UNDER THE TERMS OF THE ECLIPSE PUBLIC LICENSE
 * ("AGREEMENT"). ANY USE, REPRODUCTION OR DISTRIBUTION OF THIS FILE
 * CONSTITUTES RECIPIENTS ACCEPTANCE OF THE AGREEMENT.
 *
 * You can obtain a current copy of the Eclipse Public License from
 * http://www.opensource.org/licenses/eclipse-1.0.php
 *
 * Author:        Dave Eger <dteger@us.ibm.com>
 * Contributors:  
 *
 * Description: CMPI Wrapper Implementation
 *
 */

%module CimXml

%{

#include <stdexcept>
#include <cerrno>
#include <string>
#include <vector>

#include "Python.h"
#include "CimXml.h"

#ifdef LIBWWW
#else
#include "Cimghttp.h"
#endif

%}

%include "std_vector.i"
/* %include "std_string.i" */

using namespace std;

/* Convert from C++ --> Python */
%typemap(out) string {
  $result = PyString_FromString($1.c_str());
  }

/* Convert from Python --> C++ */
%typemap(in) string {
  $1 = PyString_AsString($result);
   if (PyErr_Occurred()) return NULL;
  } 

%extend string {
  string __str__() { return *self; }
};

%exception {
   try {
      $action
   } catch (XmlException e) {
      PyErr_SetString(PyExc_RuntimeError, e.getMsg().c_str());
      return NULL;
   } catch (std::range_error e) {
      PyErr_SetString(PyExc_StopIteration, "Array index out of bounds");
      return NULL;
   } catch (std::out_of_range e) {
      PyErr_SetString(PyExc_StopIteration, "Array index out of bounds");
      return NULL;
   } catch (DoesNotExist e) {
      PyErr_SetString(PyExc_RuntimeError, e.desc.c_str());
      return NULL;
   } catch(...) {
     PyErr_SetString(PyExc_RuntimeError, "Indeterminate CIM Exception");
     return NULL;
   }
}


extern
class XmlBuffer {
  public:
   char *cur;
   XmlBuffer(string s);
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
   char *getContent(const XmlAttr*);
   int skipElement(const char*);
   int testAttr(char*,char*,int);
};


template<class T> class ArrayXml : public std::vector<T> {
  public:
%rename(__len__) size;
   ArrayXml() {}
   void add(const T& t) { push_back(t); }
   void toStringBuffer(string &sb, char *sep=" ",char *q="") { 
     for (int n=0; n<next; n++) { 
       (*this)[n].toStringBuffer(sb,q); 
       if (n!=next-1) 
	 sb += sep; 
     }}
};

template<class T> 
class ArrayIterator  {
public:
  int it;
  ArrayXml<T> * c;
  ArrayIterator() : it(0), c(0) {}
  ArrayIterator(const ArrayIterator&otit) : it(otit.it), c(otit.c) {}
  ArrayIterator(int anit, ArrayXml<T> * cont) 
                    : it(anit), c(cont) {}
  ArrayIterator __iter__() {return *this;}
  T * next() {
    return &((*c)[it++]);
  }
};

%extend ArrayXml {
  void __setitem__(int key, T* val) {
    (*self)[key] = *val;
  }
  T * __getitem__(int key) {
    return &((*self)[key]); 
  }
   string __str__() { 
     string b;
     self->toStringBuffer(b,"\n"); 
     return b;
   }
   ArrayIterator<T> __iter__() { return ArrayIterator<T>(0, self); }
}; 


extern class ValueXml;
extern class QualifierXml;
extern class PropertyXml;
extern class MethodXml;
extern class InstanceNameXml;
extern class InstancePathXml;
extern class InstanceXml;
extern class KeyBindingXml;
extern class ClassNameXml;
extern class ClassXml;
extern class NamedInstanceXml;
extern class ObjectWithPathXml;
extern class NameSpaceXml;
extern class URL;
extern class AValue;

%define XML_TEMPLATE_WRAP(T)
typedef ArrayXml<T##Xml>         T##Array;
namespace std {
%template(T##Vector)             vector<T##Xml>;
};
%template(T##Array)              ArrayXml<T##Xml>;
typedef ArrayIterator<T##Xml>    T##Iterator;
%template(T##Iterator)           ArrayIterator<T##Xml>;
%enddef

/*typedef ArrayXml<MethodXml>        MethodArray;
  %template(MethodArray)             ArrayXml<MethodXml>; */
/* XML_TEMPLATE_WRAP(Qualifier) */
/* XML_TEMPLATE_WRAP(void) */
XML_TEMPLATE_WRAP(NameSpace)
/* XML_TEMPLATE_WRAP(AValue) */

namespace std {
%template(StringVector)            vector<string>;
};

class StringArray : public vector<string> {
  public:
     void add(const string& t);
     void toStringBuffer(string &sb, char *sep=" ",char *q="");
};

XML_TEMPLATE_WRAP(Property)
XML_TEMPLATE_WRAP(InstanceName)
XML_TEMPLATE_WRAP(Instance)
XML_TEMPLATE_WRAP(KeyBinding)
XML_TEMPLATE_WRAP(ClassName)
XML_TEMPLATE_WRAP(Class)
XML_TEMPLATE_WRAP(ObjectWithPath)
XML_TEMPLATE_WRAP(NamedInstance)

%extend ArrayXml<PropertyXml> {
  ValueXml* __getitem__(char *key) {
    PropertyArray::iterator i;
    for(i=self->begin(); i != self->end(); i++) {
      if(strcasecmp(i->name.c_str(),key)==0) {
	return &i->value.val();
      }
    }
    throw std::range_error(string("The key ")+key+"is not in the dictionary.");
  }
  /*  string __setitem__(string key, char * val) {
   * 
   *} */
  void __delitem__(string key, char * val) {
    PropertyArray::iterator i;
    for(i=self->begin(); i != self->end(); i++) {
      if(strcasecmp(i->name.c_str(),key.c_str())==0) {
	self->erase(i);
	break;
      }
    }
  }
  vector<string> keys() {
    PropertyArray::iterator i;
    vector<string> the_keys;
    for(i=self->begin(); i != self->end(); i++) {
      the_keys.push_back(i->name);
    }
    return the_keys;
  }
};

extern
class DoesNotExist {
  public:
  string desc; 
  DoesNotExist(string str) : desc(str) {}
};

template<class T> class Option {
  bool e;
  T t;
 public:
  Option() : e(false) {}
  Option(const T& s) : e(true), t(s) {}
  bool exists() {return e;}
  T& val() {
    if(e) return t; 
    else throw DoesNotExist("Object of type "##T##"not found...");
  }
};

%define OPTION_TEMPLATE_WRAP(T)
typedef Option<T>         T##Option;
%template(T##Option)      Option<T>;
%enddef

OPTION_TEMPLATE_WRAP(string)


extern
class MethodXml {
  public:
   MethodXml(XmlBuffer&);
};

extern
class QualifierXml {
  public:
   string str;
   int key;
   QualifierXml(XmlBuffer&);
};

extern
class AValue {
  public:
   string attr, val;
   StringArray vals;
   AValue(const char *a, const char *v);
   void add(const char *v);
};

extern
class AVPs {
 public:
   AVPs(char*);
   static AVPs *makeAVPs(char *s, unsigned int n);
   AValue* get(unsigned int n);
   AValue* get(const char *a);
   int check(const char *a);
   Option<string> notGotten();
};

extern
class AList {
  public:
   AList(char*);
   ~AList();
   char **attrv;
   char *attrl;
   int attrs;
};

extern
class XmlException {
  public:
  string getMsg();
};

extern
class URLException : public XmlException {
  public:
   URLException(const string& m);
};

extern
class ParseException : public XmlException {
  public:
   ParseException(const string& m);
};

extern
class HttpException : public XmlException {
  public:
   HttpException(const string& m);
};

extern
class CmdException : public XmlException {
  public:
  CmdException(const string& m);
};

extern
class ErrorXml : public XmlException {
  public:
   string desc, code;
   ErrorXml(XmlBuffer&);
};

extern
class KeyValueXml {
  public:
   string valType, val;
   KeyValueXml(XmlBuffer&);
   KeyValueXml(const char*);
   void toStringBuffer(string&,char*);
};

%extend KeyValueXml { string __str__() { string b; self->toStringBuffer(b,""); return b; }};

extern
class NameSpaceXml {
  public:
   string name;
   NameSpaceXml(XmlBuffer&);
   void toStringBuffer(string&,char*);
};

extern
class HostXml {
  public:
   string name;
   HostXml(XmlBuffer&);
};

extern
class LocalNameSpacePathXml : public NameSpaceArray {
  public:
   LocalNameSpacePathXml(XmlBuffer&);
};

extern
class NameSpacePathXml {
  public:
   HostXml host;
   LocalNameSpacePathXml lns;
   NameSpacePathXml(XmlBuffer&);
};

extern
class RetValXml {
  public:
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
#define TypeRValue 13
   virtual ~RetValXml() {}
   virtual RetValXml * clone()=0;
   int getType() { return type; }
   virtual void toStringBuffer(string &sb, char *q="")=0;
};

%extend RetValXml { 
   string __str__() { string b; self->toStringBuffer(b,""); return b; }
   long long __long__() { 
     string b; char *p=0; long long val;
     self->toStringBuffer(b,""); 
     errno = 0;
     val = strtoll(b.c_str(), &p, 10); 
     if(errno==ERANGE) {
       errno=0; 
       throw std::runtime_error(b+"is to large to be a long");
     }
     return val;
     }
};

extern
class ValueXml : public RetValXml {};

extern
class KeyBindingXml {
  public:
   string name;
   KeyBindingXml(XmlBuffer&);
   KeyBindingXml(char*,char*);
   void toStringBuffer(string&, char*s="");
};

%extend KeyBindingXml { string __str__() { string b; self->toStringBuffer(b,""); return b; } };

extern
class InstanceNameXml : public RetValXml {
  public:
   KeyBindingArray keys;
   InstanceNameXml(XmlBuffer&);
   void toStringBuffer(string&,char *q="");
};


extern
class InstanceNamesXml : public RetValXml, public InstanceNameArray {
  public:
   InstanceNamesXml(InstanceNameXml &n);
};

extern
class PropertyXml {
  public:
  string name, type, origin, propagated, refCls, arraySize;
   int pType;
  string str;
#define TypeProperty 1
#define TypePropertyRef 2
#define TypePropertyArray 3
   QualifierArray qualifiers;
   Option<ValueXml> value;
   int key;
   PropertyXml(XmlBuffer&);
   PropertyXml(char*,int);
   void addQualifier(QualifierXml& q);
   void toStringBuffer(string&,char*);
   void toXmlBuffer(string&,AValue*,int vOnly=0);
};

%extend PropertyXml { string __str__() { string b; self->toStringBuffer(b,""); return b; }};

extern
class ClassXml : public RetValXml {
  public:
   string clsName;
   string superClass;
   ClassXml(XmlBuffer&);
   PropertyArray &getProperties();
   QualifierArray &getQualifiers();
   PropertyArray &getKeys();
   void addQualifier(QualifierXml& q);
   void addProperty(PropertyXml& p);
   void addMethod(MethodXml& m);
   void toStringBuffer(string&,char *q="");
   int keysOk(URL&);
};

extern
class ClassesXml : public RetValXml, public ClassArray {
  public:
   ClassesXml(ClassXml *n);
};

extern
class InstanceXml : public RetValXml {
  public:
   InstanceXml() {}
   InstanceXml(XmlBuffer&);
   void propagateKeyFlags(PropertyArray& keys);
   void propagateKeyFlags(KeyBindingArray& keys);
   PropertyArray &getProperties();
   QualifierArray &getQualifiers();
   void addQualifier(QualifierXml& q);
   void addProperty(PropertyXml& p);
   virtual void toStringBuffer(string&,char *p="");
};

extern
class NamedInstanceXml : public RetValXml {
  public:
   NamedInstanceXml(XmlBuffer&);
   InstanceNameXml instname;
   InstanceXml inst;
};

extern
class InstancesXml : public RetValXml, public InstanceArray {
  public:
   InstancesXml(InstanceXml& n);
};

extern
class ObjectWithPathXml : public RetValXml {
  public:
   ObjectWithPathXml(XmlBuffer&);
   InstancePathXml *ip;
   InstanceXml inst;
};

extern
class ObjectsWithPathXml : public RetValXml {
  public:
   InstanceArray ia;
   InstancePathArray ipa;
   ObjectsWithPathXml(ObjectWithPathXml *n);
   void add(ObjectWithPathXml& n);
   InstancePathXml& getName(int n);
   InstanceXml& getInst(int n);
   int size();
};


extern
class NamedInstancesXml : public RetValXml {
  public:
   InstanceArray ia;
   InstanceNameArray ina;
   NamedInstancesXml(NamedInstanceXml *n);
   void add(NamedInstanceXml& n);
   InstanceNameXml& getName(int n);
   InstanceXml& getInst(int n);
%rename(__len__) size;
   int size();
};

extern
class ClassNameXml : public RetValXml {
  public:
   ClassNameXml(XmlBuffer&);
   void toStringBuffer(string &sb, char *q="");
};

extern
class ClassNamesXml : public RetValXml, public ClassNameArray {
  public:
   ClassNamesXml(ClassNameXml *n);
};

extern
class IReturnvalueXml {
  public:
   IReturnvalueXml(XmlBuffer&);
   RetValXml *getReturnData();
};

extern
class IMethodresponseXml {
  public:
   IMethodresponseXml(XmlBuffer&, int noIRET=0);
   RetValXml *getReturnData();
};

extern
class SimplerspXml {
  public:
   SimplerspXml(XmlBuffer&, int noIRET = 0);
   RetValXml *getReturnData();
};

extern
class MessageXml {
  public:
   MessageXml(XmlBuffer&, int noIRET = 0);
   SimplerspXml *getResponse();
};

extern
class CimXml {
  public:
   CimXml(string in, int noIRET = 0);
   SimplerspXml *getResponse();
};


extern
class URL {
  public:
   string host;
   string port;
   string cName;
   string user;
   string password;
   StringArray ns;
   PropertyArray keys;
   URL(char*);
   char *resolveIndirection(char *ind, char *u);
   int  refLookAhead(char*,char**);
   void toStringBuffer(string&);
   void toStringBuffer(string&,int nsOnly);
};

%extend URL { 
string __str__() { 
  string b;
  self->toStringBuffer(b,0);
  b = string("http://")+b; 
  return b; 
}
};

class Cimom {
 private:
  static char xmlHeader[];
  static char xmlTrailer[];
 protected:
  virtual void genRequest(URL&,char*)=0;
  virtual void addPayload(const string& pl)=0;
  virtual string getResponse()=0;
 public:
  virtual ~Cimom() {}
   ClassXml		*getClass(URL&, char *attr=NULL,
   				bool qual=true, bool origin=true, bool local=false);
   string               getClass2str(URL*, char *attr=NULL,
   				bool qual=true, bool origin=true, bool local=false);
   int			testInst(URL&,AValue*);
   InstanceXml 		*getInstance(URL&, char *attr=NULL, bool qual=true,
                             bool origin=true, bool local=false);
   NamedInstancesXml 	*enumInstances(URL&, char *attr=NULL, bool deep=true, bool qual=true,
                                       bool origin=true, bool local=false);
   string               enumInstances2str(URL*, char *attr=NULL, bool deep=true, bool qual=true,
                           bool origin=true, bool local=false);
   InstanceNameXml 	*createInstance(URL&,ClassXml*,char*);
   InstanceNamesXml 	*enumInstanceNames(URL&);
   ClassesXml 		*enumClasses(URL&, bool deep=true, bool qual=true,
                                 bool origin=true, bool local=false);
   ClassNamesXml 	*enumClassNames(URL&, bool deep=true);
   void                  modifyInstance(URL&,InstanceXml*,char*);

   /* Associator Provider Interface */
   InstanceNamesXml 	*associatorNames(URL&, char* assocClass, char *resultClass,
					 char *role, char *resultRole);
   ObjectsWithPathXml 	*associators(URL&, char *assocClass, char *resultClass,
                          char *role, char *resultRole, char *attr=NULL,
                          bool qual=true, bool origin=true);
   string                associators2str(URL*, char* assocClass, char *resultClass,
                          char *role, char *resultRole, char *attr=NULL,
                          bool qual=true, bool origin=true);
   InstanceNamesXml 	*referenceNames(URL&, char *resultClass, char *role);
   ObjectsWithPathXml	*references(URL&, char *resultClass, char *role, char *attr=NULL,
                                    bool qual=true, bool origin=true);
   string               references2str(URL*, char *resultClass, char *role, char *attr=NULL,
                                    bool qual=true, bool origin=true);
   /* end of Associator Provider Interface */

   ValueXml		*getProperty(URL&,char*);
   void			setProperty(URL&,ClassXml*,char*);

   void                  deleteClass(URL&);
   void                  deleteInstance(URL&);
//   char			*getReferences(URL*, bool qual=false, bool origin=false);
   static string         unescapeXml(const char*,int nq=0);
};

%newobject Cimom::getClass;
%newobject Cimom::getInstance;
%newobject Cimom::enumInstances;
%newobject Cimom::enumInstanceNames;
%newobject Cimom::enumClasses;
%newobject Cimom::enumClassNames;
%newobject Cimom::createInstance;
%newobject Cimom::associators;
%newobject Cimom::associatorNames;
%newobject Cimom::references;
%newobject Cimom::referenceNames;
%newobject Cimom::getProperty;


#ifdef LIBWWW

#else
extern
class CimomGhttp : public Cimom {
  private:
   ghttp_request *request;
   virtual void genRequest(URL&,char*);
   virtual void addPayload(const string& pl);
   virtual string getResponse();
  public:
   CimomGhttp();
};
#endif

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
