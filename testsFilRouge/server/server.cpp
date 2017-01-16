#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include "server.h"
#include <thread> 

#include <fstream>
#include <boost/archive/text_oarchive.hpp>



sf::Packet& operator <<(sf::Packet& packet, sf::RectangleShape& rect)
{
    return packet << (int)rect.getSize().x << (int)rect.getSize().y;
    
}
void clientListening(sf::TcpSocket& client);

int main()
{
    
    sf::TcpListener listener;

    // lie l'écouteur à un port
    if (listener.listen(5001) != sf::Socket::Done)
    {
        std::cerr << "Erreur 1" << std::endl;
    }

    while(true)
    {
        // accepte une nouvelle connexion
        sf::TcpSocket client;
        if (listener.accept(client) != sf::Socket::Done)
        {
            std::cerr << "Erreur 2" << std::endl;
        }
        std::thread threadSocket(clientListening, std::ref(client));
        threadSocket.detach();//parceque join() c'est pour les faibles ;)
    }
    std::cerr << "Vous êtes sorti d'un while(true), bravo !" << std::endl;
    return 0;
}


void clientListening(sf::TcpSocket& client)
{
    std::cout << "Création nouveau thread !" << std::endl;
    sf::RectangleShape rectangle(sf::Vector2f(120, 50));
    sf::Packet packetReception;
    sf::Packet packetEnvoie;
    std::string str("");
    
    while(true)
    {
        std::cerr << "Erreur 1" << std::endl;
        if (client.receive(packetReception) != sf::Socket::Done)
        {
            std::cerr << "Erreur 2" << std::endl;
            break;
        }
        packetReception >> str;
        std::cout << str << std::endl;
        //--------------------------------------- test serializable
//         std::ofstream ofs("filename");
//         {
//         boost::archive::text_oarchive oa(ofs);
//         oa << rectangle;
//         packetEnvoie << oa;
//         }   
        
        //---------------------------------------
        packetEnvoie << rectangle;
        if (client.send(packetEnvoie) != sf::Socket::Done){
            std::cerr << "Erreur transmission rectangle" << std::endl;
        }
    }
    std::cerr << "Suppression d'un Thread" << std::endl;
}


namespace boost {
    namespace serialization {

        template<class Archive>
        void serialize(Archive & ar, sf::RectangleShape & r, const unsigned int version)
        {
            ar & r;
        }

    } // namespace serialization
} // namespace boost









