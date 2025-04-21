/*
  Copyright (c) 2011-2019 EditorConfig Team
  All rights reserved.

  This file is part of EditorConfig-geany.

  EditorConfig-geany is free software: you can redistribute it and/or modify it
  under the terms of the GNU General Public License as published by the Free
  Software Foundation, either version 2 of the License, or (at your option) any
  later version.

  EditorConfig-geany is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
  FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
  details.

  You should have received a copy of the GNU General Public License along with
  EditorConfig-geany. If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdlib.h>
#include <string.h>
#include <geanyplugin.h>
#include <editorconfig/editorconfig.h>

PLUGIN_VERSION_CHECK(211)

PLUGIN_SET_INFO("EditorConfig", "EditorConfig Plugin for Geany",
        "0.2", "https://editorconfig.org");

GeanyPlugin*         geany_plugin;
GeanyData*           geany_data;

/* Reload EditorConfig menu item */
static GtkWidget*  menu_item_reload_editorconfig;

    static void
on_document_open(GObject* obj, GeanyDocument* gd, gpointer user_data);

    static void
on_document_save(GObject* obj, GeanyDocument* gd, gpointer user_data);

    static void
on_geany_startup_complete(GObject* obj, gpointer user_data);

/* plugin signals */
PluginCallback plugin_callbacks[] =
{
    { "document-open", (GCallback)&on_document_open, TRUE, NULL },
    { "document-before-save", (GCallback)&on_document_save, TRUE, NULL },
    { "geany-startup-complete",
        (GCallback)&on_geany_startup_complete, TRUE, NULL },
    { NULL, NULL, FALSE, NULL }
};

struct editorconfig {
    const char*     indent_style;
    #define INDENT_SIZE_TAB (-1000) /* indent_size = -1000 means indent_size = tab*/
    int             indent_size;
    int             tab_width;
    const char*     end_of_line;
    int             insert_final_newline;
}; /* obtained EditorConfig settings will be here */

    static void
parse_editorconfig(editorconfig_handle* eh, struct editorconfig* ec)
{
    int                     i;
    int                     name_value_count;
    memset(ec, 0, sizeof(*ec));

    name_value_count = editorconfig_handle_get_name_value_count(eh);

    /* get settings */
    for (i = 0; i < name_value_count; ++i) {
        const char* name;
        const char* value;

        editorconfig_handle_get_name_value(eh, i, &name, &value);

        if (!strcmp(name, "indent_style"))
            ec->indent_style = value;
        else if (!strcmp(name, "tab_width"))
            ec->tab_width = atoi(value);
        else if (!strcmp(name, "indent_size")) {
            int     value_i = atoi(value);

            if (!strcmp(value, "tab"))
                ec->indent_size = INDENT_SIZE_TAB;
            else if (value_i > 0)
                ec->indent_size = value_i;
        }
        else if (!strcmp(name, "end_of_line"))
            ec->end_of_line = value;
        else if (!strcmp(name, "insert_final_newline") && !strcmp(value, "true"))
            ec->insert_final_newline = 1;
    }
}

    static int
load_editorconfig(const GeanyDocument* gd)
{
    struct editorconfig     ec;
    editorconfig_handle     eh = editorconfig_handle_init();
    int                     err_num;
    ScintillaObject*        sci = gd->editor->sci;

    /* start parsing */
    if ((err_num = editorconfig_parse(DOC_FILENAME(gd), eh)) != 0 &&
            /* Ignore full path error, whose error code is
             * EDITORCONFIG_PARSE_NOT_FULL_PATH */
            err_num != EDITORCONFIG_PARSE_NOT_FULL_PATH) {
        editorconfig_handle_destroy(eh);
        return err_num;
    }

    parse_editorconfig(eh, &ec);

    /* apply the settings */
    if (ec.indent_style) {
        if (!strcmp(ec.indent_style, "tab"))
            editor_set_indent_type(gd->editor, GEANY_INDENT_TYPE_TABS);
        else if (!strcmp(ec.indent_style, "space"))
            editor_set_indent_type(gd->editor, GEANY_INDENT_TYPE_SPACES);
    }
    if (ec.indent_size > 0) {
        editor_set_indent_width(gd->editor, ec.indent_size);

        /*
         * We set the tab width here, so that this could be overrided then
         * if ec.tab_wdith > 0
         */
        scintilla_send_message(sci, SCI_SETTABWIDTH,
                (uptr_t)ec.indent_size, 0);
    }

    if (ec.tab_width > 0)
        scintilla_send_message(sci, SCI_SETTABWIDTH,
                (uptr_t)ec.tab_width, 0);

    if (ec.indent_size == INDENT_SIZE_TAB) {
        int cur_tabwidth = scintilla_send_message(sci, SCI_GETTABWIDTH, 0, 0);

        /* set indent_size to tab_width here */
        scintilla_send_message(sci, SCI_SETINDENT, (uptr_t)cur_tabwidth, 0);
    }

    /* set eol */
    if (ec.end_of_line) {
        if (!strcmp(ec.end_of_line, "lf"))
            scintilla_send_message(sci, SCI_SETEOLMODE, (uptr_t)SC_EOL_LF, 0);
        else if (!strcmp(ec.end_of_line, "crlf"))
            scintilla_send_message(sci, SCI_SETEOLMODE, (uptr_t)SC_EOL_CRLF, 0);
        else if (!strcmp(ec.end_of_line, "cr"))
            scintilla_send_message(sci, SCI_SETEOLMODE, (uptr_t)SC_EOL_CR, 0);
    }

    editorconfig_handle_destroy(eh);

    return 0;
}

