#include "mygl.h"
#include <la.h>

#include <iostream>
#include <QApplication>
#include <QKeyEvent>
#include <math.h>
#include <QTimer>

Node::Node(){
    glm::vec4 a(1,0,0,0), b(0,1,0,0), c(0,0,1,0), d(0,0,0,1);
    glm::mat4 t(a,b,c,d);
    Transformation = t;
    Animation = t;
    Translate = nullptr;
    Rotate = nullptr;
    Scale = nullptr;
    Geometry = nullptr;
    name = nullptr;
    //Children.clear();
}

Node::Node(const QString &name){
    glm::vec4 a(1,0,0,0), b(0,1,0,0), c(0,0,1,0), d(0,0,0,1);
    glm::mat4 t(a,b,c,d);
    Transformation = t;
    Animation = t;
    this->name = name;
    Translate = nullptr;
    Rotate = nullptr;
    Scale = nullptr;
    Geometry = nullptr;
    //Children.clear();
    setText(0, name);
}

Node::Node(const QString &name, TranslateNode *trans, RotateNode *rot, ScaleNode *scale){
    Animation = glm::mat4(1);
    Translate = trans;
    Rotate = rot;
    Scale = scale;
    Transformation = (trans->Transformation) * (rot->Transformation) * (scale->Transformation);
    Geometry = nullptr;
    setText(0, name);
   }

void Node::addChild(QTreeWidgetItem *c)
{
    QTreeWidgetItem::addChild(c);
    Children.push_back((Node*)c);
}

TranslateNode::TranslateNode(){
    Animation = glm::mat4(1);
    glm::vec4 a(1,0,0,0), b(0,1,0,0), c(0,0,1,0), d(0,0,0,1);
    glm::mat4 t(a,b,c,d);
    Transformation = t;
    Translate = nullptr;
    Rotate = nullptr;
    Scale = nullptr;
    Geometry = nullptr;
    name = nullptr;
}

TranslateNode::TranslateNode(float x, float y, float z){
    Animation = glm::mat4(1);
    glm::vec4 a(1,0,0,0), b(0,1,0,0), c(0,0,1,0), d(x,y,z,1);
    glm::mat4 t(a,b,c,d);
    Transformation = t;
    Translate = nullptr;
    Rotate = nullptr;
    Scale = nullptr;
    Geometry = nullptr;
    name = nullptr;
}

RotateNode::RotateNode(){
    Animation = glm::mat4(1);
    glm::vec4 a(1,0,0,0), b(0,1,0,0), c(0,0,1,0), d(0,0,0,1);
    glm::mat4 t(a,b,c,d);
    angle = 0;
    Transformation = t;
    Translate = nullptr;
    Rotate = nullptr;
    Scale = nullptr;
    Geometry = nullptr;
    name = nullptr;
}

RotateNode::RotateNode(float angle, float x, float y, float z){
    Animation = glm::mat4(1);
    this->angle = angle;
    float rad = angle/180*3.1415926;
    if(x == 1 && y == 0 && z == 0){
        glm::vec4 a(1,0,0,0), b(0,cos(rad),sin(rad),0), c(0,-sin(rad),cos(rad),0), d(0,0,0,1);
        glm::mat4 t(a,b,c,d);
        Transformation = t;
        Translate = nullptr;
        Rotate = nullptr;
        Scale = nullptr;
        Geometry = nullptr;
        name = nullptr;
    }
    else if(x == 0 && y == 1 && z == 0){
        glm::vec4 a(cos(rad),0,-sin(rad),0), b(0,1,0,0), c(sin(rad),0,cos(rad),0), d(0,0,0,1);
        glm::mat4 t(a,b,c,d);
        Transformation = t;
        Translate = nullptr;
        Rotate = nullptr;
        Scale = nullptr;
        Geometry = nullptr;
        name = nullptr;
    }
    else if(x ==0 && y == 0 && z == 1){
        glm::vec4 a(cos(rad),sin(rad),0,0), b(-sin(rad),cos(rad),0,0), c(0,0,1,0), d(0,0,0,1);
        glm::mat4 t(a,b,c,d);
        Transformation = t;
        Translate = nullptr;
        Rotate = nullptr;
        Scale = nullptr;
        Geometry = nullptr;
        name = nullptr;
    }
    else{
        Transformation = glm::mat4(1);
    }
}

ScaleNode::ScaleNode(){
    Animation = glm::mat4(1);
    glm::vec4 a(1,0,0,0), b(0,1,0,0), c(0,0,1,0), d(0,0,0,1);
    glm::mat4 t(a,b,c,d);
    Transformation = t;
    Translate = nullptr;
    Rotate = nullptr;
    Scale = nullptr;
    Geometry = nullptr;
    name = nullptr;
}

ScaleNode::ScaleNode(float x, float y, float z){
    Animation = glm::mat4(1);
    glm::vec4 a(x,0,0,0), b(0,y,0,0), c(0,0,z,0), d(0,0,0,1);
    glm::mat4 t(a,b,c,d);
    Transformation = t;
    Translate = nullptr;
    Rotate = nullptr;
    Scale = nullptr;
    Geometry = nullptr;
    name = nullptr;
}




