/*
 * ReaderHelper.hpp
 *
 */

#ifndef INCLUDE_PERSISTENCY_IO_POSTGRES_READERHELPER_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_POSTGRES_READERHELPER_HPP_FILE

#include <string>
#include <pqxx/pqxx>

#include "Base/NullValue.hpp"
#include "Persistency/Timestamp.hpp"
#include "Persistency/IO/Postgres/timestampFromString.hpp"
namespace Persistency
{
namespace IO
{
namespace Postgres
{

/** \brief helper class which gets values from SQL queries
 */
template<typename T, typename S>
class ReaderHelper
{
public:
// 

static Base::NullValue<T> readAs(const pqxx::result::field &r)
{
  if( r.is_null() )
  {
    Base::NullValue<T> ret;
    return ret;
  }
  else
  {
    S s;
    r.to(s);
    Base::NullValue<T> ret( T(s) );
    return ret;
  }
}

// TODO: make this partial specialization of set() method.
// NOTE: partial specialization is a bit tricky - you may want to make class with
//       template methods instead, just as Append is done.
// NOTE: you can separate it to own header - it will make testing easier.
static Base::NullValue<T> readToString(const pqxx::result::field &r)
{
  if( r.is_null() )
  {
    Base::NullValue<T> ret;
    return ret;
  }
  else
  {
    S s;
    r.to(s);
    // TODO: memory leak here.
    Base::NullValue<T> ret( T::from_string(s) );
    return ret;
  }
}

}; // ReaderHelper class

template<typename S>
class ReaderHelper<Persistency::Timestamp, S>
{
// TODO: make this partial specialization of set() method.
static Base::NullValue<Persistency::Timestamp> readAs(const pqxx::result::field &r)
{
  if( r.is_null() )
  {
    Base::NullValue<Timestamp> ret;
    return ret;
  }
  else
  {
    std::string s;
    r.to(s);
    Base::NullValue<Timestamp> ret( new Timestamp( timestampFromString(s) ) );
    return ret;
  }
}
}; // ReaderHelper class partial specjalization

} // namespace Postgres
} // namespace IO
} // namespace Persistency

#endif
