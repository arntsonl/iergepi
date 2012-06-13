#include "Networker.h"

Networker::Networker(std::string playerName){
    timeSinceUpdate = sf::Time::Zero;
    connected = false;
    //srand(playerName.at(0)+(playerName.at(1)));
    clientPort = (unsigned short)(50000 +(rand()%5000));

    serverIP = sf::IpAddress("184.82.23.104");
    //serverIP = sf::IpAddress("127.0.0.1");
    socket.bind(clientPort);
    socket.setBlocking(true); //Blocking for now (at least on connect) -RL


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
    }
}

void Networker::Update(sf::Time elapsed)
{
    timeSinceUpdate += elapsed;

    //Receive Packet to Update
    sf::IpAddress serverPacketIP;
    unsigned short portPacket;
    if(socket.receive(recvPacket, serverPacketIP, portPacket))
    {
        cout<<"Received Packet\n";
        timeSinceUpdate = sf::Time::Zero;
        //break down recvPacket
    }

    //Build Player Update to Send to Server
    sendPacket.clear();
    sendPacket<<PLAYER<<clientPlayer->getName()<<clientPlayer->isAlive()<<clientPlayer->getPosition().x<<clientPlayer->getPosition().y<<clientPlayer->getPosition().z<<clientPlayer->GetAngle();
    socket.send(sendPacket,serverIP, 51515);


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
