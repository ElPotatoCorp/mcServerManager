#include "window.h"

namespace MCSM
{
#pragma region Server info
    void MCServerManagerWindow::init_IP_address_PORT_section()
    {
        // IP-PORT
        m_IP_address_Entry.set_can_focus(false);
        m_IP_address_Entry.set_editable(false);
        m_IP_address_Entry.set_text(cpr::Get(cpr::Url{"https://ipinfo.io/ip"}).text);
        m_PORT_Entry.set_can_focus(false);
        m_PORT_Entry.set_editable(false);
        m_copy_address_Button.set_label("cp");

        m_copy_address_Button.signal_clicked().connect(sigc::mem_fun(*this, &MCServerManagerWindow::on_copy_button_clicked));

        m_ip_port_HBox.append(m_IP_address_Entry);
        m_ip_port_HBox.append(m_PORT_Entry);
        m_ip_port_HBox.append(m_copy_address_Button);

        m_Grid.attach(m_ip_port_HBox, 0, 0, 3, 1);
    }

    void MCServerManagerWindow::init_server_list_section()
    {
        // Server List
        m_server_name_DropDown.set_model(m_server_StringList);
        m_server_name_DropDown.set_selected(0);

        m_server_name_DropDown.property_selected().signal_changed().connect(
            sigc::mem_fun(*this, &MCServerManagerWindow::on_server_dropdown_value_changed));

        m_server_properties_VBox.append(m_server_name_DropDown);
    }

    void MCServerManagerWindow::init_start_script_section()
    {
        // Start Script
        if (!start_script_name.empty())
        {
            m_start_script_Entry.set_text(start_script_name);
        }
        m_start_script_Entry.set_editable(false);
        m_start_script_Entry.set_can_focus(false);
        m_start_script_Entry.set_hexpand(true);
        m_open_start_script_Button.set_label("📂");

        m_open_start_script_Button.signal_clicked().connect(sigc::mem_fun(*this, &MCServerManagerWindow::on_open_start_script_button_clicked));

        m_start_script_HBox.set_hexpand(false);

        m_start_script_HBox.append(m_start_script_Entry);
        m_start_script_HBox.append(m_open_start_script_Button);

        m_server_properties_VBox.append(m_start_script_HBox);
    }

    void MCServerManagerWindow::init_world_name_section()
    {
        // World Name
        m_world_name_Label.set_label("World Name");
        m_world_name_Label.set_halign(Gtk::Align::START);

        m_world_name_Entry.signal_activate().connect(
            sigc::bind(sigc::mem_fun(*this, &MCServerManagerWindow::on_entry_confirmed), &m_world_name_Entry, WORLD_NAME_PROPERTY));

        m_properties_Grid.attach(m_world_name_Label, 0, 0);
        m_properties_Grid.attach(m_world_name_Entry, 1, 0);
    }

    void MCServerManagerWindow::init_description_section()
    {
        // Description
        m_description_Label.set_label("Description");
        m_description_Label.set_halign(Gtk::Align::START);

        m_description_Entry.signal_activate().connect(
            sigc::bind(sigc::mem_fun(*this, &MCServerManagerWindow::on_entry_confirmed), &m_description_Entry, DESCRIPTION_PROPERTY));

        m_properties_Grid.attach(m_description_Label, 0, 1);
        m_properties_Grid.attach(m_description_Entry, 1, 1);
    }

    void MCServerManagerWindow::init_editable_port_section()
    {
        // Port
        m_editable_port_Label.set_label("Port");
        m_editable_port_Label.set_halign(Gtk::Align::START);

        m_editable_port_Entry.signal_activate().connect(
            sigc::bind(sigc::mem_fun(*this, &MCServerManagerWindow::on_entry_confirmed), &m_editable_port_Entry, PORT_PROPERTY));

        m_properties_Grid.attach(m_editable_port_Label, 0, 2);
        m_properties_Grid.attach(m_editable_port_Entry, 1, 2);
    }

    void MCServerManagerWindow::init_max_players_section()
    {
        // Max Players
        m_max_players_Label.set_label("Max Players");
        m_max_players_Label.set_halign(Gtk::Align::START);
        m_max_players_SpinButton = Gtk::SpinButton(Gtk::Adjustment::create(1, 1, 20));

        m_max_players_SpinButton.signal_value_changed().connect(sigc::bind(
            sigc::mem_fun(*this, &MCServerManagerWindow::on_spinbutton_value_changed), &m_max_players_SpinButton, MAX_PLAYERS_PROPERTY));

        m_properties_Grid.attach(m_max_players_Label, 0, 3);
        m_properties_Grid.attach(m_max_players_SpinButton, 1, 3);
    }

