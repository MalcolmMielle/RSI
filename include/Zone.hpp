#ifndef RSI_ZONE_17052016
#define RSI_ZONE_17052016

#include <iostream>
#include <stdio.h>
#include <opencv2/opencv.hpp>

class Zone{
protected:
	std::pair < size_t, std::deque <cv::Point2i > > _zone;
	///@brief sum of all value of x and y of all point in zone. For fast update and get of centroid
	cv::Point2i _sum_of_x_and_y;
	
public:
	Zone(){};
	
	void push_back(const cv::Point2i& p){_zone.second.push_back(p); addPoint(p);}
	void push_front(const cv::Point2i& p){_zone.second.push_front(p); addPoint(p);}
	
	void pop_back(){_zone.second.pop_back(); removePoint(_zone.second.size()-1);}
	void pop_front(){_zone.second.pop_front(); removePoint(0);}
	
	bool isEmpty(){return (0 == _zone.second.size());}
	
	size_t size(){return _zone.second.size();}
	void clear(){_zone.second.clear(); _zone.first = 0; _sum_of_x_and_y.x = 0 ; _sum_of_x_and_y.y = 0;}
	cv::Point2i& operator[](int i){return _zone.second[i];};
	const cv::Point2i& operator[](const int i) const {return _zone.second[i];};
	
	void setValue(size_t i){_zone.first = i;}
	size_t getValue(){return _zone.first;}
	size_t getValue() const {return _zone.first;}
	const std::deque <cv::Point2i >& getZone() const {return _zone.second;}
// 	std::deque <cv::Point2i >& getZone(){return _zone.second;}
	
	cv::Point2i getCentroid(){
// 		int x = 0;
// 		int y = 0;
// 		for(size_t i = 0 ; i < _zone.second.size(); ++i){
// 			x = x + _zone.second[i].x;
// 			y = y + _zone.second[i].y;
// 		}
// 		x = x / _zone.second.size();
// 		y = y / _zone.second.size();
// 		return cv::Point2i(y, x );	
		if( _zone.second.size() == 0 ){
			throw std::runtime_error("zone is empty");
		}
		return cv::Point2i(_sum_of_x_and_y.y / _zone.second.size(), _sum_of_x_and_y.x / _zone.second.size());	
	}
	const cv::Point2i getCentroid() const {
		/*int x = 0;
		int y = 0;
		for(size_t i = 0 ; i < _zone.second.size(); ++i){
			x = x + _zone.second[i].x;
			y = y + _zone.second[i].y;
		}
		
		std::cout << "Place " << x << " " << _zone.second.size() << std::endl;
		x = x / _zone.second.size();
		y = y / _zone.second.size();
		return cv::Point2i(y, x );*/
		if( _zone.second.size() == 0 ){
			throw std::runtime_error("zone is empty");
		}
		return cv::Point2i(_sum_of_x_and_y.y / _zone.second.size(), _sum_of_x_and_y.x / _zone.second.size());		
	}
	
private: 
	void addPoint(const cv::Point2i& p){
		_sum_of_x_and_y.x = _sum_of_x_and_y.x + p.x;
		_sum_of_x_and_y.y = _sum_of_x_and_y.y + p.y;
	}
	void removePoint(int i){
		_sum_of_x_and_y.x = _sum_of_x_and_y.x - _zone.second[i].x;
		_sum_of_x_and_y.y = _sum_of_x_and_y.y - _zone.second[i].y;
	}
	
};

#endif