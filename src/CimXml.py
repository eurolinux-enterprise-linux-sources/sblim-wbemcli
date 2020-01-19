# This file was created automatically by SWIG.
# Don't modify this file, modify the SWIG interface instead.
# This file is compatible with both classic and new-style classes.
import _CimXml
def _swig_setattr(self,class_type,name,value):
    if (name == "this"):
        if isinstance(value, class_type):
            self.__dict__[name] = value.this
            if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
            del value.thisown
            return
    method = class_type.__swig_setmethods__.get(name,None)
    if method: return method(self,value)
    self.__dict__[name] = value

def _swig_getattr(self,class_type,name):
    method = class_type.__swig_getmethods__.get(name,None)
    if method: return method(self)
    raise AttributeError,name

import types
try:
    _object = types.ObjectType
    _newclass = 1
except AttributeError:
    class _object : pass
    _newclass = 0


class XmlBuffer(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, XmlBuffer, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, XmlBuffer, name)
    __swig_setmethods__["cur"] = _CimXml.XmlBuffer_cur_set
    __swig_getmethods__["cur"] = _CimXml.XmlBuffer_cur_get
    if _newclass:cur = property(_CimXml.XmlBuffer_cur_get,_CimXml.XmlBuffer_cur_set)
    def __init__(self,*args):
        _swig_setattr(self, XmlBuffer, 'this', apply(_CimXml.new_XmlBuffer,args))
        _swig_setattr(self, XmlBuffer, 'thisown', 1)
    def skipWS(*args): return apply(_CimXml.XmlBuffer_skipWS,args)
    def getChar(*args): return apply(_CimXml.XmlBuffer_getChar,args)
    def getWord(*args): return apply(_CimXml.XmlBuffer_getWord,args)
    def nextTag(*args): return apply(_CimXml.XmlBuffer_nextTag,args)
    def nextEquals(*args): return apply(_CimXml.XmlBuffer_nextEquals,args)
    def xmlHdr(*args): return apply(_CimXml.XmlBuffer_xmlHdr,args)
    def tagEquals(*args): return apply(_CimXml.XmlBuffer_tagEquals,args)
    def endTag(*args): return apply(_CimXml.XmlBuffer_endTag,args)
    def attrsOk(*args): return apply(_CimXml.XmlBuffer_attrsOk,args)
    def getValue(*args): return apply(_CimXml.XmlBuffer_getValue,args)
    def getContent(*args): return apply(_CimXml.XmlBuffer_getContent,args)
    def skipElement(*args): return apply(_CimXml.XmlBuffer_skipElement,args)
    def testAttr(*args): return apply(_CimXml.XmlBuffer_testAttr,args)
    def __del__(self, destroy= _CimXml.delete_XmlBuffer):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C XmlBuffer instance at %s>" % (self.this,)

class XmlBufferPtr(XmlBuffer):
    def __init__(self,this):
        _swig_setattr(self, XmlBuffer, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, XmlBuffer, 'thisown', 0)
        _swig_setattr(self, XmlBuffer,self.__class__,XmlBuffer)
_CimXml.XmlBuffer_swigregister(XmlBufferPtr)

class NameSpaceVector(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, NameSpaceVector, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, NameSpaceVector, name)
    def __init__(self,*args):
        _swig_setattr(self, NameSpaceVector, 'this', apply(_CimXml.new_NameSpaceVector,args))
        _swig_setattr(self, NameSpaceVector, 'thisown', 1)
    def __len__(*args): return apply(_CimXml.NameSpaceVector___len__,args)
    def clear(*args): return apply(_CimXml.NameSpaceVector_clear,args)
    def append(*args): return apply(_CimXml.NameSpaceVector_append,args)
    def __nonzero__(*args): return apply(_CimXml.NameSpaceVector___nonzero__,args)
    def pop(*args): return apply(_CimXml.NameSpaceVector_pop,args)
    def __getitem__(*args): return apply(_CimXml.NameSpaceVector___getitem__,args)
    def __getslice__(*args): return apply(_CimXml.NameSpaceVector___getslice__,args)
    def __setitem__(*args): return apply(_CimXml.NameSpaceVector___setitem__,args)
    def __setslice__(*args): return apply(_CimXml.NameSpaceVector___setslice__,args)
    def __delitem__(*args): return apply(_CimXml.NameSpaceVector___delitem__,args)
    def __delslice__(*args): return apply(_CimXml.NameSpaceVector___delslice__,args)
    def __del__(self, destroy= _CimXml.delete_NameSpaceVector):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C NameSpaceVector instance at %s>" % (self.this,)

class NameSpaceVectorPtr(NameSpaceVector):
    def __init__(self,this):
        _swig_setattr(self, NameSpaceVector, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, NameSpaceVector, 'thisown', 0)
        _swig_setattr(self, NameSpaceVector,self.__class__,NameSpaceVector)
_CimXml.NameSpaceVector_swigregister(NameSpaceVectorPtr)

class NameSpaceArray(NameSpaceVector):
    __swig_setmethods__ = {}
    for _s in [NameSpaceVector]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, NameSpaceArray, name, value)
    __swig_getmethods__ = {}
    for _s in [NameSpaceVector]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, NameSpaceArray, name)
    def __init__(self,*args):
        _swig_setattr(self, NameSpaceArray, 'this', apply(_CimXml.new_NameSpaceArray,args))
        _swig_setattr(self, NameSpaceArray, 'thisown', 1)
    def add(*args): return apply(_CimXml.NameSpaceArray_add,args)
    def toStringBuffer(*args): return apply(_CimXml.NameSpaceArray_toStringBuffer,args)
    def __setitem__(*args): return apply(_CimXml.NameSpaceArray___setitem__,args)
    def __getitem__(*args): return apply(_CimXml.NameSpaceArray___getitem__,args)
    def __str__(*args): return apply(_CimXml.NameSpaceArray___str__,args)
    def __iter__(*args): return apply(_CimXml.NameSpaceArray___iter__,args)
    def __del__(self, destroy= _CimXml.delete_NameSpaceArray):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C NameSpaceArray instance at %s>" % (self.this,)

class NameSpaceArrayPtr(NameSpaceArray):
    def __init__(self,this):
        _swig_setattr(self, NameSpaceArray, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, NameSpaceArray, 'thisown', 0)
        _swig_setattr(self, NameSpaceArray,self.__class__,NameSpaceArray)
_CimXml.NameSpaceArray_swigregister(NameSpaceArrayPtr)

class NameSpaceIterator(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, NameSpaceIterator, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, NameSpaceIterator, name)
    __swig_setmethods__["it"] = _CimXml.NameSpaceIterator_it_set
    __swig_getmethods__["it"] = _CimXml.NameSpaceIterator_it_get
    if _newclass:it = property(_CimXml.NameSpaceIterator_it_get,_CimXml.NameSpaceIterator_it_set)
    __swig_setmethods__["c"] = _CimXml.NameSpaceIterator_c_set
    __swig_getmethods__["c"] = _CimXml.NameSpaceIterator_c_get
    if _newclass:c = property(_CimXml.NameSpaceIterator_c_get,_CimXml.NameSpaceIterator_c_set)
    def __init__(self,*args):
        _swig_setattr(self, NameSpaceIterator, 'this', apply(_CimXml.new_NameSpaceIterator,args))
        _swig_setattr(self, NameSpaceIterator, 'thisown', 1)
    def __iter__(*args): return apply(_CimXml.NameSpaceIterator___iter__,args)
    def next(*args): return apply(_CimXml.NameSpaceIterator_next,args)
    def __del__(self, destroy= _CimXml.delete_NameSpaceIterator):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C NameSpaceIterator instance at %s>" % (self.this,)

class NameSpaceIteratorPtr(NameSpaceIterator):
    def __init__(self,this):
        _swig_setattr(self, NameSpaceIterator, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, NameSpaceIterator, 'thisown', 0)
        _swig_setattr(self, NameSpaceIterator,self.__class__,NameSpaceIterator)
_CimXml.NameSpaceIterator_swigregister(NameSpaceIteratorPtr)

class StringVector(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, StringVector, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, StringVector, name)
    def __init__(self,*args):
        _swig_setattr(self, StringVector, 'this', apply(_CimXml.new_StringVector,args))
        _swig_setattr(self, StringVector, 'thisown', 1)
    def __len__(*args): return apply(_CimXml.StringVector___len__,args)
    def clear(*args): return apply(_CimXml.StringVector_clear,args)
    def append(*args): return apply(_CimXml.StringVector_append,args)
    def __nonzero__(*args): return apply(_CimXml.StringVector___nonzero__,args)
    def pop(*args): return apply(_CimXml.StringVector_pop,args)
    def __getitem__(*args): return apply(_CimXml.StringVector___getitem__,args)
    def __getslice__(*args): return apply(_CimXml.StringVector___getslice__,args)
    def __setitem__(*args): return apply(_CimXml.StringVector___setitem__,args)
    def __setslice__(*args): return apply(_CimXml.StringVector___setslice__,args)
    def __delitem__(*args): return apply(_CimXml.StringVector___delitem__,args)
    def __delslice__(*args): return apply(_CimXml.StringVector___delslice__,args)
    def __del__(self, destroy= _CimXml.delete_StringVector):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C StringVector instance at %s>" % (self.this,)

class StringVectorPtr(StringVector):
    def __init__(self,this):
        _swig_setattr(self, StringVector, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, StringVector, 'thisown', 0)
        _swig_setattr(self, StringVector,self.__class__,StringVector)
_CimXml.StringVector_swigregister(StringVectorPtr)

class StringArray(StringVector):
    __swig_setmethods__ = {}
    for _s in [StringVector]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, StringArray, name, value)
    __swig_getmethods__ = {}
    for _s in [StringVector]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, StringArray, name)
    def add(*args): return apply(_CimXml.StringArray_add,args)
    def toStringBuffer(*args): return apply(_CimXml.StringArray_toStringBuffer,args)
    def __init__(self,*args):
        _swig_setattr(self, StringArray, 'this', apply(_CimXml.new_StringArray,args))
        _swig_setattr(self, StringArray, 'thisown', 1)
    def __del__(self, destroy= _CimXml.delete_StringArray):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C StringArray instance at %s>" % (self.this,)

class StringArrayPtr(StringArray):
    def __init__(self,this):
        _swig_setattr(self, StringArray, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, StringArray, 'thisown', 0)
        _swig_setattr(self, StringArray,self.__class__,StringArray)
_CimXml.StringArray_swigregister(StringArrayPtr)

class PropertyVector(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, PropertyVector, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, PropertyVector, name)
    def __init__(self,*args):
        _swig_setattr(self, PropertyVector, 'this', apply(_CimXml.new_PropertyVector,args))
        _swig_setattr(self, PropertyVector, 'thisown', 1)
    def __len__(*args): return apply(_CimXml.PropertyVector___len__,args)
    def clear(*args): return apply(_CimXml.PropertyVector_clear,args)
    def append(*args): return apply(_CimXml.PropertyVector_append,args)
    def __nonzero__(*args): return apply(_CimXml.PropertyVector___nonzero__,args)
    def pop(*args): return apply(_CimXml.PropertyVector_pop,args)
    def __getitem__(*args): return apply(_CimXml.PropertyVector___getitem__,args)
    def __getslice__(*args): return apply(_CimXml.PropertyVector___getslice__,args)
    def __setitem__(*args): return apply(_CimXml.PropertyVector___setitem__,args)
    def __setslice__(*args): return apply(_CimXml.PropertyVector___setslice__,args)
    def __delitem__(*args): return apply(_CimXml.PropertyVector___delitem__,args)
    def __delslice__(*args): return apply(_CimXml.PropertyVector___delslice__,args)
    def __del__(self, destroy= _CimXml.delete_PropertyVector):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C PropertyVector instance at %s>" % (self.this,)

class PropertyVectorPtr(PropertyVector):
    def __init__(self,this):
        _swig_setattr(self, PropertyVector, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, PropertyVector, 'thisown', 0)
        _swig_setattr(self, PropertyVector,self.__class__,PropertyVector)
_CimXml.PropertyVector_swigregister(PropertyVectorPtr)

class PropertyArray(PropertyVector):
    __swig_setmethods__ = {}
    for _s in [PropertyVector]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, PropertyArray, name, value)
    __swig_getmethods__ = {}
    for _s in [PropertyVector]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, PropertyArray, name)
    def __init__(self,*args):
        _swig_setattr(self, PropertyArray, 'this', apply(_CimXml.new_PropertyArray,args))
        _swig_setattr(self, PropertyArray, 'thisown', 1)
    def add(*args): return apply(_CimXml.PropertyArray_add,args)
    def toStringBuffer(*args): return apply(_CimXml.PropertyArray_toStringBuffer,args)
    def __setitem__(*args): return apply(_CimXml.PropertyArray___setitem__,args)
    def __str__(*args): return apply(_CimXml.PropertyArray___str__,args)
    def __iter__(*args): return apply(_CimXml.PropertyArray___iter__,args)
    def __getitem__(*args): return apply(_CimXml.PropertyArray___getitem__,args)
    def __delitem__(*args): return apply(_CimXml.PropertyArray___delitem__,args)
    def keys(*args): return apply(_CimXml.PropertyArray_keys,args)
    def __del__(self, destroy= _CimXml.delete_PropertyArray):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C PropertyArray instance at %s>" % (self.this,)

