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


OpenGLWidget::OpenGLWidget(QWidget *parent)
        : QOpenGLWidget(parent), mClearColor{120, 120, 120}
{
}

void OpenGLWidget::initializeGL()
{
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glClearColor((GLclampf) mClearColor.redF(), (GLclampf) mClearColor.greenF(), (GLclampf) mClearColor.blueF(),
                    1.0f);
}

void OpenGLWidget::resizeGL(int w, int h)
{
    Q_UNUSED(w);
    Q_UNUSED(h);
}

void OpenGLWidget::paintGL()
{
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glClearColor((GLclampf) mClearColor.redF(), (GLclampf) mClearColor.greenF(), (GLclampf) mClearColor.blueF(),
                    1.0f);
    f->glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGLWidget::start()
{
    mClearColor.setRgb(0, 120, 0);
    update();
}

void OpenGLWidget::pause()
{
    mClearColor.setRgb(120, 0, 0);
    update();
}
