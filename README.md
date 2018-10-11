# EditorConfig Plugin for Geany

This is an [EditorConfig][] plugin for [Geany][].

## Installation

### Install From source

1.  Install the [EditorConfig C Core][]. If you are using Debian/Ubuntu, you can
    install it by running the following command:

        apt-get install libeditorconfig-dev

    You also need to install geany and GTK development files (GTK2 or GTK3 depending on your system). This could be
    done by executing the following command if you are using Debian/Ubuntu:

        apt-get install libgtk2.0-dev libgtk-3-dev intltool geany

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
