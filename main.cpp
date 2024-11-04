// Code formatting: 4 space indent, no tabs (spaces only!)
// Opening brace of functions and control structures on end-of-line.
// Might have some other nuances..
// try to check that your git diff plays nicely with original!
#ifdef WIN32
#include <GL/freeglut.h>
#else
#include <GL/glut.h>
#endif
#include <cmath>
#include <iostream>
#include <sstream>
#include <vector>
#include <vecmath.h>
#include <mesh.h>
using namespace std;

// Globals
Mesh mesh{"./garg.obj"};
// This is the list of points (3D vectors)
vector<Vector3f> vecv;

// This is the list of normals (also 3D vectors)
vector<Vector3f> vecn;

// This is the list of faces (indices into vecv and vecn)
vector<vector<unsigned> > vecf;
//Color index
int COLOR_INDEX = 0;
GLfloat COLOR_CASE[4][4] = { 
    {0.5, 0.5, 0.9, 1.0},
    {0.9, 0.5, 0.5, 1.0},
    {0.5, 0.9, 0.3, 1.0},
    {0.3, 0.8, 0.9, 1.0} 
};
int ANGLE = 0;
bool rotate_flag = false;
// Light position
float x_offset = 0;
float y_offset = 0;
// You will need more global variables to implement color and position changes


// These are convenience functions which allow us to call OpenGL 
// methods on Vec3d objects
inline void glVertex(const Vector3f& a) { glVertex3fv(a); }

inline void glNormal(const Vector3f& a) { glNormal3fv(a); }


// This function is called whenever a "Normal" key press is received.
void keyboardFunc(unsigned char key, int x, int y) {
    switch (key) {
    case 27: // Escape key
        exit(0);
        break;
    case 'c':{
        // add code to change color here
        cout << "Change Color " << key << "." << endl;
        ++COLOR_INDEX;COLOR_INDEX%=4;
        if(!rotate_flag)glutPostRedisplay();
        break;
    }
    case 'r':{
        cout << "Change Color " << key << "." << endl;
        rotate_flag = rotate_flag^1;
        if(rotate_flag)glutPostRedisplay();
        break;
    }
    default:
        cout << "Unhandled key press " << key << "." << endl;
    }
    // this will refresh the screen so that the user sees the color change
    //drawScene on next frame
}

// This function is called whenever a "Special" key press is received.
// Right now, it's handling the arrow keys.
void specialFunc(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_UP:
        // add code to change light position
        cout << "up arrow." << endl;
        y_offset+=(0.5f);
        break;
    case GLUT_KEY_DOWN:
        // add code to change light position
        cout << "down arrow." << endl;
        y_offset-=(0.5f);
        break;
    case GLUT_KEY_LEFT:
        // add code to change light position
        cout << "left arrow." << endl;
        x_offset-=(0.5f);
        break;
    case GLUT_KEY_RIGHT:
        // add code to change light position
        cout << "right arrow." << endl;
        x_offset+=(0.5f);
        break;
    }

    // this will refresh the screen so that the user sees the light position
    glutPostRedisplay();
}

void update_rotation_angle(int value){
    ANGLE+=value;
    if(ANGLE>=360)ANGLE%=360;
}

void enqueue_draw_call(int val){
    std::cout<<"enqueued Draw \n";
    update_rotation_angle(val);
    glutPostRedisplay();
}

// This function is responsible for displaying the object.
void drawScene(void) {
    int i;

    // Clear the rendering window
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Rotate the image
    glMatrixMode(GL_MODELVIEW);  // Current matrix affects objects positions
    glLoadIdentity();              // Initialize to the identity

    // Position the camera at [0,0,5], looking at [0,0,0],
    // with [0,1,0] as the up direction.
    gluLookAt(0.0, 0.0, 5.0,
              0.0, 0.0, 0.0,
              0.0, 1.0, 0.0);
    
    // Set material properties of object
    // Here we use the first color entry as the diffuse color
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, COLOR_CASE[COLOR_INDEX]);

    // Define specular color and shininess
    GLfloat specColor[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat shininess[] = { 100.0 };

    // Note that the specular color and shininess can stay constant
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specColor);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

    // Set light properties

    // Light color (RGBA)
    GLfloat Lt0diff[] = { 1.0,1.0,1.0,1.0 };
    GLfloat Lt0pos[] = { 1.0f + x_offset, 1.0f + y_offset, 5.0f, 1.0f };

    glLightfv(GL_LIGHT0, GL_DIFFUSE, Lt0diff);
    glLightfv(GL_LIGHT0, GL_POSITION, Lt0pos);

    // This GLUT method draws a teapot.  You should replace
    // it with code which draws the object you loaded.
    //save model view matrix
    glPushMatrix();
    //add rotation transformation
    glRotatef(ANGLE,0.0f,1.0f,0.0f);
    //draw
    mesh.draw_obj();
    //restore old modelview matrix
    glPopMatrix();
    // Dump the image to the screen.
    glutSwapBuffers();
    if(rotate_flag){
        //register callback for rotation
        glutTimerFunc(100,enqueue_draw_call,10);
    }
}

// Initialize OpenGL's rendering modes
void initRendering() {
    glEnable(GL_DEPTH_TEST);   // Depth testing must be turned on
    glEnable(GL_LIGHTING);     // Enable lighting calculations
    glEnable(GL_LIGHT0);       // Turn on light #0.
}

// Called when the window is resized
// w, h - width and height of the window in pixels.
void reshapeFunc(int w, int h) {
    // Always use the largest square viewport possible
    if (w > h) {
        glViewport((w - h) / 2, 0, h, h);
    }
    else {
        glViewport(0, (h - w) / 2, w, w);
    }

    // Set up a perspective view, with square aspect ratio
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // 50 degree fov, uniform aspect ratio, near = 1, far = 100
    gluPerspective(50.0, 1.0, 1.0, 100.0);
}

void loadInput() {
    // load the OBJ file here
    mesh.load_obj();
}

// Main routine.
// Set up OpenGL, define the callbacks and start the main loop
int main(int argc, char** argv) {
    loadInput();

    glutInit(&argc, argv);

    // We're going to animate it, so double buffer 
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    // Initial parameters for window position and size
    glutInitWindowPosition(60, 60);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Assignment 0");

    // Initialize OpenGL parameters.
    initRendering();

    // Set up callback functions for key presses
    glutKeyboardFunc(keyboardFunc); // Handles "normal" ascii symbols
    glutSpecialFunc(specialFunc);   // Handles "special" keyboard keys

     // Set up the callback function for resizing windows
    glutReshapeFunc(reshapeFunc);

    // Call this whenever window needs redrawing
    glutDisplayFunc(drawScene);

    // Start the main loop.  glutMainLoop never returns.
    glutMainLoop();

    return 0;	// This line is never reached.
}
