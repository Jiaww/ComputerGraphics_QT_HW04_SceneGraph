#pragma once

#include "drawable.h"
#include <la.h>

#include <QOpenGLContext>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>

class Pipe : public Drawable
{
public:
    Pipe(GLWidget277* context);
    virtual void create();
};
