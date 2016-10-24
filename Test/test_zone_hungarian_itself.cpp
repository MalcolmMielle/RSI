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

#include "ZoneExtractor.hpp"
#include "FuzzyOpening.hpp"
#include "Kmean.hpp"
#include "ZoneReducer.hpp"
#include "hungarian.h"
#include "HungarianMatcher.hpp"

BOOST_AUTO_TEST_CASE(trying)
{
	
	int argc = boost::unit_test::framework::master_test_suite().argc;
	char** argv = boost::unit_test::framework::master_test_suite().argv;
		
// 	std::string file = argv[1];
	std::string file = "../../Test/Preprocessed/00.png";
	cv::Mat slam = cv::imread(file, CV_LOAD_IMAGE_GRAYSCALE);
	
	cv::imshow("input", slam);
	cv::waitKey(0);
	
	cv::threshold(slam, slam, 20, 255, cv::THRESH_BINARY);
	cv::threshold(slam, slam, 20, 255, cv::THRESH_BINARY_INV);
	
	std::cout << "/************ FUZZY OPENING*************/ \n";
	AASS::RSI::FuzzyOpening fuzzy_slam;
	fuzzy_slam.fast(false);
	
	cv::Mat out_slam;
	cv::imshow("SLAM", slam);
	cv::waitKey(0);
	fuzzy_slam.fuzzyOpening(slam, out_slam, 500);
	std::cout << "Done opening " << std::endl;
	out_slam.convertTo(out_slam, CV_8U);
	
// 	std::cout << out << std::endl;
	
	std::cout << "/************ REDUCING THE SPACE OF VALUES *****************/\n";
	cv::Mat out_tmp_slam;
	AASS::RSI::reduceZone(out_slam, out_tmp_slam);
	cv::imshow("REDUCED", out_tmp_slam);
	cv::waitKey(0);
	
	AASS::RSI::ZoneExtractor zone_maker;
	std::cout << "WHATERSHED SLAM" << std::endl;
	zone_maker.extract(out_tmp_slam);
	
	std::cout << "Got the ZONES" << std::endl;

	
	AASS::RSI::GraphZone graph_slam;
// 	std::cout << "Getting the graph" << std::endl;
	
	std::cout << "/*********** MAKING AND TRIMMING THE GRAPH ***************/\n";
	graph_slam = zone_maker.getGraph();
	graph_slam.removeVertexValue(0);

	std::cout << "Number of nodes" << graph_slam.getNumVertices() << std::endl;
	
	//Watershed Algorithm
	graph_slam.watershed(0.25);
	
	int size_to_remove = 100;
	graph_slam.removeVertexUnderSize(size_to_remove, true);
	graph_slam.removeLonelyVertices();
	if(graph_slam.lonelyVertices())
		throw std::runtime_error("Fuck you lonelyness");
	
	
	
	/********** PCA of all zones in Graph and removing the ripples **********/
	
	graph_slam.updatePCA();
	graph_slam.removeRiplesv2();
	
	std::cout << "Size of graph" << graph_slam.getNumVertices() << std::endl;
	
	/********** Hungarian matching of graph onto itself***************/
			
	
	AASS::RSI::HungarianMatcher hungmatch;
	std::vector<int> scores;
	auto match = hungmatch.match(graph_slam, graph_slam, scores);
	
	for(size_t i = 0 ; i < match.size() ; ++i){
		cv::imshow("Zone1", graph_slam[match[i].first].getZoneMat());
		std::cout << "SCORE : " << scores[i] << std::endl;
		cv::waitKey(0);
	}
	
}
