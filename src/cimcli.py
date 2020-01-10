/*
 *
 * $Id: cimcli.py,v 1.2 2008/08/07 18:25:41 tyreld Exp $
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

import CimXml
import os
import string
import re

cimom = CimXml.CimomGhttp();

def cimpath(machstr):
    return lambda q, machstr=machstr: CimXml.URL("http://"+machstr.__str__()+":5988/root/cimv2:"+q.__str__())

def setmachine(machstr, locals):
    locals['cimtarget'] = cimpath(machstr)

def ecn(obj, cimt=None):
    if not cimt: cimt = cimtarget
    url = cimt(obj)
    return cimom.enumClassNames(url)
    
def ei(obj, cimt=None):
    if not cimt: cimt = cimtarget
    url = cimt(obj)
    if not url.cName:
        raise RuntimeException("Class name not specified")
    return cimom.enumInstances(url,"")

def ein(obj, cimt=None):
    if not cimt: cimt = cimtarget
    url = cimt(obj)
    if not url.cName:
        raise RuntimeException("Class name not specified")
    return cimom.enumInstanceNames(url)

def gi(obj, cimt=None):
    if not cimt: cimt = cimtarget
    url = cimt(obj)
    if not url.cName:
        raise RuntimeException("Class name not specified")
    if len(url.keys) == 0:
        raise RuntimeException("Keys not specified")
    return cimom.getInstance(url,"")

def interesting(str):
     return (str.find("teresting") > 0)

dqMatcher = re.compile("[0-9]{1,3}[.][0-9]{1,3}[.][0-9]{1,3}[.][0-9]{1,3}")

def getdottedquad(str):
     m = dqMatcher.search(str)
     if m: return m.group()
     return None

def notNone(n):
     return n != None;

def getCIMOM_IPs_ping(dotted_quad, bits):
     file = os.popen("nmap -p 5988 "+dotted_quad+"/"+bits, "r")
     output = file.readlines()
     goodlines = filter(interesting, output)
     machines  = filter(notNone, map(getdottedquad, goodlines))
     return machines

def getCIMOMs(dotted_quad, bits=32, method="PING"):
     """ getCIMOMs(dotted_quad: string, bits: string, method: string)
           dotted_quad: The IP address your masking to get the subnet
           bits:        The number of high bits that are important
           method:      "PING" - ping port 5988 and see who answers
                        "SLP" - use SLP to query who has a CIM server """
     IPs = []
     dq = string.split(dotted_quad,".")
     if len(dq) != 4:
          raise RuntimeException("getcimservers expects dotted quads")
     b = int(bits);
     if b != 8 and b !=16 and b != 24 and b != 32:
          raise RuntimeException("must specify 8, 16, 24, or 32 bits to use as mask")
     # I excplicitly repackage the dotted quad to make sure we really got a
     # dotted quad.  If not, int() will raise an exception
     checked_dq = int(dq[0]).__str__()+"."+int(dq[1]).__str__()+"."\
                  +int(dq[2]).__str__()+"."+int(dq[3]).__str__()
     checked_bits = b.__str__()

     if method=="PING":
          print "Portscanning port 5988 on subnet "+checked_dq + "/" +checked_bits+" for prospective CIMOMs"
          IPs = getCIMOM_IPs_ping(checked_dq, checked_bits)
     elif method=="SLP":
          raise RuntimeException("SLP detection of CIMOMs not support at this time")
     else:
          raise RuntimeException("Unknown CIMOM detection method")
     print "IPs found: ",IPs
     if not IPs: return []
     return map(cimpath,IPs)
