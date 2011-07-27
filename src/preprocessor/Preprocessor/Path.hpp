/*
 * Path.hpp
 *
 */
#ifndef INCLUDE_PREPROCESSOR_PATH_HPP_FILE
#define INCLUDE_PREPROCESSOR_PATH_HPP_FILE

#include <string>
#include <vector>

#include "Preprocessor/ExceptionInvalidPath.hpp"

// TODO: to be removed and replaced with Persistency::Facades::StrAccess

namespace Preprocessor
{

/** \brief class representing path as sequence of tokens.
 */
class Path
{
private:
  typedef std::vector<std::string> Elements;

public:
  /** \brief const iterator for tokends. */
  typedef Elements::const_iterator const_iterator;

  /** \brief tokenize given path string.
   *  \param path path to be tokenized.
   */
  explicit Path(const std::string &path);

  /** \brief gets begin iterator - const version.
   *  \return begin const iterator.
   */
  const_iterator begin(void) const
  {
    return e_.begin();
  }
  /** \brief gets end iterator - const version.
   *  \return begin end iterator.
   */
  const_iterator end(void) const
  {
    return e_.end();
  }
  /** \brief gets path as string.
   *  \return path in string form.
   */
  const std::string &get(void) const
  {
    return path_;
  }

private:
  void tokenizePath(void);

  std::string path_;
  Elements    e_;
}; // class Path

} // namespace Preprocessor

#endif
