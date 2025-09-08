#include "app.h"

namespace MCSM
{
    MCServerManagerApp::MCServerManagerApp()
        : Gtk::Application("MC Server Manager", Gio::Application::Flags::HANDLES_OPEN)
    {
    }

    Glib::RefPtr<MCServerManagerApp> MCServerManagerApp::create()
    {
        return Glib::make_refptr_for_instance<MCServerManagerApp>(new MCServerManagerApp());
    }

    MCServerManagerWindow *MCServerManagerApp::create_appwindow()
    {
        MCServerManagerWindow *appwindow = MCServerManagerWindow::create();

        // Make sure that the application runs for as long this window is still open.
        add_window(*appwindow);

        // A window can be added to an application with Gtk::Application::add_window()
        // or Gtk::Window::set_application(). When all added windows have been hidden
        // or removed, the application stops running (Gtk::Application::run() returns()),
        // unless Gio::Application::hold() has been called.

        // Delete the window when it is hidden.
        appwindow->signal_hide().connect([appwindow]()
                                         { delete appwindow; });

        return appwindow;
    }

    void MCServerManagerApp::on_startup()
    {
        // Call the base class's implementation.
        Gtk::Application::on_startup();

        // Add actions and keyboard accelerators for the menu.
        add_action("preferences", sigc::mem_fun(*this, &MCServerManagerApp::on_action_preferences));
        add_action("quit", sigc::mem_fun(*this, &MCServerManagerApp::on_action_quit));
        set_accel_for_action("app.quit", "<Ctrl>Q");
    }

    void MCServerManagerApp::on_activate()
    {
        try
        {
            // The application has been started, so let's show a window.
            auto appwindow = create_appwindow();
            appwindow->present();
        }
        catch (const Glib::Error &ex)
        {
            std::cerr << "MCServerManagerApp::on_activate(): " << ex.what() << std::endl;
        }
        catch (const std::exception &ex)
        {
            std::cerr << "MCServerManagerApp::on_activate(): " << ex.what() << std::endl;
        }
    }

    void MCServerManagerApp::on_action_preferences()
    {
    }

    void MCServerManagerApp::on_action_quit()
    {
        // Gio::Application::quit() will make Gio::Application::run() return,
        // but it's a crude way of ending the program. The window is not removed
        // from the application. Neither the window's nor the application's
        // destructors will be called, because there will be remaining reference
        // counts in both of them. If we want the destructors to be called, we
        // must remove the window from the application. One way of doing this
        // is to hide the window. See comment in create_appwindow().
        std::vector<Gtk::Window *> windows = get_windows();
        for (Gtk::Window *window : windows)
            window->set_visible(false);

        // Not really necessary, when Gtk::Widget::set_visible(false) is called,
        // unless Gio::Application::hold() has been called without a corresponding
        // call to Gio::Application::release().
        quit();
    }

}