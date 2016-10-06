#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <vector>
#include <array>
#include <chrono>
#include <iostream>
#define WINDOW_TITLE_PREFIX "Sfere"

static const double radius = 0.1;
static const int subs = 32;
static const int CurrentWidth = 800, CurrentHeight = 600;

std::chrono::high_resolution_clock::time_point last_frame_time;

std::vector<std::array<double, 3>> spheres;
int WindowHandle = 0;
int mouse_x = -1, mouse_y = -1;

GLdouble perspective_matrix[16];

bool gluInvertMatrix(const double m[16], double invOut[16])
{
    double inv[16], det;
    int i;

    inv[0] = m[5]  * m[10] * m[15] - 
             m[5]  * m[11] * m[14] - 
             m[9]  * m[6]  * m[15] + 
             m[9]  * m[7]  * m[14] +
             m[13] * m[6]  * m[11] - 
             m[13] * m[7]  * m[10];

    inv[4] = -m[4]  * m[10] * m[15] + 
              m[4]  * m[11] * m[14] + 
              m[8]  * m[6]  * m[15] - 
              m[8]  * m[7]  * m[14] - 
              m[12] * m[6]  * m[11] + 
              m[12] * m[7]  * m[10];

    inv[8] = m[4]  * m[9] * m[15] - 
             m[4]  * m[11] * m[13] - 
             m[8]  * m[5] * m[15] + 
             m[8]  * m[7] * m[13] + 
             m[12] * m[5] * m[11] - 
             m[12] * m[7] * m[9];

    inv[12] = -m[4]  * m[9] * m[14] + 
               m[4]  * m[10] * m[13] +
               m[8]  * m[5] * m[14] - 
               m[8]  * m[6] * m[13] - 
               m[12] * m[5] * m[10] + 
               m[12] * m[6] * m[9];

    inv[1] = -m[1]  * m[10] * m[15] + 
              m[1]  * m[11] * m[14] + 
              m[9]  * m[2] * m[15] - 
              m[9]  * m[3] * m[14] - 
              m[13] * m[2] * m[11] + 
              m[13] * m[3] * m[10];

    inv[5] = m[0]  * m[10] * m[15] - 
             m[0]  * m[11] * m[14] - 
             m[8]  * m[2] * m[15] + 
             m[8]  * m[3] * m[14] + 
             m[12] * m[2] * m[11] - 
             m[12] * m[3] * m[10];

    inv[9] = -m[0]  * m[9] * m[15] + 
              m[0]  * m[11] * m[13] + 
              m[8]  * m[1] * m[15] - 
              m[8]  * m[3] * m[13] - 
              m[12] * m[1] * m[11] + 
              m[12] * m[3] * m[9];

    inv[13] = m[0]  * m[9] * m[14] - 
              m[0]  * m[10] * m[13] - 
              m[8]  * m[1] * m[14] + 
              m[8]  * m[2] * m[13] + 
              m[12] * m[1] * m[10] - 
              m[12] * m[2] * m[9];

    inv[2] = m[1]  * m[6] * m[15] - 
             m[1]  * m[7] * m[14] - 
             m[5]  * m[2] * m[15] + 
             m[5]  * m[3] * m[14] + 
             m[13] * m[2] * m[7] - 
             m[13] * m[3] * m[6];

    inv[6] = -m[0]  * m[6] * m[15] + 
              m[0]  * m[7] * m[14] + 
              m[4]  * m[2] * m[15] - 
              m[4]  * m[3] * m[14] - 
              m[12] * m[2] * m[7] + 
              m[12] * m[3] * m[6];

    inv[10] = m[0]  * m[5] * m[15] - 
              m[0]  * m[7] * m[13] - 
              m[4]  * m[1] * m[15] + 
              m[4]  * m[3] * m[13] + 
              m[12] * m[1] * m[7] - 
              m[12] * m[3] * m[5];

    inv[14] = -m[0]  * m[5] * m[14] + 
               m[0]  * m[6] * m[13] + 
               m[4]  * m[1] * m[14] - 
               m[4]  * m[2] * m[13] - 
               m[12] * m[1] * m[6] + 
               m[12] * m[2] * m[5];

    inv[3] = -m[1] * m[6] * m[11] + 
              m[1] * m[7] * m[10] + 
              m[5] * m[2] * m[11] - 
              m[5] * m[3] * m[10] - 
              m[9] * m[2] * m[7] + 
              m[9] * m[3] * m[6];

    inv[7] = m[0] * m[6] * m[11] - 
             m[0] * m[7] * m[10] - 
             m[4] * m[2] * m[11] + 
             m[4] * m[3] * m[10] + 
             m[8] * m[2] * m[7] - 
             m[8] * m[3] * m[6];

    inv[11] = -m[0] * m[5] * m[11] + 
               m[0] * m[7] * m[9] + 
               m[4] * m[1] * m[11] - 
               m[4] * m[3] * m[9] - 
               m[8] * m[1] * m[7] + 
               m[8] * m[3] * m[5];

    inv[15] = m[0] * m[5] * m[10] - 
              m[0] * m[6] * m[9] - 
              m[4] * m[1] * m[10] + 
              m[4] * m[2] * m[9] + 
              m[8] * m[1] * m[6] - 
              m[8] * m[2] * m[5];

    det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

    if (det == 0)
        return false;

    det = 1.0 / det;

    for (i = 0; i < 16; i++)
        invOut[i] = inv[i] * det;

    return true;
}

