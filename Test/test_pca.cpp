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

BOOST_AUTO_TEST_CASE(trying)
{
	
	AASS::RSI::FuzzyOpening fuzzy_2;
	fuzzy_2.fast(true);
	
	AASS::RSI::FuzzyOpening fuzzy_slam;
	fuzzy_slam.fast(true);
	
	int argc = boost::unit_test::framework::master_test_suite().argc;
	char** argv = boost::unit_test::framework::master_test_suite().argv;
	
	char* str = argv[1];
	cv::Mat slam = cv::imread(str, CV_LOAD_IMAGE_GRAYSCALE);
	cv::Mat map = cv::imread("../Test/labfull.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	
	cv::threshold(slam, slam, 20, 255, cv::THRESH_BINARY);
	cv::threshold(slam, slam, 20, 255, cv::THRESH_BINARY_INV);
	
	cv::Mat out, out_slam;
	cv::imshow("Base input ", map);
	cv::imshow("SLAM", slam);
// 	fuzzy_2.fuzzyOpening(map, out, 500);
	fuzzy_slam.fuzzyOpening(slam, out_slam, 500);
// 	std::cout << out << std::endl;
	
// 	cv::normalize(out, out, 0, 1, cv::NORM_MINMAX, CV_32F);
	
	std::cout << "Done opening " << std::endl;
	out.convertTo(out, CV_8U);
	out_slam.convertTo(out_slam, CV_8U);
	
// 	std::cout << out << std::endl;
	
	
	cv::Mat out_tmp;
	cv::Mat out_tmp_slam;
	
// 	cv::imshow("REDUCED", out_tmp_slam);
	cv::Mat copp;
	out_slam.copyTo(copp);
	
	AASS::RSI::reduceZone(out_slam, out_tmp_slam);
		
// 	cv::threshold(out_slam, out_slam, 20, 0, cv::THRESH_TOZERO);
// 	AASS::RSI::Kmeans kmeans_slam;
// 	kmeans_slam.setK(5);
// 	kmeans_slam.kmeansColor(out_slam, out_tmp_slam, slam, 0);
// 	kmeans_slam.kmeansColor(out_slam, out_tmp_slam);
	
	cv::imshow("REDUCED", out_tmp_slam);
	cv::Mat cop;
	out_tmp_slam.copyTo(cop);
// 	cv::waitKey(0);

	
	AASS::RSI::ZoneExtractor wzoneextract;
	std::cout << "WHATERSHED SLAM" << std::endl;
	wzoneextract.extract(out_tmp_slam);
	
	std::cout << "WATERSHED DONE" << std::endl;
	cv::Mat graphmat, graphmat_init;
	graphmat = cv::Mat::zeros(out_tmp_slam.size(), CV_8U);
// 	out_tmp_slam.copyTo(graphmat);
	out_tmp_slam.copyTo(graphmat_init);
	AASS::RSI::GraphZone graph_slam;
	std::cout << "Getting the graph" << std::endl;
	graph_slam = wzoneextract.getGraph();
	
// // 		std::cout << "REmove Vertex" << std::endl;
// 	if(graph_slam.lonelyVertices())
// 		throw std::runtime_error("Fuck you lonelyness1");
// 	graph_slam.removeVertexValue(10);
// 	if(graph_slam.lonelyVertices())
// 		throw std::runtime_error("Fuck you lonelyness2");
	graph_slam.removeVertexValue(0);
// 	if(graph_slam.lonelyVertices())
// 		throw std::runtime_error("Fuck you lonelyness2");
// 	
// 	
	graph_slam.draw(graphmat_init);
	cv::imshow("GRAPH_init", graphmat_init);
// 	cv::waitKey(0);
	
	
	std::ofstream outt("bob2.txt");
	graph_slam.write(outt);

	graph_slam.watershed(50);
	
	int size_to_remove = 100;
	graph_slam.removeVertexUnderSize(size_to_remove, true);
	graph_slam.removeLonelyVertices();
	if(graph_slam.lonelyVertices())
		throw std::runtime_error("Fuck you lonelyness");
	
	
	
	/*** PCA of all zones in Graph**/
	
	graph_slam.updatePCA();
	
	/*******************************/
	
	graph_slam.draw(graphmat);
	cv::imshow("GRAPH FINAL before remove", graphmat);
// 	cv::waitKey(0);
	
	graph_slam.removeRiplesv2();
	
	cv::Mat graphmat2 = cv::Mat::zeros(out_tmp_slam.size(), CV_8U);
	graph_slam.draw(graphmat2);
	cv::imshow("GRAPH FINAL after remove", graphmat2);
// 	cv::waitKey(0);
	
	std::cout << "Graph : " << std::endl;
	
	std::cout << "Number of vertices : " << graph_slam.getNumVertices() << " number of edges " << graph_slam.getNumEdges() << std::endl;
	
	std::string stri= "../doc/Output/";
	std::string strired= stri;
	std::string stridist= stri;
	
	boost::filesystem::path p(str);
	while(!boost::filesystem::exists(p)){
		std::cout << "need a valid path toward the slam images" << std::endl;
		std::cin >> str;
		p = str;
	}
	if(boost::filesystem::is_regular_file(p)){
		std::cout << p.filename()<<std::endl;
		stri = stri + "out_" + p.filename().string();
		strired = strired + "out_reduced_" + p.filename().string();
		stridist = stridist + "out_diust_" + p.filename().string();
	}
	
// 	stri = stri + str; 
	
	std::cout << "Printing to " << stri << std::endl;
	cv::imwrite(stri, graphmat2);
	cv::imwrite(strired, cop);
	cv::imwrite(stridist, copp);
// 		cv::imshow("out_tmp", out_tmp);
// 	while(1)
// 		cv::waitKey(0);
// 		watershed.drawAllZones(out_tmp, 0);
	std::cout << "Value of size to remove : " ;
// 	std::cin >> size_to_remove;
	
	
}