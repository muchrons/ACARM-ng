/*
 * printBanner.hpp
 *
 */
#ifndef INCLUDE_ACARMNG_PRINTBANNER_HPP_FILE
#define INCLUDE_ACARMNG_PRINTBANNER_HPP_FILE

#include <iosfwd>


namespace AcarmNG
{

/** \brief priend greeting banner for application.
 *  \param os  stream to output data to.
 *  \param app application/binary name.
 */
void printBanner(std::ostream &os, const char *app);

} // namespace AcarmNG

#endif
