#include "mygl.h"
#include <la.h>

#include <iostream>
#include <QApplication>
#include <QKeyEvent>
#include <math.h>

Node::Node(){
    glm::vec4 a(1,0,0,0), b(0,1,0,0), c(0,0,1,0), d(0,0,0,1);
    glm::mat4 t(a,b,c,d);
    *Transformation = t;
    Translate = nullptr;
    Rotate = nullptr;
    Scale = nullptr;
    Geometry = nullptr;
}

Node::Node(std::vector<Node *> children, TranslateNode *trans, RotateNode *rot, ScaleNode *scale){
    Translate = trans;
    Rotate = rot;
    Scale = scale;
    for(int i = 0; i < (int)children.size(); i++){
        Children.push_back(children[i]);
    }
    *Transformation = (*trans->Transformation) * (*rot->Transformation) * (*scale->Transformation);
    Geometry = nullptr;
}

TranslateNode::TranslateNode(){
    glm::vec4 a(1,0,0,0), b(0,1,0,0), c(0,0,1,0), d(0,0,0,1);
    glm::mat4 t(a,b,c,d);
    *Transformation = t;
    Translate = nullptr;
    Rotate = nullptr;
    Scale = nullptr;
    Geometry = nullptr;
}

TranslateNode::TranslateNode(float x, float y, float z){
    glm::vec4 a(1,0,0,0), b(0,1,0,0), c(0,0,1,0), d(x,y,z,1);
    glm::mat4 t(a,b,c,d);
    *Transformation = t;
    Translate = nullptr;
    Rotate = nullptr;
    Scale = nullptr;
    Geometry = nullptr;
}

RotateNode::RotateNode(){
    glm::vec4 a(1,0,0,0), b(0,1,0,0), c(0,0,1,0), d(0,0,0,1);
    glm::mat4 t(a,b,c,d);
    *Transformation = t;
    Translate = nullptr;
    Rotate = nullptr;
    Scale = nullptr;
    Geometry = nullptr;
}

RotateNode::RotateNode(float angle, float x, float y, float z){
    if(x == 1 && y == 0 && z == 0){
        glm::vec4 a(1,0,0,0), b(0,cos(angle),sin(angle),0), c(0,-sin(angle),cos(angle),0), d(0,0,0,1);
        glm::mat4 t(a,b,c,d);
        *Transformation = t;
        Translate = nullptr;
        Rotate = nullptr;
        Scale = nullptr;
        Geometry = nullptr;
    }
    else if(x == 0 && y == 1 && z == 0){
        glm::vec4 a(cos(angle),0,-sin(angle),0), b(0,1,0,0), c(sin(angle),0,cos(angle),0), d(0,0,0,1);
        glm::mat4 t(a,b,c,d);
        *Transformation = t;
        Translate = nullptr;
        Rotate = nullptr;
        Scale = nullptr;
        Geometry = nullptr;
    }
    else if(x ==0 && y == 0 && z == 1){
        glm::vec4 a(cos(angle),sin(angle),0,0), b(-sin(angle),cos(angle),0,0), c(0,0,1,0), d(0,0,0,1);
        glm::mat4 t(a,b,c,d);
        *Transformation = t;
        Translate = nullptr;
        Rotate = nullptr;
        Scale = nullptr;
        Geometry = nullptr;
    }
}

ScaleNode::ScaleNode(){
    glm::vec4 a(1,0,0,0), b(0,1,0,0), c(0,0,1,0), d(0,0,0,1);
    glm::mat4 t(a,b,c,d);
    *Transformation = t;
    Translate = nullptr;
    Rotate = nullptr;
    Scale = nullptr;
    Geometry = nullptr;
}

ScaleNode::ScaleNode(float x, float y, float z){
    glm::vec4 a(x,0,0,0), b(0,y,0,0), c(0,0,z,0), d(0,0,0,1);
    glm::mat4 t(a,b,c,d);
    *Transformation = t;
    Translate = nullptr;
    Rotate = nullptr;
    Scale = nullptr;
    Geometry = nullptr;
}



MyGL::MyGL(QWidget *parent)
    : GLWidget277(parent),
      geom_cylinder(this), geom_sphere(this),geom_cube(this), //TODO: When you make your Cube instance, add it to this init list
      prog_lambert(this), prog_flat(this)
{
}

MyGL::~MyGL()
{
    makeCurrent();
    glDeleteVertexArrays(1, &vao);
    geom_cylinder.destroy();
    geom_sphere.destroy();
    geom_cube.destroy();
}

