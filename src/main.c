#include "main.h"
#include "data.h"

int main ( int argc, char *argv[] ) {

  if ( argc != 2 ) { // argc should be 2 for correct execution
    // We print argv[0] assuming it is the program name
    qDebug() <<"usage: "<< argv[0] <<" <filename>";
    exit(EXIT_FAILURE);
  }
  qDebug() << "\nStarting BarnesHut GPU with OpenGL output ";
  qDebug() << "-----------------------------------------";

  QTextStream cout(stdout, QIODevice::WriteOnly); // Standard output

  //CUT_DEVICE_INIT();
  QString filename(argv[1]);
  qDebug() << "Reading" << filename;

  readFile(filename);

  timer.start();

  run(argc,argv);
}
void run(int argc, char** argv) {
//  cudaGLSetGLDevice(0);

  glutInit(&argc,argv);

  glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE);
  glutInitWindowSize( window_width, window_height);
  glutCreateWindow( "BarnesHut Test");

  // initialize GL
   if( CUTFalse == initGL()) {
       return;
   }

   // Set Camera
   camera_position.x = -10.0f;
   camera_position.y = -10.0f;
   camera_position.z = 10.0f;

   camera_direction.x = -camera_position.x;
   camera_direction.y = -camera_position.y;
   camera_direction.z = -camera_position.z;
   normalize3(&camera_direction);

   camera_up.x = 0.0;
   camera_up.y = 1.0;
   camera_up.z = 0.0;

//   // register functions for glut
   glutDisplayFunc(display);
   glutKeyboardFunc(keyboard);
   glutMouseFunc(mouse);
   glutMotionFunc(motion);

   // create VBO

  glGenBuffers(1, &positionsVBO);
  glBindBuffer(GL_ARRAY_BUFFER, positionsVBO);
  unsigned int size = barneshut->getElements() * sizeof(float4);
  glBufferData(GL_ARRAY_BUFFER, size, 0, GL_DYNAMIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  //cudaGraphicsGLRegisterBuffer(&positionsVBO_CUDA, positionsVBO, cudaGraphicsMapFlagsWriteDiscard);
  CUDA_SAFE_CALL(cudaGLRegisterBufferObject(positionsVBO));
  CUT_CHECK_ERROR_GL();
  glutMainLoop();
}

void runTimestep() {
  qDebug() << "Running new timestep";
  barneshut->runComputation();
  barneshut->computeNextStep();

}

void display() {
  // Our input
  int elements = barneshut->getElements();

  timer.restart();
  if (!pauseFlag) {
    float4 *d_particles = barneshut->getCudaParticlePointer();
    float *d_opengl;
    CUDA_SAFE_CALL(cudaGLMapBufferObject( (void**)&d_opengl, positionsVBO));
    runTimestep();
    CUDA_SAFE_CALL(cudaMemcpy(d_opengl,d_particles,elements*sizeof(float4),cudaMemcpyDeviceToDevice));
    CUDA_SAFE_CALL(cudaGLUnmapBufferObject(positionsVBO));
  }
  long int elapsedTime = timer.elapsed();

  // Render from buffer object

  // Render from buffer object
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // set view matrix
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
//   glTranslatef(0.0, 0.0, -5.0);
   glRotatef(rotate_x, 1.0, 0.0, 0.0);
   glRotatef(rotate_y, 0.0, 1.0, 0.0);
   glTranslated(-posX,-posY,-posZ);

   // Render
  glBindBuffer(GL_ARRAY_BUFFER, positionsVBO);
  glVertexPointer(4, GL_FLOAT, 0, 0);
  glEnableClientState(GL_VERTEX_ARRAY);
  glDrawArrays(GL_POINTS, 0, elements);
  glDisableClientState(GL_VERTEX_ARRAY);
  // Swap buffers
  showFPS(1000.0/((float)elapsedTime));

  glutSwapBuffers();
  glutPostRedisplay();
  qDebug() << "rX:"<<rotate_x << "ry:"<<rotate_y <<"pX:" << posX << "pY:" <<posY << "pZ:" << posZ;

}

