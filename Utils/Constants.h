#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <iostream>

namespace MCSM 
{
    const std::string SERVER_FOLDER        = "/home/potatocorp/Documents/Perso/Useful/mcServManager++/Java";
    const std::string PROPERTIES_FILE_NAME = "server.properties";
    
    const std::string WORLD_NAME_PARAMETER    = "level-name";
    const std::string DESCRIPTION_PARAMETER   = "motd";
    const std::string PORT_PARAMETER          = "server-port";
    const std::string MAX_PLAYERS_PARAMETER   = "max-players";
    const std::string VIEW_DISTANCE_PARAMETER = "view-distance";
    const std::string GAMEMODE_PARAMETER      = "gamemode";
    const std::string DIFFICULTY_PARAMETER    = "difficulty";
    const std::string HARDCORE_PARAMETER      = "hardcore";
    const std::string PVP_PARAMETER           = "pvp";
    const std::string FLY_PARAMETER           = "allow-flight";
    const std::string NETHER_PARAMETER        = "allow-nether";
    const std::string WHITELIST_PARAMETER     = "white-list";

}

#endif // CONSTANTS_H