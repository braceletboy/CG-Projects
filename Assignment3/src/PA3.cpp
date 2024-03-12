#include <math.h>
#include <iostream>
#include <string>
#include <GL/glut.h>
#include "global.h"

using namespace std;

void fillPolygon(ShadedPolygon currentPolygon);
void drawPolygonBorder(ShadedPolygon currentPolygon);
void polygonContainingRectangle(
    ShadedPolygon* polygons, int polygonIndex,
    double* xFrom, double* xTo, double* yFrom, double* yTo
);

void polygonCentroidApprox(ShadedPolygon &polygons);

void polygonCentroidAccurate(ShadedPolygon &polygons);

ShadedPolygon polygons[POLYGON_NUM];  // store coordinates of all polygons
int polygonCount = 0;  // total number of polygons drawn
int recentMouseButton = 0;


int previousMouseLoc[2] = { 0,0 };  // default is the left lower corner
bool drawing = false;  // whether we are drawing
bool animating = false;  // whether we are animating
Color borderDrawColor = {.r = 0, .g=0, .b = 0, .a=1};  // black color
ShadedPolygon currentPolygon = {.numVertices=0};  // polygon being drawn

/**
 * @brief Setup the options in the OpenGL context
 * 
 */
void init() {
    glClearColor(1.0, 1.0, 1.0, 1.0);  // choose white for background color
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, X_MAX, 0, Y_MAX);  // 2D projection

    glEnable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

    // by default blend using the source alpha for the blending factors
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

/**
 * @brief The display callback function
 * 
 */
void display(void)
{
    // apply transformations to the Modelview matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT);  // set background to choosen color
    int rx, ry;
    for (int polygonIndex = 0; polygonIndex < polygonCount; polygonIndex++)
    {
        rx = polygons[polygonIndex].centroidX, ry = polygons[polygonIndex].centroidY;
        glPushMatrix();  // push a new model view matrix onto the stack
        glTranslatef(rx, ry, 0);  // translate the centroid back to original
        glTranslatef(polygons[polygonIndex].tx, polygons[polygonIndex].ty, 0);  // translate polygon
        // glScalef(polygons[polygonIndex].sx, polygons[polygonIndex].sy, 1);  // scale polygon
        glRotatef(polygons[polygonIndex].rtheta, 0.0, 0.0, 1.0);  // rotate polygon
        glTranslatef(-rx, -ry, 0);  // translate the centroid to origin
        fillPolygon(polygons[polygonIndex]);
        drawPolygonBorder(polygons[polygonIndex]);
        glPopMatrix(); //pops the top modelview matrix on the matrix stacks
    }
    glFlush();
}

void onMouseMove(int x, int y)
{
    // do nothing when there are no new points on the window
    if (currentPolygon.numVertices == 0 || recentMouseButton != GLUT_LEFT_BUTTON)
    {
        previousMouseLoc[0] = x;
        previousMouseLoc[1] = y;
        return;
    }

    y = (glutGet(GLUT_WINDOW_HEIGHT) - y) * (float)Y_MAX / glutGet(GLUT_WINDOW_HEIGHT);
    x = x * ((float)X_MAX / glutGet(GLUT_WINDOW_WIDTH));

    // erase previous line from recent polygon point to previous cursor loc
    if(drawing)
    {
        glBlendFunc(GL_ONE, GL_ZERO);
        glColor4f(1.0, 1.0, 1.0, 1.0);  // background is white
        glBegin(GL_LINE_STRIP);
            glVertex2i(
                currentPolygon.vertices[currentPolygon.numVertices - 1].x,
                currentPolygon.vertices[currentPolygon.numVertices - 1].y
            );
            glVertex2i(previousMouseLoc[0], previousMouseLoc[1]);
        glEnd();
    }

    // draw new line from recent polygon point to current cursor loc
    glColor4f(borderDrawColor.r, borderDrawColor.g,
                borderDrawColor.b, borderDrawColor.a);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBegin(GL_LINE_STRIP);
        glVertex2i(
            currentPolygon.vertices[currentPolygon.numVertices - 1].x,
            currentPolygon.vertices[currentPolygon.numVertices - 1].y
        );
        glVertex2i(x, y);
    glEnd();
    glFlush();
    previousMouseLoc[0] = x;
    previousMouseLoc[1] = y;
    drawing = true;
}

