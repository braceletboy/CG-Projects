#include <GL/glut.h>
#include <math.h>
#include "Tools.h"
GLfloat translated_quadratic(GLfloat x) {
    return 200 + (x - 300) * (x - 300) / 100;
}
GLfloat inverse1_translated_quadratic(GLfloat y) {
    if (y <= 200)
        return 300;
    return 300 + 10 * sqrt(y - 200);
}
GLfloat inverse2_translated_quadratic(GLfloat y) {
    if (y <= 200)
        return 300;
    return 300 - 10 * sqrt(y - 200);
}

GLfloat translated_sin(GLfloat x) {
    return 350 + 50 * sin(((double)x - 250) / 64.0);
}
GLfloat inverse1_translated_sin(GLfloat y) {
    if (y >= 400)
        return 350.5;
    if (y <= 300)
        return 149.5;
    return 250 + 64 * asin(((double)y - 350) / 50.0);
}
GLfloat inverse2_translated_sin(GLfloat y) {
    if (y >= 400)
        return 350.5;
    if (y <= 300)
        return 551.5;
    return 250 + 64 * (3.141592 - asin(((double)y - 350) / 50.0));
}
void draw_function_cartesian_system(GLfloat init_x, GLfloat final_x, GLfloat(*f)(GLfloat), GLfloat(*inverse)(GLfloat)) {
    GLfloat x, y, x_step = 0, y_step = 0, dx, dy;
    bool inc_x = final_x > init_x;
    bool inc_y = f(final_x) > f(init_x);
    for (x = init_x, y = f(init_x); ; x += x_step, y += y_step)
    {
        if (inc_x && x > final_x || !inc_x && x < final_x)
            break;
        glVertex2i(x, y);
        //Next choices: (x,y+1), (x,y-1), (x+1,y), (x+1,y+1), (x+1, y-1), (x-1,y), (x-1,y+1), (x-1,y-1)
        dx = inverse(y + (inc_y ? 1 : -1)) - x;
        dy = f(x + (inc_x ? 1 : -1)) - y;
        x_step = (dx >= .5) ? 1 : (dx > -.5) ? 0 : -1;
        y_step = (dy >= .5) ? 1 : (dy > -.5) ? 0 : -1;
        if (x_step == 0 && y_step == 0)
            break;
    }
}
Color getPixelColor(GLint x, GLint y) {
    Color color;
    glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, &color);
    return color;
}