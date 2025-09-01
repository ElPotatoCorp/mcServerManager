#ifndef MCSM_CHECK_BUTTON_H
#define MCSM_CHECK_BUTTON_H

#include <gtkmm.h>

namespace MCSM
{
    class CheckButton
    {
    public:
        CheckButton();
        CheckButton(const Glib::ustring &name);
        ~CheckButton();

        Gtk::Box &get_box();
        const Gtk::Box &get_box() const;

        Gtk::Label &get_label();
        const Gtk::Label &get_label() const;

        Gtk::CheckButton &get_checkbutton();
        const Gtk::CheckButton &get_checkbutton() const;

        operator Gtk::Box &();
        operator const Gtk::Box &() const;

    protected:
        Gtk::Box m_HBox;
        Gtk::Label m_Label;
        Gtk::CheckButton m_CheckButton;
    };
}

#endif // MCSM_CHECK_BUTTON_H
