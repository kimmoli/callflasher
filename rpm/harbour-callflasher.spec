#
# harbour-callfalsher spec
# (C) kimmoli 2014
#

Name:       harbour-callflasher

%{!?qtc_qmake:%define qtc_qmake %qmake}
%{!?qtc_qmake5:%define qtc_qmake5 %qmake5}
%{!?qtc_make:%define qtc_make make}
%{?qtc_builddir:%define _builddir %qtc_builddir}

Summary:    Flashes flash-led during incoming phonecall
Version:    0.0.devel
Release:    1
Group:      Qt/Qt
License:    LICENSE
URL:        https://github.com/kimmoli/callflasher
Source0:    %{name}-%{version}.tar.bz2

BuildRequires:  pkgconfig(Qt5Core)
BuildRequires:  pkgconfig(Qt5DBus)
BuildRequires:  pkgconfig(mlite5)

%description
%{summary}

%prep
%setup -q -n %{name}-%{version}

%build

%qtc_qmake5 SPECVERSION=%{version}

%qtc_make %{?_smp_mflags}

%install
rm -rf %{buildroot}

%qmake5_install

%preun
systemctl-user stop harbour-callflasher
systemctl-user disable harbour-callflasher

%post
systemctl-user start harbour-callflasher
systemctl-user enable harbour-callflasher

%pre
# In case of update, stop first
if [ "$1" = "2" ]; then
  systemctl-user stop harbour-callflasher
fi

%files
%defattr(644,root,root,755)
%attr(755,root,root) %{_bindir}/%{name}
%{_datadir}/dbus-1/
/etc/systemd/user/

