#include <iostream>
#define BOOST_TEST_DYN_LINK

#define BOOST_TEST_MODULE MyTest
#include <time.h>
#include <cstdlib>
#include <boost/test/unit_test.hpp>
#include <fstream>
#include <ctime> 
#include <sys/time.h>

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

struct results{
	double time;
	double precision;
	double recall;
};

typedef std::map <std::vector<int>, std::vector <cv::Point> > match2points;
typedef std::map <int, std::vector <cv::Point> > tag2points;
typedef std::map <int, tag2points> tag2tagMapper;

void extract_results(results& pixel, results& Regions, double& pixel_precision, double & pixel_recall, std::vector < std::vector<float> >& Precisions, std::vector < std::vector<float> >& Recalls, std::vector<double>& Times){
	pixel.precision = 100*pixel_precision;
	pixel.recall    = 100*pixel_recall;
	
	
	float cum_precision=0;
	float cum_recall=0;
	int size_precision=0, size_recall=0;

	for(int j=0; j < Precisions.back().size();j++){
		cum_precision += Precisions.back()[j];
		size_precision++;
	}
	for(int j=0; j < Recalls.back().size();j++){
		cum_recall    += Recalls.back()[j];
		size_recall++;
	}
	Regions.precision = cum_precision/Precisions.back().size();
	Regions.recall    = cum_recall/Recalls.back().size();


//			Precisions.clear();
//			Recalls.clear();
}

inline double getTime() //in millisecond
{
    //assuming unix-type systems
    //timezone tz;
    timeval  tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec*1000000+tv.tv_usec)*1.0/1000;
}

cv::Mat segment_Ground_Truth(cv::Mat GroundTruth_BW){
	cv::Mat src = GroundTruth_BW.clone();
	cv::Mat drawing = cv::Mat::zeros(src.rows, src.cols, CV_8UC1);

	src = src > 250;
	
	cv::erode(src, src, cv::Mat(), cv::Point(-1,-1), 1, cv::BORDER_CONSTANT, cv::morphologyDefaultBorderValue() );			// erode ground truth obstacle
	
	std::vector<std::vector<cv::Point> > contours;
	std::vector<cv::Vec4i> hierarchy;
	
	cv::findContours( src, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );

	
	// iterate through all the top-level contours,
	// draw each connected component with its own random color
	int idx = 0;
	//DO NOT CHANGE THAT. WRONG THE COMPARISON LATER OTHERWISE :/
// 	int color= 249 / contours.size();
	int count = 1;
	for( ; idx >= 0; idx = hierarchy[idx][0] )
	{
		cv::drawContours( drawing, contours, idx, count , CV_FILLED, 20, hierarchy );
		count++;
	
	}
	cv::dilate(drawing, drawing, cv::Mat(), cv::Point(-1,-1), 1, cv::BORDER_CONSTANT, cv::morphologyDefaultBorderValue() );			// erode dilate drawing
	return drawing;
}

std::map<int,int> compare_images(cv::Mat GT_segmentation_in, cv::Mat DuDe_segmentation_in, double& pixel_precision, double & pixel_recall, std::vector < std::vector<float> >& Precisions, std::vector < std::vector<float> >& Recalls, std::vector<double>& Times){
			
	std::map<int,int> segmented2GT_tags;
	
	cv::Mat GT_segmentation   = cv::Mat::zeros(GT_segmentation_in.size(),CV_8UC1);
	cv::Mat DuDe_segmentation = cv::Mat::zeros(GT_segmentation_in.size(),CV_8UC1);
	
	GT_segmentation_in  .convertTo(GT_segmentation, CV_8UC1);
	DuDe_segmentation_in.convertTo(DuDe_segmentation, CV_8UC1);			
	tag2tagMapper gt_tag2mapper,DuDe_tag2mapper;
	
	for(int x=0; x < GT_segmentation.size().width; x++){
		for(int y=0; y < GT_segmentation.size().height; y++){
			cv::Point current_pixel(x,y);
								
			int tag_GT   = GT_segmentation.at<uchar>(current_pixel);
			int tag_DuDe  = DuDe_segmentation.at<uchar>(current_pixel);
			
			if(tag_DuDe>0 && tag_GT>0 ){
				gt_tag2mapper  [tag_GT][tag_DuDe].push_back(current_pixel);
				DuDe_tag2mapper[tag_DuDe][tag_GT].push_back(current_pixel);
			}
		}
	}
	
	std::vector<float> precisions_inside, recalls_inside;			
	double cum_precision=0, cum_total=0, cum_recall=0;

//			std::cout << "Regions in GT: "<< std::endl;
	for( tag2tagMapper::iterator it = gt_tag2mapper.begin(); it!= gt_tag2mapper.end(); it++ ){
//				std::cout << "   " << it->first << " connected to "<< std::endl;
		tag2points inside = it->second;
		int max_intersection=0, total_points=0; 
		int gt_tag_max = -1;
		for( tag2points::iterator it2 = inside.begin(); it2!= inside.end(); it2++ ){
			total_points += it2->second.size();
			if (it2->second.size() > max_intersection){
				max_intersection = it2->second.size();
				gt_tag_max = it2->first;
			}
//					std::cout << "      " << it2->first << " with "<< it2->second.size() <<" points" << std::endl;					
		}
		segmented2GT_tags[gt_tag_max] = it->first;
//				std::cout << "   max is " << max_intersection << " that represents " << 100*max_intersection/total_points   << std::endl;
		precisions_inside.push_back(100*max_intersection/total_points);
		cum_precision += max_intersection;
		cum_total += total_points;
	}	
	pixel_precision = cum_precision/cum_total;
			
	cum_total=0;
//			std::cout << "Regions in DuDe: "<< std::endl;
	for( tag2tagMapper::iterator it = DuDe_tag2mapper.begin(); it!= DuDe_tag2mapper.end(); it++ ){
//				std::cout << "   " << it->first << " connected to "<< std::endl;
		tag2points inside = it->second;
		int max_intersection=0, total_points=0; 
		for( tag2points::iterator it2 = inside.begin(); it2!= inside.end(); it2++ ){
			total_points += it2->second.size();
			if (it2->second.size() > max_intersection) max_intersection = it2->second.size();
//					std::cout << "      " << it2->first << " with "<< it2->second.size() <<" points" << std::endl;					
		}
//				std::cout << "   max is " << max_intersection << " that represents " << 100*max_intersection/total_points   << std::endl;
		recalls_inside.push_back(100*max_intersection/total_points);
		cum_recall += max_intersection;
		cum_total += total_points;
	}			
	pixel_recall=cum_recall/cum_total;

	Precisions.push_back(precisions_inside);
	Recalls.push_back   (recalls_inside);
	return(segmented2GT_tags);
}



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
	fuzzy_slam.fast(true);
	
	cv::Mat out_slam;