CUTBoolean initGL() {
  // initialize necessary OpenGL extensions
  glewInit();
  if (! glewIsSupported( "GL_VERSION_2_0 "
      "GL_ARB_pixel_buffer_object"
  )) {
      fprintf( stderr, "ERROR: Support for necessary OpenGL extensions missing.");
      fflush( stderr);
      return CUTFalse;
  }

  // default initialization
  glClearColor( 0.0, 0.0, 0.0, 0.0);
  glDisable( GL_DEPTH_TEST);

  // viewport
  glViewport( 0, 0, window_width, window_height);

  // projection
  glMatrixMode( GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60.0, (GLfloat)window_width / (GLfloat) window_height, 0.1, 100000.0);

  CUT_CHECK_ERROR_GL();
  return CUTTrue;
}

void showFPS(float fps) {
//  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glTranslatef(-1.6,-0.9,-2);
  //  gluOrtho2D(0,window_width,0,window_height);
//  glMatrixMode(GL_MODELVIEW);

  float x = 0.0;
  float y = 0.0;
  float z = 0.0;
  QString text = "FPS: ";
  text.append(QString::number(fps));

  glRasterPos2i (x, y);

  for (int i=0;text.toLocal8Bit().data()[i] != '\0'; i++) {
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10,text.toLocal8Bit().data()[i]);
  }

//  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
//  glMatrixMode(GL_MODELVIEW);
}

// Event Handlers
void keyboard( unsigned char keyIn, int x, int y) {
    float xrotrad, yrotrad;

    unsigned char key = keyIn;
    switch(keyIn) {
      case(GLUT_KEY_UP):
        key = 'w';
        break;
      case(GLUT_KEY_DOWN):
        key = 's';
        break;
      case(GLUT_KEY_RIGHT):
        key = 'd';
        break;
      case(GLUT_KEY_LEFT):
        key = 'a';
        break;
    }

    switch(key) {
//    case( 27) :
//        deleteVBO( &positionsVBO);
//        exit( 0);
      case('p'):
        qDebug() << "Pause flag";
        if (!pauseFlag)
          pauseFlag = 1;
        else
          pauseFlag = 0;
        break;
      case('w'):
        yrotrad = (rotate_y / 180 * 3.141592654f);
        xrotrad = (rotate_x / 180 * 3.141592654f);
        posX += (sin(yrotrad))*movingFactor;
        posZ -= (cos(yrotrad))*movingFactor;
        posY -= (sin(xrotrad))*movingFactor;
        break;
      case('s'):
        yrotrad = (rotate_y / 180 * 3.141592654f);
        xrotrad = (rotate_x / 180 * 3.141592654f);
        posX -= (sin(yrotrad))*movingFactor;
        posZ += (cos(yrotrad))*movingFactor;
        posY += (sin(xrotrad))*movingFactor;
        break;
      case('d'):
        yrotrad = (rotate_y / 180 * 3.141592654f);
        posX += (cos(yrotrad)) * 0.2*movingFactor;
        posZ += (sin(yrotrad)) * 0.2*movingFactor;
        break;
      case('a'):
        yrotrad = (rotate_y / 180 * 3.141592654f);
        posX -= (cos(yrotrad)) * 0.2*movingFactor;
        posZ -= (sin(yrotrad)) * 0.2*movingFactor;
    }
}

void mouse(int button, int state, int x, int y) {
    if (state == GLUT_DOWN) {
        mouse_buttons |= 1<<button;
    } else if (state == GLUT_UP) {
        mouse_buttons = 0;
    }

    mouse_old_x = x;
    mouse_old_y = y;
    glutPostRedisplay();
}

void motion(int x, int y) {
    float dx, dy;
    dx = x - mouse_old_x;
    dy = y - mouse_old_y;

    if (mouse_buttons & 1) {
        rotate_x += dy * 0.2;
        rotate_y += dx * 0.2;
    } else if (mouse_buttons & 4) {
        posZ += dy * 0.01;
    }

    mouse_old_x = x;
    mouse_old_y = y;
}
