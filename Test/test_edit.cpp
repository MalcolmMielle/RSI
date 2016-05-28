#include <iostream>
#define BOOST_TEST_DYN_LINK

#define BOOST_TEST_MODULE MyTest
#include <time.h>
#include <cstdlib>
#include <boost/test/unit_test.hpp>
#include <fstream>
#include <ctime> 

#include <ctime>

#include "editDistance.hpp"

double compare(const char& c, const char& c2){if(c == c2) {return 0;} return 1;}

BOOST_AUTO_TEST_CASE(trying)
{
	
	std::deque<char> str1;
	str1.push_back('a');
	str1.push_back('t');
	str1.push_back('a');
	str1.push_back('g');
	std::deque<char> str2;
	str2.push_back('b');
	str2.push_back('a');
	str2.push_back('a');
	str2.push_back('a');
	str2.push_back('a');
	
	std::string string;
	
	double res = AASS::RSI::editDistanceNeighborhood<char, char>(str1, str2, compare, string);

	std::cout << "res : " <<res << " thanks to " << string << std::endl;
	
}