class PropertyArrayPtr(PropertyArray):
    def __init__(self,this):
        _swig_setattr(self, PropertyArray, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, PropertyArray, 'thisown', 0)
        _swig_setattr(self, PropertyArray,self.__class__,PropertyArray)
_CimXml.PropertyArray_swigregister(PropertyArrayPtr)

class PropertyIterator(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, PropertyIterator, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, PropertyIterator, name)
    __swig_setmethods__["it"] = _CimXml.PropertyIterator_it_set
    __swig_getmethods__["it"] = _CimXml.PropertyIterator_it_get
    if _newclass:it = property(_CimXml.PropertyIterator_it_get,_CimXml.PropertyIterator_it_set)
    __swig_setmethods__["c"] = _CimXml.PropertyIterator_c_set
    __swig_getmethods__["c"] = _CimXml.PropertyIterator_c_get
    if _newclass:c = property(_CimXml.PropertyIterator_c_get,_CimXml.PropertyIterator_c_set)
    def __init__(self,*args):
        _swig_setattr(self, PropertyIterator, 'this', apply(_CimXml.new_PropertyIterator,args))
        _swig_setattr(self, PropertyIterator, 'thisown', 1)
    def __iter__(*args): return apply(_CimXml.PropertyIterator___iter__,args)
    def next(*args): return apply(_CimXml.PropertyIterator_next,args)
    def __del__(self, destroy= _CimXml.delete_PropertyIterator):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C PropertyIterator instance at %s>" % (self.this,)

class PropertyIteratorPtr(PropertyIterator):
    def __init__(self,this):
        _swig_setattr(self, PropertyIterator, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, PropertyIterator, 'thisown', 0)
        _swig_setattr(self, PropertyIterator,self.__class__,PropertyIterator)
_CimXml.PropertyIterator_swigregister(PropertyIteratorPtr)

class InstanceNameVector(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, InstanceNameVector, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, InstanceNameVector, name)
    def __init__(self,*args):
        _swig_setattr(self, InstanceNameVector, 'this', apply(_CimXml.new_InstanceNameVector,args))
        _swig_setattr(self, InstanceNameVector, 'thisown', 1)
    def __len__(*args): return apply(_CimXml.InstanceNameVector___len__,args)
    def clear(*args): return apply(_CimXml.InstanceNameVector_clear,args)
    def append(*args): return apply(_CimXml.InstanceNameVector_append,args)
    def __nonzero__(*args): return apply(_CimXml.InstanceNameVector___nonzero__,args)
    def pop(*args): return apply(_CimXml.InstanceNameVector_pop,args)
    def __getitem__(*args): return apply(_CimXml.InstanceNameVector___getitem__,args)
    def __getslice__(*args): return apply(_CimXml.InstanceNameVector___getslice__,args)
    def __setitem__(*args): return apply(_CimXml.InstanceNameVector___setitem__,args)
    def __setslice__(*args): return apply(_CimXml.InstanceNameVector___setslice__,args)
    def __delitem__(*args): return apply(_CimXml.InstanceNameVector___delitem__,args)
    def __delslice__(*args): return apply(_CimXml.InstanceNameVector___delslice__,args)
    def __del__(self, destroy= _CimXml.delete_InstanceNameVector):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C InstanceNameVector instance at %s>" % (self.this,)

class InstanceNameVectorPtr(InstanceNameVector):
    def __init__(self,this):
        _swig_setattr(self, InstanceNameVector, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, InstanceNameVector, 'thisown', 0)
        _swig_setattr(self, InstanceNameVector,self.__class__,InstanceNameVector)
_CimXml.InstanceNameVector_swigregister(InstanceNameVectorPtr)

class InstanceNameArray(InstanceNameVector):
    __swig_setmethods__ = {}
    for _s in [InstanceNameVector]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, InstanceNameArray, name, value)
    __swig_getmethods__ = {}
    for _s in [InstanceNameVector]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, InstanceNameArray, name)
    def __init__(self,*args):
        _swig_setattr(self, InstanceNameArray, 'this', apply(_CimXml.new_InstanceNameArray,args))
        _swig_setattr(self, InstanceNameArray, 'thisown', 1)
    def add(*args): return apply(_CimXml.InstanceNameArray_add,args)
    def toStringBuffer(*args): return apply(_CimXml.InstanceNameArray_toStringBuffer,args)
    def __setitem__(*args): return apply(_CimXml.InstanceNameArray___setitem__,args)
    def __getitem__(*args): return apply(_CimXml.InstanceNameArray___getitem__,args)
    def __str__(*args): return apply(_CimXml.InstanceNameArray___str__,args)
    def __iter__(*args): return apply(_CimXml.InstanceNameArray___iter__,args)
    def __del__(self, destroy= _CimXml.delete_InstanceNameArray):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C InstanceNameArray instance at %s>" % (self.this,)

class InstanceNameArrayPtr(InstanceNameArray):
    def __init__(self,this):
        _swig_setattr(self, InstanceNameArray, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, InstanceNameArray, 'thisown', 0)
        _swig_setattr(self, InstanceNameArray,self.__class__,InstanceNameArray)
_CimXml.InstanceNameArray_swigregister(InstanceNameArrayPtr)

class InstanceNameIterator(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, InstanceNameIterator, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, InstanceNameIterator, name)
    __swig_setmethods__["it"] = _CimXml.InstanceNameIterator_it_set
    __swig_getmethods__["it"] = _CimXml.InstanceNameIterator_it_get
    if _newclass:it = property(_CimXml.InstanceNameIterator_it_get,_CimXml.InstanceNameIterator_it_set)
    __swig_setmethods__["c"] = _CimXml.InstanceNameIterator_c_set
    __swig_getmethods__["c"] = _CimXml.InstanceNameIterator_c_get
    if _newclass:c = property(_CimXml.InstanceNameIterator_c_get,_CimXml.InstanceNameIterator_c_set)
    def __init__(self,*args):
        _swig_setattr(self, InstanceNameIterator, 'this', apply(_CimXml.new_InstanceNameIterator,args))
        _swig_setattr(self, InstanceNameIterator, 'thisown', 1)
    def __iter__(*args): return apply(_CimXml.InstanceNameIterator___iter__,args)
    def next(*args): return apply(_CimXml.InstanceNameIterator_next,args)
    def __del__(self, destroy= _CimXml.delete_InstanceNameIterator):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C InstanceNameIterator instance at %s>" % (self.this,)

class InstanceNameIteratorPtr(InstanceNameIterator):
    def __init__(self,this):
        _swig_setattr(self, InstanceNameIterator, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, InstanceNameIterator, 'thisown', 0)
        _swig_setattr(self, InstanceNameIterator,self.__class__,InstanceNameIterator)
_CimXml.InstanceNameIterator_swigregister(InstanceNameIteratorPtr)

class InstanceVector(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, InstanceVector, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, InstanceVector, name)
    def __init__(self,*args):
        _swig_setattr(self, InstanceVector, 'this', apply(_CimXml.new_InstanceVector,args))
        _swig_setattr(self, InstanceVector, 'thisown', 1)
    def __len__(*args): return apply(_CimXml.InstanceVector___len__,args)
    def clear(*args): return apply(_CimXml.InstanceVector_clear,args)
    def append(*args): return apply(_CimXml.InstanceVector_append,args)
    def __nonzero__(*args): return apply(_CimXml.InstanceVector___nonzero__,args)
    def pop(*args): return apply(_CimXml.InstanceVector_pop,args)
    def __getitem__(*args): return apply(_CimXml.InstanceVector___getitem__,args)
    def __getslice__(*args): return apply(_CimXml.InstanceVector___getslice__,args)
    def __setitem__(*args): return apply(_CimXml.InstanceVector___setitem__,args)
    def __setslice__(*args): return apply(_CimXml.InstanceVector___setslice__,args)
    def __delitem__(*args): return apply(_CimXml.InstanceVector___delitem__,args)
    def __delslice__(*args): return apply(_CimXml.InstanceVector___delslice__,args)
    def __del__(self, destroy= _CimXml.delete_InstanceVector):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C InstanceVector instance at %s>" % (self.this,)

class InstanceVectorPtr(InstanceVector):
    def __init__(self,this):
        _swig_setattr(self, InstanceVector, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, InstanceVector, 'thisown', 0)
        _swig_setattr(self, InstanceVector,self.__class__,InstanceVector)
_CimXml.InstanceVector_swigregister(InstanceVectorPtr)

class InstanceArray(InstanceVector):
    __swig_setmethods__ = {}
    for _s in [InstanceVector]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, InstanceArray, name, value)
    __swig_getmethods__ = {}
    for _s in [InstanceVector]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, InstanceArray, name)
    def __init__(self,*args):
        _swig_setattr(self, InstanceArray, 'this', apply(_CimXml.new_InstanceArray,args))
        _swig_setattr(self, InstanceArray, 'thisown', 1)
    def add(*args): return apply(_CimXml.InstanceArray_add,args)
    def toStringBuffer(*args): return apply(_CimXml.InstanceArray_toStringBuffer,args)
    def __setitem__(*args): return apply(_CimXml.InstanceArray___setitem__,args)
    def __getitem__(*args): return apply(_CimXml.InstanceArray___getitem__,args)
    def __str__(*args): return apply(_CimXml.InstanceArray___str__,args)
    def __iter__(*args): return apply(_CimXml.InstanceArray___iter__,args)
    def __del__(self, destroy= _CimXml.delete_InstanceArray):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C InstanceArray instance at %s>" % (self.this,)

class InstanceArrayPtr(InstanceArray):
    def __init__(self,this):
        _swig_setattr(self, InstanceArray, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, InstanceArray, 'thisown', 0)
        _swig_setattr(self, InstanceArray,self.__class__,InstanceArray)
_CimXml.InstanceArray_swigregister(InstanceArrayPtr)

class InstanceIterator(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, InstanceIterator, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, InstanceIterator, name)
    __swig_setmethods__["it"] = _CimXml.InstanceIterator_it_set
    __swig_getmethods__["it"] = _CimXml.InstanceIterator_it_get
    if _newclass:it = property(_CimXml.InstanceIterator_it_get,_CimXml.InstanceIterator_it_set)
    __swig_setmethods__["c"] = _CimXml.InstanceIterator_c_set
    __swig_getmethods__["c"] = _CimXml.InstanceIterator_c_get
    if _newclass:c = property(_CimXml.InstanceIterator_c_get,_CimXml.InstanceIterator_c_set)
    def __init__(self,*args):
        _swig_setattr(self, InstanceIterator, 'this', apply(_CimXml.new_InstanceIterator,args))
        _swig_setattr(self, InstanceIterator, 'thisown', 1)
    def __iter__(*args): return apply(_CimXml.InstanceIterator___iter__,args)
    def next(*args): return apply(_CimXml.InstanceIterator_next,args)
    def __del__(self, destroy= _CimXml.delete_InstanceIterator):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C InstanceIterator instance at %s>" % (self.this,)

class InstanceIteratorPtr(InstanceIterator):
    def __init__(self,this):
        _swig_setattr(self, InstanceIterator, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, InstanceIterator, 'thisown', 0)
        _swig_setattr(self, InstanceIterator,self.__class__,InstanceIterator)
_CimXml.InstanceIterator_swigregister(InstanceIteratorPtr)

class KeyBindingVector(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, KeyBindingVector, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, KeyBindingVector, name)
    def __init__(self,*args):
        _swig_setattr(self, KeyBindingVector, 'this', apply(_CimXml.new_KeyBindingVector,args))
        _swig_setattr(self, KeyBindingVector, 'thisown', 1)
    def __len__(*args): return apply(_CimXml.KeyBindingVector___len__,args)
    def clear(*args): return apply(_CimXml.KeyBindingVector_clear,args)
    def append(*args): return apply(_CimXml.KeyBindingVector_append,args)
    def __nonzero__(*args): return apply(_CimXml.KeyBindingVector___nonzero__,args)
    def pop(*args): return apply(_CimXml.KeyBindingVector_pop,args)
    def __getitem__(*args): return apply(_CimXml.KeyBindingVector___getitem__,args)
    def __getslice__(*args): return apply(_CimXml.KeyBindingVector___getslice__,args)
    def __setitem__(*args): return apply(_CimXml.KeyBindingVector___setitem__,args)
    def __setslice__(*args): return apply(_CimXml.KeyBindingVector___setslice__,args)
    def __delitem__(*args): return apply(_CimXml.KeyBindingVector___delitem__,args)
    def __delslice__(*args): return apply(_CimXml.KeyBindingVector___delslice__,args)
    def __del__(self, destroy= _CimXml.delete_KeyBindingVector):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C KeyBindingVector instance at %s>" % (self.this,)

