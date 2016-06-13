#include <iostream>
#define BOOST_TEST_DYN_LINK

#define BOOST_TEST_MODULE MyTest
#include <time.h>
#include <cstdlib>
#include <boost/test/unit_test.hpp>
#include <fstream>
#include <ctime> 

#include <ctime>

#include "ZoneExtractor.hpp"
#include "FuzzyOpening.hpp"
#include "Kmean.hpp"
#include "MeanShift.hpp"


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
	
	cv::threshold(slam, slam, 20, 255, cv::THRESH_BINARY);
	
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
	int size_to_remove = 50;
	while(K != 0){
// 		AASS::RSI::Kmeans kmeans;
// 		kmeans.setK(K);
// 		kmeans.kmeansColor(out, out_tmp);
		
		//TODO : valuye here 
		//ATTENTION : do this only for slam
// 		cv::threshold(out_slam, out_slam, 20, 0, cv::THRESH_TOZERO);
		
		cv::imshow("nefore", out_slam);
		cv::waitKey(0);
		
		bool kmean_f = true;
		AASS::RSI::Kmeans kmeans_slam;
		AASS::RSI::MeanShift mshift;
		//ATTENTION : KMEAN
		if(kmean_f == true){
			kmeans_slam.setK(K);
			kmeans_slam.kmeansColor(out_slam, out_tmp_slam, slam, 255);
		}
		//ATTENTION : MeanShift
		else{
// 			cv::Mat tmp;
			cv::Mat mean;
// 			cv::cvtColor(out_slam, tmp, CV_GRAY2RGB);
			mshift.meanshift(out_slam, mean);
			cv::imshow("Mean Shift", mean);
			kmeans_slam.setK(K);
			kmeans_slam.kmeansColor(mean, out_tmp_slam, slam, 255);
			
			cv::Mat kmeanout;
			//For comparison prurpose
			kmeans_slam.kmeansColor(out_slam, kmeanout, slam, 255);
			cv::imshow("Kmean old", kmeanout);
			cv::waitKey(0);
		}
// 		out_tmp_slam = out_slam;
// 		std::cout << out_tmp_slam << std::endl;
// 		exit(0);
		//Still good here
// 		std::cout << out_tmp_slam << std::endl;
		
// 		cv::imshow("kmenas", out_tmp);
		cv::imshow("kmenas or mShift SLAM", out_tmp_slam);
		cv::waitKey(0);
		
		AASS::RSI::ZoneExtractor wzoneextract;
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
		
		wzoneextract.extract(out_tmp_slam);
		
		std::cout << "WATERSHED DONE" << std::endl;
		
// 		Still good
// 		std::cout << out_tmp_slam << std::endl;
// 		exit(0);
// 		watershed.print();
// 		
		std::cout << "Final zone number " << wzoneextract.size() <<std::endl;
		cv::Mat graphmat;
		out_tmp_slam.copyTo(graphmat);
		
		AASS::RSI::GraphZone graph_slam;
		std::cout << "Getting the graph" << std::endl;
		graph_slam = wzoneextract.getGraph();
		
		std::cout << "HGetting the color" << std::endl;
		int color_wall_slam = 0 ;
		if(kmean_f == true){
			if(kmeans_slam.getColors().size() >0 ){
				color_wall_slam = kmeans_slam.getColors()[0];
			}
			
			std::cout << "WALLS ARE " << color_wall_slam << std::endl;
			kmeans_slam.printColors();
		}
		
// 		std::cout << "REmove Vertex" << std::endl;
		
// 		graph_slam.draw(graphmat);
// 		cv::imshow("GRAPH before", graphmat);
		
		graph_slam.removeVertexValue(color_wall_slam);
// 		graph_slam.removeVertexUnderSize(size_to_remove, true);
		
		graph_slam.drawPartial(graphmat);
		cv::imshow("GRAPH after", graphmat);
		
		std::cout << "Graph : " << std::endl;
		
		std::cout << "Number of vertices : " << graph_slam.getNumVertices() << " number of edges " << graph_slam.getNumEdges() << std::endl;
		
// 		cv::imshow("out_tmp", out_tmp);
		cv::waitKey(0);
// 		watershed.drawAllZones(out_tmp, 0);
// 		std::cout << "Value of K : " ;
// 		std::cin >> K;
// 		std::cout << "Value of size to remove : " ;
// 		std::cin >> size_to_remove;
	}
	
	
}