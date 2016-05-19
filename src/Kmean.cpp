#include "Kmean.hpp"

void AASS::RSI::Kmeans::kmeansColor(cv::Mat& in, cv::Mat& dest){
	cv::imshow("input", in);
	cv::Mat input;
	std::cout << "TYPE " << type2str(in.type()) << std::endl;
	in.convertTo(input, CV_8UC1);
	
	cv::Mat p = cv::Mat::zeros(input.cols*input.rows, 3, CV_32F);
	std::vector<cv::Mat> bgr;
	cv::split(input, bgr);
// 				SLOWWWWW
	for(int i = 0 ; i < input.cols*input.rows ; i++) {
		p.at<float>(i,0) = (i/input.cols) / input.rows; //Needs to be zero because it fails when I seem to put the good values...
		p.at<float>(i,1) = (i%input.cols) / input.cols;
		p.at<float>(i,2) =  bgr[0].data[i] / 255.0;
	}
	
// 				for(int row = 0 ; row < input.rows ; row++){
// 					uchar* pointer = input.ptr<uchar>(row); //point to each row
// 					for(int col = 0 ; col < input.cols ; col++){
// // 						std::cout << "value : " << (float) pointer[col] << " at " << (row * input.cols) + col << std::endl;
// 						p.at<float>( (row * input.cols) + col, 0) = (float) row / input.rows;
// 						p.at<float>( (row * input.cols) + col, 1) = (float) col / input.cols;
// 						p.at<float>( (row * input.cols) + col, 2) = pointer[col] / 255.0;
// 					}
// 				}
	
// 				std::cout << "second p " << std::endl << p << std::endl;
// 				exit(0);
// 				std::cout << "DATA "<< std::endl << p << std::endl;
	cv::kmeans(p, _K, _bestLabels, cv::TermCriteria( CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 10, 1.0), 3, cv::KMEANS_PP_CENTERS, _centers);
	
	int colors[_K];
	for(int i=0; i<_K; i++) {
		colors[i] = 255/(i+1);
	}
	// i think there is a better way to do this mayebe some Mat::reshape?
	dest = cv::Mat::zeros(input.rows, input.cols, CV_8U);
// 				for(int i=0; i<input.cols*input.rows; i++) {
// 					dest.at<float>(i/input.cols, i%input.cols) = (float)(colors[bestLabels.at<int>(0,i)]);
// 				//      cout << bestLabels.at<int>(0,i) << " " << 
// 				//              colors[bestLabels.at<int>(0,i)] << " " << 
// 				//              clustered.at<float>(i/src.cols, i%src.cols) << " " <<
// 				//              endl;
// 				}
	
	std::cout << "TYPE " << type2str(_centers.type()) << " size " << _centers.rows << " " << _centers.cols << std::endl;
// 				
	uint32_t* pointer_label = _bestLabels.ptr<uint32_t>(0); //point to each row
	
	for(int row = 0 ; row < input.rows ; row++){
		uchar* pointer = dest.ptr<uchar>(row); //point to each row
		
		for(int col = 0 ; col < input.cols ; col++){
// 						std::cout << "AT " << pointer_label[(row * input.cols) + col] << std::endl;
			float* pointer_center = _centers.ptr<float>(pointer_label[(row * input.cols) + col]);
			
// 						std::cout << "Center : " << pointer_center[0] << " " << pointer_center[1] << " " << pointer_center[2] * 255 << " " << std::endl;
// 						std::cout << "Value in base " << (int) in.at<uchar>(pointer_center[0] * in.rows, pointer_center[1] * in.cols) << std::endl;
		
// 						pointer[col] = (uchar)(colors[ pointer_label[(row * input.cols) + col] ]);
// 						not working
// 						pointer[col] = (uchar)(pointer_center[2] * 255);
// 			std::cout << "adding " << (int) ( ((float)pointer_center[2] * 255) ) << std::endl;
			pointer[col] = (int) ( ((float)pointer_center[2] * 255) );
		}				
	} 
// 	exit(0);
	//Add all colors to colors
	for(int row = 0 ; row < _centers.rows ; row++){
		float* pointer_center = _centers.ptr<float>(row); //point to each row
		
		
// 						std::cout << "AT " << pointer_label[(row * input.cols) + col] << std::endl;
		_colors.push_back( (int) ( ((float)pointer_center[2] * 255) ) );
		
	}
	std::sort(_colors.begin(), _colors.end());
	
// 				for(int row = 0 ; row < centers.rows ; row++){
// 					uchar* pointer = dest.ptr<uchar>(row); //point to each row
// 					float* pointer_center = centers.ptr<float>(row);
// // 						std::cout << "AT " << pointer_label[(row * input.cols) + col] << std::endl;
// 					std::cout << "Center : " << pointer_center[0] * input.rows << " " << pointer_center[1] * input.cols << " " << (float) ((float)pointer_center[2] * 255) << " color " << (int) (in.at<uchar>(pointer_center[0] * input.rows, pointer_center[1] * input.cols)) << std::endl;
// 						
// 					
// 				}
	
// 				std::cout << centers << std::endl;
// 				cv::normalize(dest, dest, 0, 255, cv::NORM_MINMAX, CV_8U);
// 				dest.convertTo(dest, CV_8U);
	
// 	std::cout << dest << std::endl;
}