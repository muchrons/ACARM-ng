/*
 * TestAccount.t.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_JABBER_TESTACCOUNT_T_HPP_FILE
#define INCLUDE_TRIGGER_JABBER_TESTACCOUNT_T_HPP_FILE

#include <loudmouth/loudmouth.h>

#include "System/ScopedPtrCustom.hpp"
#include "System/Timer.hpp"
#include "Trigger/Jabber/AccountConfig.hpp"
#include "TestHelpers/Data/jabber1.hpp"
#include "TestHelpers/Data/jabber2.hpp"

namespace Trigger
{
namespace Jabber
{

extern "C" struct MessageHandler;

AccountConfig getTestConfig1(void);

AccountConfig getTestConfig2(void);

// helper for backward compatibility
AccountConfig getTestConfig(void);

extern "C" LmHandlerResult
handleMessages (LmMessageHandler * /*handler*/,
                LmConnection     * /*connection*/,
                LmMessage        *m,
                MessageHandler   *mh);

std::string getMessageFromAccount(const Trigger::Jabber::AccountConfig &account, const std::string &sender);

}  // Jabber

}  // Trigger

#endif
