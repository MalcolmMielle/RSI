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
#include "Histogram.hpp"


BOOST_AUTO_TEST_CASE(trying)
{
	
	AASS::RSI::FuzzyOpening fuzzy_2;
	fuzzy_2.setSize(50);
	fuzzy_2.fast(false);
	
	AASS::RSI::FuzzyOpening fuzzy_slam;
	fuzzy_slam.setSize(50);
	fuzzy_slam.fast(false);
	
	int argc = boost::unit_test::framework::master_test_suite().argc;
	char** argv = boost::unit_test::framework::master_test_suite().argv;
	
	char* str = argv[1];
	cv::Mat map = cv::imread(str, CV_LOAD_IMAGE_GRAYSCALE);
	cv::Mat slam = cv::imread("../Test/labfull.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	cv::Mat out, out_slam;
	cv::imshow("Base input ", map);
	cv::imshow("SLAM", slam);
	fuzzy_2.fuzzyOpening(map, out, 35);
	fuzzy_slam.fuzzyOpening(slam, out_slam, 35);
// 	std::cout << out << std::endl;
	
// 	cv::normalize(out, out, 0, 1, cv::NORM_MINMAX, CV_32F);
	
	std::cout << "Done opening " << std::endl;
	out.convertTo(out, CV_8U);
	out_slam.convertTo(out_slam, CV_8U);
	
// 	std::cout << out << std::endl;
	
	
	AASS::RSI::Histogram histo;
	
	histo.histogram(out);
	
// 
		
	
	
}