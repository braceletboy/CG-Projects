#include "openGLPart.h"


Point bunnyLocation[NUMBER_OF_BUNNIES];
Point snakeBody[MAX_SEGMENTS];// circular array to implemenet the queue of body segments...
int bodySegments;
int firstIndex;//shows the tail position
int width, height;
Point snakeUp = { 0,0,1 };
//lastIndex = (firstIndex + bodySegments) % MAX_SEGMENTS;//shows the head position
//lastIndex = (firstIndex + bodySegments - 1) % MAX_SEGMENTS;//shows the starting point of line segment which ends at the head
GLfloat randomColors[300000];
int lats = 30, longs = 200;
ColorPattern color = SOLID;
bool moving = false, gameover = false;
int blinking = BLINKING_PERIOD;
bool defaultCamera = true;
int eaten = 0;
double bunnyRotDegree = 0;
const void* font = GLUT_BITMAP_TIMES_ROMAN_24;//GLUT_BITMAP_9_BY_15;
std::vector<GLfloat> vertices;
std::vector<GLuint> faces;
std::vector<GLfloat> normals;

float x = 0.0, y = 0.0, scaleFactor = 1, rotateX = 0, rotateY = 0, rotateZ = 0;
float deltaXMove = 0.0, deltaYMove = 0.0; // initially camera doesn't move
GLfloat dims[4] = {-X_MAX, X_MAX, -Y_MAX, Y_MAX};

typedef GLint vertex3[3];
vertex3 pt[8] = { {-1, -1, -1}, {-1, 1, -1}, {1, -1, -1}, {1, 1, -1},
{-1, -1, 1}, {-1, 1, 1}, {1, -1, 1}, {1, 1, 1} };


void quad(GLint n1, GLint n2, GLint n3, GLint n4, double depth)
{
    glBegin(GL_QUADS);
    glVertex3f(pt[n1][0], pt[n1][1], (pt[n1][2] + 1) * depth / 2 - 1);
    glVertex3f(pt[n2][0], pt[n2][1], (pt[n2][2] + 1) * depth / 2 - 1);
    glVertex3f(pt[n3][0], pt[n3][1], (pt[n3][2] + 1) * depth / 2 - 1);
    glVertex3f(pt[n4][0], pt[n4][1], (pt[n4][2] + 1) * depth / 2 - 1);
    glEnd();
}


void cube(double depth)
{
    glColor4f(0.6, 0.6, 0.6, .5);
    glNormal3f(0, 0, -1);
    quad(2, 0, 1, 3, depth);//FLOOR, Z=0
    glColor4f(0.0, 1.0, 1.0, 0.5);
    glNormal3f(-1, 0, 0);
    quad(5, 1, 0, 4, depth);//X=0
    glColor4f(0.0, 0.0, 1.0, 0.5);//BLUE
    glNormal3f(0, 1, 0);
    quad(7, 3, 1, 5, depth);//Y=100
    glColor4f(1.0, 1.0, 0.0, 0.5);
    glNormal3f(0, -1, 0);
    quad(4, 0, 2, 6, depth);//Y=0
    glColor4f(1.0, 0.0, 1.0, .5);//GREEN
    glNormal3f(1, 0, 0);
    quad(6, 2, 3, 7, depth);//X=100
}


void renderBitmapString(float x, float y, void* font, const char* string) {
    const char* c;
    glRasterPos2f(x, y);
    for (c = string; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
}


Point diff(Point p2, Point  p1) {
    Point result = { p2.x - p1.x, p2.y - p1.y, p2.z - p1.z };
    return result;
}


double euclideanDistanceSquare(Point p1, Point p2) {
    return (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y) + (p1.z - p2.z) * (p1.z - p2.z);
}


static void init(void)
{
    glClearColor(1, 1, 1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0f, 0.0f, EYE_Z, //< eye position
        0.0f, 0.0f, 0.0f,  //< aim position
        0.0f, 1.f, 0.0f); //< up direction
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    //glDepthFunc(GL_LESS);//GL_GREATER, GL_ALWAYS, GL_NEVER


    GLfloat mat_specular[] = { 0.0, 1.0, 0.0,1 };
    GLfloat mat_specularback[] = { 1.0, 0.0, 0.0, 0.0 };
    GLfloat mat_shininess[] = { .2 };
    GLfloat mat_shininessback[] = { .5 };
    GLfloat light_position0[] = { -1, -1.0, 1.0, 0 };
    GLfloat light_position1[] = { 0.5, 0.5, 0.5, 0 };
    GLfloat ambient[] = { 1,1, 1, 0 };
    glShadeModel(GL_SMOOTH);

    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glMaterialfv(GL_BACK, GL_SPECULAR, mat_specularback);
    glMaterialfv(GL_BACK, GL_SHININESS, mat_shininessback);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
    glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, ambient);
    //glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);



    glEnable(GL_FOG);
    glFogi(GL_FOG_MODE, GL_EXP);
    glFogf(GL_FOG_START, DNEAR);
    glFogf(GL_FOG_END, EYE_Z);
    //glFogf(GL_FOG_COLOR, .8);
    glFogf(GL_FOG_DENSITY, .14);
    srand(time(NULL));
    for (int i = 0; i < 300000; i++)
        randomColors[i] = (double)rand() / RAND_MAX;
    for (int i = 0; i < NUMBER_OF_BUNNIES; i++)
        bunnyLocation[i] = randomBunnyLocation(2, 2, FIELD_DEPTH, i, i);
    firstIndex = 0;
    bodySegments = 1;
    snakeBody[0] = { 0,0,INITIAL_Z };
    snakeBody[1] = { INITIAL_LENGTH, 0, INITIAL_Z };
    const char* filename = LOW_RES ? "bunny-low-res.obj" : "bunny.obj";
    load_bunny_data(filename, vertices, faces, normals);
}


