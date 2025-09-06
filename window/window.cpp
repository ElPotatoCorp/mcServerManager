#include "window.h"

namespace MCSM
{
    MCServerManagerWindow::MCServerManagerWindow() :
        m_ip_port_HBox(Gtk::Orientation::HORIZONTAL),
        m_server_properties_VBox(Gtk::Orientation::VERTICAL),
        m_run_VBox(Gtk::Orientation::VERTICAL),
        m_start_script_HBox(Gtk::Orientation::HORIZONTAL)
    {
        set_title("MC Server Manager++");

        m_Grid.set_margin(10);
        m_Grid.set_row_spacing(10);
        m_Grid.set_column_spacing(10);

        set_child(m_Grid);

        // Init important values
        m_server_StringList = Gtk::StringList::create(list_directories(SERVER_FOLDER));

        current_server = m_server_StringList->get_string(0);
        current_server_directory = SERVER_FOLDER + "/" + current_server;
        serv_props_path = current_server_directory + "/" + "server.properties";

        const bool has_config_dir_been_created = create_config_directory();

        if (!create_server_config_file(current_server))
        {
            start_script_name = find_val_in_file_by_prop("./config/" + current_server + ".properties", START_SCRIPT_NAME_PROPERTY);
        }

        // Add widgets to the grid
        init_IP_address_PORT_section();

        separators.push_back(Gtk::Separator(Gtk::Orientation::HORIZONTAL));
        separators.push_back(Gtk::Separator(Gtk::Orientation::VERTICAL));

        separators[0].set_vexpand(false);
        m_Grid.attach(separators[0], 0, 1, 3, 1);

        init_server_list_section();
        init_server_properties_secions();

        separators[1].set_hexpand(false);
        separators[1].set_margin_top(-10);
        m_Grid.attach(separators[1], 1, 2, 1, 1);
        
        init_launcher_sections();

        if (!has_config_dir_been_created)
        {
            refresh_serv_infos();
        }

        m_run_Button.grab_focus();
    }

    MCServerManagerWindow::~MCServerManagerWindow()
    {
        // Cleanup code here
    }

    void MCServerManagerWindow::refresh_list_view(Glib::RefPtr<Gtk::StringList> &m_StringList, Glib::RefPtr<Gtk::SingleSelection> &m_SingleSelection, Gtk::ListView &m_ListView)
    {
        m_StringList = Gtk::StringList::create(list_files(current_server_directory + "/" + "backups"));
        m_SingleSelection = Gtk::SingleSelection::create(m_StringList);

        m_ListView.set_model(m_SingleSelection);
    }

    void MCServerManagerWindow::refresh_serv_infos()
    {
        if (!create_server_config_file(current_server))
        {
            start_script_name = find_val_in_file_by_prop("./config/" + current_server + ".properties", START_SCRIPT_NAME_PROPERTY);
            m_start_script_Entry.set_text(start_script_name);
        }

        m_PORT_Entry.set_text(find_val_in_file_by_prop(serv_props_path, PORT_PROPERTY));
        m_world_name_Entry.set_text(find_val_in_file_by_prop(serv_props_path, WORLD_NAME_PROPERTY));
        m_description_Entry.set_text(find_val_in_file_by_prop(serv_props_path, DESCRIPTION_PROPERTY));
        m_editable_port_Entry.set_text(find_val_in_file_by_prop(serv_props_path, PORT_PROPERTY));

        m_max_players_SpinButton.set_value(atoi(find_val_in_file_by_prop(serv_props_path, MAX_PLAYERS_PROPERTY).c_str()));
        m_view_distance_SpinButton.set_value(atoi(find_val_in_file_by_prop(serv_props_path, VIEW_DISTANCE_PROPERTY).c_str()));

        m_gamemode_DropDown.set_selected(m_gamemode_StringList->find(find_val_in_file_by_prop(serv_props_path, GAMEMODE_PROPERTY)));
        m_difficulty_DropDown.set_selected(m_difficulty_StringList->find(find_val_in_file_by_prop(serv_props_path, DIFFICULTY_PROPERTY)));

        m_hardcore_CheckButton.set_active(find_val_in_file_by_prop(serv_props_path, HARDCORE_PROPERTY) == "true" ? true : false);
        m_pvp_CheckButton.set_active(find_val_in_file_by_prop(serv_props_path, PVP_PROPERTY) == "true" ? true : false);
        m_fly_CheckButton.set_active(find_val_in_file_by_prop(serv_props_path, FLY_PROPERTY) == "true" ? true : false);
        m_nether_CheckButton.set_active(find_val_in_file_by_prop(serv_props_path, NETHER_PROPERTY) == "true" ? true : false);
        m_whitelist_CheckButton.set_active(find_val_in_file_by_prop(serv_props_path, WHITELIST_PROPERTY) == "true" ? true : false);

        world_name = m_world_name_Entry.get_text();

        refresh_list_view(m_backups_StringList, m_backups_SingleSelection, m_backups_ListView);
    }
  