/**
 * @brief Callback function when a mouse button is pressed
 * 
 * @param button The button which was pressed - right, middle or left
 * @param state The state of the button - up or down
 * @param x The x-coordinate of the mouse pointer
 * @param y The y-coordinate of the mouse pointer
 * 
 * @note The x and y coordinates are measured from the top leftcorner of the
 * drawable area in the window
 */

void onMouseClick(int button, int state, int x, int y)
{
    recentMouseButton = button;

    // compute coordinates from bottom left corner
    y = (glutGet(GLUT_WINDOW_HEIGHT) - y) * (float)Y_MAX / glutGet(GLUT_WINDOW_HEIGHT);
    x = x * ((float)X_MAX / glutGet(GLUT_WINDOW_WIDTH));

    if (button == GLUT_RIGHT_BUTTON && currentPolygon.numVertices >= 3)
    {
        // fill polygon first
        fillPolygon(currentPolygon);
        drawPolygonBorder(currentPolygon);
        polygonCentroidAccurate(currentPolygon);
        polygons[polygonCount] = currentPolygon;
        polygonCount++;
        currentPolygon.numVertices = 0;
        drawing = false;
    }
    if (button == GLUT_LEFT_BUTTON &&
        (state == GLUT_DOWN && currentPolygon.numVertices == 0 ||
            state == GLUT_UP && drawing))
    {
        // draw a point on button click
        glColor4f(0, 0, 0, 0);
        glBegin(GL_POINTS);
            glVertex2i(x, y);
        glEnd();
        glFlush();

        // save the point location
        currentPolygon.vertices[currentPolygon.numVertices].x = x;  // mouseLoc[0] = x;
        currentPolygon.vertices[currentPolygon.numVertices].y = y;  // mouseLoc[1] = y;
        currentPolygon.numVertices++;

        // state management
        drawing = true;
    }
}


/**
 * @brief Ask the user to input color through command line
 * 
 * @return The inputed color
 */
Color askColor()
{
    Color color;
    cout << "(range is between 0 and 1)"<<endl;
    cout << "R: ";
    cin >> color.r;
    cout << "G: ";
    cin >> color.g;
    cout << "B: ";
    cin >> color.b;
    cout << "A: ";
    cin >> color.a;
    return color;
}

/**
 * @brief Ask the user to input the movement configuration
 * 
 * @return MovementConfig 
 */
MovementConfig askMoveConfig()
{
    MovementConfig config;
    string mvmnt;
    cout << "Choose the initial rotation direction of the polygon (ccw / cw): ";
    cin >> mvmnt;

    if(mvmnt == "ccw")
    {
        config.mode ^= Movement::CCW_ROTATE;
    }
    else
    {
        config.mode ^= Movement::CW_ROTATE;
    }

    cout << "Choose rotation speed: ";
    cin >> config.rotSpeed;

    cout << "Initial Movement Direction (right-up, left-up, right-down, left-down): ";
    cin >> mvmnt;

    if (mvmnt == "left-up")
    {
        config.mode ^= Movement::LEFT;
        config.mode ^= Movement::UP;
    }
    else if (mvmnt == "right-down")
    {
        config.mode ^= Movement::RIGHT;
        config.mode ^= Movement::DOWN;
    }
    else if (mvmnt == "left-down")
    {
        config.mode ^= Movement::LEFT;
        config.mode ^= Movement::DOWN;
    }
    else
    {
        config.mode ^= Movement::RIGHT;
        config.mode ^= Movement::UP;
    }

    cout << "Choose horizontal speed: ";
    cin >> config.xSpeed;

    cout << "Choose vertical speed: ";
    cin >> config.ySpeed;

    cout << "Choose shrink factor: ";
    cin >> config.shrinkFactor;

    cout << "Choose bounce duration: ";
    cin >> config.bouncePeriod;

    return config;
}

/**
 * @brief As for the drawing configurtion for the polygons
 * 
 * @return Index of the polygon
 */
