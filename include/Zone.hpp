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
			
			std::tuple<cv::Point, cv::Point, cv::Point> _pca;
			double _pca_orientation;
			
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
			std::tuple<cv::Point, cv::Point, cv::Point> getPCA(){return _pca;}
			const std::tuple<cv::Point, cv::Point, cv::Point>& getPCA() const {return _pca;}
			double getPCAOrientation(){return _pca_orientation;}
			double getPCAOrientation() const {return _pca_orientation;}
			
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
				
				//Draw PCA
// 				cv::circle(img, std::get<0>(_pca), 3, CV_RGB(255, 0, 255), 10);
				cv::line(img, std::get<0>(_pca), std::get<1>(_pca) , cv::Scalar(255), 3);
				cv::line(img, std::get<0>(_pca), std::get<2>(_pca) , cv::Scalar(155), 3);	
				
				std::cout << "One line " << std::get<0>(_pca) << " " << std::get<1>(_pca) << std::endl;
				
			}
			
// 			void draw()const {
// 				
// 			}

			void PCA(){
// 				find countour
				cv::Mat copy_tmp;
				_zone_mat.copyTo(copy_tmp);
				cv::Mat copy =  cv::Scalar::all(255) - _zone_mat;
				
				
				std::vector< std::vector< cv::Point> > contours;
				std::vector<cv::Vec4i> hierarchy;
				cv::findContours(copy_tmp, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);
				
// 				assert(contours.size() == 1 && "More than one shape in Zone");
				
				// Calculate the area of each contour
				
				//Use a lambda function to sort the contours
				std::sort(contours.begin(), contours.end(), [](std::vector<cv::Point> &pts, std::vector<cv::Point> &pts2){return pts.size() > pts2.size(); } );
				
				if(contours.size() > 1)
					assert(contours[0].size() > contours[1].size());
				
				auto area = cv::contourArea(contours[0]);
				// Ignore contours that are too small or too large
// 				if (area < 1e2 || 1e5 < area) {
			
					// Draw each contour only for visualisation purposes
	// 				cv::drawContours(img, contours, i, CV_RGB(255, 0, 0), 2, 8, hierarchy, 0);
					// Find the orientation of each shape
					getOrientation(contours[0]);
// 				}
// 				else{
					
// 					std::cout << "TOO small :( " << std::endl;
					
// 				}
				
				std::cout << "PCAAAAA" << std::endl;
				std::cout << "One line " << std::get<0>(_pca) << " " << std::get<1>(_pca) << std::endl;
				std::cout << "One line " << std::get<0>(_pca) << " " << std::get<2>(_pca) << std::endl;
				std::cout << "Next PCA " << std::endl << std::endl;
// 				cv::imshow("Bobafter", _zone_mat);
// 				cv::imshow("Bob", copy_tmp);
// 				cv::waitKey(0);

			}
			
			//Return the number of contact points in percent compared to size of contour
			int contactPoint(const Zone& zone){
				cv::Mat copyTest;
				zone.getZoneMat().copyTo(copyTest);
				
// 				cv::imshow("i",zone.getZoneMat());
// 				cv::imshow("b",_zone_mat);
				cv::Mat copy_tmp;
				_zone_mat.copyTo(copy_tmp);
				cv::Mat copy =  cv::Scalar::all(255) - _zone_mat;
				
				std::vector< std::vector< cv::Point> > contours;
				std::vector<cv::Vec4i> hierarchy;
				cv::findContours(copy_tmp, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);
				
// 				assert(contours.size() == 1 && "More than one shape in Zone");
				
				//Do for all contour and add percentage
				// Calculate the area of each contour
				int whitepix = 0 ;
				int final_size = 0;
				
				for(size_t i = 0 ; i < contours.size() ; i++){
					auto contour = contours[i];
					
					final_size = final_size + contour.size();
					
					cv::Mat matcon = cv::Mat::zeros(_zone_mat.size(), CV_8U);
					for(auto it = contour.begin() ; it != contour.end() ; ++it ){
						matcon.at<uchar>(it->y, it->x) = 255;
					}
					
					cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size( 3, 3 ), cv::Point( -1, -1 ) );
		/// Apply the specified morphology operation
					cv::morphologyEx( matcon, matcon, cv::MORPH_DILATE, element);
					cv::Mat diff = matcon - copyTest;
					cv::Mat diff2 = copyTest - matcon;
					cv::Mat fin;
					matcon.copyTo(fin);
					fin = fin - diff;
					fin = fin - diff2;
					
					for(int row = 0; row < fin.rows; ++row) {
						uchar* p = fin.ptr(row);
						for(int col = 0; col < fin.cols; ++col) {
							if(p[col] != 0){
								++whitepix;
							}
						}
					}
				}
// 				std::cout << "PRINT FIN" << std::endl;
// 				cv::imshow("FIIIn", fin);
// 				cv::imshow("3", matcon);
// 				cv::imshow("1", diff);
// 				cv::imshow("2", diff2);
// 				cv::waitKey(0);
				auto percent = whitepix * 100 / final_size;
				return percent;
				
			}
			
			
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
			
			void getOrientation(std::vector<cv::Point> &pts){
				//Construct a buffer used by the pca analysis
				auto data_pts = cv::Mat(pts.size(), 2, CV_64FC1);
				for (int i = 0; i < data_pts.rows; ++i)
				{
					data_pts.at<double>(i, 0) = pts[i].x;
					data_pts.at<double>(i, 1) = pts[i].y;
				}
			
				//Perform PCA analysis
				cv::PCA pca_analysis(data_pts, cv::Mat(), CV_PCA_DATA_AS_ROW);
			
				//Store the position of the object
				auto pos = cv::Point(pca_analysis.mean.at<double>(0, 0),
								pca_analysis.mean.at<double>(0, 1));
			
				//Store the eigenvalues and eigenvectors
				std::vector<cv::Point2d> eigen_vecs(2);
				std::vector<double> eigen_val(2);
				for (int i = 0; i < 2; ++i)
				{
					eigen_vecs[i] = cv::Point2d(pca_analysis.eigenvectors.at<double>(i, 0),
											pca_analysis.eigenvectors.at<double>(i, 1));
			
					eigen_val[i] = pca_analysis.eigenvalues.at<double>(0, i);
				}				
				_pca = std::tuple<cv::Point2i, cv::Point2i, cv::Point2i>(pos, pos + 0.02 * cv::Point(eigen_vecs[0].x * eigen_val[0], eigen_vecs[0].y * eigen_val[0]), pos + 0.02 * cv::Point(eigen_vecs[1].x * eigen_val[1], eigen_vecs[1].y * eigen_val[1]));
			
				_pca_orientation = atan2(eigen_vecs[0].y, eigen_vecs[0].x);
				
				
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