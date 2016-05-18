#ifndef RSI_FUZZYOPENING_16052016
#define RSI_FUZZYOPENING_16052016

#include <iostream>
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "Utils.hpp"

namespace AASS{
	namespace RSI{
		/**
		* @brief Fuzzy opening on an image
		* Calculate the size of the biggest circle that any pixel can be fitted in. Circles don't have to be centered on the pixel considered.
		*/
		class FuzzyOpening{
			
		protected:
			/**
			* @brief Max distance from a point to a wall to be consider an open space.
			* All point that can fit in a circle with at least radius _size will be considered the same
			*/
			int _size;
			/**
			* @brief Fast or accurate
			*/
			bool _fast;
			/**
			* @brief vector of element object of different size for not having to generate them each time
			*/
		// 	std::vector<cv::Mat> _elements;
			/**
			* @brief id of element
			* 0 : circle
			* 1 : rectangle
			*/
		// 	size_t _element_id;
			
		public:
			FuzzyOpening(): _size(3), _fast(false){};
			void setSize(int i){_size = i;}
			
		// 	void generateElements();
		// 	void generateCircle(size_t size, cv::Mat& out);
			
			///@brief Determine if the algorithm needs to run fast or accuratly. Cannot be fast AND accurate for now
			void fast(bool fast){_fast = fast;}
			///@brief Determine if the algorithm needs to run fast or accuratly. Cannot be fast AND accurate for now
			void accurate(bool acc){_fast = !acc;}
			
			///@brief return true or false depending on if the circle discribed by circle is empty in input
			bool circleIsEmpty(cv::Mat& input, cv::Mat& circle);
			//Start with biggest circle and go down until empty
			void fuzzyOpening(const cv::Mat& src, cv::Mat& output, int size);
			void addPointValueInCircle(cv::Mat& input, cv::Mat& output, int value);
			
			
			
		private:
			
		};

		// void FuzzyOpening::generateElements()
		// {
		// 	for(size_t i = 0 ; i < _size ; ++i){
		// // 		if()
		// 	}
		// 
		// }
	}
}





#endif