    void MCServerManagerWindow::init_view_distance_section()
    {
        // View Distance
        m_view_distance_Label.set_label("View Distance");
        m_view_distance_Label.set_halign(Gtk::Align::START);
        m_view_distance_SpinButton = Gtk::SpinButton(Gtk::Adjustment::create(1, 1, 64));

        m_view_distance_SpinButton.signal_value_changed().connect(sigc::bind(
            sigc::mem_fun(*this, &MCServerManagerWindow::on_spinbutton_value_changed), &m_view_distance_SpinButton, VIEW_DISTANCE_PROPERTY));

        m_properties_Grid.attach(m_view_distance_Label, 0, 4);
        m_properties_Grid.attach(m_view_distance_SpinButton, 1, 4);
    }

    void MCServerManagerWindow::init_gamemode_section()
    {
        // Gamemode
        m_gamemode_StringList = Gtk::StringList::create({"survival", "creative", "spectator", "adventure"});
        
        m_gamemode_Label.set_label("Game Mode");
        m_gamemode_Label.set_halign(Gtk::Align::START);
        m_gamemode_DropDown.set_model(m_gamemode_StringList);

        m_gamemode_DropDown.property_selected().signal_changed().connect(
            sigc::bind(sigc::mem_fun(*this, &MCServerManagerWindow::on_dropdown_value_changed), &m_gamemode_DropDown, m_gamemode_StringList, GAMEMODE_PROPERTY));

        m_properties_Grid.attach(m_gamemode_Label, 0, 5);
        m_properties_Grid.attach(m_gamemode_DropDown, 1, 5);
    }

    void MCServerManagerWindow::init_difficulty_section()
    {
        // Difficulty
        m_difficulty_StringList = Gtk::StringList::create({"peaceful", "easy", "normal", "hard"});

        m_difficulty_Label.set_label("Difficulty");
        m_difficulty_Label.set_halign(Gtk::Align::START);
        m_difficulty_DropDown.set_model(m_difficulty_StringList);

        m_difficulty_DropDown.property_selected().signal_changed().connect(
            sigc::bind(sigc::mem_fun(*this, &MCServerManagerWindow::on_dropdown_value_changed), &m_difficulty_DropDown, m_difficulty_StringList, DIFFICULTY_PROPERTY));

        m_properties_Grid.attach(m_difficulty_Label, 0, 6);
        m_properties_Grid.attach(m_difficulty_DropDown, 1, 6);
    }

    void MCServerManagerWindow::init_hardcore_section()
    {
        // Hardcore
        m_hardcore_CheckButton.set_label("Hardcore");
        m_hardcore_CheckButton.set_direction(Gtk::TextDirection::RTL);
        m_hardcore_CheckButton.set_margin(-4);

        m_hardcore_CheckButton.signal_toggled().connect(sigc::bind(
            sigc::mem_fun(*this, &MCServerManagerWindow::on_button_toggled), &m_hardcore_CheckButton, HARDCORE_PROPERTY));

        m_properties_Grid.attach(m_hardcore_CheckButton, 0, 7, 2, 1);
    }

    void MCServerManagerWindow::init_pvp_section()
    {
        // PVP
        m_pvp_CheckButton.set_label("PVP");
        m_pvp_CheckButton.set_direction(Gtk::TextDirection::RTL);
        m_pvp_CheckButton.set_margin(-4);

        m_pvp_CheckButton.signal_toggled().connect(sigc::bind(
            sigc::mem_fun(*this, &MCServerManagerWindow::on_button_toggled), &m_pvp_CheckButton, PVP_PROPERTY));

        m_properties_Grid.attach(m_pvp_CheckButton, 0, 8, 2, 1);
    }

    void MCServerManagerWindow::init_fly_section()
    {
        // Fly
        m_fly_CheckButton.set_label("Allow Flight");
        m_fly_CheckButton.set_direction(Gtk::TextDirection::RTL);
        m_fly_CheckButton.set_margin(-4);

        m_fly_CheckButton.signal_toggled().connect(sigc::bind(
            sigc::mem_fun(*this, &MCServerManagerWindow::on_button_toggled), &m_fly_CheckButton, FLY_PROPERTY));

        m_properties_Grid.attach(m_fly_CheckButton, 0, 9, 2, 1);
    }

