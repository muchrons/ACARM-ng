/*
 * Value.hpp
 *
 * simple representation of value for a given node.
 *
 */
#ifndef INCLUDE_XML_VALUE_HPP_FILE
#define INCLUDE_XML_VALUE_HPP_FILE

/* public header */

#include <string>
#include <boost/operators.hpp>


namespace XML
{

/**
 * class representing value as string.
 */
class Value: public boost::equality_comparable<Value>
{
public:
  /**
   * creates empty value.
   */
  Value(void):
    value_("")
  {
  }
  /**
   * creates value from a given string.
   * \param value value to be set.
   */
  explicit Value(const std::string &value):
    value_(value)
  {
  }
  /**
   * accessor to internal data.
   * \return stored value.
   */
  const std::string &get(void) const
  {
    return value_;
  }
  /**
   * comparison operator.
   * \param val other value to compare with.
   * \return true if values are equal.
   */
  bool operator==(const Value &val) const
  {
    return get()==val.get();
  }

private:
  std::string value_;
}; // class Value

} // namespace XML

#endif

