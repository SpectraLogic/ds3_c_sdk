# http://stackoverflow.com/questions/880227/what-is-the-minimum-i-have-to-do-to-create-an-rpm-file
# Don't try fancy stuff like debuginfo, which is useless on binary-only
# packages. Don't strip binary too
# Be sure buildpolicy set to do nothing
%define        __spec_install_post %{nil}
%define          debug_package %{nil}
%define        __os_install_post %{_dbpath}/brp-compress
Name:		ds3_c_sdk
#Version:	3.2.3
Version:	%{sdk_version}
Release:	1%{?dist}
Summary:	Spectra DS3 C SDK

Group:		Utilities/System
License:	ASL 2.0
URL:		https://github.com/SpectraLogic/ds3_c_sdk
Source0:	%{name}-%{version}.tar.gz
BuildRoot:	%{_tmppath}/%{name}-%{version}-%{release}-root

BuildRequires:	libcurl-devel
BuildRequires:	libxml2-devel
BuildRequires:	glib2-devel
BuildRequires:	openssl-devel
BuildRequires:	cmake
Requires:	curl openssl

%description
This project contains a C library for using the Spectra S3 Deep Storage REST interface.

%prep
%setup -q


%build

cmake -DCMAKE_INSTALL_PREFIX=/usr  .
make

%install
#rm -rf %{buildroot}
#mkdir -p  %{buildroot}
#
## in builddir
#cp -a * %{buildroot}
#find %buildroot -type f \( -name '*.so' -o -name '*.so.*' \) -exec chmod 755 {} +
make DESTDIR=%{buildroot}/usr install 


%files
%defattr(-,root,root,-)
%{_bindir}/*
%{_libdir}/libds3.so*
%doc /usr/share/doc/%{name}-%{version}/*




%changelog
* Wed Feb 15 2017  Yuriy Shestakov <yuriy.shestakov@archive-engines.com> 3.4.1
- Assemble binary RPM 
