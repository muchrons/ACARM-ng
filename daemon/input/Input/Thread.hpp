/*
 * Thread.hpp
 *
 */
#ifndef INCLUDE_INPUT_THREAD_HPP_FILE
#define INCLUDE_INPUT_THREAD_HPP_FILE

#include "Logger/Logger.hpp"
#include "Core/Types/AlertsFifo.hpp"
#include "Input/Reader.hpp"
#include "Input/CommonData.hpp"

namespace Input
{

/** \brief thread's function object
 */
class Thread
{
public:
  /** \brief create object.
   *  \param reader reader to run in background.
   *  \param conn   data base connection to be used.
   *  \param output queue to output data to.
   */
  Thread(ReaderPtrNN                       reader,
         Persistency::IO::ConnectionPtrNN  conn,
         Core::Types::AlertsFifo          &output,
         CommonDataPtrNN                   commonData);

  /** \brief thread loop implementation.
   */
  void operator()(void);

private:
  ReaderPtrNN                       reader_;
  Logger::Node                      log_;
  Persistency::IO::ConnectionPtrNN  conn_;
  Core::Types::AlertsFifo          *output_;
  CommonDataPtrNN                   commonData_;
}; // class Thread

} // namespace Input

#endif
