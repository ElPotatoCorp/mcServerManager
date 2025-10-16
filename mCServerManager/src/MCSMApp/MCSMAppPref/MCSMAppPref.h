#ifndef MCSM_APP_PREF_WINDOW_H
#define MCSM_APP_PREF_WINDOW_H

#include <gtk/gtk.h>
#include "../MainWindow/MCSMAppWindow.h"

#define MCSM_APP_PREFS_TYPE (mcsm_app_prefs_get_type())

G_DECLARE_FINAL_TYPE(MCSMAppPrefs, mcsm_app_prefs, MCSM, APP_PREFS, GtkDialog)

MCSMAppPrefs *mcsm_app_prefs_new(MCSMAppWindow *win);

#endif // MCSM_APP_PREF_WINDOW_H