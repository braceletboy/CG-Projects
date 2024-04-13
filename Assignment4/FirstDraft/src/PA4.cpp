#include "openGLPart.h"

Point snakeBody[MAX_SEGMENTS];
int bodySegments;
GLfloat randomColors[300000];
int lats = 40, longs = 400;
bool panning = false;
double clickedX, clickedY;
ColorPattern color = SOLID;
GLfloat eye[3] = {0.0, 0.0, EYE_Z};
GLfloat dims[4] = {-X_MAX, X_MAX, -Y_MAX, Y_MAX};
GLfloat up[3] = {0.0, 1.0, 0.0};
GLfloat xTheta = 0.0;
GLfloat yTheta = 0.0;
GLfloat zTheta = 0.0;


static void init(void)
{
    glClearColor(1, 1, 1, 1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(dims[0], dims[1], dims[2], dims[3], 0.0, 20.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(
        eye[0], eye[1], eye[2], //< eye position
        0.0f, 0.0f, 0.0f,  //< aim position
        up[0], up[1], up[2] //< up direction
    );

    for (int i = 0; i < 300000;i++)
        randomColors[i] = (double)rand() / RAND_MAX;
    bodySegments = 15;
    snakeBody[0] = { -0.1,0,0 };
    snakeBody[1] = {-0.5, 0, 0 };
    snakeBody[2] = { -0.5,0, 0.5 };
    snakeBody[3] = { -0.5,0.5, 0.5 };
    snakeBody[4] = { 0,0.5, 0.5 };
    snakeBody[5] = { 0,0, 0.5 };
    snakeBody[6] = { 0,0, 0 };
    snakeBody[7] = { 0,0.5, 0 };
    snakeBody[8] = { -0.7,0.5, 0 };
    snakeBody[9] = { -0.7,-0.4, 0 };
    snakeBody[10] = { 0,-0.4, 0 };
    snakeBody[11] = { 0,-0.27, 0 };
    snakeBody[12] = { -0.6,-.27, 0 };
    snakeBody[13] = { -0.6,-.27, 1};
    snakeBody[14] = { -0.6,0, 1 };
    snakeBody[15] = { 0,0, 1 };
}
Point diff(Point p2, Point p1) {
    Point result = { p2.x - p1.x, p2.y - p1.y, p2.z - p1.z };
    return result;
}

void snake(double r, ColorPattern color) {
    Point d, nextD;
    double len;
    for (int i = 1; i <= bodySegments;i++) {
        glPushMatrix();
        glTranslatef(snakeBody[i - 1].x, snakeBody[i - 1].y, snakeBody[i - 1].z);
        d = diff(snakeBody[i], snakeBody[i - 1]);
        if (d.y != 0) {
            len = d.y;
            glRotatef(90, 0, 0, 1);
        }
        else if (d.z != 0) {
            len = d.z;
            glRotatef(-90, 0, 1, 0);
        }
        else
            len = d.x;
        if (abs(len) >= 4 * r) {
            double temp = abs(len) - 4 * r+r/10;
            glTranslatef(len > 0 ? 1.95 * r : -temp - 1.95 * r, 0, 0);
            cylinder(lats, longs*temp, r, temp, color);
            glTranslatef(len > 0 ? temp : 0, 0, 0);
        }
        if (i == bodySegments)
        {
            //draw the head here!
        }
        else {//turn
            nextD = diff(snakeBody[i + 1], snakeBody[i]);
            if (d.x == 0 && nextD.x == 0) {//in y-z plane
                if (d.z == 0) {//y-->z
                    glTranslatef(0, 0, nextD.z > 0 ? 2 * r : -2 * r);
                    glRotatef(90, 1, 0, 0);
                    double torusAngle = nextD.z > 0 ? -M_PI / 2 : M_PI / 2;
                    torus(lats, longs * M_PI * r, torusAngle, d.y > 0 ? 0 : 2 * torusAngle, r, 2 * r, color);
                }
                else {//z-->y
                    glTranslatef(0, nextD.y > 0 ? 2 * r : -2 * r, 0);
                    glRotatef(-90, 0, 0, 1);
                    double torusAngle = d.z > 0 ? M_PI / 2 : -M_PI / 2;
                    torus(lats, longs * M_PI * r, torusAngle, nextD.y > 0 ? 0 : 2 * torusAngle, r, 2 * r, color);
                }
            }
            else if (d.y == 0 && nextD.y == 0) {//in z-x plane 
                if (d.x == 0) {//z-->x
                    glTranslatef(0, 0, nextD.x > 0 ? -2 * r : 2 * r);
                    glRotatef(-90,1, 0, 0);
                    double torusAngle = nextD.x > 0 ? -M_PI / 2 : M_PI / 2;
                    torus(lats, longs * M_PI * r, torusAngle, d.z > 0 ? 0 : 2 * torusAngle, r, 2 * r, color);
                }
                else {//x-->z 
                    glTranslatef(0, 0, nextD.z > 0 ? 2 * r : -2 * r);
                    glRotatef(-90, 1, 0, 0);
                    double torusAngle = nextD.z > 0 ? M_PI / 2 : -M_PI / 2;
                    torus(lats, longs * M_PI * r, torusAngle, d.x > 0 ? 0 : 2 * torusAngle, r, 2 * r, color);
                }
            }
            else {//in x-y plane
                if (d.y == 0) {//x-->y
                    glTranslatef(0, nextD.y > 0 ? 2 * r : -2 * r, 0);
                    double torusAngle = nextD.y > 0 ? -M_PI / 2 : M_PI / 2;
                    torus(lats, longs * M_PI * r, torusAngle, d.x > 0 ? 0 : 2 * torusAngle, r, 2 * r, color);
                }
                else {//y-->x
                    glTranslatef(0, nextD.x > 0 ? -2 * r : 2 * r, 0);
                    double torusAngle = nextD.x > 0 ? M_PI / 2 : -M_PI / 2;
                    torus(lats, longs * M_PI * r, torusAngle, d.y > 0 ? 0 : 2 * torusAngle, r, 2 * r, color);
                }
            }
        }
        glPopMatrix();
    }
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(dims[0], dims[1], dims[2], dims[3], 0.0, 20.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eye[0], eye[1], eye[2], //< eye position
        0.0f, 0.0f, 0.0f,  //< aim position
        0.0f, 1.f, 0.0f); //< up direction
    snake(THICKNESS, color);
    glFlush();
}

void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(30, (GLfloat)w / (GLfloat)h, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eye[0], eye[1], eye[2], 0, 0, 0, 0, 1, 0);
}

void keyboard(unsigned char key, int x, int y)
{
    glMatrixMode(GL_MODELVIEW);
    switch (key)
    {
        case 'x': case 'X':
            xTheta += 0.01;
            xTheta = (xTheta > 2*M_PI) ? (xTheta - 2*M_PI) : 
                        ((xTheta < 0) ? xTheta + 2*M_PI : xTheta);
            break;
        case 'y': case 'Y':
            yTheta += 0.01;
            yTheta = (yTheta > 2*M_PI) ? (yTheta - 2*M_PI) : 
                        ((yTheta < 0) ? yTheta + 2*M_PI : yTheta);
            break;
        case 'z': case 'Z':
            zTheta += 0.01;
            zTheta = (zTheta > 2*M_PI) ? (zTheta - 2*M_PI) : 
                        ((zTheta < 0) ? zTheta + 2*M_PI : zTheta);
            break;
        case 'l': case 'L':
            dims[0] += 0.1;
            dims[1] -= 0.1;
            dims[2] += 0.1;
            dims[3] -= 0.1;
            break;
        case 's': case 'S':
            dims[0] -= 0.1;
            dims[1] += 0.1;
            dims[2] -= 0.1;
            dims[3] += 0.1;
            break;
        case 'i': case 'I':
            glLoadIdentity();
            gluLookAt(0, 0, 10, 0, 0, 0, 0, 1, 0);
            glutPostRedisplay();
            break;
        case 27:
            exit(0);
            break;
    }

    // Reference - https://en.wikipedia.org/wiki/Rotation_matrix#General_3D_rotations
    eye[0] = EYE_Z * (
        cos(xTheta) * sin(yTheta) * cos(zTheta) + sin(xTheta) * sin(zTheta)
    );
    eye[1] = EYE_Z * (
        cos(xTheta) * sin(yTheta) * sin(zTheta) - sin(xTheta) * cos(zTheta)
    );
    eye[2] = EYE_Z * cos(xTheta) * cos(yTheta);
    glutPostRedisplay();
}


int main()
{
    int argc = 1;
    char* argv[1] = { (char*)"3D Snakes" };
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(300, 200);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutCreateWindow(argv[0]);
    init();
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
