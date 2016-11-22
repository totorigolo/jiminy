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


#include <QtWidgets>

#include "OpenGLWidget.h"
#include "MainWindow.h"


MainWindow::MainWindow()
{
    openGLWidget = new OpenGLWidget;

    startButton = new QPushButton(tr("&Start"));
    startButton->setFocusPolicy(Qt::NoFocus);
    pauseButton = new QPushButton(tr("&Pause"));
    pauseButton->setFocusPolicy(Qt::NoFocus);
    quitButton = new QPushButton(tr("&Quit"));
    quitButton->setFocusPolicy(Qt::NoFocus);

    connect(startButton, SIGNAL(clicked()), openGLWidget, SLOT(start()));
    connect(pauseButton, SIGNAL(clicked()), openGLWidget, SLOT(pause()));
    connect(quitButton , SIGNAL(clicked()), qApp, SLOT(quit()));

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(startButton);
    buttonLayout->addWidget(pauseButton);
    buttonLayout->addWidget(quitButton);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(openGLWidget);
    layout->addLayout(buttonLayout);
    setLayout(layout);

    setWindowTitle(tr("Jiminy - Client"));
    resize(550, 370);
}
