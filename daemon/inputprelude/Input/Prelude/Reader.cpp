/*
 * Reader.cpp
 *
 */
#include <cassert>

#include "Input/Prelude/Reader.hpp"
#include "Persistency/Alert.hpp"

#include <idmef.hxx>                                                                                                                         
#include <prelude-client.hxx>

namespace Input
{
namespace Prelude
{

Reader::Reader(void):
  preludeLogger_("input.prelude.reader.preludelog")
{
}

Reader::DataPtr Reader::read(const unsigned int /*timeout*/)
{
  // TODO: implement
  DataPtr tmp;
  assert(tmp.get()==NULL);

  //IDMEF idmef;
  //client >> idmef;

  
  
  
  return tmp;//new Alert();
}

} // namespace Prelude
} // namespace Input
