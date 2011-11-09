/*
 * boost_prog_opt.mt.cpp
 *
 * this code is based on the example sources from
 * http://www.boost.org/doc/libs/1_47_0/doc/html/program_options/tutorial.html
 *
 */
#include <iostream>
#include <boost/program_options.hpp>

using namespace std;
namespace po = boost::program_options;

int main(int argc, char **argv)
{
  // prepare command line options
  po::options_description desc("Possible options");
  desc.add_options()
    ("help,h", "produce help message")
    ("compression,c", po::value<int>(), "set compression level")
  ;

  // parse options
  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);

  // check help flag
  if( vm.count("help") )
  {
    cout<<desc<<endl;
    return 1;
  }

  // check compression
  if( vm.count("compression") )
    cout<<"Compression level was set to "<<vm["compression"].as<int>()<<"."<<endl;
  else
    cout<<"Compression level was not set."<<endl;

  return 0;
}
