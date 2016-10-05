#include "pipe.h"
#include<la.h>
#include <iostream>

static const int PIPE_IDX_COUNT = 480;
static const int PIPE_VERT_COUNT = 160;

Pipe::Pipe(GLWidget277 *context) : Drawable(context)
{}

//These are functions that are only defined in this cpp file. They're used for organizational purposes
//when filling the arrays used to hold the vertex and index data.
void createPipeVertexPositions(glm::vec4 (&pipe_vert_pos)[PIPE_VERT_COUNT]){
    //Create two rings of vertices
    //We'll be using 20-sided polygons to approximate circles
    //for the endcaps

    //Larger Ring:
    //Store top cap verts (IDX 0 - 19)
    for(int i = 0; i < 20; i++){
        glm::vec4 v = glm::rotate(glm::mat4(1.0f), glm::radians(i*18.0f), glm::vec3(0, 1, 0)) * glm::vec4(0.5f,0.5f,0,1);
        pipe_vert_pos[i] = v;
    }
    //Store bottom cap verts (IDX 20 - 39)
    for(int i = 20; i < 40; i++){
        glm::vec4 v = glm::rotate(glm::mat4(1.0f), glm::radians((i-20)*18.0f), glm::vec3(0, 1, 0)) * glm::vec4(0.5f,-0.5f,0,1);
        pipe_vert_pos[i] = v;
    }

    //Create two more rings of vertices, this time for the barrel faces
    //We could use the same vertex rings as above, but GL only supports one normal per vertex
    //so we have to have two vertices per position in order to have sharp edges between the endcaps
    //and the barrel

    //Store top cap verts (IDX 40 - 59)
    for(int i = 0; i < 20; i++){
        glm::vec4 v = glm::rotate(glm::mat4(1.0f), glm::radians(i*18.0f), glm::vec3(0, 1, 0)) * glm::vec4(0.5f,0.5f,0,1);
        pipe_vert_pos[i + 40] = v;
    }
    //Store bottom cap verts (IDX 60 - 79)
    for(int i = 20; i < 40; i++){
        glm::vec4 v = glm::rotate(glm::mat4(1.0f), glm::radians((i-20)*18.0f), glm::vec3(0, 1, 0)) * glm::vec4(0.5f,-0.5f,0,1);
        pipe_vert_pos[i + 40] = v;
    }

    //Smaller Ring:
    //Store top cap verts (IDX 0 - 19)
    for(int i = 80; i < 100; i++){
        glm::vec4 v = glm::rotate(glm::mat4(1.0f), glm::radians(i*18.0f), glm::vec3(0, 1, 0)) * glm::vec4(0.3f,0.5f,0,1);
        pipe_vert_pos[i] = v;
    }
    //Store bottom cap verts (IDX 20 - 39)
    for(int i = 100; i < 120; i++){
        glm::vec4 v = glm::rotate(glm::mat4(1.0f), glm::radians((i-20)*18.0f), glm::vec3(0, 1, 0)) * glm::vec4(0.3f,-0.5f,0,1);
        pipe_vert_pos[i] = v;
    }

    //Create two more rings of vertices, this time for the barrel faces
    //We could use the same vertex rings as above, but GL only supports one normal per vertex
    //so we have to have two vertices per position in order to have sharp edges between the endcaps
    //and the barrel

    //Store top cap verts (IDX 40 - 59)
    for(int i = 80; i < 100; i++){
        glm::vec4 v = glm::rotate(glm::mat4(1.0f), glm::radians(i*18.0f), glm::vec3(0, 1, 0)) * glm::vec4(0.3f,0.5f,0,1);
        pipe_vert_pos[i + 40] = v;
    }
    //Store bottom cap verts (IDX 60 - 79)
    for(int i = 100; i < 120; i++){
        glm::vec4 v = glm::rotate(glm::mat4(1.0f), glm::radians((i-20)*18.0f), glm::vec3(0, 1, 0)) * glm::vec4(0.3f,-0.5f,0,1);
        pipe_vert_pos[i + 40] = v;
    }
}


void createPipeVertexNormals(glm::vec4 (&pipe_vert_nor)[PIPE_VERT_COUNT]){
    //Store top cap normals (IDX 0 - 19)
    for(int i = 0; i < 20; i++){
        pipe_vert_nor[i] =  glm::vec4(0,1,0,0);
        pipe_vert_nor[i+80] = glm::vec4(0,1,0,0);
    }
    //Store bottom cap normals (IDX 20 - 39)
    for(int i = 20; i < 40; i++){
        pipe_vert_nor[i] =  glm::vec4(0,-1,0,0);
        pipe_vert_nor[i+80] = glm::vec4(0,-1,0,0);
    }

    //Store top of barrel normals (IDX 40 - 59)
    for(int i = 0; i < 20; i++){
        glm::vec4 v = glm::rotate(glm::mat4(1.0f), glm::radians(i*18.0f), glm::vec3(0, 1, 0)) * glm::vec4(1,0,0,0);
        pipe_vert_nor[i + 40] = v;
        pipe_vert_nor[i + 80] = -v;
    }
    //Store bottom of barrel normals (IDX 60 - 79)
    for(int i = 20; i < 40; i++){
        glm::vec4 v = glm::rotate(glm::mat4(1.0f), glm::radians((i-20)*18.0f), glm::vec3(0, 1, 0)) * glm::vec4(1,0,0,0);
        pipe_vert_nor[i + 40] = v;
        pipe_vert_nor[i + 120] = -v;
    }
}


