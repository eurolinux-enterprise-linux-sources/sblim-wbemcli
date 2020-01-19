#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_INIT(SBLIM WBEMCLI, 1.6.2, sblim-devel@lists.sourceforge.net,sblim-wbemcli)
AC_CONFIG_SRCDIR([CimXml.cpp])
AC_CANONICAL_HOST

AM_INIT_AUTOMAKE
AM_CONFIG_HEADER([config.h])

AC_ARG_ENABLE(pythoncli,
	[AC_HELP_STRING([--enable-pythoncli],
		[enable Python CLI - doesn't work currently])],
		[wbemcli_enable_swig=yes],[wbemcli_enable_swig=no])
AC_ARG_VAR([CACERT],[the name of the certificate file (truststore) to use. Default PREFIX/share/sblim-wbemcli/cacert.pem])

if test x$CACERT == x; then
   CACERT=$datadir/$PACKAGE/cacert.pem
fi
AC_SUBST(CACERT)

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_LN_S

if test "$wbemcli_enable_swig" = "yes"; then
  AC_CHECK_PROG(wbemcli_swig,swig,yes,no)
  AC_CHECK_PROG(wbemcli_python,python,yes,no)
fi

# Checks for libraries.
wbemcli_http=

case $host_os 
# check for /opt/gnome
  in linux*) AC_CHECK_FILE(/opt/gnome,[wbemcli_optgnome=yes],
		[wbemcli_optgnome=no]);;
esac

if test "$wbemcli_optgnome" = "yes"; then
   wbemcli_saveLDFLAGS=$LDFLAGS
   wbemcli_saveCPPFLAGS=$CPPFLAGS
   GNOME_LDFLAGS="-L /opt/gnome/lib"
   GNOME_CPPFLAGS="-I /opt/gnome/include"
   LDFLAGS="$GNOME_LDFLAGS $LDFLAGS"
   CPPFLAGS="$GNOME_CPPFLAGS $CPPFLAGS"
fi

if test "$wbemcli_http" = ""; then
    # Check for libcurl
    HAVE_CURL_HEADER=""
    HAVE_CURL_LIB=""
    AC_CHECK_HEADER(curl/curl.h, [HAVE_CURL_HEADER=yes])
    AC_CHECK_LIB(curl,curl_easy_init,[HAVE_CURL_LIB=yes])
    if test -n "$HAVE_CURL_HEADER" &&
       test -n "$HAVE_CURL_LIB"; then
        wbemcli_http=curl
    fi
fi

if test "$wbemcli_http" = ""; then
    AC_CHECK_LIB(ghttp,main,[wbemcli_http=ghttp],)
    if test "$wbemcli_optgnome" = "yes"; then
        LDFLAGS=$wbemcli_saveLDFLAGS
        CPPFLAGS=$wbemcli_saveCPPFLAGS
    fi      
fi

if test "$wbemcli_http" = ""; then
    #check for libwww
    AC_CHECK_LIB(www,main,[wbemcli_http=www])
    AC_CHECK_PROG(wbemcli_libwww_config,libwww-config,yes,no)
    if test "$wbemcli_http" = "www" && 
        test "$wbemcli_libwww_config" = "yes"; then   
        # keep info about LIBS and INCLUDES
        WWW_LIBS=`libwww-config --libs`
        WWW_CPPFLAGS=`libwww-config --cflags`
    fi
fi

case $wbemcli_http in
curl)
    LIBS="-lcurl $LIBS"
    CIMHTTP='CimCurl.$(OBJEXT)'
    AC_DEFINE(HAVE_LIBCURL, 1, "Have libcurl support")
    AC_MSG_NOTICE([using libcurl as HTTP library])
    ;;
ghttp)
    if test "$wbemcli_optgnome" = "yes"; then
        LDFLAGS="$GNOME_LDFLAGS $LDFLAGS" 
        CPPFLAGS="$GNOME_CPPFLAGS $CPPFLAGS" 
    fi	
    LIBS="-l ghttp $LIBS"
    CIMHTTP='Cimghttp.$(OBJEXT)'
    AC_DEFINE(HAVE_LIBGHTTP,1,"Have libghttp support.")
    AC_MSG_NOTICE([using libghttp as HTTP library])       
    ;;
www)
    CPPFLAGS="$WWW_CPPFLAGS $CPPFLAGS"
    LIBS="$WWW_LIBS $LIBS"
    CIMHTTP='Cimwww.$(OBJEXT)'
    AC_DEFINE(HAVE_LIBWWW,1,"Have libwww support.")
    AC_MSG_NOTICE([using libwww as HTTP library])
    ;;
*)
    CIMHTTP=
    AC_MSG_ERROR([no usable HTTP library found])
    ;;
esac

AC_SUBST(CIMHTTP)

# Checks for header files.
AC_FUNC_ALLOCA
AC_HEADER_STDC
AC_CHECK_HEADERS([malloc.h stdlib.h string.h])

if test "$wbemcli_enable_swig" = "yes" &&
   "$wbemcli_swig" = "yes" && test "$wbemcli_python" = "yes" ; then
  case $host_os 
# check for Python Include
    in linux*) 
      wbemcli_pythinc=`find /usr/include -name Python.h -printf "%h " | sed 's/ .*$//'`
      wbemcli_saveCPPFLAGS=$CPPFLAGS
      if test -n "$wbemcli_pythinc"; then
        CPPFLAGS="-I $wbemcli_pythinc $CPPFLAGS"
	AC_CHECK_HEADER(Python.h,[wbemcli_swig_ok=yes],[wbemcli_swig_ok=no])
	SWIGINCLUDE="-I $wbemcli_pythinc"
	AC_SUBST(SWIGINCLUDE)
	CPPFLAGS=$wbemcli_saveCPPFLAGS	
      fi
    ;;
  esac
fi
AM_CONDITIONAL(HAVE_SWIG,[test "$wbemcli_swig_ok" = "yes"])


# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_C_INLINE
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_VPRINTF
AC_CHECK_FUNCS([memmove memset strcasecmp strchr strdup strncasecmp strndup strpbrk strstr strtol])

AC_CONFIG_FILES([Makefile sblim-wbemcli.spec man/wbemcli.1.pre])
AC_OUTPUT
