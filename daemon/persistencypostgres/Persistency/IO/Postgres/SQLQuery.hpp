/*
 * SQLQuery.hpp
 *
 */

#ifndef INCLUDE_PERSISTENCY_IO_POSTGRES_SQLQUERY_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_POSTGRES_SQLQUERY_HPP_FILE

#include <string>
#include <pqxx/pqxx>

#include "Base/NullValue.hpp"

namespace Persistency
{
namespace IO
{
namespace Postgres
{

/** \brief helper class which gets values from SQL queries
 */
class SQLQuery
{
public:

template<typename T, typename S>
Base::NullValue<T> readAs(const pqxx::result::field &r)
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
    T data(s);
    // TODO: memory leak here. should be ret(&s).
    Base::NullValue<T> ret( data );
    return ret;
  }
}


// TODO: make this partial specialization of set() method.
// NOTE: partial specialization is a bit tricky - you may want to make class with
//       template methods instead, just as Append is done.
// NOTE: you can separate it to own header - it will make testing easier.
template<typename T, typename S>
Base::NullValue<T> readToString(const pqxx::result::field &r)
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
    T data( T::from_string(s) );
    // TODO: memory leak here.
    Base::NullValue<T> ret( data );
    return ret;
  }
}

// TODO: make this partial specialization of set() method.
Base::NullValue<Timestamp> readAs(const pqxx::result::field &r)
{
  if( r.is_null() )
  {
    Base::NullValue<Timestamp> ret;
    return ret;
  }
  else
  {
    string s;
    r.to(s);
    Base::NullValue<Timestamp> ret( new Timestamp( timestampFromString(s) ) );
    return ret;
  }
}
}; // SQLQuery class

} // namespace Postgres
} // namespace IO
} // namespace Persistency

#endif
