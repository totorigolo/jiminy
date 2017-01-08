cmake_minimum_required(VERSION 3.6)
project(Jiminy)

set(SERVER_SRC
        main.cpp

        Simulation.h
        Simulation.cpp
        Commander.h
        Commander.cpp
        NetServer.h
        NetServer.cpp

        World.h
        World.cpp
        SpacePartitioner.h
        SpacePartitioner.cpp

        Animal.h
        Animal.cpp
        Limb.h
        Limb.cpp
        Brain.h
        Brain.cpp

        PhysicalManager.h
        PhysicalManager.cpp
        PhysicalOwner.h
        PhysicalOwner.cpp

        Physical3DPrimitive.h
        Physical3DPrimitive.cpp
        PhysicalCuboid.h
        PhysicalCuboid.cpp
        PhysicalSphere.h
        PhysicalSphere.cpp
        PhysicalStaticPlane.h
        PhysicalStaticPlane.cpp
        Constraint.h
        Constraint.cpp

        Drawable.h
        Entity.h
        Entity.cpp
)

if (DEBUG_DRAW_SERVER MATCHES OFF)
    set(SERVER_SRC
            ${SERVER_SRC}

            PARENT_SCOPE
    )
else ()
    set(SERVER_SRC
            ${SERVER_SRC}
            DebugDrawer.h
            DebugDrawer.cpp
            PARENT_SCOPE
    )
endif()