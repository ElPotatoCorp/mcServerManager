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
    public:
        MCServerManagerWindow();
        ~MCServerManagerWindow() override;

    protected:
        std::string current_server, start_script_path, serv_props_path;

        // Member widgets:
        Gtk::Grid m_Grid, m_properties_Grid;

        Gtk::Box m_ip_port_HBox, m_server_properties_VBox, m_run_VBox;

        Gtk::Entry m_IP_address_Entry, m_PORT_Entry;

        Glib::RefPtr<Gtk::StringList> m_server_StringList, m_gamemode_StringList, m_difficulty_StringList;

        Gtk::DropDown m_server_name_DropDown;

        Gtk::ListBox m_backup_ListBox;

        Gtk::CheckButton m_run_from_backup_CheckButton;

        Gtk::Button m_run_Button, m_copy_address_Button, m_open_start_script_Button;

        Gtk::Label m_world_name_Label, m_description_Label, m_editable_port_Label, m_max_players_Label,
            m_view_distance_Label, m_gamemode_Label, m_difficulty_Label;

        Gtk::Entry m_start_script_Entry, m_world_name_Entry, m_description_Entry, m_editable_port_Entry;

        Gtk::SpinButton m_max_players_SpinButton, m_view_distance_SpinButton;

        Gtk::DropDown m_gamemode_DropDown, m_difficulty_DropDown;

        Gtk::CheckButton m_hardcore_CheckButton, m_pvp_CheckButton,
            m_fly_CheckButton, m_nether_CheckButton, m_whitelist_CheckButton;

        Gtk::Box m_start_script_HBox;

        // Signal handlers:
        void on_copy_button_clicked();

        void on_open_start_script_button_clicked();

        void on_start_script_file_dialog_finish(const Glib::RefPtr<Gio::AsyncResult>& result, const Glib::RefPtr<Gtk::FileDialog>& dialog);

        void on_entry_confirmed(const Gtk::Entry *m_Entry, const std::string &property);

        void on_dropdown_value_changed(const Gtk::DropDown *m_DropDown, const Glib::RefPtr<Gtk::StringList> &m_StringList, const std::string &property);

        void on_spinbutton_value_changed(const Gtk::SpinButton *m_SpinButton, const std::string &property);

        void on_button_toggled(const Gtk::CheckButton *m_CheckButton, const std::string &property);
        
    };
}

#endif // MCSM_H