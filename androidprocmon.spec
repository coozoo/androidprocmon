# spectool -g -R androidprocmon.spec
# rpmbuild -ba androidprocmon.spec

%define name androidprocmon
%define reponame androidprocmon
%define version %(echo "$(curl --silent 'https://raw.githubusercontent.com/coozoo/androidprocmon/master/main.cpp'|grep -oP '(?<=const QString APP_VERSION = \").*(?=\";)')")
%define build_timestamp %{lua: print(os.date("%Y%m%d"))}

Summary: Android Process Monitor some  kind of GUI for adb command. That allows monitor process perfomance using dumpsys and top.
Name: %{name}
Version: %{version}
Release: %{build_timestamp}%{?dist}
Source0: https://github.com/coozoo/androidprocmon/archive/master.zip#/%{name}-%{version}.tar.gz


License: MIT

BuildRequires: qt5-qtbase-devel >= 5.9
BuildRequires: qt5-linguist >= 5.9

# Requires: qt5 >= 5.5

Url: https://github.com/coozoo/androidprocmon

%description

It's a tool that allow monitoring of android application with logging possibility through ADB. 
Actually it's some kind of UI for adb and tools on android device - top and dumpsys.
There is chart representation of live data, possibility to execute command on device and some kind of interface for logcat.
So it's very good when you need to test your application on android device and monitor resources.

%global debug_package %{nil}

%prep
%setup -q -n %{name}-%{version}

%build
# don't know maybe it's stupid me but lrelease in qt looks like runs after make file generation as result automatic file list inside qmake doesn't work
# so what I need just run it twice...
qmake-qt5
make
qmake-qt5
make

%install
make INSTALL_ROOT=%{buildroot} -j$(nproc) install

%post

%postun

%files
%{_bindir}/*
%{_datadir}/*

%changelog
