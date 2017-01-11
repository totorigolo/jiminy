#if defined ( SERVER_H )
#define SERVER_H

#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>




sf::Packet& operator <<(sf::Packet& packet, const sf::RectangleShape& rect)
{
    return packet << rect;
}

sf::Packet& operator <<(sf::Packet& packet, sf::RectangleShape& rect)
{
    return packet << rect;
}


#endif
