#include "window.h"

namespace MCSM
{
    MCServerManagerWindow::MCServerManagerWindow()
        : m_ip_port_HBox(Gtk::Orientation::HORIZONTAL),
          m_server_properties_VBox(Gtk::Orientation::VERTICAL),
          m_run_VBox(Gtk::Orientation::VERTICAL),
          m_run_from_backup_CheckButton("Run from Backup"),
          m_run_Button("Launch"),
          m_copy_address_Button("cp"),
          m_open_start_script_Button("📂"),
          m_world_name_Label("World Name"),
          m_description_Label("Description"),
          m_editable_port_Label("Editable Port"),
          m_max_players_Label("Max Players"),
          m_view_distance_Label("View Distance"),
          m_game_mode_Label("Game Mode"),
          m_difficulty_Label("Difficulty"),
          m_hardcore_CheckButton("Hardcore"),
          m_pvp_CheckButton("PVP"),
          m_fly_CheckButton("Allow Flight"),
          m_nether_CheckButton("Allow Nether"),
          m_whitelist_CheckButton("Enable Whitelist"),
          m_start_script_HBox(Gtk::Orientation::HORIZONTAL)
    {
        set_title("MC Server Manager++");

        m_Grid.set_margin(10);

        set_child(m_Grid);

        // Init important values
        m_server_StringList = Gtk::StringList::create(list_servers(SERVER_FOLDER));

        current_server = m_server_StringList.get()->get_string(0);
        serv_props_path = SERVER_FOLDER + "/" + current_server + "/" + "server.properties";

        // Add widgets to the grid
        // - Top IP-PORT part
        m_IP_address_Entry.set_can_focus(false);
        m_IP_address_Entry.set_editable(false);
        m_IP_address_Entry.set_text(cpr::Get(cpr::Url{"https://ipinfo.io/ip"}).text);
        m_PORT_Entry.set_can_focus(false);
        m_PORT_Entry.set_editable(false);
        m_PORT_Entry.set_text(find_val_in_file_by_prop(serv_props_path, PORT_PROPERTY));
        m_copy_address_Button.signal_clicked().connect(sigc::mem_fun(*this, &MCServerManagerWindow::on_copy_button_clicked));

        m_ip_port_HBox.append(m_IP_address_Entry);
        m_ip_port_HBox.append(m_PORT_Entry);
        m_ip_port_HBox.append(m_copy_address_Button);

        m_Grid.attach(m_ip_port_HBox, 0, 0, 2, 1);

        // - Left Part
        //    - Server List
        m_server_name_DropDown.set_model(m_server_StringList);
        m_server_name_DropDown.set_selected(0);

        m_server_properties_VBox.append(m_server_name_DropDown);

        //    - Start Script
        m_start_script_Entry.set_editable(false);
        m_start_script_Entry.set_can_focus(false);

        m_open_start_script_Button.signal_clicked().connect(sigc::mem_fun(*this, &MCServerManagerWindow::on_open_start_script_button_clicked));

        m_start_script_HBox.append(m_start_script_Entry);
        m_start_script_HBox.append(m_open_start_script_Button);

        m_server_properties_VBox.append(m_start_script_HBox);

        //    - Setting up the properties Grid
        m_properties_Grid.set_column_spacing(5);

        //    - World Name
        m_world_name_Label.set_halign(Gtk::Align::START);
        m_world_name_Entry.set_text(find_val_in_file_by_prop(serv_props_path, WORLD_NAME_PROPERTY));

        m_world_name_Entry.signal_activate().connect(
            sigc::bind(sigc::mem_fun(*this, &MCServerManagerWindow::on_entry_confirmed), &m_world_name_Entry, WORLD_NAME_PROPERTY));

        m_properties_Grid.attach(m_world_name_Label, 0, 0);
        m_properties_Grid.attach(m_world_name_Entry, 1, 0);

        //    - Description
        m_description_Label.set_halign(Gtk::Align::START);
        m_description_Entry.set_text(find_val_in_file_by_prop(serv_props_path, DESCRIPTION_PROPERTY));

        m_description_Entry.signal_activate().connect(
            sigc::bind(sigc::mem_fun(*this, &MCServerManagerWindow::on_entry_confirmed), &m_description_Entry, DESCRIPTION_PROPERTY));

        m_properties_Grid.attach(m_description_Label, 0, 1);
        m_properties_Grid.attach(m_description_Entry, 1, 1);

        //    - Port
        m_editable_port_Label.set_halign(Gtk::Align::START);
        m_editable_port_Entry.set_text(find_val_in_file_by_prop(serv_props_path, PORT_PROPERTY));

        m_editable_port_Entry.signal_activate().connect(
            sigc::bind(sigc::mem_fun(*this, &MCServerManagerWindow::on_entry_confirmed), &m_editable_port_Entry, PORT_PROPERTY));

        m_properties_Grid.attach(m_editable_port_Label, 0, 2);
        m_properties_Grid.attach(m_editable_port_Entry, 1, 2);

        //    - Max Players
        m_max_players_Label.set_halign(Gtk::Align::START);
        m_max_players_SpinButton = Gtk::SpinButton(Gtk::Adjustment::create(atoi(find_val_in_file_by_prop(serv_props_path, MAX_PLAYERS_PROPERTY).data()), 1, 20));

        m_max_players_SpinButton.signal_value_changed().connect(sigc::bind(
            sigc::mem_fun(*this, &MCServerManagerWindow::on_spinbutton_value_changed), &m_max_players_SpinButton, MAX_PLAYERS_PROPERTY));

        m_properties_Grid.attach(m_max_players_Label, 0, 3);
        m_properties_Grid.attach(m_max_players_SpinButton, 1, 3);

        //    - View Distance
        m_view_distance_Label.set_halign(Gtk::Align::START);
        m_view_distance_SpinButton = Gtk::SpinButton(Gtk::Adjustment::create(atoi(find_val_in_file_by_prop(serv_props_path, VIEW_DISTANCE_PROPERTY).data()), 1, 64));

        m_view_distance_SpinButton.signal_value_changed().connect(sigc::bind(
            sigc::mem_fun(*this, &MCServerManagerWindow::on_spinbutton_value_changed), &m_view_distance_SpinButton, VIEW_DISTANCE_PROPERTY));

        m_properties_Grid.attach(m_view_distance_Label, 0, 4);
        m_properties_Grid.attach(m_view_distance_SpinButton, 1, 4);

        //    - Gamemode
        m_gamemode_StringList = Gtk::StringList::create({"survival", "creative", "spectator", "adventure"});
        
        m_game_mode_Label.set_halign(Gtk::Align::START);
        m_game_mode_DropDown.set_model(m_gamemode_StringList);

        m_game_mode_DropDown.property_selected().signal_changed().connect(
            sigc::bind(sigc::mem_fun(*this, &MCServerManagerWindow::on_dropdown_value_changed), &m_game_mode_DropDown, m_gamemode_StringList, GAMEMODE_PROPERTY));

        m_properties_Grid.attach(m_game_mode_Label, 0, 5);
        m_properties_Grid.attach(m_game_mode_DropDown, 1, 5);

        //    - Difficulty
        m_difficulty_StringList = Gtk::StringList::create({"peaceful", "easy", "normal", "hard"});

        m_difficulty_Label.set_halign(Gtk::Align::START);
        m_difficulty_DropDown.set_model(m_difficulty_StringList);

        m_difficulty_DropDown.property_selected().signal_changed().connect(
            sigc::bind(sigc::mem_fun(*this, &MCServerManagerWindow::on_dropdown_value_changed), &m_difficulty_DropDown, m_difficulty_StringList, DIFFICULTY_PROPERTY));

        m_properties_Grid.attach(m_difficulty_Label, 0, 6);
        m_properties_Grid.attach(m_difficulty_DropDown, 1, 6);

        //    - Hardcore
        m_hardcore_CheckButton.set_direction(Gtk::TextDirection::RTL);
        m_hardcore_CheckButton.set_margin(-4);

        m_properties_Grid.attach(m_hardcore_CheckButton, 0, 7, 2, 1);

        //    - PVP
        m_pvp_CheckButton.set_direction(Gtk::TextDirection::RTL);
        m_pvp_CheckButton.set_margin(-4);

        m_properties_Grid.attach(m_pvp_CheckButton, 0, 8, 2, 1);

        //    - Fly
        m_fly_CheckButton.set_direction(Gtk::TextDirection::RTL);
        m_fly_CheckButton.set_margin(-4);

        m_properties_Grid.attach(m_fly_CheckButton, 0, 9, 2, 1);

        //    - Nether
        m_nether_CheckButton.set_direction(Gtk::TextDirection::RTL);
        m_nether_CheckButton.set_margin(-4);

        m_properties_Grid.attach(m_nether_CheckButton, 0, 10, 2, 1);

        //    - Whitelist
        m_whitelist_CheckButton.set_direction(Gtk::TextDirection::RTL);
        m_whitelist_CheckButton.set_margin(-4);

        m_properties_Grid.attach(m_whitelist_CheckButton, 0, 11, 2, 1);

        m_server_properties_VBox.append(m_properties_Grid);

        m_Grid.attach(m_server_properties_VBox, 0, 1, 1, 1);
        m_Grid.attach(m_run_VBox, 1, 1, 1, 1);
    }