void draw_sphere(std::array<double, 3> sphere) {
    glPushMatrix();
    glTranslatef(sphere[0], sphere[1], sphere[2]);
    glutSolidSphere(radius, subs, subs);
    glPopMatrix();
}

void add_sphere() {
    double sx, sy, sz;
    sx = (rand()%60-30)*0.1;
    sy = (rand()%40-20)*0.1;
    sz = -(rand()%100-50)*0.1;
    fprintf(stderr, "Add sphere at (%f, %f, %f)\n", sx, sy, sz);
    spheres.push_back({sx, sy, sz});
}

void ResizeFunction(int Width, int Height){
    if (Width != CurrentWidth || Height != CurrentHeight) {
        fprintf(stderr, "Resize to %dx%d not allowed\n", Width, Height);
        glutReshapeWindow(CurrentWidth, CurrentHeight);
        return;
    }
    fprintf(stderr, "window size %dx%d\n", Width, Height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(39, 1.0*CurrentWidth/CurrentHeight, 0.6, 10);
    glGetDoublev(GL_PROJECTION_MATRIX, perspective_matrix);
	glMatrixMode(GL_MODELVIEW);
	glViewport(0, 0, CurrentWidth, CurrentHeight);
}

void RenderFunction(void){
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.5f, 0.0f, 1.0f);

    auto frame_time = std::chrono::high_resolution_clock::now();
    uint64_t frame_microseconds = std::chrono::duration_cast<std::chrono::microseconds>(frame_time - last_frame_time).count();
    last_frame_time = frame_time;

    double mouse_relx = mouse_x - CurrentWidth/2;
    double mouse_rely = -mouse_y + CurrentHeight/2;
    double mouse_power = sqrt(mouse_relx*mouse_relx + mouse_rely*mouse_rely);
    double window_diag = sqrt(CurrentWidth*CurrentWidth+CurrentHeight*CurrentHeight);

    if (mouse_x != -1 && mouse_power > window_diag/10.0) {
        // double mouse_angle = atan2(mouse_relx, mouse_rely)/M_PI*360;
        mouse_power = mouse_power/16000000*frame_microseconds;
        glMatrixMode(GL_PROJECTION);
        // glRotated(-mouse_power, 0, 1, 0);
        
        GLdouble axis[4] = {mouse_rely, -mouse_relx, 0, 0};
        GLdouble current_matrix[16];
        glGetDoublev(GL_PROJECTION_MATRIX, current_matrix);
        GLdouble inverse_matrix[16];
        // glm::inverse(current_matrix);
        gluInvertMatrix(current_matrix, inverse_matrix);
        
        /*for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                std::cout << current_matrix[4*i+j] << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;*/
        
        GLdouble temp[4] = {0, 0, 0, 0};
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                temp[j] += perspective_matrix[4*j+i]*axis[i];
            }
        }
        GLdouble absolute_axis[4] = {0, 0, 0, 0};
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                absolute_axis[j] += inverse_matrix[4*j+i]*temp[i];
            }
        }
        glRotated(-mouse_power, absolute_axis[0], absolute_axis[1], absolute_axis[2]);
    }

    glMatrixMode(GL_MODELVIEW);
    // glLoadIdentity();
    for (auto sphere: spheres) draw_sphere(sphere);
    glFlush();
	glutSwapBuffers();
	glutPostRedisplay();
}

void Keyboard (unsigned char key, int x, int y){
    if (key == 'a')
        add_sphere();
}

void PassiveMouse(int x, int y) {
    mouse_x = x;
    mouse_y = y;
}

void InitWindow(int argc, char* argv[]){
	glutInit(&argc, argv);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE,GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	glutInitWindowSize(CurrentWidth, CurrentHeight);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	WindowHandle = glutCreateWindow(WINDOW_TITLE_PREFIX);
	if (WindowHandle < 1) {
		fprintf(stderr,"ERROR: Could not create a new rendering window.\n");
		exit(EXIT_FAILURE);
	}
	glutReshapeFunc(ResizeFunction);
	glutDisplayFunc(RenderFunction);
	glutKeyboardFunc(Keyboard);
	glutPassiveMotionFunc(PassiveMouse);
}

void Initialize(int argc, char* argv[]){
	InitWindow(argc, argv);
	fprintf(stderr, "INFO: OpenGL Version: %s\n",glGetString(GL_VERSION));
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

int main(int argc, char* argv[]){
	Initialize(argc, argv);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	spheres.push_back({0.0, 0.0, -4.0});
	for (int i=0; i<40; i++) add_sphere();
	last_frame_time = std::chrono::high_resolution_clock::now();
	glutMainLoop();
	return 0;
}
