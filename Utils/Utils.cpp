#include "Utils.h"

namespace MCSM
{
    const std::vector<Glib::ustring> list_servers(const std::string &path_to_folder)
    {
        std::vector<Glib::ustring> servers = {};

        for (const auto &entry : std::filesystem::directory_iterator(path_to_folder))
        {
            if (entry.is_directory())
            {
                std::string file_name = entry.path().filename();
                servers.push_back(Glib::ustring(file_name));
            }
        }

        return servers;
    }

    const std::string find_val_in_file_by_prop(const std::string &file_path, const std::string &property)
    {
        std::ifstream file(file_path);
        std::string line;

        while (std::getline(file, line))
        {
            if (line.find(property) != std::string::npos)
            {
                size_t pos = line.find('=');
                if (pos != std::string::npos)
                {
                    file.close();
                    return line.substr(pos + 1);
                }
            }
        }

        file.close();

        // Not found
        std::cout << property << " has not been found in the properties file (" << file_path << ")" << "\n";

        return "NOTHING FOUND";
    }

    void rewrite_property(const std::string &file_path, const std::string &property, const std::string &new_value)
    {
        std::ifstream infile(file_path);
        if (!infile.is_open())
            throw std::runtime_error("File is not open");

        std::string line;
        std::vector<std::string> lines;
        bool found = false;
        while (getline(infile, line))
        {
            if (line.find('=') == property.length() && line.substr(0, property.length()) == property)
            {
                line = property + "=" + new_value;
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
        if (!outfile.is_open())
            throw std::runtime_error("File is not open");

        for (const std::string &line : lines)
        {
            outfile << line << "\n";
        }

        outfile.close();
    }
}