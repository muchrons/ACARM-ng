/*
 * printBanner.mt.cpp
 *
 */
#include <iostream>

#include "AcarmNG/printBanner.hpp"
#include "AcarmNG/randomizeSeed.hpp"

int main(int /*argc*/, char const * const * const argv)
{
  AcarmNG::randomizeSeed();
  AcarmNG::printBanner(std::cout, argv[0]);
  return 0;
}
