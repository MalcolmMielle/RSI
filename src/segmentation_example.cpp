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

#include "Uniqueness.hpp"



void draw(AASS::RSI::GraphZone& gp_real, AASS::RSI::GraphZone& gp_model, const cv::Mat& obstacle, const cv::Mat& obstacle_model, std::vector< std::pair<AASS::RSI::GraphZone::Vertex, AASS::RSI::GraphZone::Vertex> > matches){
	
	cv::Mat obst_copy;
	obstacle.copyTo(obst_copy);
	
	cv::imshow("OBSOBS", obst_copy);
	
	cv::Mat obst_model_copy;
	obstacle_model.copyTo(obst_model_copy);
	
// 	cv::Mat draw_links = cv::Mat::zeros(obst_model_copy.size(), CV_8UC3);
// 	cv::Mat draw_graph = cv::Mat::zeros(obst_copy.size(), CV_8UC3);
// 	cv::Mat draw_graph_model = cv::Mat::zeros(obst_model_copy.size(), CV_8UC3);
	
	int cols_max = obst_model_copy.size().width;
	if(cols_max < obst_copy.size().width){
		cols_max = obst_copy.size().width;
	}
	
	cv::Size size(cols_max, obst_model_copy.size().height + obst_copy.size().height);
	cv::Mat all = cv::Mat::zeros(size, CV_8UC1);
// 	cv::Mat only_linked = cv::Mat::zeros(size, CV_8UC3);
	cv::Mat all_maps = cv::Mat::zeros(size, CV_8UC3);
	
	cv::Mat roi = all(cv::Rect(0,0,obst_copy.size().width,obst_copy.size().height));
// 	cv::Mat roi_linked = only_linked(cv::Rect(0,0,obst_copy.size().width,obst_copy.size().height));
	cv::Mat roi_model = all(cv::Rect(0 ,obst_copy.size().height, obst_model_copy.size().width,obst_model_copy.size().height));
	
// 	cv::Mat roi_maps = all_maps(cv::Rect(0,0,obst_copy.size().width,obst_copy.size().height));
// 	cv::Mat roi_model_maps = all_maps(cv::Rect(0 ,obst_copy.size().height, obst_model_copy.size().width,obst_model_copy.size().height));
	
// 	gp_real.draw(roi);
// 	gp_model.draw(roi_model);
	
	obst_copy.copyTo(roi);
	obst_model_copy.copyTo(roi_model);
	
	cv::Scalar color;
	cv::RNG rrng(12345);
		
	if(all.channels() == 1){
		color = rrng.uniform(50, 255);
	}
	
	else if(all.channels() == 3){
		color[1] = rrng.uniform(50, 255);
		color[3] = rrng.uniform(50, 255);
		color[2] = rrng.uniform(50, 255);
	}
// 	
// 	cv::Scalar color_model;
// 
// 	
	auto it = matches.begin();
	
	for( ; it != matches.end() ; ++it){
		std::cout << "DRAW LINE " << std::endl;
		
		auto point = gp_model[it->second].getCentroid();
		point.y = point.y + obst_copy.size().height;
		
		cv::line(all, gp_real[it->first].getCentroid(), point, color, 5);
	}
	
	cv::imshow("all links", all);
	
}




int i = 0;

void makeGraph(const std::string& file, AASS::RSI::GraphZone& graph_slam){
	
	++i ;
	
	cv::Mat slam = cv::imread(file, CV_LOAD_IMAGE_GRAYSCALE);
// 	
// 	cv::imshow("input", slam);
// 	cv::waitKey(0);
	
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
	AASS::RSI::reduceZone(out_slam, out_tmp_slam, 2);
	
	cv::imshow("REDUCED", out_tmp_slam);
	cv::waitKey(0);
	
	AASS::RSI::ZoneExtractor zone_maker;
	std::cout << "WHATERSHED SLAM" << std::endl;
	zone_maker.extract(out_tmp_slam);
	
	std::cout << "Got the ZONES" << std::endl;

	// 	std::cout << "Getting the graph" << std::endl;
	
	std::cout << "/*********** MAKING AND TRIMMING THE GRAPH ***************/\n";
	graph_slam = zone_maker.getGraph();
	graph_slam.setThreshold(0.25);
	graph_slam.removeVertexValue(0);

	int size_to_remove2 = 10;
	graph_slam.removeVertexUnderSize(size_to_remove2, true);

	graph_slam.updatePCA();
	graph_slam.updateContours();

	cv::Mat graphmat2 = cv::Mat::zeros(out_slam.size(), CV_8U);
	graph_slam.draw(graphmat2);
	std::string s = std::to_string(i);
	cv::imshow(s, graphmat2);
	cv::waitKey(0);

	graph_slam.removeRiplesv2();
	graph_slam.updatePCA();
	graph_slam.updateContours();

	cv::Mat graphmat3 = cv::Mat::zeros(out_slam.size(), CV_8U);
	graph_slam.draw(graphmat3);
	std::string ss = std::to_string(i+1);
	cv::imshow("RIPPLE", graphmat3);
	cv::waitKey(0);


	

	std::cout << "Number of nodes" << graph_slam.getNumVertices() << std::endl;
	
	//Watershed Algorithm
	graph_slam.watershed();
	
	int size_to_remove = 100;
	graph_slam.removeVertexUnderSize(size_to_remove, true);
	graph_slam.removeLonelyVertices();
	if(graph_slam.lonelyVertices())
		throw std::runtime_error("Fuck you lonelyness");	
	
	
}


BOOST_AUTO_TEST_CASE(trying)
{
	
	int argc = boost::unit_test::framework::master_test_suite().argc;
	char** argv = boost::unit_test::framework::master_test_suite().argv;
		
	std::string file = argv[1];
// 	std::string file = "../../Test/Thermal/cold.jpg";
	AASS::RSI::GraphZone graph_slam;
	makeGraph(file, graph_slam);
	
	cv::Mat slam1 = cv::imread(file, CV_LOAD_IMAGE_GRAYSCALE);
		
	/********** PCA of all zones in Graph and removing the ripples **********/
	
	graph_slam.updatePCA();
	graph_slam.removeRiplesv2();
	
	graph_slam.update();

    cv::Mat graphmat = cv::Mat::zeros(slam1.size(), CV_8U);
    graph_slam.draw(graphmat);
	cv::resize(graphmat, graphmat, cv::Size(graphmat.cols * 2, graphmat.rows * 2));
	cv::imshow("GRAPH", graphmat);
	
	std::cout << "Size of graph" << graph_slam.getNumVertices() << std::endl;

    cv::waitKey(0);
	

}