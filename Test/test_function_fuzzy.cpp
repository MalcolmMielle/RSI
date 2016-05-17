#include <iostream>
#define BOOST_TEST_DYN_LINK

#define BOOST_TEST_MODULE MyTest
#include <time.h>
#include <cstdlib>
#include <boost/test/unit_test.hpp>
#include <fstream>
#include <ctime> 

#include <ctime>

#include "FuzzyOpening.hpp"



BOOST_AUTO_TEST_CASE(trying)
{
	
	FuzzyOpening fuzzy;
	cv::Mat base = cv::imread("../Test/points.png", CV_LOAD_IMAGE_GRAYSCALE);

	//Making circle
	int size = 10;
	cv::Mat circle = cv::Mat::zeros(size, size, CV_8U);
	if(size%2 != 0){
		cv::circle(circle, cv::Point2i(size/2, size/2), size/2, cv::Scalar(1), -1);
	}
	else{
		cv::circle(circle, cv::Point2i((size/2)-1, (size/2)-1), (size/2)-1, cv::Scalar(1), -1);
		cv::circle(circle, cv::Point2i((size/2), (size/2)), (size/2)-1, cv::Scalar(1), -1);
		cv::circle(circle, cv::Point2i((size/2)-1, (size/2)), (size/2)-1, cv::Scalar(1), -1);
		cv::circle(circle, cv::Point2i((size/2), (size/2)-1), (size/2)-1, cv::Scalar(1), -1);
	}
	
	cv::Mat roi = base(cv::Rect(0,0, size, size));
	BOOST_CHECK_EQUAL(fuzzy.circleIsEmpty(roi, circle), true);
	
	roi = base(cv::Rect(15, 15, size, size));
	BOOST_CHECK_EQUAL(fuzzy.circleIsEmpty(roi, circle), false);
	
	roi = base(cv::Rect(11, 11, size, size));
	BOOST_CHECK_EQUAL(fuzzy.circleIsEmpty(roi, circle), true);
	
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
	
	cv::normalize(out, out, 0, 1, cv::NORM_MINMAX, CV_32F);
	
// 	std::cout << out << std::endl;
	
	cv::imshow("out final", out);
	cv::waitKey(0);
	
	
}