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

#include "Kmean.hpp"


void draw(AASS::RSI::GraphZone& gp_real, AASS::RSI::GraphZone& gp_model, const cv::Mat& obstacle, const cv::Mat& obstacle_model, std::vector< AASS::RSI::ZoneCompared > matches){
	
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
		
		auto point = gp_model[it->target].getCentroid();
		point.y = point.y + obst_copy.size().height;
		
		cv::line(all, gp_real[it->source].getCentroid(), point, color, 5);
	}
	
	cv::imshow("all links", all);
	
}



void makeGraphSLAM(const std::string& file, AASS::RSI::GraphZone& graph_slam){
	
	cv::Mat slam_tmp = cv::imread(file, CV_LOAD_IMAGE_GRAYSCALE);
// 	
// 	cv::imshow("input", slam_tmp);
// 	cv::waitKey(0);
	
	cv::threshold(slam_tmp, slam_tmp, 20, 255, cv::THRESH_BINARY);
	cv::threshold(slam_tmp, slam_tmp, 20, 255, cv::THRESH_BINARY_INV);
	
// 	cv::imshow("input", slam_tmp);
// 	cv::waitKey(0);
	std::cout << "/************ FUZZY OPENING*************/ \n";
	
	cv::blur(slam_tmp, slam_tmp, cv::Size(15,15));
	
// 	cv::imshow("input", slam_tmp);
// 	cv::waitKey(0);
	
	cv::threshold(slam_tmp, slam_tmp, 200, 255, cv::THRESH_BINARY);
	cv::threshold(slam_tmp, slam_tmp, 20, 0, cv::THRESH_TOZERO);
	
// 	cv::Mat invSrc =  cv::Scalar::all(255) - slam_tmp;
	
// 	cv::imshow("input", slam_tmp);
// 	cv::waitKey(0);
	
	cv::Mat slam;		
	AASS::RSI::Kmeans kmeans;
	kmeans.setK(4);
	kmeans.kmeansColor(slam_tmp, slam);
	
	AASS::RSI::FuzzyOpening fuzzy_slam;
	fuzzy_slam.fast(false);
	
	cv::Mat out_slam;
// 	cv::imshow("SLAM", slam);
// 	cv::waitKey(0);
	fuzzy_slam.fuzzyOpening(slam, out_slam, 500);
	std::cout << "Done opening " << std::endl;
	out_slam.convertTo(out_slam, CV_8U);
	
// 	std::cout << out << std::endl;
	
	std::cout << "/************ REDUCING THE SPACE OF VALUES *****************/\n";
	cv::Mat out_tmp_slam;
	AASS::RSI::reduceZone(out_slam, out_tmp_slam);
// 	cv::imshow("REDUCED", out_tmp_slam);
// 	cv::waitKey(0);
	
	AASS::RSI::ZoneExtractor zone_maker;
	std::cout << "WHATERSHED SLAM" << std::endl;
	zone_maker.extract(out_tmp_slam);
	
	std::cout << "Got the ZONES" << std::endl;

	// 	std::cout << "Getting the graph" << std::endl;
	
	std::cout << "/*********** MAKING AND TRIMMING THE GRAPH ***************/\n";
	graph_slam = zone_maker.getGraph();
	graph_slam.removeVertexValue(0);

	std::cout << "Number of nodes" << graph_slam.getNumVertices() << std::endl;
	
	//Watershed Algorithm
	graph_slam.watershed(0.25);
	
	int size_to_remove = 100;
	graph_slam.removeVertexUnderSize(size_to_remove, true);
	graph_slam.removeLonelyVertices();
	if(graph_slam.lonelyVertices())
		throw std::runtime_error("Fuck you lonelyness");	
	
	cv::Mat graphmat2 = cv::Mat::zeros(out_tmp_slam.size(), CV_8U);
	graph_slam.draw(graphmat2);
	std::string s = "Blob";
// 	cv::imshow(s, graphmat2);
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
// 	cv::imshow("SLAM", slam);
// 	cv::waitKey(0);
	fuzzy_slam.fuzzyOpening(slam, out_slam, 500);
	std::cout << "Done opening " << std::endl;
	out_slam.convertTo(out_slam, CV_8U);
	
// 	std::cout << out << std::endl;
	
	std::cout << "/************ REDUCING THE SPACE OF VALUES *****************/\n";
	cv::Mat out_tmp_slam;
	AASS::RSI::reduceZone(out_slam, out_tmp_slam);
// 	cv::imshow("REDUCED", out_tmp_slam);
// 	cv::waitKey(0);
	
	AASS::RSI::ZoneExtractor zone_maker;
	std::cout << "WHATERSHED SLAM" << std::endl;
	zone_maker.extract(out_tmp_slam);
	
	std::cout << "Got the ZONES" << std::endl;

	// 	std::cout << "Getting the graph" << std::endl;
	
	std::cout << "/*********** MAKING AND TRIMMING THE GRAPH ***************/\n";
	graph_slam = zone_maker.getGraph();
	graph_slam.removeVertexValue(0);

	std::cout << "Number of nodes" << graph_slam.getNumVertices() << std::endl;
	
	//Watershed Algorithm
	graph_slam.watershed(1);
	
	int size_to_remove = 200;
	graph_slam.removeVertexUnderSize(size_to_remove, true);
	graph_slam.removeLonelyVertices();
	if(graph_slam.lonelyVertices())
		throw std::runtime_error("Fuck you lonelyness");	
	
	cv::Mat graphmat2 = cv::Mat::zeros(out_tmp_slam.size(), CV_8U);
	graph_slam.draw(graphmat2);
	std::string s = std::to_string(i);
// 	cv::imshow(s, graphmat2);
}


