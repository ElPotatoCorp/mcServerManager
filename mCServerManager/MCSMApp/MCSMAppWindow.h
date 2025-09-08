#ifndef MCSM_APP_WINDOW_H
#define MCSM_APP_WINDOW_H

#include <gtk/gtk.h>
#include "MCSMApp.h"

#define MCSM_APP_WINDOW_TYPE (mcsm_app_window_get_type())

G_DECLARE_FINAL_TYPE(MCSMAppWindow, mcsm_app_window, MCSM, APP_WINDOW, GtkApplicationWindow)

MCSMAppWindow *mcsm_app_window_new(MCSMApp *app);

void mcsm_app_window_activate(MCSMAppWindow *win);

static void on_copy_button_clicked(GtkButton *button, MCSMAppWindow *win);

#endif // MCSM_APP_WINDOW_H