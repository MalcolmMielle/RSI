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

#include "Uniqueness.hpp"

BOOST_AUTO_TEST_CASE(trying)
{

// 	int argc = boost::unit_test::framework::master_test_suite().argc;
// 	char** argv = boost::unit_test::framework::master_test_suite().argv;

// 	std::string file = argv[1];
	std::string file = "../../Test/Preprocessed/Sketches/05.png";
//	cv::Mat image = cv::imread(file, CV_LOAD_IMAGE_GRAYSCALE);

//	cv::threshold(image, image, 20, 255, cv::THRESH_BINARY);
//	cv::threshold(image, image, 20, 255, cv::THRESH_BINARY_INV);

//	cv::imshow("IN befoire", image);
//	cv::waitKey(0);

//	AASS::RSI::Uniqueness unique;

//	unique.uniqueness(image);

}
