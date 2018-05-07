# EditorConfig Plugin for Geany

This is an [EditorConfig][] plugin for [Geany][].

## Installation

### Install From source

1.  Install the [EditorConfig C Core][]. If you are using Debian/Ubuntu, you can
    install it by running the following command:

        apt-get install libeditorconfig-dev

    You also need to install geany and GTK 2.0 development files. This could be
    done by executing the following command if you are using Debian/Ubuntu:

        apt-get install libgtk2.0-dev intltool geany

2.  Download EditorConfig plugin for Geany source tree, and you will find a
    `Makefile` in the root. Execute `make` to compile this plugin. Note that if
    the prefix of EditorConfig on your system is not `/usr/local`, you need to
    specify the `EDITORCONFIG_PREFIX` when executing make. For example, your
    EditorConfig prefix is `/usr`, then you should execute:

        make EDITORCONFIG_PREFIX=/usr

3.  Once the compilation succeeds, there will be an `editorconfig-geany.so` in
    the root of the source tree. Copy this file to `${geany_prefix}/lib/geany`.
    But sometimes the path may be a little bit tricky on some systems. For
    these cases, the path can usually be obtained by
    `echo $(pkg-config --variable=libdir geany)/geany` if `pkg-config` is
    present in your system.

4.  Run Geany, click on the `Tools` menu, and choose the `Plugin Manager` menu
    item. In the Plugin Manager Dialog, check the plugin named `EditorConfig`.
    Click on the `OK` button. Now you are ready to use this plugin.


### Build Debian/Ubuntu/etc. package from source

1.  Requirements for building the package are a GTK3 Version of Geany >= 1.32.
    This should work on all Debian like operating systems that meet the
    requirements in the control file. Further you require `build-essential`
    and `fakeroot` or `pbuilder` depending on how to build the package.

2.  Build with dpkg-buildpackage:
    Run `dpkg-buildpackage -rfakeroot -uc -us` to build inside sources.
    Alternatively build with pbuilder:
    Run `dpkg-source -b DIRECTORYNAME` on the directory where the source tree
    root resides. This builds the source package, especially the dsc file.
    Run `sudo pbuilder --build editorconfig-geany_1.dsc` to build. The
    resulting package lies in /var/cache/pbuilder/result .

3.  Install resulting debian package(s) with `dpkg` or `gdebi`.

### Install From Binary

There is no prebuilt binary package currently.

## Supported Properties

The EditorConfig Geany plugin supports the following EditorConfig
[properties][]:

* indent_style
* indent_size
* tab_width
* end_of_line
* root (only used by EditorConfig core)

[EditorConfig]: http://editorconfig.org
[EditorConfig C Core]: https://github.com/editorconfig/editorconfig-core
[properties]: http://editorconfig.org/#supported-properties
[Geany]: http://www.geany.org
[Geany Plugin Binary Download]: https://sourceforge.net/projects/editorconfig/files/EditorConfig-Geany-Plugin/
