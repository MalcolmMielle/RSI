#ifndef BETTERGRAPH_MATCH_BASE_15FEB2016
#define BETTERGRAPH_MATCH_BASE_15FEB2016

#include <iostream>
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

std::string type2str(int type) {
  std::string r;

  uchar depth = type & CV_MAT_DEPTH_MASK;
  uchar chans = 1 + (type >> CV_CN_SHIFT);

  switch ( depth ) {
    case CV_8U:  r = "8U"; break;
    case CV_8S:  r = "8S"; break;
    case CV_16U: r = "16U"; break;
    case CV_16S: r = "16S"; break;
    case CV_32S: r = "32S"; break;
    case CV_32F: r = "32F"; break;
    case CV_64F: r = "64F"; break;
    default:     r = "User"; break;
  }

  r += "C";
  r += (chans+'0');

  return r;
}



class FuzzyOpening{
	
protected:
	int _size;
	
public:
	FuzzyOpening(): _size(3){};
	void setSize(int i){_size = i;}
	
	///@brief return true or false depending on if the circle discribed by circle is empty in input
	bool circleIsEmpty(cv::Mat& input, cv::Mat& circle);
	//Start with biggest circle and go down until empty
	void swypeThroughSrc(const cv::Mat& src, cv::Mat& output, int size);
	void addPointValueInCircle(cv::Mat& input, cv::Mat& output, int value);
	
	
	void fuzzyOpen(cv::Mat& src, cv::Mat& dest){
		
		src.convertTo(src, CV_8UC1);
		
		src.copyTo(dest);
		dest = cv::Scalar(0);
		
		for(size_t i = 1 ; i < _size ;  i = i+1){
			cv::Mat tmp;
			opening(src, tmp, i);
			dest = dest + tmp;
			
			std::cout << "zone " << i << " to " << _size <<std::endl;
			
			cv::imshow("dest tmp", dest);
			cv::imshow("dest tmp tmp", tmp);
			std::cout << cv::Mat::zeros(i, i, CV_8U) << std::endl << std::endl;
			std::cout << tmp << std::endl << "over" << std::endl;
			cv::waitKey(0);
		}
	}
	
private:
	void opening(cv::Mat& src, cv::Mat& dest, int size){
// 		cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(size, size), cv::Point(-1, -1) );
		cv::Mat kernel = cv::Mat::zeros(size, size, CV_8U);
// 		std::cout << kernel << std::endl;
		if(size%2 != 0){
			cv::circle(kernel, cv::Point2i(size/2, size/2), size/2, cv::Scalar(1), -1);
		}
		else{
			cv::circle(kernel, cv::Point2i((size/2)-1, (size/2)-1), (size/2)-1, cv::Scalar(1), -1);
			cv::circle(kernel, cv::Point2i((size/2), (size/2)), (size/2)-1, cv::Scalar(1), -1);
			cv::circle(kernel, cv::Point2i((size/2)-1, (size/2)), (size/2)-1, cv::Scalar(1), -1);
			cv::circle(kernel, cv::Point2i((size/2), (size/2)-1), (size/2)-1, cv::Scalar(1), -1);
		}
// 		cv::imshow("circle__", kernel);
// 		cv::waitKey(0);
		
// 		std::cout << kernel << std::endl << std::endl;
		cv::morphologyEx(src, dest, cv::MORPH_OPEN, kernel);
	};
	
};

//Needs to be int now
void FuzzyOpening::addPointValueInCircle(cv::Mat& input, cv::Mat& output, int value)
{
// 	std::cout << "INPUT " << std::endl << input << std::endl;
	
// 	std::cout << "OUTPUT " << std::endl << output << std::endl;
	//Making circle;
	cv::Mat circle = cv::Mat::zeros(value, value, CV_32F);
	if(value%2 != 0){
		cv::circle(circle, cv::Point2i(value/2, value/2), value/2, cv::Scalar(1), -1);
	}
	else{
		cv::circle(circle, cv::Point2i((value/2)-1, (value/2)-1), (value/2)-1, cv::Scalar(255), -1);
		cv::circle(circle, cv::Point2i((value/2), (value/2)), (value/2)-1, cv::Scalar(255), -1);
		cv::circle(circle, cv::Point2i((value/2)-1, (value/2)), (value/2)-1, cv::Scalar(255), -1);
		cv::circle(circle, cv::Point2i((value/2), (value/2)-1), (value/2)-1, cv::Scalar(255), -1);
	}
	
// 	std::cout << "CERLCE " << std::endl << circle << std::endl;
	
	if(input.rows != circle.rows || input.cols != circle.cols){
		std::ostringstream str_test;
		str_test <<  "Input and cirle are not the same at line " << __LINE__ << " in file " << __FILE__ << "." << std::endl << "Input rows and col : " << input.rows << " " << input.cols << " circle : " << circle.rows << " " << circle.cols ;
		throw std::runtime_error(str_test.str() );
	}
	
	
	for(int row = 0 ; row < input.rows ; row++){
		float* p = input.ptr<float>(row); //point to each row
		float* p_output = output.ptr<float>(row); //point to each row
		float* p_circle = circle.ptr<float>(row); //point to each row
		for(int col = 0 ; col < input.cols ; col++){
			//p[j] <- how to access element
	// 				std::cout << (int)p[j]<< std::endl;
// 			std::cout << "Comparing " << (float)p[col] << " and " << value << " while circle " << p_circle[col] << std::endl;
			if((float)p[col] > 0 && (float)p_output[col] < value && p_circle[col] > 0){
				p_output[col] = value;
			}
// 			if((float)p[col] == 0 && p_circle[col] > 0){
// 				p[col] = value;
// 			}
		}
	}
// 	return true;
	
	

}

