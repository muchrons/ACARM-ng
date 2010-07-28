/*
 * Convert.hpp
 *
 */
#ifndef INCLUDE_COMMONS_CONVERT_HPP_FILE
#define INCLUDE_COMMONS_CONVERT_HPP_FILE

/* public header */

#include <string>
#include <cassert>

#include "Commons/Exception.hpp"
#include "Commons/detail/Converter.hpp"

namespace Commons
{

/** \brief class for performing convertions between different formats.
 */
class Convert
{
public:
  /** \brief thrown when convertion error has occured.
   */
  class ExceptionConvertionError: public Exception
  {
  public:
    /** \brief create execption with given message.
     *  \param where  place wxception has been thrown.
     *  \param value  value that convertion failed.
     *  \param reason reason of failure.
     */
    template<typename T>
    ExceptionConvertionError(const Location &where, const T &value, const std::string &reason):
      Exception(where, cc("convertion of '", value, "' failed: ", reason) )
    {
    }
  }; // class ExceptionConvertionError

  /** \brief converting method.
   *  \param f value to convert from.
   *  \return converted value.
   */
  template<typename T, typename F>
  static T to(const F &f)
  {
    try
    {
      return detail::Converter<T, F>::convert(f);
    }
    catch(const std::exception &ex)
    {
      throw ExceptionConvertionError(SYSTEM_SAVE_LOCATION, f, ex.what());
    }
    catch(...)  // fallback
    {
      throw ExceptionConvertionError(SYSTEM_SAVE_LOCATION, f, "unknown error");
    }
    assert(!"code never reaches here");
  }

private:
  Convert(void);    // no instances
}; // class Convert

} // namespace Commons

#endif
