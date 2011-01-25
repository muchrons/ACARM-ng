/*
 * AutoSession.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_JABBER_AUTOSESSION_HPP_FILE
#define INCLUDE_TRIGGER_JABBER_AUTOSESSION_HPP_FILE

#include <gloox/client.h>
#include <memory>

namespace Trigger
{
namespace Jabber
{
/** \brief session object implementation. */
typedef std::auto_ptr<gloox::Client> AutoSession;

} // namespace Jabber
} // namespace Trigger

#endif
