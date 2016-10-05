#include "drawable.h"
#include <la.h>

#include <QOpenGLContext>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>

class Cone : public Drawable
{
public:
    Cone(GLWidget277* context);
    virtual void create();
};
