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
#include "Neighborhood.hpp"

double compare(const char& c, const char& c2){if(c == c2) {return 0;} return 1;}

BOOST_AUTO_TEST_CASE(trying)
{
	std::cout << "Comparing strings" << std::endl;
	
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
	
	double res = AASS::RSI::levenshteinDistance<char, char>(str1, str2, compare, string);

	std::cout << "res : " <<res << " thanks to " << string << std::endl;
	
	std::cout << "Comparing Neighbbor" << std::endl;
	
	AASS::RSI::GraphZone::VertexZone vertex;
	AASS::RSI::GraphZone::EdgeZone edge;
	AASS::RSI::Zone zone;
	AASS::RSI::EdgeElement edgeelement;
	edgeelement.setDiff(20);
	AASS::RSI::Neighbor nei1(vertex, edge, zone, edgeelement);
	edgeelement.setDiff(-10);
	AASS::RSI::Neighbor nei2(vertex, edge, zone, edgeelement);
	edgeelement.setDiff(50);
	AASS::RSI::Neighbor nei3(vertex, edge, zone, edgeelement);
	
	AASS::RSI::Neighborhood n1;
	n1.push_back(nei1);
	n1.push_back(nei2);
	n1.push_back(nei3);
	AASS::RSI::Neighborhood n2;
	
	double resres = n1.compare(n1);
	
	BOOST_CHECK_EQUAL(resres, 0);
	
	
}