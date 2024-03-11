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
        glVertex2i(polygon.vertices[i].x, 
                    polygon.vertices[i].y);
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
        glVertex2i(polygon.vertices[i].x,
                    polygon.vertices[i].y);
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
        glVertex2i(polygon.vertices[i].x,
                    polygon.vertices[i].y);
    }
    glEnd();
    glDisable(GL_LINE_STIPPLE);
    glFlush();
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}


void polygonContainingRectangle(
    ShadedPolygon* polygons, int polygonIndex,
    double* xFrom, double* xTo, double* yFrom, double* yTo
)
{
    int i = 0;
    double xMin = 1000000, yMin = 1000000, xMax = -1000000, yMax = -1000000;
    double tcos = cos(polygons[polygonIndex].rtheta * PI / 180), tsin = sin(polygons[polygonIndex].rtheta * PI / 180);
    int xc = polygons[polygonIndex].centroidX, yc = polygons[polygonIndex].centroidY;
    while (polygons[polygonIndex].vertices[i].x >= 0) {
        double x = (polygons[polygonIndex].vertices[i].x - xc) * tcos - (polygons[polygonIndex].vertices[i].y - yc) * tsin
            , y = (polygons[polygonIndex].vertices[i].x - xc) * tsin + (polygons[polygonIndex].vertices[i].y - yc) * tcos;
        if (x > xMax)
            xMax = x;
        if (y > yMax)
            yMax = y;
        if (x < xMin)
            xMin = x;
        if (y < yMin)
            yMin = y;
        i++;
    }
    *xFrom = xMin * polygons[polygonIndex].sx;
    *yFrom = yMin * polygons[polygonIndex].sy;
    *xTo = xMax * polygons[polygonIndex].sx;
    *yTo = yMax * polygons[polygonIndex].sy;
}


void polygonCentroidApprox(ShadedPolygon &polygon)
{
    int i = 0;
    double xAvg = 0, yAvg = 0;
    while (polygon.vertices[i].x >= 0) {
        xAvg += polygon.vertices[i].x;
        yAvg += polygon.vertices[i].y;
        i++;
    }
    xAvg /= i;
    yAvg /= i;
    polygon.centroidX = xAvg;
    polygon.centroidY = yAvg;
}


void polygonCentroidAccurate(ShadedPolygon &polygon)
{
    float centroidX = 0, centroidY = 0, det = 0, tempDet = 0;
    int i = 0, j = 0;
    while (polygon.vertices[i].x >= 0) {
        if (polygon.vertices[i + 1].x < 0)
            j = 0;
        else
            j = i + 1;
        tempDet = polygon.vertices[i].x * polygon.vertices[j].y
            - polygon.vertices[j].x * polygon.vertices[i].y;
        det += tempDet;
        centroidX += (polygon.vertices[i].x + polygon.vertices[j].x) * tempDet;
        centroidY += (polygon.vertices[i].y + polygon.vertices[j].y) * tempDet;
        i++;
    }
    centroidX /= 3 * det;
    centroidY /= 3 * det;
    polygon.centroidX = centroidX;
    polygon.centroidY = centroidY;
}