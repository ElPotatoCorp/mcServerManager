#include "MCSMApp/MCSMApp.h"

int main(int argc,
         char *argv[])
{
#ifdef GTK_SRCDIR
  g_chdir(GTK_SRCDIR);
#endif

  return g_application_run(G_APPLICATION(mcsm_app_new()), argc, argv);
}
