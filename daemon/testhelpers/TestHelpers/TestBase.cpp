/*
 * TestBase.cpp
 *
 */
#include <tut/tut.hpp>
#include <cstring>
#include <unistd.h>
#include <stdlib.h>
#include <cstring>

#include "System/DiskFile.hpp"
#include "ConfigIO/Singleton.hpp"
#include "TestHelpers/TestBase.hpp"

namespace TestHelpers
{

TestBase::TestBase(const char *xml):
  defaultFile_("test_base_config_file.xml")
{
  // remove old file
  unlink(defaultFile_);

  // write new content
  if(xml!=NULL)
  {
    const int        size=strlen(xml);
    System::DiskFile df(defaultFile_);
    tut::ensure_equals("unable to write to default config file",
                       write(df.get(), xml, size), size);
    // force rereading newly set configuration
    ConfigIO::Singleton::get()->rereadConfig(defaultFile_);
  }
  else
    defaultConfig();
}

TestBase::~TestBase(void)
{
  unlink(defaultFile_);
  defaultConfig();
}

void TestBase::defaultConfig(void)
{
  // force rereading default configuration
  try
  {
    ConfigIO::Singleton::get()->rereadConfig(NULL);
  }
  catch(...)
  {
    assert(!"unable to read default configuration file");
  }
}

} // namespace TestHelpers
