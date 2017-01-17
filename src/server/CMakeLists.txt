cmake_minimum_required(VERSION 3.6)
project(Jiminy)

set(SERVER_SRC
        main.cpp

        Simulation.h
        Simulation.cpp

        Entity.h
        Entity.cpp

        Limb.h
        Limb.cpp

        DebugDraw.h
        DebugDraw.cpp

        sf2b2.hpp

        PARENT_SCOPE
)
