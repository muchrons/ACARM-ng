/*
 * Thread.hpp
 *
 */
#ifndef INCLUDE_INPUT_THREAD_HPP_FILE
#define INCLUDE_INPUT_THREAD_HPP_FILE

#include "Logger/Logger.hpp"
#include "Commons/SharedPtrNotNULL.hpp"
#include "DataFacades/AnalyzersCreator.hpp"
#include "Core/Types/AlertsFifo.hpp"
#include "Input/Reader.hpp"

namespace Input
{

/** \brief thread's function object
 */
class Thread
{
public:
  /** \brief create object for thread.
   *  \param reader     reader to run in background.
   *  \param conn       data base connection to be used.
   *  \param output     queue to output data to.
   */
  Thread(ReaderPtrNN                       reader,
         Persistency::IO::ConnectionPtrNN  conn,
         Core::Types::AlertsFifo          &output);

  /** \brief thread loop implementation.
   */
  void operator()(void);

private:
  void sendHeartbeat(unsigned int timeout, unsigned int deadline);

  typedef Commons::SharedPtrNotNULL<DataFacades::AnalyzersCreator> AnalyzersCreatorPtrNN;

  ReaderPtrNN                             reader_;
  Logger::Node                            log_;
  Persistency::IO::ConnectionPtrNN        conn_;
  Core::Types::AlertsFifo                *output_;
  Persistency::Timestamp                  lastHeartbeat_;
  Persistency::IO::Heartbeats::Owner      owner_;
  AnalyzersCreatorPtrNN                   creator_;
}; // class Thread

} // namespace Input

#endif
