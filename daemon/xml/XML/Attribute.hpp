/*
 * Attribute.hpp
 *
 */
#ifndef INCLUDE_XML_ATTRIBUTE_HPP_FILE
#define INCLUDE_XML_ATTRIBUTE_HPP_FILE

/* public header */

#include <string>

namespace XML
{

/**
 * Representation of attribute name="value".
 */
class Attribute
{
public:
  /**
   * creates attribute from name and value pair.
   * \param name  name of an attribute.
   * \param value value of an attribute.
   */
  Attribute(const std::string &name,
            const std::string value):
    name_(name),
    value_(value)
  {
  }
  /**
   * returns attribute name.
   * \return name of attribute.
   */
  const std::string &getName(void) const
  {
    return name_;
  }
  /**
   * gets attribute value.
   * \return value of attribute.
   */
  const std::string &getValue(void) const
  {
    return value_;
  }

private:
  std::string name_;
  std::string value_;
}; // class Attribute

} // namespace XML

#endif

