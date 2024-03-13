#include <math.h>
#include <iostream>
#include <string>
#include <GL/glut.h>
#include "global.h"

using namespace std;

void fillPolygon(ShadedPolygon currentPolygon);
void drawPolygonBorder(ShadedPolygon currentPolygon);
void polygonContainingRectangle(
    ShadedPolygon &polygons,
    double &xFrom, double &xTo, double &yFrom, double &yTo
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
    glBlendFunc(GL_ONE, GL_ZERO);
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
        glTranslatef(polygons[polygonIndex].tx, polygons[polygonIndex].ty, 0);  // translate polygon
        glTranslatef(rx, ry, 0);  // translate the centroid back to original
        glScalef(polygons[polygonIndex].sx, polygons[polygonIndex].sy, 1);  // scale polygon
        glRotatef(polygons[polygonIndex].rtheta, 0.0, 0.0, 1.0);  // rotate polygon
        glTranslatef(-rx, -ry, 0);  // translate the centroid to origin
        fillPolygon(polygons[polygonIndex]);
        drawPolygonBorder(polygons[polygonIndex]);
        glPopMatrix(); //pops the top modelview matrix on the matrix stacks
    }
    glFlush();
    glMatrixMode(GL_PROJECTION);
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
    glLineWidth(1);
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
    glBlendFunc(GL_ONE, GL_ZERO);
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
        config.mode ^= (uint)Movement::CCW_ROTATE;
    }
    else
    {
        config.mode ^= (uint)Movement::CW_ROTATE;
    }

    cout << "Choose rotation speed: ";
    cin >> config.rotSpeed;

    cout << "Initial Movement Direction (right-up, left-up, right-down, left-down): ";
    cin >> mvmnt;

    if (mvmnt == "left-up")
    {
        config.mode ^= (uint)Movement::LEFT;
        config.mode ^= (uint)Movement::UP;
    }
    else if (mvmnt == "right-down")
    {
        config.mode ^= Movement::RIGHT;
        config.mode ^= Movement::DOWN;
    }
    else if (mvmnt == "left-down")
    {
        config.mode ^= (uint)Movement::LEFT;
        config.mode ^= (uint)Movement::DOWN;
    }
    else
    {
        config.mode ^= (uint)Movement::RIGHT;
        config.mode ^= (uint)Movement::UP;
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


void transformMine()
{
    for (int polygonIndex = 0; polygonIndex < polygonCount; polygonIndex++)
    {
        // calculate next time step attributes of the polygon in temp variable
        ShadedPolygon tempPolygon = polygons[polygonIndex];

        // update orientation
        tempPolygon.rtheta += (
            tempPolygon.moveConfig.mode & ((uint)Movement::CCW_ROTATE)
        ) ? tempPolygon.moveConfig.rotSpeed : (
            tempPolygon.moveConfig.mode & ((uint)Movement::CW_ROTATE)
        ) ? -tempPolygon.moveConfig.rotSpeed : 0;

        // check for out of bound values and correct it
        tempPolygon.rtheta += (
            tempPolygon.rtheta > 360
        ) ? -360 : (
            tempPolygon.rtheta < 0
        ) ? 360 : 0;

        // update x-position
        tempPolygon.tx += (
            tempPolygon.moveConfig.mode & ((uint)Movement::RIGHT)
        ) ? tempPolygon.moveConfig.xSpeed : (
            tempPolygon.moveConfig.mode & ((uint)Movement::LEFT)
        ) ? -tempPolygon.moveConfig.xSpeed : 0;

        // update y-position
        tempPolygon.ty += (
            tempPolygon.moveConfig.mode & ((uint)Movement::UP)
        ) ? tempPolygon.moveConfig.ySpeed : (
            tempPolygon.moveConfig.mode & ((uint)Movement::DOWN)
        ) ? -tempPolygon.moveConfig.ySpeed : 0;

        // update horizontal scaling
        tempPolygon.sx += (
            tempPolygon.moveConfig.mode & ((uint)Movement::HOR_GROWTH)
        ) ? (2 / tempPolygon.moveConfig.bouncePeriod *
                tempPolygon.moveConfig.shrinkFactor) : (
            tempPolygon.moveConfig.mode & ((uint)Movement::HOR_SHRINK)
        ) ? -(2 / tempPolygon.moveConfig.bouncePeriod *
                tempPolygon.moveConfig.shrinkFactor) : 0;

        // update vertical scaling
        tempPolygon.sy += (
            tempPolygon.moveConfig.mode & ((uint)Movement::VERT_GROWTH)
        ) ? (2 / tempPolygon.moveConfig.bouncePeriod *
                tempPolygon.moveConfig.shrinkFactor) : (
            tempPolygon.moveConfig.mode & ((uint)Movement::VERT_SHRINK)
        ) ? -(2 / tempPolygon.moveConfig.bouncePeriod *
                tempPolygon.moveConfig.shrinkFactor) : 0;

        // check for collision with the window edges
        bool colliding = false;
        double xFrom, xTo, yFrom, yTo;
        polygonContainingRectangle(tempPolygon, xFrom, xTo, yFrom, yTo);
        if(xFrom <= 0)  // left side collision
        {
            if(tempPolygon.moveConfig.mode & ((uint)Movement::HOR_SHRINK))
            {
                if(tempPolygon.sx <= (1-tempPolygon.moveConfig.shrinkFactor))
                {
                    // start the growth phase
                    polygons[polygonIndex].moveConfig.mode &= ~(uint)Movement::HOR_SHRINK;
                    polygons[polygonIndex].moveConfig.mode ^= (uint)Movement::HOR_GROWTH;

                    // change motion to right
                    polygons[polygonIndex].moveConfig.mode &= ~(uint)Movement::LEFT;
                    polygons[polygonIndex].moveConfig.mode ^= (uint)Movement::RIGHT;
                }
                colliding = true;
            }
            else if(tempPolygon.moveConfig.mode & ((uint)Movement::HOR_GROWTH))
            {
                if(tempPolygon.sx >= 1)
                {
                    // stop the growth phase
                    polygons[polygonIndex].moveConfig.mode &= ~(uint)Movement::HOR_GROWTH;
                }
                colliding = false;
            }
            else
            {
                // start the shrink phase
                polygons[polygonIndex].moveConfig.mode ^= (uint)Movement::HOR_SHRINK;
                colliding = true;
            }
        }
        else if(xTo >= X_MAX)  // right side collision
        {
            if(tempPolygon.moveConfig.mode & ((uint)Movement::HOR_SHRINK))
            {
                if(tempPolygon.sx <= (1-tempPolygon.moveConfig.shrinkFactor))
                {
                    // start the growth phase
                    polygons[polygonIndex].moveConfig.mode &= ~(uint)Movement::HOR_SHRINK;
                    polygons[polygonIndex].moveConfig.mode ^= (uint)Movement::HOR_GROWTH;

                    // change motion to left
                    polygons[polygonIndex].moveConfig.mode &= ~(uint)Movement::RIGHT;
                    polygons[polygonIndex].moveConfig.mode ^= (uint)Movement::LEFT;
                    colliding = true;
                }
            }
            else if(tempPolygon.moveConfig.mode & ((uint)Movement::HOR_GROWTH))
            {
                if(tempPolygon.sx >= 1)
                {
                    // stop the growth phase
                    polygons[polygonIndex].moveConfig.mode &= ~(uint)Movement::HOR_GROWTH;
                    colliding = false;
                }
            }
            else
            {
                // start the shrink phase
                polygons[polygonIndex].moveConfig.mode ^= (uint)Movement::HOR_SHRINK;
                colliding = true;
            }
        }
        else
        {
            if(tempPolygon.sx >= 1)
            {
                // stop the growth phase
                polygons[polygonIndex].moveConfig.mode &= ~(uint)Movement::HOR_GROWTH;
                colliding = false;
            }
            else if (tempPolygon.sx <= (1-tempPolygon.moveConfig.shrinkFactor))
            {
                // stop the shrink phase
                polygons[polygonIndex].moveConfig.mode &= ~(uint)Movement::HOR_SHRINK;
                colliding = false;
            }
        }
        if(yFrom <= 0)  // bottom collision
        {
            if(tempPolygon.moveConfig.mode & ((uint)Movement::VERT_SHRINK))
            {
                if(tempPolygon.sy <= (1-tempPolygon.moveConfig.shrinkFactor))
                {
                    // start the growth phase
                    polygons[polygonIndex].moveConfig.mode &= ~(uint)Movement::VERT_SHRINK;
                    polygons[polygonIndex].moveConfig.mode ^= (uint)Movement::VERT_GROWTH;

                    // change motion to up
                    polygons[polygonIndex].moveConfig.mode &= ~(uint)Movement::DOWN;
                    polygons[polygonIndex].moveConfig.mode ^= (uint)Movement::UP;
                    colliding = true;
                }
            }
            else if(tempPolygon.moveConfig.mode & ((uint)Movement::VERT_GROWTH))
            {
                if(tempPolygon.sy >= 1)
                {
                    // stop the growth phase
                    polygons[polygonIndex].moveConfig.mode &= ~(uint)Movement::VERT_GROWTH;
                    colliding = false;
                }
            }
            else
            {
                // start the shrink phase
                polygons[polygonIndex].moveConfig.mode ^= (uint)Movement::VERT_SHRINK;
                colliding = true;
            }
        }
        else if(yTo >= Y_MAX)  // top collision
        {
            if(tempPolygon.moveConfig.mode & ((uint)Movement::VERT_SHRINK))
            {
                if(tempPolygon.sy <= (1-tempPolygon.moveConfig.shrinkFactor))
                {
                    // start the growth phase
                    polygons[polygonIndex].moveConfig.mode &= ~(uint)Movement::VERT_SHRINK;
                    polygons[polygonIndex].moveConfig.mode ^= (uint)Movement::VERT_GROWTH;

                    // change motion to down
                    polygons[polygonIndex].moveConfig.mode &= ~(uint)Movement::UP;
                    polygons[polygonIndex].moveConfig.mode ^= (uint)Movement::DOWN;
                    colliding = true;
                }
            }
            else if(tempPolygon.moveConfig.mode & ((uint)Movement::VERT_GROWTH))
            {
                if(tempPolygon.sy >= 1)
                {
                    // stop the growth phase
                    polygons[polygonIndex].moveConfig.mode &= ~(uint)Movement::VERT_GROWTH;
                    colliding = false;
                }
            }
            else
            {
                // start the shrink phase
                polygons[polygonIndex].moveConfig.mode ^= (uint)Movement::VERT_SHRINK;
                colliding = true;
            }
        }
        else
        {
            if(tempPolygon.sy >= 1)
            {
                // stop the growth phase
                polygons[polygonIndex].moveConfig.mode &= ~(uint)Movement::VERT_GROWTH;
                colliding = false;
            }
            else if (tempPolygon.sy <= (1-tempPolygon.moveConfig.shrinkFactor))
            {
                // stop the shrink phase
                polygons[polygonIndex].moveConfig.mode &= ~(uint)Movement::VERT_SHRINK;
                colliding = false;
            }
        }

        // actual update of the polygon
        if (~colliding)
        {
            // don't rotate when colliding
            polygons[polygonIndex].rtheta += (
                polygons[polygonIndex].moveConfig.mode & ((uint)Movement::CCW_ROTATE)
            ) ? polygons[polygonIndex].moveConfig.rotSpeed : (
                polygons[polygonIndex].moveConfig.mode & ((uint)Movement::CW_ROTATE)
            ) ? -polygons[polygonIndex].moveConfig.rotSpeed : 0;
            polygons[polygonIndex].rtheta += (
                polygons[polygonIndex].rtheta > 360
            ) ? -360 : (
                polygons[polygonIndex].rtheta < 0
            ) ? 360 : 0;
        }
        polygons[polygonIndex].tx += (
            polygons[polygonIndex].moveConfig.mode & ((uint)Movement::RIGHT)
        ) ? polygons[polygonIndex].moveConfig.xSpeed : (
            polygons[polygonIndex].moveConfig.mode & ((uint)Movement::LEFT)
        ) ? -polygons[polygonIndex].moveConfig.xSpeed : 0;
        polygons[polygonIndex].ty += (
            polygons[polygonIndex].moveConfig.mode & ((uint)Movement::UP)
        ) ? polygons[polygonIndex].moveConfig.ySpeed : (
            polygons[polygonIndex].moveConfig.mode & ((uint)Movement::DOWN)
        )? -polygons[polygonIndex].moveConfig.ySpeed : 0;
        if (colliding)
        {
            if(xFrom <= 0)
            {
                polygons[polygonIndex].tx += (
                    polygons[polygonIndex].moveConfig.mode & ((uint)Movement::HOR_GROWTH)
                ) ? -(1 / polygons[polygonIndex].moveConfig.bouncePeriod *
                        polygons[polygonIndex].moveConfig.shrinkFactor) * 
                        (polygons[polygonIndex].centroidX - xFrom) : (
                    polygons[polygonIndex].moveConfig.mode & ((uint)Movement::HOR_SHRINK)
                ) ? (1 / polygons[polygonIndex].moveConfig.bouncePeriod *
                        polygons[polygonIndex].moveConfig.shrinkFactor) * 
                        (polygons[polygonIndex].centroidX - xFrom) : 0;
            }
            else if (xTo >= X_MAX)
            {
                polygons[polygonIndex].tx += (
                    polygons[polygonIndex].moveConfig.mode & ((uint)Movement::HOR_GROWTH)
                ) ? -(1 / polygons[polygonIndex].moveConfig.bouncePeriod *
                        polygons[polygonIndex].moveConfig.shrinkFactor) * 
                        (xTo - polygons[polygonIndex].centroidX) : (
                    polygons[polygonIndex].moveConfig.mode & ((uint)Movement::HOR_SHRINK)
                ) ? (1 / polygons[polygonIndex].moveConfig.bouncePeriod *
                        polygons[polygonIndex].moveConfig.shrinkFactor) * 
                        (xTo - polygons[polygonIndex].centroidX) : 0;
            }
            if (yFrom <= 0)
            {
                polygons[polygonIndex].ty += (
                    polygons[polygonIndex].moveConfig.mode & ((uint)Movement::VERT_GROWTH)
                ) ? -(1 / polygons[polygonIndex].moveConfig.bouncePeriod *
                        polygons[polygonIndex].moveConfig.shrinkFactor) * 
                        (polygons[polygonIndex].centroidY - yFrom) : (
                    polygons[polygonIndex].moveConfig.mode & ((uint)Movement::VERT_SHRINK)
                ) ? (1 / polygons[polygonIndex].moveConfig.bouncePeriod *
                        polygons[polygonIndex].moveConfig.shrinkFactor) * 
                        (polygons[polygonIndex].centroidY - yFrom) : 0;
            }
            else if (yTo >= Y_MAX)
            {
                polygons[polygonIndex].ty += (
                    polygons[polygonIndex].moveConfig.mode & ((uint)Movement::VERT_GROWTH)
                ) ? -(1 / polygons[polygonIndex].moveConfig.bouncePeriod *
                        polygons[polygonIndex].moveConfig.shrinkFactor) * 
                        (yTo - polygons[polygonIndex].centroidY) : (
                    polygons[polygonIndex].moveConfig.mode & ((uint)Movement::VERT_SHRINK)
                ) ? (1 / polygons[polygonIndex].moveConfig.bouncePeriod *
                        polygons[polygonIndex].moveConfig.shrinkFactor) * 
                        (yTo - polygons[polygonIndex].centroidY) : 0;
            }
        }

        polygons[polygonIndex].sx += (
            polygons[polygonIndex].moveConfig.mode & ((uint)Movement::HOR_GROWTH)
        ) ? (2 / polygons[polygonIndex].moveConfig.bouncePeriod *
                polygons[polygonIndex].moveConfig.shrinkFactor) : (
            polygons[polygonIndex].moveConfig.mode & ((uint)Movement::HOR_SHRINK)
        ) ? -(2 / polygons[polygonIndex].moveConfig.bouncePeriod *
                polygons[polygonIndex].moveConfig.shrinkFactor) : (
            1 - polygons[polygonIndex].sx
        );
        polygons[polygonIndex].sy += (
            polygons[polygonIndex].moveConfig.mode & ((uint)Movement::VERT_GROWTH)
        ) ? (2 / polygons[polygonIndex].moveConfig.bouncePeriod *
                polygons[polygonIndex].moveConfig.shrinkFactor) : (
            polygons[polygonIndex].moveConfig.mode & ((uint)Movement::VERT_SHRINK)
        ) ? -(2 / polygons[polygonIndex].moveConfig.bouncePeriod *
                polygons[polygonIndex].moveConfig.shrinkFactor) : (
            1 - polygons[polygonIndex].sy
        );

        polygons[polygonIndex].sx = max(min(polygons[polygonIndex].sx, 1.0), 0.0);
        polygons[polygonIndex].sy = max(min(polygons[polygonIndex].sy, 1.0), 0.0);
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
            glutIdleFunc(transformMine);
        }
        animating = !animating;
    }
    else if (key == 'd')
    {
        cout << "\nThe 'd' key was pressed - ";
        cout << "Enter a new drawing config! ---------------------->" << endl;
        currentPolygon.drawConfig = askDrawConfig();
    }
    else if (key == 'm')
    {
        cout << "\nThe 'm' key was pressed - ";
        cout << "Enter a new moving config! ---------------------->" << endl;
        currentPolygon.moveConfig = askMoveConfig();
    }
}

int main()
{
    cout << "\nEnter a new drawing config! ---------------------->" << endl;
    currentPolygon.drawConfig = askDrawConfig();
    cout << "\nEnter a new moving config! ---------------------->" << endl;
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
