/*
 * DShieldParser.hpp
 *
 */
#ifndef INCLUDE_FILTER_IPBLACKILIST_DSHIELDPARSER_HPP_FILE
#define INCLUDE_FILTER_IPBLACKILIST_DSHIELDPARSER_HPP_FILE

#include <string>
#include <boost/operators.hpp>
#include <boost/noncopyable.hpp>

#include "Filter/IPBlackList/IP.hpp"
#include "Filter/IPBlackList/ExceptionInvalidFormat.hpp"

namespace Filter
{
namespace IPBlackList
{

class DShieldParser: private boost::noncopyable
{
public:
  class const_iterator: public boost::equality_comparable<const_iterator>
  {
  public:
    explicit const_iterator(const char *str=NULL);
    const const_iterator &operator++(void);
    bool operator==(const const_iterator &other) const;
    IP operator*(void) const;

  private:
    const char *str_;
  }; // class const_iteator

  explicit DShieldParser(const std::string &data);

  const_iterator begin(void) const;
  const_iterator end(void) const;

private:
  const std::string &data_;
}; // class DShieldParser

} // namespace IPBlackList
} // namespace Filter

#endif
