#include "bitmap_image.hpp"
#include <iostream>

int main(int argc, char *argv[])
{
  bitmap_image image(argv[1]);
  if(!image)
    {
      std::cout << "Unable to open file. Exiting now." << std::endl;
      exit(EXIT_FAILURE);
    }

  std::cout << "Width of image (pixels): " << image.width() << std::endl;
  std::cout << "Height of image (pixels): " << image.height() << std::endl;
  std::cout << "Number of pixels: " << image.height()*image.width() << std::endl;

  return 0;
}
