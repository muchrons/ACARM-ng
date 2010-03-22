/*
 * LeakTest.mt.cpp
 *
 */
#include "curlpp/cURLpp.hpp"

int main(void)
{
  curlpp::initialize();
  //curlpp::Cleanup myCleanup;
  curlpp::terminate();
  return 0;
}
