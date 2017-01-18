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

#include "Simulation.hpp"
#include "DebugTools.hpp"
#include "DebugDraw.hpp"
#include "Entity.hpp"
#include "Limb.hpp"
#include "sf2b2.hpp"
#include "MouseJointCallback.hpp"


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

void createB2Circle(b2World &world, const b2Vec2 &position, float radius)
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position = position;
    b2CircleShape boxShape;
    boxShape.m_radius = radius;
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &boxShape;
    fixtureDef.density = 2.0f;
    fixtureDef.friction = 0.3f;

    world.CreateBody(&bodyDef)->CreateFixture(&fixtureDef);
}

void Simulation::simulate()
{
    REPORT_BEGIN

    sf::RenderWindow window(sf::VideoMode(800, 600), "Window");
    window.setFramerateLimit(60); // TODO: Better framerate control
    sf::View view(window.getDefaultView());
    view.zoom(10.f);
    window.setView(view);
    float currentZoom{10.f};

    // Create things
    b2Vec2 gravity(0.0f, -10.f);
    b2World world(gravity);

    // The entities
    std::shared_ptr<Entity> being{std::make_shared<Entity>()};
    std::shared_ptr<Entity> worldEntity{std::make_shared<Entity>()};

    // Ground
    b2Body *ground{};
    {
        b2BodyDef groundBodyDef;
        groundBodyDef.position.Set(0.0f, -40.f);
        b2PolygonShape groundBox;
        groundBox.SetAsBox(45.0f, 0.01f);
        b2FixtureDef groundFixtureDef;
        groundFixtureDef.shape = &groundBox;
        groundFixtureDef.friction = 1.5f;
        groundFixtureDef.restitution = 0.1f;
        ground = worldEntity->CreateLimb(groundBodyDef, groundFixtureDef, &world)->mB2Body;

        groundBodyDef.position.Set(-40.0f, 0.f);
        groundBox.SetAsBox(0.01f, 40.0f);
        worldEntity->CreateLimb(groundBodyDef, groundFixtureDef, &world);

        groundBodyDef.position.Set(40.0f, 0.f);
        groundBox.SetAsBox(0.01f, 40.0f);
        worldEntity->CreateLimb(groundBodyDef, groundFixtureDef, &world);
    }

    // Being
    {
        // The base
        b2BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody;
        bodyDef.position.Set(4.0f, 1.0f);
        bodyDef.fixedRotation = true;
        b2PolygonShape boxShape;
        boxShape.SetAsBox(3.f, 1.f);
        b2FixtureDef fixtureDef;
        fixtureDef.shape = &boxShape;
        fixtureDef.density = 2.0f;
        fixtureDef.friction = 0.9f;

        std::shared_ptr<Limb> base{being->CreateLimb(bodyDef, fixtureDef, &world)};

        // The arm
        bodyDef.position.Set(4.0f, 0.f);
        bodyDef.angle = 0.1f;
        bodyDef.fixedRotation = false;
        boxShape.SetAsBox(0.7f, 5.0f);
        fixtureDef.density = 1.0f;

        std::shared_ptr<Limb> arm{being->CreateLimb(bodyDef, fixtureDef, &world)};

        // Join them
        b2RevoluteJointDef jointDef;
        jointDef.bodyA = base->mB2Body;
        jointDef.bodyB = arm->mB2Body;
        jointDef.localAnchorA = b2Vec2(0.f, 0.4f);
        jointDef.localAnchorB = b2Vec2(0.f, -4.4f);
        jointDef.collideConnected = false;
        jointDef.enableMotor = true;
        b2RevoluteJoint *joint = (b2RevoluteJoint *) world.CreateJoint(&jointDef);

        // Register the available actions and info to the brain
        being->mBrain.mActions["x."] = [&base](float force)
        {
            base->mB2Body->ApplyLinearImpulse(b2Vec2(force, 0), b2Vec2(0, 0), true);
        };
        being->mBrain.mActions["theta."] = [&joint](float speed)
        {
            joint->SetMotorSpeed(speed);
        };
        being->mBrain.mInfo["theta"] = [joint]()
        {
            return joint->GetJointAngle();
        };
        being->mBrain.mInfo["theta."] = [joint]() -> float
        {
            return joint->GetJointSpeed();
        };
    }

    // Debug draw
    DebugDraw debugDraw(&world);
    debugDraw.SetRenderTarget(&window);
    world.SetDebugDraw(&debugDraw);

    // Loop
    b2MouseJoint *mouseJoint{};
    bool mouseJointCreated{false};
    sf::Vector2i prevMousePos{};
    while (isSimulating() && window.isOpen())
    {
        // Events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Exit
            if (event.type == sf::Event::Closed ||
                (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
                window.close();

            if (!window.hasFocus()) continue;

            // Zoom
            if (event.type == sf::Event::MouseWheelScrolled)
            {
                constexpr float zoomFactor = 1.2f;
                currentZoom *= (event.mouseWheelScroll.delta < 0) ? zoomFactor : 1.f / zoomFactor;
                view.zoom((event.mouseWheelScroll.delta < 0) ? zoomFactor : 1.f / zoomFactor);
                window.setView(view);
            }
            else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R)
            {
                view = window.getDefaultView();
                currentZoom = 1.f;
                window.setView(view);
            }

            // Random body creation
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::C)
            {
                createB2Circle(world, convert(window.mapPixelToCoords(sf::Mouse::getPosition(window), view)), 2.f);
            }

            // Tests
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::D)
            {
                being.reset();
            }
            else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::F)
            {
                worldEntity.reset();
            }
        }
        if (window.hasFocus() && sf::Mouse::isButtonPressed(sf::Mouse::Middle))
        {
            view.move(sf::Vector2f(prevMousePos - sf::Mouse::getPosition(window)) * currentZoom);
            window.setView(view);
        }
        if (window.hasFocus())
        {
            if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) // Create or update the mouse joint
            {
                b2Vec2 b2MousePos{convert(window.mapPixelToCoords(sf::Mouse::getPosition(window)))};

                // If the join already exists, update the target position
                if (mouseJointCreated)
                {
                    if (mouseJoint)
                    {
                        mouseJoint->SetTarget(b2MousePos);
                    }
                    else
                    {
                        mouseJointCreated = false;
                    }
                }
                else // Search which body is under the mouse right now
                {
                    MouseJointCallback callback(b2MousePos);
                    world.QueryAABB(&callback, callback.GetAABB());

                    if (callback.GetFixture()) // Found something
                    {
                        b2Body *body = callback.GetFixture()->GetBody();
                        b2MouseJointDef jointDef;
                        jointDef.bodyA = ground; // Le body A ne sert pas, mais doit exister
                        jointDef.bodyB = body; // Le body utilisé est le B
                        jointDef.target = b2MousePos;
                        jointDef.collideConnected = true;
                        jointDef.maxForce = 100000000.f * body->GetMass();
                        mouseJoint = (b2MouseJoint *) world.CreateJoint(&jointDef);
                        mouseJointCreated = true;
                    }
                }
            }
            else if (mouseJointCreated) // Destroy the join
            {
                mouseJointCreated = false;
                if (mouseJoint)
                {
                    world.DestroyJoint(mouseJoint);
                }
            }
        }
        prevMousePos = sf::Mouse::getPosition(window);

        // Physics
        float32 timeStep = 1.0f / 60.0f;
        int32 velocityIterations = 6;
        int32 positionIterations = 2;
        world.Step(timeStep, velocityIterations, positionIterations);

        being->Think();

        // Display
        window.clear(sf::Color::Black);

        world.DrawDebugData();

        window.display();
    }

    mSimulating = false;
//    exit(0);

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

        if (action == "quit" || action == "q")
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