ShadedPolygonDrawConfig askDrawConfig()
{
    ShadedPolygonDrawConfig polyConfig;

    // choose border pattern
    cout << "Choose border pattern (input a 4 digit hex): ";
    cin >> hex >> polyConfig.borderPattern >> dec;  // set stream to hex and reset it to dec

    // choose border coloring pattern
    string bcp;
    cout << "Choose border coloring pattern (solid / gradient): ";
    cin >> bcp;
    if(bcp == "gradient")
    {
        polyConfig.borderColorPattern = ColorPattern::GRADIENTCOLOR;
        cout << "Number of border colors: ";
        cin >> polyConfig.numBorderColors;
        for(int idx = 0; idx < polyConfig.numBorderColors; idx ++)
        {
            cout << "Gradient Color" << idx << " ";
            polyConfig.borderColors[idx] = askColor();
        }
    }
    else
    {
        polyConfig.borderColorPattern = ColorPattern::SOLIDCOLOR;
        polyConfig.numBorderColors = 1;
        cout << "Single Solid Color ";
        polyConfig.borderColors[0] = askColor();
    }

    // choose fill pattern
    string fp;
    cout<<"Choose fill pattern (solid / fly / hatched / halftone): ";
    cin >> fp;

    if (fp == "fly")
    {
        polyConfig.fillPattern = FillPattern::FLY;
    }
    else if (fp == "hatched")
    {
        polyConfig.fillPattern = FillPattern::HATCHED;
    }
    else if (fp == "halftone")
    {
        polyConfig.fillPattern = FillPattern::HALFTONE;
    }
    else
    {
        polyConfig.fillPattern = FillPattern::SOLID;
    }

    // choose fill coloring pattern
    string fcp;
    cout << "Choose fill coloring pattern (solid / gradient): ";
    cin >> fcp;
    if (fcp == "gradient")
    {
        polyConfig.fillColorPattern = ColorPattern::GRADIENTCOLOR;
        cout << "Number of fill colors: ";
        cin >> polyConfig.numFillColors;
        for(int idx = 0; idx < polyConfig.numFillColors; idx ++)
        {
            cout << "Gradient Color" << idx <<" ";
            polyConfig.fillColors[idx] = askColor();
        }
    }
    else
    {
        polyConfig.fillColorPattern = ColorPattern::SOLIDCOLOR;
        cout << "Single Solid Color ";
        polyConfig.fillColors[0] = askColor();
    }
    return polyConfig;
}


