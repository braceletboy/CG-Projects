#include <math.h>
#include <iostream>
#include <string>
#include <GL/glut.h>
#include "global.h"
#define X_MAX 600
#define Y_MAX 400
#define X_POSITION 200
#define Y_POSITION 200

using namespace std;
static Coords polygons[20][100];  // store coordinates of all polygons
static Color polygonColors[20];  // store colors of all polygons
static Color polygonBorderColors[20][100];  // store border colors of all polygons
static int no_vertices[20];  // no of vertices for each polygon
static int coordsP = 0;  // number of coordinates in the polygon being drawn
static Color colorsP[50];  // store colors of the polygon being drawn
static Color colorP;  // store recent color of the polygon being drawn
static int no_colors = 0;  // number of colors already entered
static int no_border_colors = 0;  // number of border colors already entered
static Color borderColorsP[50];  // store border colors of the polygon being drawn
static Color borderColorP;  // store recent border color of the polygon being drawn
static int polygonsP = 0;  // total number of polygons drawn
static int previousMouseLoc[2] = { 0,0 };
static int recentMouseButton = 0;
static GLushort lineStyle;
static string lineColorPattern;
static string fillPattern;
static string fillColorPattern;

static bool drawn = false;

Color getPixelColor(GLint x, GLint y) {
    Color color;
    glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, &color);
    return color;
}

void init() {
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, X_MAX, 0, Y_MAX);
    glEnable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();
}

void display(void) {
}

void askfillColor()
{
    cout << "Choose fill color (range is between 0 and 1)"<<endl;
    cout << "R: ";
    cin >> colorP.r;
    cout << "G: ";
    cin >> colorP.g;
    cout << "B: ";
    cin >> colorP.b;
    cout << "A: ";
    cin >> colorP.a;
    colorsP[no_colors] = colorP;
    no_colors++;

}

void askBorderColor()
{
    cout << "Choose border color (range is between 0 and 1)"<<endl;
    cout << "R: ";
    cin >> borderColorP.r;
    cout << "G: ";
    cin >> borderColorP.g;
    cout << "B: ";
    cin >> borderColorP.b;
    cout << "A: ";
    cin >> borderColorP.a;
    borderColorsP[no_border_colors] = borderColorP;
    no_border_colors++;
}

void fillPolygon()
{
    if(fillColorPattern == "solid")
    {
        glColor4f(colorP.r, colorP.g, colorP.b, colorP.a);
    }
    if(fillPattern == "solid")
    {
        glColor4f(colorP.r, colorP.g, colorP.b, colorP.a);
        glBegin(GL_POLYGON);
        for (int i = 0; i < coordsP; i++)
        {
            if(fillColorPattern == "gradient")
            {
                glColor4f(
                    colorsP[i%no_colors].r,
                    colorsP[i%no_colors].g,
                    colorsP[i%no_colors].b,
                    colorsP[i%no_colors].a
                );
            }
            glVertex2i(polygons[polygonsP][i].x, polygons[polygonsP][i].y);
        }
        glEnd();
    }
    else if (fillPattern == "fly")
    {
        glEnable(GL_POLYGON_STIPPLE);
        glPolygonStipple(fly);
        glBegin(GL_POLYGON);
        for (int i = 0; i < coordsP; i++)
        {
            if(fillColorPattern == "gradient")
            {
                glColor4f(
                    colorsP[i%no_colors].r,
                    colorsP[i%no_colors].g,
                    colorsP[i%no_colors].b,
                    colorsP[i%no_colors].a
                );
            }
            glVertex2i(polygons[polygonsP][i].x, polygons[polygonsP][i].y);
        }
        glEnd();
        glDisable(GL_POLYGON_STIPPLE);
    }
    else if (fillPattern == "hatched")
    {
        glEnable(GL_POLYGON_STIPPLE);
        glPolygonStipple(HorHatch);
        glBegin(GL_POLYGON);
        for (int i = 0; i < coordsP; i++)
        {
            if(fillColorPattern == "gradient")
            {
                glColor4f(
                    colorsP[i%no_colors].r,
                    colorsP[i%no_colors].g,
                    colorsP[i%no_colors].b,
                    colorsP[i%no_colors].a
                );
            }
            glVertex2i(polygons[polygonsP][i].x, polygons[polygonsP][i].y);
        }
        glEnd();
        glDisable(GL_POLYGON_STIPPLE);
    }
    else if (fillPattern == "halftone")
    {
        glEnable(GL_POLYGON_STIPPLE);
        glPolygonStipple(halftone);
        glBegin(GL_POLYGON);
        for (int i = 0; i < coordsP; i++)
        {
            if(fillColorPattern == "gradient")
            {
                glColor4f(
                    colorsP[i%no_colors].r,
                    colorsP[i%no_colors].g,
                    colorsP[i%no_colors].b,
                    colorsP[i%no_colors].a
                );
            }
            glVertex2i(polygons[polygonsP][i].x, polygons[polygonsP][i].y);
        }
        glEnd();
        glDisable(GL_POLYGON_STIPPLE);
    }
}

