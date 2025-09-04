#ifndef MCSM_H
#define MCSM_H

#include <gtkmm.h>
#include <iostream>
#include <fstream>
#include <cpr/cpr.h>
#include "../Utils/Utils.h"
#include "../Utils/Constants.h"

namespace MCSM
{
    class MCServerManagerWindow : public Gtk::Window
    {
    private:
    #pragma region INIT_METHODS
        void init_IP_address_PORT_section();
        void init_server_list_section();

        /* --- SERVER PROPERTIES --- */
        void init_start_script_section();
        void init_world_name_section();
        void init_description_section();
        void init_editable_port_section();
        void init_max_players_section();
        void init_view_distance_section();
        void init_gamemode_section();
        void init_difficulty_section();
        void init_hardcore_section();
        void init_pvp_section();
        void init_fly_section();
        void init_nether_section();
        void init_whitelist_section();

        void init_server_properties_secions();
        /* --- END: SERVER PROPERTIES --- */

        /* --- LAUNCHER --- */
        void init_backups_list_section();
        void init_launcher_sections();
    #pragma endregion

    public:
        MCServerManagerWindow();
        ~MCServerManagerWindow() override;

    protected:
        std::string current_server, start_script_path, serv_props_path;

        // Member widgets:
        Gtk::Grid m_Grid, m_properties_Grid;

        std::vector<Gtk::Separator> separators;

        Gtk::Box m_ip_port_HBox, m_server_properties_VBox, m_run_VBox;

        Gtk::Entry m_IP_address_Entry, m_PORT_Entry;

        Glib::RefPtr<Gtk::StringList> m_server_StringList;

        // - Server Settings Widgets
        Glib::RefPtr<Gtk::StringList> m_gamemode_StringList, m_difficulty_StringList;

        Gtk::DropDown m_server_name_DropDown;

        Gtk::Button m_run_Button, m_copy_address_Button, m_open_start_script_Button;

        Gtk::Label m_world_name_Label, m_description_Label, m_editable_port_Label, m_max_players_Label,
            m_view_distance_Label, m_gamemode_Label, m_difficulty_Label;

        Gtk::Entry m_start_script_Entry, m_world_name_Entry, m_description_Entry, m_editable_port_Entry;

        Gtk::SpinButton m_max_players_SpinButton, m_view_distance_SpinButton;

        Gtk::DropDown m_gamemode_DropDown, m_difficulty_DropDown;

        Gtk::CheckButton m_hardcore_CheckButton, m_pvp_CheckButton,
            m_fly_CheckButton, m_nether_CheckButton, m_whitelist_CheckButton;

        Gtk::Box m_start_script_HBox;

        // - Launcher Settings Widgets
        Glib::RefPtr<Gtk::StringList> m_backups_StringList;
        
        Gtk::Frame m_backups_Frame;
        
        Gtk::ScrolledWindow m_backups_ScrolledWindow;
        
        Gtk::ListView m_backups_ListView;

        Gtk::CheckButton m_run_from_backup_CheckButton;

        // General functions:
        void refresh_serv_infos();

        // Signal handlers:
        void on_copy_button_clicked();

        void on_open_start_script_button_clicked();

        void on_start_script_file_dialog_finish(const Glib::RefPtr<Gio::AsyncResult>& result, const Glib::RefPtr<Gtk::FileDialog>& dialog);

        void on_entry_confirmed(const Gtk::Entry *m_Entry, const std::string &property);

        void on_dropdown_value_changed(const Gtk::DropDown *m_DropDown, const Glib::RefPtr<Gtk::StringList> &m_StringList, const std::string &property);

        void on_spinbutton_value_changed(const Gtk::SpinButton *m_SpinButton, const std::string &property);

        void on_button_toggled(const Gtk::CheckButton *m_CheckButton, const std::string &property);

        void on_setup_label(const Glib::RefPtr<Gtk::ListItem>& list_item);

        void on_bind_name(const Glib::RefPtr<Gtk::ListItem>& list_item);
    };
}

#endif // MCSM_H