void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (defaultCamera)
    {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(40, (GLfloat)800 / (GLfloat)800, SPEED, 100.0);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(
            x, y, EYE_Z,
            x, y, 0.0,
            0.0, 1.0, 0.0);
    }
    else
    {
        Point ultimate = snakeBody[
            (bodySegments + firstIndex) % MAX_SEGMENTS];
        Point penultimate = snakeBody[
            (bodySegments + firstIndex - 1 + MAX_SEGMENTS) % MAX_SEGMENTS];
        Point d = diff(ultimate, penultimate);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(40, (GLfloat)800 / (GLfloat)800, SPEED, 100.0);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(
            ultimate.x, ultimate.y, ultimate.z,
            ultimate.x + d.x, ultimate.y + d.y, ultimate.z + d.z,
            0.0, 1.0, 0.0);
    }
    if (gameover) {
        glColor4f(1, 0, 0, ((double)blinking) / BLINKING_PERIOD);
        renderBitmapString(-0.04 * EYE_Z, 0.1, (void*)font, "GAME OVER");
    }
    if (!moving) {
        glColor4f(0, 0, 1, ((double)blinking) / BLINKING_PERIOD);
        renderBitmapString(-0.09 * EYE_Z, .15 * EYE_Z, (void*)font, "PRESS SPACE TO START");
    }
    glScalef(scaleFactor, scaleFactor, scaleFactor);
    glRotatef(rotateX, 1, 0, 0);
    glRotatef(rotateY, 0, 1, 0);
    glRotatef(rotateZ, 0, 0, 1);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //glDisable(GL_CULL_FACE);
    glDepthMask(GL_FALSE);//make the depth buffer read-only
    cube(FIELD_DEPTH);//draw the borders of the field
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_FRONT);
    glDepthMask(GL_TRUE);//make the detpth buffer read-write permission set to true
    glPolygonMode(GL_FRONT, GL_FILL);
    //glNormal3f(0, 0, 1);
    conicSnake(THICKNESS, TAIL_THICKNESS, color, snakeUp);
    //glCullFace(GL_BACK);
    for (int i = 0; i < NUMBER_OF_BUNNIES; i++) {
        double brightness = 1.0;// = (bunnyLocation[i].z + 1) / 2;
        glColor4f(brightness, .3 * brightness, .3 * brightness, .9);
        glPushMatrix();
        glTranslatef(bunnyLocation[i].x, bunnyLocation[i].y, bunnyLocation[i].z);
        glRotatef(bunnyRotDegree + (i + 1) * 360.0 / NUMBER_OF_BUNNIES, 0, 1, 0);
        glTranslatef(-bunnyLocation[i].x, -bunnyLocation[i].y, -bunnyLocation[i].z);
        bunny(BUNNY_SIZE, bunnyLocation[i]);
        glPopMatrix();
    }
    glutSwapBuffers();
    glFlush();
}


