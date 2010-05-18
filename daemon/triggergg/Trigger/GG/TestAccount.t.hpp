/*
 * TestAccount.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_GG_TESTACCOUNT_T_HPP_FILE
#define INCLUDE_TRIGGER_GG_TESTACCOUNT_T_HPP_FILE

#include "Trigger/GG/AccountConfig.hpp"

namespace
{

Trigger::GG::AccountConfig getTestConfig1(void)
{
  return Trigger::GG::AccountConfig(23436545, "testowekonto");
}

Trigger::GG::AccountConfig getTestConfig2(void)
{
  return Trigger::GG::AccountConfig(23461426, "testowekonto");
}

// helper for backward compatibility
Trigger::GG::AccountConfig getTestConfig(void)
{
  return getTestConfig1();
}

} // unnamed namespace

#endif
