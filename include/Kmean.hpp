#ifndef RSI_KMEANS_18052016
#define RSI_KMEANS_18052016

#include <opencv2/opencv.hpp>
#include "Utils.hpp"

namespace AASS{
	namespace RSI{
		
		class Kmeans{
		protected :
			int _K;
			cv::Mat _bestLabels, _centers;
			
			///@brief All the colors used in the kmean in increasing order
			std::vector< int > _colors;
			
		public:
			Kmeans() : _K(4){}
			
			void setK(int k){_K = k ;}
			int getK(){return _K;}
			
			void printColors() {
				for(size_t i = 0 ; i < _colors.size() ; ++i){
					std::cout << " color " << i << " : " << _colors[i] << std::endl;
				}
			}
			
			std::vector< int > getColors(){return _colors;}
			const std::vector< int >& getColors() const {return _colors;}
			
			void kmeansColor(cv::Mat& in, cv::Mat& dest);
			
		};
	}
}
#endif