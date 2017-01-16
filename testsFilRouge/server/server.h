#if defined ( SERVER_H )
#define SERVER_H

#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>

// friend class boost::serialization::access;
// 
// template<class Archive>
// inline void serialize(Archive & ar, my_class & t, const unsigned int file_version{
//     
// }


sf::Packet& operator <<(sf::Packet& packet, sf::RectangleShape& rect)
{
    return packet << rect;
}

void clientListening(sf::TcpSocket& client);

#endif
