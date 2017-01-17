cmake_minimum_required(VERSION 3.6)
project(Jiminy)

set(SERVER_SRC
        main.cpp

        Simulation.hpp
        Simulation.cpp

        Entity.hpp
        Entity.cpp

        Limb.hpp
        Limb.cpp

        DebugDraw.hpp
        DebugDraw.cpp

        sf2b2.hpp

        PARENT_SCOPE
)
