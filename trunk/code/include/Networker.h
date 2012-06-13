#pragma once
#include <SFML/Network.hpp>
#include "EntityManager.h"
#include "Player.h"

//DATA PACKING TYPES - SAME FOR SERVER
const int PLAYER = 1;
const int ENTITY = 2;
const int CHAT = 3;
const int CONNECT = 4;
const int DISCONNECT = 5;
const int SERVERMSG = 6;

class Player;

class Networker
{
public:



    Networker(std::string playerName);
    void Update(sf::Time elapsed);
    bool isConnected(){return connected;};
    void Disconnect();

    void SetupUpdater(Player* player, EntityManager* ent){clientPlayer = player; entManager = ent;};
    ~Networker();

    sf::Packet appendSendPacket;    //Append Random Whatever at the end of a default packet

private:
    sf::IpAddress   serverIP;
    sf::IpAddress   clientIP;
    sf::UdpSocket   socket;
    sf::Packet      sendPacket;
    sf::Packet      recvPacket;
    unsigned short  clientPort;


    Player* clientPlayer;
    EntityManager* entManager;

    sf::Time timeSinceUpdate;
    bool connected;
};
