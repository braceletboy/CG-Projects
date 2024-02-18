#include <iostream>
#include <math.h>

int main()
{
  double radius;
  std::cout << "Radius of the Sphere: ";
  std::cin >> radius;
  std::cout << "Volume of the Sphere: " << M_PI * std::pow(radius, 3) * 4 / 3;
  std::cout << std::endl;
}
