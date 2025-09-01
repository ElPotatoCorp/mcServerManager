#include "CheckButton.h"

namespace MCSM
{
    CheckButton::CheckButton()
        : m_HBox(Gtk::Orientation::HORIZONTAL), m_Label(), m_CheckButton()
    {
        m_HBox.set_margin(4);

        m_Label.set_margin_end(4);

        m_CheckButton.set_focusable(false);

        m_HBox.append(m_CheckButton);
        m_HBox.append(m_Label);
    }


    CheckButton::CheckButton(const Glib::ustring &name)
        : m_HBox(Gtk::Orientation::HORIZONTAL), m_Label(name), m_CheckButton()
    {
        m_HBox.set_margin(4);

        m_Label.set_margin_end(4);

        m_CheckButton.set_focusable(false);

        m_HBox.append(m_Label);
        m_HBox.append(m_CheckButton);
    }

    CheckButton::~CheckButton() {}

    Gtk::Box &CheckButton::get_box() { return m_HBox; }
    const Gtk::Box &CheckButton::get_box() const { return m_HBox; }

    Gtk::Label &CheckButton::get_label() { return m_Label; }
    const Gtk::Label &CheckButton::get_label() const { return m_Label; }

    Gtk::CheckButton &CheckButton::get_checkbutton() { return m_CheckButton; }
    const Gtk::CheckButton &CheckButton::get_checkbutton() const { return m_CheckButton; }

    CheckButton::operator Gtk::Box &() { return m_HBox; }
    CheckButton::operator const Gtk::Box &() const { return m_HBox; }
}
