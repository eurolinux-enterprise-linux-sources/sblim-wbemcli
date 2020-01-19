#
# $Id: sblim-wbemcli.spec.in,v 1.7 2008/08/07 18:25:41 tyreld Exp $
#
# Package spec for wbemcli
#

BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)

Summary: SBLIM WBEM Command Line Interface
Name: sblim-wbemcli
Version: 1.6.2
Release: 1
Group: Systems Management/Base
URL: http://www.sblim.org
License: EPL

Source0: http://prdownloads.sourceforge.net/sblim/%{name}-%{version}.tar.bz2

BuildRequires: curl-devel >= 7.9.3
Requires: curl >= 7.9.3

%Description
WBEM Command Line Interface is a standalone, command line WBEM client. It is
specially suited for basic systems management tasks as it can be used in
scripts.

%prep
%setup

%build
%configure
make %{?smp_mflags}

%install
[ "$RPM_BUILD_ROOT" != "/" ] && rm -rf $RPM_BUILD_ROOT
%makeinstall
mkdir -p $RPM_BUILD_ROOT/%{_datadir}/%{name}

%clean
[ "$RPM_BUILD_ROOT" != "/" ] && rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root)
%{_bindir}/wbem*
%{_mandir}/man1/*
%{_datadir}/%{name}

%changelog
* Thu Aug 07 2008 Tyrel Datwyler <tyreld@us.ibm.com> 1.6.0-1
  - Re-licensed under the EPL
  - Several bug fixes

* Mon Nov 21 2005 Viktor Mihajlovski <mihajlov@de.ibm.com> 1.5.1-1
  - Upgrade to version 1.5.1 (SSL V3 enforced, default CACERT).

* Thu Oct 28 2005 Viktor Mihajlovski <mihajlov@de.ibm.com> 1.5.0-1
  - Minor enhancements for Fedora compatibility, still not daring to
    nuke the build root though

* Thu Jul 28 2005 Viktor Mihajlovski <mihajlov@de.ibm.com> 1.5.0-0
  - Updates for rpmlint complaints
