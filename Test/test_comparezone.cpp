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
#include "ZoneReducer.hpp"

BOOST_AUTO_TEST_CASE(trying)
{
	
	cv::Mat slam = cv::imread("../Test/blob3.png", CV_LOAD_IMAGE_GRAYSCALE);
	cv::imshow("SLAM", slam);

// 	std::cout << out << std::endl;
	
// 	cv::normalize(out, out, 0, 1, cv::NORM_MINMAX, CV_32F);
	
	std::cout << "Done opening " << std::endl;
	slam.convertTo(slam, CV_8U);
	
// 	std::cout << out << std::endl;

	
	AASS::RSI::ZoneExtractor wzoneextract2;
	wzoneextract2.extract(slam);
	
	auto graph_slam = wzoneextract2.getGraph();
	
	graph_slam.updatePCA();
	
	cv::Mat draw;
	slam.copyTo(draw);
	graph_slam.drawPartial(draw);
	cv::imshow("Map", draw);
	cv::waitKey(0);
	
	BOOST_CHECK_EQUAL(graph_slam.getNumVertices(), 2);
	
	
	for (auto vp = boost::vertices(graph_slam.getGraph()); vp.first != vp.second;) {
		auto v = *vp.first;
		++vp.first;
		if(vp.first != vp.second){
			auto v2 = *vp.first;
			
			AASS::RSI::ZoneCompared zcomp(v, v2, graph_slam);
			auto zoneslam2 = graph_slam[v2];
			auto zoneslam = graph_slam[v];
			double p, h;
			auto res = zoneslam.compare(zoneslam2);
			auto res2 = zoneslam.compare(zoneslam);
			std::cout << "Compare : " << res.getSimilarity() << std::endl;
			std::cout << "Compare itself : " << res2.getSimilarity() << std::endl;
		}
		std::cout << "OU"<< std::endl;
	}
	
	
}