//  	cv::imshow("SLAM", slam);
// 	cv::waitKey(0);
	fuzzy_slam.fuzzyOpening(slam, out_slam, 500);
	std::cout << "Done opening " << std::endl;
	out_slam.convertTo(out_slam, CV_8U);
	
// 	std::cout << out << std::endl;
	
	std::cout << "/************ REDUCING THE SPACE OF VALUES *****************/\n";
	cv::Mat out_tmp_slam;
	AASS::RSI::reduceZone(out_slam, out_tmp_slam, 2);
	
// 	cv::imshow("REDUCED", out_tmp_slam);
// 	cv::waitKey(0);
	
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

// 	cv::Mat graphmat2 = cv::Mat::zeros(out_slam.size(), CV_8U);
// 	graph_slam.draw(graphmat2);
// 	std::string s = std::to_string(i);
// 	cv::imshow(s, graphmat2);
// 	cv::waitKey(0);

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
	std::string full_path_GT = argv[2];
// 	std::string file = "../../Test/Thermal/cold.jpg";
	AASS::RSI::GraphZone graph_slam;
	
	double begin_process, end_process, decompose_time;
	begin_process = getTime();	
	makeGraph(file, graph_slam);
			
	/********** PCA of all zones in Graph and removing the ripples **********/
	
	graph_slam.updatePCA();
	graph_slam.removeRiplesv2();
	graph_slam.update();
	
	end_process = getTime();	decompose_time = end_process - begin_process;

	cv::Mat slam1 = cv::imread(file, CV_LOAD_IMAGE_GRAYSCALE);
    cv::Mat graphmat = cv::Mat::zeros(slam1.size(), CV_8U);
    graph_slam.drawEvaluation(graphmat);
// 	cv::resize(graphmat, graphmat, cv::Size(graphmat.cols * 2, graphmat.rows * 2));
	cv::imshow("GRAPH", graphmat);
	
    cv::Mat graphmat_vis = cv::Mat::zeros(slam1.size(), CV_8U);
    graph_slam.draw(graphmat_vis);
// 	cv::resize(graphmat, graphmat, cv::Size(graphmat.cols * 2, graphmat.rows * 2));
	cv::imshow("GRAPH Visible", graphmat_vis);
	
	std::cout << "Size of graph" << graph_slam.getNumVertices() << std::endl;

    
	
	//Reading the GT
	
	cv::Mat image_GT = cv::imread(full_path_GT,0);
	cv::Mat GT_segmentation = segment_Ground_Truth(image_GT);
	
	cv::imshow("GT", GT_segmentation);
	cv::waitKey(0);
	
	assert(graphmat.rows == GT_segmentation.rows);
	assert(graphmat.cols == GT_segmentation.cols);
				
	double pixel_precision, pixel_recall;
	std::vector < std::vector<float> > Precisions, Recalls;
	std::vector<double> Times;
	compare_images(GT_segmentation, graphmat, pixel_precision, pixel_recall, Precisions, Recalls, Times);
	
	results pixel, Regions;
	pixel.time = Regions.time = decompose_time;
	extract_results(pixel, Regions, pixel_precision, pixel_recall, Precisions, Recalls, Times);

	double min, max;
	cv::minMaxLoc(GT_segmentation,&min,&max);
	float rows = GT_segmentation.rows;
	float cols = GT_segmentation.cols;
	float proper_size = rows*cols/1000;
	proper_size = proper_size/1000;
			
	std::cout << " No_Furniture Precision: " << Regions.precision << " Recall: "<< Regions.recall << " time: "<< Regions.time <<" Labels " << max <<"  size " << proper_size << std::endl;
	

}