void onMouseMove(int x, int y) {
    if (coordsP == 0 || recentMouseButton == GLUT_RIGHT_BUTTON) {
        previousMouseLoc[0] = x;
        previousMouseLoc[1] = y;
        return;
    }
    y = (glutGet(GLUT_WINDOW_HEIGHT) - y) * (float)Y_MAX / glutGet(GLUT_WINDOW_HEIGHT);
    x = x * ((float)X_MAX / glutGet(GLUT_WINDOW_WIDTH));
    glBlendFunc(GL_ONE, GL_ZERO);
    glColor4f(1.0, 1.0, 1.0, 1.0);
    glBegin(GL_LINE_STRIP);
    glVertex2i(previousMouseLoc[0], previousMouseLoc[1]);
    glVertex2i(polygons[polygonsP][coordsP - 1].x, polygons[polygonsP][coordsP - 1].y);
    glEnd();
    glColor4f(borderColorP.r, borderColorP.g, borderColorP.b, borderColorP.a);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    for (int j = 0; j <= polygonsP; j++) {
        int no_vertex = (j == polygonsP) ? coordsP : no_vertices[j];
        glBegin(GL_LINE_STRIP);
        for (int i = 0; i < no_vertex; i++)
            glVertex2i(polygons[j][i].x, polygons[j][i].y);
        if(j == polygonsP)
            glVertex2i(x, y);
        else
            glVertex2i(polygons[j][0].x, polygons[j][0].y);
        glEnd();
    }
    glFlush();
    previousMouseLoc[0] = x;
    previousMouseLoc[1] = y;
    drawn = true;
}

void onMouseClick(int button, int state, int x, int y)
{
    recentMouseButton = button;
    y = (glutGet(GLUT_WINDOW_HEIGHT) - y) * (float)Y_MAX / glutGet(GLUT_WINDOW_HEIGHT);
    x = x * ((float)X_MAX / glutGet(GLUT_WINDOW_WIDTH));
    if (button == GLUT_RIGHT_BUTTON && coordsP >= 3) {
        // fill polygon first
        fillPolygon();

        glBlendFunc(GL_ONE, GL_ZERO);
        glLineWidth(3);
        glEnable(GL_LINE_STIPPLE);
        glLineStipple(3, lineStyle);
        if(lineColorPattern == "solid")
        {
            glColor4f(
                borderColorP.r, borderColorP.g, borderColorP.b, borderColorP.a
            );
        }
        glBegin(GL_LINE_STRIP);
        for (int i = 0; i < coordsP; i++)
        {
            if(lineColorPattern == "gradient")
            {
                glColor4f(
                    borderColorsP[i%no_border_colors].r,
                    borderColorsP[i%no_border_colors].g,
                    borderColorsP[i%no_border_colors].b,
                    borderColorsP[i%no_border_colors].a
                );
            }
            glVertex2i(polygons[polygonsP][i].x, polygons[polygonsP][i].y);
        }
        glVertex2i(polygons[polygonsP][0].x, polygons[polygonsP][0].y);
        glEnd();
        glDisable(GL_LINE_STIPPLE);

        /*glBegin(GL_LINES);
        glVertex2i(polygons[polygonsP][coordsP - 1].x, polygons[polygonsP][coordsP - 1].y);
        glVertex2i(polygons[polygonsP][0].x, polygons[polygonsP][0].y);
        glEnd();*/
        glFlush();
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        no_vertices[polygonsP] = coordsP;
        polygonsP++;
        coordsP = 0;
        drawn = false;
    }
    if (button == GLUT_LEFT_BUTTON && (state == GLUT_DOWN && coordsP == 0 || state == GLUT_UP && drawn)) {
        glPointSize(2);
        glColor4f(
            borderColorP.r, borderColorP.g, borderColorP.b, borderColorP.a
        );
        glBegin(GL_POINTS);
        glVertex2i(x, y);
        glEnd();
        glFlush();
        polygons[polygonsP][coordsP].x = x;  // mouseLoc[0] = x;
        polygons[polygonsP][coordsP].y = y;  // mouseLoc[1] = y;
        coordsP++;
        drawn = false;
    }
}
int main()
{
    int argc = 1;
    char* argv[1] = { (char*)"Something" };

    cout << "Choose line style (input a 4 digit hex): ";
    cin >> hex >> lineStyle >> dec;  // set stream to hex and reset it to dec

    cout << "Choose line coloring pattern (solid / gradient): ";
    cin >> lineColorPattern;
    if (lineColorPattern == "solid")
    {
        cout << "Single Solid Color - ";
        askBorderColor();
    }
    else if(lineColorPattern == "gradient")
    {
        int numColors;
        cout << "Number of border colors: ";
        cin >> numColors;
        for(int idx = 0; idx < numColors; idx ++)
        {
            cout << "Gradient Color" << idx <<" - ";
            askBorderColor();
        }
    }

    cout<<"Choose fill pattern (solid / fly / hatched / halftone): ";
    cin >> fillPattern;

    cout << "Choose fill coloring pattern (solid / gradient): ";
    cin >> fillColorPattern;
    
    if (fillColorPattern == "solid")
    {
        cout << "Single Solid Color - ";
        askfillColor();
    }
    else if (fillColorPattern == "gradient")
    {
        int numColors;
        cout << "Number of fill colors: ";
        cin >> numColors;
        for(int idx = 0; idx < numColors; idx ++)
        {
            cout << "Gradient Color" << idx <<" - ";
            askfillColor();
        }
    }

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(X_MAX, Y_MAX);
    glutInitWindowPosition(X_POSITION, Y_POSITION);
    //start setting up a display window
    glutCreateWindow("Polygon Shader");
    init();
    glutDisplayFunc(display);
    //finish setting up a display window
    glutMotionFunc(onMouseMove);
    glutMouseFunc(onMouseClick);
    glutMainLoop();
    return 0;
}