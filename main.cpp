#include "window/window.h"
#include <gtkmm/application.h>

int main(int argc, char *argv[])
{
    // Create the config directory if it doesn't exist
    MCSM::create_config_directory();

    auto app = Gtk::Application::create("MC Server Manager");

    // Shows the window and returns when it is closed.
    return app->make_window_and_run<MCSM::MCServerManagerWindow>(argc, argv);
}
