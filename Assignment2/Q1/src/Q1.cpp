#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

// Data points for the bar chart
std::vector<int> dataPoints = {10, 20, 30, 40, 50};

// Labels for the x and y axes
std::string xAxisLabel = "Data Point";
std::string yAxisLabel = "Value";


void drawBarChart() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Set up the viewport and projection matrix
    glViewport(50, 50, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1, dataPoints.size() * 2 + 5, -5, *std::max_element(dataPoints.begin(), dataPoints.end())+10);

    // Draw bars for each data point
    for (int i = 0; i < dataPoints.size(); i++) {

        int value = dataPoints[i];

        glColor3f(0.0, 1.0, 0.0);
        glBegin(GL_POLYGON);
        glVertex2f(i * 2 + 0.5, 0);
        glVertex2f(i * 2 + 0.5, value);
        glVertex2f(i * 2 + 1.5, value);
        glVertex2f(i * 2 + 1.5, 0);
        glEnd();

        // write value above bar
        glColor3f(0.0, 0.0, 1.0);
        glRasterPos2i(i*2 + 1, value + 1);
        for (char c : std::to_string(value)) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
        }
    }

    // Draw x and y axes
    glColor3f(0.0, 0.0, 1.0);
    glLineWidth(20);
    glBegin(GL_LINES);
    glVertex2f(0, 0);
    glVertex2f(0, GLUT_WINDOW_HEIGHT);
    glVertex2f(0, 0);
    glVertex2f(GLUT_WINDOW_WIDTH, 0);
    glEnd();

    // Draw x axis label
    glRasterPos2i(dataPoints.size(), -3);
    for (char c : xAxisLabel) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }

    // Draw y axis label
    glRasterPos2i(-1, *std::max_element(dataPoints.begin(), dataPoints.end())/2);
    for (char c : yAxisLabel) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }

    // Draw value for each bar
    for (int i = 0; i < dataPoints.size(); i++) {
    }

    glFlush();
}

void onMouseClick(int button, int state, int x, int y)
{
    ;
}


int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Bar Chart");

    glutDisplayFunc(drawBarChart);
    glutMouseFunc(onMouseClick);

    glClearColor(1.0, 1.0, 1.0, 0.0);

    glutMainLoop();

    return 0;
}
