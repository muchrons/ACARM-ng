/*
 * Process.cpp
 *
 */

#include "Persistency/Postgres/Process.hpp"

using namespace std;

namespace Persistency
{
namespace Postgres
{

Process::Process(const Path        &path,
                 const Name        &name,
                 const MD5Sum      *md5,
                 const pid_t       *pid,
                 const int         *uid,
                 const Username    &username,
                 const std::string *arguments,
                 ReferenceURLPtr    url):
  Persistency::Process(path,
                       name,
                       md5,
                       pid,
                       uid,
                       username,
                       arguments,
                       url)
{
}

} // namespace Postgres
} // namespace Persistency
