# spectool -g -R androidprocmon.spec
# rpmbuild -ba androidprocmon.spec

%define name androidprocmon
%define reponame androidprocmon
%define version %(echo "$(curl --silent 'https://raw.githubusercontent.com/coozoo/androidprocmon/main/main.cpp'|grep 'QString APP_VERSION'| tr -d ' '|grep -oP '(?<=constQStringAPP_VERSION=").*(?=\";)')")
%define build_timestamp %{lua: print(os.date("%Y%m%d"))}

Summary: Android Process Monitor some  kind of GUI for adb command. That allows monitor process perfomance using dumpsys and top.
Name: %{name}
Version: %{version}
Release: %{build_timestamp}%{?dist}
Source0: https://github.com/coozoo/androidprocmon/archive/main.zip#/%{name}-%{version}.tar.gz


License: GPL-3.0-or-later AND MIT
Url: https://github.com/coozoo/androidprocmon

%if 0%{?fedora} || 0%{?rhel_version} || 0%{?centos_version}
BuildRequires: qt6-qtbase-devel >= 6.2
BuildRequires: qt6-linguist >= 6.2
BuildRequires: qt6-qtserialport-devel >= 6.2
BuildRequires: qt6-qtcharts-devel >= 6.2
BuildRequires: desktop-file-utils
Requires:      desktop-file-utils
%endif
%if 0%{?suse_version} || 0%{?sle_version}
Group:          Electronics
BuildRequires:  pkgconfig(Qt6Widgets)
BuildRequires:  qt6-tools-linguist
BuildRequires:  qt6-base-devel
BuildRequires:  qt6-tools-devel
BuildRequires:  qt6-serialport-devel
BuildRequires:  qt6-charts-devel
BuildRequires:  update-desktop-files
Requires(post): update-desktop-files
Requires(postun): update-desktop-files
%endif
%if 0%{?mageia} || 0%{?mdkversion}
BuildRequires: qtbase6-common-devel >= 6.2
BuildRequires: lib64qt6base6-devel >= 6.2
BuildRequires: lib64qt6help-devel >= 6.2
BuildRequires: lib64qt6serialport-devel >= 6.2
BuildRequires: lib64qt6charts-devel >= 6.2
%endif

# Requires: qt6 >= 6.2

%description

It's a tool that allow monitoring of android application with logging possibility through ADB. 
Actually it's some kind of UI for adb and tools on android device - top and dumpsys.
There is chart representation of live data, possibility to execute command on device and some kind of interface for logcat.
So it's very good when you need to test your application on android device and monitor resources.

%global debug_package %{nil}

%prep
%setup -q -n %{reponame}-main

%build
# don't know maybe it's stupid me but lrelease in qt looks like runs after make file generation as result automatic file list inside qmake doesn't work
# so what I need just run it twice...
%if 0%{?fedora} || 0%{?rhel_version} || 0%{?centos_version}
    qmake6
    make
    qmake6
    make
%endif
%if 0%{?suse_version} || 0%{?sle_version} || 0%{?mdkversion}
    %qmake6
    %make_build
    %qmake6
    %make_build
%endif
%if 0%{?mageia}
    qmake6
    %make_build
    qmake6
    %make_build
%endif

%install
%if 0%{?fedora} || 0%{?rhel_version} || 0%{?centos_version} || 0%{?mageia} || 0%{?mdkversion}
    make INSTALL_ROOT=%{buildroot} -j$(nproc) install
    desktop-file-install \
        --dir=%{buildroot}%{_datadir}/applications \
        %{buildroot}%{_datadir}/applications/androidprocmon.desktop
%endif
%if 0%{?suse_version} || 0%{?sle_version}
    %qmake6_install
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
