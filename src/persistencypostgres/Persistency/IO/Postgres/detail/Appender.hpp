/*
 * Appender.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IO_POSTGRES_DETAIL_APPENDER_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_POSTGRES_DETAIL_APPENDER_HPP_FILE

#include <sstream>
#include <string>
#include <pqxx/pqxx>
#include <boost/noncopyable.hpp>

#include "Persistency/Analyzer.hpp"
#include "Persistency/Timestamp.hpp"
#include "Persistency/IO/Transaction.hpp"
#include "Persistency/IO/Postgres/TransactionAPI.hpp"

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
class Appender: private boost::noncopyable
{
public:

  /** \brief
   *  \param tr
   */
  explicit Appender(Transaction &tr):
    tr_(tr)
  {
  }

  /** \brief appends given pointer's data to statement.
   *  \param ss output stream.
   *  \param t  pointer to element to be added.
   */

  template<typename T>
  void append(std::stringstream &ss, const T *t);
  /** \brief appends given data to statement.
   *  \param ss output stream.
   *  \param t  element to be added.
   */
  template<typename T>
  void append(std::stringstream &ss, const T &t);

private:
  inline void appendEscape(std::stringstream &ss, const char *t)
  {
    if(t==NULL)
      ss << "NULL";
    else
      ss << "E'" << tr_.getAPI<Postgres::TransactionAPI>().esc(t) << "'";
  }

  template<typename T>
  inline void appendNoEscape(std::stringstream &ss, const T *t)
  {
    if(t==NULL)
      ss << "NULL";
    else
      ss << *t;
  }

  Transaction &tr_;
}; // struct Appender


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
  appendEscape(ss, (t?t->c_str():NULL));
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

template<>
inline void Appender::append<std::string>(std::stringstream &ss, const std::string &t)
{
  appendEscape(ss, t.c_str());
}

template<typename T>
inline void Appender::append(std::stringstream &ss, const T *t)
{
  appendNoEscape(ss, t);
}


template<typename T>
inline void Appender::append(std::stringstream &ss, const T &t)
{
  appendNoEscape(ss, &t);
}



} // namespace detail
} // namespace Postgres
} // namespace IO
} // namespace Persistency

#endif
