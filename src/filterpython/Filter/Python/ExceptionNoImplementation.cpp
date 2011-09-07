#include "Filter/Python/ExceptionNoImplementation.hpp"

namespace Filter
{
namespace Python
{

ExceptionNoImplementation::ExceptionNoImplementation(const Location &where, const char *call):
  Exception(where, cc("method ", call, " is not implemented by Python script") )
{
}

} // namespace Python
} // namespace Filter
