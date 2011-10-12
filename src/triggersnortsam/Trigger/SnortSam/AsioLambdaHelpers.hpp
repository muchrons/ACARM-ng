/*
 * AsioLambdaHelpers.hpp
 *
 * helper classes that replaces lambdas, that are not available in required form in boost before 1.42.
 */
#include <boost/asio.hpp>
#include <boost/date_time.hpp>

namespace Trigger
{
namespace SnortSam
{

/** \brief deadline checking object.
 *
 *  (re-)registration is done automatically when called operator().
 */
struct DeadlineChecker
{
  /** \brief initialize with elements to operate on.
   *  \param deadline deadline object to operate on.
   *  \param sock     socket to disconnect uppon timeout.
   */
  DeadlineChecker(boost::asio::deadline_timer &deadline, boost::asio::ip::tcp::socket &sock):
    deadline_(&deadline),
    sock_(&sock)
  {
  }

  /** \brief added for compatibility - error code is discarded.
   */
  void operator()(const boost::system::error_code)
  {
    (*this)();
  }
  /** \brief check for timeout and reregisters callback once again.
   */
  void operator()(void)
  {
    assert(deadline_!=NULL);
    assert(sock_    !=NULL);
    // check for timeout
    if( deadline_->expires_at() <= boost::asio::deadline_timer::traits_type::now() )
    {
      sock_->close();
      deadline_->expires_at(boost::posix_time::pos_infin);
    }
    // (re-)register
    deadline_->async_wait(*this);
  }

private:
  boost::asio::deadline_timer  *deadline_;
  boost::asio::ip::tcp::socket *sock_;
}; // struct DeadlineChecker



/** \brief this code replaces lambda that requres boost over 1.40 to work.
 */
struct ErrorCodeSetter
{
  /** \brief initialize with error code to operate on.
   *  \param ec locaiton for error code to be save in.
   */
  explicit ErrorCodeSetter(boost::system::error_code &ec):
    ec_(&ec)
  {
  }
  /** \brief callback.
   *  \param ec error code to process.
   */
  void operator()(const boost::system::error_code ec)
  {
    assert(ec_!=NULL);
    *ec_=ec;
  }

private:
  boost::system::error_code *ec_;
}; // struct ErrorCodeSetter


/** \brief this code replaces lambda that requres boost over 1.40 to work.
 */
struct ErrorCodeWithSizeSetter
{
  /** \brief initialize with error code and size output variables to operate on.
   *  \param ec   locaiton for error code to be saved in.
   *  \param size location for size to be saved in.
   */
  ErrorCodeWithSizeSetter(boost::system::error_code &ec, size_t &size):
    ec_(&ec),
    size_(&size)
  {
  }
  /** \brief callback.
   *  \param ec   error code to be saved.
   *  \param size size to be saved.
   */
  void operator()(const boost::system::error_code ec, const size_t size)
  {
    assert(ec_  !=NULL);
    assert(size_!=NULL);
    *ec_  =ec;
    *size_=size;
  }

private:
  boost::system::error_code *ec_;
  size_t                    *size_;
}; // struct ErrorCodeWithSizeSetter

} // namespace SnortSam
} // namespace Trigger
