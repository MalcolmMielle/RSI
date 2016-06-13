#ifndef RSI_SKETCHMAP_13062016
#define RSI_SKETCHMAP_13062016

#include <opencv2/opencv.hpp>
#include "GraphZone.hpp"

//TODO
namespace AASS{
	namespace RSI{
		
		struct SketchMap{
			cv::Mat sketch;
			cv::Mat room;
			int background_color;
			GraphZone graph;
		};
		
	}
}


#endif