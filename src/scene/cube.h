#pragma once

#include "drawable.h"
#include <la.h>

#include <QOpenGLContext>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>

class Cube:public Drawable
{
public:
public:
    Cube(GLWidget277* context);
    virtual void create();
};

