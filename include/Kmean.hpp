#ifndef RSI_KMEANS_18052016
#define RSI_KMEANS_18052016

#include <opencv2/opencv.hpp>
#include "Utils.hpp"

namespace AASS{
	namespace RSI{
		
		class Kmeans{
		protected :
			int _K;
			
		public:
			Kmeans() : _K(4){}
			
			void setK(int k){_K = k ;}
			int getK(){return _K;}
			
			void kmeansColor(cv::Mat& in, cv::Mat& dest);
			
		};
	}
}
#endif