void MyGL::Traverse(Node *N, glm::mat4 T, ShaderProgram p, glm::vec4 origin_color){
    //the last 2 items are for Animation part
    T = T * N->Transformation * N->Animation * N->Animation_Rotation;
    for(int i = 0; i < N->Children.size(); i++){
        Traverse(N->Children[i], T, p, origin_color);
    }
    if(N->Geometry != nullptr){
        //If being selected, change the color to white
        if(N == selected){
            p.setGeometryColor(glm::vec4(1,1,1,1));
        }
        else{
            p.setGeometryColor(origin_color);
        }
        p.setModelMatrix(T);
        p.draw(*(N->Geometry));
    }
}


MyGL::MyGL(QWidget *parent)
    : GLWidget277(parent),
      geom_cylinder(this), geom_sphere(this),geom_cube(this),geom_cone(this),geom_pipe(this),//TODO: When you make your Cube instance, add it to this init list
      prog_lambert(this), prog_flat(this),selected(nullptr)
{
//    emit sig_RootNode(Root);
}

MyGL::~MyGL()
{
    makeCurrent();
    glDeleteVertexArrays(1, &vao);
    geom_cylinder.destroy();
    geom_sphere.destroy();
    geom_cube.destroy();
    geom_cone.destroy();
    geom_pipe.destroy();
}

// Rotate!!!!!Limb!!!!
void Rotate_Limb(Node *N, float scaler, float angle, int x, int y, int z){
    //Scale in y;
    scaler = scaler /2;
    TranslateNode *Trans1 = new TranslateNode(0, scaler, 0), *Trans2 = new TranslateNode(0, -scaler, 0);
    RotateNode *R = new RotateNode(angle, x, y, z);
    N->Transformation = N->Transformation * Trans1->Transformation * R->Transformation * Trans2->Transformation;
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
    geom_cube.create();
    geom_cone.create();
    geom_sphere.create();
    geom_pipe.create();

    Root = new Node("Root");
    //Character:
    //Body Part
    TranslateNode *TBody = new TranslateNode(0,0,0);
    RotateNode *RBody = new RotateNode(45, 0, 1, 0);
    ScaleNode *SBody = new ScaleNode(1,1,2);
    Node *Body = new Node("Body",TBody, RBody, SBody);
    Body->Geometry = &geom_cube;
    //Head Part
    TranslateNode *THead = new TranslateNode(0,1,0);
    RotateNode *RHead = new RotateNode(0, 0, 1, 0);
    ScaleNode *SHead = new ScaleNode(1,1,0.5f);
    Node *Head = new Node("Head",THead,RHead,SHead);
    Head->Geometry = &geom_sphere;
    //Limb Part
    //Limb01
    TranslateNode *TLimb01 = new TranslateNode(0,-1,0);
    RotateNode *RLimb01 = new RotateNode(0, 0, 1, 0);
    ScaleNode *SLimb01 = new ScaleNode(1,1,0.5f);
    Node *Limb01 = new Node("Limb01",TLimb01,RLimb01,SLimb01);
    Limb01->Geometry = &geom_cylinder;
    //Limb11
    TranslateNode *TLimb11 = new TranslateNode(0,-1,0);
    RotateNode *RLimb11 = new RotateNode(0, 0, 0, 1);
    ScaleNode *SLimb11 = new ScaleNode(0.5f,1,0.5f);
    Node *Limb11 = new Node("Limb11",TLimb11,RLimb11,SLimb11);
    Limb11->Geometry = &geom_cylinder;
    glm::mat4 r;
    Rotate_Limb(Limb01, 1, 30, 1, 0, 0);
    Rotate_Limb(Limb11, 1, 30, 1, 0, 0);
    Root->addChild(Body);
    Body->addChild(Head);
    Body->addChild(Limb01);
    Limb01->addChild(Limb11);

    // Create and set up the diffuse shader
    prog_lambert.create(":/glsl/lambert.vert.glsl", ":/glsl/lambert.frag.glsl");
    // Create and set up the flat lighting shader
    prog_flat.create(":/glsl/flat.vert.glsl", ":/glsl/flat.frag.glsl");

    // Set a color with which to draw geometry since you won't have one
    // defined until you implement the Node classes.
    // This makes your geometry render green.
    prog_lambert.setGeometryColor(glm::vec4(0,1,0,1));
    prog_flat.setGeometryColor(glm::vec4(0,1,0,1));

    // We have to have a VAO bound in OpenGL 3.2 Core. But if we're not
    // using multiple VAOs, we can just bind one once.
//    vao.bind();
    glBindVertexArray(vao);

//Animation:
    TranslateNode *T = new TranslateNode(1,1,1);
    RotateNode *R = new RotateNode(30,0,0,1);
    ScaleNode *S = new ScaleNode(2,2,2);
    setAnimation(Body,T,R,S);
    emit sig_RootNode(this->Root);

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
    RotateNode *RR= new RotateNode(90, 0 , 1 ,0);
    TranslateNode *TT = new TranslateNode(-0.3,-0.3,0);
    model = model * RR->Transformation * TT->Transformation;
    prog_lambert.setModelMatrix(model);
    prog_flat.setModelMatrix(model);
    //Draw the example sphere using our lambert shader
    prog_lambert.draw(geom_cone);

    //Now do the same to render the cylinder
    //We've rotated it -45 degrees on the Z axis, then translated it to the point <2,2,0>
    model = glm::translate(glm::mat4(1.0f), glm::vec3(2,2,0)) * glm::rotate(glm::mat4(1.0f), glm::radians(-45.0f), glm::vec3(0,0,1));
    prog_lambert.setModelMatrix(model);
    //prog_lambert.draw(geom_cylinder);
    prog_lambert.draw(geom_pipe);

#endif


    //^^^ CLEAR THIS CODE WHEN YOU IMPLEMENT SCENE GRAPH TRAVERSAL ^^^/////////////////

    //Here is a good spot to call your scene graph traversal function.

    Traverse(Root,Root->Transformation,prog_lambert, glm::vec4(0,1,0,1));
}


