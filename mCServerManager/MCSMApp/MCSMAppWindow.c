#include "MCSMApp.h"
#include "MCSMAppWindow.h"

static void reverse_check_button(GtkWidget *widget)
{
    gtk_widget_set_direction(widget, GTK_TEXT_DIR_RTL);
    gtk_widget_set_margin_start(widget, -4);
    gtk_widget_set_margin_top(widget, -4);
    gtk_widget_set_margin_end(widget, -4);
    gtk_widget_set_margin_bottom(widget, -4);
}

struct _MCSMAppWindow
{
    GtkApplicationWindow parent;

    GtkStringList *server_name_StringList;

    GtkWidget *gears;
    GtkWidget *serv_dir_loaded_Label;
    GtkWidget *ip_Entry, *port_Entry, *start_script_Entry, *world_name_Entry, *description_Entry, *editable_port_Entry;
    GtkWidget *copy_Button, *start_script_Button, *make_backup_Button, run_Button;
    GtkWidget *server_name_DropDown, *gamemode_DropDown, *difficulty_DropDown;
    GtkWidget *max_players_SpinButton, *view_distance_SpinButton;
    GtkWidget *hardcore_CheckButton, *pvp_CheckButton, *fly_CheckButton, *nether_CheckButton, *whitelist_CheckButton, *run_backup_CheckButton;
    GtkWidget *backups_ListView;
};

G_DEFINE_TYPE(MCSMAppWindow, mcsm_app_window, GTK_TYPE_APPLICATION_WINDOW)

static void mcsm_app_window_init(MCSMAppWindow *win)
{
    GtkBuilder *builder;
    GMenuModel *menu;

    gtk_widget_init_template(GTK_WIDGET(win));

    builder = gtk_builder_new_from_resource("/mcsm/menu.xml");
    menu = G_MENU_MODEL(gtk_builder_get_object(builder, "menu"));
    gtk_menu_button_set_menu_model(GTK_MENU_BUTTON(win->gears), menu);
    g_object_unref(builder);

    reverse_check_button(win->hardcore_CheckButton);
    reverse_check_button(win->pvp_CheckButton);
    reverse_check_button(win->fly_CheckButton);
    reverse_check_button(win->nether_CheckButton);
    reverse_check_button(win->whitelist_CheckButton);
    reverse_check_button(win->run_backup_CheckButton);

    /* --- Signals Handler --- */
    
}

static void mcsm_app_window_class_init(MCSMAppWindowClass *class)
{
    GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(class);

    gtk_widget_class_set_template_from_resource(widget_class, "/mcsm/window.xml");

    gtk_widget_class_bind_template_child(widget_class, MCSMAppWindow, gears);
    gtk_widget_class_bind_template_child(widget_class, MCSMAppWindow, serv_dir_loaded_Label);
    gtk_widget_class_bind_template_child(widget_class, MCSMAppWindow, ip_Entry);
    gtk_widget_class_bind_template_child(widget_class, MCSMAppWindow, port_Entry);
    gtk_widget_class_bind_template_child(widget_class, MCSMAppWindow, start_script_Entry);
    gtk_widget_class_bind_template_child(widget_class, MCSMAppWindow, world_name_Entry);
    gtk_widget_class_bind_template_child(widget_class, MCSMAppWindow, description_Entry);
    gtk_widget_class_bind_template_child(widget_class, MCSMAppWindow, editable_port_Entry);
    gtk_widget_class_bind_template_child(widget_class, MCSMAppWindow, copy_Button);
    gtk_widget_class_bind_template_child(widget_class, MCSMAppWindow, start_script_Button);
    gtk_widget_class_bind_template_child(widget_class, MCSMAppWindow, make_backup_Button);
    gtk_widget_class_bind_template_child(widget_class, MCSMAppWindow, run_Button);
    gtk_widget_class_bind_template_child(widget_class, MCSMAppWindow, server_name_DropDown);
    gtk_widget_class_bind_template_child(widget_class, MCSMAppWindow, gamemode_DropDown);
    gtk_widget_class_bind_template_child(widget_class, MCSMAppWindow, difficulty_DropDown);
    gtk_widget_class_bind_template_child(widget_class, MCSMAppWindow, max_players_SpinButton);
    gtk_widget_class_bind_template_child(widget_class, MCSMAppWindow, view_distance_SpinButton);
    gtk_widget_class_bind_template_child(widget_class, MCSMAppWindow, hardcore_CheckButton);
    gtk_widget_class_bind_template_child(widget_class, MCSMAppWindow, pvp_CheckButton);
    gtk_widget_class_bind_template_child(widget_class, MCSMAppWindow, fly_CheckButton);
    gtk_widget_class_bind_template_child(widget_class, MCSMAppWindow, nether_CheckButton);
    gtk_widget_class_bind_template_child(widget_class, MCSMAppWindow, whitelist_CheckButton);
    gtk_widget_class_bind_template_child(widget_class, MCSMAppWindow, run_backup_CheckButton);
    gtk_widget_class_bind_template_child(widget_class, MCSMAppWindow, backups_ListView);

    gtk_widget_class_bind_template_callback(widget_class, on_copy_button_clicked);
}

MCSMAppWindow *mcsm_app_window_new(MCSMApp *app)
{
    return g_object_new(MCSM_APP_WINDOW_TYPE, "application", app, NULL);
}

void mcsm_app_window_activate(MCSMAppWindow *win)
{
}

static void on_copy_button_clicked(GtkButton *button, MCSMAppWindow *win)
{
    GdkClipboard *clipboard = gtk_widget_get_clipboard(GTK_WIDGET(win));
    gdk_clipboard_set_text(clipboard, "test"); 
}