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

#include "maoris/Segmentor.hpp"

#include "GraphZoneRI.hpp"
#include "hungarian.h"
#include "HungarianMatcher.hpp"

// #include "Uniqueness.hpp"

#include <valgrind/callgrind.h>
#include <sstream>




void draw(AASS::RSI::GraphZoneRI& gp_real, AASS::RSI::GraphZoneRI& gp_model, const cv::Mat& obstacle, const cv::Mat& obstacle_model, std::vector< AASS::RSI::ZoneCompared > matches){
	
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

 	cv::Scalar color_model;


	auto it = matches.begin();

	for( ; it != matches.end() ; ++it){
		std::cout << "DRAW LINE " << std::endl;

		auto point = gp_model[it->target].getCentroid();
		point.y = point.y + obst_copy.size().height;

		cv::line(all, gp_real[it->source].getCentroid(), point, color, 5);
	}

	cv::imshow("all links", all);
	
}





cv::Mat makeGraph(const std::string& file, AASS::RSI::GraphZoneRI& graph_slam){
	
	cv::Mat slam1 = cv::imread(file, CV_LOAD_IMAGE_GRAYSCALE);
/** Segmenting the map**/
	AASS::maoris::Segmentor segmenteur;
	AASS::maoris::GraphZone graph_segmented;
	
	double time = 0;
// 	makeGraph(slam, graph_slam, time);
	time = segmenteur.segmentImage(slam1, graph_segmented);
	cv::Mat segmented_map = segmenteur.getSegmentedMap();
	
	cv::imshow("Segmented", segmented_map);
	cv::waitKey(0);
	
	graph_slam = AASS::RSI::GraphZoneRI(graph_segmented);
	
	graph_slam.updatePCA();
	graph_slam.setPCAClassification();
	graph_slam.setSizesClassification();

	return segmented_map;
}