    MCServerManagerWindow::~MCServerManagerWindow()
    {
        // Cleanup code here
    }

    void MCServerManagerWindow::on_copy_button_clicked()
    {
        get_clipboard()->set_text(m_IP_address_Entry.get_text() + ":" + m_PORT_Entry.get_text());
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
        dialog->set_initial_folder(Gio::File::create_for_path(SERVER_FOLDER + "/" + current_server));

        // Show the dialog and wait for a user response:
        dialog->open(sigc::bind(sigc::mem_fun(*this, &MCServerManagerWindow::on_start_script_file_dialog_finish), dialog));
    }

    void MCServerManagerWindow::on_start_script_file_dialog_finish(const Glib::RefPtr<Gio::AsyncResult> &result, const Glib::RefPtr<Gtk::FileDialog> &dialog)
    {
        // Handle the response:
        try
        {
            Glib::RefPtr<Gio::File> file = dialog->open_finish(result);

            start_script_path = file->get_path();

            std::string file_name = start_script_path.substr(start_script_path.find_last_of('/') + 1);

            m_start_script_Entry.set_text(file_name);
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

        std::string new_value = m_StringList.get()->get_string(selected_item);

        rewrite_property(serv_props_path, property, new_value);
    }

    void MCServerManagerWindow::on_spinbutton_value_changed(const Gtk::SpinButton *m_SpinButton, const std::string &property)
    {
        std::string new_value = std::to_string(m_SpinButton->get_value_as_int());

        rewrite_property(serv_props_path, property, new_value);
    }
        }
}