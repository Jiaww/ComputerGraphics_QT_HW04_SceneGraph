#include "cone.h"
#include<la.h>
#include <iostream>
#include <math.h>

static const int CONE_IDX_COUNT = 114;//760 tris * 3
static const int CONE_VERT_COUNT = 381;

Cone::Cone(GLWidget277 *context) : Drawable(context)
{}

//These are functions that are only defined in this cpp file. They're used for organizational purposes
//when filling the arrays used to hold the vertex and index data.
void createConeVertexPositions(glm::vec4 (&cone_vert_pos)[CONE_VERT_COUNT]){
    //Create rings of vertices for the non-pole vertices
    //These will fill indices 1 - 380. Indices 0 and 381 will be filled by the two pole vertices.

    //Store bottom cap verts (IDX 0 - 19)
    for(int j = 0; j < 19; j++){    //height
        for(int i = 0; i < 20; i++){ // rings
            float r = (1.0f - j * 0.05f) * 0.5f; //radius
            glm::vec4 v = glm::rotate(glm::mat4(1.0f), glm::radians(i*18.0f), glm::vec3(0, 1, 0)) * glm::vec4(r,-0.5f+j*0.05f,0,1);
            cone_vert_pos[j*20 + i] = v;
        }
    }

    //store the top vertex
    cone_vert_pos[380] = glm::vec4(0, 0, 0.5f, 1);

}


void createConeVertexNormals(glm::vec4 (&cone_vert_nor)[CONE_VERT_COUNT]){
    //Unlike a cylinder, a coneere only needs to be one normal per vertex
    //because a coneere does not have sharp edges.

    glm::vec4 v;
    //i is the Z axis rotation
    for(int i = 0; i < 20; i++){
        cone_vert_nor[i] = glm::vec4(0,-1,0,0);
    }
    //Store top of barrel normals (IDX 40 - 59)
    float r = atan(0.5f);
    for(int i = 0; i < 20; i++){
        glm::vec4 v = glm::rotate(glm::mat4(1.0f), glm::radians(i*18.0f), glm::vec3(0, 1, 0)) * glm::vec4(1,0,0,0);
        v = glm::normalize(v + glm::vec4(0, 0.5*cos(r)*sin(r), 0, 1));
        for(int j = 0; j < 19; j++){
            cone_vert_nor[20*j+i] = v;
        }
    }
    //Add the pole normals
    cone_vert_nor[380] = glm::vec4(0, 1.0f, 0, 0);
}


void createConeIndices(GLuint (&cone_idx)[CONE_IDX_COUNT]){
    int index = 0;
    //Build indices for the bottom cap (18 tris)
    for(int i = 0; i < 18; i++){
        cone_idx[index] = 0;
        cone_idx[index + 1] = i+1;
        cone_idx[index + 2] = i+2;
        index += 3;
    }

    //Build indices for the body vertices
    //i is the Z axis rotation
    for(int i =0; i< 19;i++){
        cone_idx[index] = 380;
        cone_idx[index + 1] = i;
        cone_idx[index + 2] = i+1;
        index += 3;
    }
    cone_idx[111] = 380;
    cone_idx[112] = 19;
    cone_idx[113] = 0;

}

void Cone::create()
{
    GLuint cone_idx[CONE_IDX_COUNT];
    glm::vec4 cone_vert_pos[CONE_VERT_COUNT];
    glm::vec4 cone_vert_nor[CONE_VERT_COUNT];

    createConeVertexPositions(cone_vert_pos);
    createConeVertexNormals(cone_vert_nor);
    createConeIndices(cone_idx);

    count = CONE_IDX_COUNT;

    // Create a VBO on our GPU and store its handle in bufIdx
    generateIdx();
    // Tell OpenGL that we want to perform subsequent operations on the VBO referred to by bufIdx
    // and that it will be treated as an element array buffer (since it will contain triangle indices)
    context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
    // Pass the data stored in cyl_idx into the bound buffer, reading a number of bytes equal to
    // cone_IDX_COUNT multiplied by the size of a GLuint. This data is sent to the GPU to be read by shader programs.
    context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, CONE_IDX_COUNT * sizeof(GLuint), cone_idx, GL_STATIC_DRAW);

    // The next few sets of function calls are basically the same as above, except bufPos and bufNor are
    // array buffers rather than element array buffers, as they store vertex attributes like position.
    generatePos();
    context->glBindBuffer(GL_ARRAY_BUFFER, bufPos);
    context->glBufferData(GL_ARRAY_BUFFER, CONE_VERT_COUNT * sizeof(glm::vec4), cone_vert_pos, GL_STATIC_DRAW);

    generateNor();
    context->glBindBuffer(GL_ARRAY_BUFFER, bufNor);
    context->glBufferData(GL_ARRAY_BUFFER, CONE_VERT_COUNT * sizeof(glm::vec4), cone_vert_nor, GL_STATIC_DRAW);
}
