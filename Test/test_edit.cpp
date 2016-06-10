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

double compare(const char& c, const char& c2){if(c == c2) {return 1;} return 0;}

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
	
	std::string out;
	double resres = n1.compare(n1, out);
	std::cout << "Out " << out << std::endl;
	BOOST_CHECK_EQUAL(resres, 0);
	BOOST_CHECK_EQUAL(out, "nnn");
	
	resres = n1.compare(n2, out);
	std::cout << "Out " << out << std::endl;
	BOOST_CHECK_EQUAL(out, "ddd");
	BOOST_CHECK_EQUAL(resres, 3);
	
	resres = n2.compare(n1, out);
	std::cout << "Out " << out << std::endl;
	BOOST_CHECK_EQUAL(out, "iii");
	BOOST_CHECK_EQUAL(resres, 3);
	
	n2.push_back(nei2);
	n2.push_back(nei3);
	
	resres = n1.compare(n2, out);
	std::cout << "Out " << out << std::endl;
	BOOST_CHECK_EQUAL(out, "dnn");
	BOOST_CHECK_EQUAL(resres, 1);
	
	resres = n2.compare(n1, out);
	std::cout << "Out " << out << std::endl;
	BOOST_CHECK_EQUAL(out, "inn");
	BOOST_CHECK_EQUAL(resres, 1);
	
	edgeelement.setDiff(50);
	AASS::RSI::Neighbor nei4(vertex, edge, zone, edgeelement);
	edgeelement.setDiff(-10);
	AASS::RSI::Neighbor nei5(vertex, edge, zone, edgeelement);
	edgeelement.setDiff(100);
	AASS::RSI::Neighbor nei6(vertex, edge, zone, edgeelement);
	
	AASS::RSI::Neighborhood n3;
	n3.push_back(nei4);
	n3.push_back(nei5);
	n3.push_back(nei6);
	
	resres = n3.compare(n1, out);
	std::cout << "Out " << out << std::endl;
	BOOST_CHECK_EQUAL(out, "nnn");
	BOOST_CHECK_EQUAL(resres, 0);
	std::cout << "Out " << out << std::endl;
	BOOST_CHECK_EQUAL(out, "nnn");
	resres = n1.compare(n3, out);
	BOOST_CHECK_EQUAL(resres, 0);
	
	
}