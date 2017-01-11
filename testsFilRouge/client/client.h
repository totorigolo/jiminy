#if defined ( CLIENT_H )
#define CLIENT_H

#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
// #include <iostream>

    
    
    
sf::Packet& operator<<(sf::Packet& packet, const sf::RectangleShape& shape)
{
    return packet << shape;
}

sf::Packet& operator<<(sf::Packet& packet, sf::RectangleShape& shape)
{
    return packet << shape;
}


sf::Packet& operator>>(sf::Packet& packet, const sf::RectangleShape& shape)
{
    return packet >> shape;
}

sf::Packet& operator>>(sf::Packet& packet, sf::RectangleShape& shape)
{
    return packet >> shape;
}
#endif
