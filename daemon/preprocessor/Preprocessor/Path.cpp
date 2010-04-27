/*
 * Path.cpp
 *
 */
#include <vector>
#include <boost/tokenizer.hpp>

#include "Preprocessor/Path.hpp"

namespace Preprocessor
{

Path::Path(const std::string &path)
{
  assert( e_.size()==0u );
  tokenizePath(path);

  // not enought elements?
  if( e_.size()<2u )
    throw ExceptionInvalidPath(SYSTEM_SAVE_LOCATION, path, path);

  // too many elements?
  assert( e_.size()>=2u );
  if( e_.size()>6u )
    throw ExceptionInvalidPath(SYSTEM_SAVE_LOCATION, path, e_.back() );
}

void Path::tokenizePath(const std::string &path)
{
  typedef boost::char_separator<char> Separator;
  typedef boost::tokenizer<Separator> Tokenizer;
  const Separator sep(".");
  const Tokenizer tokens(path, sep);

  // got through all tokens (i.e. path element names)
  for(Tokenizer::const_iterator it=tokens.begin(); it!=tokens.end(); ++it)
    e_.push_back(*it);
}

} // namespace Preprocessor
