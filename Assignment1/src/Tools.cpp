#include <GL/glut.h>
#include <math.h>
#include "Tools.h"

GLfloat cosine(GLfloat x)
{
    return 25*(std::cos(((double)x)/25-1) + 4);
}

GLfloat inverse_cosine_1(GLfloat y)
{
    return 25*(1 + std::acos(((double)y)/25-4));
}

GLfloat inverse_cosine_2(GLfloat y)
{
    return 25*(1 - std::acos(((double)y)/25-4));
}

GLfloat inverse_cosine_3(GLfloat y)
{
    return 25*(1 + std::acos(((double)y)/25-4) + 2 * M_PI);
}

GLfloat quadratic_first_eq(GLfloat x)
{
    return 25*(((double)x)/25-3)*(((double)x)/25-3);
}

GLfloat inverse_quadratic_first_eq1(GLfloat y)
{
    return 25*(3 + std::sqrt(((double)y)/25));
}

GLfloat inverse_quadratic_first_eq2(GLfloat y)
{
    return 25*(3 - std::sqrt(((double)y)/25));
}

GLfloat inverse_cubic(GLfloat y)
{
    return 100*std::cbrt((double)y - 50) + 200;
}

GLfloat cubic(GLfloat x)
{
    return ((double)x - 200)*((double)x - 200)*((double)x - 200)/1000000 + 50;
}

GLfloat inverse_quadratic_second_eq2(GLfloat y)
{
    return 100 + 100 * std::sqrt( ((double)y)/3 - 15 );
}

GLfloat inverse_quadratic_second_eq1(GLfloat y)
{
    return 100 - 100 * std::sqrt( ((double)y)/3 - 15 );
}

GLfloat quadratic_second_eq(GLfloat x)
{
    return ((double)x - 200)*((double)x - 200)*3/10000 + 6*((double)x - 200)/100 + 48;
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
