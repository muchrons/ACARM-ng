/*
 * Mode.cpp
 *
 */
#include "Logger/Logger.hpp"
#include "Preprocessor/Checkers/Mode.hpp"

namespace Preprocessor
{
namespace Checkers
{

Mode::~Mode(void)
{
}

Mode::Mode(const std::string &name):
  log_( Logger::NodeName("preprocessor.checkers.mode", name.c_str() ) )
{
}

} // namespace Checkers
} // namespace Preprocessor
