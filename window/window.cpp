#include "window.h"

namespace MCSM
{
    MCServerManagerWindow::MCServerManagerWindow()
        : m_ip_port_HBox(Gtk::Orientation::HORIZONTAL),
          m_server_parameters_VBox(Gtk::Orientation::VERTICAL),
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
          m_start_script_HBox(Gtk::Orientation::HORIZONTAL),
          m_world_name_HBox(Gtk::Orientation::HORIZONTAL),
          m_description_HBox(Gtk::Orientation::HORIZONTAL),
          m_editable_port_HBox(Gtk::Orientation::HORIZONTAL),
          m_max_players_HBox(Gtk::Orientation::HORIZONTAL),
          m_view_distance_HBox(Gtk::Orientation::HORIZONTAL),
          m_game_mode_HBox(Gtk::Orientation::HORIZONTAL),
          m_difficulty_HBox(Gtk::Orientation::HORIZONTAL)
    {
        set_title("MC Server Manager++");

        m_Grid.set_margin(10);

        set_child(m_Grid);

        // Init important values 
        m_StringList = Gtk::StringList::create(list_servers(SERVER_FOLDER));

        current_server = m_StringList.get()->get_string(0);
        server_properties_file.open(SERVER_FOLDER + "/" + current_server + "/" + "server.properties");

        // Add widgets to the grid
        // - Top IP-PORT part
        m_IP_address_Entry.set_can_focus(false);
        m_IP_address_Entry.set_editable(false);
        m_IP_address_Entry.set_text(cpr::Get(cpr::Url{"https://ipinfo.io/ip"}).text);
        m_Port_Entry.set_can_focus(false);
        m_Port_Entry.set_editable(false);
        m_Port_Entry.set_text(findValueInFileByParameterName(server_properties_file, PORT_PARAMETER));
        m_copy_address_Button.signal_clicked().connect(sigc::mem_fun(*this, &MCServerManagerWindow::on_copy_button_clicked));
    
        m_ip_port_HBox.append(m_IP_address_Entry);
        m_ip_port_HBox.append(m_Port_Entry);
        m_ip_port_HBox.append(m_copy_address_Button);

        m_Grid.attach(m_ip_port_HBox, 0, 0, 2, 1);

        // - Left Part
        //    - Server List
        m_server_name_DropDown.set_model(m_StringList);
        m_server_name_DropDown.set_selected(0);
        
        m_server_parameters_VBox.append(m_server_name_DropDown);

        //    - Start Script
        m_start_script_Entry.set_editable(false);
        m_start_script_Entry.set_focusable(false);

        m_open_start_script_Button.signal_clicked().connect(sigc::mem_fun(*this, &MCServerManagerWindow::on_open_start_script_button_clicked));

        m_start_script_HBox.append(m_start_script_Entry);
        m_start_script_HBox.append(m_open_start_script_Button);

        m_server_parameters_VBox.append(m_start_script_HBox);

        //    - World Name
        m_world_name_Entry.set_text(findValueInFileByParameterName(server_properties_file, WORLD_NAME_PARAMETER));

        m_world_name_HBox.append(m_world_name_Label);
        m_world_name_HBox.append(m_world_name_Entry);

        m_server_parameters_VBox.append(m_world_name_HBox);

        //    - Description
        m_description_Entry.set_text(findValueInFileByParameterName(server_properties_file, DESCRIPTION_PARAMETER));

        m_description_HBox.append(m_description_Label);
        m_description_HBox.append(m_description_Entry);

        m_server_parameters_VBox.append(m_description_HBox);

        //    - Port
        m_editable_port_Entry.set_text(findValueInFileByParameterName(server_properties_file, PORT_PARAMETER));

        m_editable_port_HBox.append(m_editable_port_Label);
        m_editable_port_HBox.append(m_editable_port_Entry);

        m_server_parameters_VBox.append(m_editable_port_HBox);

        //    - Max Players
        m_max_players_SpinButton = Gtk::SpinButton(Gtk::Adjustment::create(atoi(findValueInFileByParameterName(server_properties_file, MAX_PLAYERS_PARAMETER).data()), 1, 20));

        m_max_players_HBox.append(m_max_players_Label);
        m_max_players_HBox.append(m_max_players_SpinButton);

        m_server_parameters_VBox.append(m_max_players_HBox);

        //    - View Distance


        //    - Gamemode


        //    - Difficulty


        //    - Hardcore


        //    - PVP


        //    - Fly


        //    - Nether


        //    - Whitelist


        m_Grid.attach(m_server_parameters_VBox, 0, 1, 1, 1);

        m_Grid.attach(m_run_VBox, 1, 1, 1, 1);
    }

    MCServerManagerWindow::~MCServerManagerWindow()
    {
        // Cleanup code here
        if (server_properties_file.is_open())
            server_properties_file.close();
    }

    void MCServerManagerWindow::on_copy_button_clicked() 
    {
        std::cout << "on_copy_button_clicked" << std::endl;
    }

    void MCServerManagerWindow::on_open_start_script_button_clicked() 
    {
        std::cout << "on_open_start_script_button_clicked" << std::endl;
    }
}