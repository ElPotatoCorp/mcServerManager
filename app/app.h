#ifndef MCSM_APP_H
#define MCSM_APP_H

#include <gtkmm.h>
#include <iostream>
#include <exception>
#include "../window/window.h"

namespace MCSM
{
    class MCServerManagerApp: public Gtk::Application
    {
    protected:
        MCServerManagerApp();

    public:
        static Glib::RefPtr<MCServerManagerApp> create();

    protected:
        // Override default signal handlers:
        void on_startup() override;
        void on_activate() override;

    private:
        MCServerManagerWindow *create_appwindow();
        void on_action_preferences();
        void on_action_quit();
    };
}

#endif // MCSM_APP_H