class KeyBindingVectorPtr(KeyBindingVector):
    def __init__(self,this):
        _swig_setattr(self, KeyBindingVector, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, KeyBindingVector, 'thisown', 0)
        _swig_setattr(self, KeyBindingVector,self.__class__,KeyBindingVector)
_CimXml.KeyBindingVector_swigregister(KeyBindingVectorPtr)

class KeyBindingArray(KeyBindingVector):
    __swig_setmethods__ = {}
    for _s in [KeyBindingVector]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, KeyBindingArray, name, value)
    __swig_getmethods__ = {}
    for _s in [KeyBindingVector]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, KeyBindingArray, name)
    def __init__(self,*args):
        _swig_setattr(self, KeyBindingArray, 'this', apply(_CimXml.new_KeyBindingArray,args))
        _swig_setattr(self, KeyBindingArray, 'thisown', 1)
    def add(*args): return apply(_CimXml.KeyBindingArray_add,args)
    def toStringBuffer(*args): return apply(_CimXml.KeyBindingArray_toStringBuffer,args)
    def __setitem__(*args): return apply(_CimXml.KeyBindingArray___setitem__,args)
    def __getitem__(*args): return apply(_CimXml.KeyBindingArray___getitem__,args)
    def __str__(*args): return apply(_CimXml.KeyBindingArray___str__,args)
    def __iter__(*args): return apply(_CimXml.KeyBindingArray___iter__,args)
    def __del__(self, destroy= _CimXml.delete_KeyBindingArray):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C KeyBindingArray instance at %s>" % (self.this,)

class KeyBindingArrayPtr(KeyBindingArray):
    def __init__(self,this):
        _swig_setattr(self, KeyBindingArray, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, KeyBindingArray, 'thisown', 0)
        _swig_setattr(self, KeyBindingArray,self.__class__,KeyBindingArray)
_CimXml.KeyBindingArray_swigregister(KeyBindingArrayPtr)

class KeyBindingIterator(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, KeyBindingIterator, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, KeyBindingIterator, name)
    __swig_setmethods__["it"] = _CimXml.KeyBindingIterator_it_set
    __swig_getmethods__["it"] = _CimXml.KeyBindingIterator_it_get
    if _newclass:it = property(_CimXml.KeyBindingIterator_it_get,_CimXml.KeyBindingIterator_it_set)
    __swig_setmethods__["c"] = _CimXml.KeyBindingIterator_c_set
    __swig_getmethods__["c"] = _CimXml.KeyBindingIterator_c_get
    if _newclass:c = property(_CimXml.KeyBindingIterator_c_get,_CimXml.KeyBindingIterator_c_set)
    def __init__(self,*args):
        _swig_setattr(self, KeyBindingIterator, 'this', apply(_CimXml.new_KeyBindingIterator,args))
        _swig_setattr(self, KeyBindingIterator, 'thisown', 1)
    def __iter__(*args): return apply(_CimXml.KeyBindingIterator___iter__,args)
    def next(*args): return apply(_CimXml.KeyBindingIterator_next,args)
    def __del__(self, destroy= _CimXml.delete_KeyBindingIterator):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C KeyBindingIterator instance at %s>" % (self.this,)

class KeyBindingIteratorPtr(KeyBindingIterator):
    def __init__(self,this):
        _swig_setattr(self, KeyBindingIterator, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, KeyBindingIterator, 'thisown', 0)
        _swig_setattr(self, KeyBindingIterator,self.__class__,KeyBindingIterator)
_CimXml.KeyBindingIterator_swigregister(KeyBindingIteratorPtr)

class ClassNameVector(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, ClassNameVector, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, ClassNameVector, name)
    def __init__(self,*args):
        _swig_setattr(self, ClassNameVector, 'this', apply(_CimXml.new_ClassNameVector,args))
        _swig_setattr(self, ClassNameVector, 'thisown', 1)
    def __len__(*args): return apply(_CimXml.ClassNameVector___len__,args)
    def clear(*args): return apply(_CimXml.ClassNameVector_clear,args)
    def append(*args): return apply(_CimXml.ClassNameVector_append,args)
    def __nonzero__(*args): return apply(_CimXml.ClassNameVector___nonzero__,args)
    def pop(*args): return apply(_CimXml.ClassNameVector_pop,args)
    def __getitem__(*args): return apply(_CimXml.ClassNameVector___getitem__,args)
    def __getslice__(*args): return apply(_CimXml.ClassNameVector___getslice__,args)
    def __setitem__(*args): return apply(_CimXml.ClassNameVector___setitem__,args)
    def __setslice__(*args): return apply(_CimXml.ClassNameVector___setslice__,args)
    def __delitem__(*args): return apply(_CimXml.ClassNameVector___delitem__,args)
    def __delslice__(*args): return apply(_CimXml.ClassNameVector___delslice__,args)
    def __del__(self, destroy= _CimXml.delete_ClassNameVector):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C ClassNameVector instance at %s>" % (self.this,)

class ClassNameVectorPtr(ClassNameVector):
    def __init__(self,this):
        _swig_setattr(self, ClassNameVector, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, ClassNameVector, 'thisown', 0)
        _swig_setattr(self, ClassNameVector,self.__class__,ClassNameVector)
_CimXml.ClassNameVector_swigregister(ClassNameVectorPtr)

class ClassNameArray(ClassNameVector):
    __swig_setmethods__ = {}
    for _s in [ClassNameVector]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, ClassNameArray, name, value)
    __swig_getmethods__ = {}
    for _s in [ClassNameVector]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, ClassNameArray, name)
    def __init__(self,*args):
        _swig_setattr(self, ClassNameArray, 'this', apply(_CimXml.new_ClassNameArray,args))
        _swig_setattr(self, ClassNameArray, 'thisown', 1)
    def add(*args): return apply(_CimXml.ClassNameArray_add,args)
    def toStringBuffer(*args): return apply(_CimXml.ClassNameArray_toStringBuffer,args)
    def __setitem__(*args): return apply(_CimXml.ClassNameArray___setitem__,args)
    def __getitem__(*args): return apply(_CimXml.ClassNameArray___getitem__,args)
    def __str__(*args): return apply(_CimXml.ClassNameArray___str__,args)
    def __iter__(*args): return apply(_CimXml.ClassNameArray___iter__,args)
    def __del__(self, destroy= _CimXml.delete_ClassNameArray):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C ClassNameArray instance at %s>" % (self.this,)

class ClassNameArrayPtr(ClassNameArray):
    def __init__(self,this):
        _swig_setattr(self, ClassNameArray, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, ClassNameArray, 'thisown', 0)
        _swig_setattr(self, ClassNameArray,self.__class__,ClassNameArray)
_CimXml.ClassNameArray_swigregister(ClassNameArrayPtr)

class ClassNameIterator(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, ClassNameIterator, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, ClassNameIterator, name)
    __swig_setmethods__["it"] = _CimXml.ClassNameIterator_it_set
    __swig_getmethods__["it"] = _CimXml.ClassNameIterator_it_get
    if _newclass:it = property(_CimXml.ClassNameIterator_it_get,_CimXml.ClassNameIterator_it_set)
    __swig_setmethods__["c"] = _CimXml.ClassNameIterator_c_set
    __swig_getmethods__["c"] = _CimXml.ClassNameIterator_c_get
    if _newclass:c = property(_CimXml.ClassNameIterator_c_get,_CimXml.ClassNameIterator_c_set)
    def __init__(self,*args):
        _swig_setattr(self, ClassNameIterator, 'this', apply(_CimXml.new_ClassNameIterator,args))
        _swig_setattr(self, ClassNameIterator, 'thisown', 1)
    def __iter__(*args): return apply(_CimXml.ClassNameIterator___iter__,args)
    def next(*args): return apply(_CimXml.ClassNameIterator_next,args)
    def __del__(self, destroy= _CimXml.delete_ClassNameIterator):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C ClassNameIterator instance at %s>" % (self.this,)

class ClassNameIteratorPtr(ClassNameIterator):
    def __init__(self,this):
        _swig_setattr(self, ClassNameIterator, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, ClassNameIterator, 'thisown', 0)
        _swig_setattr(self, ClassNameIterator,self.__class__,ClassNameIterator)
_CimXml.ClassNameIterator_swigregister(ClassNameIteratorPtr)

class ClassVector(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, ClassVector, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, ClassVector, name)
    def __init__(self,*args):
        _swig_setattr(self, ClassVector, 'this', apply(_CimXml.new_ClassVector,args))
        _swig_setattr(self, ClassVector, 'thisown', 1)
    def __len__(*args): return apply(_CimXml.ClassVector___len__,args)
    def clear(*args): return apply(_CimXml.ClassVector_clear,args)
    def append(*args): return apply(_CimXml.ClassVector_append,args)
    def __nonzero__(*args): return apply(_CimXml.ClassVector___nonzero__,args)
    def pop(*args): return apply(_CimXml.ClassVector_pop,args)
    def __getitem__(*args): return apply(_CimXml.ClassVector___getitem__,args)
    def __getslice__(*args): return apply(_CimXml.ClassVector___getslice__,args)
    def __setitem__(*args): return apply(_CimXml.ClassVector___setitem__,args)
    def __setslice__(*args): return apply(_CimXml.ClassVector___setslice__,args)
    def __delitem__(*args): return apply(_CimXml.ClassVector___delitem__,args)
    def __delslice__(*args): return apply(_CimXml.ClassVector___delslice__,args)
    def __del__(self, destroy= _CimXml.delete_ClassVector):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C ClassVector instance at %s>" % (self.this,)

class ClassVectorPtr(ClassVector):
    def __init__(self,this):
        _swig_setattr(self, ClassVector, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, ClassVector, 'thisown', 0)
        _swig_setattr(self, ClassVector,self.__class__,ClassVector)
_CimXml.ClassVector_swigregister(ClassVectorPtr)

class ClassArray(ClassVector):
    __swig_setmethods__ = {}
    for _s in [ClassVector]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, ClassArray, name, value)
    __swig_getmethods__ = {}
    for _s in [ClassVector]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, ClassArray, name)
    def __init__(self,*args):
        _swig_setattr(self, ClassArray, 'this', apply(_CimXml.new_ClassArray,args))
        _swig_setattr(self, ClassArray, 'thisown', 1)
    def add(*args): return apply(_CimXml.ClassArray_add,args)
    def toStringBuffer(*args): return apply(_CimXml.ClassArray_toStringBuffer,args)
    def __setitem__(*args): return apply(_CimXml.ClassArray___setitem__,args)
    def __getitem__(*args): return apply(_CimXml.ClassArray___getitem__,args)
    def __str__(*args): return apply(_CimXml.ClassArray___str__,args)
    def __iter__(*args): return apply(_CimXml.ClassArray___iter__,args)
    def __del__(self, destroy= _CimXml.delete_ClassArray):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C ClassArray instance at %s>" % (self.this,)

class ClassArrayPtr(ClassArray):
    def __init__(self,this):
        _swig_setattr(self, ClassArray, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, ClassArray, 'thisown', 0)
        _swig_setattr(self, ClassArray,self.__class__,ClassArray)
_CimXml.ClassArray_swigregister(ClassArrayPtr)

class ClassIterator(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, ClassIterator, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, ClassIterator, name)
    __swig_setmethods__["it"] = _CimXml.ClassIterator_it_set
    __swig_getmethods__["it"] = _CimXml.ClassIterator_it_get
    if _newclass:it = property(_CimXml.ClassIterator_it_get,_CimXml.ClassIterator_it_set)
    __swig_setmethods__["c"] = _CimXml.ClassIterator_c_set
    __swig_getmethods__["c"] = _CimXml.ClassIterator_c_get
    if _newclass:c = property(_CimXml.ClassIterator_c_get,_CimXml.ClassIterator_c_set)
    def __init__(self,*args):
        _swig_setattr(self, ClassIterator, 'this', apply(_CimXml.new_ClassIterator,args))
        _swig_setattr(self, ClassIterator, 'thisown', 1)
    def __iter__(*args): return apply(_CimXml.ClassIterator___iter__,args)
    def next(*args): return apply(_CimXml.ClassIterator_next,args)
    def __del__(self, destroy= _CimXml.delete_ClassIterator):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C ClassIterator instance at %s>" % (self.this,)

class ClassIteratorPtr(ClassIterator):
    def __init__(self,this):
        _swig_setattr(self, ClassIterator, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, ClassIterator, 'thisown', 0)
        _swig_setattr(self, ClassIterator,self.__class__,ClassIterator)
