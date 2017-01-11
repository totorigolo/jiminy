#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include "server.h"


sf::Packet& operator <<(sf::Packet& packet, sf::RectangleShape& rect)
{
    return packet << (int)rect.getSize().x << (int)rect.getSize().y;
    
}

int main()
{
    
    sf::RectangleShape rectangle(sf::Vector2f(120, 50));
    sf::TcpListener listener;

    // lie l'écouteur à un port
    if (listener.listen(5001) != sf::Socket::Done)
    {
        std::cerr << "Erreur 1" << std::endl;
    }

    // accepte une nouvelle connexion
    sf::TcpSocket client;
    if (listener.accept(client) != sf::Socket::Done)
    {
        std::cerr << "Erreur 2" << std::endl;
    }
    
    sf::Packet packetReception;
    sf::Packet packetEnvoie;
    std::string str("");
    
    while(true)
    {
        if (client.receive(packetReception) != sf::Socket::Done)
        {
            break;
        }
        packetReception >> str;
        std::cout << str << std::endl;
        packetEnvoie << rectangle;
        if (client.send(packetEnvoie) != sf::Socket::Done){
            std::cerr << "Erreur transmission rectangle" << std::endl;
        }
    }
    return 0;
}
// utilisez la socket "client" pour communiquer avec le client connecté,
// et continuez à attendre de nouvelles connexions avec l'écouteur
