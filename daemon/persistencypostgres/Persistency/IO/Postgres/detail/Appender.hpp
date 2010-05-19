/*
 * Appender.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IO_POSTGRES_DETAIL_APPENDER_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_POSTGRES_DETAIL_APPENDER_HPP_FILE

#include <sstream>
#include <string>
#include <pqxx/pqxx>

#include "Persistency/Analyzer.hpp"
#include "Persistency/Timestamp.hpp"

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

  /** \brief appends given data to statement.
   *  \param ss output stream.
   *  \param t  element to be added.
   */
  template<typename T>
  static void append(std::stringstream &ss, const T &t);
private:
  inline static void appendEscape(std::stringstream &ss, const char *t)
  {
    if(t==NULL)
      ss << "NULL";
    else
      ss << "'" << pqxx::sqlesc(t) << "'";
  }

  template<typename T>
  inline static void appendNoEscape(std::stringstream &ss, const T *t)
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

/** \brief appends given pointer's data to statement.
 *  \param ss output stream.
 *  \param t  pointer to element to be added.
 */
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

template<>
inline void Appender::append<Persistency::Timestamp>(std::stringstream            &ss,
                                                     const Persistency::Timestamp &t)
{
  // this long lithany ensures that timestamp (already in UTC) is saved in UTC as well.
  ss << t.get() << "::abstime::timestamp WITH TIME ZONE AT TIME ZONE 'UTC'";
}

template<>
inline void Appender::append<Persistency::Timestamp>(std::stringstream            &ss,
                                                     const Persistency::Timestamp *t)
{
  if(t==NULL)
    ss << "NULL";
  else
    append(ss, *t);
}

template<typename T>
inline void Appender::append(std::stringstream &ss, const T &t)
{
  appendNoEscape(ss, &t);
}

template<>
inline void Appender::append<std::string>(std::stringstream &ss, const std::string &t)
{
  appendEscape(ss, t.c_str() );
}

} // namespace detail
} // namespace Postgres
} // namespace IO
} // namespace Persistency

#endif
