/*
 * TestAccount.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_GG_TESTACCOUNT_T_HPP_FILE
#define INCLUDE_TRIGGER_GG_TESTACCOUNT_T_HPP_FILE

#include "Trigger/GG/AccountConfig.hpp"

namespace
{

Trigger::GG::AccountConfig getTestConfig(void)
{
  return Trigger::GG::AccountConfig(23436545, "testowekonto");
}

} // unnamed namespace

#endif
