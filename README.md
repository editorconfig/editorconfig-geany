# EditorConfig Plugin for Geany

This is an [EditorConfig][] plugin for [Geany][].

## Installation

1.  Download the [EditorConfig core][] and follow the instructions in the
    README and INSTALL files to install it. You also need to intall geany and
    GTK 2.0 development files. This could be done by executing the following
    command if you are using Debian/Ubuntu:

        apt-get install libgtk2.0-dev intltool geany

2.  Download EditorConfig plugin for Geany source tree, and you will find a
    `Makefile` in the root. Execute `make` to compile this plugin.

3.  Once the compilation succeeds, there will be an `editorconfig-geany.so` in
    the root of the source tree. Copy this file to `${geany_prefix}/lib/geany`.

4.  Run Geany, click on the `Tools` menu, and choose the `Plugin Manager` menu
    item. In the Plugin Manager Dialog, check the plugin named `EditorConfig`.
    Click on the `OK` button. Now you are ready to use this plugin.

## Supported properties

The EditorConfig Geany plugin supports the following EditorConfig
[properties][]:

* indent_style
* indent_size
* tab_width
* end_of_line
* root (only used by EditorConfig core)

[EditorConfig]: http://editorconfig.org
[EditorConfig core]: https://github.com/editorconfig/editorconfig
[properties]: http://editorconfig.org/#supported-properties
[Geany]: http://www.geany.org
