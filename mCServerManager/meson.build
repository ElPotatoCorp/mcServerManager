project('mcsm', 'c',
  version: '1.0.0',
  license: 'GPL-3.0-or-later',
  default_options: [
    'warning_level=2',
    'c_std=c11',
  ],
  meson_version: '>= 0.56.0'
)

# Dependencies
gtk_dep = dependency('gtk4', version: '>= 4.0')
glib_dep = dependency('glib-2.0', version: '>= 2.66')
gio_dep = dependency('gio-2.0', version: '>= 2.66')
curl_dep = dependency('libcurl')

common_deps = [
  gtk_dep,
  glib_dep,
  gio_dep,
  curl_dep,
]

# Configuration
conf = configuration_data()
conf.set_quoted('VERSION', meson.project_version())
conf.set_quoted('PACKAGE_NAME', meson.project_name())
conf.set_quoted('GETTEXT_PACKAGE', meson.project_name())

# Build subdirectories
subdir('data')
subdir('src')