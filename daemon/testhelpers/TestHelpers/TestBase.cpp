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
  "    <type>postgres</type>"
  "    <host>localhost</host>"
  "    <port>5432</port>"
  "    <dbname>acarm_ng_test</dbname>"
  "    <user>acarm-ng-daemon</user>"
  "    <pass>test.daemon</pass>"
  "  </persistency>"
  ""
  "  <logger>"
  "    <appenders>"
  "      <file name=\"default\">"
  "        <output>loggerfile.log</output>"
  "      </file>"
  "    </appenders>"
  ""
  "    <nodes appender=\"default\">"
  "    </nodes>"
  "  </logger>"
  ""
  "</acarm_ng>"
  ;

const char *defaultConfigContentStubs=
  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
  "<acarm_ng>"
  ""
  "  <persistency>"
  "    <type>stubs</type>"
  "  </persistency>"
  ""
  "  <logger>"
  "    <appenders>"
  "      <file name=\"default\">"
  "        <output>loggerfile.log</output>"
  "      </file>"
  "    </appenders>"
  ""
  "    <nodes appender=\"default\">"
  "    </nodes>"
  "  </logger>"
  ""
  "</acarm_ng>"
  ;
}

namespace TestHelpers
{

TestBase::TestBase(const bool useStub):
  defaultFile_("acarm_ng_config.xml")
{
  // remove old file
  unlink(defaultFile_);
  // write new content
  const char *xml =useStub ? defaultConfigContentStubs : defaultConfigContent;
  const int   size=strlen(xml);
  System::DiskFile df(defaultFile_);
  tut::ensure_equals("unable to write to default config file",
                     write(df.get(), xml, size), size);
}

TestBase::~TestBase(void)
{
  //unlink(defaultFile_);
}

} // namespace TestHelpers