BOOST_AUTO_TEST_CASE(trying)
{
	
	int argc = boost::unit_test::framework::master_test_suite().argc;
	char** argv = boost::unit_test::framework::master_test_suite().argv;
	std::string file;
// 	if(argc > 2){
		file = argv[1];
// 	}
// 	else{
// 		file = "../../Test/Saeed/bird4.png";
// 	}
	AASS::RSI::GraphZone graph_slam;
	makeGraphSLAM(file, graph_slam);
		
	cv::Mat slam1 = cv::imread(file, CV_LOAD_IMAGE_GRAYSCALE);
	std::string file2;
// 	if(argc > 3){
		file2 = argv[2];
// 	}
// 	else{
// 		file2 = "../../Test/Saeed/bird2.png";
// 	}
	AASS::RSI::GraphZone graph_slam2;
	makeGraphSLAM(file2, graph_slam2);
	
	cv::Mat slam2 = cv::imread(file2, CV_LOAD_IMAGE_GRAYSCALE);
	
	/********** PCA of all zones in Graph and removing the ripples **********/
	
// 	graph_slam.updatePCA();
// 	graph_slam.removeRiplesv2();
// 	graph_slam.updateContours();
	graph_slam.update();
	
	std::cout << "Size of graph" << graph_slam.getNumVertices() << std::endl;
	
// 	graph_slam2.updatePCA();
// 	graph_slam2.removeRiplesv2();
// 	graph_slam2.updateContours();
	graph_slam2.update();
	
// 	std::cout << "Size of graph2" << graph_slam2.getNumVertices() << std::endl;
	
	/********** Drawing the graphs *******************************************/
	
	cv::Mat graphmat = cv::Mat::zeros(slam1.size(), CV_8U);
	graph_slam.draw(graphmat);
	
	cv::Mat graphmat2 = cv::Mat::zeros(slam2.size(), CV_8U);
	graph_slam2.draw(graphmat2);
	
	cv::imshow("graph1", graphmat);
	cv::imshow("graph2", graphmat2);
// 	cv::waitKey(0);
	
	
	/********** Uniqueness *******************************************/
	
	AASS::RSI::Uniqueness unique;
	
	std::cout << "FIRST UNIA" << std::endl;
	
	auto uni1 = unique.uniqueness(graph_slam);
	
// 	/********** Uniqueness *******************************************/
// 	
	std::cout << "SECOND UNIA" << std::endl;
	
	auto uni2 = unique.uniqueness(graph_slam2);
	
	assert(graph_slam.zoneUniquenessWasCalculated() == true);
	assert(graph_slam2.zoneUniquenessWasCalculated() == true);
	
	
	/********** Hungarian matching of graph onto itself***************/
			
	std::cout << "Hungarian Match" << std::endl;
	AASS::RSI::HungarianMatcher hungmatch;
	std::vector<int> scores;
	auto match = hungmatch.match(graph_slam, graph_slam2, scores);
	
// 	exit(0);
	
	std::sort(match.begin(), match.end(), [&graph_slam, &graph_slam2](AASS::RSI::ZoneCompared &match, AASS::RSI::ZoneCompared &match1){
		return match.getRanking(graph_slam, graph_slam2) > match1.getRanking(graph_slam, graph_slam2); 
	} );
	

// 	
// 	/********** Visualization ****************************************/
// 	
	for(size_t i = 0 ; i < match.size() ; ++i){
		std::cout << "matching " << i << " : " << match[i].source << " " << match[i].target << std::endl;
		cv::imshow("Zone1", graph_slam[match[i].source].getZoneMat());
		cv::imshow("Zone2", graph_slam2[match[i].target].getZoneMat());
		
		//TODO: Add uniqueness measurement with it
		std::cout << "SCORE of similarity (diff than uniqueness), it's the matching score between the zones, 0 is good, 1 is bad : " <<  " \nUniqueness : ";
		
		std::cout << graph_slam[match[i].source].getUniquenessScore() << " ";
		std::cout << graph_slam2[match[i].target].getUniquenessScore() << " ";
		std::cout << graph_slam[match[i].source].getUniquenessScore() + graph_slam2[match[i].target].getUniquenessScore() << " ";
		
		std::cout << "Match print :" << std::endl;
		
		match[i].print();
		
		std::cout << "\nrank " << match[i].getRanking(graph_slam, graph_slam2) << std::endl;
		
// 		std::cout << " score " << match[i].getSimilarity() << " diff size " << match[i].size_diff << " pca diff " << match[i].pca_diff << " rank " <<match[i].getRanking(graph_slam, graph_slam2) << " ";
		
		std::cout << "Zone  print :" << std::endl;
		std::cout << std::endl << "zone 1 " ;
		graph_slam[match[i].source].print(); 
		std::cout << std::endl << "zone 2 ";
		graph_slam2[match[i].target].print();
		
// 		for( auto it = uni1.begin(); it != uni1.end() ; ++it){
// 			if(it->first == match[i].first){
// 				std::cout << it->second << " ";
// 			}
// 		}
// 		std::cout << " And " ;
// 		for( auto it = uni2.begin(); it != uni2.end() ; ++it){
// 			if(it->first == match[i].second){
// 				std::cout << it->second << " ";
// 			}
// 		}
// 		
		std::cout << std::endl;
		
		cv::waitKey(0);
	}
	
	cv::imshow("TEST", slam1);
	draw(graph_slam, graph_slam2, slam1, slam2, match);
	cv::waitKey(0);
	
}


