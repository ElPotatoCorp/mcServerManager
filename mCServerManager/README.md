# MCSM - Minecraft Server Manager

A GTK4 application for managing Minecraft servers.

## Building with Meson

### Prerequisites

Make sure you have the following dependencies installed:

**Ubuntu/Debian:**
```bash
sudo apt update
sudo apt install meson ninja-build pkg-config libgtk-4-dev libcurl4-openssl-dev
```

**Fedora:**
```bash
sudo dnf install meson ninja-build pkg-config gtk4-devel libcurl-devel
```

**Arch Linux:**
```bash
sudo pacman -S meson ninja pkgconf gtk4 curl
```

### Building

#### Option 1: Using the build script (recommended)
```bash
# Build the project
./build.sh build

# Run the application
./build.sh run

# Clean build files
./build.sh clean

# Rebuild from scratch
./build.sh rebuild
```

#### Option 2: Using meson directly
```bash
# Setup build directory
meson setup build

# Build the project
meson compile -C build

# Run the application
./build/src/mcsm
```

### Installation

To install the application system-wide:
```bash
# Using build script
./build.sh install

# Or using meson directly
meson install -C build
```

### Development

For development builds with debug symbols:
```bash
meson setup build --buildtype=debug
meson compile -C build
```

## Project Structure

```
mcsm/
├── meson.build          # Main build configuration
├── meson_options.txt    # Build options
├── build.sh            # Convenience build script
├── src/                # Source code
│   ├── meson.build     # Source build configuration
│   ├── main.c          # Main application entry point
│   ├── MCSMApp/        # Application classes
│   └── Utils/          # Utility functions
└── data/               # Resources and data files
    ├── meson.build     # Data build configuration
    ├── mcsm.gresource.xml  # Resource bundle definition
    ├── mcsm.gschema.xml    # GSettings schema
    └── *.xml           # UI definition files
```

## Features

- GTK4 modern UI
- GSettings for configuration management
- Resource bundling for easy distribution
- Proper installation support
- Debug and release builds