#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include "client.h"

sf::Packet& operator >>(sf::Packet& packet, sf::RectangleShape& rect)
{
    int x, y;
    packet >> x >> y;
    rect.setSize(sf::Vector2f(x, y));
    return packet;
}

int main()
{
    int x, y;
    sf::RectangleShape rectangle;
    std::vector<sf::RectangleShape> listRectangle;
    sf::TcpSocket socket;
    sf::Socket::Status status = socket.connect("127.0.0.1", 5001);
    if (status != sf::Socket::Done)
    {
        std::cerr << "Problème de connection client" << std::endl;
    }
    
    sf::Packet packetEnvoie;
    sf::Packet packetReception;
    
    std::string str("Demande nouveau rectangle");
    packetEnvoie << str;
    
    // création de la fenêtre
    sf::RenderWindow window(sf::VideoMode(800, 600), "My window");

    // on fait tourner le programme tant que la fenêtre n'a pas été fermée
    while (window.isOpen())
    {
        // on traite tous les évènements de la fenêtre qui ont été générés depuis la dernière itération de la boucle
        sf::Event event;
        while (window.pollEvent(event))
        {
            // fermeture de la fenêtre lorsque l'utilisateur le souhaite
            if (event.type == sf::Event::Closed)
            {window.close();}
            if (event.type == sf::Event::MouseButtonPressed)
            {
                x=event.mouseButton.x;
                y=event.mouseButton.y;
                socket.send(packetEnvoie);
                if (socket.receive(packetReception) != sf::Socket::Done)
                    {
                        std::cerr << "Erreur réception rectangle" << std::endl;
                    }
                packetReception >> rectangle;
                rectangle.setPosition(x, y);
                listRectangle.push_back(rectangle); 
                
                
            }
        }
        // effacement de la fenêtre en noir
        window.clear(sf::Color::Black);

        // c'est ici qu'on dessine tout
        sf::CircleShape shape(50);

        // change la couleur de la forme pour du vert
        rectangle.setFillColor(sf::Color(100, 250, 50));
        shape.setFillColor(sf::Color(100, 250, 50));
        
        for (std::vector<sf::RectangleShape>::iterator it = listRectangle.begin() ; it != listRectangle.end(); ++it)
        {
            window.draw(*it);
        }
        
        window.draw(shape);
        // fin de la frame courante, affichage de tout ce qu'on a dessiné
        window.display();
    }

    return 0;
}
