#ifndef MYGL_H
#define MYGL_H

#include <glwidget277.h>
#include <utils.h>
#include <shaderprogram.h>
#include <scene/cylinder.h>
#include <scene/sphere.h>
#include <scene/cube.h>
#include <scene/cone.h>
#include <scene/pipe.h>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>
#include <QTreeWidgetItem>
#include <QString>
#include <QTimer>
//Node:
class TranslateNode;
class RotateNode;
class ScaleNode;


class Node : public QTreeWidgetItem{
public:
    QString name;
    TranslateNode *Translate;
    RotateNode *Rotate;
    ScaleNode *Scale;
    glm::mat4 Transformation;
    Drawable *Geometry;
    std::vector<Node*> Children;
    //Animation:
    glm::mat4 Animation;
    glm::mat4 Animation_Rotation = glm::mat4(1);
    Node();
    Node(const QString &name);
    Node(const QString &name, TranslateNode *trans, RotateNode *rot, ScaleNode *scale);

    virtual void addChild(QTreeWidgetItem*);
};

class TranslateNode:public Node{
public:
    TranslateNode();
    TranslateNode(float x, float y, float z);
};

class RotateNode:public Node{
public:
    float angle;
    RotateNode();
    RotateNode(float angle, float x, float y, float z);

};

class ScaleNode:public Node{
public:
    ScaleNode();
    ScaleNode(float x, float y, float z);
};



class MyGL
    : public GLWidget277
{
    Q_OBJECT

private:
    Cylinder geom_cylinder;// The instance of a unit cylinder we can use to render any cylinder
    Sphere geom_sphere;// The instance of a unit sphere we can use to render any sphere
    Cube geom_cube;
    Cone geom_cone;
    Pipe geom_pipe;
    ShaderProgram prog_lambert;// A shader program that uses lambertian reflection
    ShaderProgram prog_flat;// A shader program that uses "flat" reflection (no shadowing at all)

    GLuint vao; // A handle for our vertex array object. This will store the VBOs created in our geometry classes.
                // Don't worry too much about this. Just know it is necessary in order to render geometry.
    Node *Root;
    Node *selected;
    QTimer *timer;

//Animation:
    TranslateNode *AniTrans;
    RotateNode *AniRot;
    ScaleNode *AniSca;
    Node *AniNode;

public:
    explicit MyGL(QWidget *parent = 0);
    ~MyGL();

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    void Traverse(Node *N, glm::mat4 T, ShaderProgram p, glm::vec4 origin_color);

    void SetTimer();
    void setAnimation(Node *N, TranslateNode *T, RotateNode *R, ScaleNode *S);
protected:
    void keyPressEvent(QKeyEvent *e);
signals:
    void sig_RootNode(QTreeWidgetItem*);
public slots:
    void slot_ChosenPart(QTreeWidgetItem*);
    void onTimerOut();

};


#endif // MYGL_H
