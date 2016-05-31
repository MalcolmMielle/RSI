#ifndef RSI_ZONE_17052016
#define RSI_ZONE_17052016

#include <iostream>
#include <stdio.h>
#include <opencv2/opencv.hpp>

namespace AASS{
	
	namespace RSI{
		class Zone{
		protected:
			size_t _value;
			std::deque <cv::Point2i> _zone;
			///@brief sum of all value of x and y of all point in zone. For fast update and get of centroid
			cv::Point2i _sum_of_x_and_y;
			///@brief Zone drawn on the Mat
			cv::Mat _zone_mat;
			
		public:
			Zone(){}; //Used for the read function of betterGraph
			Zone(const cv::Size& size){
				_zone_mat = cv::Mat::zeros(size, CV_8U);
			};
			Zone(int rows, int cols){
				_zone_mat = cv::Mat::zeros(rows, cols, CV_8U);
			};
			
			void push_back(const cv::Point2i& p){_zone.push_back(p); addPoint(p);}
			void push_front(const cv::Point2i& p){_zone.push_front(p); addPoint(p);}
			
			void pop_back(){removePoint(_zone.size()-1); _zone.pop_back();}
			void pop_front(){removePoint(0); _zone.pop_front();}
			
			bool isEmpty(){return (0 == _zone.size());}
			void setImageSize(const cv::Mat& in){_zone_mat = cv::Mat::zeros(in.size(), CV_8U);};
			
			size_t size() const {return _zone.size();}
			
			void clear(){
				_zone.clear(); 
				_value = 0; _sum_of_x_and_y.x = 0 ; _sum_of_x_and_y.y = 0; 
				_zone_mat = cv::Mat::zeros(_zone_mat.size(), CV_8U);
			}
			
			cv::Point2i& operator[](int i){return _zone[i];};
			const cv::Point2i& operator[](const int i) const {return _zone[i];};
			
			void setValue(size_t i){_value = i;}
			size_t getValue(){return _value;}
			size_t getValue() const {return _value;}
			const std::deque <cv::Point2i >& getZone() const {return _zone;}
		// 	std::deque <cv::Point2i >& getZone(){return _zone;}
			const cv::Mat& getZoneMat() const {return _zone_mat;}
			
			cv::Point2i getCentroid(){
				if( _zone.size() == 0 ){
					throw std::runtime_error("zone is empty");
				}
				return cv::Point2i(_sum_of_x_and_y.y / _zone.size(), _sum_of_x_and_y.x / _zone.size());	
			}
			const cv::Point2i getCentroid() const {
				if( _zone.size() == 0 ){
					throw std::runtime_error("zone is empty");
				}
				return cv::Point2i(_sum_of_x_and_y.y / _zone.size(), _sum_of_x_and_y.x / _zone.size());		
			}
			
			
			void fuse(const Zone& input){
				for(size_t i = 0 ; i < input.size() ; ++i){
					this->push_back(input.getZone()[i]);
				}
			}
			
			
			void draw(cv::Mat& img, const cv::Scalar& color) const{
				img.convertTo(img, CV_8U);
				for(int row = 0 ; row < img.rows  ; row++){
// 					uchar* p = img.ptr<uchar>(row); //point to each row
// 					uchar* pz = _zone_mat.ptr<uchar>(row); //point to each row
					for(int col = 0 ; col < img.cols; col++){
						if(_zone_mat.at<uchar>(row, col) == 255){
							uchar colllo = color[0];
							img.at<uchar>(row, col) = colllo;
						}
					}
				}
			}
			
// 			void draw()const {
// 				
// 			}
			
		private: 
			void addPoint(const cv::Point2i& p){
				_sum_of_x_and_y.x = _sum_of_x_and_y.x + p.x;
				_sum_of_x_and_y.y = _sum_of_x_and_y.y + p.y;
				//TODO drawing function of new point
				_zone_mat.at<uchar>(p.x, p.y) = 255;
			}
			void removePoint(int i){
				_sum_of_x_and_y.x = _sum_of_x_and_y.x - _zone[i].x;
				_sum_of_x_and_y.y = _sum_of_x_and_y.y - _zone[i].y;
				//TODO un-drawing function of new point
				_zone_mat.at<uchar>(_zone[i].x, _zone[i].y) = 0;
			}
			
		};
		
		
		inline std::ostream& operator<<(std::ostream& in, const Zone &p){
			
			in << "v " << p.getValue() << " s " << p.size();

			return in;
			
		}
		
		inline std::istream& operator>>(std::istream& in, Zone &p){
			char tmp;
			in >> tmp;
			std::cout << "v " << tmp << std::endl;
			int vall;
			
			in >> vall;
// 			std::cout << "v " << vall << std::endl;
			in >> tmp;
			int vall_s;
			in >> vall_s;
			
			p.setValue(vall);
					
			return in;
		}
	}
}

#endif