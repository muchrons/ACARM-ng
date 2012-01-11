/*
 * Config.t.cpp
 *
 */
#include <tut.h>

#include "Trigger/Whois/Config.hpp"

using namespace Trigger::Whois;

namespace
{

struct TestClass
{
  TestClass(void):
    path_( "/tmp/message.tmpl" ),
    th_( "1.2", "4" ),
    srv_("server", 1234,
        ::Mail::Config::Server::Protocol::SMTP,
        ::Mail::Config::Server::Security::SSL, "/cert/path"),
    auth_("john", "doe"),
    to_("to"),
    //cc_("cc"),
    mc_( "from",
         to_,
         srv_ ),
    cfg_( path_, th_, mc_ )
  {
  }
  const boost::filesystem::path          path_;
  const Trigger::Simple::ThresholdConfig th_;
  const ::Mail::Config::Server           srv_;
  const ::Mail::Config::Authorization    auth_;
  const ::Mail::Config::Recipients       to_;
  //const ::Mail::Config::copyRecipients   cc_;
  const ::Mail::Config                   mc_;
  const Config                           cfg_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Trigger/Whois/Config");
} // unnamed namespace

namespace tut
{
// test getting message template file path
template<>
template<>
void testObj::test<1>(void)
{
  ensure_equals("invalid path", cfg_.getTemplateFilePath(), "/tmp/message.tmpl");
}

// get severity threshold
template<>
template<>
void testObj::test<2>(void)
{
  ensure("severity threshold is NULL", cfg_.getThresholdConfig().getSeverityThreshold().get()!=NULL );
  ensure_equals("invalid severity threshold", *cfg_.getThresholdConfig().getSeverityThreshold().get(), 1.2);

  ensure("alerts' count threshold is NULL", cfg_.getThresholdConfig().getAlertCountThreshold().get()!=NULL );
  ensure_equals("invalid alerts' count threshold", *cfg_.getThresholdConfig().getAlertCountThreshold().get(), 4);
}

// test copyability
template<>
template<>
void testObj::test<3>(void)
{
  const Config tmp=cfg_;
}
} // namespace tut