void MyGL::keyPressEvent(QKeyEvent *e)
{
    // http://doc.qt.io/qt-5/qt.html#Key-enum
    if (e->key() == Qt::Key_Escape) {
        QApplication::quit();
    }
}

void MyGL::slot_ChosenPart(QTreeWidgetItem *n)
{
    selected = (Node*) n;
}

void MyGL::SetTimer(){
    timer = new QTimer;
    timer->setInterval(1000);
    timer->start();
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimerOut()));
}

void MyGL::setAnimation(Node *N, TranslateNode *T, RotateNode *R, ScaleNode *S){
    AniNode = N;
    AniTrans = T;
    AniRot = R;
    AniSca = S;
    SetTimer();
}

void MyGL::onTimerOut(){
   //Translation Animation
    if((AniTrans->Transformation[3][0] - 0.1)>=0){
        AniNode->Animation[3][0] = AniNode->Animation[3][0] + 0.1;
        AniTrans->Transformation[3][0] = AniTrans->Transformation[3][0] - 0.1;
    }
    else{
        AniNode->Animation[3][0] = AniNode->Animation[3][0] + AniTrans->Transformation[3][0];
        AniTrans->Transformation[3][0] = 0;
    }
    if((AniTrans->Transformation[3][1] - 0.1)>=0){
        AniNode->Animation[3][1] = AniNode->Animation[3][1] + 0.1;
        AniTrans->Transformation[3][1] = AniTrans->Transformation[3][1] - 0.1;
    }
    else{
        AniNode->Animation[3][1] = AniNode->Animation[3][1] + AniTrans->Transformation[3][1];
        AniTrans->Transformation[3][1] = 0;
    }
    if((AniTrans->Transformation[3][2] - 0.1)>=0){
        AniNode->Animation[3][2] = AniNode->Animation[3][2] + 0.1;
        AniTrans->Transformation[3][2] = AniTrans->Transformation[3][2] - 0.1;
    }
    else{
        AniNode->Animation[3][2] = AniNode->Animation[3][2] + AniTrans->Transformation[3][2];
        AniTrans->Transformation[3][2] = 0;
    }
    //Scale Animation
     if((AniSca->Transformation[0][0] - 0.1)>=1){
         AniNode->Animation[0][0] = AniNode->Animation[0][0] + 0.1;
         AniSca->Transformation[0][0] = AniSca->Transformation[0][0] - 0.1;
     }
     else{
         AniNode->Animation[0][0] = AniNode->Animation[0][0] + AniSca->Transformation[0][0]-1;
         AniSca->Transformation[0][0] = 1;
     }
     if((AniSca->Transformation[1][1] - 0.1)>=1){
         AniNode->Animation[1][1] = AniNode->Animation[1][1] + 0.1;
         AniSca->Transformation[1][1] = AniSca->Transformation[1][1] - 0.1;
     }
     else{
         AniNode->Animation[1][1] = AniNode->Animation[1][1] + AniSca->Transformation[1][1]-1;
         AniSca->Transformation[1][1] = 1;
     }
     if((AniSca->Transformation[2][2] - 0.1)>=1){
         AniNode->Animation[2][2] = AniNode->Animation[2][2] + 0.1;
         AniSca->Transformation[2][2] = AniSca->Transformation[2][2] - 0.1;
     }
     else{
         AniNode->Animation[2][2] = AniNode->Animation[2][2] + AniSca->Transformation[2][2]-1;
         AniSca->Transformation[2][2] = 1;
     }
     //Rotate Animation
     if(AniRot->angle - 5 >= 0){
         RotateNode *R = new RotateNode(5, 0, 0, 1);
         AniNode->Animation_Rotation = AniNode->Animation_Rotation * R->Transformation;
         RotateNode *F = new RotateNode(AniRot->angle - 5, 0, 0, 1);
         AniRot = F;
     }
     else{
         AniNode->Animation_Rotation = AniNode->Animation_Rotation * AniRot->Transformation;
         AniRot->angle = 0;
     }
}
