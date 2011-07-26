/*
 * Path.cpp
 *
 */
#include <vector>
#include <boost/tokenizer.hpp>

#include "Preprocessor/Path.hpp"

// TODO: to be removed and replaced with Persistency::Facades::StrAccess

namespace Preprocessor
{

Path::Path(const std::string &path):
  path_(path)
{
  assert( e_.size()==0u );
  tokenizePath();

  // not enought elements?
  if( e_.size()<2u )
    throw ExceptionInvalidPath(SYSTEM_SAVE_LOCATION, path_, path_);

  // too many elements?
  assert( e_.size()>=2u );
  if( e_.size()>7u )
    throw ExceptionInvalidPath(SYSTEM_SAVE_LOCATION, path_, e_.back() );

  assert(path_==path);  // sanity check
}

void Path::tokenizePath(void)
{
  typedef boost::char_separator<char> Separator;
  typedef boost::tokenizer<Separator> Tokenizer;
  const Separator sep(".");
  const Tokenizer tokens(path_, sep);

  // got through all tokens (i.e. path element names)
  for(Tokenizer::const_iterator it=tokens.begin(); it!=tokens.end(); ++it)
    e_.push_back(*it);
}

} // namespace Preprocessor
