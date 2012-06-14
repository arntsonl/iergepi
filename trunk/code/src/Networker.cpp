#include "Networker.h"

Networker::Networker(std::string playerName){
    timeSinceUpdate = sf::Time::Zero;
    connected = false;
    //srand(playerName.at(0)+(playerName.at(1)));
    clientPort = (unsigned short)(50000 +(rand()%5000));

    serverIP = sf::IpAddress("184.82.23.104");
    //serverIP = sf::IpAddress("127.0.0.1");
    socket.bind(clientPort);
    //socket.setBlocking(true); //Blocking for now (at least on connect) -RL

    timeSince.restart();
    clientIP = sf::IpAddress::getPublicAddress();

    //Build connect packet
    sendPacket.clear();
    sendPacket << CONNECT << playerName;
    socket.send(sendPacket, serverIP, 51515);

    //Wait for Reception
    sf::IpAddress serverPacketIP;
    unsigned short portPacket;
    socket.receive(recvPacket, serverPacketIP, portPacket);
    int pType;
    recvPacket>>pType;

    if(pType == CONNECT)
    {
        connected = true;
        socket.setBlocking(false);
    }
    else if(pType == DISCONNECT)
    {
        connected = false;
        string message;
        recvPacket>>message;
        cout<<"CONNECTION ERROR: "<<message<<endl;

            sendPacket.clear();
            sendPacket << DISCONNECT << playerName;
            socket.send(sendPacket, serverIP, 51515);
    }
}

void Networker::Update(sf::Time elapsed)
{
    timeSinceUpdate += elapsed;

    //Receive Packet to Update
    sf::IpAddress serverPacketIP;
    unsigned short portPacket;
    if(sf::Socket::Done == socket.receive(recvPacket, serverPacketIP, portPacket))
    {

        timeSinceUpdate = sf::Time::Zero;
        //break down recvPacket

        while(!recvPacket.endOfPacket())
        {
            int packetType;
            recvPacket>>packetType;

            if(packetType == PLAYER)    //Deal with player
            {
                string pName;
                float x;
                float y;
                float z;
                float angle;


                if(recvPacket>>pName>>x>>y>>z>>angle)
                {
                    if(pName.compare(clientPlayer->getName()) != 0) //Check to see if the player is us
                    {
                        if(!entManager->HasPlayer(pName))
                        {
                            entManager->AddPlayer(pName, new Player(pName, sf::Vector2f(x,y), "resources/Charsheet.png",  false));
                        }

                        ( (Player*) entManager->GetPlayer(pName) )->SetNetworkedValues(sf::Vector3f(x,y,z), angle);

                    }

                }
                else
                {
                    #ifdef DEBUG
                    cout<<"Error Receiving Player Packet from "<<serverPacketIP.toString()<<endl;
                    #endif
                }
            }
        }

    }

    //Build Player Update to Send to Server
    if((clientPlayer->isDirty() && timeSince.getElapsedTime().asMicroseconds()>40000) || timeSince.getElapsedTime().asSeconds() > 2.5f)
    {
        clientPlayer->setDirty(false);
        sendPacket.clear();
        sendPacket<<PLAYER<<clientPlayer->getName()<<clientPlayer->isAlive()<<clientPlayer->getPosition().x<<clientPlayer->getPosition().y<<clientPlayer->getPosition().z<<clientPlayer->GetAngle();
        socket.send(sendPacket,serverIP, 51515);
        timeSince.restart();
    }

}

void Networker::Disconnect()
{
    //Build connect packet
    sendPacket.clear();
    sendPacket << DISCONNECT;
    socket.send(sendPacket, serverIP, 51515);
}

Networker::~Networker()
{
}
