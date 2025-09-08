#include "app/app.h"
#include <gtkmm/application.h>

int main(int argc, char *argv[])
{
    Glib::RefPtr<MCSM::MCServerManagerApp> app = MCSM::MCServerManagerApp::create();

    // Shows the window and returns when it is closed.
    return app->run(argc, argv);
}
