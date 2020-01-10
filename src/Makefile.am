# $Id: Makefile.am,v 1.7 2005/11/21 12:33:19 mihajlov Exp $

if HAVE_SWIG
  bin_PROGRAMS = wbemcli clipy
else
  bin_PROGRAMS = wbemcli 
endif

wbemcli_SOURCES = main.cpp CimXml.h CimXml.cpp SunSMC.cpp 
EXTRA_wbemcli_SOURCES =	Cimghttp.h CimWinHttp.h  Cimwww.h CimCurl.h \
	Cimghttp.cpp  CimWinHttp.cpp  Cimwww.cpp CimCurl.cpp
wbemcli_CPPFLAGS = -DCACERT=\"@CACERT@\"
wbemcli_LDADD = @CIMHTTP@
wbemcli_DEPENDENCIES = @CIMHTTP@

if HAVE_SWIG
  clipy_SOURCES = CimXml_wrap.cpp CimXml.h CimXml.cpp
  clipy_LDADD = @CIMHTTP@
  clipy_DEPENDENCIES = @CIMHTTP@
  clipy_CPPFLAGS = @SWIGINCLUDE@ $(AM_CPPFLAGS)
endif

MANFILES=man/wbemcli.1

man_MANS=$(MANFILES)

EXTRA_DIST=$(man_MANS) sblim-wbemcli.spec samples \
	README.pycli wbemcli.ind cimcli.py CimXml.py CimXml.i

SUFFIXES=.1 .1.pre

.1.pre.1:
	sed -e s?$$\{prefix\}?$(prefix)?g \
	-e s?$$\{exec_prefix\}?$(prefix)?g $< > $@

if HAVE_SWIG
.cxx.cpp:
	mv $< $@

CimXml_wrap.cxx: CimXml.i
	swig -c++ -python CimXml.i
endif

clean-local:
	rm -f wbemcli $(MANFILES) 

install-exec-hook:
	cd $(DESTDIR)$(bindir) && \
	       $(RM) wbemgi$(EXEEXT) && \
               $(RM) wbemci$(EXEEXT) && \
               $(RM) wbemdi$(EXEEXT) && \
               $(RM) wbemecn$(EXEEXT) && \
               $(RM) wbemein$(EXEEXT) && \
               $(RM) wbemec$(EXEEXT) && \
               $(RM) wbemei$(EXEEXT) && \
               $(RM) wbemgc$(EXEEXT) && \
               $(RM) wbemmi$(EXEEXT) && \
               $(RM) wbemdc$(EXEEXT) && \
               $(RM) wbemgcd$(EXEEXT) && \
               $(RM) wbemain$(EXEEXT) && \
               $(RM) wbemai$(EXEEXT) && \
               $(RM) wbemrin$(EXEEXT) && \
               $(RM) wbemri$(EXEEXT) && \
               $(RM) wbemgp$(EXEEXT) && \
               $(RM) wbemsp$(EXEEXT) && \
               $(RM) wbemcm$(EXEEXT) && \
               $(RM) wbemcmx$(EXEEXT) && \
	       $(LN_S) wbemcli$(EXEEXT) wbemgi$(EXEEXT) && \
               $(LN_S) wbemcli$(EXEEXT) wbemci$(EXEEXT) && \
               $(LN_S) wbemcli$(EXEEXT) wbemdi$(EXEEXT) && \
               $(LN_S) wbemcli$(EXEEXT) wbemecn$(EXEEXT) && \
               $(LN_S) wbemcli$(EXEEXT) wbemein$(EXEEXT) && \
               $(LN_S) wbemcli$(EXEEXT) wbemec$(EXEEXT) && \
               $(LN_S) wbemcli$(EXEEXT) wbemei$(EXEEXT) && \
               $(LN_S) wbemcli$(EXEEXT) wbemgc$(EXEEXT) && \
               $(LN_S) wbemcli$(EXEEXT) wbemmi$(EXEEXT) && \
               $(LN_S) wbemcli$(EXEEXT) wbemdc$(EXEEXT) && \
               $(LN_S) wbemcli$(EXEEXT) wbemgcd$(EXEEXT) && \
               $(LN_S) wbemcli$(EXEEXT) wbemain$(EXEEXT) && \
               $(LN_S) wbemcli$(EXEEXT) wbemai$(EXEEXT) && \
               $(LN_S) wbemcli$(EXEEXT) wbemrin$(EXEEXT) && \
               $(LN_S) wbemcli$(EXEEXT) wbemri$(EXEEXT) && \
               $(LN_S) wbemcli$(EXEEXT) wbemgp$(EXEEXT) && \
               $(LN_S) wbemcli$(EXEEXT) wbemsp$(EXEEXT) && \
               $(LN_S) wbemcli$(EXEEXT) wbemcm$(EXEEXT) && \
               $(LN_S) wbemcli$(EXEEXT) wbemcmx$(EXEEXT) 
uninstall-hook:
	cd $(DESTDIR)$(bindir) && \
	       $(RM) wbemgi$(EXEEXT) && \
               $(RM) wbemci$(EXEEXT) && \
               $(RM) wbemdi$(EXEEXT) && \
               $(RM) wbemecn$(EXEEXT) && \
               $(RM) wbemein$(EXEEXT) && \
               $(RM) wbemec$(EXEEXT) && \
               $(RM) wbemei$(EXEEXT) && \
               $(RM) wbemgc$(EXEEXT) && \
               $(RM) wbemmi$(EXEEXT) && \
               $(RM) wbemdc$(EXEEXT) && \
               $(RM) wbemgcd$(EXEEXT) && \
               $(RM) wbemain$(EXEEXT) && \
               $(RM) wbemai$(EXEEXT) && \
               $(RM) wbemrin$(EXEEXT) && \
               $(RM) wbemri$(EXEEXT) && \
               $(RM) wbemgp$(EXEEXT) && \
               $(RM) wbemsp$(EXEEXT) && \
               $(RM) wbemcm$(EXEEXT) && \
               $(RM) wbemcmx$(EXEEXT) 

dist-hook:
	$(RM) -r `find $(distdir) -name CVS`

postinstall:

runtest:
