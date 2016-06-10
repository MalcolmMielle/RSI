#include <iostream>
#define BOOST_TEST_DYN_LINK

#define BOOST_TEST_MODULE MyTest
#include <time.h>
#include <cstdlib>
#include <boost/test/unit_test.hpp>
#include <fstream>
#include <ctime> 

#include <ctime>

#include "ZoneExtractor.hpp"
#include "FuzzyOpening.hpp"
#include "Kmean.hpp"


BOOST_AUTO_TEST_CASE(trying)
{
	
	
	std::cout << "Setting everything" << std::endl;
	
	AASS::RSI::Zone zone(10, 10);
	cv::Point2i point;
	zone.push_back(point);
	zone.push_back(point);
	zone.push_back(point);
	zone.push_back(point);
	zone.push_back(point);
	zone.setValue(0);
	
	AASS::RSI::Zone zone_3(10, 10);
	zone_3.push_back(point);
	zone_3.push_back(point);
	zone_3.push_back(point);
	zone_3.push_back(point);
	zone_3.push_back(point);
	zone_3.setValue(100);
	
	AASS::RSI::Zone zone_2(10, 10);
	zone_2.push_back(point);
	zone_2.push_back(point);
	zone_2.setValue(70);
	
	
	std::cout << "Set everything" << std::endl;
	
	AASS::RSI::GraphZone graph;
	AASS::RSI::GraphZone::VertexZone vzone;
	AASS::RSI::GraphZone::VertexZone vzone_1;
	AASS::RSI::GraphZone::VertexZone vzone_2;
	graph.addVertex(vzone, zone);
	graph.addVertex(vzone_1, vzone, zone_2);
	graph.addVertex(vzone_2, vzone_1, zone_3);
	
	BOOST_CHECK_EQUAL(3, graph.getNumVertices());
	BOOST_CHECK_EQUAL(2, graph.getNumEdges());
	
// 	std::cout << "Removing Vertex" << std::endl;
	
// 	graph.removeVertexUnderSize(4, true);
// 	BOOST_CHECK_EQUAL(1, graph.getNumEdges());
// 	BOOST_CHECK_EQUAL(2, graph.getNumVertices());
	
	AASS::RSI::Zone zone_4(10, 10);
	zone_4.push_back(point);
	zone_4.push_back(point);
	zone_4.setValue(120);
	AASS::RSI::GraphZone::VertexZone vzone_4;
	graph.addVertex(vzone_4, vzone_2, zone_4);
	
	zone_4.setValue(110);
	AASS::RSI::GraphZone::VertexZone vzone_5;
	graph.addVertex(vzone_5, vzone_2, zone_4);
	zone_4.setValue(90);
	AASS::RSI::GraphZone::VertexZone vzone_6;
	graph.addVertex(vzone_6, vzone_2, zone_4);
	zone_4.setValue(80);
	AASS::RSI::GraphZone::VertexZone vzone_7;
	graph.addVertex(vzone_7, vzone_6, zone_4);
	zone_4.setValue(79);
	AASS::RSI::GraphZone::VertexZone vzone_8;
	graph.addVertex(vzone_8, vzone_7, zone_4);
// 	AASS::RSI::GraphZone::EdgeZone edg;
// 	graph.addEdge(edg, vzone_8, vzone_6);
	zone_4.setValue(91);
	AASS::RSI::GraphZone::VertexZone vzone_9;
	graph.addVertex(vzone_9, vzone_8, zone_4);
	
	std::cout << "read" << std::endl;
	std::ifstream in("bob2.txt");
// 	graph.clear();
// 	graph.read(in);
	std::cout << "done reading" << std::endl;
	
	int ju = 0;
	std::pair<AASS::RSI::GraphZone::VertexIteratorZone, AASS::RSI::GraphZone::VertexIteratorZone> vp;
		//vertices access all the vertix
		std::cout << "NEW start" << std::endl;
// 		std::cout << "num of vertices " << getNumVertices() << std::endl; 
		for (vp = boost::vertices(graph.getGraph()); vp.first != vp.second;) {
// 			std::cout << "Looking up vertex " << std::endl;
			auto v = *vp.first;
			++vp.first;
			if(graph.getNumEdges(v) == 0){
				std::cout << ju << std::endl;
				throw std::runtime_error("pb one node not liked at the start");
			}
			++ju;
		}
	graph.print();
	
	graph.watershed(42);
	
	graph.print();

	
	
}