//
//  main.cpp
//  Assignment1-Q3
//

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#include <math.h>

// Variables for window dimensions
const int windowWidth = 200;
const int windowHeight = 150;

void display() {
    // Clear the window
    glClear(GL_COLOR_BUFFER_BIT);

    // Set the color (e.g., red)
    glColor3f(1.0, 0.0, 0.0);

    // Calculate the rectangle dimensions
    float rectWidth = windowWidth/2;
    float rectHeight = windowHeight/2;

    // Calculate the center position
    float centerX = windowWidth/2; // Since the coordinate system is centered
    float centerY = windowHeight/2; // Since the coordinate system is centered

    // Draw the rectangle
    glBegin(GL_POLYGON);
        glVertex2f(centerX - rectWidth / 2, centerY - rectHeight / 2);
        glVertex2f(centerX + rectWidth / 2, centerY - rectHeight / 2);
        glVertex2f(centerX + rectWidth / 2, centerY + rectHeight / 2);
        glVertex2f(centerX - rectWidth / 2, centerY + rectHeight / 2);
    glEnd();

    // Flush drawing commands
    glFlush();
}

void init() {
    // Set the clear color (background)
    glClearColor(1.0, 1.0, 1.0, 0.0);

    // Initialize viewing values
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, windowWidth, 0, windowHeight);
}

int main(int argc, char** argv) {
    // Initialize GLUT
    glutInit(&argc, argv);

    // Set display mode
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

    // Set the window position
    glutInitWindowPosition(75, 50);

    // Set the window size
    glutInitWindowSize(windowWidth, windowHeight);

    // Create the window with a title
    glutCreateWindow("OpenGL Rectangle");

    // Initialize
    init();

    // Set the display callback function
    glutDisplayFunc(display);

    // Enter the GLUT main loop
    glutMainLoop();

    return 0;
}