_CimXml.ClassIterator_swigregister(ClassIteratorPtr)

class ObjectWithPathVector(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, ObjectWithPathVector, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, ObjectWithPathVector, name)
    def __init__(self,*args):
        _swig_setattr(self, ObjectWithPathVector, 'this', apply(_CimXml.new_ObjectWithPathVector,args))
        _swig_setattr(self, ObjectWithPathVector, 'thisown', 1)
    def __len__(*args): return apply(_CimXml.ObjectWithPathVector___len__,args)
    def clear(*args): return apply(_CimXml.ObjectWithPathVector_clear,args)
    def append(*args): return apply(_CimXml.ObjectWithPathVector_append,args)
    def __nonzero__(*args): return apply(_CimXml.ObjectWithPathVector___nonzero__,args)
    def pop(*args): return apply(_CimXml.ObjectWithPathVector_pop,args)
    def __getitem__(*args): return apply(_CimXml.ObjectWithPathVector___getitem__,args)
    def __getslice__(*args): return apply(_CimXml.ObjectWithPathVector___getslice__,args)
    def __setitem__(*args): return apply(_CimXml.ObjectWithPathVector___setitem__,args)
    def __setslice__(*args): return apply(_CimXml.ObjectWithPathVector___setslice__,args)
    def __delitem__(*args): return apply(_CimXml.ObjectWithPathVector___delitem__,args)
    def __delslice__(*args): return apply(_CimXml.ObjectWithPathVector___delslice__,args)
    def __del__(self, destroy= _CimXml.delete_ObjectWithPathVector):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C ObjectWithPathVector instance at %s>" % (self.this,)

class ObjectWithPathVectorPtr(ObjectWithPathVector):
    def __init__(self,this):
        _swig_setattr(self, ObjectWithPathVector, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, ObjectWithPathVector, 'thisown', 0)
        _swig_setattr(self, ObjectWithPathVector,self.__class__,ObjectWithPathVector)
_CimXml.ObjectWithPathVector_swigregister(ObjectWithPathVectorPtr)

class ObjectWithPathArray(ObjectWithPathVector):
    __swig_setmethods__ = {}
    for _s in [ObjectWithPathVector]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, ObjectWithPathArray, name, value)
    __swig_getmethods__ = {}
    for _s in [ObjectWithPathVector]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, ObjectWithPathArray, name)
    def __init__(self,*args):
        _swig_setattr(self, ObjectWithPathArray, 'this', apply(_CimXml.new_ObjectWithPathArray,args))
        _swig_setattr(self, ObjectWithPathArray, 'thisown', 1)
    def add(*args): return apply(_CimXml.ObjectWithPathArray_add,args)
    def toStringBuffer(*args): return apply(_CimXml.ObjectWithPathArray_toStringBuffer,args)
    def __setitem__(*args): return apply(_CimXml.ObjectWithPathArray___setitem__,args)
    def __getitem__(*args): return apply(_CimXml.ObjectWithPathArray___getitem__,args)
    def __str__(*args): return apply(_CimXml.ObjectWithPathArray___str__,args)
    def __iter__(*args): return apply(_CimXml.ObjectWithPathArray___iter__,args)
    def __del__(self, destroy= _CimXml.delete_ObjectWithPathArray):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C ObjectWithPathArray instance at %s>" % (self.this,)

class ObjectWithPathArrayPtr(ObjectWithPathArray):
    def __init__(self,this):
        _swig_setattr(self, ObjectWithPathArray, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, ObjectWithPathArray, 'thisown', 0)
        _swig_setattr(self, ObjectWithPathArray,self.__class__,ObjectWithPathArray)
_CimXml.ObjectWithPathArray_swigregister(ObjectWithPathArrayPtr)

class ObjectWithPathIterator(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, ObjectWithPathIterator, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, ObjectWithPathIterator, name)
    __swig_setmethods__["it"] = _CimXml.ObjectWithPathIterator_it_set
    __swig_getmethods__["it"] = _CimXml.ObjectWithPathIterator_it_get
    if _newclass:it = property(_CimXml.ObjectWithPathIterator_it_get,_CimXml.ObjectWithPathIterator_it_set)
    __swig_setmethods__["c"] = _CimXml.ObjectWithPathIterator_c_set
    __swig_getmethods__["c"] = _CimXml.ObjectWithPathIterator_c_get
    if _newclass:c = property(_CimXml.ObjectWithPathIterator_c_get,_CimXml.ObjectWithPathIterator_c_set)
    def __init__(self,*args):
        _swig_setattr(self, ObjectWithPathIterator, 'this', apply(_CimXml.new_ObjectWithPathIterator,args))
        _swig_setattr(self, ObjectWithPathIterator, 'thisown', 1)
    def __iter__(*args): return apply(_CimXml.ObjectWithPathIterator___iter__,args)
    def next(*args): return apply(_CimXml.ObjectWithPathIterator_next,args)
    def __del__(self, destroy= _CimXml.delete_ObjectWithPathIterator):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C ObjectWithPathIterator instance at %s>" % (self.this,)

class ObjectWithPathIteratorPtr(ObjectWithPathIterator):
    def __init__(self,this):
        _swig_setattr(self, ObjectWithPathIterator, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, ObjectWithPathIterator, 'thisown', 0)
        _swig_setattr(self, ObjectWithPathIterator,self.__class__,ObjectWithPathIterator)
_CimXml.ObjectWithPathIterator_swigregister(ObjectWithPathIteratorPtr)

class NamedInstanceVector(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, NamedInstanceVector, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, NamedInstanceVector, name)
    def __init__(self,*args):
        _swig_setattr(self, NamedInstanceVector, 'this', apply(_CimXml.new_NamedInstanceVector,args))
        _swig_setattr(self, NamedInstanceVector, 'thisown', 1)
    def __len__(*args): return apply(_CimXml.NamedInstanceVector___len__,args)
    def clear(*args): return apply(_CimXml.NamedInstanceVector_clear,args)
    def append(*args): return apply(_CimXml.NamedInstanceVector_append,args)
    def __nonzero__(*args): return apply(_CimXml.NamedInstanceVector___nonzero__,args)
    def pop(*args): return apply(_CimXml.NamedInstanceVector_pop,args)
    def __getitem__(*args): return apply(_CimXml.NamedInstanceVector___getitem__,args)
    def __getslice__(*args): return apply(_CimXml.NamedInstanceVector___getslice__,args)
    def __setitem__(*args): return apply(_CimXml.NamedInstanceVector___setitem__,args)
    def __setslice__(*args): return apply(_CimXml.NamedInstanceVector___setslice__,args)
    def __delitem__(*args): return apply(_CimXml.NamedInstanceVector___delitem__,args)
    def __delslice__(*args): return apply(_CimXml.NamedInstanceVector___delslice__,args)
    def __del__(self, destroy= _CimXml.delete_NamedInstanceVector):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C NamedInstanceVector instance at %s>" % (self.this,)

class NamedInstanceVectorPtr(NamedInstanceVector):
    def __init__(self,this):
        _swig_setattr(self, NamedInstanceVector, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, NamedInstanceVector, 'thisown', 0)
        _swig_setattr(self, NamedInstanceVector,self.__class__,NamedInstanceVector)
_CimXml.NamedInstanceVector_swigregister(NamedInstanceVectorPtr)

class NamedInstanceArray(NamedInstanceVector):
    __swig_setmethods__ = {}
    for _s in [NamedInstanceVector]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, NamedInstanceArray, name, value)
    __swig_getmethods__ = {}
    for _s in [NamedInstanceVector]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, NamedInstanceArray, name)
    def __init__(self,*args):
        _swig_setattr(self, NamedInstanceArray, 'this', apply(_CimXml.new_NamedInstanceArray,args))
        _swig_setattr(self, NamedInstanceArray, 'thisown', 1)
    def add(*args): return apply(_CimXml.NamedInstanceArray_add,args)
    def toStringBuffer(*args): return apply(_CimXml.NamedInstanceArray_toStringBuffer,args)
    def __setitem__(*args): return apply(_CimXml.NamedInstanceArray___setitem__,args)
    def __getitem__(*args): return apply(_CimXml.NamedInstanceArray___getitem__,args)
    def __str__(*args): return apply(_CimXml.NamedInstanceArray___str__,args)
    def __iter__(*args): return apply(_CimXml.NamedInstanceArray___iter__,args)
    def __del__(self, destroy= _CimXml.delete_NamedInstanceArray):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C NamedInstanceArray instance at %s>" % (self.this,)

class NamedInstanceArrayPtr(NamedInstanceArray):
    def __init__(self,this):
        _swig_setattr(self, NamedInstanceArray, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, NamedInstanceArray, 'thisown', 0)
        _swig_setattr(self, NamedInstanceArray,self.__class__,NamedInstanceArray)
_CimXml.NamedInstanceArray_swigregister(NamedInstanceArrayPtr)

class NamedInstanceIterator(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, NamedInstanceIterator, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, NamedInstanceIterator, name)
    __swig_setmethods__["it"] = _CimXml.NamedInstanceIterator_it_set
    __swig_getmethods__["it"] = _CimXml.NamedInstanceIterator_it_get
    if _newclass:it = property(_CimXml.NamedInstanceIterator_it_get,_CimXml.NamedInstanceIterator_it_set)
    __swig_setmethods__["c"] = _CimXml.NamedInstanceIterator_c_set
    __swig_getmethods__["c"] = _CimXml.NamedInstanceIterator_c_get
    if _newclass:c = property(_CimXml.NamedInstanceIterator_c_get,_CimXml.NamedInstanceIterator_c_set)
    def __init__(self,*args):
        _swig_setattr(self, NamedInstanceIterator, 'this', apply(_CimXml.new_NamedInstanceIterator,args))
        _swig_setattr(self, NamedInstanceIterator, 'thisown', 1)
    def __iter__(*args): return apply(_CimXml.NamedInstanceIterator___iter__,args)
    def next(*args): return apply(_CimXml.NamedInstanceIterator_next,args)
    def __del__(self, destroy= _CimXml.delete_NamedInstanceIterator):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C NamedInstanceIterator instance at %s>" % (self.this,)

class NamedInstanceIteratorPtr(NamedInstanceIterator):
    def __init__(self,this):
        _swig_setattr(self, NamedInstanceIterator, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, NamedInstanceIterator, 'thisown', 0)
        _swig_setattr(self, NamedInstanceIterator,self.__class__,NamedInstanceIterator)
_CimXml.NamedInstanceIterator_swigregister(NamedInstanceIteratorPtr)

class DoesNotExist(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, DoesNotExist, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, DoesNotExist, name)
    __swig_setmethods__["desc"] = _CimXml.DoesNotExist_desc_set
    __swig_getmethods__["desc"] = _CimXml.DoesNotExist_desc_get
    if _newclass:desc = property(_CimXml.DoesNotExist_desc_get,_CimXml.DoesNotExist_desc_set)
    def __init__(self,*args):
        _swig_setattr(self, DoesNotExist, 'this', apply(_CimXml.new_DoesNotExist,args))
        _swig_setattr(self, DoesNotExist, 'thisown', 1)
    def __del__(self, destroy= _CimXml.delete_DoesNotExist):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C DoesNotExist instance at %s>" % (self.this,)

class DoesNotExistPtr(DoesNotExist):
    def __init__(self,this):
        _swig_setattr(self, DoesNotExist, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, DoesNotExist, 'thisown', 0)
        _swig_setattr(self, DoesNotExist,self.__class__,DoesNotExist)
_CimXml.DoesNotExist_swigregister(DoesNotExistPtr)

class stringOption(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, stringOption, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, stringOption, name)
    def __init__(self,*args):
        _swig_setattr(self, stringOption, 'this', apply(_CimXml.new_stringOption,args))
        _swig_setattr(self, stringOption, 'thisown', 1)
    def exists(*args): return apply(_CimXml.stringOption_exists,args)
    def val(*args): return apply(_CimXml.stringOption_val,args)
    def __del__(self, destroy= _CimXml.delete_stringOption):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C stringOption instance at %s>" % (self.this,)

class stringOptionPtr(stringOption):
    def __init__(self,this):
        _swig_setattr(self, stringOption, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, stringOption, 'thisown', 0)
        _swig_setattr(self, stringOption,self.__class__,stringOption)
_CimXml.stringOption_swigregister(stringOptionPtr)

class MethodXml(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, MethodXml, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, MethodXml, name)
    def __init__(self,*args):
        _swig_setattr(self, MethodXml, 'this', apply(_CimXml.new_MethodXml,args))
        _swig_setattr(self, MethodXml, 'thisown', 1)
    def __del__(self, destroy= _CimXml.delete_MethodXml):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C MethodXml instance at %s>" % (self.this,)

