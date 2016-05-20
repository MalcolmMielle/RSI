#include <iostream>
#define BOOST_TEST_DYN_LINK

#define BOOST_TEST_MODULE MyTest
#include <time.h>
#include <cstdlib>
#include <boost/test/unit_test.hpp>
#include <fstream>
#include <ctime> 

#include <ctime>

#include "WaterShed.hpp"
#include "FuzzyOpening.hpp"
#include "Kmean.hpp"


BOOST_AUTO_TEST_CASE(trying)
{
	
	AASS::RSI::Zone zone;
	cv::Point2i point;
	zone.push_back(point);
	zone.push_back(point);
	zone.push_back(point);
	zone.push_back(point);
	zone.push_back(point);
	
	AASS::RSI::Zone zone_3;
	zone_3.push_back(point);
	zone_3.push_back(point);
	zone_3.push_back(point);
	zone_3.push_back(point);
	zone_3.push_back(point);
	
	AASS::RSI::Zone zone_2;
	zone_2.push_back(point);
	zone_2.push_back(point);
	
	AASS::RSI::GraphZone graph;
	AASS::RSI::GraphZone::VertexZone vzone;
	AASS::RSI::GraphZone::VertexZone vzone_1;
	AASS::RSI::GraphZone::VertexZone vzone_2;
	graph.addVertex(vzone, zone);
	graph.addVertex(vzone_1, vzone, zone_2);
	graph.addVertex(vzone_2, vzone_1, zone_3);
	
	BOOST_CHECK_EQUAL(3, graph.getNumVertices());
	BOOST_CHECK_EQUAL(2, graph.getNumEdges());
	
	std::cout << "Removing Vertex" << std::endl;
	
	graph.removeVertexUnderSize(4, true);
	BOOST_CHECK_EQUAL(1, graph.getNumEdges());
	BOOST_CHECK_EQUAL(2, graph.getNumVertices());

	
	
}