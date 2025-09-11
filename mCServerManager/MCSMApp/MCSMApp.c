#include "MCSMApp.h"
#include "MCSMAppWindow.h"

struct _MCSMApp
{
    GtkApplication parent;

    MCSMAppWindow *win;
};

G_DEFINE_TYPE(MCSMApp, mcsm_app, GTK_TYPE_APPLICATION);

static void preferences_activated(GSimpleAction *action, GVariant *parameter, gpointer app) {}

static void quit_activated(GSimpleAction *action, GVariant *parameter, gpointer app)
{
    GApplication *g_app = G_APPLICATION(app);

    MCSMApp *mcsm_app = MCSM_APP(g_app);

    if (mcsm_app->win)
    {
        on_window_destroyed(NULL, mcsm_app->win);
    }

    g_application_quit(g_app);
}

static GActionEntry app_entries[] =
{
    {"preferences", preferences_activated, NULL, NULL, NULL},
    {"quit", quit_activated, NULL, NULL, NULL}
};

static void mcsm_app_startup(GApplication *app)
{
    const char *quit_accels[2] = { "<Ctrl>Q", NULL };

    G_APPLICATION_CLASS(mcsm_app_parent_class)->startup(app);

    g_action_map_add_action_entries(G_ACTION_MAP(app), app_entries, G_N_ELEMENTS(app_entries), app);
    gtk_application_set_accels_for_action(GTK_APPLICATION(app), "app.quit", quit_accels);
}

static void mcsm_app_activate(GApplication *app)
{
    MCSMApp *mcsm_app = MCSM_APP(app);

    mcsm_app->win = mcsm_app_window_new(mcsm_app);
    gtk_window_present(GTK_WINDOW(mcsm_app->win));
}

static void mcsm_app_init(MCSMApp *app) {}

static void mcsm_app_class_init(MCSMAppClass *class)
{
    G_APPLICATION_CLASS(class)->startup = mcsm_app_startup;
    G_APPLICATION_CLASS(class)->activate = mcsm_app_activate;
}

MCSMApp *mcsm_app_new(void)
{
    return g_object_new(MCSM_APP_TYPE,
                        "application-id", "mc.server.manager",
                        "flags", G_APPLICATION_HANDLES_OPEN,
                        NULL);
}