    void MCServerManagerWindow::init_nether_section()
    {
        // Nether
        m_nether_CheckButton.set_label("Allow Nether");
        m_nether_CheckButton.set_direction(Gtk::TextDirection::RTL);
        m_nether_CheckButton.set_margin(-4);

        m_nether_CheckButton.signal_toggled().connect(sigc::bind(
            sigc::mem_fun(*this, &MCServerManagerWindow::on_button_toggled), &m_nether_CheckButton, NETHER_PROPERTY));

        m_properties_Grid.attach(m_nether_CheckButton, 0, 10, 2, 1);
    }

    void MCServerManagerWindow::init_whitelist_section()
    {
        // Whitelist
        m_whitelist_CheckButton.set_label("Enable Whitelist");
        m_whitelist_CheckButton.set_direction(Gtk::TextDirection::RTL);
        m_whitelist_CheckButton.set_margin(-4);

        m_whitelist_CheckButton.signal_toggled().connect(sigc::bind(
            sigc::mem_fun(*this, &MCServerManagerWindow::on_button_toggled), &m_whitelist_CheckButton, WHITELIST_PROPERTY));

        m_properties_Grid.attach(m_whitelist_CheckButton, 0, 11, 2, 1);
    }

    void MCServerManagerWindow::init_server_properties_secions()
    {
        init_start_script_section();

        // Setting up the properties Grid
        m_properties_Grid.set_margin_top(10);
        m_properties_Grid.set_column_spacing(5);

        init_world_name_section();
        init_description_section();
        init_editable_port_section();
        init_max_players_section();
        init_view_distance_section();
        init_gamemode_section();
        init_difficulty_section();
        init_hardcore_section();
        init_pvp_section();
        init_fly_section();
        init_nether_section();
        init_whitelist_section();
        
        // Attach elements to the main container
        m_server_properties_VBox.append(m_properties_Grid);
        m_Grid.attach(m_server_properties_VBox, 0, 2, 1, 1);
    }
#pragma endregion

#pragma region Server launcher
    void MCServerManagerWindow::init_backups_list_section()
    {
        // Backups
        m_backups_StringList = Gtk::StringList::create(list_files(current_server_directory + "/" + "backups"));

        m_backups_SingleSelection = Gtk::SingleSelection::create(m_backups_StringList);
        m_backups_SingleSelection->set_autoselect(false);
        m_backups_SingleSelection->set_can_unselect(true);

        m_backups_ListView.set_model(m_backups_SingleSelection);
        m_backups_ListView.add_css_class("data-table");

        Glib::RefPtr<Gtk::SignalListItemFactory> factory = Gtk::SignalListItemFactory::create();
        factory = Gtk::SignalListItemFactory::create();
        factory->signal_setup().connect(
            sigc::mem_fun(*this, &MCServerManagerWindow::on_setup_label));
        factory->signal_bind().connect(
            sigc::mem_fun(*this, &MCServerManagerWindow::on_bind_name));
        m_backups_ListView.set_factory(factory);

        m_backups_ScrolledWindow.set_policy(Gtk::PolicyType::AUTOMATIC, Gtk::PolicyType::AUTOMATIC);
        m_backups_ScrolledWindow.set_expand(true);

        m_backups_ScrolledWindow.set_child(m_backups_ListView);

        m_backups_Frame.set_child(m_backups_ScrolledWindow);

        m_run_VBox.append(m_backups_Frame);
    }

    void MCServerManagerWindow::init_make_backup_button_section()
    {
        m_make_backup_button.set_label("Make Backup");
        m_make_backup_button.set_margin_bottom(5);
        m_make_backup_button.signal_clicked().connect(sigc::mem_fun(*this, &MCServerManagerWindow::make_backup));

        m_run_VBox.append(m_make_backup_button);
    }

    void MCServerManagerWindow::init_run_backup_section()
    {
        m_run_from_backup_CheckButton.set_label("Run From Backup");
        //m_run_from_backup_CheckButton.set_halign(Gtk::Align::END);
        m_run_from_backup_CheckButton.set_direction(Gtk::TextDirection::RTL);
        m_run_from_backup_CheckButton.set_margin(-4);

        m_run_VBox.append(m_run_from_backup_CheckButton);
    }

    void MCServerManagerWindow::init_run_button_section()
    {
        m_run_Button.set_label("RUN");
        m_run_Button.signal_clicked().connect(sigc::mem_fun(*this, &MCServerManagerWindow::on_run_button_clicked));

        m_run_VBox.append(m_run_Button);
    }

    void MCServerManagerWindow::init_launcher_sections()
    {
        m_run_VBox.set_size_request(250, -1);
        m_run_VBox.set_hexpand(false);

        init_make_backup_button_section();
        init_backups_list_section();
        init_run_backup_section();
        init_run_button_section();

        m_Grid.attach(m_run_VBox, 2, 2, 1, 1);
    }
#pragma endregion
}