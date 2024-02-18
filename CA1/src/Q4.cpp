#include <iostream>

int main()
{
  int arr[10];
  for(int i = 0; i < 10; i++)
  {
    std::cout << "Input integer-" << i << ": ";
    std::cin >> arr[i];
  }
  for(int i=9; i>=0; i--)
  {
    std::cout << arr[i];
  }
  std::cout << std::endl;
}