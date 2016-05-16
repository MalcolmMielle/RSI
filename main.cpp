#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "FuzzyOpening.hpp"

int main(int argc, char **argv) {
    //Voronoi
	cv::Mat mapin = cv::imread("labsketch.png");
	cv::imshow("input", mapin);
	cv::Mat voronoi;
	cv::Mat label;
	cv::Mat sharpner;
	cv::Mat res;
	cv::Mat big_zones;
	
	if(mapin.channels() == 3){
		cv::cvtColor(mapin, voronoi, CV_RGB2GRAY);
	}
	else{
		mapin.copyTo(voronoi);
	}
	
// 	cv::imshow("That voro", _map_in);
// 	cv::waitKey(0);
	
	cv::threshold(voronoi, voronoi, 50, 255, CV_THRESH_BINARY_INV);
	
	std::cout << "distance transform" << std::endl;
	//Laplace use a pixel distance instead of the Euclidean one
// 		Pixel distance give way to good result for sketch map but is PERFECT for building maps.
// 		cv::distanceTransform(voronoi, voronoi, _label, CV_DIST_C, CV_DIST_MASK_PRECISE, CV_DIST_LABEL_CCOMP);
	cv::distanceTransform(voronoi, voronoi, label, CV_DIST_L2, CV_DIST_MASK_PRECISE, CV_DIST_LABEL_CCOMP);
// 	std::cout << "Voronoi " << std::endl << voronoi << std::endl;
	
// 	cv::imshow("That voro", voronoi);
// 	cv::waitKey(0);

	//0 - 200 seems pretty good as an interval
	cv::normalize(label, label, 0, 200, cv::NORM_MINMAX);
	
	//std::cout << _label << "Tdammmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm"<<std::endl;
	
	//Make it more sharp.
	cv::Mat tmp;
	cv::GaussianBlur(voronoi, tmp, cv::Size(0, 0), 3);
	cv::addWeighted(voronoi, 1.5, tmp, 0, 0, voronoi);
	
	voronoi.convertTo(voronoi,CV_32F);
	
	int trunc = 100;
	cv::threshold(voronoi, voronoi, trunc, 150, CV_THRESH_TRUNC);
	for (int i = 0; i < voronoi.rows; ++i) {
		for (int j = 0; j < voronoi.cols; ++j) {
// 			std::cout << voronoi.at<float>(i, j) << std::endl;
			if(voronoi.at<float>(i, j) == trunc){
// 				std::cout << "YEA"<<std::endl;
				voronoi.at<float>(i, j) = trunc + 50;
			}
		}
	}
	
	voronoi.copyTo(res);
	voronoi.copyTo(big_zones);
	big_zones.convertTo(big_zones, CV_8U);
	cv::threshold(big_zones, big_zones, trunc, 150, CV_THRESH_BINARY);
	
	for (int i = 0; i < res.rows; ++i) {
		for (int j = 0; j < res.cols; ++j) {
			int val = res.at<float>(i, j);
			val = val/10;
			val = val * 10;
			res.at<float>(i, j) = val;
		}
	}
// 	std::cout << "Voronoi " << std::endl << voronoi << std::endl;
	cv::normalize(voronoi, voronoi, 0, 255, cv::NORM_MINMAX);
	cv::normalize(res, res, 0, 255, cv::NORM_MINMAX);
// 	std::cout << "Voronoi " << std::endl << voronoi << std::endl;
	
	/*******************************************************************************/
	
	FuzzyOpening fuzzy;
	
	
	
	for(size_t i = 3 ; i < 100 ; i = i +10){
		cv::Mat mapin_fuzzy = cv::imread("test.png", CV_LOAD_IMAGE_GRAYSCALE);
		cv::Mat dest;
		fuzzy.setSize(i);
		fuzzy.fuzzyOpen(mapin_fuzzy, dest);
		std::cout << "OPEN" << std::endl;
		cv::imshow("Open fuzzy", dest);
		cv::waitKey(0);
	}
	
	voronoi.convertTo(voronoi, CV_8U);
	cv::imshow("Final wzones", voronoi);
	
	big_zones.convertTo(big_zones, CV_8U);
	cv::imshow("Only biggest wzones", big_zones);
	
	res.convertTo(res, CV_8U);
	cv::imshow("Some sort of zones wzones", res);
	
	cv::waitKey(0);
	
	//Threshold the voronoi
	
	//Every distance more than 25 pixel get the value 25
	//And then distance are redistributed
	
	
	
	
	
// 	/*
// 	 * Those two method are the same
// 	 * 
// 	 * METHOD 1
// 	 */
// 	cv::Laplacian(voronoi, res, -1);
// 	
// 	
// 	double min;
// 	double max;
// 	cv::Point2i p_minloc;
// 	cv::Point2i p_maxloc;
// 	cv::minMaxLoc(res, &min, &max, &p_minloc, &p_maxloc);
// 	std::cout << "partial calculation is " << (0 * (-min) ) / 100 << std::endl;
// 	double level_tmp = 0 / ( (10 * (-min) ) / 100);
// 	std::cout << "level is "<< level_tmp << " with min " << min <<  std::endl;
// 	
// 	for (int i = 0; i < res.rows; ++i) {
// 		for (int j = 0; j < res.cols; ++j) {
// 			//remove positive values
// 			if(res.at<float>(i, j)>=0){
// 				res.at<float>(i, j) = 0;
// 			}
// 			else{
// 				//Magic bit
// 				//this remove lots of value close to zero but not zero. like 5.96046e-08 but keep 0.00803226 and that's what we want.
// 				
// 				/*
// 				 * This only keep every value LESS than the value of the percentage of min we want. For exmaple, _level = 30, we remove every value above 30% of minima and keep the remaining 70%.
// 				 * This is how it work : 
// 				 * * level_tmp is 1/(percentage of minima we want)
// 				 * * pixel value * level_tmp > 1 if pixel value is more than 1/(percentage of minima we want)
// 				 * * pixel value * level_tmp < 1 if pexel value is less than 1/(percentage of minima we want)
// 				 * * int level is the new value of pixel value * level_tmp. Thus level is 0 is pixel value * level_tmp is less than 1 and level is more than 0 if pixel value * level_tmp is more than one.
// 				 * * pixel_value is replace by level.
// 				 * 
// 				 * And then the full imaged is treshold keeping all value above 0.
// 				 */
// 				if(level_tmp != 0){
// 					int level =  level_tmp * ( - res.at<float>(i, j) );
// 					res.at<float>(i, j) = level;
// 				}
// 				else{
// 					res.at<float>(i, j) = 100 * ( - res.at<float>(i, j) );
// 				}
// 			}
// 		}
// 	}
// 	cv::threshold(res, res, 0, 255, CV_THRESH_BINARY);
// 	res.convertTo(res, CV_8U);
// 	
// // 	cv::threshold(res, res, 0, 255, CV_THRESH_BINARY);
// // 	res.convertTo(res, CV_8U);
// 	
// 	cv::imshow("voronoi", res);
// 	cv::waitKey(0);
// 	
	
	//WATERSHED
	
	
}