class MethodXmlPtr(MethodXml):
    def __init__(self,this):
        _swig_setattr(self, MethodXml, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, MethodXml, 'thisown', 0)
        _swig_setattr(self, MethodXml,self.__class__,MethodXml)
_CimXml.MethodXml_swigregister(MethodXmlPtr)

class QualifierXml(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, QualifierXml, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, QualifierXml, name)
    __swig_setmethods__["str"] = _CimXml.QualifierXml_str_set
    __swig_getmethods__["str"] = _CimXml.QualifierXml_str_get
    if _newclass:str = property(_CimXml.QualifierXml_str_get,_CimXml.QualifierXml_str_set)
    __swig_setmethods__["key"] = _CimXml.QualifierXml_key_set
    __swig_getmethods__["key"] = _CimXml.QualifierXml_key_get
    if _newclass:key = property(_CimXml.QualifierXml_key_get,_CimXml.QualifierXml_key_set)
    def __init__(self,*args):
        _swig_setattr(self, QualifierXml, 'this', apply(_CimXml.new_QualifierXml,args))
        _swig_setattr(self, QualifierXml, 'thisown', 1)
    def __del__(self, destroy= _CimXml.delete_QualifierXml):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C QualifierXml instance at %s>" % (self.this,)

class QualifierXmlPtr(QualifierXml):
    def __init__(self,this):
        _swig_setattr(self, QualifierXml, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, QualifierXml, 'thisown', 0)
        _swig_setattr(self, QualifierXml,self.__class__,QualifierXml)
_CimXml.QualifierXml_swigregister(QualifierXmlPtr)

class AValue(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, AValue, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, AValue, name)
    __swig_setmethods__["attr"] = _CimXml.AValue_attr_set
    __swig_getmethods__["attr"] = _CimXml.AValue_attr_get
    if _newclass:attr = property(_CimXml.AValue_attr_get,_CimXml.AValue_attr_set)
    __swig_setmethods__["val"] = _CimXml.AValue_val_set
    __swig_getmethods__["val"] = _CimXml.AValue_val_get
    if _newclass:val = property(_CimXml.AValue_val_get,_CimXml.AValue_val_set)
    __swig_setmethods__["vals"] = _CimXml.AValue_vals_set
    __swig_getmethods__["vals"] = _CimXml.AValue_vals_get
    if _newclass:vals = property(_CimXml.AValue_vals_get,_CimXml.AValue_vals_set)
    def __init__(self,*args):
        _swig_setattr(self, AValue, 'this', apply(_CimXml.new_AValue,args))
        _swig_setattr(self, AValue, 'thisown', 1)
    def add(*args): return apply(_CimXml.AValue_add,args)
    def __del__(self, destroy= _CimXml.delete_AValue):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C AValue instance at %s>" % (self.this,)

class AValuePtr(AValue):
    def __init__(self,this):
        _swig_setattr(self, AValue, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, AValue, 'thisown', 0)
        _swig_setattr(self, AValue,self.__class__,AValue)
_CimXml.AValue_swigregister(AValuePtr)

class AVPs(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, AVPs, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, AVPs, name)
    def __init__(self,*args):
        _swig_setattr(self, AVPs, 'this', apply(_CimXml.new_AVPs,args))
        _swig_setattr(self, AVPs, 'thisown', 1)
    __swig_getmethods__["makeAVPs"] = lambda x: _CimXml.AVPs_makeAVPs
    if _newclass:makeAVPs = staticmethod(_CimXml.AVPs_makeAVPs)
    def get(*args): return apply(_CimXml.AVPs_get,args)
    def check(*args): return apply(_CimXml.AVPs_check,args)
    def notGotten(*args): return apply(_CimXml.AVPs_notGotten,args)
    def __del__(self, destroy= _CimXml.delete_AVPs):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C AVPs instance at %s>" % (self.this,)

class AVPsPtr(AVPs):
    def __init__(self,this):
        _swig_setattr(self, AVPs, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, AVPs, 'thisown', 0)
        _swig_setattr(self, AVPs,self.__class__,AVPs)
_CimXml.AVPs_swigregister(AVPsPtr)
AVPs_makeAVPs = _CimXml.AVPs_makeAVPs


class AList(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, AList, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, AList, name)
    def __init__(self,*args):
        _swig_setattr(self, AList, 'this', apply(_CimXml.new_AList,args))
        _swig_setattr(self, AList, 'thisown', 1)
    def __del__(self, destroy= _CimXml.delete_AList):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_setmethods__["attrv"] = _CimXml.AList_attrv_set
    __swig_getmethods__["attrv"] = _CimXml.AList_attrv_get
    if _newclass:attrv = property(_CimXml.AList_attrv_get,_CimXml.AList_attrv_set)
    __swig_setmethods__["attrl"] = _CimXml.AList_attrl_set
    __swig_getmethods__["attrl"] = _CimXml.AList_attrl_get
    if _newclass:attrl = property(_CimXml.AList_attrl_get,_CimXml.AList_attrl_set)
    __swig_setmethods__["attrs"] = _CimXml.AList_attrs_set
    __swig_getmethods__["attrs"] = _CimXml.AList_attrs_get
    if _newclass:attrs = property(_CimXml.AList_attrs_get,_CimXml.AList_attrs_set)
    def __repr__(self):
        return "<C AList instance at %s>" % (self.this,)

class AListPtr(AList):
    def __init__(self,this):
        _swig_setattr(self, AList, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, AList, 'thisown', 0)
        _swig_setattr(self, AList,self.__class__,AList)
_CimXml.AList_swigregister(AListPtr)

class XmlException(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, XmlException, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, XmlException, name)
    def getMsg(*args): return apply(_CimXml.XmlException_getMsg,args)
    def __init__(self,*args):
        _swig_setattr(self, XmlException, 'this', apply(_CimXml.new_XmlException,args))
        _swig_setattr(self, XmlException, 'thisown', 1)
    def __del__(self, destroy= _CimXml.delete_XmlException):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C XmlException instance at %s>" % (self.this,)

class XmlExceptionPtr(XmlException):
    def __init__(self,this):
        _swig_setattr(self, XmlException, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, XmlException, 'thisown', 0)
        _swig_setattr(self, XmlException,self.__class__,XmlException)
_CimXml.XmlException_swigregister(XmlExceptionPtr)

class URLException(XmlException):
    __swig_setmethods__ = {}
    for _s in [XmlException]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, URLException, name, value)
    __swig_getmethods__ = {}
    for _s in [XmlException]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, URLException, name)
    def __init__(self,*args):
        _swig_setattr(self, URLException, 'this', apply(_CimXml.new_URLException,args))
        _swig_setattr(self, URLException, 'thisown', 1)
    def __del__(self, destroy= _CimXml.delete_URLException):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C URLException instance at %s>" % (self.this,)

class URLExceptionPtr(URLException):
    def __init__(self,this):
        _swig_setattr(self, URLException, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, URLException, 'thisown', 0)
        _swig_setattr(self, URLException,self.__class__,URLException)
_CimXml.URLException_swigregister(URLExceptionPtr)

class ParseException(XmlException):
    __swig_setmethods__ = {}
    for _s in [XmlException]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, ParseException, name, value)
    __swig_getmethods__ = {}
    for _s in [XmlException]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, ParseException, name)
    def __init__(self,*args):
        _swig_setattr(self, ParseException, 'this', apply(_CimXml.new_ParseException,args))
        _swig_setattr(self, ParseException, 'thisown', 1)
    def __del__(self, destroy= _CimXml.delete_ParseException):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C ParseException instance at %s>" % (self.this,)

class ParseExceptionPtr(ParseException):
    def __init__(self,this):
        _swig_setattr(self, ParseException, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, ParseException, 'thisown', 0)
        _swig_setattr(self, ParseException,self.__class__,ParseException)
_CimXml.ParseException_swigregister(ParseExceptionPtr)

class HttpException(XmlException):
    __swig_setmethods__ = {}
    for _s in [XmlException]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, HttpException, name, value)
    __swig_getmethods__ = {}
    for _s in [XmlException]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, HttpException, name)
    def __init__(self,*args):
        _swig_setattr(self, HttpException, 'this', apply(_CimXml.new_HttpException,args))
        _swig_setattr(self, HttpException, 'thisown', 1)
    def __del__(self, destroy= _CimXml.delete_HttpException):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C HttpException instance at %s>" % (self.this,)

class HttpExceptionPtr(HttpException):
    def __init__(self,this):
        _swig_setattr(self, HttpException, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, HttpException, 'thisown', 0)
        _swig_setattr(self, HttpException,self.__class__,HttpException)
_CimXml.HttpException_swigregister(HttpExceptionPtr)

class CmdException(XmlException):
    __swig_setmethods__ = {}
    for _s in [XmlException]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CmdException, name, value)
    __swig_getmethods__ = {}
    for _s in [XmlException]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CmdException, name)
    def __init__(self,*args):
        _swig_setattr(self, CmdException, 'this', apply(_CimXml.new_CmdException,args))
        _swig_setattr(self, CmdException, 'thisown', 1)
    def __del__(self, destroy= _CimXml.delete_CmdException):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C CmdException instance at %s>" % (self.this,)

class CmdExceptionPtr(CmdException):
    def __init__(self,this):
        _swig_setattr(self, CmdException, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, CmdException, 'thisown', 0)
        _swig_setattr(self, CmdException,self.__class__,CmdException)
_CimXml.CmdException_swigregister(CmdExceptionPtr)

class ErrorXml(XmlException):
    __swig_setmethods__ = {}
    for _s in [XmlException]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, ErrorXml, name, value)
    __swig_getmethods__ = {}
    for _s in [XmlException]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, ErrorXml, name)
    __swig_setmethods__["desc"] = _CimXml.ErrorXml_desc_set
    __swig_getmethods__["desc"] = _CimXml.ErrorXml_desc_get
    if _newclass:desc = property(_CimXml.ErrorXml_desc_get,_CimXml.ErrorXml_desc_set)
    __swig_setmethods__["code"] = _CimXml.ErrorXml_code_set
    __swig_getmethods__["code"] = _CimXml.ErrorXml_code_get
    if _newclass:code = property(_CimXml.ErrorXml_code_get,_CimXml.ErrorXml_code_set)
    def __init__(self,*args):
        _swig_setattr(self, ErrorXml, 'this', apply(_CimXml.new_ErrorXml,args))
        _swig_setattr(self, ErrorXml, 'thisown', 1)
    def __del__(self, destroy= _CimXml.delete_ErrorXml):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C ErrorXml instance at %s>" % (self.this,)

class ErrorXmlPtr(ErrorXml):
    def __init__(self,this):
        _swig_setattr(self, ErrorXml, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, ErrorXml, 'thisown', 0)
        _swig_setattr(self, ErrorXml,self.__class__,ErrorXml)
_CimXml.ErrorXml_swigregister(ErrorXmlPtr)

class KeyValueXml(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, KeyValueXml, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, KeyValueXml, name)
    __swig_setmethods__["valType"] = _CimXml.KeyValueXml_valType_set
    __swig_getmethods__["valType"] = _CimXml.KeyValueXml_valType_get
    if _newclass:valType = property(_CimXml.KeyValueXml_valType_get,_CimXml.KeyValueXml_valType_set)
    __swig_setmethods__["val"] = _CimXml.KeyValueXml_val_set
    __swig_getmethods__["val"] = _CimXml.KeyValueXml_val_get
    if _newclass:val = property(_CimXml.KeyValueXml_val_get,_CimXml.KeyValueXml_val_set)
    def __init__(self,*args):
        _swig_setattr(self, KeyValueXml, 'this', apply(_CimXml.new_KeyValueXml,args))
        _swig_setattr(self, KeyValueXml, 'thisown', 1)
    def toStringBuffer(*args): return apply(_CimXml.KeyValueXml_toStringBuffer,args)
    def __str__(*args): return apply(_CimXml.KeyValueXml___str__,args)
    def __del__(self, destroy= _CimXml.delete_KeyValueXml):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C KeyValueXml instance at %s>" % (self.this,)

class KeyValueXmlPtr(KeyValueXml):
    def __init__(self,this):
        _swig_setattr(self, KeyValueXml, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, KeyValueXml, 'thisown', 0)
        _swig_setattr(self, KeyValueXml,self.__class__,KeyValueXml)
_CimXml.KeyValueXml_swigregister(KeyValueXmlPtr)

class NameSpaceXml(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, NameSpaceXml, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, NameSpaceXml, name)
    __swig_setmethods__["name"] = _CimXml.NameSpaceXml_name_set
    __swig_getmethods__["name"] = _CimXml.NameSpaceXml_name_get
    if _newclass:name = property(_CimXml.NameSpaceXml_name_get,_CimXml.NameSpaceXml_name_set)
    def __init__(self,*args):
        _swig_setattr(self, NameSpaceXml, 'this', apply(_CimXml.new_NameSpaceXml,args))
        _swig_setattr(self, NameSpaceXml, 'thisown', 1)
    def toStringBuffer(*args): return apply(_CimXml.NameSpaceXml_toStringBuffer,args)
    def __del__(self, destroy= _CimXml.delete_NameSpaceXml):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C NameSpaceXml instance at %s>" % (self.this,)

