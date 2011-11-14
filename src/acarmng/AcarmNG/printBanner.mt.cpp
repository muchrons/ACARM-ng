/*
 * printBanner.mt.cpp
 *
 */
#include "AcarmNG/printBanner.hpp"
#include "AcarmNG/randomizeSeed.hpp"

int main(int /*argc*/, char const * const * const argv)
{
  AcarmNG::randomizeSeed();
  AcarmNG::printBanner(argv[0]);
  return 0;
}
