#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include "Server.h"
// #include "Shape.h"
#include <thread> 
#include <map>
#include <boost/bind.hpp>


Server::Server(){}
Server::~Server(){}
void Server::Listen()
{
    // lie l'écouteur à un port
    if (listener.listen(5001) != sf::Socket::Done)
    {
        std::cerr << "Erreur 1" << std::endl;
    }

    while(true)
    {
        // accepte une nouvelle connexion
        if (listener.accept(client[nbSocketCree]) != sf::Socket::Done)
        {
            std::cerr << "Erreur 2 (main)" << std::endl;
            continue;
        }
        std::thread threadSocket(boost::bind(&Server::clientListening, this, std::ref(client[nbSocketCree])));
        threadSocket.detach();//parceque join() c'est pour les faibles ;)
        nbSocketCree++;
    }
    std::cerr << "Vous êtes sorti d'un while(true), bravo !" << std::endl;
}




void Server::clientListening(sf::TcpSocket& client)
{
    sf::RectangleShape rectangle(sf::Vector2f(120, 50));
    sf::Packet packetReception;
    sf::Packet packetEnvoie;
    std::string str("");
    
    while(true)//À modifier, tant que client existe
    {
        if (client.receive(packetReception) != sf::Socket::Done)
        {
            std::cerr << "Erreur réception rectangle !" << std::endl;
            break;
        }
        packetReception >> str;

        packetEnvoie << rectangle;
        if (client.send(packetEnvoie) != sf::Socket::Done){
            std::cerr << "Erreur transmission rectangle !" << std::endl;
        }
    }
}


sf::Packet& operator<<(sf::Packet& packet, sf::RectangleShape& rect)
    {
        return packet << (int)rect.getSize().x << (int)rect.getSize().y;
    }






