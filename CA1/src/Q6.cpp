#include <iostream>
#include <cmath>

typedef struct
{
  /* data */
  double x;
  double y;
} Point;

double euclideanDistance(Point A, Point B)
{
  return std::sqrt((A.x - B.x)*(A.x - B.x) + (A.y - B.y)*(A.y - B.y));
}

double triangleArea(Point A, Point B, Point C)
{
  double a = euclideanDistance(B, C);
  double b = euclideanDistance(C, A);
  double c = euclideanDistance(A, B);

  double s = (a+b+c)/2;
  return std::sqrt(s * (s-a) * (s-b) * (s-c));
}

Point triangleCentroid(Point A, Point B, Point C)
{
  Point centroid;
  centroid.x = (A.x + B.x + C.x)/3;
  centroid.y = (A.y + B.y + C.y)/3;
  return centroid;
}

double quadArea(Point A, Point B, Point C, Point D)
{
  return triangleArea(A, B, D) + triangleArea(B, C, D);
}

Point quadCentroid(Point A, Point B, Point C, Point D)
{
  double abdArea = triangleArea(A, B, D);
  Point abdCentroid = triangleCentroid(A, B, D);
  double bcdArea = triangleArea(B, C, D);
  Point bcdCentroid = triangleCentroid(B, C, D);
  Point centroid;
  centroid.x = (abdArea*abdCentroid.x + bcdArea*bcdCentroid.x)/
    (abdArea + bcdArea);
  centroid.y = (abdArea*abdCentroid.y + bcdArea*bcdCentroid.y)/
    (abdArea + bcdArea);
  return centroid;
}

int main()
{
  int numVertices = 4;
  Point vertices[numVertices];
  for(int i = 0; i < numVertices; i++)
  {
    std::cout << "x-coordinate of vertex-" << i << ": ";
    std::cin >> vertices[i].x;
    std::cout << "y-coordinate of vertex-" << i << ": ";
    std::cin >> vertices[i].y;
  }
  std::cout << "Quadrilateral Area: " << quadArea(
    vertices[0], vertices[1], vertices[2], vertices[3]) << std::endl;
  Point centroid = quadCentroid(
    vertices[0], vertices[1], vertices[2], vertices[3]);
  std::cout << "Quadrilateral Centroid: (" << centroid.x << "," << centroid.y
    << ")" << std::endl;
}