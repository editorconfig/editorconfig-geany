# EditorConfig Plugin for Geany

This is an [EditorConfig][] plugin for [Geany][].

## Installation

### Install From source

1.  Download the [EditorConfig core][] and follow the instructions in the
    README and INSTALL files to install it. You also need to intall geany and
    GTK 2.0 development files. This could be done by executing the following
    command if you are using Debian/Ubuntu:

        apt-get install libgtk2.0-dev intltool geany

2.  Download EditorConfig plugin for Geany source tree, and you will find a
    `Makefile` in the root. Execute `make` to compile this plugin. Note that if
    the prefix of EditorConfig on your system is not `/usr/local`, you need to
    specify the `EDITORCONFIG_PREFIX` when executing make. For example, your
    EditorConfig prefix is `/usr`, then you should execute:

        make EDITORCONFIG_PREFIX=/usr

3.  Once the compilation succeeds, there will be an `editorconfig-geany.so` in
    the root of the source tree. Copy this file to `${geany_prefix}/lib/geany`.

4.  Run Geany, click on the `Tools` menu, and choose the `Plugin Manager` menu
    item. In the Plugin Manager Dialog, check the plugin named `EditorConfig`.
    Click on the `OK` button. Now you are ready to use this plugin.


### Install From Binary

1.  Install [EditorConfig core][] library. This can be either done by building
    it from source or install the binary directly.

2.  [Download the Geany plugin binary][Geany Plugin Binary Download] file and
    copy this file to `${geany_prefix}/lib/geany`.

3.  Run Geany, click on the `Tools` menu, and choose the `Plugin Manager` menu
    item. In the Plugin Manager Dialog, check the plugin named `EditorConfig`.
    Click on the `OK` button. Now you are ready to use this plugin.

## Supported Properties

The EditorConfig Geany plugin supports the following EditorConfig
[properties][]:

* indent_style
* indent_size
* tab_width
* end_of_line
* root (only used by EditorConfig core)

[EditorConfig]: http://editorconfig.org
[EditorConfig core]: https://github.com/editorconfig/editorconfig-core
[properties]: http://editorconfig.org/#supported-properties
[Geany]: http://www.geany.org
[Geany Plugin Binary Download]: https://github.com/editorconfig/editorconfig-geany/downloads
