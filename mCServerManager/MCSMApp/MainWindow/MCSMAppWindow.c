#include "../MCSMApp.h"
#include "MCSMAppWindow.h"
#include "../../Utils/Constants.h"
#include "../../Utils/Utils.h"

static char *server_directory = NULL, *server_config_file = NULL, *current_server = NULL, *current_server_directory = NULL, *start_script_name = NULL, *server_properties = NULL, *world_name = NULL;

static void reverse_check_button(GtkWidget *widget)
{
    gtk_widget_set_direction    (widget, GTK_TEXT_DIR_RTL);
    gtk_widget_set_margin_start (widget, -4);
    gtk_widget_set_margin_top   (widget, -4);
    gtk_widget_set_margin_end   (widget, -4);
    gtk_widget_set_margin_bottom(widget, -4);
}

static void gtk_entry_set_text(GtkEntry *entry, const char *str)
{
    if (is_str_empty(str))
    {
        return;
    }

    GtkEntryBuffer *entry_buffer = gtk_entry_get_buffer(entry);

    gtk_entry_buffer_set_text(entry_buffer, str, strlen(str));
}

typedef struct
{
    MCSMAppWindow *win;
    struct StringList *str_data;
} ThreadData;

struct _MCSMAppWindow
{
    GtkApplicationWindow parent;

    GtkStringList *server_name_StringList, *backups_StringList;

