/*
 * TestAccount.t.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_JABBER_TESTACCOUNT_T_HPP_FILE
#define INCLUDE_TRIGGER_JABBER_TESTACCOUNT_T_HPP_FILE


#include "System/ScopedPtrCustom.hpp"
#include "Trigger/Jabber/AccountConfig.hpp"
#include "TestHelpers/Data/jabber1.hpp"
#include "TestHelpers/Data/jabber2.hpp"

namespace
{

Trigger::Jabber::AccountConfig getTestConfig1(void)
{
  return Trigger::Jabber::AccountConfig(JABBER1_TEST_ACCOUNT_SERVER,
                                        JABBER1_TEST_ACCOUNT_LOGIN,
                                        JABBER1_TEST_ACCOUNT_PASS);
}

Trigger::Jabber::AccountConfig getTestConfig2(void)
{
  return Trigger::Jabber::AccountConfig(JABBER2_TEST_ACCOUNT_SERVER,
                                        JABBER2_TEST_ACCOUNT_LOGIN,
                                        JABBER2_TEST_ACCOUNT_PASS);
}

// helper for backward compatibility
Trigger::Jabber::AccountConfig getTestConfig(void)
{
  return getTestConfig1();
}


} // unnamed namespace

#endif
