/*
 * inputs.hpp
 *
 */
#ifndef INCLUDE_CONFIGCONSTS_INPUTS_HPP_FILE
#define INCLUDE_CONFIGCONSTS_INPUTS_HPP_FILE

/* public header */

namespace ConfigConsts
{

/** \brief compile-time known constant representing default name of the input type. */
extern const char * const defaultInputTypeName;

/** \brief compile-time known constant representing default name of the input instance. */
extern const char * const defaultInputInstanceName;

/** \brief compile-time known constant representing timeout to wait for alerts on input. */
extern const unsigned int inputTimeout;

/** \brief size of the input queue (to be used in templates). */
static const unsigned int  inputQueueSize=32;

} // namespace ConfigConsts

#endif
