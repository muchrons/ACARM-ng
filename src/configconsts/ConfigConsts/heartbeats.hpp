/*
 * heartbeats.hpp
 *
 */
#ifndef INCLUDE_CONFIGCONSTS_HEARTBEATS_HPP_FILE
#define INCLUDE_CONFIGCONSTS_HEARTBEATS_HPP_FILE

/* public header */

namespace ConfigConsts
{

/** \brief number of heartbeats, that can be skipped before component is reported as dead. */
extern const unsigned int maxLostHeartbeats;

} // namespace ConfigConsts

#endif