    void MCServerManagerWindow::delete_current_world()
    {
        std::string world_name = m_world_name_Entry.get_text();

        std::string world_parent_path = current_server_directory + "/";

        std::string world_path = world_name + world_parent_path;

        if (!world_name.empty() && world_path != world_parent_path && std::filesystem::exists(world_path))
        {
            std::filesystem::remove(world_path);
        }
    }

    void MCServerManagerWindow::make_backup()
    {
        std::string world_path = current_server_directory + "/" + m_world_name_Entry.get_text();

        if (world_path != current_server_directory + "/")
        {
            easy_zip(world_path, current_server_directory + "/" + "backups");
        }

        refresh_list_view(m_backups_StringList, m_backups_SingleSelection, m_backups_ListView);
    }

    void MCServerManagerWindow::load_backup()
    {
        guint m_pos = m_backups_SingleSelection->get_selected();

        if (m_pos == GTK_INVALID_LIST_POSITION)
        {
            return;
        }

        std::string backup_name = m_backups_StringList->get_string(m_pos);

        std::string backup_path = current_server_directory + "/" + "backups" + "/" + backup_name;

        delete_current_world();

        easy_unzip(backup_path, current_server_directory + "/");
    }

    void MCServerManagerWindow::on_copy_button_clicked()
    {
        get_clipboard()->set_text(m_IP_address_Entry.get_text() + ":" + m_PORT_Entry.get_text());
    }

    void MCServerManagerWindow::on_server_dropdown_value_changed()
    {
        guint m_pos = m_server_name_DropDown.get_selected();

        if (m_pos == GTK_INVALID_LIST_POSITION)
        {
            return;
        }

        current_server = m_server_StringList->get_string(m_pos);
        current_server_directory = SERVER_FOLDER + "/" + current_server;
        serv_props_path = current_server_directory + "/" + "server.properties";

        if (!create_server_config_file(current_server))
        {
            start_script_name = find_val_in_file_by_prop("./config/" + current_server + ".properties", START_SCRIPT_NAME_PROPERTY);
            m_start_script_Entry.set_text(start_script_name);
        }

        refresh_serv_infos();
    }

    void MCServerManagerWindow::on_open_start_script_button_clicked()
    {
        auto dialog = Gtk::FileDialog::create();

        // Add filters, so that only certain file types can be selected:
        auto filters = Gio::ListStore<Gtk::FileFilter>::create();

        Glib::RefPtr<Gtk::FileFilter> filter_sh = Gtk::FileFilter::create();
        filter_sh->set_name("Bash scripts");
        filter_sh->add_pattern("*.sh");
        filters->append(filter_sh);

        Glib::RefPtr<Gtk::FileFilter> filter_text = Gtk::FileFilter::create();
        filter_text->set_name("Text files");
        filter_text->add_mime_type("text/plain");
        filters->append(filter_text);

        Glib::RefPtr<Gtk::FileFilter> filter_any = Gtk::FileFilter::create();
        filter_any->set_name("Any files");
        filter_any->add_pattern("*");
        filters->append(filter_any);

        dialog->set_filters(filters);
        dialog->set_initial_folder(Gio::File::create_for_path(current_server_directory));

        // Show the dialog and wait for a user response:
        dialog->open(sigc::bind(sigc::mem_fun(*this, &MCServerManagerWindow::on_start_script_file_dialog_finish), dialog));
    }