class NameSpaceXmlPtr(NameSpaceXml):
    def __init__(self,this):
        _swig_setattr(self, NameSpaceXml, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, NameSpaceXml, 'thisown', 0)
        _swig_setattr(self, NameSpaceXml,self.__class__,NameSpaceXml)
_CimXml.NameSpaceXml_swigregister(NameSpaceXmlPtr)

class HostXml(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, HostXml, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, HostXml, name)
    __swig_setmethods__["name"] = _CimXml.HostXml_name_set
    __swig_getmethods__["name"] = _CimXml.HostXml_name_get
    if _newclass:name = property(_CimXml.HostXml_name_get,_CimXml.HostXml_name_set)
    def __init__(self,*args):
        _swig_setattr(self, HostXml, 'this', apply(_CimXml.new_HostXml,args))
        _swig_setattr(self, HostXml, 'thisown', 1)
    def __del__(self, destroy= _CimXml.delete_HostXml):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C HostXml instance at %s>" % (self.this,)

class HostXmlPtr(HostXml):
    def __init__(self,this):
        _swig_setattr(self, HostXml, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, HostXml, 'thisown', 0)
        _swig_setattr(self, HostXml,self.__class__,HostXml)
_CimXml.HostXml_swigregister(HostXmlPtr)

class LocalNameSpacePathXml(NameSpaceArray):
    __swig_setmethods__ = {}
    for _s in [NameSpaceArray]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, LocalNameSpacePathXml, name, value)
    __swig_getmethods__ = {}
    for _s in [NameSpaceArray]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, LocalNameSpacePathXml, name)
    def __init__(self,*args):
        _swig_setattr(self, LocalNameSpacePathXml, 'this', apply(_CimXml.new_LocalNameSpacePathXml,args))
        _swig_setattr(self, LocalNameSpacePathXml, 'thisown', 1)
    def __del__(self, destroy= _CimXml.delete_LocalNameSpacePathXml):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C LocalNameSpacePathXml instance at %s>" % (self.this,)

class LocalNameSpacePathXmlPtr(LocalNameSpacePathXml):
    def __init__(self,this):
        _swig_setattr(self, LocalNameSpacePathXml, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, LocalNameSpacePathXml, 'thisown', 0)
        _swig_setattr(self, LocalNameSpacePathXml,self.__class__,LocalNameSpacePathXml)
_CimXml.LocalNameSpacePathXml_swigregister(LocalNameSpacePathXmlPtr)

class NameSpacePathXml(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, NameSpacePathXml, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, NameSpacePathXml, name)
    __swig_setmethods__["host"] = _CimXml.NameSpacePathXml_host_set
    __swig_getmethods__["host"] = _CimXml.NameSpacePathXml_host_get
    if _newclass:host = property(_CimXml.NameSpacePathXml_host_get,_CimXml.NameSpacePathXml_host_set)
    __swig_setmethods__["lns"] = _CimXml.NameSpacePathXml_lns_set
    __swig_getmethods__["lns"] = _CimXml.NameSpacePathXml_lns_get
    if _newclass:lns = property(_CimXml.NameSpacePathXml_lns_get,_CimXml.NameSpacePathXml_lns_set)
    def __init__(self,*args):
        _swig_setattr(self, NameSpacePathXml, 'this', apply(_CimXml.new_NameSpacePathXml,args))
        _swig_setattr(self, NameSpacePathXml, 'thisown', 1)
    def __del__(self, destroy= _CimXml.delete_NameSpacePathXml):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C NameSpacePathXml instance at %s>" % (self.this,)

class NameSpacePathXmlPtr(NameSpacePathXml):
    def __init__(self,this):
        _swig_setattr(self, NameSpacePathXml, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, NameSpacePathXml, 'thisown', 0)
        _swig_setattr(self, NameSpacePathXml,self.__class__,NameSpacePathXml)
_CimXml.NameSpacePathXml_swigregister(NameSpacePathXmlPtr)

class RetValXml(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, RetValXml, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, RetValXml, name)
    __swig_setmethods__["type"] = _CimXml.RetValXml_type_set
    __swig_getmethods__["type"] = _CimXml.RetValXml_type_get
    if _newclass:type = property(_CimXml.RetValXml_type_get,_CimXml.RetValXml_type_set)
    def __del__(self, destroy= _CimXml.delete_RetValXml):
        try:
            if self.thisown: destroy(self)
        except: pass
    def clone(*args): return apply(_CimXml.RetValXml_clone,args)
    def getType(*args): return apply(_CimXml.RetValXml_getType,args)
    def toStringBuffer(*args): return apply(_CimXml.RetValXml_toStringBuffer,args)
    def __str__(*args): return apply(_CimXml.RetValXml___str__,args)
    def __long__(*args): return apply(_CimXml.RetValXml___long__,args)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C RetValXml instance at %s>" % (self.this,)

class RetValXmlPtr(RetValXml):
    def __init__(self,this):
        _swig_setattr(self, RetValXml, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, RetValXml, 'thisown', 0)
        _swig_setattr(self, RetValXml,self.__class__,RetValXml)
_CimXml.RetValXml_swigregister(RetValXmlPtr)
TypeNull = _CimXml.TypeNull
TypeInstance = _CimXml.TypeInstance
TypeInstances = _CimXml.TypeInstances
TypeClass = _CimXml.TypeClass
TypeInstanceName = _CimXml.TypeInstanceName
TypeInstanceNames = _CimXml.TypeInstanceNames
TypeClassName = _CimXml.TypeClassName
TypeClassNames = _CimXml.TypeClassNames
TypeClasses = _CimXml.TypeClasses
TypeNamedInstance = _CimXml.TypeNamedInstance
TypeNamedInstances = _CimXml.TypeNamedInstances
TypeObjectWithPath = _CimXml.TypeObjectWithPath
TypeObjectsWithPath = _CimXml.TypeObjectsWithPath
TypeRValue = _CimXml.TypeRValue

class ValueXml(RetValXml):
    __swig_setmethods__ = {}
    for _s in [RetValXml]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, ValueXml, name, value)
    __swig_getmethods__ = {}
    for _s in [RetValXml]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, ValueXml, name)
    def __del__(self, destroy= _CimXml.delete_ValueXml):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C ValueXml instance at %s>" % (self.this,)

class ValueXmlPtr(ValueXml):
    def __init__(self,this):
        _swig_setattr(self, ValueXml, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, ValueXml, 'thisown', 0)
        _swig_setattr(self, ValueXml,self.__class__,ValueXml)
_CimXml.ValueXml_swigregister(ValueXmlPtr)

class KeyBindingXml(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, KeyBindingXml, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, KeyBindingXml, name)
    __swig_setmethods__["name"] = _CimXml.KeyBindingXml_name_set
    __swig_getmethods__["name"] = _CimXml.KeyBindingXml_name_get
    if _newclass:name = property(_CimXml.KeyBindingXml_name_get,_CimXml.KeyBindingXml_name_set)
    def __init__(self,*args):
        _swig_setattr(self, KeyBindingXml, 'this', apply(_CimXml.new_KeyBindingXml,args))
        _swig_setattr(self, KeyBindingXml, 'thisown', 1)
    def toStringBuffer(*args): return apply(_CimXml.KeyBindingXml_toStringBuffer,args)
    def __str__(*args): return apply(_CimXml.KeyBindingXml___str__,args)
    def __del__(self, destroy= _CimXml.delete_KeyBindingXml):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C KeyBindingXml instance at %s>" % (self.this,)

class KeyBindingXmlPtr(KeyBindingXml):
    def __init__(self,this):
        _swig_setattr(self, KeyBindingXml, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, KeyBindingXml, 'thisown', 0)
        _swig_setattr(self, KeyBindingXml,self.__class__,KeyBindingXml)
_CimXml.KeyBindingXml_swigregister(KeyBindingXmlPtr)

class InstanceNameXml(RetValXml):
    __swig_setmethods__ = {}
    for _s in [RetValXml]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, InstanceNameXml, name, value)
    __swig_getmethods__ = {}
    for _s in [RetValXml]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, InstanceNameXml, name)
    __swig_setmethods__["keys"] = _CimXml.InstanceNameXml_keys_set
    __swig_getmethods__["keys"] = _CimXml.InstanceNameXml_keys_get
    if _newclass:keys = property(_CimXml.InstanceNameXml_keys_get,_CimXml.InstanceNameXml_keys_set)
    def toStringBuffer(*args): return apply(_CimXml.InstanceNameXml_toStringBuffer,args)
    def __del__(self, destroy= _CimXml.delete_InstanceNameXml):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C InstanceNameXml instance at %s>" % (self.this,)

class InstanceNameXmlPtr(InstanceNameXml):
    def __init__(self,this):
        _swig_setattr(self, InstanceNameXml, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, InstanceNameXml, 'thisown', 0)
        _swig_setattr(self, InstanceNameXml,self.__class__,InstanceNameXml)
_CimXml.InstanceNameXml_swigregister(InstanceNameXmlPtr)

class InstanceNamesXml(RetValXml,InstanceNameArray):
    __swig_setmethods__ = {}
    for _s in [RetValXml,InstanceNameArray]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, InstanceNamesXml, name, value)
    __swig_getmethods__ = {}
    for _s in [RetValXml,InstanceNameArray]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, InstanceNamesXml, name)
    def __del__(self, destroy= _CimXml.delete_InstanceNamesXml):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C InstanceNamesXml instance at %s>" % (self.this,)

class InstanceNamesXmlPtr(InstanceNamesXml):
    def __init__(self,this):
        _swig_setattr(self, InstanceNamesXml, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, InstanceNamesXml, 'thisown', 0)
        _swig_setattr(self, InstanceNamesXml,self.__class__,InstanceNamesXml)
_CimXml.InstanceNamesXml_swigregister(InstanceNamesXmlPtr)

class PropertyXml(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, PropertyXml, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, PropertyXml, name)
    __swig_setmethods__["name"] = _CimXml.PropertyXml_name_set
    __swig_getmethods__["name"] = _CimXml.PropertyXml_name_get
    if _newclass:name = property(_CimXml.PropertyXml_name_get,_CimXml.PropertyXml_name_set)
    __swig_setmethods__["type"] = _CimXml.PropertyXml_type_set
    __swig_getmethods__["type"] = _CimXml.PropertyXml_type_get
    if _newclass:type = property(_CimXml.PropertyXml_type_get,_CimXml.PropertyXml_type_set)
    __swig_setmethods__["origin"] = _CimXml.PropertyXml_origin_set
    __swig_getmethods__["origin"] = _CimXml.PropertyXml_origin_get
    if _newclass:origin = property(_CimXml.PropertyXml_origin_get,_CimXml.PropertyXml_origin_set)
    __swig_setmethods__["propagated"] = _CimXml.PropertyXml_propagated_set
    __swig_getmethods__["propagated"] = _CimXml.PropertyXml_propagated_get
    if _newclass:propagated = property(_CimXml.PropertyXml_propagated_get,_CimXml.PropertyXml_propagated_set)
    __swig_setmethods__["refCls"] = _CimXml.PropertyXml_refCls_set
    __swig_getmethods__["refCls"] = _CimXml.PropertyXml_refCls_get
    if _newclass:refCls = property(_CimXml.PropertyXml_refCls_get,_CimXml.PropertyXml_refCls_set)
    __swig_setmethods__["arraySize"] = _CimXml.PropertyXml_arraySize_set
    __swig_getmethods__["arraySize"] = _CimXml.PropertyXml_arraySize_get
    if _newclass:arraySize = property(_CimXml.PropertyXml_arraySize_get,_CimXml.PropertyXml_arraySize_set)
    __swig_setmethods__["pType"] = _CimXml.PropertyXml_pType_set
    __swig_getmethods__["pType"] = _CimXml.PropertyXml_pType_get
    if _newclass:pType = property(_CimXml.PropertyXml_pType_get,_CimXml.PropertyXml_pType_set)
    __swig_setmethods__["str"] = _CimXml.PropertyXml_str_set
    __swig_getmethods__["str"] = _CimXml.PropertyXml_str_get
    if _newclass:str = property(_CimXml.PropertyXml_str_get,_CimXml.PropertyXml_str_set)
    __swig_setmethods__["qualifiers"] = _CimXml.PropertyXml_qualifiers_set
    __swig_getmethods__["qualifiers"] = _CimXml.PropertyXml_qualifiers_get
    if _newclass:qualifiers = property(_CimXml.PropertyXml_qualifiers_get,_CimXml.PropertyXml_qualifiers_set)
    __swig_setmethods__["value"] = _CimXml.PropertyXml_value_set
    __swig_getmethods__["value"] = _CimXml.PropertyXml_value_get
    if _newclass:value = property(_CimXml.PropertyXml_value_get,_CimXml.PropertyXml_value_set)
    __swig_setmethods__["key"] = _CimXml.PropertyXml_key_set
    __swig_getmethods__["key"] = _CimXml.PropertyXml_key_get
    if _newclass:key = property(_CimXml.PropertyXml_key_get,_CimXml.PropertyXml_key_set)
    def __init__(self,*args):
        _swig_setattr(self, PropertyXml, 'this', apply(_CimXml.new_PropertyXml,args))
        _swig_setattr(self, PropertyXml, 'thisown', 1)
    def addQualifier(*args): return apply(_CimXml.PropertyXml_addQualifier,args)
    def toStringBuffer(*args): return apply(_CimXml.PropertyXml_toStringBuffer,args)
    def toXmlBuffer(*args): return apply(_CimXml.PropertyXml_toXmlBuffer,args)
    def __str__(*args): return apply(_CimXml.PropertyXml___str__,args)
    def __del__(self, destroy= _CimXml.delete_PropertyXml):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C PropertyXml instance at %s>" % (self.this,)

