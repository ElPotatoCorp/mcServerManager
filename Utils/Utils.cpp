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

    // TODO: Rename all parameters by 'properties'
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

    // TODO: Rename all parameters by 'properties'
    void rewrite_parameter(const std::string& file_path, const std::string& parameter, const std::string& new_value) 
    {
        std::ifstream infile(file_path);
        if (!infile.is_open()) throw std::runtime_error("File is not open");

        std::string line;
        std::vector<std::string> lines;
        bool found = false;
        while (getline(infile, line))
        {
            if (line.find(parameter) != std::string::npos)
            {
                line = parameter + "=" + new_value;
                found = true;
            }
            lines.push_back(line);
        }
        infile.close();

        if (!found) 
        {
            std::cout << "The value has not been found..." << "\n";
            return;
        }

        std::ofstream outfile(file_path);
        if (!outfile.is_open()) throw std::runtime_error("File is not open");

        for (const std::string& line : lines)
        {
            outfile << line << "\n";
        }

        outfile.close();
    }
}