    void MCServerManagerWindow::on_run_button_clicked()
    {
        if (start_script_name.empty())
        {
            std::cout << "You did not set any starting script yet." << "\n";
            return on_open_start_script_button_clicked();
        }

        if (m_run_from_backup_CheckButton.get_active())
        {
            load_backup();
        }

        std::string command = "ptyxis -- bash -c \"bash __start_server__ \"" + current_server_directory + "/" + start_script_name + "\"; exec bash\" &";
        std::system(command.c_str());

        close();
    }

    void MCServerManagerWindow::on_start_script_file_dialog_finish(const Glib::RefPtr<Gio::AsyncResult> &result, const Glib::RefPtr<Gtk::FileDialog> &dialog)
    {
        // Handle the response:
        try
        {
            Glib::RefPtr<Gio::File> file = dialog->open_finish(result);
            
            start_script_name = file->get_basename();

            m_start_script_Entry.set_text(start_script_name);

            rewrite_property("./config/" + current_server + ".properties", START_SCRIPT_NAME_PROPERTY, start_script_name);
        }
        catch (const Gtk::DialogError &err)
        {
            // Can be thrown by dialog->open_finish(result).
            std::cout << "No file selected. " << err.what() << std::endl;
        }
        catch (const Glib::Error &err)
        {
            std::cout << "Unexpected exception. " << err.what() << std::endl;
        }
    }

    void MCServerManagerWindow::on_entry_confirmed(const Gtk::Entry *m_Entry, const std::string &property)
    {
        if (m_Entry == &m_editable_port_Entry)
        {
            for (char c : m_Entry->get_text())
            {
                if (!isdigit(c))
                {
                    return;
                }
            }
        } 
        else if (m_Entry == &m_world_name_Entry && std::filesystem::exists(current_server_directory + "/" + world_name))
        {
            std::filesystem::rename((current_server_directory + "/" + world_name).c_str(), (current_server_directory + "/" + m_Entry->get_text()).c_str());
            world_name = m_Entry->get_text();
        }

        std::string new_value = m_Entry->get_text();

        rewrite_property(serv_props_path, property, new_value);
    }

    void MCServerManagerWindow::on_dropdown_value_changed(const Gtk::DropDown *m_DropDown, const Glib::RefPtr<Gtk::StringList> &m_StringList, const std::string &property)
    {
        guint selected_item = m_DropDown->get_selected();

        if (selected_item == GTK_INVALID_LIST_POSITION)
        {
            return;
        }

        std::string new_value = m_StringList->get_string(selected_item);

        rewrite_property(serv_props_path, property, new_value);
    }

    void MCServerManagerWindow::on_spinbutton_value_changed(const Gtk::SpinButton *m_SpinButton, const std::string &property)
    {
        std::string new_value = std::to_string(m_SpinButton->get_value_as_int());

        rewrite_property(serv_props_path, property, new_value);
    }

    void MCServerManagerWindow::on_button_toggled(const Gtk::CheckButton *m_CheckButton, const std::string &property)
    {
        std::string new_value = (m_CheckButton->get_active() ? "true" : "false");

        rewrite_property(serv_props_path, property, new_value);
    }

    void MCServerManagerWindow::on_setup_label(const Glib::RefPtr<Gtk::ListItem>& list_item)
    {
        list_item->set_child(*Gtk::make_managed<Gtk::Label>("", Gtk::Align::START));
    }

    void MCServerManagerWindow::on_bind_name(const Glib::RefPtr<Gtk::ListItem>& list_item) 
    {
        guint pos = list_item->get_position();
        if (pos == GTK_INVALID_LIST_POSITION)
            return;
        Gtk::Label *label = dynamic_cast<Gtk::Label*>(list_item->get_child());
        if (!label)
            return;
        label->set_text(m_backups_StringList->get_string(pos));
    };
}