/*
 * Section.hpp
 *
 */
#ifndef INCLUDE_CONFIGIO_PREPROCESSOR_SECTION_HPP_FILE
#define INCLUDE_CONFIGIO_PREPROCESSOR_SECTION_HPP_FILE

/* public header */

#include "System/Enum.hpp"
#include "ConfigIO/Preprocessor/Expression.hpp"

namespace ConfigIO
{
namespace Preprocessor
{
namespace detail
{
/** \brief implementation helper.
 */
struct SectionType
{
  /** \brief implementaiton enum. only to be used via Section::Type. */
  typedef enum
  {
    ACCEPT,
    REJECT
  }
  Type;
};
} // namespace detail


/** \brief class representing single section
 */
class Section
{
public:
  /** \brief type of rule ot be used. */
  typedef System::Enum<detail::SectionType> Type;

  /** \brief create section.
   *  \param type       section type.
   *  \param expression expression to compute.
   */
  Section(const Type type, const Expression &expression):
    type_(type),
    expression_(expression)
  {
  }

  /** \brief get section type.
   *  \return section type.
   */
  Type getType(void) const
  {
    return type_;
  }
  /** \brief gets expression object.
   *  \return expression object.
   */
  const Expression &getExpression(void) const
  {
    return expression_;
  }

private:
  Type       type_;
  Expression expression_;
}; // class Section

} // namespace Preprocessor
} // namespace ConfigIO

#endif