class PropertyXmlPtr(PropertyXml):
    def __init__(self,this):
        _swig_setattr(self, PropertyXml, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, PropertyXml, 'thisown', 0)
        _swig_setattr(self, PropertyXml,self.__class__,PropertyXml)
_CimXml.PropertyXml_swigregister(PropertyXmlPtr)
TypeProperty = _CimXml.TypeProperty
TypePropertyRef = _CimXml.TypePropertyRef
TypePropertyArray = _CimXml.TypePropertyArray

class ClassXml(RetValXml):
    __swig_setmethods__ = {}
    for _s in [RetValXml]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, ClassXml, name, value)
    __swig_getmethods__ = {}
    for _s in [RetValXml]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, ClassXml, name)
    __swig_setmethods__["clsName"] = _CimXml.ClassXml_clsName_set
    __swig_getmethods__["clsName"] = _CimXml.ClassXml_clsName_get
    if _newclass:clsName = property(_CimXml.ClassXml_clsName_get,_CimXml.ClassXml_clsName_set)
    __swig_setmethods__["superClass"] = _CimXml.ClassXml_superClass_set
    __swig_getmethods__["superClass"] = _CimXml.ClassXml_superClass_get
    if _newclass:superClass = property(_CimXml.ClassXml_superClass_get,_CimXml.ClassXml_superClass_set)
    def getProperties(*args): return apply(_CimXml.ClassXml_getProperties,args)
    def getQualifiers(*args): return apply(_CimXml.ClassXml_getQualifiers,args)
    def getKeys(*args): return apply(_CimXml.ClassXml_getKeys,args)
    def addQualifier(*args): return apply(_CimXml.ClassXml_addQualifier,args)
    def addProperty(*args): return apply(_CimXml.ClassXml_addProperty,args)
    def addMethod(*args): return apply(_CimXml.ClassXml_addMethod,args)
    def toStringBuffer(*args): return apply(_CimXml.ClassXml_toStringBuffer,args)
    def keysOk(*args): return apply(_CimXml.ClassXml_keysOk,args)
    def __del__(self, destroy= _CimXml.delete_ClassXml):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C ClassXml instance at %s>" % (self.this,)

class ClassXmlPtr(ClassXml):
    def __init__(self,this):
        _swig_setattr(self, ClassXml, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, ClassXml, 'thisown', 0)
        _swig_setattr(self, ClassXml,self.__class__,ClassXml)
_CimXml.ClassXml_swigregister(ClassXmlPtr)

class ClassesXml(RetValXml,ClassArray):
    __swig_setmethods__ = {}
    for _s in [RetValXml,ClassArray]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, ClassesXml, name, value)
    __swig_getmethods__ = {}
    for _s in [RetValXml,ClassArray]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, ClassesXml, name)
    def __del__(self, destroy= _CimXml.delete_ClassesXml):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C ClassesXml instance at %s>" % (self.this,)

class ClassesXmlPtr(ClassesXml):
    def __init__(self,this):
        _swig_setattr(self, ClassesXml, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, ClassesXml, 'thisown', 0)
        _swig_setattr(self, ClassesXml,self.__class__,ClassesXml)
_CimXml.ClassesXml_swigregister(ClassesXmlPtr)

class InstanceXml(RetValXml):
    __swig_setmethods__ = {}
    for _s in [RetValXml]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, InstanceXml, name, value)
    __swig_getmethods__ = {}
    for _s in [RetValXml]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, InstanceXml, name)
    def propagateKeyFlags(*args): return apply(_CimXml.InstanceXml_propagateKeyFlags,args)
    def getProperties(*args): return apply(_CimXml.InstanceXml_getProperties,args)
    def getQualifiers(*args): return apply(_CimXml.InstanceXml_getQualifiers,args)
    def addQualifier(*args): return apply(_CimXml.InstanceXml_addQualifier,args)
    def addProperty(*args): return apply(_CimXml.InstanceXml_addProperty,args)
    def toStringBuffer(*args): return apply(_CimXml.InstanceXml_toStringBuffer,args)
    def __del__(self, destroy= _CimXml.delete_InstanceXml):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C InstanceXml instance at %s>" % (self.this,)

class InstanceXmlPtr(InstanceXml):
    def __init__(self,this):
        _swig_setattr(self, InstanceXml, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, InstanceXml, 'thisown', 0)
        _swig_setattr(self, InstanceXml,self.__class__,InstanceXml)
_CimXml.InstanceXml_swigregister(InstanceXmlPtr)

class NamedInstanceXml(RetValXml):
    __swig_setmethods__ = {}
    for _s in [RetValXml]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, NamedInstanceXml, name, value)
    __swig_getmethods__ = {}
    for _s in [RetValXml]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, NamedInstanceXml, name)
    __swig_setmethods__["instname"] = _CimXml.NamedInstanceXml_instname_set
    __swig_getmethods__["instname"] = _CimXml.NamedInstanceXml_instname_get
    if _newclass:instname = property(_CimXml.NamedInstanceXml_instname_get,_CimXml.NamedInstanceXml_instname_set)
    __swig_setmethods__["inst"] = _CimXml.NamedInstanceXml_inst_set
    __swig_getmethods__["inst"] = _CimXml.NamedInstanceXml_inst_get
    if _newclass:inst = property(_CimXml.NamedInstanceXml_inst_get,_CimXml.NamedInstanceXml_inst_set)
    def __del__(self, destroy= _CimXml.delete_NamedInstanceXml):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C NamedInstanceXml instance at %s>" % (self.this,)

class NamedInstanceXmlPtr(NamedInstanceXml):
    def __init__(self,this):
        _swig_setattr(self, NamedInstanceXml, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, NamedInstanceXml, 'thisown', 0)
        _swig_setattr(self, NamedInstanceXml,self.__class__,NamedInstanceXml)
_CimXml.NamedInstanceXml_swigregister(NamedInstanceXmlPtr)

class InstancesXml(RetValXml,InstanceArray):
    __swig_setmethods__ = {}
    for _s in [RetValXml,InstanceArray]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, InstancesXml, name, value)
    __swig_getmethods__ = {}
    for _s in [RetValXml,InstanceArray]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, InstancesXml, name)
    def __del__(self, destroy= _CimXml.delete_InstancesXml):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C InstancesXml instance at %s>" % (self.this,)

class InstancesXmlPtr(InstancesXml):
    def __init__(self,this):
        _swig_setattr(self, InstancesXml, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, InstancesXml, 'thisown', 0)
        _swig_setattr(self, InstancesXml,self.__class__,InstancesXml)
_CimXml.InstancesXml_swigregister(InstancesXmlPtr)

class ObjectWithPathXml(RetValXml):
    __swig_setmethods__ = {}
    for _s in [RetValXml]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, ObjectWithPathXml, name, value)
    __swig_getmethods__ = {}
    for _s in [RetValXml]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, ObjectWithPathXml, name)
    __swig_setmethods__["ip"] = _CimXml.ObjectWithPathXml_ip_set
    __swig_getmethods__["ip"] = _CimXml.ObjectWithPathXml_ip_get
    if _newclass:ip = property(_CimXml.ObjectWithPathXml_ip_get,_CimXml.ObjectWithPathXml_ip_set)
    __swig_setmethods__["inst"] = _CimXml.ObjectWithPathXml_inst_set
    __swig_getmethods__["inst"] = _CimXml.ObjectWithPathXml_inst_get
    if _newclass:inst = property(_CimXml.ObjectWithPathXml_inst_get,_CimXml.ObjectWithPathXml_inst_set)
    def __del__(self, destroy= _CimXml.delete_ObjectWithPathXml):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C ObjectWithPathXml instance at %s>" % (self.this,)

class ObjectWithPathXmlPtr(ObjectWithPathXml):
    def __init__(self,this):
        _swig_setattr(self, ObjectWithPathXml, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, ObjectWithPathXml, 'thisown', 0)
        _swig_setattr(self, ObjectWithPathXml,self.__class__,ObjectWithPathXml)
_CimXml.ObjectWithPathXml_swigregister(ObjectWithPathXmlPtr)

class ObjectsWithPathXml(RetValXml):
    __swig_setmethods__ = {}
    for _s in [RetValXml]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, ObjectsWithPathXml, name, value)
    __swig_getmethods__ = {}
    for _s in [RetValXml]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, ObjectsWithPathXml, name)
    __swig_setmethods__["ia"] = _CimXml.ObjectsWithPathXml_ia_set
    __swig_getmethods__["ia"] = _CimXml.ObjectsWithPathXml_ia_get
    if _newclass:ia = property(_CimXml.ObjectsWithPathXml_ia_get,_CimXml.ObjectsWithPathXml_ia_set)
    __swig_setmethods__["ipa"] = _CimXml.ObjectsWithPathXml_ipa_set
    __swig_getmethods__["ipa"] = _CimXml.ObjectsWithPathXml_ipa_get
    if _newclass:ipa = property(_CimXml.ObjectsWithPathXml_ipa_get,_CimXml.ObjectsWithPathXml_ipa_set)
    def add(*args): return apply(_CimXml.ObjectsWithPathXml_add,args)
    def getName(*args): return apply(_CimXml.ObjectsWithPathXml_getName,args)
    def getInst(*args): return apply(_CimXml.ObjectsWithPathXml_getInst,args)
    def size(*args): return apply(_CimXml.ObjectsWithPathXml_size,args)
    def __del__(self, destroy= _CimXml.delete_ObjectsWithPathXml):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C ObjectsWithPathXml instance at %s>" % (self.this,)

class ObjectsWithPathXmlPtr(ObjectsWithPathXml):
    def __init__(self,this):
        _swig_setattr(self, ObjectsWithPathXml, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, ObjectsWithPathXml, 'thisown', 0)
        _swig_setattr(self, ObjectsWithPathXml,self.__class__,ObjectsWithPathXml)
_CimXml.ObjectsWithPathXml_swigregister(ObjectsWithPathXmlPtr)

class NamedInstancesXml(RetValXml):
    __swig_setmethods__ = {}
    for _s in [RetValXml]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, NamedInstancesXml, name, value)
    __swig_getmethods__ = {}
    for _s in [RetValXml]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, NamedInstancesXml, name)
    __swig_setmethods__["ia"] = _CimXml.NamedInstancesXml_ia_set
    __swig_getmethods__["ia"] = _CimXml.NamedInstancesXml_ia_get
    if _newclass:ia = property(_CimXml.NamedInstancesXml_ia_get,_CimXml.NamedInstancesXml_ia_set)
    __swig_setmethods__["ina"] = _CimXml.NamedInstancesXml_ina_set
    __swig_getmethods__["ina"] = _CimXml.NamedInstancesXml_ina_get
    if _newclass:ina = property(_CimXml.NamedInstancesXml_ina_get,_CimXml.NamedInstancesXml_ina_set)
    def add(*args): return apply(_CimXml.NamedInstancesXml_add,args)
    def getName(*args): return apply(_CimXml.NamedInstancesXml_getName,args)
    def getInst(*args): return apply(_CimXml.NamedInstancesXml_getInst,args)
    def __len__(*args): return apply(_CimXml.NamedInstancesXml___len__,args)
    def __del__(self, destroy= _CimXml.delete_NamedInstancesXml):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C NamedInstancesXml instance at %s>" % (self.this,)

class NamedInstancesXmlPtr(NamedInstancesXml):
    def __init__(self,this):
        _swig_setattr(self, NamedInstancesXml, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, NamedInstancesXml, 'thisown', 0)
        _swig_setattr(self, NamedInstancesXml,self.__class__,NamedInstancesXml)
