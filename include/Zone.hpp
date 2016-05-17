#ifndef RSI_ZONE_17052016
#define RSI_ZONE_17052016

#include <iostream>
#include <stdio.h>
#include <opencv2/opencv.hpp>

class Zone{
protected:
	std::pair < size_t, std::deque <cv::Point2i > > _zone;
	
public:
	Zone(){};
	
	void push_back(const cv::Point2i& p){_zone.second.push_back(p);}
	void push_front(const cv::Point2i& p){_zone.second.push_front(p);}
	void pop_back(){_zone.second.pop_back();}
	void pop_front(){_zone.second.pop_front();}
	size_t size(){return _zone.second.size();}
	void clear(){_zone.second.clear(); _zone.first = 0;}
	cv::Point2i& operator[](int i){return _zone.second[i];};
	const cv::Point2i& operator[](const int i) const {return _zone.second[i];};
	
	void setValue(size_t i){_zone.first = i;}
	size_t getValue(){return _zone.first;}
	const std::deque <cv::Point2i >& getZone() const {return _zone.second;}
	std::deque <cv::Point2i >& getZone(){return _zone.second;}
	
	
};

#endif