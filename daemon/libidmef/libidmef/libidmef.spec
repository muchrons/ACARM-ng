# $Id: libidmef.spec,v 1.4 2005/07/10 09:38:51 alcapone Exp $

%define	version_number	1.1.0
%define	version_tag	beta
%define release		1.sp

Summary: Intrusion Detection Message Exchange Format (IDMEF) Library
Name: libidmef
Version: %{version_number}_%{version_tag}
Release: %{release}
Group: Development/Libraries
URL: http://www.migus.org/software/libidmef/
Source0: http://www.migus.org/software/download/%{name}-%{version_number}-%{version_tag}.tar.gz
License: BSD/GPL
BuildRoot: %{_tmppath}/%{name}-%{version_number}-%{version_tag}-buildroot

Prereq:		libxml2 >= 2.5.10

Provides:	libidmef

BuildRequires:	libxml2-devel >= 2.5.10

%{!?IDMEF_DEBUG: 	%define 	IDMEF_DEBUG 1}

%description
LibIDMEF is an implementation of the Internet Engineering Task Force (IETF),
Intrusion Detection Exchange Format Charter Working Group (IDWG), draft
standard Intrusion Detection Message Exchange Format (IDMEF) protocol.

%package devel
Group: Development/Libraries
Summary: LibIDMEF development files.
Prereq: pkgconfig

%description devel
Package containing header files, libraries and config script used for
developing applications which use libidmef.

%define _dtd_dir	%{_datadir}/%{name}
%define _examples_dir	%{_datadir}/%{name}/idmef-messages
%define _dtd		idmef-message.dtd

%prep
%setup -q -n %{name}-%{version_number}-%{version_tag}

%build 
%configure \
%if %{IDMEF_DEBUG}
	--enable-debug \
%endif
	--with-dtd-dir=%{_dtd_dir} \
	--with-examples-dir=%{_examples_dir} 

%install
%makeinstall
[ ! -d %{buildroot}%{_dtd_dir} ] && %{__mkdir} -p %{buildroot}%{_dtd_dir}
%{__mv} -f %{buildroot}%{_datadir}/%{_dtd} %{buildroot}%{_dtd_dir}/%{_dtd}
[ ! -d %{buildroot}%{_examples_dir} ] && \
	%{__mkdir} -p %{buildroot}%{_examples_dir}
%{__mv} -f %{buildroot}%{_datadir}/example*.xml %{buildroot}%{_examples_dir}/
%{__rm} -f %{buildroot}%{_libdir}/%{name}.la

%clean
%{__rm} -rf %{buildroot}

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%files 
%defattr(-,root,root) 
%doc AUTHORS README FAQ
%{_libdir}/%{name}.so.%{version_number}
%{_dtd_dir}/%{_dtd}

%files devel
%defattr(-,root,root) 
%doc ChangeLog COPYING COPYING.BSD INSTALL NEWS TODO libidmef-functions.map
%{_bindir}/%{name}-config
%{_libdir}/%{name}.a
%{_libdir}/%{name}.so
%{_libdir}/%{name}.so.1
%{_libdir}/pkgconfig/%{name}.pc
%{_includedir}/%{name}/idmef_create.h
%{_includedir}/%{name}/idmef_global.h
%{_includedir}/%{name}/idmef_strings.h
%{_includedir}/%{name}/idmef_types.h
%{_includedir}/%{name}/idmef_helpers.h
%{_includedir}/%{name}/idmef_sax_parser.h
%{_examples_dir}/*

%changelog
* Mon Dec 27 2004 Sandro Poppi <spoppi@gmx.de> 1.0.1_alpha-1
  - added Changelog to devel docs
  - rebuilt to reflect draft 12

* Fri Apr 23 2004 Adam C. Migus <adam@migus.org> 1.0.0_alpha-4
  - Add %clean section to remove buildroot.
  - Add %post and %postun sections to run /sbin/ldconfig
  - Require pkgconfig for -devel package.
  - Do not install .la file.

* Sat Apr 17 2004 Adam C. Migus <adam@migus.org> 1.0.0_alpha-3
  - Changes to reflect new support for pkgconfig and installation of example
    IDMEF message files.

* Fri Apr 16 2004 Adam C. Migus <adam@migus.org> 1.0.0_beta-2
  - Changes to reflect new version of libidmef.
  - Change IDMEF_DEBUG build-time argument.
  - Fix dependency version number problem for libxml2.

* Mon Mar 15 2004 Adam C. Migus <adam@migus.org> 1.0.0_alpha-1
  - initial build.
