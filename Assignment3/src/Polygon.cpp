#include "global.h"


GLubyte* toStipple(FillPattern p) {
    switch (p) {
    case FillPattern::SOLID:
        return solid;
    case FillPattern::HATCHED:
        return hatched;
    case FillPattern::HALFTONE:
        return halftone;
    case FillPattern::FLY:
        return fly;
    }
    return solid;  // default is solid
}


void fillPolygon(ShadedPolygon polygon)
{
    glBlendFunc(GL_ONE, GL_ZERO);
    glEnable(GL_POLYGON_STIPPLE);
    glPolygonStipple(toStipple(polygon.drawConfig.fillPattern));
    glBegin(GL_POLYGON);
    for (int i = 0; i < polygon.numVertices; i++)
    {
        if(polygon.drawConfig.fillColorPattern ==
            ColorPattern::GRADIENTCOLOR)
        {
            glColor4f(
                polygon.drawConfig.fillColors[
                    i%polygon.drawConfig.numFillColors].r,
                polygon.drawConfig.fillColors[
                    i%polygon.drawConfig.numFillColors].g,
                polygon.drawConfig.fillColors[
                    i%polygon.drawConfig.numFillColors].b,
                polygon.drawConfig.fillColors[
                    i%polygon.drawConfig.numFillColors].a
            );
        }
        else
        {
            glColor4f(
                polygon.drawConfig.fillColors[0].r,
                polygon.drawConfig.fillColors[0].g,
                polygon.drawConfig.fillColors[0].b,
                polygon.drawConfig.fillColors[0].a
            );
        }
        glVertex2i((int)polygon.vertices[i].x, 
                    (int)polygon.vertices[i].y);
    }
    glEnd();
    glDisable(GL_POLYGON_STIPPLE);
}


void drawPolygonBorder(ShadedPolygon polygon)
{
    glBlendFunc(GL_ONE, GL_ZERO);
    
    // remove previous lines
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < polygon.numVertices; i++)
    {
        glColor4f(1.0, 1.0, 1.0, 1.0);
        glVertex2i((int)polygon.vertices[i].x,
                    (int)polygon.vertices[i].y);
    }
    glEnd();

    glLineWidth(3);
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(3, polygon.drawConfig.borderPattern);

    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < polygon.numVertices; i++)
    {
        if(polygon.drawConfig.borderColorPattern ==
            ColorPattern::GRADIENTCOLOR)
        {
            glColor4f(
                polygon.drawConfig.borderColors[
                    i%polygon.drawConfig.numBorderColors].r,
                polygon.drawConfig.borderColors[
                    i%polygon.drawConfig.numBorderColors].g,
                polygon.drawConfig.borderColors[
                    i%polygon.drawConfig.numBorderColors].b,
                polygon.drawConfig.borderColors[
                    i%polygon.drawConfig.numBorderColors].a
            );
        }
        else
        {
            glColor4f(
                polygon.drawConfig.borderColors[0].r,
                polygon.drawConfig.borderColors[0].g,
                polygon.drawConfig.borderColors[0].b,
                polygon.drawConfig.borderColors[0].a
            );
        }
        glVertex2i((int)polygon.vertices[i].x,
                    (int)polygon.vertices[i].y);
    }
    glEnd();
    glDisable(GL_LINE_STIPPLE);
    glFlush();
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

/**
 * @brief Return the boundaries of the rectangle that contains the polygon
 * 
 * @param polygon The polygon
 * @param xFrom The starting x-coordinate of the rectangle
 * @param xTo The ending x-coordinate of the rectangle
 * @param yFrom The starting y-coordinate of the rectangle
 * @param yTo The ending y-coordinate of the rectangele
 */
void polygonContainingRectangle(
    ShadedPolygon &polygon,
    double &xFrom, double &xTo, double &yFrom, double &yTo
)
{
    xFrom = 1000000, yFrom = 1000000, xTo = -1000000, yTo = -1000000;
    double tcos = cos(polygon.rtheta * PI / 180);
    double tsin = sin(polygon.rtheta * PI / 180);
    double xc = polygon.centroidX;
    double yc = polygon.centroidY;

    // find boundaries of the polygon
    for (int i = 0; i < polygon.numVertices; i++)
    {
        double x = + polygon.tx + xc + polygon.sx * (
            (polygon.vertices[i].x - xc) * tcos -
            (polygon.vertices[i].y - yc) * tsin
        );
        double y = polygon.ty + yc + polygon.sy * (
            (polygon.vertices[i].x - xc) * tsin +
            (polygon.vertices[i].y - yc) * tcos
        );
        if (x > xTo)
            xTo = x;
        if (y > yTo)
            yTo = y;
        if (x < xFrom)
            xFrom = x;
        if (y < yFrom)
            yFrom = y;
    }
}


void polygonCentroidApprox(ShadedPolygon &polygon)
{
    double xSum = 0, ySum = 0;
    for(int idx = 0; idx < polygon.numVertices; idx++)
    {
        xSum += polygon.vertices[idx].x;
        ySum += polygon.vertices[idx].y;
    }
    polygon.centroidX = xSum/polygon.numVertices;
    polygon.centroidY = ySum/polygon.numVertices;
}


void polygonCentroidAccurate(ShadedPolygon &polygon)
{
    double totalArea = 0, centroidSumX = 0, centroidSumY = 0;
    for(int idx = 1; idx < polygon.numVertices-1; idx++)
    {
        double triangleArea = abs(
            0.5 * (
                (polygon.vertices[0].x*polygon.vertices[idx].y -
                 polygon.vertices[idx].x*polygon.vertices[0].y) +
                (polygon.vertices[idx].x*polygon.vertices[idx+1].y -
                 polygon.vertices[idx+1].x*polygon.vertices[idx].y) +
                (polygon.vertices[idx+1].x*polygon.vertices[0].y -
                 polygon.vertices[0].x*polygon.vertices[idx+1].y)
            )
        );
        double triangleCentroidX = (
            polygon.vertices[0].x +
            polygon.vertices[idx].x +
            polygon.vertices[idx+1].x
        )/3.0;
        double triangleCentroidY = (
            polygon.vertices[0].y +
            polygon.vertices[idx].y +
            polygon.vertices[idx+1].y
        )/3.0;
        centroidSumX += triangleArea*triangleCentroidX;
        centroidSumY += triangleArea*triangleCentroidY;
        totalArea += triangleArea;
    }
    polygon.centroidX = centroidSumX/totalArea;
    polygon.centroidY = centroidSumY/totalArea;
}
