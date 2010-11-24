/*
 * Config.hpp
 *
 */
#ifndef INCLUDE_LOGGER_CONFIG_HPP_FILE
#define INCLUDE_LOGGER_CONFIG_HPP_FILE

/* public header */

namespace Logger
{

/** \brief public API for configuration-related operations
 */
struct Config
{
  static void reinitAppenders(void);
}; // struct Config

} // namespace Logger

#endif
