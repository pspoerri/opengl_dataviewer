#ifndef MAIN_H
#define MAIN_H

#include "opengl.h"
// QT
#include <QDebug>
#include <QString>
#include <QTime>

#include <cmath>
// Data import and export
#include "data.h"

////////////////////////////////////////////////////////
// Constants for the display

// vbo variables
GLuint positionsVBO;

float anim = 0.0;
int pauseFlag = 0;
int frames =0;

QTime timer;

// mouse controls
int mouse_old_x, mouse_old_y;
int mouse_buttons = 0;
float rotate_x = -37.4, rotate_y = -231.399;
float posX = -10;
float posY = -10;
float posZ = 0.0;
float movingFactor = 10;
////////////////////////////////////////////////////////
// Declarations
float3 camera_position;
float3 camera_direction;
float3 camera_up;

// OpenGL related
CUTBoolean initGL();
void createVBO( GLuint* vbo);
void deleteVBO( GLuint* vbo);

// Callbacks
void display();
void keyboard( unsigned char key, int x, int y);
void mouse(int button, int state, int x, int y);
void motion(int x, int y);

// Runtime
void showFPS(float fps);
void runTimestep();
void run(int argc, char** argv);

///////////////////////////////////////////////////////
// Vector functions
void normalize3(float3* vec) {
  float norm = sqrt(vec->x*vec->x + vec->y*vec->y + vec->z*vec->z);
  vec->x = vec->x/norm;
  vec->y = vec->y/norm;
  vec->z = vec->z/norm;
}

#endif // MAIN_H
