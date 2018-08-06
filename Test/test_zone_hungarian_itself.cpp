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

#include "maoris/ZoneExtractor.hpp"
#include "maoris/FuzzyOpening.hpp"
//#include "maoris/Kmean.hpp"
#include "maoris/ZoneReducer.hpp"
#include "maoris/Segmentor.hpp"

#include "GraphZoneRI.hpp"
#include "ZoneCompared.hpp"
#include "hungarian.h"
#include "HungarianMatcher.hpp"



void draw(AASS::RSI::GraphZoneRI& gp_real, AASS::RSI::GraphZoneRI& gp_model, const cv::Mat& obstacle, const cv::Mat& obstacle_model, std::vector< AASS::RSI::ZoneCompared > matches){
	
	cv::Mat obst_copy;
	obstacle.copyTo(obst_copy);
	
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
	cv::Mat all = cv::Mat::zeros(size, CV_8UC3);
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
		color[2] = rrng.uniform(50, 255);
		color[3] = rrng.uniform(50, 255);
	}
	
	cv::Scalar color_model;

	
	auto it = matches.begin();
	
	for( ; it != matches.end() ; ++it){
		std::cout << "DRAW LINE " << std::endl;
		
		auto point = gp_model[it->target].getCentroid();
		point.y = point.y + obst_model_copy.size().height;
		
		cv::line(all, gp_real[it->source].getCentroid(), point, color, 5);
	}
	
	cv::imshow("all links", all);
	
}


BOOST_AUTO_TEST_CASE(trying)
{
	
	int argc = boost::unit_test::framework::master_test_suite().argc;
	char** argv = boost::unit_test::framework::master_test_suite().argv;
		
// 	std::string file = argv[1];
	std::string file = "../../Test/Preprocessed/Sketches/13.png";
	cv::Mat slam = cv::imread(file, CV_LOAD_IMAGE_GRAYSCALE);
	
	AASS::maoris::Segmentor segmenteur;
	AASS::maoris::GraphZone graph_segmented;
	
	double time = 0;
// 	makeGraph(slam, graph_slam, time);
	time = segmenteur.segmentImage(slam, graph_segmented);
	cv::Mat segmented_map = segmenteur.getSegmentedMap();
	
	cv::imshow("Segmented", segmented_map);
	cv::waitKey(0);
	
	AASS::RSI::GraphZoneRI graph_slam(graph_segmented);
	
	
	cv::Mat imgimg = cv::Mat::zeros(slam.size(), CV_8UC1);
	graph_slam.draw(imgimg);
	cv::imshow("SLAMish", imgimg);
	graph_slam.updatePCA();
	graph_slam.setPCAClassification();
	graph_slam.setSizesClassification();
	graph_slam.draw(imgimg);
	cv::imshow("SLAMish2", imgimg);
	
	cv::waitKey(0);
	
	graph_slam.updatePCA();
	
	std::cout << "Size of graph" << graph_slam.getNumVertices() << std::endl;
	
	/********** Hungarian matching of graph onto itself***************/
			
	
	AASS::RSI::HungarianMatcher hungmatch;
	std::vector<int> scores;
	auto match = hungmatch.match(graph_slam, graph_slam, scores);
	
// 	for(size_t i = 0 ; i < match.size() ; ++i){
// 		cv::imshow("Zone1", graph_slam[match[i].first].getZoneMat());
// 		std::cout << "SCORE : " << scores[i] << std::endl;
// 		cv::waitKey(0);
// 	}
	
	
	cv::Mat slam_tmp = cv::imread(file, CV_LOAD_IMAGE_GRAYSCALE);
	draw(graph_slam, graph_slam, slam_tmp , slam_tmp, match);
	cv::waitKey(0);
	
	std::cout << "FIN" << std::endl;
}
