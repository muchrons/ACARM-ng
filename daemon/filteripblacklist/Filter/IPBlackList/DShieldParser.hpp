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

/** \brief parser for dshield's black list.
 */
class DShieldParser: private boost::noncopyable
{
public:
  /** \brief const-iterator on IPs from dshield.
   */
  class const_iterator: public boost::equality_comparable<const_iterator>
  {
  public:
    /** \brief move on next element.
     *  \return self-reference.
     */
    const const_iterator &operator++(void);
    /** \brief compare if two iterators equals.
     *  \param other iterator to compare with.
     *  \return true if iterators equal, false otherwise.
     */
    bool operator==(const const_iterator &other) const;
    /** \brief get IP pointed by iterator.
     *  \return parsed IP.
     */
    IP operator*(void) const;

  private:
    friend class DShieldParser;

    explicit const_iterator(const char *str=NULL);

    const char *str_;
  }; // class const_iteator

  /** \brief create parser from given string.
   *  \param data string to be parsed.
   */
  explicit DShieldParser(const std::string &data);
  /** \brief gets begin iterator.
   *  \return iterator pointing to begin of collection.
   */
  const_iterator begin(void) const;
  /** \brief gets end iterator.
   *  \return iterator pointing to end of collection.
   */
  const_iterator end(void) const;

private:
  const std::string &data_;
}; // class DShieldParser

} // namespace IPBlackList
} // namespace Filter

#endif
