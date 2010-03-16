/*
 * Thread.hpp
 *
 */
#ifndef INCLUDE_INPUT_THREAD_HPP_FILE
#define INCLUDE_INPUT_THREAD_HPP_FILE

#include "Core/Types/AlertsFifo.hpp"
#include "Input/Reader.hpp"

namespace Input
{

/** \brief thread's function object
 */
class Thread
{
public:
  /** \brief create object.
   *  \param reader reader to run in background.
   *  \param output queue to output data to.
   */
  Thread(ReaderPtrNN reader, Core::Types::AlertsFifo &output);

  void operator()(void);

private:
  ReaderPtrNN              reader_;
  Core::Types::AlertsFifo *output_;
}; // class Thread

} // namespace Input

#endif
