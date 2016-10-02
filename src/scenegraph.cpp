#include "scenegraph.h"

SceneGraph::SceneGraph()
{

}

Node::Node(){
    vec4 a(1,0,0,0), b(0,1,0,0), c(0,0,1,0), d(0,0,0,1);
    mat4 t(a,b,c,d), r(a,b,c,d), s(a,b,c,d);
    Translate = t;
    Rotate = r;
    Scale = s;

    Geometry = nullptr;
}

Node::Node(mat4 trans, mat4 rot, mat4 scale){
    Translate = trans;
    Rotate = row;
    Scale = scale;

    Geometry = nullptr;
}

mat4 Node::Transformation(){
    return Translate * Rotate * Scale;
}

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
