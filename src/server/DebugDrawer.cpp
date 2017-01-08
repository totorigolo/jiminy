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


#include <iostream>
#include <GL/glew.h>
#include <GL/glu.h>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>


#include "DebugDrawer.h"
#include "DebugTools.h"


const sf::Vector3f DEFAULT_EYE_POS(10, 20, 10);
const sf::Vector3f DEFAULT_TARGET_POS(0, 0, 0);

DebugDrawer::DebugDrawer()
        : mWindow(nullptr), mEyePos(DEFAULT_EYE_POS), mTargetPos(DEFAULT_TARGET_POS)
{
    REPORT();

//    setDebugMode(0);
    setDebugMode(DBG_MAX_DEBUG_DRAW_MODE);
//    mNextDebugMode = 0;

//    mDebugMode = DBG_MAX_DEBUG_DRAW_MODE;
//    mNextDebugMode = DBG_MAX_DEBUG_DRAW_MODE;
//    createWindow();
}

DebugDrawer::~DebugDrawer()
{
    REPORT();
}

void DebugDrawer::drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &color)
{
    if (mWindow == nullptr || !mWindow->isOpen()) return;
    mWindow->setActive(true); // TODO: Make it RAII style

    glColor4f(color.getX(), color.getY(), color.getZ(), 1.0f);
    glBegin(GL_LINES);
    {
        glVertex3f(from.getX(), from.getY(), from.getZ());
        glVertex3f(to.getX(), to.getY(), to.getZ());
    }
    glEnd();

    mWindow->setActive(false);
}

void DebugDrawer::drawPlane(const btVector3 &planeNormal, btScalar planeConst, const btTransform &transform,
                            const btVector3 &color)
{
    if (mWindow == nullptr || !mWindow->isOpen()) return;
    mWindow->setActive(true);

    btVector3 planeOrigin = planeNormal * planeConst;
    btVector3 vec0, vec1;
    btPlaneSpace1(planeNormal, vec0, vec1);
    int nbLines = 31;
    float tileSize = 3.f;
    btScalar vecLen = (nbLines / 2) * tileSize;
    for (int i = 0; i < nbLines; ++i)
    {
        for (int j = 0; j < nbLines; ++j)
        {
            btVector3 pt0 = planeOrigin + vec0 * vecLen + vec1 * ((i - (nbLines / 2)) * tileSize);
            btVector3 pt1 = planeOrigin - vec0 * vecLen + vec1 * ((i - (nbLines / 2)) * tileSize);
            btVector3 pt2 = planeOrigin + vec1 * vecLen + vec0 * ((j - (nbLines / 2)) * tileSize);
            btVector3 pt3 = planeOrigin - vec1 * vecLen + vec0 * ((j - (nbLines / 2)) * tileSize);
            drawLine(transform * pt0, transform * pt1, color);
            drawLine(transform * pt2, transform * pt3, color);
        }
    }

    mWindow->setActive(false);
}

void DebugDrawer::drawContactPoint(const btVector3 &PointOnB, const btVector3 &normalOnB, btScalar distance,
                                   int lifeTime, const btVector3 &color)
{
    (void) distance;
    (void) lifeTime;

    if (mWindow == nullptr || !mWindow->isOpen()) return;
    mWindow->setActive(true);

    float tmp[6] = {
            PointOnB.getX(), PointOnB.getY(), PointOnB.getZ(),
            normalOnB.getX(), normalOnB.getY(), normalOnB.getZ()
    };

    glPushMatrix();
    {
        glColor4f(color.getX(), color.getY(), color.getZ(), 1.0f);
        glVertexPointer(3, GL_FLOAT, 0, &tmp);

        glPointSize(5.0f);
        glDrawArrays(GL_POINTS, 0, 2);
    }
    glPopMatrix();

    mWindow->setActive(false);
}

void DebugDrawer::reportErrorWarning(const char *warningString)
{
    REPORT();
    std::cerr << warningString << std::endl;
}

void DebugDrawer::draw3dText(const btVector3 &location, const char *textString)
{
    REPORT();

    (void) location;
    (void) textString;
}

void DebugDrawer::setDebugMode(int debugMode)
{
    REPORT();
    std::lock_guard<std::mutex> lock(mRenderingMutex);

    std::cout << "Debug mode set to : " << debugMode << "\n";
    mDebugMode = debugMode;

    if (getDebugMode() > btIDebugDraw::DBG_NoDebug)
    {
        // TODO: Logger
        std::cout << "Enabling Debug Draw." << std::endl;

        createWindow();
    }
    else
    {
        // TODO: Logger
        std::cout << "Disabling Debug Draw." << std::endl;

        destroyWindow();
    }
}

int DebugDrawer::getDebugMode() const
{
    return mDebugMode;
}

