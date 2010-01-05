/*
 * Processor.cpp
 *
 */
#include "Filter/Processor.hpp"

namespace Filter
{

namespace
{
class ThreadImpl
{
public:
  ThreadImpl(Core::Types::NodesFifo &mainQueue,
             Core::Types::NodesFifo &filterQueue,
             Interface              *filter):
    log_( Logger::Node("filter.processor.threadimpl") ),
    mainQueue_(&mainQueue),
    filterQueue_(&filterQueue),
    filter_(filter)
  {
    if(filter_==NULL)
      throw ExceptionInvalidFilter(__FILE__, "NULL");
  }

  void operator()(void)
  {
    assert(mainQueue_!=NULL);
    assert(filterQueue_!=NULL);
    assert(filter_!=NULL);

    LOGMSG_INFO_S(log_)<<"thread started: "<<filter_->getFilterName();

    // loop forever
    for(;;)
    {
      try
      {
        // get new data
        boost::this_thread::interruption_point();             // allow interrupts
        LOGMSG_DEBUG_S(log_)<<"waiting for data for: "<<filter_->getFilterName();
        Persistency::GraphNodePtrNN node=filterQueue_->pop(); // wait for data

        // process new data
        LOGMSG_DEBUG_S(log_)<<"data recieved for: "<<filter_->getFilterName()
                            <<" - processing";
        Interface::ChangedNodes changed;
        filter_->process(node, changed);
        LOGMSG_DEBUG_S(log_)<<"data processing done for: "
                            <<filter_->getFilterName();

        boost::this_thread::interruption_point();             // allow interrupts
        // signal others about changes made
        for(Interface::ChangedNodes::iterator it=changed.begin();
            it!=changed.end(); ++it)
          mainQueue_->push(*it);
      }
      catch(const std::exception &ex)
      {
        LOGMSG_ERROR_S(log_)<<"exception cought in thread for filter ("
                            <<filter_->getFilterName()
                            <<"): "<<ex.what();
      }
    } // for(;;)
  }

private:
  Logger::Node            log_;
  Core::Types::NodesFifo *mainQueue_;
  Core::Types::NodesFifo *filterQueue_;
  Interface              *filter_;
}; // class ThreadImpl
} // unnamed namespace


Processor::Processor(Core::Types::NodesFifo &mainQueue,
                     InterfaceAutoPtr        filter):
  mainQueue_(mainQueue),
  log_( Logger::NodeName("filter.processor") ),
  filter_( filter.release() ),
  th_( ThreadImpl(mainQueue_, filterQueue_, filter_.get() ) )
{
  // if() throw() is already in thread's data c-tor
  assert( filter_.get()!=NULL );
  assert( filter.get()==NULL );

  LOGMSG_INFO_S(log_)<<"processor for filter '"<<filter_->getFilterName()
                     <<"' started";
}

Processor::~Processor(void)
{
  LOGMSG_INFO_S(log_)<<"processor for filter '"<<filter_->getFilterName()
                     <<"' - stopping...";
  th_.interrupt();
  filterQueue_.signalAll();
  th_.join();
  LOGMSG_INFO_S(log_)<<"processor for filter '"<<filter_->getFilterName()
                     <<"' - stopped";
}

void Processor::process(Persistency::GraphNodePtrNN node)
{
  // it will be processed in separate thread
  filterQueue_.push(node);
}

} // namespace Filter
