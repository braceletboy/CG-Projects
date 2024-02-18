#include <math.h>
#include <iostream>
#include "Tools.h"

using namespace std;
void display(void)
{
    // draw
    glClear(GL_COLOR_BUFFER_BIT);
    glColor4f(1, 0, 0,1);
    glBegin(GL_LINE_STRIP);

    // drawing the left hand side of the first equation
    draw_function_cartesian_system(0, 15, cosine, inverse_cosine_2);
    draw_function_cartesian_system(15, 25*(1 + M_PI)-10, cosine, inverse_cosine_1);
    draw_function_cartesian_system(25*(1 + M_PI)-10, 25*(1 + 2*M_PI)-10, cosine, inverse_cosine_3);

    // drawing the left hand side of the second equation
    // draw_function_cartesian_system(0, 700, cubic, inverse_cubic);

    glEnd();

    glLineWidth(2.0);
    glColor4f(0, 0, 1,.9);
    glBegin(GL_LINE_STRIP);

    // drawing the right hand side of the first equation
    draw_function_cartesian_system(0, 70, quadratic_first_eq, inverse_quadratic_first_eq2);
    draw_function_cartesian_system(70, 25 * (1+2*M_PI), quadratic_first_eq, inverse_quadratic_first_eq1);

    // drawing the right hand side of the second equation
    // draw_function_cartesian_system(0, 100 - 50, quadratic_second_eq, inverse_quadratic_second_eq1);
    // draw_function_cartesian_system(100-50, 700, quadratic_second_eq, inverse_quadratic_second_eq2);

    glEnd();
    
    glFlush();
}

void init()
{
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glColor3f(0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, X_MAX, 0, Y_MAX);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
void onMouseClick(int button, int state, int x, int y)
{
    y = Y_MAX - y;
    if (state != GLUT_DOWN || button != GLUT_LEFT_BUTTON)
        return;
    //A left click triggers a search for nearby intersection points.
    //Every intersection point is corresponding to a root of the equation
    for(int i = x - SEARCH_RANGE; i < x + SEARCH_RANGE; i++)
    {
        for (int j = y - SEARCH_RANGE; j < y + SEARCH_RANGE; j++)
        {
            Color c = getPixelColor(i, j);
            if (c.g == 0 && c.b != 0)
                cout << "(" << i << "," << j << ")" << endl;
        }
    }
}

int main(int argc, char** argv)
{
    /* The goal of this program is to provide a graphical tool 
    * find all the roots of equation f(x) = g(x) where x varies in 
    a given interval (min,max), f and g are two functions in this range.
    */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(X_MAX, Y_MAX);
    glutInitWindowPosition(X_POSITION, Y_POSITION);
    glutCreateWindow("Non-linear Equation Solver");
    init();
    glutDisplayFunc(display);//drawing plots of functions f(x) and g(x)
    glutMouseFunc(onMouseClick);//using mouse clicks to trigger a search for roots
    glutMainLoop();
    return 0;
}