void DebugDrawer::createWindow()
{
    if (mWindow != nullptr && !mWindow->isOpen()) destroyWindow();
    else if (mWindow != nullptr) return;

    /// Creating the window
    // Request a 24-bits depth buffer when creating the window
    sf::ContextSettings contextSettings;
    contextSettings.depthBits = 24;
    contextSettings.stencilBits = 8;
    contextSettings.majorVersion = 2;
    contextSettings.minorVersion = 1;
//    contextSettings.attributeFlags = sf::ContextSettings::Core | sf::ContextSettings::Debug;

    // Create the main window
    mWindow = new sf::Window(sf::VideoMode(800, 600), "Debug draw window", sf::Style::Default,
                             contextSettings);
//    window.setVerticalSyncEnabled(true);

    mWindowHasFocus = true; // TODO: Check that

    /// Log
    // TODO: Logger
    sf::ContextSettings actualSettings = mWindow->getSettings();
    std::cout << "Major: " << actualSettings.majorVersion << " Minor: " << actualSettings.minorVersion << std::endl;
    std::string vendor = reinterpret_cast<const char *>(glGetString(GL_VENDOR));
    std::string renderer = reinterpret_cast<const char *>(glGetString(GL_RENDERER));
    std::cout << "Vendor: " << vendor << ". Renderer: " << renderer << std::endl;

    /// OpenGL
    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    glewInit();

    // Define the viewport dimensions
    glViewport(0, 0, mWindow->getSize().x, mWindow->getSize().y);
}

void DebugDrawer::destroyWindow()
{
    if (mWindow == nullptr) return;

    mWindow->close();
    delete mWindow;
    mWindow = nullptr;
}

void drawAxes(void)
{
    glColor3ub(255, 0, 0);
    glBegin(GL_LINE_STRIP);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.75f, 0.25f, 0.0f);
    glVertex3f(0.75f, -0.25f, 0.0f);
    glVertex3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.75f, 0.0f, 0.25f);
    glVertex3f(0.75f, 0.0f, -0.25f);
    glVertex3f(1.0f, 0.0f, 0.0f);
    glEnd();
    glBegin(GL_LINE_STRIP);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, 0.75f, 0.25f);
    glVertex3f(0.0f, 0.75f, -0.25f);
    glVertex3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.25f, 0.75f, 0.0f);
    glVertex3f(-0.25f, 0.75f, 0.0f);
    glVertex3f(0.0f, 1.0f, 0.0f);
    glEnd();
    glBegin(GL_LINE_STRIP);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.25f, 0.0f, 0.75f);
    glVertex3f(-0.25f, 0.0f, 0.75f);
    glVertex3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.25f, 0.75f);
    glVertex3f(0.0f, -0.25f, 0.75f);
    glVertex3f(0.0f, 0.0f, 1.0f);
    glEnd();

//    glColor3ub(255, 255, 0);
//    glRasterPos3f(1.1, 0.0, 0.0);
//    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, 'x');
//    glRasterPos3f(0.0, 1.1, 0.0);
//    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, 'y');
//    glRasterPos3f(0.0, 0.0, 1.1);
//    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, 'z');
}

void DebugDrawer::render(btCollisionWorld *bulletWorld)
{
    std::lock_guard<std::mutex> lock(mRenderingMutex);

    if (!onPreRender()) return;
    bulletWorld->debugDrawWorld();
    onPostRender();
}

bool DebugDrawer::onPreRender()
{
    if (mWindow == nullptr || !mWindow->isOpen()) return false;
    mWindow->setActive(true);

    // Check focus
    mWindowHasFocus = mWindow->hasFocus();

    /// Process events
    sf::Event event;
    while (mWindow->pollEvent(event))
    {
        // Close window: exit
        if (event.type == sf::Event::Closed ||
            ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape) && mWindowHasFocus))
        {
            destroyWindow();
            return true;
        }

        /*/ Handle the focus
        if (event.type == sf::Event::MouseLeft)
        {
            mWindowHasFocus = false;
        }
        else if (event.type == sf::Event::MouseEntered)
        {
            mWindowHasFocus = true;
        }*/

        // Adjust the viewport when the window is resized
        if (event.type == sf::Event::Resized)
        {
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glViewport(
                    0, 0,
                    event.size.width, event.size.height
            );
            gluPerspective(75, (float) (event.size.width) / (float) (event.size.height), 0.1, 10000);

            std::cout << "Resized !" << std::endl;
        }

        // Don't do the rest if we don't have the focus
        if (!mWindowHasFocus) continue;

        // R: Reset camera
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R)
        {
            mEyePos = DEFAULT_EYE_POS;
            mTargetPos = DEFAULT_TARGET_POS;
        }
    }

    /// Real time events
    if (mWindowHasFocus)
    {
        GLfloat posStep = 0.15f; // TODO: Change this
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::V))
        {
            posStep = 0.6f;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
        {
            mEyePos.x += posStep;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            mEyePos.x -= posStep;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
        {
            mEyePos.z += posStep;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            mEyePos.z -= posStep;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) || sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad0))
        {
            mEyePos.y += posStep;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::RControl))
        {
            mEyePos.y -= posStep;
        }
    }

    /// Clear the color buffer
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /// OpenGL / Display
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(
            mEyePos.x, mEyePos.y, mEyePos.z,
            mTargetPos.x, mTargetPos.y, mTargetPos.z,
            0, 1, 0
    );

    drawAxes();

    mWindow->setActive(false);

    return true;
}

void DebugDrawer::onPostRender()
{
    if (mWindow == nullptr || !mWindow->isOpen()) return;
    mWindow->setActive(true);

    // Flip buffers
    mWindow->display();

    mWindow->setActive(false);
}
