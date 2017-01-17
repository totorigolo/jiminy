// This file is part of Jiminy.
// 
// Jiminy is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// Jiminy is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with Jiminy.  If not, see <http://www.gnu.org/licenses/>.


#include <string>
#include <sstream>
#include <chrono>
#include <thread>
#include <iostream>

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

#include "Simulation.h"
#include "DebugTools.h"
#include "Limb.h"
#include "DebugDraw.h"


using namespace std::chrono_literals;


Simulation::Simulation()
        : mSimulating{false},
          mSimulationThread{}, mCommandLineThread{}
{
    REPORT_BEGIN

    REPORT_END
}

Simulation::~Simulation()
{
    REPORT_BEGIN

    mSimulating = false;

    if (mSimulationThread && mSimulationThread->joinable())
        mSimulationThread->join();
    if (mCommandLineThread && mCommandLineThread->joinable())
        mCommandLineThread->join();

    REPORT_END
}

int Simulation::run()
{
    REPORT_BEGIN

    mSimulating = true;

    mSimulationThread = std::make_unique<std::thread>(&Simulation::simulate, this);
    mCommandLineThread = std::make_unique<std::thread>(&Simulation::listenCommandLine, this);

    if (mSimulationThread->joinable())
        mSimulationThread->join();
    if (mCommandLineThread->joinable())
        mCommandLineThread->join();

    REPORT_END
    return 0;
}

bool Simulation::isSimulating() const
{
    REPORT_BEGIN_EXTRA

    REPORT_END
    return mSimulating;
}

void Simulation::simulate()
{
    REPORT_BEGIN

    sf::RenderWindow window(sf::VideoMode(800, 600), "Window");
    window.setFramerateLimit(30); // TODO: Better framerate control
    sf::View view(window.getDefaultView());
    window.setView(view);
    float currentZoom{1.f};

    // Create things
    b2Vec2 gravity(0.0f, 10.f);
    b2World world(gravity);

    // Ground
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(4.0f, 8.f);
    b2Body *groundBody = world.CreateBody(&groundBodyDef);
    b2PolygonShape groundBox;
    groundBox.SetAsBox(6.0f, 0.02f);
    b2FixtureDef groundFixtureDef;
    groundFixtureDef.shape = &groundBox;

    // Circle
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(4.0f, 0.0f);
    b2CircleShape dynamicCircle;
    dynamicCircle.m_p.Set(1.0f, 1.0f);
    dynamicCircle.m_radius = 0.4f;
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicCircle;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;

    // Create limbs
    Limb ground{groundBodyDef, groundFixtureDef, world};
    Limb head{bodyDef, fixtureDef, world};

    DebugDraw debugDraw(&world);
    debugDraw.SetRenderTarget(&window);
    world.SetDebugDraw(&debugDraw);

    // Loop
    sf::Vector2i prevMousePos{};
    while (isSimulating() && window.isOpen())
    {
        // Events
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseWheelScrolled)
            {
                constexpr float zoomFactor = 1.2f;
                currentZoom *= (event.mouseWheelScroll.delta < 0) ? zoomFactor : 1.f / zoomFactor;
                view.zoom((event.mouseWheelScroll.delta < 0) ? zoomFactor : 1.f / zoomFactor);
                window.setView(view);
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R)
            {
                view = window.getDefaultView();
                currentZoom = 1.f;
                window.setView(view);
            }
        }
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            view.move(sf::Vector2f(prevMousePos - sf::Mouse::getPosition(window)) * currentZoom);
            window.setView(view);
        }
        prevMousePos = sf::Mouse::getPosition(window);

        // Physics
        float32 timeStep = 1.0f / 60.0f;
        int32 velocityIterations = 6;
        int32 positionIterations = 2;
        world.Step(timeStep, velocityIterations, positionIterations);

        // Display
        window.clear(sf::Color::Black);

        window.draw(ground);
        window.draw(head);

        world.DrawDebugData();

        window.display();
    }

    mSimulating = false;

    REPORT_END
}

void Simulation::listenCommandLine()
{
    REPORT_BEGIN

    std::string line, action;
    while (isSimulating())
    {
        std::getline(std::cin, line);
        std::stringstream ss;
        ss << line;
        ss >> action;

        if (action == "quit")
        {
            std::cout << "Bye !" << std::endl;
            mSimulating = false;
        }
        else if (action == "help")
        {
            std::cout << "Lol, find by yourself. But don't, DON'T, ask Thomas." << std::endl;
        }
        else if (action == "debug_draw" || action == "dd")
        {
            std::string param;
            ss >> param;
            if (param == "on" || param == "1")
            {
//                PhysicalManager::Instance().EnableDebugDraw();
            }
            else if (param == "off" || param == "0")
            {
//                PhysicalManager::Instance().DisableDebugDraw();
            }
            else
            {
                std::cout << "Parameter \"" << param << "\" unknown. Choose among: on, off or 0, 1." << std::endl;
            }
        }
        else
        {
            std::cout << "Unknown action \"" << action << "\" (from \"" << line << "\")." << std::endl;
            std::cout << "Type \"help\" to know the available commands." << std::endl;
        }
    }

    REPORT_END
}