BOOST_AUTO_TEST_CASE(trying)
{

	bool is_sketch = false;

	int argc = boost::unit_test::framework::master_test_suite().argc;
	char** argv = boost::unit_test::framework::master_test_suite().argv;
	std::string file;
	if(argc > 1){
		file = argv[1];
	}
	else{
		file = "../../Test/Saeed/bird4.png";
	}
	std::string file2;
	if(argc > 2){
		file2 = argv[2];
	}
	else{
		file2 = "../../Test/Saeed/bird2.png";
	}
	if(argc > 3){
		std::stringstream ss(argv[3]);
		if(!(ss >> std::boolalpha >> is_sketch)) {
			std::cout << "NOT CORRECT BOOLEAN VALUE" << std::endl;
		}
	}
	if(is_sketch) std::cout << "Is a SKETCH" << std::endl;
	else std::cout << "Is NOT a sketch" << std::endl;
	
	cv::Mat slam1 = cv::imread(file, CV_LOAD_IMAGE_GRAYSCALE);
	cv::Mat slam2 = cv::imread(file2, CV_LOAD_IMAGE_GRAYSCALE);
	
	
	
	
		
	AASS::RSI::GraphZoneRI graph_slam;
	cv::Mat graph_slam_segmented = makeGraph(file, graph_slam);
	
	AASS::RSI::GraphZoneRI graph_slam2;
	cv::Mat graph_slam2_segmented = makeGraph(file2, graph_slam2);
	
	
	/********** PCA of all zones in Graph and removing the ripples **********/
	
// 	graph_slam.updatePCA();
// 	graph_slam.removeRiplesv2();
// 	graph_slam.updateContours();
// 	graph_slam.update();
	
	std::cout << "Size of graph" << graph_slam.getNumVertices() << std::endl;
	
// 	graph_slam2.updatePCA();
// 	graph_slam2.removeRiplesv2();
// 	graph_slam2.updateContours();
// 	graph_slam2.update();
	
	std::cout << "Size of graph2" << graph_slam2.getNumVertices() << std::endl;


	/*********** print grsahs *****************/

	cv::Mat gmat = cv::Mat::zeros(slam1.size(), CV_8U);
	graph_slam.draw(gmat);
	cv::resize(gmat, gmat, cv::Size(gmat.cols * 2, gmat.rows * 2));
	cv::imshow("input", gmat);

	cv::Mat gmat2 = cv::Mat::zeros(slam2.size(), CV_8U);
	graph_slam2.draw(gmat2);
	cv::resize(gmat2, gmat2, cv::Size(gmat2.cols * 2, gmat2.rows * 2));
	cv::imshow("model", gmat2);
	cv::waitKey(0);
	
	
	/********** Uniqueness *******************************************/
	
// 	AASS::RSI::Uniqueness unique;
	
// 	std::cout << "FIRST UNIA" << std::endl;
// 	
// 	auto uni1 = unique.uniqueness(graph_slam);
	
// 	/********** Uniqueness *******************************************/
// 	
// 	std::cout << "SECOND UNIA" << std::endl;
	
// 	auto uni2 = unique.uniqueness(graph_slam2);
	

	/*********** print unique graphs *****************/
// 	int sd = 0;
// 	
// 	for(sd ; sd <= 20 ; ++sd){
// 	
// 		graph_slam.setSDAwayFromMeanForUniqueness(sd);
// 		graph_slam2.setSDAwayFromMeanForUniqueness(sd);
// 		
// 		/********** Uniqueness *******************************************/
// 		graph_slam.updateUnique();
// 		graph_slam2.updateUnique();
// 		
// 		assert(graph_slam.zoneUniquenessWasCalculated() == true);
// 		assert(graph_slam2.zoneUniquenessWasCalculated() == true);
// 
// 		cv::Mat gmatu = cv::Mat::zeros(slam1.size(), CV_8U);
// 		graph_slam.drawUnique(gmatu);
// 		cv::imshow("input unique", gmatu);
// 
// 		cv::Mat gmat2u = cv::Mat::zeros(slam2.size(), CV_8U);
// 		graph_slam2.drawUnique(gmat2u);
// 		cv::imshow("model unique", gmat2u);
// 		
// 		std::cout << "For sd " << sd << std::endl;
// 		cv::waitKey(0);
// 	}
// 	
// 	exit(0);


	graph_slam.setSDAwayFromMeanForUniqueness(1);
	graph_slam2.setSDAwayFromMeanForUniqueness(1);
	
	/********** Uniqueness *******************************************/
	graph_slam.updateUnique();
	if(is_sketch) {
		graph_slam2.updateUnique();
	}
	else{
		graph_slam2.updateUnique(graph_slam);
	}
	
	assert(graph_slam.zoneUniquenessWasCalculated() == true);
	assert(graph_slam2.zoneUniquenessWasCalculated() == true);

	cv::Mat gmatu = cv::Mat::zeros(slam1.size(), CV_8U);
	graph_slam.drawUnique(gmatu);
	cv::imshow("model unique", gmatu);

	cv::Mat gmat2u = cv::Mat::zeros(slam2.size(), CV_8U);
	graph_slam2.drawUnique(gmat2u);
	cv::imshow("input unique", gmat2u);
	cv::waitKey(0);
	
	
	/********** Hungarian matching of graph onto itself***************/
			
	std::cout << "Hungarian Match" << std::endl;
	AASS::RSI::HungarianMatcher hungmatch;
	std::vector<int> scores;
	auto match = hungmatch.match(graph_slam, graph_slam2, scores);
	
// 	exit(0);
	
	std::sort(match.begin(), match.end(), [&graph_slam, &graph_slam2](AASS::RSI::ZoneCompared &match, AASS::RSI::ZoneCompared &match1){
//		return match.getRanking(graph_slam, graph_slam2) > match1.getRanking(graph_slam, graph_slam2);
		return match.getSimilarity() < match1.getSimilarity();
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
		std::cout << graph_slam2[match[i].target].getUniquenessScore() << " sum of uniqueness score ";
		std::cout << graph_slam[match[i].source].getUniquenessScore() + graph_slam2[match[i].target].getUniquenessScore() << " ";
		
		std::cout << "Match print :" << std::endl;
		
		match[i].print();
		
		std::cout << "\nrank " << match[i].getSimilarity() << std::endl;
		
		std::cout << std::endl << "zone 1 ";
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
	draw(graph_slam, graph_slam2, graph_slam_segmented, graph_slam2_segmented, match);
	cv::waitKey(0);
	
}

