#include <iostream>
#define BOOST_TEST_DYN_LINK

#define BOOST_TEST_MODULE MyTest
#include <time.h>
#include <cstdlib>
#include <boost/test/unit_test.hpp>
#include <fstream>
#include <ctime> 

#include <boost/filesystem.hpp>
#include <boost/iterator/filter_iterator.hpp>
#include <boost/filesystem/path.hpp>

#include "Uniqueness.hpp"

BOOST_AUTO_TEST_CASE(trying)
{
	
// 	int argc = boost::unit_test::framework::master_test_suite().argc;
// 	char** argv = boost::unit_test::framework::master_test_suite().argv;
		
// 	std::string file = argv[1];
	std::string file = "../../Test/Preprocessed/00.png";
	
	AASS::RSI::Uniqueness unique;
	
	unique.uniqueness(file);
	
}
