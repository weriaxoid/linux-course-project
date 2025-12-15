Name: kp-backup
Version:        1.0
Release:        7%{?dist}
Summary:        Student Course Project Backup System

License:        GPL
Source0:        %{name}-%{version}.tar.gz

Requires:       tar
BuildRequires:  cmake, gcc-c++

%description
Automated backup system written in C++ for Linux Administration Course Project.
Runs as a specific user but performs privileged read operations.

%prep
%setup -q

%build
cmake . -DCMAKE_INSTALL_PREFIX=/usr
make %{?_smp_mflags}

%install
make install DESTDIR=%{buildroot}
mkdir -p %{buildroot}/%{_unitdir}
install -m 644 systemd/%{name}.service %{buildroot}/%{_unitdir}/%{name}.service

%pre
getent group user-11-01 >/dev/null || groupadd -r user-11-01
getent passwd user-11-01 >/dev/null || \
    useradd -r -g user-11-01 -d /var/tmp/backups -s /sbin/nologin \
    -c "Student Backup User" user-11-01
exit 0

%post
%systemd_post %{name}.service
setcap cap_dac_read_search+ep /usr/bin/%{name}

%preun
%systemd_preun %{name}.service

%postun
%systemd_postun_with_restart %{name}.service

%files
/usr/bin/%{name}
/etc/%{name}.conf
%{_unitdir}/%{name}.service

%changelog
* Mon Dec 15 2025 Student Name <email@example.com> - 1.0-1
- Initial release
