#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <iostream>

namespace MCSM 
{
    const std::string SERVER_FOLDER        = "/home/potatocorp/Documents/Perso/Useful/mcServManager++/Java";
    const std::string PROPERTIES_FILE_NAME = "server.properties";
    
    const std::string WORLD_NAME_PROPERTY    = "level-name";
    const std::string DESCRIPTION_PROPERTY   = "motd";
    const std::string PORT_PROPERTY          = "server-port";
    const std::string MAX_PLAYERS_PROPERTY   = "max-players";
    const std::string VIEW_DISTANCE_PROPERTY = "view-distance";
    const std::string GAMEMODE_PROPERTY      = "gamemode";
    const std::string DIFFICULTY_PROPERTY    = "difficulty";
    const std::string HARDCORE_PROPERTY      = "hardcore";
    const std::string PVP_PROPERTY           = "pvp";
    const std::string FLY_PROPERTY           = "allow-flight";
    const std::string NETHER_PROPERTY        = "allow-nether";
    const std::string WHITELIST_PROPERTY     = "white-list";

}

#endif // CONSTANTS_H