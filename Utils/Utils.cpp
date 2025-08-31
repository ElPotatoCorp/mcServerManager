#include "Utils.h"

namespace MCSM
{
    const std::vector<Glib::ustring> list_servers(std::string path_to_folder)
    {
        std::vector<Glib::ustring> servers = {};

        for (const auto & entry : std::filesystem::directory_iterator(path_to_folder))
        {
            if (entry.is_directory())
            {
                std::string file_name = entry.path().filename();
                servers.push_back(Glib::ustring(file_name));
            }
        }

        return servers;
    }

    const std::string findValueInFileByParameterName(std::string parameter_name, std::string file_path)
    {
        
    }
}