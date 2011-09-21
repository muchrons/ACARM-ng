/*
 * Path.cpp
 *
 */
#include <boost/tokenizer.hpp>

#include "Persistency/Facades/StrAccess/Path.hpp"

namespace Persistency
{
namespace Facades
{
namespace StrAccess
{

Path::Path(const std::string &path):
  path_(path)
{
  assert( e_.size()==0u );
  tokenizePath();

  // not enought elements?
  if( e_.size()<2u )
    throw ExceptionInvalidPath(SYSTEM_SAVE_LOCATION, path_, path_, "path too short");
  // NOTE: since meta-alerts can be processed as well as alerts there is not such thing as too long
  //       path, since there can be a sequence like metaalert.children.0.metalert...(and so on)
  //       many times. thus only minimal length will be checked.

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

} // namespace StrAccess
} // namespace Facades
} // namespace Persistency