_CimXml.NamedInstancesXml_swigregister(NamedInstancesXmlPtr)

class ClassNameXml(RetValXml):
    __swig_setmethods__ = {}
    for _s in [RetValXml]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, ClassNameXml, name, value)
    __swig_getmethods__ = {}
    for _s in [RetValXml]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, ClassNameXml, name)
    def toStringBuffer(*args): return apply(_CimXml.ClassNameXml_toStringBuffer,args)
    def __del__(self, destroy= _CimXml.delete_ClassNameXml):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C ClassNameXml instance at %s>" % (self.this,)

class ClassNameXmlPtr(ClassNameXml):
    def __init__(self,this):
        _swig_setattr(self, ClassNameXml, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, ClassNameXml, 'thisown', 0)
        _swig_setattr(self, ClassNameXml,self.__class__,ClassNameXml)
_CimXml.ClassNameXml_swigregister(ClassNameXmlPtr)

class ClassNamesXml(RetValXml,ClassNameArray):
    __swig_setmethods__ = {}
    for _s in [RetValXml,ClassNameArray]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, ClassNamesXml, name, value)
    __swig_getmethods__ = {}
    for _s in [RetValXml,ClassNameArray]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, ClassNamesXml, name)
    def __del__(self, destroy= _CimXml.delete_ClassNamesXml):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C ClassNamesXml instance at %s>" % (self.this,)

class ClassNamesXmlPtr(ClassNamesXml):
    def __init__(self,this):
        _swig_setattr(self, ClassNamesXml, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, ClassNamesXml, 'thisown', 0)
        _swig_setattr(self, ClassNamesXml,self.__class__,ClassNamesXml)
_CimXml.ClassNamesXml_swigregister(ClassNamesXmlPtr)

class IReturnvalueXml(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, IReturnvalueXml, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, IReturnvalueXml, name)
    def __init__(self,*args):
        _swig_setattr(self, IReturnvalueXml, 'this', apply(_CimXml.new_IReturnvalueXml,args))
        _swig_setattr(self, IReturnvalueXml, 'thisown', 1)
    def getReturnData(*args): return apply(_CimXml.IReturnvalueXml_getReturnData,args)
    def __del__(self, destroy= _CimXml.delete_IReturnvalueXml):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C IReturnvalueXml instance at %s>" % (self.this,)

class IReturnvalueXmlPtr(IReturnvalueXml):
    def __init__(self,this):
        _swig_setattr(self, IReturnvalueXml, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, IReturnvalueXml, 'thisown', 0)
        _swig_setattr(self, IReturnvalueXml,self.__class__,IReturnvalueXml)
_CimXml.IReturnvalueXml_swigregister(IReturnvalueXmlPtr)

class IMethodresponseXml(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, IMethodresponseXml, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, IMethodresponseXml, name)
    def __init__(self,*args):
        _swig_setattr(self, IMethodresponseXml, 'this', apply(_CimXml.new_IMethodresponseXml,args))
        _swig_setattr(self, IMethodresponseXml, 'thisown', 1)
    def getReturnData(*args): return apply(_CimXml.IMethodresponseXml_getReturnData,args)
    def __del__(self, destroy= _CimXml.delete_IMethodresponseXml):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C IMethodresponseXml instance at %s>" % (self.this,)

class IMethodresponseXmlPtr(IMethodresponseXml):
    def __init__(self,this):
        _swig_setattr(self, IMethodresponseXml, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, IMethodresponseXml, 'thisown', 0)
        _swig_setattr(self, IMethodresponseXml,self.__class__,IMethodresponseXml)
_CimXml.IMethodresponseXml_swigregister(IMethodresponseXmlPtr)

class SimplerspXml(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, SimplerspXml, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, SimplerspXml, name)
    def __init__(self,*args):
        _swig_setattr(self, SimplerspXml, 'this', apply(_CimXml.new_SimplerspXml,args))
        _swig_setattr(self, SimplerspXml, 'thisown', 1)
    def getReturnData(*args): return apply(_CimXml.SimplerspXml_getReturnData,args)
    def __del__(self, destroy= _CimXml.delete_SimplerspXml):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C SimplerspXml instance at %s>" % (self.this,)

class SimplerspXmlPtr(SimplerspXml):
    def __init__(self,this):
        _swig_setattr(self, SimplerspXml, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, SimplerspXml, 'thisown', 0)
        _swig_setattr(self, SimplerspXml,self.__class__,SimplerspXml)
_CimXml.SimplerspXml_swigregister(SimplerspXmlPtr)

class MessageXml(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, MessageXml, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, MessageXml, name)
    def __init__(self,*args):
        _swig_setattr(self, MessageXml, 'this', apply(_CimXml.new_MessageXml,args))
        _swig_setattr(self, MessageXml, 'thisown', 1)
    def getResponse(*args): return apply(_CimXml.MessageXml_getResponse,args)
    def __del__(self, destroy= _CimXml.delete_MessageXml):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C MessageXml instance at %s>" % (self.this,)

class MessageXmlPtr(MessageXml):
    def __init__(self,this):
        _swig_setattr(self, MessageXml, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, MessageXml, 'thisown', 0)
        _swig_setattr(self, MessageXml,self.__class__,MessageXml)
_CimXml.MessageXml_swigregister(MessageXmlPtr)

class CimXml(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, CimXml, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, CimXml, name)
    def __init__(self,*args):
        _swig_setattr(self, CimXml, 'this', apply(_CimXml.new_CimXml,args))
        _swig_setattr(self, CimXml, 'thisown', 1)
    def getResponse(*args): return apply(_CimXml.CimXml_getResponse,args)
    def __del__(self, destroy= _CimXml.delete_CimXml):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C CimXml instance at %s>" % (self.this,)

class CimXmlPtr(CimXml):
    def __init__(self,this):
        _swig_setattr(self, CimXml, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, CimXml, 'thisown', 0)
        _swig_setattr(self, CimXml,self.__class__,CimXml)
_CimXml.CimXml_swigregister(CimXmlPtr)

class URL(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, URL, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, URL, name)
    __swig_setmethods__["host"] = _CimXml.URL_host_set
    __swig_getmethods__["host"] = _CimXml.URL_host_get
    if _newclass:host = property(_CimXml.URL_host_get,_CimXml.URL_host_set)
    __swig_setmethods__["port"] = _CimXml.URL_port_set
    __swig_getmethods__["port"] = _CimXml.URL_port_get
    if _newclass:port = property(_CimXml.URL_port_get,_CimXml.URL_port_set)
    __swig_setmethods__["cName"] = _CimXml.URL_cName_set
    __swig_getmethods__["cName"] = _CimXml.URL_cName_get
    if _newclass:cName = property(_CimXml.URL_cName_get,_CimXml.URL_cName_set)
    __swig_setmethods__["user"] = _CimXml.URL_user_set
    __swig_getmethods__["user"] = _CimXml.URL_user_get
    if _newclass:user = property(_CimXml.URL_user_get,_CimXml.URL_user_set)
    __swig_setmethods__["password"] = _CimXml.URL_password_set
    __swig_getmethods__["password"] = _CimXml.URL_password_get
    if _newclass:password = property(_CimXml.URL_password_get,_CimXml.URL_password_set)
    __swig_setmethods__["ns"] = _CimXml.URL_ns_set
    __swig_getmethods__["ns"] = _CimXml.URL_ns_get
    if _newclass:ns = property(_CimXml.URL_ns_get,_CimXml.URL_ns_set)
    __swig_setmethods__["keys"] = _CimXml.URL_keys_set
    __swig_getmethods__["keys"] = _CimXml.URL_keys_get
    if _newclass:keys = property(_CimXml.URL_keys_get,_CimXml.URL_keys_set)
    def __init__(self,*args):
        _swig_setattr(self, URL, 'this', apply(_CimXml.new_URL,args))
        _swig_setattr(self, URL, 'thisown', 1)
    def resolveIndirection(*args): return apply(_CimXml.URL_resolveIndirection,args)
    def refLookAhead(*args): return apply(_CimXml.URL_refLookAhead,args)
    def toStringBuffer(*args): return apply(_CimXml.URL_toStringBuffer,args)
    def __str__(*args): return apply(_CimXml.URL___str__,args)
    def __del__(self, destroy= _CimXml.delete_URL):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C URL instance at %s>" % (self.this,)

class URLPtr(URL):
    def __init__(self,this):
        _swig_setattr(self, URL, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, URL, 'thisown', 0)
        _swig_setattr(self, URL,self.__class__,URL)
_CimXml.URL_swigregister(URLPtr)

class Cimom(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, Cimom, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, Cimom, name)
    def __del__(self, destroy= _CimXml.delete_Cimom):
        try:
            if self.thisown: destroy(self)
        except: pass
    def getClass(*args): return apply(_CimXml.Cimom_getClass,args)
    def getClass2str(*args): return apply(_CimXml.Cimom_getClass2str,args)
    def testInst(*args): return apply(_CimXml.Cimom_testInst,args)
    def getInstance(*args): return apply(_CimXml.Cimom_getInstance,args)
    def enumInstances(*args): return apply(_CimXml.Cimom_enumInstances,args)
    def enumInstances2str(*args): return apply(_CimXml.Cimom_enumInstances2str,args)
    def createInstance(*args): return apply(_CimXml.Cimom_createInstance,args)
    def enumInstanceNames(*args): return apply(_CimXml.Cimom_enumInstanceNames,args)
    def enumClasses(*args): return apply(_CimXml.Cimom_enumClasses,args)
    def enumClassNames(*args): return apply(_CimXml.Cimom_enumClassNames,args)
    def modifyInstance(*args): return apply(_CimXml.Cimom_modifyInstance,args)
    def associatorNames(*args): return apply(_CimXml.Cimom_associatorNames,args)
    def associators(*args): return apply(_CimXml.Cimom_associators,args)
    def associators2str(*args): return apply(_CimXml.Cimom_associators2str,args)
    def referenceNames(*args): return apply(_CimXml.Cimom_referenceNames,args)
    def references(*args): return apply(_CimXml.Cimom_references,args)
    def references2str(*args): return apply(_CimXml.Cimom_references2str,args)
    def getProperty(*args): return apply(_CimXml.Cimom_getProperty,args)
    def setProperty(*args): return apply(_CimXml.Cimom_setProperty,args)
    def deleteClass(*args): return apply(_CimXml.Cimom_deleteClass,args)
    def deleteInstance(*args): return apply(_CimXml.Cimom_deleteInstance,args)
    __swig_getmethods__["unescapeXml"] = lambda x: _CimXml.Cimom_unescapeXml
    if _newclass:unescapeXml = staticmethod(_CimXml.Cimom_unescapeXml)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C Cimom instance at %s>" % (self.this,)

class CimomPtr(Cimom):
    def __init__(self,this):
        _swig_setattr(self, Cimom, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, Cimom, 'thisown', 0)
        _swig_setattr(self, Cimom,self.__class__,Cimom)
_CimXml.Cimom_swigregister(CimomPtr)
Cimom_unescapeXml = _CimXml.Cimom_unescapeXml


class CimomGhttp(Cimom):
    __swig_setmethods__ = {}
    for _s in [Cimom]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CimomGhttp, name, value)
    __swig_getmethods__ = {}
    for _s in [Cimom]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CimomGhttp, name)
    def __init__(self,*args):
        _swig_setattr(self, CimomGhttp, 'this', apply(_CimXml.new_CimomGhttp,args))
        _swig_setattr(self, CimomGhttp, 'thisown', 1)
    def __del__(self, destroy= _CimXml.delete_CimomGhttp):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C CimomGhttp instance at %s>" % (self.this,)

class CimomGhttpPtr(CimomGhttp):
    def __init__(self,this):
        _swig_setattr(self, CimomGhttp, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, CimomGhttp, 'thisown', 0)
        _swig_setattr(self, CimomGhttp,self.__class__,CimomGhttp)
_CimXml.CimomGhttp_swigregister(CimomGhttpPtr)

GetInstance = _CimXml.GetInstance
CreateInstance = _CimXml.CreateInstance
ModifyInstance = _CimXml.ModifyInstance
DeleteInstance = _CimXml.DeleteInstance
EnumInstanceNames = _CimXml.EnumInstanceNames
EnumInstances = _CimXml.EnumInstances
GetClass = _CimXml.GetClass
EnumClasses = _CimXml.EnumClasses
EnumClassNames = _CimXml.EnumClassNames
DeleteClass = _CimXml.DeleteClass
AssociatorNames = _CimXml.AssociatorNames
Associators = _CimXml.Associators
ReferenceNames = _CimXml.ReferenceNames
References = _CimXml.References
GetProperty = _CimXml.GetProperty
SetProperty = _CimXml.SetProperty

