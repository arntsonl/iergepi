#include <windows.h>
#include <cstdio>
#include <iostream>
#include <SFML/Network.hpp>
#include <deque>
#include <vector>

using namespace std;

#define LOG_LENGTH (15)

const int PLAYER = 1;
const int ENTITY = 2;
const int CHAT = 3;
const int CONNECT = 4;
const int DISCONNECT = 5;
const int SERVERMSG = 6;

deque<string> log;
sf::Clock timer;
sf::Clock packetDelayer;
bool dataChanged;

//Networking Goods
    sf::IpAddress   serverIP;
    sf::UdpSocket   udpSocket;
    sf::Packet      sendPacket;
    sf::Packet      recvPacket;

//SERVER SWEETNESS

    int numPlayers;

    struct PlayerNetData
    {
        string playerName;
        sf::IpAddress IP;
        unsigned short port;
        bool isAlive;
        bool isDirty;
        float x;
        float y;
        float z;
        float angle;
    };

    struct Connection
    {
        sf::IpAddress IP;
        unsigned short port;
        sf::Clock timeSinceUpdate;
    };

    //Organizational Data Structures
    vector<std::string>                                     nameList;
    map<std::string, Connection>                            nameMap;
    map<sf::IpAddress, map<unsigned short, PlayerNetData> >   playerData;

void ClearScreen(){
  HANDLE                     hStdOut;
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  DWORD                      count;
  DWORD                      cellCount;
  COORD                      homeCoords = { 0, 0 };

  hStdOut = GetStdHandle( STD_OUTPUT_HANDLE );

  if(dataChanged){
      if (hStdOut == INVALID_HANDLE_VALUE) return;

      /* Get the number of cells in the current buffer */
      if (!GetConsoleScreenBufferInfo( hStdOut, &csbi )) return;
      cellCount = csbi.dwSize.X *csbi.dwSize.Y;

        /* Fill the entire buffer with spaces */
      if (!FillConsoleOutputCharacter(
        hStdOut,
        (TCHAR) ' ',
        cellCount,
        homeCoords,
        &count
        )) return;

      /* Fill the entire buffer with the current colors and attributes */
      if (!FillConsoleOutputAttribute(
        hStdOut,
        csbi.wAttributes,
        cellCount,
        homeCoords,
        &count
        )) return;

        dataChanged = false;
  }
  /* Move the cursor home */
  SetConsoleCursorPosition( hStdOut, homeCoords );
  }

void drawHeader(){
ClearScreen();
printf("              C@,@                \n");
printf("             @    @@@             \n");
printf("           C@.       @            \n");
printf("        @@@           ;           \n");
printf("      L@   ,@@@@ l    @           \n");
printf("     i@   @@@@@@  @               \n");
printf("     @C .@@;t@, @    ,,           \n");
printf("     @    @@@@  G,   @@           \n");
printf("     @L  , .f@   @   @@@          \n");
printf("     :@   @@@@   @@G@@@C          \n");
printf("      @@     ;@  C@t@@CG          \n");
printf("       @@  tG@@   @@:L@L          \n");
printf("        f@     @  .@C@:           \n");
printf("          @@    @  L@@            \n");
printf("           ;@    @  @,            \n");
printf("             @   f@: @            \n");
printf("             .f   f . t .         \n");
printf("           . .:G  f i@f..         \n");
printf("         .     @   @   @          \n");
printf("               @   @   @          \n");
printf("     @@@@@@t   :   L@@@@          \n");
printf("   @@      G@@@@   @   :@@        \n");
printf("  @@           @   @   t  @@      \n");
printf("  @      ,    :@   @@Ct@    @:    \n");
printf(" @@       @@@@C    @   C     @    \n");
printf(" @G               @    @     @@   \n");
printf(" @ :             @@ ,G@      G@   \n");
printf(" @C@:        ,@@@@@@@@@      @@   \n");
printf("=================================\n");
printf("=|  D  E  T  H  S  N  4  K  E  |=\n");
printf("=|                             |=\n");
printf("=|   RL SERVING TECHNOLOGIES   |=\n");
printf("=================================\n");
}

