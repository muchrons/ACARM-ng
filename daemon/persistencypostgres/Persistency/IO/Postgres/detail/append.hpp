/*
 * append.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IO_POSTGRES_DETAIL_APPEND_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_POSTGRES_DETAIL_APPEND_HPP_FILE

#include <sstream>
#include <string>
#include <pqxx/pqxx>

namespace Persistency
{
namespace IO
{
namespace Postgres
{
namespace detail
{

/** \brief class with string-stream appenders.
 */
class Appender
{
public:
  /** \brief appends given pointer's data to statement.
   *  \param ss output stream.
   *  \param t  pointer to element to be added.
   */
  template<typename T>
  static void append(std::stringstream &ss, const T *t);

private:
  static void appendEscape(std::stringstream &ss, const char *t)
  {
    if(t==NULL)
      ss << "NULL";
    else
      ss << "'" << pqxx::sqlesc(t) << "'";
  }

  template<typename T>
  static void appendNoEscape(std::stringstream &ss, const T *t)
  {
    if(t==NULL)
      ss << "NULL";
    else
      ss << *t;
  }
}; // struct Appender



template<typename T>
inline void Appender::append(std::stringstream &ss, const T *t)
{
  appendNoEscape(ss, t);
}

template<>
inline void Appender::append<char>(std::stringstream &ss, const char *t)
{
  appendEscape(ss, t);
}

template<>
inline void Appender::append<std::string>(std::stringstream &ss, const std::string *t)
{
  appendEscape(ss, (t?t->c_str():NULL) );
}

} // namespace detail
} // namespace Postgres
} // namespace IO
} // namespace Persistency

#endif
