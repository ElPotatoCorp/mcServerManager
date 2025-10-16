#ifndef MCSM_APP_H
#define MCSM_APP_H

#include <gtk/gtk.h>

#define MCSM_APP_TYPE (mcsm_app_get_type())

G_DECLARE_FINAL_TYPE(MCSMApp, mcsm_app, MCSM, APP, GtkApplication)

MCSMApp *mcsm_app_new(void);

#endif // MCSM_APP_H