    GtkSingleSelection *backups_SingleSelection;
    GtkListItemFactory *backups_Factory;

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

static void init_list_view(MCSMAppWindow *win)
{
    // Create the factory
    win->backups_Factory = mcsm_g_object_new(gtk_signal_list_item_factory_new());
    g_signal_connect(win->backups_Factory, "setup", G_CALLBACK(setup_listitem_cb), NULL);
    g_signal_connect(win->backups_Factory, "bind", G_CALLBACK(bind_listitem_cb), NULL);

    // Assign factory to the list view
    gtk_list_view_set_factory(GTK_LIST_VIEW(win->backups_ListView), GTK_LIST_ITEM_FACTORY(win->backups_Factory));

    // Create an empty string list and wrap in single selection
    win->backups_StringList = gtk_string_list_new(NULL); // No 'mcsm_g_object_new' because the backups_SingleSelection will take the ownership.
    win->backups_SingleSelection = mcsm_g_object_new(gtk_single_selection_new(G_LIST_MODEL(win->backups_StringList)));

    // Assign model to the list view
    gtk_list_view_set_model(GTK_LIST_VIEW(win->backups_ListView), GTK_SELECTION_MODEL(win->backups_SingleSelection));
}

static void init_ip_entry(MCSMAppWindow *win)
{
    char *ip = curl_from_url("https://ifconfig.me/ip");
    if (ip == NULL)
    {
        gtk_entry_set_text(GTK_ENTRY(win->ip_Entry), "INVALID");
    }
    else
    {
        gtk_entry_set_text(GTK_ENTRY(win->ip_Entry), ip);
        mcsm_free(ip);
    }
}

static void init_server_name_drop_down(MCSMAppWindow *win)
{
    char *config_file_path = concat_all_strings(2, CONFIG_FOLDER_PATH, "/.config");
    server_directory = get_value_from_properties_file(config_file_path, SERVER_DIR_PROPERTY);

    if (!is_directory(server_directory))
    {
        return;
    }

    gtk_widget_set_visible(win->serv_dir_loaded_Label, FALSE);

    struct StringList *servers = list_directories_from_path(server_directory);

    char **str_servers = mcsm_malloc(servers->size * sizeof(char *));
    for (int i = 0; i < servers->size; i++)
    {
        char *server = servers->strings[i];

        str_servers[i] = strset(server);
    }
    str_servers[servers->size] = NULL;

    win->server_name_StringList = mcsm_g_object_new(gtk_string_list_new((const char *const *)str_servers));

    gtk_drop_down_set_model(GTK_DROP_DOWN(win->server_name_DropDown), G_LIST_MODEL(win->server_name_StringList));
    gtk_drop_down_set_selected(GTK_DROP_DOWN(win->server_name_DropDown), 0);

    current_server = strset(str_servers[0]);

    /* --- free Section --- */
    for (int i = 0; i < servers->size; i++)
    {
        mcsm_free(str_servers[i]);
    }
    mcsm_free(str_servers);
    free_string_list(servers);
    mcsm_free(config_file_path);
}

static void init_key_values(MCSMAppWindow *win)
{
    char *relative_path;

    relative_path = concat_all_strings(4, CONFIG_FOLDER_PATH, "/", current_server, ".properties");
    get_real_path(&server_config_file, relative_path);
    mcsm_free(relative_path);

    relative_path = concat_all_strings(4, server_directory, "/", current_server, "/");
    get_real_path(&current_server_directory, relative_path);
    mcsm_free(relative_path);

    relative_path = concat_all_strings(2, current_server_directory, "/server.properties");
    get_real_path(&server_properties, relative_path);
    mcsm_free(relative_path);
}

static void mcsm_app_window_init(MCSMAppWindow *win)
{
    GtkBuilder *builder;
    GMenuModel *menu;

    gtk_widget_init_template(GTK_WIDGET(win));

    builder = mcsm_g_object_new(gtk_builder_new_from_resource("/mcsm/menu.xml"));
    menu = G_MENU_MODEL(gtk_builder_get_object(builder, "menu"));
    gtk_menu_button_set_menu_model(GTK_MENU_BUTTON(win->gears), menu);
    mcsm_g_object_unref(builder);

    #pragma region Linking Attributes
    g_object_set_data(G_OBJECT(win->world_name_Entry        ), "prop", (char *)WORLD_NAME_PROPERTY   );
    g_object_set_data(G_OBJECT(win->description_Entry       ), "prop", (char *)DESCRIPTION_PROPERTY  );
    g_object_set_data(G_OBJECT(win->editable_port_Entry     ), "prop", (char *)PORT_PROPERTY         );
    g_object_set_data(G_OBJECT(win->max_players_SpinButton  ), "prop", (char *)MAX_PLAYERS_PROPERTY  );
    g_object_set_data(G_OBJECT(win->view_distance_SpinButton), "prop", (char *)VIEW_DISTANCE_PROPERTY);
    g_object_set_data(G_OBJECT(win->gamemode_DropDown       ), "prop", (char *)GAMEMODE_PROPERTY     );
    g_object_set_data(G_OBJECT(win->difficulty_DropDown     ), "prop", (char *)DIFFICULTY_PROPERTY   );
    g_object_set_data(G_OBJECT(win->hardcore_CheckButton    ), "prop", (char *)HARDCORE_PROPERTY     );
    g_object_set_data(G_OBJECT(win->pvp_CheckButton         ), "prop", (char *)PVP_PROPERTY          );
    g_object_set_data(G_OBJECT(win->fly_CheckButton         ), "prop", (char *)FLY_PROPERTY          );
    g_object_set_data(G_OBJECT(win->nether_CheckButton      ), "prop", (char *)NETHER_PROPERTY       );
    g_object_set_data(G_OBJECT(win->whitelist_CheckButton   ), "prop", (char *)WHITELIST_PROPERTY    );
    #pragma endregion // Linking Attributes

    #pragma region Formatting Button
    reverse_check_button(win->hardcore_CheckButton  );
    reverse_check_button(win->pvp_CheckButton       );
    reverse_check_button(win->fly_CheckButton       );
    reverse_check_button(win->nether_CheckButton    );
    reverse_check_button(win->whitelist_CheckButton );
    reverse_check_button(win->run_backup_CheckButton);
    #pragma endregion // Formatting Button

    init_list_view(win);

    if (create_config_directory())
    {
        return;
    }

    init_server_name_drop_down(win);
    init_key_values(win);
    init_ip_entry(win);

    refresh_serv_infos(win);
}

static void mcsm_app_window_class_init(MCSMAppWindowClass *class)
{
    GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(class);

    gtk_widget_class_set_template_from_resource(widget_class, "/mcsm/window.xml");

    gtk_widget_class_bind_template_child(widget_class, MCSMAppWindow, gears                   );
    gtk_widget_class_bind_template_child(widget_class, MCSMAppWindow, serv_dir_loaded_Label   );
    gtk_widget_class_bind_template_child(widget_class, MCSMAppWindow, ip_Entry                );
    gtk_widget_class_bind_template_child(widget_class, MCSMAppWindow, port_Entry              );
    gtk_widget_class_bind_template_child(widget_class, MCSMAppWindow, start_script_Entry      );
    gtk_widget_class_bind_template_child(widget_class, MCSMAppWindow, world_name_Entry        );
    gtk_widget_class_bind_template_child(widget_class, MCSMAppWindow, description_Entry       );
    gtk_widget_class_bind_template_child(widget_class, MCSMAppWindow, editable_port_Entry     );
    gtk_widget_class_bind_template_child(widget_class, MCSMAppWindow, copy_Button             );
    gtk_widget_class_bind_template_child(widget_class, MCSMAppWindow, start_script_Button     );
    gtk_widget_class_bind_template_child(widget_class, MCSMAppWindow, make_backup_Button      );
    gtk_widget_class_bind_template_child(widget_class, MCSMAppWindow, run_Button              );
    gtk_widget_class_bind_template_child(widget_class, MCSMAppWindow, server_name_DropDown    );
    gtk_widget_class_bind_template_child(widget_class, MCSMAppWindow, gamemode_DropDown       );
    gtk_widget_class_bind_template_child(widget_class, MCSMAppWindow, difficulty_DropDown     );
    gtk_widget_class_bind_template_child(widget_class, MCSMAppWindow, max_players_SpinButton  );
    gtk_widget_class_bind_template_child(widget_class, MCSMAppWindow, view_distance_SpinButton);
    gtk_widget_class_bind_template_child(widget_class, MCSMAppWindow, hardcore_CheckButton    );
    gtk_widget_class_bind_template_child(widget_class, MCSMAppWindow, pvp_CheckButton         );
    gtk_widget_class_bind_template_child(widget_class, MCSMAppWindow, fly_CheckButton         );
    gtk_widget_class_bind_template_child(widget_class, MCSMAppWindow, nether_CheckButton      );
    gtk_widget_class_bind_template_child(widget_class, MCSMAppWindow, whitelist_CheckButton   );
    gtk_widget_class_bind_template_child(widget_class, MCSMAppWindow, run_backup_CheckButton  );
    gtk_widget_class_bind_template_child(widget_class, MCSMAppWindow, backups_ListView        );

    /* --- Signals Handler --- */
    gtk_widget_class_bind_template_callback(widget_class, on_server_drop_down_selected);
    gtk_widget_class_bind_template_callback(widget_class, on_open_start_script_clicked);
    gtk_widget_class_bind_template_callback(widget_class, on_copy_button_clicked);
    gtk_widget_class_bind_template_callback(widget_class, on_entry_activated);
    gtk_widget_class_bind_template_callback(widget_class, on_spin_button_value_changed);
    gtk_widget_class_bind_template_callback(widget_class, on_drop_down_selected);
    gtk_widget_class_bind_template_callback(widget_class, on_check_button_toggled);
    gtk_widget_class_bind_template_callback(widget_class, on_make_backup_button_clicked);
    gtk_widget_class_bind_template_callback(widget_class, on_run_button_clicked);
    gtk_widget_class_bind_template_callback(widget_class, on_window_destroyed);
}

MCSMAppWindow *mcsm_app_window_new(MCSMApp *app)
{
    return g_object_new(MCSM_APP_WINDOW_TYPE, "application", app, NULL);
}

void mcsm_app_window_activate(MCSMAppWindow *win) {}

#pragma region Refresh The Main Display
static void refresh_entry(GtkEntry *entry, const char *properties_file_path, const char *property)
{
    char *value = get_value_from_properties_file(properties_file_path, property);
    gtk_entry_set_text(GTK_ENTRY(entry), value);
    mcsm_free(value);
}

static void refresh_spin_button(GtkSpinButton *spin_button, const char *properties_file_path, const char *property)
{
    int value;
    char *str_value = get_value_from_properties_file(properties_file_path, property);
    if (!is_str_empty(str_value) && sscanf(str_value, "%d", &value))
    {
        gtk_spin_button_set_value(spin_button, value);
    }
    mcsm_free(str_value);
}

static void refresh_drop_down(GtkDropDown *drop_down, const char *properties_file_path, const char *property)
{
    char *value = get_value_from_properties_file(properties_file_path, property);
    value[0] = value[0] - 32;

    GtkStringList *string_list = GTK_STRING_LIST(gtk_drop_down_get_model(drop_down));

    guint pos = gtk_string_list_find(string_list, value);
    if (pos != GTK_INVALID_LIST_POSITION)
    {
        gtk_drop_down_set_selected(drop_down, pos);
    }
    mcsm_free(value);
}

static void refresh_check_button(GtkCheckButton *check_button, const char *properties_file_path, const char *property)
{
    char *value = get_value_from_properties_file(properties_file_path, property);

    if (!is_str_empty(value))
    {
        gtk_check_button_set_active(check_button, strcmp(value, "true") == 0);
    }

    mcsm_free(value);
}

static void refresh_backups_list_view(MCSMAppWindow *win)
{
    char *backups_path = concat_all_strings(2, current_server_directory, "/backups/");
    struct StringList *backups = list_regular_files_from_path(backups_path);

    gtk_string_list_splice(win->backups_StringList, 0, g_list_model_get_n_items(G_LIST_MODEL(win->backups_StringList)), (const char *const *)backups->strings);
    
    mcsm_free(backups_path);
    free_string_list(backups);
}

static void refresh_serv_infos(MCSMAppWindow *win)
{
    if (!create_server_config_file(server_config_file))
    {
        mcsm_free(start_script_name);
        start_script_name = get_value_from_properties_file(server_config_file, START_SCRIPT_NAME_PROPERTY);
        gtk_entry_set_text(GTK_ENTRY(win->start_script_Entry), start_script_name);
    }

    mcsm_free(world_name);
    world_name = get_value_from_properties_file(server_properties, WORLD_NAME_PROPERTY);
    gtk_entry_set_text(GTK_ENTRY(win->world_name_Entry), world_name);

    refresh_entry(GTK_ENTRY(win->description_Entry), server_properties, DESCRIPTION_PROPERTY);

    char *port = get_value_from_properties_file(server_properties, PORT_PROPERTY);
    gtk_entry_set_text(GTK_ENTRY(win->port_Entry), port);
    gtk_entry_set_text(GTK_ENTRY(win->editable_port_Entry), port);
    mcsm_free(port);

    refresh_spin_button(GTK_SPIN_BUTTON(win->max_players_SpinButton), server_properties, MAX_PLAYERS_PROPERTY);
    refresh_spin_button(GTK_SPIN_BUTTON(win->view_distance_SpinButton), server_properties, VIEW_DISTANCE_PROPERTY);

    refresh_drop_down(GTK_DROP_DOWN(win->gamemode_DropDown), server_properties, GAMEMODE_PROPERTY);
    refresh_drop_down(GTK_DROP_DOWN(win->difficulty_DropDown), server_properties, DIFFICULTY_PROPERTY);

    refresh_check_button(GTK_CHECK_BUTTON(win->hardcore_CheckButton), server_properties, HARDCORE_PROPERTY);
    refresh_check_button(GTK_CHECK_BUTTON(win->pvp_CheckButton), server_properties, PVP_PROPERTY);
    refresh_check_button(GTK_CHECK_BUTTON(win->fly_CheckButton), server_properties, FLY_PROPERTY);
    refresh_check_button(GTK_CHECK_BUTTON(win->nether_CheckButton), server_properties, NETHER_PROPERTY);
    refresh_check_button(GTK_CHECK_BUTTON(win->whitelist_CheckButton), server_properties, WHITELIST_PROPERTY);

    refresh_backups_list_view(win);
}
#pragma endregion // Refresh The Main Display

static gboolean on_making_backup_finished(ThreadData *thread_data)
{
    refresh_backups_list_view(thread_data->win);

    if (thread_data != NULL)
    {
        free_string_list(thread_data->str_data);
        mcsm_free(thread_data);
    }

    return G_SOURCE_REMOVE;
}

static void make_backup_async(ThreadData *thread_data)
{
    const char *from = thread_data->str_data->strings[0];
    const char *entry_name = thread_data->str_data->strings[1];
    const char *to = thread_data->str_data->strings[2];

    easy_zip_from_path(from, entry_name, to);

    g_idle_add((GSourceFunc)on_making_backup_finished, thread_data);
}

#pragma region Signals
static void setup_listitem_cb(GtkListItemFactory *factory, GtkListItem *list_item)
{
    GtkWidget *label;

    label = gtk_label_new(NULL);
    gtk_label_set_xalign(GTK_LABEL(label), GTK_ALIGN_FILL);

    gtk_list_item_set_child(list_item, label);
}

static void bind_listitem_cb(GtkListItemFactory *factory, GtkListItem *list_item)
{
    GtkWidget *label = gtk_list_item_get_child(list_item);
    if (label == NULL)
    {
        return;
    }

    GtkStringObject *item = GTK_STRING_OBJECT(gtk_list_item_get_item(list_item));
    if (item == NULL)
    {
        return;
    }

    const char *text = gtk_string_object_get_string(item);

    gtk_label_set_text(GTK_LABEL(label), text ? text : "");
}

static void on_server_drop_down_selected(GtkDropDown *drop_down, GParamSpec *gparam, MCSMAppWindow *win)
{
    if (!gtk_widget_get_realized(GTK_WIDGET(drop_down)))
    {
        return;
    }

    guint pos = gtk_drop_down_get_selected(drop_down);

    if (pos == GTK_INVALID_LIST_POSITION)
    {
        return;
    }

    GtkStringList *string_list = GTK_STRING_LIST(gtk_drop_down_get_model(drop_down));

    mcsm_free(server_config_file);
    mcsm_free(current_server);
    mcsm_free(current_server_directory);
    mcsm_free(server_properties);

    server_config_file = concat_all_strings(4, CONFIG_FOLDER_PATH, "/", current_server, ".properties");
    current_server = strset(gtk_string_list_get_string(string_list, pos));
    current_server_directory = concat_all_strings(4, server_directory, "/", current_server, "/");
    server_properties = concat_all_strings(2, current_server_directory, "/server.properties");

    refresh_serv_infos(win);
}

static void on_open_start_script_clicked(GtkButton *button, MCSMAppWindow *win)
{
    GFile *default_folder = mcsm_g_object_new(g_file_new_for_path(current_server_directory));
    GtkFileDialog *dialog = mcsm_g_object_new(gtk_file_dialog_new());
    GListStore *filters = mcsm_g_object_new(g_list_store_new(GTK_TYPE_FILE_FILTER));
    GCancellable *cancellable = mcsm_g_object_new(g_cancellable_new());
    
    GtkFileFilter *filter = mcsm_g_object_new(gtk_file_filter_new());
    gtk_file_filter_set_name(filter, "Bash");
    gtk_file_filter_add_pattern(filter, "*.sh");
    g_list_store_append(filters, filter);
    mcsm_g_object_unref(filter);

    filter = mcsm_g_object_new(gtk_file_filter_new());
    gtk_file_filter_set_name(filter, "Any");
    gtk_file_filter_add_pattern(filter, "*");
    g_list_store_append(filters, filter);
    mcsm_g_object_unref(filter);

    gtk_file_dialog_set_initial_folder(dialog, default_folder);
    gtk_file_dialog_set_filters(dialog, G_LIST_MODEL(filters));

    gtk_file_dialog_open(dialog, GTK_WINDOW(win), cancellable, on_start_script_file_dialog_finished, win);

    mcsm_g_object_unref(cancellable);
    mcsm_g_object_unref(filters);
    mcsm_g_object_unref(dialog);
    mcsm_g_object_unref(default_folder);
}

static void on_start_script_file_dialog_finished(GObject *object, GAsyncResult *res, gpointer user_data)
{
    GtkFileDialog *dialog = GTK_FILE_DIALOG(object);
    MCSMAppWindow *win = MCSM_APP_WINDOW(user_data);
    GError *err = NULL;

    GFile *file = gtk_file_dialog_open_finish(dialog, res, &err);

    if (err != NULL)
    {
        if (err->code == GTK_DIALOG_ERROR_DISMISSED)
        {
            perror("It has been cancelled by the user.");
        }
        else
        {
            perror(err->message);
        }
        g_error_free(err);
        return;
    }

    if (file == NULL)
    {
        return;
    }

    create_server_config_file(server_config_file);

    char *file_name = g_file_get_basename(file);
    overwrite_property_from_properties_file(server_config_file, START_SCRIPT_NAME_PROPERTY, file_name);
    gtk_entry_set_text(GTK_ENTRY(win->start_script_Entry), file_name);

    mcsm_g_free(file_name);
    mcsm_g_object_unref(file);
}

static void on_copy_button_clicked(GtkButton *button, MCSMAppWindow *win)
{
    GdkClipboard *clipboard = gtk_widget_get_clipboard(GTK_WIDGET(win));

    GtkEntryBuffer *ip_entry_buffer = gtk_entry_get_buffer(GTK_ENTRY(win->ip_Entry));
    GtkEntryBuffer *port_entry_buffer = gtk_entry_get_buffer(GTK_ENTRY(win->port_Entry));

    const char *ip = gtk_entry_buffer_get_text(ip_entry_buffer);
    const char *port = gtk_entry_buffer_get_text(port_entry_buffer);

    char *ip_port = concat_all_strings(3, ip, ":", port);

    gdk_clipboard_set_text(clipboard, ip_port);

    mcsm_free(ip_port);
}

static void on_entry_activated(GtkEntry *entry, MCSMAppWindow *win)
{
    if (server_properties == NULL || is_str_empty(server_properties))
    {
        perror("The server_properties is not set");
        return;
    }

    const char *property = g_object_get_data(G_OBJECT(entry), "prop");

    GtkEntryBuffer *entry_buffer = gtk_entry_get_buffer(entry);

    const char *new_value = gtk_entry_buffer_get_text(entry_buffer);

    if (entry == GTK_ENTRY(win->editable_port_Entry))
    {
        size_t n = strlen(new_value);
        int i = 0;
        int is_a_number = 1;
        while (is_a_number && i < n)
        {
            if (new_value[i] < '0' || new_value[i] > '9')
            {
                is_a_number = 0;
            }
            i++;
        }

        if (!is_a_number)
        {
            perror("Le port n'est pas valide");
            return;
        }
    }

    overwrite_property_from_properties_file(server_properties, property, new_value);
}

static void on_spin_button_value_changed(GtkSpinButton *spin_button, MCSMAppWindow *win)
{
    if (server_properties == NULL || is_str_empty(server_properties))
    {
        perror("The server_properties is not set");
        return;
    }

    const char *property = g_object_get_data(G_OBJECT(spin_button), "prop");

    int int_value = gtk_spin_button_get_value_as_int(spin_button);

    char new_value[10];
    sprintf(new_value, "%d", int_value);

    overwrite_property_from_properties_file(server_properties, property, new_value);
}

static void on_drop_down_selected(GtkDropDown *drop_down, GParamSpec *gparam, MCSMAppWindow *win)
{
    if (server_properties == NULL || is_str_empty(server_properties))
    {
        perror("The server_properties is not set");
        return;
    }

    const char *property = g_object_get_data(G_OBJECT(drop_down), "prop");

    guint pos = gtk_drop_down_get_selected(drop_down);

    if (pos == GTK_INVALID_LIST_POSITION)
    {
        return;
    }

    GtkStringList *string_list = GTK_STRING_LIST(gtk_drop_down_get_model(drop_down));

    char *new_value = strset(gtk_string_list_get_string(string_list, pos));
    new_value[0] = new_value[0] + 32;

    overwrite_property_from_properties_file(server_properties, property, new_value);

    mcsm_free(new_value);
}

static void on_check_button_toggled(GtkCheckButton *check_button, MCSMAppWindow *win)
{
    if (server_properties == NULL || is_str_empty(server_properties))
    {
        perror("The server_properties is not set");
        return;
    }

    const char *property = g_object_get_data(G_OBJECT(check_button), "prop");

    char *new_value = gtk_check_button_get_active(GTK_CHECK_BUTTON(check_button)) ? "true" : "false";

    overwrite_property_from_properties_file(server_properties, property, new_value);
}

static void on_make_backup_button_clicked(GtkButton *button, MCSMAppWindow *win)
{
    char *backups_directory = concat_all_strings(2, current_server_directory, "/backups/");
    
    ThreadData *thread_data = mcsm_malloc(sizeof(ThreadData));
    thread_data->win = win;
    thread_data->str_data = new_string_list_from_strings(3, current_server_directory, world_name, backups_directory);

    GThread *thread = g_thread_new("make-backup-thread", (GThreadFunc)make_backup_async, thread_data);
    g_thread_unref(thread);

    mcsm_free(backups_directory);
}

static void on_run_button_clicked(GtkButton *button, MCSMAppWindow *win)
{
    GtkEntryBuffer *entry_buffer = gtk_entry_get_buffer(GTK_ENTRY(win->start_script_Entry));

    if (is_str_empty(gtk_entry_buffer_get_text(entry_buffer)))
    {
        perror("You did not set any starting script yet.\n");
        return on_open_start_script_clicked(NULL, win);
    }

    char *command = concat_all_strings(5, "ptyxis -- bash -c \"bash __start_server__ \"", current_server_directory, "/", start_script_name, "\"; exec bash\" 2>/dev/null &");
    system(command);
    mcsm_free(command);

    gtk_window_close(GTK_WINDOW(win));
}

void on_window_destroyed(GtkWindow *gtk_win, MCSMAppWindow *win)
{
    if (win == NULL)
    {
        return;
    }

    if (GTK_IS_LIST_VIEW(win->backups_ListView)) {
        gtk_list_view_set_model(GTK_LIST_VIEW(win->backups_ListView), NULL);
        gtk_list_view_set_factory(GTK_LIST_VIEW(win->backups_ListView), NULL);
    }
    mcsm_g_object_unref(win->backups_SingleSelection);
    mcsm_g_object_unref(win->backups_Factory);
    
    mcsm_g_object_unref(win->server_name_StringList);

    mcsm_free(server_directory);
    mcsm_free(current_server);
    mcsm_free(current_server_directory);
    mcsm_free(server_config_file);
    mcsm_free(server_properties);
    mcsm_free(start_script_name);
    mcsm_free(world_name);

    ptr_remaining();
}
#pragma endregion // Signals