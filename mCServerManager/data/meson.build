# GSettings Schema
gnome = import('gnome')

# Install GSettings schema
install_data(
  'mcsm.gschema.xml',
  install_dir: join_paths(get_option('datadir'), 'glib-2.0', 'schemas')
)

# Compile GSettings schema for development
compiled_schemas = gnome.compile_schemas(
  build_by_default: true,
  depend_files: 'mcsm.gschema.xml'
)

# GResource bundle
mcsm_resources = gnome.compile_resources(
  'mcsm-resources',
  'mcsm.gresource.xml',
  source_dir: '.',
  c_name: 'mcsm',
  export: true,
)