void reshape(int w, int h)
{
    width = w;
    height = h;
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(30, (GLfloat)w / (GLfloat)h, SPEED, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(
        x, y, EYE_Z,
        x, y, 0.0,
        0.0, 1.0, 0.0
    );
}


void keyboard(unsigned char key, int x, int y)
{
    glMatrixMode(GL_MODELVIEW);
    switch (key)
    {
        case 'x':
            rotateX -= 0.1;
            rotateX = (rotateX > 360) ? (rotateX - 360) : 
                        ((rotateX < 0) ? rotateX + 360 : rotateX);
            break;
        case 'X':
            rotateX += 0.1;
            rotateX = (rotateX > 360) ? (rotateX - 360) : 
                        ((rotateX < 0) ? rotateX + 360 : rotateX);
            break;
        case 'y':
            rotateY -= 0.1;
            rotateY = (rotateY > 360) ? (rotateY - 360) : 
                        ((rotateY < 0) ? rotateY + 360 : rotateY);
            break;
        case 'Y':
            rotateY += 0.1;
            rotateY = (rotateY > 360) ? (rotateY - 360) : 
                        ((rotateY < 0) ? rotateY + 360 : rotateY);
            break;
        case 'z':
            rotateZ -= 0.1;
            rotateZ = (rotateZ > 360) ? (rotateZ - 360) : 
                        ((rotateZ < 0) ? rotateZ + 360 : rotateZ);
            break;
        case 'Z':
            rotateZ += 0.1;
            rotateZ = (rotateZ > 360) ? (rotateZ - 360) : 
                        ((rotateZ < 0) ? rotateZ + 360 : rotateZ);
            break;
        case '+':
            scaleFactor += 0.05;
            break;
        case '-':
            scaleFactor -= 0.05;
            break;
        case 'i': case 'I':
            glLoadIdentity();
            gluLookAt(0, 0, EYE_Z, 0, 0, 0, 0, 1, 0);
            x = 0.0, y = 0.0, scaleFactor = 1, rotateX = 0, rotateY = 0, rotateZ = 0;
            glutPostRedisplay();
            break;
        case ' ':
            if (!gameover)
                moving = !moving;
            break;
        case '1':
            defaultCamera = true;
            break;
        case '2':
            defaultCamera = false;
            break;
        case 27:
            exit(0);
            break;
    }
    glutPostRedisplay();
}


void pressSpecialKey(int key, int xx, int yy)
{
    int current = (bodySegments + firstIndex - 1) % (MAX_SEGMENTS);
    int next = (current + 1) % (MAX_SEGMENTS);
    Point p = diff(snakeBody[next], snakeBody[current]);
    double len = (p.x < 0 ? -p.x : p.x) + (p.y < 0 ? -p.y : p.y) + (p.z < 0 ? -p.z : p.z);
    Point change = { 0,0,0 };
    switch (key) {
    case GLUT_KEY_UP:
        if (len >= 6 * THICKNESS && moving)
        {
            bodySegments++;
            if (p.x != 0) {
                if (defaultCamera)
                {
                    change.y = SPEED;
                    if (snakeUp.y != 0)
                        snakeUp = { p.x > 0 ? -1.0 : 1.0,0,0 };
                }
                else
                {
                    if (snakeUp.z != 0)
                        change.z = snakeUp.z > 0 ? SPEED : -SPEED;
                    else
                        change.y = snakeUp.y > 0 ? SPEED : -SPEED;
                    snakeUp = { p.x > 0 ? -1.0 : 1.0, 0, 0 };
                }
            }
            else if (p.y != 0) {
                if (defaultCamera) change.z = SPEED;
                else
                {
                    if (snakeUp.z != 0)
                        change.z = snakeUp.z > 0 ? SPEED : -SPEED;
                    else
                        change.x = snakeUp.x > 0 ? SPEED : -SPEED;
                }
                snakeUp = { 0, p.y > 0.0 ? -1.0 : 1, 0 };
            }
            else {
                if (defaultCamera) change.y = SPEED;
                else
                {
                    if (snakeUp.y != 0)
                        change.y = snakeUp.y > 0 ? SPEED : -SPEED;
                    else
                        change.x = snakeUp.x > 0 ? SPEED : -SPEED;
                }
                snakeUp = { 0, 0, p.z > 0 ? -1.0 : 1.0 };
            }
            snakeBody[(bodySegments + firstIndex) % MAX_SEGMENTS] =
            { snakeBody[next].x + change.x, snakeBody[next].y + change.y, snakeBody[next].z + change.z };
        }
        break;
    case GLUT_KEY_DOWN:
        if (len >= 6 * THICKNESS && moving)
        {
            bodySegments++;
            if (p.x != 0) {
                if (defaultCamera)
                {
                    change.y = -SPEED;
                    if (snakeUp.y != 0)
                        snakeUp = { p.x > 0 ? 1.0 : -1.0,0,0 };
                }
                else
                {
                    if (snakeUp.z != 0)
                        change.z = snakeUp.z > 0 ? -SPEED : SPEED;
                    else
                        change.y = snakeUp.y > 0 ? -SPEED : SPEED;
                    snakeUp = { p.x > 0 ? 1.0 : -1.0, 0, 0 };
                }
            }
            else if (p.y != 0) {
                if (defaultCamera) change.z = -SPEED;
                else {
                    if (snakeUp.z != 0)
                        change.z = snakeUp.z > 0 ? -SPEED : SPEED;
                    else
                        change.x = snakeUp.x > 0 ? -SPEED : SPEED;
                }
                snakeUp = { 0, p.y > 0 ? 1.0 : -1.0, 0 };
            }
            else {
                if (defaultCamera) change.y = -SPEED;
                else {
                    if (snakeUp.y != 0)
                        change.y = snakeUp.y > 0 ? -SPEED : SPEED;
                    else
                        change.x = snakeUp.x > 0 ? -SPEED : SPEED;
                }
                snakeUp = { 0, 0, p.z > 0 ? 1.0 : -1.0 };
            }
            snakeBody[(bodySegments + firstIndex) % MAX_SEGMENTS] =
            { snakeBody[next].x + change.x, snakeBody[next].y + change.y, snakeBody[next].z + change.z };
        }
        break;
    case GLUT_KEY_RIGHT:
        if (len >= 6 * THICKNESS && moving)
        {
            bodySegments++;
            if (p.x != 0) {
                if (defaultCamera)
                {
                    change.z = SPEED;
                    snakeUp = { p.x > 0 ? -1.0 : 1.0, 0, 0 };
                }
                else {
                    if (snakeUp.z != 0)
                        change.y = snakeUp.z * p.x > 0 ? -SPEED : SPEED;
                    else
                        change.z = snakeUp.y > 0 * p.x ? SPEED : -SPEED;
                }
            }
            else if (p.y != 0) {
                if (defaultCamera) change.x = SPEED;
                else {
                    if (snakeUp.z != 0)
                        change.x = snakeUp.z * p.y > 0 ? SPEED : -SPEED;
                    else
                        change.z = snakeUp.x * p.y > 0 ? -SPEED : SPEED;
                }

            }
            else {
                if (defaultCamera) change.x = SPEED;
                else {
                    if (snakeUp.y != 0)
                        change.x = p.z * snakeUp.y > 0 ? -SPEED : SPEED;
                    else
                        change.y = p.z * snakeUp.x > 0 ? SPEED : -SPEED;
                }
            }
        }
        snakeBody[(bodySegments + firstIndex) % MAX_SEGMENTS] = {
            snakeBody[next].x + change.x,
            snakeBody[next].y + change.y,
            snakeBody[next].z + change.z
        };
        break;
    case GLUT_KEY_LEFT:
        if (len >= 6 * THICKNESS && moving)
        {
            bodySegments++;
            if (p.x != 0) {
                if (defaultCamera)
                {
                    change.z = -SPEED;
                    snakeUp = { p.x > 0 ? 1.0 : -1.0, 0, 0 };
                }
                else {
                    if (snakeUp.z != 0)
                        change.y = snakeUp.z * p.x > 0 ? SPEED : -SPEED;
                    else
                        change.z = snakeUp.y > 0 * p.x ? -SPEED : SPEED;
                }
            }
            else if (p.y != 0) {
                if (defaultCamera) change.x = -SPEED;
                else {
                    if (snakeUp.z != 0)
                        change.x = snakeUp.z * p.y > 0 ? -SPEED : SPEED;
                    else
                        change.z = snakeUp.x * p.y > 0 ? SPEED : -SPEED;
                }
            }
            else {
                if (defaultCamera)
                {
                    change.x = -SPEED;
                }
                else {
                    if (snakeUp.y != 0)
                        change.x = p.z * snakeUp.y > 0 ? SPEED : -SPEED;
                    else
                        change.y = p.z * snakeUp.x > 0 ? -SPEED : SPEED;
                }
                if (snakeUp.x != 0)
                    snakeUp = { 0, 0, snakeUp.x > 0 ? -1.0 : 1.0 };
            }
        }
        snakeBody[(bodySegments + firstIndex) % MAX_SEGMENTS] = {
            snakeBody[next].x + change.x,
            snakeBody[next].y + change.y,
            snakeBody[next].z + change.z
        };
        break;
    }
}

void releaseSpecialKey(int key, int x, int y)
{
    switch (key)
    {
        case GLUT_KEY_HOME: deltaYMove -= 0.1; break;
        case GLUT_KEY_END: deltaYMove += 0.1; break;
        case GLUT_KEY_PAGE_UP: deltaXMove -= 0.1; break;
        case GLUT_KEY_INSERT: deltaXMove += 0.1; break;
    }
}


void update(void)// what changes has to be made to the program's parameters to generate the next frame of the animation
{
    x += deltaXMove * 0.003 * scaleFactor;
    y += deltaYMove * 0.003 * scaleFactor;
    bunnyRotDegree = bunnyRotDegree > 360 ? 1 : bunnyRotDegree + 1;
    blinking = (blinking + 1) % BLINKING_PERIOD;
    if (moving && !gameover) {
        Point dTail = diff(snakeBody[(1 + firstIndex) % (MAX_SEGMENTS)], snakeBody[firstIndex]);
        Point dHead = diff(snakeBody[(bodySegments + firstIndex) % (MAX_SEGMENTS)],
            snakeBody[(firstIndex + bodySegments - 1) % (MAX_SEGMENTS)]);
        double val = abs(dTail.x + dTail.y + dTail.z);
        if (val <= 2 * THICKNESS) {
            if (dHead.x != 0)
                snakeBody[(bodySegments + firstIndex) % MAX_SEGMENTS].x += dHead.x > 0 ? SPEED * (M_PI - 2) / 2 : -SPEED * (M_PI - 2) / 2;
            else if (dHead.y != 0)
                snakeBody[(bodySegments + firstIndex) % MAX_SEGMENTS].y += dHead.y > 0 ? SPEED * (M_PI - 2) / 2 : -SPEED * (M_PI - 2) / 2;
            else
                snakeBody[(bodySegments + firstIndex) % MAX_SEGMENTS].z += dHead.z > 0 ? SPEED * (M_PI - 2) / 2 : -SPEED * (M_PI - 2) / 2;
        }
        if (val <= SPEED) {
            bodySegments--;
            firstIndex = (firstIndex + 1) % (MAX_SEGMENTS);
            dTail = diff(snakeBody[(1 + firstIndex) % (MAX_SEGMENTS)], snakeBody[firstIndex]);
            if (dTail.x != 0)
                snakeBody[firstIndex].x += dTail.x > 0 ? (M_PI - 2) * THICKNESS : -(M_PI - 2) * THICKNESS;
            else if (dTail.y != 0)
                snakeBody[firstIndex].y += dTail.y > 0 ? (M_PI - 2) * THICKNESS : -(M_PI - 2) * THICKNESS;
            else
                snakeBody[firstIndex].z += dTail.z > 0 ? (M_PI - 2) * THICKNESS : -(M_PI - 2) * THICKNESS;
        }
        else if (eaten == 0)
            if (dTail.x != 0)
                snakeBody[firstIndex].x += dTail.x > 0 ? SPEED : -SPEED;
            else if (dTail.y != 0)
                snakeBody[firstIndex].y += dTail.y > 0 ? SPEED : -SPEED;
            else
                snakeBody[firstIndex].z += dTail.z > 0 ? SPEED : -SPEED;
        else
            eaten--;
        if (dHead.x != 0)
            snakeBody[(bodySegments + firstIndex) % MAX_SEGMENTS].x += dHead.x > 0 ? SPEED : -SPEED;
        else if (dHead.y != 0)
            snakeBody[(bodySegments + firstIndex) % MAX_SEGMENTS].y += dHead.y > 0 ? SPEED : -SPEED;
        else
            snakeBody[(bodySegments + firstIndex) % MAX_SEGMENTS].z += dHead.z > 0 ? SPEED : -SPEED;

        if (abs(snakeBody[(bodySegments + firstIndex) % MAX_SEGMENTS].x) >= 1 ||
            abs(snakeBody[(bodySegments + firstIndex) % MAX_SEGMENTS].y) >= 1 ||
            snakeBody[(bodySegments + firstIndex) % MAX_SEGMENTS].z >= EYE_Z - 1 ||
            snakeBody[(bodySegments + firstIndex) % MAX_SEGMENTS].z <= -1 ||
            snakeCrossesItself()) {
            gameover = true;
        }
        int ate = eatsBunny();
        if (ate >= 0) {
            eaten = GROW_WHEN_EAT;
            bunnyLocation[ate] = randomBunnyLocation(2, 2, FIELD_DEPTH, NUMBER_OF_BUNNIES, ate);
        }
    }

    glutPostRedisplay(); // redisplay everything
}
int main()
{
    int argc = 1;
    char* argv[1] = { (char*)"3D Snakes" };
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(300, 200);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutCreateWindow(argv[0]);
    init();
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutDisplayFunc(display);
    glutSpecialFunc(pressSpecialKey);
    glutSpecialUpFunc(releaseSpecialKey);
    glutIdleFunc(update); // incremental update 
    glutMainLoop();
    return 0;
}





