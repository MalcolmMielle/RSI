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



BOOST_AUTO_TEST_CASE(trying)
{
	
	FuzzyOpening fuzzy_2;
	fuzzy_2.setSize(50);
	fuzzy_2.fast(false);
	
	int argc = boost::unit_test::framework::master_test_suite().argc;
	char** argv = boost::unit_test::framework::master_test_suite().argv;
	
	char* str = argv[1];
	cv::Mat map = cv::imread(str, CV_LOAD_IMAGE_GRAYSCALE);
	cv::Mat out;
	fuzzy_2.fuzzyOpening(map, out, 35);
// 	std::cout << out << std::endl;
	
// 	cv::normalize(out, out, 0, 1, cv::NORM_MINMAX, CV_32F);
	
	std::cout << "Done opening " << std::endl;
	out.convertTo(out, CV_8U);
	
	std::cout << out << std::endl;
	
// 	cv::medianBlur(out, out, 3);
	cv::Mat out_tmp;
	cv::bilateralFilter(out, out_tmp, 5, 10, 0);
	
	std::cout << "FILTER " << std::endl << out_tmp << std::endl;
	
	Watershed watershed;
	std::cout << "WHATERSHED" << std::endl;
	watershed.watershed(out_tmp);
	
	watershed.print();
// 	cv::imshow("out", out);
// 	cv::waitKey(0);
	
	
}