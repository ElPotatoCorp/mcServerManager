#ifndef MCSM_H
#define MCSM_H

#include <gtkmm.h>
#include <iostream>
#include <fstream>
#include <cpr/cpr.h>
#include "../Utils/Utils.h"
#include "../Utils/Constants.h"

// TODO: Rename all parameters by 'properties'
// TODO: Remove the parameters' box to replace it with a grid for better layout.
namespace MCSM
{
    class MCServerManagerWindow : public Gtk::Window
    {
    public:
        MCServerManagerWindow();
        ~MCServerManagerWindow() override;

    protected:
        std::string current_server;

        std::fstream server_properties_file;

        // Member widgets:
        Gtk::Grid m_Grid;

        Gtk::Box m_ip_port_HBox, m_server_parameters_VBox, m_run_VBox;

        Gtk::Entry m_IP_address_Entry, m_Port_Entry;

        Glib::RefPtr<Gtk::StringList> m_StringList;

        Gtk::DropDown m_server_name_DropDown;

        Gtk::ListBox m_backup_ListBox;

        Gtk::CheckButton m_run_from_backup_CheckButton;

        Gtk::Button m_run_Button, m_copy_address_Button, m_open_start_script_Button;

        Gtk::Label m_world_name_Label, m_description_Label, m_editable_port_Label, m_max_players_Label,
            m_view_distance_Label, m_game_mode_Label, m_difficulty_Label;

        Gtk::Entry m_start_script_Entry, m_world_name_Entry, m_description_Entry, m_editable_port_Entry;

        Gtk::SpinButton m_max_players_SpinButton, m_view_distance_SpinButton;

        Gtk::DropDown m_game_mode_DropDown, m_difficulty_DropDown;

        Gtk::CheckButton m_hardcore_CheckButton, m_pvp_CheckButton,
            m_fly_CheckButton, m_nether_CheckButton, m_whitelist_CheckButton;

        Gtk::Box m_start_script_HBox, m_world_name_HBox, m_description_HBox, m_editable_port_HBox,
            m_max_players_HBox, m_view_distance_HBox, m_game_mode_HBox, m_difficulty_HBox;

        // Signal handlers:
        void on_copy_button_clicked();

        void on_open_start_script_button_clicked();
    };
}

#endif // MCSM_H