void MyGL::initializeGL()
{
    // Create an OpenGL context using Qt's QOpenGLFunctions_3_2_Core class
    // If you were programming in a non-Qt context you might use GLEW (GL Extension Wrangler)instead
    initializeOpenGLFunctions();
    // Print out some information about the current OpenGL context
    debugContextVersion();

    // Set a few settings/modes in OpenGL rendering
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    // Set the size with which points should be rendered
    glPointSize(5);
    // Set the color with which the screen is filled at the start of each render call.
    glClearColor(0.5, 0.5, 0.5, 1);

    printGLErrorLog();

    // Create a Vertex Attribute Object
    glGenVertexArrays(1, &vao);

    //Create the instances of Cylinder and Sphere.
    geom_cylinder.create();

    geom_sphere.create();

    geom_cube.create();

    // Create and set up the diffuse shader
    prog_lambert.create(":/glsl/lambert.vert.glsl", ":/glsl/lambert.frag.glsl");
    // Create and set up the flat lighting shader
    prog_flat.create(":/glsl/flat.vert.glsl", ":/glsl/flat.frag.glsl");

    // Set a color with which to draw geometry since you won't have one
    // defined until you implement the Node classes.
    // This makes your geometry render green.
    prog_lambert.setGeometryColor(glm::vec4(0,1,0,1));

    // We have to have a VAO bound in OpenGL 3.2 Core. But if we're not
    // using multiple VAOs, we can just bind one once.
//    vao.bind();
    glBindVertexArray(vao);
}

void MyGL::resizeGL(int w, int h)
{
    //This code sets the concatenated view and perspective projection matrices used for
    //our scene's camera view.
    glm::vec4 c1(1.1933f, 0, 1.1933f, 0);
    glm::vec4 c2(0.9856f, 1.9712f, -0.9856f, 0);
    glm::vec4 c3(0.5785f, -0.5785f, -0.5785f, 11.9484f);
    glm::vec4 c4(0.5774f, -0.5774f, -0.5774f, 12.1244f);
    glm::mat4 viewproj(c1, c2, c3, c4);
    //Transpose since GLM is column major and I wrote out the rows of the matrix
    viewproj = glm::transpose(viewproj);

    // Upload the view-projection matrix to our shaders (i.e. onto the graphics card)

    prog_lambert.setViewProjMatrix(viewproj);
    prog_flat.setViewProjMatrix(viewproj);

    printGLErrorLog();
}

//This function is called by Qt any time your GL window is supposed to update
//For example, when the function updateGL is called, paintGL is called implicitly.
//DO NOT CONSTRUCT YOUR SCENE GRAPH IN THIS FUNCTION!
void MyGL::paintGL()
{
    // Clear the screen so that we only see newly drawn images
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //VVV CLEAR THIS CODE WHEN YOU IMPLEMENT SCENE GRAPH TRAVERSAL VVV///////////////////
#define NOPE
#ifdef NOPE
    //Create a model matrix. This one scales the sphere uniformly by 3, then translates it by <-2,0,0>.
    //Note that we have to transpose the model matrix before passing it to the shader
    //This is because OpenGL expects column-major matrices, but you've
    //implemented row-major matrices.
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(-2,0,0)) * glm::scale(glm::mat4(1.0f), glm::vec3(3,3,3));
    //Send the geometry's transformation matrix to the shader
    prog_lambert.setModelMatrix(model);
    //Draw the example sphere using our lambert shader
    prog_lambert.draw(geom_sphere);

    //Now do the same to render the cylinder
    //We've rotated it -45 degrees on the Z axis, then translated it to the point <2,2,0>
//    model = glm::translate(glm::mat4(1.0f), glm::vec3(2,2,0)) * glm::rotate(glm::mat4(1.0f), glm::radians(-45.0f), glm::vec3(0,0,1));
//    prog_lambert.setModelMatrix(model);
//    prog_lambert.draw(geom_cylinder);

    //Now do the same to render the cube
    //We've rotated it -45 degrees on the Z axis, then translated it to the point <2,2,0>
    model = glm::translate(glm::mat4(1.0f), glm::vec3(2,2,0)) * glm::rotate(glm::mat4(1.0f), glm::radians(-45.0f), glm::vec3(0,0,1));
    prog_lambert.setModelMatrix(model);
    prog_lambert.draw(geom_cube);
#endif


    //^^^ CLEAR THIS CODE WHEN YOU IMPLEMENT SCENE GRAPH TRAVERSAL ^^^/////////////////

    //Here is a good spot to call your scene graph traversal function.
//    Traverse(N,T,prog_lambert);
}


void MyGL::keyPressEvent(QKeyEvent *e)
{
    // http://doc.qt.io/qt-5/qt.html#Key-enum
    if (e->key() == Qt::Key_Escape) {
        QApplication::quit();
    }
}


void MyGL::Traverse(Node N, glm::mat4 T, ShaderProgram p){
    T = T * (*N.Transformation);
    for(int i = 0; i < N.Children.size(); i++){
        Traverse(N, T, p);
    }
    if(N.Geometry != nullptr){
        p.setModelMatrix(T);
        p.draw(*N.Geometry);
    }
}
