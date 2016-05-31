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
#include "ZoneReducer.hpp"

BOOST_AUTO_TEST_CASE(trying)
{
	
	AASS::RSI::FuzzyOpening fuzzy_2;
	fuzzy_2.fast(false);
	
	AASS::RSI::FuzzyOpening fuzzy_slam;
	fuzzy_slam.fast(false);
	
	int argc = boost::unit_test::framework::master_test_suite().argc;
	char** argv = boost::unit_test::framework::master_test_suite().argv;
	
	char* str = argv[1];
	cv::Mat slam = cv::imread(str, CV_LOAD_IMAGE_GRAYSCALE);
	cv::Mat map = cv::imread("../Test/labfull.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	cv::Mat out, out_slam;
	cv::imshow("Base input ", map);
	cv::imshow("SLAM", slam);
	fuzzy_2.fuzzyOpening(map, out, 500);
	fuzzy_slam.fuzzyOpening(slam, out_slam, 500);
// 	std::cout << out << std::endl;
	
// 	cv::normalize(out, out, 0, 1, cv::NORM_MINMAX, CV_32F);
	
	std::cout << "Done opening " << std::endl;
	out.convertTo(out, CV_8U);
	out_slam.convertTo(out_slam, CV_8U);
	
// 	std::cout << out << std::endl;
	
	
	cv::Mat out_tmp;
	cv::Mat out_tmp_slam;
	
// 	AASS::RSI::reduceZone(out_slam, out_tmp_slam);
	
	cv::threshold(out_slam, out_slam, 20, 0, cv::THRESH_TOZERO);
	
		
	AASS::RSI::Kmeans kmeans_slam;
	kmeans_slam.setK(5);
	kmeans_slam.kmeansColor(out_slam, out_tmp_slam, slam);
// 	kmeans_slam.kmeansColor(out_slam, out_tmp_slam);
	
	
	
	cv::imshow("REDUCED", out_tmp_slam);
// 	cv::waitKey(0);
	
	
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
	int size_to_remove = 150;


		
	AASS::RSI::Watershed watershed;
	std::cout << "WHATERSHED SLAM" << std::endl;
	
// 		std::cout << out_tmp_slam << std::endl;
// 		exit(0);
	
	cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size( 11, 11 ), cv::Point( -1, -1 ) );
/// Apply the specified morphology operation
// 		cv::morphologyEx( out, out_tmp, cv::MORPH_CLOSE, element);
// 		cv::morphologyEx( out, out_tmp, cv::MORPH_CLOSE, element);
// 		cv::morphologyEx( out, out_tmp, cv::MORPH_CLOSE, element);
	
// 		cv::medianBlur(out_tmp_slam, out_tmp_slam, 11);
// 		cv::medianBlur(out_tmp_slam, out_tmp_slam, 11);
// 		cv::medianBlur(out_tmp_slam, out_tmp_slam, 11);
	
	watershed.watershed(out_tmp_slam);
	
	std::cout << "WATERSHED DONE" << std::endl;
	
// 		Still good
// 		std::cout << out_tmp_slam << std::endl;
// 		exit(0);
// 		watershed.print();
// 		
// 		std::cout << "Final zone number " << watershed.size() <<std::endl;
	cv::Mat graphmat, graphmat_init;
	graphmat = cv::Mat::zeros(out_tmp_slam.size(), CV_8U);
// 	out_tmp_slam.copyTo(graphmat);
	out_tmp_slam.copyTo(graphmat_init);
	AASS::RSI::GraphZone graph_slam;
	std::cout << "Getting the graph" << std::endl;
	graph_slam = watershed.getGraph();
	
// 		std::cout << "REmove Vertex" << std::endl;
	if(graph_slam.lonelyVertices())
		throw std::runtime_error("Fuck you lonelyness1");
	graph_slam.removeVertexValue(10);
	if(graph_slam.lonelyVertices())
		throw std::runtime_error("Fuck you lonelyness2");
	graph_slam.removeVertexValue(0);
	if(graph_slam.lonelyVertices())
		throw std::runtime_error("Fuck you lonelyness2");
	
	
	graph_slam.draw(graphmat_init);
	cv::imshow("GRAPH_init", graphmat_init);
// 	graph_slam.watershed();
	
	
	
	std::ofstream outt("bob2.txt");
	graph_slam.write(outt);

	graph_slam.watershed(50);
	
	graph_slam.removeVertexUnderSize(size_to_remove, true, out_tmp_slam);
	graph_slam.removeLonelyVertices();
	if(graph_slam.lonelyVertices())
		throw std::runtime_error("Fuck you lonelyness");
	
	graph_slam.draw(graphmat);
	cv::imshow("GRAPH FINAL", graphmat);
	cv::waitKey(0);
	
	std::cout << "Graph : " << std::endl;
	
	std::cout << "Number of vertices : " << graph_slam.getNumVertices() << " number of edges " << graph_slam.getNumEdges() << std::endl;
	
// 		cv::imshow("out_tmp", out_tmp);
	while(1)
		cv::waitKey(0);
// 		watershed.drawAllZones(out_tmp, 0);
	std::cout << "Value of size to remove : " ;
// 	std::cin >> size_to_remove;
	
	
}