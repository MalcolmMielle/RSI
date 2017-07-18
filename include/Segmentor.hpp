#ifndef RSI_SEGMENTOR_17072017
#define RSI_SEGMENTOR_17072017

#include <sys/time.h>

#include "GraphZone.hpp"
#include "ZoneReducer.hpp"
#include "ZoneExtractor.hpp"
#include "FuzzyOpening.hpp"

namespace AASS{
	
	namespace RSI{
		
		class Segmentor{
			
		protected:
			std::vector< std::vector< cv::Point> > _contours;
			std::vector<std::pair<cv::Point, cv::Point> > _limits;
			cv::Mat _segmented;
			
		public:
			Segmentor(){}
			
			double segmentImage(cv::Mat& src, AASS::RSI::GraphZone& graph_src);			
			
			cv::Mat& getSegmentedMap(){return _segmented;}
			const cv::Mat& getSegmentedMap() const {return _segmented;}
			
		private:
			
			double getTime() //in millisecond
			{
				//assuming unix-type systems
				//timezone tz;
				timeval  tv;
				gettimeofday(&tv, NULL);
				return (tv.tv_sec*1000000+tv.tv_usec)*1.0/1000;
			}
			
			void addHoles(const cv::Mat& src, std::vector<std::vector<cv::Point> > contours, std::vector<cv::Vec4i> hierarchy, AASS::RSI::GraphZone& graph_src);
			
			void findLimits(const cv::Mat& src, AASS::RSI::GraphZone& graph_src);
			
			
			
			
		};
		
		
		
		
		
	}
}
#endif