void FuzzyOpening::swypeThroughSrc(const cv::Mat& src, cv::Mat& output, int size)
{
	//Calcul distance image
	cv::Mat distance_image, label;
	if(src.channels() == 3){
		std::cout << "Convert" << std::endl;
		cv::cvtColor(src, distance_image, CV_RGB2GRAY);
	}
	else{
		src.copyTo(distance_image);
	}
	
// 	distance_image.convertTo(CV_);
	
	cv::threshold(distance_image, distance_image, 50, 255, CV_THRESH_BINARY_INV);
	cv::distanceTransform(distance_image, distance_image, label, CV_DIST_L2, CV_DIST_MASK_PRECISE, CV_DIST_LABEL_CCOMP);
// 	std::cout << " TYPE " << type2str(distance_image.type()) << std::endl;	
	CV_Assert(distance_image.depth() == CV_32F);
	
	int pad = distance_image.rows;
	int old_rows = distance_image.rows;
	int old_cols = distance_image.cols;
	if(pad < distance_image.cols){
		pad = distance_image.cols;
	}
	
	cv::copyMakeBorder( distance_image, distance_image, pad, pad, pad, pad, cv::BORDER_CONSTANT, 0 );
	
	output = cv::Mat::ones(distance_image.rows, distance_image.cols, CV_32F);
	cv::Mat roi_output_final = output(cv::Rect(pad, pad, old_rows, old_cols));
// 	std::cout << roi_output_final << std::endl;
	//Update result 
// 	int count = 0;
	for(int row = pad ; row < distance_image.rows - pad + 1 ; row++){
		float* p = distance_image.ptr<float>(row); //point to each row
		float* p_output = output.ptr<float>(row); //point to each row
		for(int col = pad ; col < distance_image.cols - pad + 1 ; col++){
			
			int dist_to_obstacle = (float)p[col];
			if(dist_to_obstacle > _size){
				dist_to_obstacle = _size;
			}
// 			std::cout << row << " " << col << " " << " dist : " <<dist_to_obstacle << " same " << (float)p[col] << " | " ;
			
			cv::Mat roi = distance_image(cv::Rect(col - (dist_to_obstacle/2), row - (dist_to_obstacle/2), dist_to_obstacle, dist_to_obstacle));
			cv::Mat roi_output = output(cv::Rect(col - (dist_to_obstacle/2), row - (dist_to_obstacle/2), dist_to_obstacle, dist_to_obstacle));
			if((int)dist_to_obstacle > 0){
				addPointValueInCircle(roi, roi_output, dist_to_obstacle);
// 				std::cout << std::endl;
// 				std::cout << roi_output_final << std::endl;
// 				int a ;
// 				std::cin >> a;
// 				count++;
// 				if(count % 10 == 0){
// // 					std::cout << "LOL" << std::endl;
// 					cv::Mat copy;
// 					roi_output_final.copyTo(copy);
// 					cv::normalize(copy, copy, 0, 1, cv::NORM_MINMAX, CV_32F);
// 					cv::imshow("out", copy);
// 					
// 					cv::waitKey(1);
// 				}
			}
			else{
				p_output[col] = 0;
// 				std::cout << "Wall" << std::endl;
			}
			
			
			
		}
// 		std::cout << std::endl;
	}
	cv::normalize(roi_output_final, roi_output_final, 0, 1, cv::NORM_MINMAX, CV_32F);
	
// 	std::cout << out << std::endl;
	
	cv::imshow("out", roi_output_final);
	cv::waitKey(0);

}

//I don't need that in the end 
bool FuzzyOpening::circleIsEmpty(cv::Mat& input, cv::Mat& circle)
{
	for(int row = 0 ; row < input.rows ; row++){
		uchar* p = input.ptr(row); //point to each row
		uchar* p_circle = circle.ptr(row); //point to each row
		for(int col = 0 ; col < input.cols ; col++){
			//p[j] <- how to access element
	// 				std::cout << (int)p[j]<< std::endl;
			if(p[col] > 0 && p_circle[col] > 0){
				return false;
			}
		}
	}
	return true;
	
	
}






#endif