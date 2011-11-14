/*
 * randomizeSeed.hpp
 *
 */
#ifndef INCLUDE_ACARMNG_RANDOMIZESEED_HPP_FILE
#define INCLUDE_ACARMNG_RANDOMIZESEED_HPP_FILE

#include "AcarmNG/Exception.hpp"

namespace AcarmNG
{

/** \brief exception thrown when initialization of random number generator fails.
 */
struct ExceptionCannotInitRand: public Exception
{
  /** \brief creates exception with a proper message.
   *  \param where exact location where error has been reported.
   *  \param msg   message exaplaining details of the error.
   */
  ExceptionCannotInitRand(const Location &where, const char *msg);
};


/** \brief initializes random number gnereation with srand().
 */
void randomizeSeed(void);

} // namespace AcarmNG

#endif
