#include "Utils.h"

namespace MCSM
{
    const std::vector<Glib::ustring> list_servers(const std::string& path_to_folder)
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

    const std::string findValueInFileByParameterName(std::fstream& file, const std::string& parameter)
    {
        std::string line;

        file.clear();
        file.seekg(0);
        while (std::getline(file, line)) {
            if (line.find(parameter) != std::string::npos) {
                size_t pos = line.find('=');
                if (pos != std::string::npos) {
                    return line.substr(pos + 1);
                }
            }
        }

        // Not found
        throw std::runtime_error("Parameter not found");
    }
}