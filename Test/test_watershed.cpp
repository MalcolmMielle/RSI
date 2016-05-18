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
	
	AASS::RSI::FuzzyOpening fuzzy_2;
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
	
// 	std::cout << out << std::endl;
	
	
	cv::Mat out_tmp;
// 	cv::GaussianBlur(out, out_tmp, cv::Size(3, 3), 10);
// 	cv::medianBlur(out, out_tmp, 11);
// 	cv::bilateralFilter(out, out_tmp, 5, 10, 0);

// 	cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size( 11, 11 ), cv::Point( -1, -1 ) );
	/// Apply the specified morphology operation
// 	cv::morphologyEx( out, out_tmp, cv::MORPH_CLOSE, element);
	
// 	std::cout << "FILTER " << std::endl << out << std::endl;
	
// 	cv::pyrDown(out, out_tmp, cv::Size(out.cols/2, out.rows/2 ) );
// 	cv::pyrUp(out_tmp, out_tmp, cv::Size(out_tmp.cols*2, out_tmp.rows*2 ) );
	
// 	cv::resize(out, out_tmp, cv::Size(out.cols/2, out.rows/2 ), 0, 0, cv::INTER_NEAREST);
	
	
// 	cv::GaussianBlur(out, out_tmp, cv::Size(0, 0), 3);
// 	cv::addWeighted(out, 1.5, out_tmp, -0.5, 0, out_tmp);
	
// 	cv::resize(out_tmp, out_tmp, cv::Size(out_tmp.cols*2, out_tmp.rows*2 ), 0, 0, cv::INTER_NEAREST);
	
// 	std::cout << "out_tmp" << out_tmp << std::endl;
	
	
	AASS::RSI::Kmeans kmeans;
	
	kmeans.kmeansColor(out, out_tmp);
	
	
	
	AASS::RSI::Watershed watershed;
	std::cout << "WHATERSHED" << std::endl;
	watershed.watershed(out_tmp);
	
	watershed.print();
	
	std::cout << "Final zone number " << watershed.size() <<std::endl;
	
	cv::imshow("out_tmp", out_tmp);
	cv::waitKey(0);
	watershed.drawAllZones(out_tmp, 0);
	
	
	
}