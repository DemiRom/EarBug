# Required Libraries 
- Qt6 Development Libraries
- LibPulse dev
- X11
- GNU Compiler 12+ Toolchain and development libraries
- CMake

# How to Compile
1. Run cmake to generate build files no special flags necessary
2. Run make to build

---

# Installing Required Development Libraries

## OpenSUSE
1. Install base development tools.

    `sudo zypper install -t pattern devel_C_C++`

2. Install cmake

    `sudo zypper install cmake`

3. Install pulse audio development libraries

    `sudo zypper install libpulse-devel`

4. Install Qt6 libraries and tools

    `sudo zypper install --recommends -t pattern devel_qt6`

5. Install xcb keysyms

   [Download](https://software.opensuse.org//download.html?project=home%3Agoupilmtos%3AX11%3AXorg&package=xcb-util-keysyms) this package to install the keysyms library 

## Debian/Ubuntu

1. Install base development tools.

    `sudo apt install build-essential`

2. Install cmake

    `sudo apt install cmake`

3. Install pulse audio development libraries

    `sudo apt install libpulse-devel`

4. Install Qt6 libraries and tools

    `sudo apt install qt6-dev`

    `sudo apt install qt6-tools`

--- 

# Packaging
This project is designed to be packaged into an app image for distribution. 

## Additional Requirements
In addition to the basic development libraries and tools some additional tools are required to package EarBug
- ### linuxdeployqt
    This project uses the [linuxdeployqt](https://github.com/probonopd/linuxdeployqt) Project to package the application into
    an app image. Simply download their app image and then create a link in `/usr/bin` using `ln -s /path/to/linuxdeployqt.appimage /usr/bin/linuxdeployqt` 

- ### qmake
    Linuxdeployqt requires qmake to be installed on your system and in the path.

## qmake on Open Suse with Qt6
On OpenSUSE qt6 installs binary named `qmake6` to the `/usr/bin` directory. In order for `linuxdeployqt` to work correctly create a symlink to `qmake6` by running `ln -s /usr/bin/qmake6 /usr/bin/qmake`

## Running linuxdeployqt
Finally, to package the application after building copy the built application binary into the `deploy/usr/bin` directory
then move back to the `deploy` directory and run `linuxdeployqt` command, that will build a working app image that can be distributed.
