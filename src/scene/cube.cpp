#include "cube.h"
#include<la.h>
#include <iostream>

static const int CUBE_IDX_COUNT = 36;
static const int CUBE_VERT_COUNT = 24;

Cube::Cube(GLWidget277 *context) : Drawable(context)
{}


void createCubeVertexPositions(glm::vec4 (&cube_vert_pos)[CUBE_VERT_COUNT]){
    //top surface 0~3
    glm::vec4 v0(-0.5f,-0.5f,0.5f,1), v1(0.5f,-0.5f,0.5f,1), v2(0.5f,0.5f,0.5f,1), v3(-0.5f,0.5f,0.5f,1);
    cube_vert_pos[0] = v0;
    cube_vert_pos[1] = v1;
    cube_vert_pos[2] = v2;
    cube_vert_pos[3] = v3;
    //bottom surface 4~7
    glm::vec4 v4(-0.5f,-0.5f,-0.5f,1), v5(0.5f,-0.5f,-0.5f,1), v6(0.5f,0.5f,-0.5f,1), v7(-0.5f,0.5f,-0.5f,1);
    cube_vert_pos[4] = v4;
    cube_vert_pos[5] = v5;
    cube_vert_pos[6] = v6;
    cube_vert_pos[7] = v7;
    //front surface 8~11
    glm::vec4 v8(-0.5f,-0.5f,-0.5f,1), v9(0.5f,-0.5f,-0.5f,1), v10(0.5f,-0.5f,0.5f,1), v11(-0.5f,-0.5f,0.5f,1);
    cube_vert_pos[8] = v8;
    cube_vert_pos[9] = v9;
    cube_vert_pos[10] = v10;
    cube_vert_pos[11] = v11;
    //back surface 12~15
    glm::vec4 v12(-0.5f,0.5f,-0.5f,1), v13(0.5f,0.5f,-0.5f,1), v14(0.5f,0.5f,0.5f,1), v15(-0.5f,0.5f,0.5f,1);
    cube_vert_pos[12] = v12;
    cube_vert_pos[13] = v13;
    cube_vert_pos[14] = v14;
    cube_vert_pos[15] = v15;
    //left surface 16~19
    glm::vec4 v16(-0.5f,-0.5f,-0.5f,1), v17(-0.5f,-0.5f,0.5f,1), v18(-0.5f,0.5f,0.5f,1), v19(-0.5f,0.5f,-0.5f,1);
    cube_vert_pos[16] = v16;
    cube_vert_pos[17] = v17;
    cube_vert_pos[18] = v18;
    cube_vert_pos[19] = v19;
    //right surface 20~23
    glm::vec4 v20(0.5f,-0.5f,-0.5f,1), v21(0.5f,-0.5f,0.5f,1), v22(0.5f,0.5f,0.5f,1), v23(0.5f,0.5f,-0.5f,1);
    cube_vert_pos[20] = v20;
    cube_vert_pos[21] = v21;
    cube_vert_pos[22] = v22;
    cube_vert_pos[23] = v23;
}

void createCubeVertexNormals(glm::vec4 (&cube_vert_nor)[CUBE_VERT_COUNT]){
    //top
    for(int i = 0; i<3; i++){
        cube_vert_nor[i] = glm::vec4(0,0,1,0);
    }
    //bottom
    for(int i = 4; i<7; i++){
        cube_vert_nor[i] = glm::vec4(0,0,-1,0);
    }
    //front
    for(int i = 8; i<11; i++){
        cube_vert_nor[i] = glm::vec4(-1,0,0,0);
    }
    //back
    for(int i = 12; i<15; i++){
        cube_vert_nor[i] = glm::vec4(1,0,0,0);
    }
    //left
    for(int i = 16; i<19; i++){
        cube_vert_nor[i] = glm::vec4(0,-1,0,0);
    }
    //right
    for(int i = 20; i<23; i++){
        cube_vert_nor[i] = glm::vec4(0,1,0,0);
    }
}

void createCubeIndices(GLuint (&cube_idx)[CUBE_IDX_COUNT]){
    //top 0~5
    for(int i = 0; i<2; i++){
        cube_idx[i*3] = 0;
        cube_idx[i*3+1] = i+1;
        cube_idx[i*3+2] = i+2;
    }
    //bottom 6~11
    for(int i = 4; i<6; i++){
        cube_idx[(i-2)*3] = 4;
        cube_idx[(i-2)*3+1] = i+1;
        cube_idx[(i-2)*3+2] = i+2;
    }
    //front 12~17
    for(int i = 8; i<10; i++){
        cube_idx[(i-4)*3] = 8;
        cube_idx[(i-4)*3+1] = i+1;
        cube_idx[(i-4)*3+2] = i+2;
    }
    //back 18~23
    for(int i = 12; i<14; i++){
        cube_idx[(i-6)*3] = 12;
        cube_idx[(i-6)*3+1] = i+1;
        cube_idx[(i-6)*3+2] = i+2;
    }
    //left 24~29
    for(int i = 16; i<18; i++){
        cube_idx[(i-8)*3] = 16;
        cube_idx[(i-8)*3+1] = i+1;
        cube_idx[(i-8)*3+2] = i+2;
    }
    //right 30~35
    for(int i = 20; i<22; i++){
        cube_idx[(i-10)*3] = 20;
        cube_idx[(i-10)*3+1] = i+1;
        cube_idx[(i-10)*3+2] = i+2;
    }
}

void Cube::create(){
    GLuint cube_idx[CUBE_IDX_COUNT];
    glm::vec4 cube_vert_pos[CUBE_VERT_COUNT];
    glm::vec4 cube_vert_nor[CUBE_VERT_COUNT];

    createCubeVertexPositions(cube_vert_pos);
    createCubeVertexNormals(cube_vert_nor);
    createCubeIndices(cube_idx);

    count = CUBE_IDX_COUNT;

    // Create a VBO on our GPU and store its handle in bufIdx
    generateIdx();
    // Tell OpenGL that we want to perform subsequent operations on the VBO referred to by bufIdx
    // and that it will be treated as an element array buffer (since it will contain triangle indices)
    context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
    // Pass the data stored in cyl_idx into the bound buffer, reading a number of bytes equal to
    // CYL_IDX_COUNT multiplied by the size of a GLuint. This data is sent to the GPU to be read by shader programs.
    context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, CUBE_IDX_COUNT * sizeof(GLuint), cube_idx, GL_STATIC_DRAW);

    // The next few sets of function calls are basically the same as above, except bufPos and bufNor are
    // array buffers rather than element array buffers, as they store vertex attributes like position.
    generatePos();
    context->glBindBuffer(GL_ARRAY_BUFFER, bufPos);
    context->glBufferData(GL_ARRAY_BUFFER, CUBE_VERT_COUNT * sizeof(glm::vec4), cube_vert_pos, GL_STATIC_DRAW);

    generateNor();
    context->glBindBuffer(GL_ARRAY_BUFFER, bufNor);
    context->glBufferData(GL_ARRAY_BUFFER, CUBE_VERT_COUNT * sizeof(glm::vec4), cube_vert_nor, GL_STATIC_DRAW);
}
