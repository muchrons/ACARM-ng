/*
 * version.hpp
 *
 */
#ifndef INCLUDE_CONFIGCONSTS_VERSION_HPP_FILE
#define INCLUDE_CONFIGCONSTS_VERSION_HPP_FILE

/* public header */

namespace ConfigConsts
{

/** \brief compile-time knonw software version string. */
extern const char * const versionString;

/** \brief main version number vX.y.z. */
extern const unsigned int versionMain;
/** \brief smller version number vx.Y.z. */
extern const unsigned int versionMajor;
/** \brief bugfix version number vx.y.Z. */
extern const unsigned int versionMinor;

} // namespace ConfigConsts

#endif
