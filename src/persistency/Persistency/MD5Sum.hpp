/*
 * MD5Sum.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_MD5SUM_HPP_FILE
#define INCLUDE_PERSISTENCY_MD5SUM_HPP_FILE

/* public hedaer */

#include "Persistency/Exception.hpp"
#include "Persistency/ExceptionNULLParamter.hpp"

namespace Persistency
{

/** \brief internal MD5 string.
 */
class ExceptionInvalidMD5String: public Exception
{
public:
  /** \brief create execption with given message.
   *  \param where place of creation
   *  \param md5   md5 that has been found invalid
   */
  ExceptionInvalidMD5String(const char *where, const char *md5):
    Exception(where,
              std::string("invalid MD5 string: '") + ensureString(md5) + "'")
  {
  }
}; // class ExceptionInvalidMD5String


/** \brief class holding MD5 sum.
 */
class MD5Sum
{
public:
  /** \brief named c-tor creating MD5Sum.
   *  \param md5 string to hold.
   *  \return already build object.
   */
  static MD5Sum createFromString(const char *md5)
  {
    return MD5Sum(md5);
  }
  /** \brief gets md5 string.
   *  \return pointer to md5 string.
   */
  const char *get(void) const
  {
    return str_;
  }

private:
  explicit MD5Sum(const char *str);

  enum { MD5_STR_LEN=32 };
  char str_[MD5_STR_LEN+1];
}; // class MD5Sum

} // namespace Persistency

#endif
