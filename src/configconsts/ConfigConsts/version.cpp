/*
 * version.cpp
 *
 */
#include "ConfigConsts/version.hpp"

#define GEN_VER_CONSTS(x,y,z) const char * const versionString="v" #x "." #y "." #z; \
                              const unsigned int versionMain =x; \
                              const unsigned int versionMajor=y; \
                              const unsigned int versionMinor=z;

namespace ConfigConsts
{
// NOTE: since there is a problem to connect variables that are macros as well
//       (num->str+concat doe snot work as expected) a hack macro has been provided internally
//       to overcome this and generate nice, compile-time assignments.
GEN_VER_CONSTS(1,1,1);
} // unnamed namespace
