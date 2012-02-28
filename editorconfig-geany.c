/*
 * Copyright (c) 2012 EditorConfig Team
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdlib.h>
#include <string.h>
#include <geanyplugin.h>
#include <editorconfig/editorconfig.h>

PLUGIN_VERSION_CHECK(211)

PLUGIN_SET_INFO("EditorConfig", "EditorConfig Plugin for Geany",
        "0.1", "http://editorconfig.org");

GeanyPlugin*         geany_plugin;
GeanyData*           geany_data;
GeanyFunctions*      geany_functions;

/* Reload EditorConfig menu item */ 
static GtkWidget*  menu_item_reload_editorconfig;

    static void
on_document_open(GObject* obj, GeanyDocument* gd, gpointer user_data);

    static void
on_geany_startup_complete(GObject* obj, gpointer user_data);

/* plugin signals */
PluginCallback plugin_callbacks[] =
{
    { "document-open", (GCallback)&on_document_open, TRUE, NULL },
    { "geany-startup-complete",
        (GCallback)&on_geany_startup_complete, TRUE, NULL },
    { NULL, NULL, FALSE, NULL }
};

    static int
load_editorconfig(const GeanyDocument* gd)
{
    struct
    {
        const char*     indent_style;
#define INDENT_SIZE_TAB (-1000) /* indent_size = -1000 means indent_size = tab*/
        int             indent_size;
        int             tab_width;
        const char*     end_of_line;
    } ecConf; /* obtained EditorConfig settings will be here */

    int                     i;
    editorconfig_handle     eh = editorconfig_handle_init();
    int                     err_num;
    int                     name_value_count;
    ScintillaObject*        sci = gd->editor->sci;

    memset(&ecConf, 0, sizeof(ecConf));

    /* start parsing */
    if ((err_num = editorconfig_parse(gd->file_name, eh)) != 0 &&
            /* Ignore full path error, whose error code is
             * EDITORCONFIG_PARSE_NOT_FULL_PATH */
            err_num != EDITORCONFIG_PARSE_NOT_FULL_PATH) {
        editorconfig_handle_destroy(eh);
        return err_num;
    }

    /* apply the settings */

    name_value_count = editorconfig_handle_get_name_value_count(eh);

    /* get settings */
    for (i = 0; i < name_value_count; ++i) {
        const char* name;
        const char* value;

        editorconfig_handle_get_name_value(eh, i, &name, &value);

        if (!strcmp(name, "indent_style"))
            ecConf.indent_style = value;
        else if (!strcmp(name, "tab_width"))
            ecConf.tab_width = atoi(value);
        else if (!strcmp(name, "indent_size")) {
            int     value_i = atoi(value);

            if (!strcmp(value, "tab"))
                ecConf.indent_size = INDENT_SIZE_TAB;
            else if (value_i > 0)
                ecConf.indent_size = value_i;
        }
        else if (!strcmp(name, "end_of_line"))
            ecConf.end_of_line = value;
    }

    if (ecConf.indent_style) {
        if (!strcmp(ecConf.indent_style, "tab"))
            scintilla_send_message(sci, SCI_SETUSETABS, (uptr_t)1, 0);
        else if (!strcmp(ecConf.indent_style, "space"))
            scintilla_send_message(sci, SCI_SETUSETABS, (uptr_t)0, 0);
    }
    if (ecConf.indent_size > 0) {
        scintilla_send_message(sci, SCI_SETINDENT,
                (uptr_t)ecConf.indent_size, 0);

        /*
         * We set the tab width here, so that this could be overrided then
         * if ecConf.tab_wdith > 0
         */
        scintilla_send_message(sci, SCI_SETTABWIDTH,
                (uptr_t)ecConf.indent_size, 0);
    }

    if (ecConf.tab_width > 0)
        scintilla_send_message(sci, SCI_SETTABWIDTH,
                (uptr_t)ecConf.tab_width, 0);

    if (ecConf.indent_size == INDENT_SIZE_TAB) {
        int cur_tabwidth = scintilla_send_message(sci, SCI_GETTABWIDTH, 0, 0);

        /* set indent_size to tab_width here */ 
        scintilla_send_message(sci, SCI_SETINDENT, (uptr_t)cur_tabwidth, 0);
    }

    /* set eol */
    if (ecConf.end_of_line) {
        if (!strcmp(ecConf.end_of_line, "lf"))
            scintilla_send_message(sci, SCI_SETEOLMODE, (uptr_t)SC_EOL_LF, 0);
        else if (!strcmp(ecConf.end_of_line, "crlf"))
            scintilla_send_message(sci, SCI_SETEOLMODE, (uptr_t)SC_EOL_CRLF, 0);
        else if (!strcmp(ecConf.end_of_line, "cr"))
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

