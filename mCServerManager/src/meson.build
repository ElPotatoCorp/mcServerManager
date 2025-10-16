# Source files
mcsm_sources = [
  'main.c',
  'MCSMApp/MCSMApp.c',
  'MCSMApp/MainWindow/MCSMAppWindow.c',
  'MCSMApp/MCSMAppPref/MCSMAppPref.c',
  'Utils/Utils.c',
]

# Headers for IDE support
mcsm_headers = [
  'MCSMApp/MCSMApp.h',
  'MCSMApp/MainWindow/MCSMAppWindow.h',
  'MCSMApp/MCSMAppPref/MCSMAppPref.h',
  'Utils/Utils.h',
  'Utils/Constants.h',
]

# Include directories
mcsm_inc = include_directories(
  '.',
  'MCSMApp',
  'MCSMApp/MainWindow',
  'MCSMApp/MCSMAppPref',
  'Utils',
)

# Main executable
mcsm_exe = executable(
  'mcsm',
  [mcsm_sources, mcsm_resources],
  dependencies: common_deps,
  include_directories: mcsm_inc,
  install: true,
  install_dir: get_option('bindir')
)