void drawLog(){
    for (int i=log.size()-1; i >= 0; i--) cout<<log[i]<<endl;
    cout<<endl;
}

void Log(string message){
    char datime[128];
    sprintf(datime,"%.5f: ",timer.getElapsedTime().asSeconds());
    log.push_front(datime+message);
    log.pop_back();
    dataChanged = true;
}

void drawStaticLine()
{
    cout<<"Server IP: "<<serverIP.toString()<<" | Port: "<<udpSocket.getLocalPort()<<" |  Players: "<<numPlayers<<" | "<<timer.getElapsedTime().asSeconds()<< "\n";
}

void drawPlayerInfo(){
    for(int i =0; i <nameList.size(); i++)
    {
        PlayerNetData p =  playerData[ nameMap[ nameList[i] ].IP][ nameMap[ nameList[i] ].port];
        cout<<p.playerName<<" "<< p.IP.toInteger() << " " << p.port << " " << ((p.isAlive)?"ALIVE":"DEAD") << " | x: " << p.x << " y: " << p.y << " z: " << p.z << " | angle: " << p.angle <<"\t\n";
    }
}


int main(int argc, char * argv[])
{
    dataChanged = true;
    numPlayers = 0;
    timer.restart();
    for(int i =0; i < LOG_LENGTH; i++) log.push_front(" ");

    Log("Starting Server on "+sf::IpAddress::getLocalAddress().toString()+":51515");

    Log("Binding to Port 51515");
    udpSocket.bind(51515);
    udpSocket.setBlocking(false);


    Log("Getting Public IP");
    serverIP = sf::IpAddress::getPublicAddress();
    Log("Public IP: "+serverIP.toString());

    while(1)
    {

            numPlayers = nameList.size();

            drawHeader();
            drawLog();
            drawStaticLine();
            drawPlayerInfo();

//RECEIVE
            sf::IpAddress recvPacketIP;
            unsigned short portPacket;
            if(sf::Socket::Done == udpSocket.receive(recvPacket, recvPacketIP, portPacket) && portPacket != 51515)
            {

                int pType;
                recvPacket>>pType;

                if(pType == CONNECT)
                {
                    string pName;
                    if(recvPacket>>pName)
                    {

                        //Do Some Setup
                        if(nameMap.find(pName) == nameMap.end())
                        {
                            if(((playerData.find(recvPacketIP) != playerData.end()) &&
                               (playerData[recvPacketIP].find(portPacket) == playerData[recvPacketIP].end()))||playerData.find(recvPacketIP) == playerData.end())
                            {
                                    PlayerNetData temp;
                                    temp.IP = recvPacketIP;
                                    temp.port = portPacket;
                                    temp.playerName = pName;
                                    temp.isAlive = false;
                                    temp.isDirty = true;
                                    temp.x = 0;
                                    temp.y = -100;
                                    temp.z = 0;
                                    temp.angle = 0;

                                    playerData[recvPacketIP][portPacket] = temp;

                                   // Connection t;
                                   // t.IP = recvPacketIP;
                                   // t.port = portPacket;
                                   // connections.push_back(t);

                                    nameMap[pName].IP = recvPacketIP;
                                    nameMap[pName].port = portPacket;
                                    nameMap[pName].timeSinceUpdate.restart();

                                    nameList.push_back(pName);

                                    //Confirm Connect
                                    sendPacket.clear();
                                    sendPacket<<CONNECT;
                                    udpSocket.send(sendPacket,recvPacketIP, portPacket);
                                    Log(pName+" connected from "+recvPacketIP.toString());
                            }
                            else{
                                sendPacket.clear();
                                sendPacket<<DISCONNECT;
                                sendPacket<<"IP+PORT already taken";
                                udpSocket.send(sendPacket,recvPacketIP, portPacket);
                                Log(pName+" tried connecting from "+recvPacketIP.toString()+", IP+PORT already taken");
                            }
                        }
                        else{
                            sendPacket.clear();
                            sendPacket<<DISCONNECT;
                            sendPacket<<"Name already taken";
                            udpSocket.send(sendPacket,recvPacketIP, portPacket);
                            Log(pName+" tried connecting from "+recvPacketIP.toString()+", NAME already taken");
                        }
                    }


                }
                else if(pType == PLAYER)
                {
                    recvPacket
                    >> playerData[recvPacketIP][portPacket].playerName
                    >> playerData[recvPacketIP][portPacket].isAlive
                    >> playerData[recvPacketIP][portPacket].x
                    >> playerData[recvPacketIP][portPacket].y
                    >> playerData[recvPacketIP][portPacket].z
                    >> playerData[recvPacketIP][portPacket].angle;
                    playerData[recvPacketIP][portPacket].isDirty = true;

                    nameMap[playerData[recvPacketIP][portPacket].playerName].timeSinceUpdate.restart();

                }
                else if(pType == DISCONNECT)
                {
                    Log(playerData[recvPacketIP][portPacket].playerName + " Disconnecting...");

                    vector<std::string>::iterator vItr = nameList.begin();
                    while ( vItr != nameList.end() )
                    {
                        if ( vItr->compare(playerData[recvPacketIP][portPacket].playerName)==0)
                        {
                            vItr = nameList.erase(vItr); // Will return next valid iterator
                            break;
                        }
                        else
                            vItr++;
                    }

                    nameMap.erase(playerData[recvPacketIP][portPacket].playerName);
                    playerData[recvPacketIP].erase(portPacket);
                    if(playerData[recvPacketIP].empty()) playerData.erase(recvPacketIP);

                    dataChanged = true;
                }
            }

//SEND
            if(packetDelayer.getElapsedTime().asMicroseconds() > 40000) // 25fps
            {
                packetDelayer.restart();
                sendPacket.clear();
                for(int i = 0; i < nameList.size(); i++) //Add all connected players to Update Packet
                {
                    Connection c = nameMap[ nameList[i] ];
                    PlayerNetData t = playerData[c.IP][c.port];
                    if(t.isDirty)
                    {
                        sendPacket
                        << PLAYER
                        << t.playerName
                        << t.x
                        << t.y
                        << t.z
                        << t.angle;
                        t.isDirty = false;
                    }
                }
                if(sendPacket.getData()!=NULL)
                {
                    for(int i = 0; i < nameList.size(); i++) //now send to everyone
                    {
                        Connection c = nameMap[ nameList[i] ];
                        udpSocket.send(sendPacket, c.IP, c.port);
                    }
                }
            }

//Utilities

            //Enforce Timeouts
                std::string timedOut;
                for(int i =0; i <nameList.size(); i++)
                {
                    if(nameMap[ nameList[i] ].timeSinceUpdate.getElapsedTime().asSeconds() > 5)
                    {
                        Log(nameList[i] + " Timed Out");
                        timedOut = nameList[i];
                    }
                }

                if(timedOut.length()>0)
                {
                    playerData[nameMap[timedOut].IP].erase(nameMap[timedOut].port);
                    if(playerData[nameMap[timedOut].IP].empty()) playerData.erase(nameMap[timedOut].IP);
                    nameMap.erase(timedOut);


                    vector<std::string>::iterator vItr = nameList.begin();
                    while ( vItr != nameList.end() )
                    {
                        if ( vItr->compare(timedOut)==0)
                        {
                            vItr = nameList.erase(vItr); // Will return next valid iterator
                            break;
                        }
                        else
                            vItr++;
                    }
                }
    } // end while(1)
return 0;
}


