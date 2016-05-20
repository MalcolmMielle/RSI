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
	
	
	cv::Mat out_tmp;
	cv::Mat out_tmp_slam;
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
	cv::waitKey(0);
	int K = 4;
	while(K != 0){
// 		AASS::RSI::Kmeans kmeans;
// 		kmeans.setK(K);
// 		kmeans.kmeansColor(out, out_tmp);
		
		AASS::RSI::Kmeans kmeans_slam;
		kmeans_slam.setK(K);
		kmeans_slam.kmeansColor(out_slam, out_tmp_slam);
		
// 		out_tmp_slam = out_slam;
// 		std::cout << out_tmp_slam << std::endl;
// 		exit(0);
		//Still good here
// 		std::cout << out_tmp_slam << std::endl;
		
// 		cv::imshow("kmenas", out_tmp);
		cv::imshow("kmenas SLAM", out_tmp_slam);

		
		AASS::RSI::Watershed watershed;
		std::cout << "WHATERSHED SLAM" << std::endl;
		
// 		std::cout << out_tmp_slam << std::endl;
// 		exit(0);
		
		watershed.watershed(out_tmp_slam);
		
		std::cout << "WATERSHED DONE" << std::endl;
		
// 		Still good
// 		std::cout << out_tmp_slam << std::endl;
// 		
// 		watershed.print();
// 		
// 		std::cout << "Final zone number " << watershed.size() <<std::endl;
		cv::Mat graphmat;
		slam.copyTo(graphmat);
		
		AASS::RSI::GraphZone graph_slam;
		std::cout << "Getting the graph" << std::endl;
		graph_slam = watershed.getGraph();
		
		std::cout << "HGetting the color" << std::endl;
		int color_wall_slam = 0 ;
		if(kmeans_slam.getColors().size() >0 ){
			color_wall_slam = kmeans_slam.getColors()[0];
		}
		
		std::cout << "WALLS ARE " << color_wall_slam << std::endl;
		kmeans_slam.printColors();
		
		std::cout << "REmove Vertex" << std::endl;
		
		graph_slam.removeVertexValue(color_wall_slam);
		
		graph_slam.draw(graphmat);
		cv::imshow("GRAPH", graphmat);
		
// 		cv::imshow("out_tmp", out_tmp);
		cv::waitKey(0);
// 		watershed.drawAllZones(out_tmp, 0);
		std::cout << "Value of K : " ;
		std::cin >> K;
	}
	
	
}