/*
 * Reload EditorConfig menu call back
 */
    static void
menu_item_reload_editorconfig_cb(GtkMenuItem* menuitem, gpointer user_data)
{
    int                 err_num;
    GeanyDocument*      gd = document_get_current();

    /* if gd is NULL, do nothing */
    if (!gd)
        return;

    /* reload EditorConfig */
    if ((err_num = load_editorconfig(gd)) != 0) {
        dialogs_show_msgbox(GTK_MESSAGE_ERROR,
                "Failed to reload EditorConfig.");
    }
}

    static void
on_document_open(GObject* obj, GeanyDocument* gd, gpointer user_data)
{
    int err_num;

    if (!gd)
        return;

    /* reload EditorConfig */
    if ((err_num = load_editorconfig(gd)) != 0) {
        dialogs_show_msgbox(GTK_MESSAGE_ERROR,
                "Failed to reload EditorConfig.");
    }
}

    static void
on_document_save(GObject* obj, GeanyDocument* gd, gpointer user_data)
{
    if (!gd) {
        return;
    }

    struct editorconfig     ec;
    editorconfig_handle     eh = editorconfig_handle_init();
    GeanyEditor*            editor = gd->editor;

    if (editorconfig_parse(DOC_FILENAME(gd), eh) != 0) {
        return;
    }
    parse_editorconfig(eh, &ec);

    if (ec.insert_final_newline) {
        gint max_lines = sci_get_line_count(editor->sci);
        gboolean append_newline = (max_lines == 1);
        gint end_document = sci_get_position_from_line(
            editor->sci, max_lines);
        if (max_lines > 1) {
            append_newline = end_document > sci_get_position_from_line(
                editor->sci, max_lines - 1);
        }
        if (append_newline) {
            const gchar *eol = editor_get_eol_char(editor);
            sci_insert_text(editor->sci, end_document, eol);
        }
    }
}

    static void
on_geany_startup_complete(GObject* obj, gpointer user_data)
{
    int     i;
    int     err_num;

    /* load EditorConfig for each GeanyDocument on startup */
    foreach_document(i) {
        if ((err_num = load_editorconfig(documents[i])) != 0)
            dialogs_show_msgbox(GTK_MESSAGE_ERROR,
                    "Failed to reload EditorConfig.");
    }
}

    void
plugin_init(GeanyData* data)
{
    /* Create a new menu item and show it */
    menu_item_reload_editorconfig = gtk_menu_item_new_with_mnemonic(
            "Reload EditorConfig");
    gtk_widget_show(menu_item_reload_editorconfig);

    /* Attach the new menu item to the Tools menu */
    gtk_container_add(GTK_CONTAINER(geany->main_widgets->tools_menu),
            menu_item_reload_editorconfig);

    /*
     * Connect the menu item with a callback function
     * which is called when the item is clicked
     */
    g_signal_connect(menu_item_reload_editorconfig, "activate",
            G_CALLBACK(menu_item_reload_editorconfig_cb), NULL);
}

    void
plugin_cleanup(void)
{
    gtk_widget_destroy(menu_item_reload_editorconfig);
}

