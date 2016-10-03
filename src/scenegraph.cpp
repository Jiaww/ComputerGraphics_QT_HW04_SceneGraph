#include "scenegraph.h"
#include <math.h>

SceneGraph::SceneGraph()
{

}

Node::Node(){
    vec4 a(1,0,0,0), b(0,1,0,0), c(0,0,1,0), d(0,0,0,1);
    mat4 t(a,b,c,d);
    *Transformation = t;
    TranslateNode = RotateNode = ScaleNode = nullptr;
    Geometry = nullptr;
}

Node::Node(trans, rot, scale){
    TranslateNode = trans;
    RotateNode = rot;
    ScaleNode = scale;
    *Transformation = (*trans->Transformation) * (*rot->Transformation) * (*scale->Transformation);
    Geometry = nullptr;
}

TranslateNode::TranslateNode(){
    vec4 a(1,0,0,0), b(0,1,0,0), c(0,0,1,0), d(0,0,0,1);
    mat4 t(a,b,c,d);
    *Transformation = t;
    TranslateNode = RotateNode = ScaleNode = nullptr;
    Geometry = nullptr;
}

TranslateNode::TranslateNode(float x, float y, float z){
    vec4 a(1,0,0,0), b(0,1,0,0), c(0,0,1,0), d(x,y,z,1);
    mat4 t(a,b,c,d);
    *Transformation = t;
    TranslateNode = RotateNode = ScaleNode = nullptr;
    Geometry = nullptr;
}

RotateNode::RotateNode(){
    vec4 a(1,0,0,0), b(0,1,0,0), c(0,0,1,0), d(0,0,0,1);
    mat4 t(a,b,c,d);
    *Transformation = t;
    TranslateNode = RotateNode = ScaleNode = nullptr;
    Geometry = nullptr;
}

RotateNode::RotateNode(float angle, float x, float y, float z){
    if(x == 1 && y == 0 && z == 0){
        vec4 a(1,0,0,0), b(0,cos(angle),sin(angle),0), c(0,-sin(angle),cos(angle),0), d(0,0,0,1);
        mat4 t(a,b,c,d);
        *Transformation = t;
        TranslateNode = RotateNode = ScaleNode = nullptr;
        Geometry = nullptr;
    }
    else if(x == 0 && y == 1 && z == 0){
        vec4 a(cos(angle),0,-sin(angle),0), b(0,1,0,0), c(sin(angle),0,cos(angle),0), d(0,0,0,1);
        mat4 t(a,b,c,d);
        *Transformation = t;
        TranslateNode = RotateNode = ScaleNode = nullptr;
        Geometry = nullptr;
    }
    else if(x ==0 && y == 0 && z == 1){
        vec4 a(cos(angle),sin(angle),0,0), b(-sin(angle),cos(angle),0,0), c(0,0,1,0), d(0,0,0,1);
        mat4 t(a,b,c,d);
        *Transformation = t;
        TranslateNode = RotateNode = ScaleNode = nullptr;
        Geometry = nullptr;
    }
}

ScaleNode::ScaleNode(){
    vec4 a(1,0,0,0), b(0,1,0,0), c(0,0,1,0), d(0,0,0,1);
    mat4 t(a,b,c,d);
    *Transformation = t;
    TranslateNode = RotateNode = ScaleNode = nullptr;
    Geometry = nullptr;
}

ScaleNode::ScaleNode(float x, float y, float z){
    vec4 a(x,0,0,0), b(0,y,0,0), c(0,0,z,0), d(0,0,0,1);
    mat4 t(a,b,c,d);
    *Transformation = t;
    TranslateNode = RotateNode = ScaleNode = nullptr;
    Geometry = nullptr;
}

struct Geo{

};

void Draw(Geo *G, mat4 T){
    //Draw!!!
}

void Traverse(Node N, mat4 T){
    T = T * N::Transformation();
    for(int i = 0; i < N.Children.size(); i++){
        Traverse(N, T);
    }
    if(N.Geometry != nullptr){
        Draw(N.Geometry, T);
    }
}
