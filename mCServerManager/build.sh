#!/bin/bash

# MCSM Build Script

set -e

PROJECT_DIR="$(dirname "$0")"
BUILD_DIR="$PROJECT_DIR/build"

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

print_status() {
    echo -e "${GREEN}[INFO]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARN]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Check if meson is available
if ! command -v meson &> /dev/null; then
    print_error "Meson build system not found. Please install meson:"
    echo "  Ubuntu/Debian: sudo apt install meson"
    echo "  Fedora:       sudo dnf install meson"
    echo "  Arch:         sudo pacman -S meson"
    exit 1
fi

# Check if ninja is available
if ! command -v ninja &> /dev/null; then
    print_error "Ninja build tool not found. Please install ninja:"
    echo "  Ubuntu/Debian: sudo apt install ninja-build"
    echo "  Fedora:       sudo dnf install ninja-build"
    echo "  Arch:         sudo pacman -S ninja"
    exit 1
fi

# Function to setup build directory
setup_build() {
    print_status "Setting up build directory..."
    if [ -d "$BUILD_DIR" ]; then
        print_warning "Build directory exists, removing..."
        rm -rf "$BUILD_DIR"
    fi
    
    meson setup "$BUILD_DIR" "$PROJECT_DIR"
    print_status "Build directory setup complete"
}

# Function to build the project
build_project() {
    print_status "Building project..."
    meson compile -C "$BUILD_DIR"
    print_status "Build complete"
}

# Function to install the project
install_project() {
    print_status "Installing project..."
    meson install -C "$BUILD_DIR"
    print_status "Installation complete"
}

# Function to run the project
run_project() {
    print_status "Running MCSM..."
    if [ -f "$BUILD_DIR/src/mcsm" ]; then
        "$BUILD_DIR/src/mcsm"
    else
        print_error "Executable not found. Please build first."
        exit 1
    fi
}

# Main script logic
case "${1:-build}" in
    "setup")
        setup_build
        ;;
    "build")
        if [ ! -d "$BUILD_DIR" ]; then
            setup_build
        fi
        build_project
        ;;
    "clean")
        print_status "Cleaning build directory..."
        rm -rf "$BUILD_DIR"
        print_status "Clean complete"
        ;;
    "install")
        install_project
        ;;
    "run")
        run_project
        ;;
    "rebuild")
        setup_build
        build_project
        ;;
    *)
        echo "Usage: $0 [setup|build|clean|install|run|rebuild]"
        echo ""
        echo "Commands:"
        echo "  setup   - Setup build directory"
        echo "  build   - Build the project (default)"
        echo "  clean   - Remove build directory"
        echo "  install - Install the project"
        echo "  run     - Run the built executable"
        echo "  rebuild - Clean setup and build"
        exit 1
        ;;
esac