/*
 * TestBase.cpp
 *
 */
#include <tut/tut.hpp>
#include <cstring>
#include <unistd.h>
#include <stdlib.h>

#include "System/DiskFile.hpp"
#include "TestHelpers/TestBase.hpp"

namespace
{
const char *defaultConfigContent=
  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
  "<acarm_ng>"
  ""
  "  <persistency>"
  "    <type>stubs</type>"
  "  </persistency>"
  ""
  "  <logger>"
  "    <appenders>"
  "      <File name=\"default\">"
  "        <output>acarm-ng.log</output>"
  "      </File>"
  "    </appenders>"
  ""
  "    <nodes appender=\"default\" threshold=\"debug\">"
  "    </nodes>"
  "  </logger>"
  ""
  "</acarm_ng>"
  ;
}

namespace TestHelpers
{

TestBase::TestBase(const char *xml):
  defaultFile_("acarm_ng_config.xml")
{
  // remove old file
  unlink(defaultFile_);
  // write new content
  if(xml==NULL)
    xml=defaultConfigContent;
  const int   size=strlen(xml);
  System::DiskFile df(defaultFile_);
  tut::ensure_equals("unable to write to default config file",
                     write(df.get(), xml, size), size);
}

TestBase::~TestBase(void)
{
  unlink(defaultFile_);
}

} // namespace TestHelpers
