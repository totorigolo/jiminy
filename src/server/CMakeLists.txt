cmake_minimum_required(VERSION 3.6)
project(Jiminy)

set(SERVER_SRC
        main.cpp

        Simulation.hpp
        Simulation.cpp

        Entity.hpp
        Entity.cpp

        Brain.hpp
        Brain.cpp
        Brain_DiscretQLearning.hpp
        Brain_DiscretQLearning.cpp
        Brain_ContinuousQLearning.hpp
        Brain_ContinuousQLearning.cpp

        Limb.hpp
        Limb.cpp

        DebugDraw.hpp
        DebugDraw.cpp

        MouseJointCallback.hpp
        MouseJointCallback.cpp

        sf2b2.hpp
        utils.hpp

        PARENT_SCOPE
)
