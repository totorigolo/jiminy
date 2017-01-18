#if ! defined ( SERVER_H )
#define SERVER_H

#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>


sf::Packet& operator<<(sf::Packet& packet, sf::RectangleShape& rect);

class Server
{
public :
    
    Server();
    virtual ~Server();
    
    void Listen();

private :
        
    void clientListening(sf::TcpSocket& client);
    
    
    
    
    
    //Attributs
    int nbSocketCree=0;
    sf::TcpListener listener;
    std::map<int, sf::TcpSocket> client;
};
#endif