void createPipeIndices(GLuint (&pipe_idx)[PIPE_IDX_COUNT]){
    //Build indices for the top cap (40 tris, indices 0 - 53, up to vertex 19)
    for(int i = 0; i < 19; i++){
        pipe_idx[i*6] = i;
        pipe_idx[i*6 + 1] = i+1;
        pipe_idx[i*6 + 2] = i+80;
        pipe_idx[i*6 + 3] = i+80;
        pipe_idx[i*6 + 4] = i+1;
        pipe_idx[i*6 + 5] = i+81;
    }
    pipe_idx[114] = 19;
    pipe_idx[115] = 0;
    pipe_idx[116] = 99;
    pipe_idx[117] = 0;
    pipe_idx[118] = 99;
    pipe_idx[119] = 80;

    //Build indices for the bottom cap (40 tris, indices 54 - 107, up to vertex 39)
    for(int i = 20; i < 39; i++){
        pipe_idx[i*6] = i;
        pipe_idx[i*6 + 1] = i+1;
        pipe_idx[i*6 + 2] = i+80;
        pipe_idx[i*6 + 3] = i+80;
        pipe_idx[i*6 + 4] = i+1;
        pipe_idx[i*6 + 5] = i+81;
    }
    pipe_idx[234] = 39;
    pipe_idx[235] = 20;
    pipe_idx[236] = 119;
    pipe_idx[237] = 20;
    pipe_idx[238] = 119;
    pipe_idx[239] = 100;

    //Build indices for the barrel of the Pipe
    for(int i = 0; i < 19; i++){
        pipe_idx[240 + i*6] = i + 40;
        pipe_idx[241 + i*6] = i + 41;
        pipe_idx[242 + i*6] = i + 60;
        pipe_idx[243 + i*6] = i + 41;
        pipe_idx[244 + i*6] = i + 61;
        pipe_idx[245 + i*6] = i + 60;
    }
    //Build the last quad of the barrel, which has looping indices
    pipe_idx[354] = 59;
    pipe_idx[355] = 40;
    pipe_idx[356] = 79;
    pipe_idx[357] = 40;
    pipe_idx[358] = 60;
    pipe_idx[359] = 79;
    //Smaller:
    //Build indices for the barrel of the pipeinder
    for(int i = 0; i < 19; i++){
        pipe_idx[360 + i*6] = i + 120;
        pipe_idx[361 + i*6] = i + 121;
        pipe_idx[362 + i*6] = i + 140;
        pipe_idx[363 + i*6] = i + 121;
        pipe_idx[364 + i*6] = i + 141;
        pipe_idx[365 + i*6] = i + 140;
    }
    //Build the last quad of the barrel, which has looping indices
    pipe_idx[474] = 139;
    pipe_idx[475] = 120;
    pipe_idx[476] = 159;
    pipe_idx[477] = 120;
    pipe_idx[478] = 140;
    pipe_idx[479] = 159;
}

void Pipe::create()
{
    GLuint pipe_idx[PIPE_IDX_COUNT];
    glm::vec4 pipe_vert_pos[PIPE_VERT_COUNT];
    glm::vec4 pipe_vert_nor[PIPE_VERT_COUNT];

    createPipeVertexPositions(pipe_vert_pos);
    createPipeVertexNormals(pipe_vert_nor);
    createPipeIndices(pipe_idx);

    count = PIPE_IDX_COUNT;

    // Create a VBO on our GPU and store its handle in bufIdx
    generateIdx();
    // Tell OpenGL that we want to perform subsequent operations on the VBO referred to by bufIdx
    // and that it will be treated as an element array buffer (since it will contain triangle indices)
    context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
    // Pass the data stored in cyl_idx into the bound buffer, reading a number of bytes equal to
    // CYL_IDX_COUNT multiplied by the size of a GLuint. This data is sent to the GPU to be read by shader programs.
    context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, PIPE_IDX_COUNT * sizeof(GLuint), pipe_idx, GL_STATIC_DRAW);

    // The next few sets of function calls are basically the same as above, except bufPos and bufNor are
    // array buffers rather than element array buffers, as they store vertex attributes like position.
    generatePos();
    context->glBindBuffer(GL_ARRAY_BUFFER, bufPos);
    context->glBufferData(GL_ARRAY_BUFFER, PIPE_VERT_COUNT * sizeof(glm::vec4), pipe_vert_pos, GL_STATIC_DRAW);

    generateNor();
    context->glBindBuffer(GL_ARRAY_BUFFER, bufNor);
    context->glBufferData(GL_ARRAY_BUFFER, PIPE_VERT_COUNT * sizeof(glm::vec4), pipe_vert_nor, GL_STATIC_DRAW);
}
