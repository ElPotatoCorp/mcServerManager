#include <gtk/gtk.h>

#include "../MCSMApp.h"
#include "../MainWindow/MCSMAppWindow.h"
#include "MCSMAppPref.h"

struct _MCSMAppPrefs
{
    GtkDialog parent;

    GSettings *settings;
    GtkWidget *server_directory_Entry;
};

G_DEFINE_TYPE(MCSMAppPrefs, mcsm_app_prefs, GTK_TYPE_DIALOG)

static void mcsm_app_prefs_init(MCSMAppPrefs *prefs)
{
    gtk_widget_init_template(GTK_WIDGET(prefs));
    prefs->settings = g_settings_new("mcsm");

    g_settings_bind(prefs->settings, "server-directory",
                    prefs->server_directory_Entry, "server-directory",
                    G_SETTINGS_BIND_DEFAULT);
}

static void mcsm_app_prefs_dispose(GObject *object)
{
    MCSMAppPrefs *prefs;

    prefs = MCSM_APP_PREFS(object);

    g_clear_object((gpointer)&prefs->settings);

    G_OBJECT_CLASS(mcsm_app_prefs_parent_class)->dispose(object);
}

static void mcsm_app_prefs_class_init(MCSMAppPrefsClass *class)
{
    G_OBJECT_CLASS(class)->dispose = mcsm_app_prefs_dispose;

    gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(class), "/mcsm/prefs.xml");
    
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), MCSMAppPrefs, server_directory_Entry);
}

MCSMAppPrefs *mcsm_app_prefs_new(MCSMAppWindow *win)
{
    return g_object_new(MCSM_APP_PREFS_TYPE, "transient-for", win, "use-header-bar", TRUE, NULL);
}
