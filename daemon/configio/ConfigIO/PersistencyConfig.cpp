/*
 * PersistencyConfig.cpp
 *
 */
#include <sstream>
#include <string>

#include "ConfigIO/PersistencyConfig.hpp"

using namespace std;


namespace ConfigIO
{

PersistencyConfig::PersistencyConfig(const TypeName &type,
                                     const Options  &options):
  type_(type),
  options_(options)
{
}

} // namespace ConfigIO
