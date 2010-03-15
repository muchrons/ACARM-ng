/*
 * Reader.cpp
 *
 */
#include "Input/Reader.hpp"

namespace Input
{

Reader::DataPtr Reader::read(unsigned int timeout)
{
  try
  {
    return readImpl(timeout);
  }
  catch(const Commons::Exception &ex)
  {
    // TODO
  }
  catch(const std::exception &ex)
  {
    // TODO
  }

  // if nothing happened return NULL pointer
  return DataPtr();
}

} // namespace Input