void transform()
{
    double xFrom, xTo, yFrom, yTo;
    for (int i = 0; i < polygonCount;i++) {
        // update orientation
        polygons[i].rtheta += (
            polygons[i].moveConfig.mode & CCW_ROTATE
        ) ? polygons[i].moveConfig.rotSpeed : (
            polygons[i].moveConfig.mode & CW_ROTATE
        ) ? -polygons[i].moveConfig.rotSpeed : 0;

        // check for out of bound values and correct it
        polygons[i].rtheta += (
            polygons[i].rtheta > 360
        ) ? -360 : (
            polygons[i].rtheta < 0
        ) ? 360 : 0;

        // update x-position
        polygons[i].tx += (
            polygons[i].moveConfig.mode & RIGHT
        ) ? polygons[i].moveConfig.xSpeed : (
            polygons[i].moveConfig.mode & LEFT
        ) ? -polygons[i].moveConfig.xSpeed : 0;

        // update y-position
        polygons[i].ty += (
            polygons[i].moveConfig.mode & UP
        ) ? polygons[i].moveConfig.ySpeed : (
            polygons[i].moveConfig.mode & DOWN
        )? -polygons[i].moveConfig.ySpeed :0;

        // // update horizontal motion
        // if (polygons[i].sx >= 1.0 && 
        //     (polygons[i].moveConfig.mode & HOR_GROWTH))
        // {
        //     polygons[i].moveConfig.mode = polygons[i].moveConfig.pastMode;
        //     polygons[i].moveConfig.mode = (Movement)(
        //         polygons[i].moveConfig.mode ^
        //         (CW_ROTATE | CCW_ROTATE | LEFT | RIGHT)
        //     );
        //     continue;
        // }
        // if (polygons[i].sx < (1 - polygons[i].moveConfig.shrinkFactor) &&
        //     (polygons[i].moveConfig.mode & HOR_SHRINK))
        // {
        //     polygons[i].moveConfig.mode = HOR_GROWTH;
        //     continue;
        // }

        // // update vertical motion
        // if (polygons[i].sy >= 1.0 && 
        //     (polygons[i].moveConfig.mode & VERT_GROWTH))
        // {
        //     polygons[i].moveConfig.mode = polygons[i].moveConfig.pastMode;
        //     polygons[i].moveConfig.mode = (Movement)(
        //         polygons[i].moveConfig.mode ^
        //         (CW_ROTATE | CCW_ROTATE | UP | DOWN)
        //     );
        //     continue;
        // }
        // if (polygons[i].sy < (1 - polygons[i].moveConfig.shrinkFactor) &&
        //     (polygons[i].moveConfig.mode & VERT_SHRINK))
        // {
        //     polygons[i].moveConfig.mode = VERT_GROWTH;
        //     continue;
        // }

        // // update shrink
        // polygonContainingRectangle(polygons, i, &xFrom, &xTo, &yFrom, &yTo);
        // if (polygons[i].moveConfig.mode & HOR_SHRINK)
        // {
        //     polygons[i].tx += (polygons[i].moveConfig.pastMode & RIGHT) ?
        //         xTo / polygons[i].moveConfig.bouncePeriod / polygons[i].sx
        //         : xFrom / polygons[i].moveConfig.bouncePeriod / polygons[i].sx;
        //     polygons[i].sx -= 1 / polygons[i].moveConfig.bouncePeriod;
        //     continue;
        // }
        // if (polygons[i].moveConfig.mode & HOR_GROWTH)
        // {
        //     polygons[i].tx -= (polygons[i].moveConfig.pastMode & RIGHT) ?
        //         xTo / polygons[i].moveConfig.bouncePeriod / polygons[i].sx
        //         : xFrom / polygons[i].moveConfig.bouncePeriod / polygons[i].sx;
        //     polygons[i].sx += 1 / polygons[i].moveConfig.bouncePeriod;
        //     continue;
        // }
        // if (polygons[i].moveConfig.mode & VERT_SHRINK)
        // {
        //     polygons[i].ty += (polygons[i].moveConfig.pastMode & UP) ?
        //         yTo / polygons[i].moveConfig.bouncePeriod / polygons[i].sy
        //         : yFrom / polygons[i].moveConfig.bouncePeriod / polygons[i].sy;
        //     polygons[i].sy -= 1 / polygons[i].moveConfig.bouncePeriod;
        //     continue;
        // }
        // if (polygons[i].moveConfig.mode & VERT_GROWTH)
        // {
        //     polygons[i].ty -= (polygons[i].moveConfig.pastMode & UP) ?
        //         yTo / polygons[i].moveConfig.bouncePeriod / polygons[i].sy
        //         : yFrom / polygons[i].moveConfig.bouncePeriod / polygons[i].sy;
        //     polygons[i].sy += 1 / polygons[i].moveConfig.bouncePeriod;
        //     continue;
        // }
        // if (polygons[i].tx + polygons[i].centroidX + xTo >= X_MAX  // hit the right border
        //     || polygons[i].tx + polygons[i].centroidX + xFrom <= 0)  // hit the left border
        // {
        //     polygons[i].moveConfig.pastMode = polygons[i].moveConfig.mode;
        //     polygons[i].moveConfig.mode = HOR_SHRINK ;
        //     continue;
        // }
        // if (polygons[i].ty + polygons[i].centroidY + yTo >= Y_MAX  // hit the top border
        //     || polygons[i].ty + polygons[i].centroidY + yFrom <= 0)  // hit the bottom border 
        // {
        //     polygons[i].moveConfig.pastMode = polygons[i].moveConfig.mode;
        //     polygons[i].moveConfig.mode = VERT_SHRINK;
        //     continue;
        // }
    }
    glutPostRedisplay();  // call the display function to update the display
}

void onKeystroke(unsigned char key, int x, int y) {
    if (key == ' ' && polygonCount > 0)
    {
        cout << "\nThe space bar was pressed - ";
        if (animating)
        {
            cout << "Stopping animation" << endl;
            glutIdleFunc(NULL);
        }
        else
        {
            cout << "Staring animation" << endl;
            glutIdleFunc(transform);
        }
        animating = !animating;
    }
    else if (key == 'c')
    {
        cout << "\nThe 'c' key was pressed - ";
        cout << "Enter a new drawing config! ---------------------->" << endl;
        currentPolygon.drawConfig = askDrawConfig();
        currentPolygon.moveConfig = askMoveConfig();
    }
}

int main()
{
    cout << "Enter a new drawing config! ---------------------->" << endl;
    currentPolygon.drawConfig = askDrawConfig();
    currentPolygon.moveConfig = askMoveConfig();

    int argc = 1;
    char* argv[1] = { (char*)"Something" };

    // initialize freeglut
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(2*X_MAX, 2*Y_MAX);
    glutInitWindowPosition(X_POSITION, Y_POSITION);

    // start setting up a display window
    glutCreateWindow("Screen Saver Window");
    init();
    glutDisplayFunc(display);

    // finish setting up a display window
    glutMotionFunc(onMouseMove);
    glutMouseFunc(onMouseClick);
    glutKeyboardFunc(onKeystroke);
    glutMainLoop();
    return 0;
}