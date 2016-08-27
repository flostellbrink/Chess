/*
 * Copyright (C) 2013, 2014
 * Computer Graphics Group, University of Siegen
 * Written by Martin Lambers <martin.lambers@uni-siegen.de>
 * All rights reserved.
 */


#include <GL/glew.h>

#include "glwidget.hpp"

#include <QMouseEvent>

#define GLM_FORCE_RADIANS
#define GLM_SWIZZLE
#include <glm/gtx/transform.hpp>

#include "gui/config.h"
#include "objects/objectManager.hpp"

#include "math.h"
#ifndef M_PI
#define M_PI glm::pi<float>()
#endif

#include <iostream>

GLWidget::GLWidget(QWidget *&parent) : QOpenGLWidget(parent),
    _updateTimer(this), _stopWatch()
{
    // update the scene periodically
    QObject::connect(&_updateTimer, SIGNAL(timeout()), this, SLOT(animateGL()));
    _updateTimer.start(18);
    _stopWatch.start();


    // create all drawable elements
    //_skybox = std::make_shared<Skybox>();

    _cameraRotation = glm::vec2(0, M_PI / 2.);
}

void GLWidget::show()
{
    QOpenGLWidget::show();

    // check for a valid context
    if (!isValid() || !context()->isValid() || context()->format().majorVersion() != 4) {
        QMessageBox::critical(this, "Error", "Cannot get a valid OpenGL 4 context.");
        exit(1);
    }
}

void GLWidget::initializeGL()
{
    /* Initialize OpenGL extensions */
    glewExperimental = GL_TRUE; // otherwise some function pointers are NULL...
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
      /* Problem: glewInit failed, something is seriously wrong. */
      fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
    }
    glGetError(); // clear a gl error produced by glewInit

    // make sure the context is current
    makeCurrent();

    ObjectManager::Instance.NewGame();
}

void GLWidget::resizeGL(int width, int height)
{
    // update the viewport
    glViewport(0, 0, width, height);

    Config::viewportWidth = width;
    Config::viewportHeight = height;

    _projection_matrix = glm::perspective(glm::radians(90.0f),
            (float)Config::viewportWidth / (float)Config::viewportHeight,
            0.1f, 100.0f);
}

void GLWidget::paintGL()
{
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //glEnable(GL_MULTISAMPLE);   

    ObjectManager::Instance.Draw(_projection_matrix);
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    ObjectManager::Instance.MouseDown(event);
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event)
{
    ObjectManager::Instance.MouseUp(event);
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    ObjectManager::Instance.MouseMove(event);
}

void GLWidget::wheelEvent(QWheelEvent *event)
{
    ObjectManager::Instance.MouseWheel(event);
}

void GLWidget::animateGL()
{
    // make the context current in case there are glFunctions called
    makeCurrent();

    // get the time delta
    float timeElapsedMs = _stopWatch.nsecsElapsed() / 1000000.0f;
    _stopWatch.restart();

    QPoint mouse = mapFromGlobal(QCursor::pos());
    ObjectManager::Instance.Update(timeElapsedMs, _projection_matrix, vec2(mouse.x(), mouse.y()));

    // update the widget
    update();
}


