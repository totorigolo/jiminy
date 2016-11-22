cmake_minimum_required(VERSION 3.6)
project(Jiminy)

set(CLIENT_SRC
        main.cpp

        MainWindow.h
        MainWindow.cpp

        OpenGLWidget.h
        OpenGLWidget.cpp

        PARENT_SCOPE
)

set(CLIENT_QT_RESOURCES
#        images.qrc # For example

        PARENT_SCOPE
)