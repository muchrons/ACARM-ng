/*
 * UniqueFifoQueue.hpp
 *
 */
#ifndef INCLUDE_BASE_THREADS_UNIQUEFIFOQUEUE_HPP_FILE
#define INCLUDE_BASE_THREADS_UNIQUEFIFOQUEUE_HPP_FILE

/* public header */

#include <deque>
#include <boost/thread.hpp>
#include <cassert>

#include "Base/Threads/FifoQueue.hpp"
#include "Base/Threads/FifoAcceptUniquePolicy.hpp"

namespace Base
{
namespace Threads
{

/** \brief first-in-first-out unique queue - thread safe implementation.
 *  \note T many not throw uppon copying, or one will loose enqueued elements.
 */
template<typename T>
struct UniqueFifoQueue: public FifoQueue<T, FifoAcceptUniquePolicy>
{
}; // struct FifoQueue

} // namespace Threads
} // namespace Base

#endif
