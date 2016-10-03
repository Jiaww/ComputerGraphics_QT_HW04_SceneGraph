#ifndef MYGL_H
#define MYGL_H

#include <glwidget277.h>
#include <utils.h>
#include <shaderprogram.h>
#include <scene/cylinder.h>
#include <scene/sphere.h>

#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>

//Node:
class TranslateNode;
class RotateNode;
class ScaleNode;

struct Geo{

};

class Node{
public:
    QString *name;
    TranslateNode *Translate;
    RotateNode *Rotate;
    ScaleNode *Scale;
    glm::mat4 *Transformation;
    Drawable *Geometry;
    std::vector<Node*> Children;
    Node();
    Node(TranslateNode *trans, RotateNode *rot, ScaleNode *scale);
};

class TranslateNode:public Node{
    TranslateNode();
    TranslateNode(float x, float y, float z);
};

class RotateNode:public Node{
    RotateNode();
    RotateNode(float angle, float x, float y, float z);

};

class ScaleNode:public Node{
    ScaleNode();
    ScaleNode(float x, float y, float z);
};

void Draw(Drawable *G, glm::mat4 T);
void Traverse(Node N, glm::mat4 T);


class MyGL
    : public GLWidget277
{
private:
    Cylinder geom_cylinder;// The instance of a unit cylinder we can use to render any cylinder
    Sphere geom_sphere;// The instance of a unit sphere we can use to render any sphere
    ShaderProgram prog_lambert;// A shader program that uses lambertian reflection
    ShaderProgram prog_flat;// A shader program that uses "flat" reflection (no shadowing at all)

    GLuint vao; // A handle for our vertex array object. This will store the VBOs created in our geometry classes.
                // Don't worry too much about this. Just know it is necessary in order to render geometry.


public:
    explicit MyGL(QWidget *parent = 0);
    ~MyGL();

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

protected:
    void keyPressEvent(QKeyEvent *e);
};


#endif // MYGL_H
