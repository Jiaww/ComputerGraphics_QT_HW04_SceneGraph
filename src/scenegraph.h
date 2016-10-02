#ifndef SCENEGRAPH_H
#define SCENEGRAPH_H
#include<OpenGL.h>

class Node{
public:
    mat4 Translate;
    mat4 Rotate;
    mat4 Scale;
    Geo *Geometry;
    std::vector<Node> Children;
    Node();
    Node(mat4 trans, mat4 rot, mat4 scale);
    mat4 Transformation();
};

void Draw(Geo *G, mat4 T);
void Traverse(Node N, mat4 T);
struct Geo{
    mat4 Pos;
};

class SceneGraph
{
private:

public:
    SceneGraph();
};

#endif // SCENEGRAPH_H
