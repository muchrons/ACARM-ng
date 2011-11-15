/*
 * randomizeSeed.cpp
 *
 */
#include <stdexcept>
#include <fstream>
#include <cstdlib>

#include "AcarmNG/randomizeSeed.hpp"


namespace AcarmNG
{

ExceptionCannotInitRand::ExceptionCannotInitRand(const Location &where, const char *msg):
  Exception(where, cc("cannot init random number generator: ", msg))
{
}


void randomizeSeed(void)
{
  // open file
  std::ifstream urand;
  urand.open("/dev/urandom", std::fstream::in);
  if( !urand.is_open() )
    throw ExceptionCannotInitRand(SYSTEM_SAVE_LOCATION, "unable to open /dev/urandom for reading");

  // read random seed
  unsigned int seed=0;
  urand.read(static_cast<char*>(static_cast<void*>(&seed)), sizeof(seed));
  if( !urand.good() )
    throw ExceptionCannotInitRand(SYSTEM_SAVE_LOCATION, "unable to read (enough) data from /dev/urandom");

  // set is as a seed for random number generator (rand*())
  srand(seed);
}

} // namespace AcarmNG
