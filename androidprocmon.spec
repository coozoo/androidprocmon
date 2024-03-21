# spectool -g -R androidprocmon.spec
# rpmbuild -ba androidprocmon.spec

%define name androidprocmon
%define reponame androidprocmon
%define version %(echo "$(curl --silent 'https://raw.githubusercontent.com/coozoo/androidprocmon/master/main.cpp'|grep 'QString APP_VERSION'| tr -d ' '|grep -oP '(?<=constQStringAPP_VERSION=").*(?=\";)')")
%define build_timestamp %{lua: print(os.date("%Y%m%d"))}

Summary: Android Process Monitor some  kind of GUI for adb command. That allows monitor process perfomance using dumpsys and top.
Name: %{name}
Version: %{version}
Release: %{build_timestamp}%{?dist}
Source0: https://github.com/coozoo/androidprocmon/archive/master.zip#/%{name}-%{version}.tar.gz


License: MIT
Url: https://github.com/coozoo/androidprocmon

%if 0%{?fedora} || 0%{?rhel_version} || 0%{?centos_version}
BuildRequires: qt5-qtbase-devel >= 5.9
BuildRequires: qt5-linguist >= 5.9
BuildRequires: qt5-qtserialport-devel >= 5.9
BuildRequires: qt5-qtcharts-devel >= 5.9
%endif
%if 0%{?suse_version} || 0%{?sle_version}
Group:          Electronics
BuildRequires:  pkgconfig(Qt5Widgets)
BuildRequires:  libqt5-qtbase-devel
BuildRequires:  libqt5-linguist
BuildRequires:  libqt5-qtserialport-devel
BuildRequires:  libQt5Charts5-devel
BuildRequires:  update-desktop-files
Requires(post): update-desktop-files
Requires(postun): update-desktop-files
%endif
%if 0%{?mageia} || 0%{?mdkversion}
BuildRequires: lib64qt5base5-devel >= 5.9
BuildRequires: lib64qt5help-devel >= 5.9
BuildRequires: lib64qt5serialport-devel >= 5.9
BuildRequires: lib64qt5charts-devel >= 5.9
%endif

# Requires: qt5 >= 5.5



%description

It's a tool that allow monitoring of android application with logging possibility through ADB. 
Actually it's some kind of UI for adb and tools on android device - top and dumpsys.
There is chart representation of live data, possibility to execute command on device and some kind of interface for logcat.
So it's very good when you need to test your application on android device and monitor resources.

%global debug_package %{nil}

%prep
#copr build
#%setup -q -n %{name}-%{version}
#local build
%setup -q -n %{reponame}-master

%build
# don't know maybe it's stupid me but lrelease in qt looks like runs after make file generation as result automatic file list inside qmake doesn't work
# so what I need just run it twice...
%if 0%{?fedora} || 0%{?rhel_version} || 0%{?centos_version}
    qmake-qt5
    make
    qmake-qt5
    make
%endif
%if 0%{?mageia} || 0%{?suse_version} || 0%{?sle_version} || 0%{?mdkversion}
    %qmake5
    %make_build
    %qmake5
    %make_build
%endif

%install
%if 0%{?fedora} || 0%{?rhel_version} || 0%{?centos_version}
    make INSTALL_ROOT=%{buildroot} -j$(nproc) install
%endif
%if 0%{?mageia} || 0%{?suse_version} || 0%{?sle_version} || 0%{?mdkversion}
    %qmake5_install
    %suse_update_desktop_file -G "Android Process Monitor" -r androidprocmon Development
%endif

%post
%if 0%{?suse_version} ||  0%{?sle_version}
    %desktop_database_post
%endif

%postun
%if 0%{?suse_version} || 0%{?sle_version}
    %desktop_database_postun
%endif

%files
%if 0%{?fedora} || 0%{?rhel_version} || 0%{?centos_version} || 0%{?mageia} || 0%{?mdkversion}
    %{_bindir}/*
    %{_datadir}/*
%endif
%if 0%{?suse_version} || 0%{?sle_version}
    %license LICENSE
    %doc README.md
    %{_bindir}/*
    %{_datadir}/*
    %{_datadir}/applications/androidprocmon.desktop
%endif

%changelog
