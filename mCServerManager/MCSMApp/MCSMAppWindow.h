#ifndef MCSM_APP_WINDOW_H
#define MCSM_APP_WINDOW_H

#include <gtk/gtk.h>

#define MCSM_APP_WINDOW_TYPE (mcsm_app_window_get_type())

G_DECLARE_FINAL_TYPE(MCSMAppWindow, mcsm_app_window, MCSM, APP_WINDOW, GtkApplicationWindow)

MCSMAppWindow *mcsm_app_window_new(MCSMApp *app);

void mcsm_app_window_activate(MCSMAppWindow *win);

static void refresh_serv_infos(MCSMAppWindow *win);

static void setup_listitem_cb(GtkListItemFactory *factory, GtkListItem *list_item);
static void bind_listitem_cb(GtkListItemFactory *factory, GtkListItem *list_item);
static void on_server_drop_down_selected(GtkDropDown *drop_down, MCSMAppWindow *win);
static void on_copy_button_clicked(GtkButton *button, MCSMAppWindow *win);
static void on_entry_activated(GtkEntry *entry, MCSMAppWindow *win);
static void on_spin_button_value_changed(GtkSpinButton *spin_button, MCSMAppWindow *win);
static void on_drop_down_selected(GtkDropDown *drop_down, MCSMAppWindow *win);
static void on_check_button_toggled(GtkCheckButton *check_button, GtkWindow *win);
void on_window_destroyed(GtkWindow *gtk_win, MCSMAppWindow *win);

#endif // MCSM_APP_WINDOW_H