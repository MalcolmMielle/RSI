#ifndef RSI_ZONE_17052016
#define RSI_ZONE_17052016

#include <iostream>
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <stdexcept>
#include <sstream>

#include "maoris/Zone.hpp"

namespace AASS{
	
	namespace RSI{
		

// 		class ZoneCompared;
		class ZoneComparedInterface;
		
		/**
		 * bool _isUnique: true if the zone is unique, false otherwise. It is initialised as true, so that every new zone is considered ofr comparison unless setUniqueness as been explicitely called before.
		 * 
		 * bool _uniqueness_calculated: flag to check if the zone was initialised or not.
		 */
		class ZoneRI : public maoris::Zone{
			
		private:
			bool _flag_PCA;
			bool _uniqueness_calculated;
			
		protected:
			
			std::tuple<cv::Point, cv::Point, cv::Point> _pca;
			double _pca_orientation;
						
			bool _isUnique;
			double _uniqueness_score;
			
			double _size_classification; //Should be between 0 and 1 to be able to compare to zone size relative to other size in the same graph
			
			//TODO
			double _pca_classification; //Should be between 0 and 1 to be able to compare to zone pcas relative to other pca in the same graph
			
			
		public:
			ZoneRI() : _flag_PCA(false), _uniqueness_calculated(false), _isUnique(true), _uniqueness_score(0), _size_classification(-1), _pca_classification(-1) {};
			ZoneRI(const cv::Size& size) : _flag_PCA(false), _uniqueness_calculated(false), _isUnique(true), _uniqueness_score(0), _size_classification(-1), _pca_classification(-1), maoris::Zone(size){
			};
			ZoneRI(int rows, int cols) : _flag_PCA(false), _uniqueness_calculated(false), _isUnique(true), _uniqueness_score(0), _size_classification(-1), _pca_classification(-1), maoris::Zone(rows, cols){
			};
			
			ZoneRI(const ZoneRI& zone_i){
				_flag_PCA = zone_i._flag_PCA;
				_uniqueness_calculated= zone_i._uniqueness_calculated;
				_isUnique = zone_i._isUnique;
				_uniqueness_score = zone_i._uniqueness_score;
				_size_classification = zone_i._size_classification;
				_pca_classification = zone_i._pca_classification;
				_use_cvMat = zone_i.useCvMat();
				_value = zone_i.getValue();
				
				//Change it for a set for constant time find !
// 				_zone = z.getZone();
				for(auto it = zone_i.getZone().begin() ; it != zone_i.getZone().end() ; ++it){
					_zone.push_back(*it);
				}
				///@brief sum of all value of x and y of all point in zone. For fast update and get of centroid
				_sum_of_x_and_y = zone_i.getSumXY();
				///@brief Zone drawn on the Mat
				zone_i.getZoneMat().copyTo(_zone_mat);
				_img_size = zone_i.getSizeImg();
							
				_contours = zone_i.getContour();
			}
			
			ZoneRI(const maoris::Zone& z) : _flag_PCA(false), _uniqueness_calculated(false), _isUnique(true), _uniqueness_score(0), _size_classification(-1), _pca_classification(-1) {
				//TODO
				_use_cvMat = z.useCvMat();
				_value = z.getValue();
				
				//Change it for a set for constant time find !
// 				_zone = z.getZone();
				for(auto it = z.getZone().begin() ; it != z.getZone().end() ; ++it){
					_zone.push_back(*it);
				}
				///@brief sum of all value of x and y of all point in zone. For fast update and get of centroid
				_sum_of_x_and_y = z.getSumXY();
				///@brief Zone drawn on the Mat
				z.getZoneMat().copyTo(_zone_mat);
				_img_size = z.getSizeImg();
							
				_contours = z.getContour();
			}
			
			void print() const {
				std::cout << "Size classification " <<  _size_classification << " " ;
				std::cout << "pca clasificcation " << _pca_classification << " ";
			}
			
						
			void setSizeClassification(double size){_size_classification = size;}
			double getSizeClassification() const {if(_size_classification == -1 ){throw std::runtime_error("Classificatio size not set");} return _size_classification;}
			
			void setPCAClassification(double size){_pca_classification = size;}
			double getPCAClassification() const {if(_pca_classification == -1 ){throw std::runtime_error("Classificatio pca not set");}return _pca_classification;}
			
			void setUniqueness(bool b, double u_score){
				_uniqueness_calculated = true;
				_isUnique = b;
				_uniqueness_score = u_score;
				assert(_isUnique == b);
			}
			
			bool isUnique() const {	
				std::cout << "Watwat uniqueness " << _uniqueness_calculated << std::endl; 
// 				if(_uniqueness_calculated == false){
// // 					std::cout << "FUCK" << std::endl;
// // 					throw std::runtime_error("you forgot to set the uniqueness first");
// // 					std::cout << "FUCK2" << std::endl;
// 				}
// 				else{
// 					std::cout << "Supposed it's good" << std::endl;
// 				}
				return _isUnique;
				
			}
			
			bool uniquenessInit(){return _uniqueness_calculated;}
			double getUniquenessScore() const {if(_uniqueness_calculated == false){throw std::runtime_error("score uniqueness asked but zone uniqueness was not init");} return _uniqueness_score;}
			

			
			void clear(){
				maoris::Zone::clear();
			}
			
			std::tuple<cv::Point, cv::Point, cv::Point> getPCA(){return _pca;}
			const std::tuple<cv::Point, cv::Point, cv::Point>& getPCA() const {return _pca;}
			double getPCAOrientation(){return _pca_orientation;}
			double getPCAOrientation() const {return _pca_orientation;}
			
			void fuse(const Zone& input){
// 				assert(input.getValue() < _value);
				for(size_t i = 0 ; i < input.size() ; ++i){
					this->push_back(input.getZone()[i]);
				}
				if(_use_cvMat == true){
					try{
						updateContour();
					}
					catch(const std::runtime_error& e){
						std::cout << "Warning: " << e.what() << std::endl;
					}
				}
			}
			
			///@rbief draw the map and info such as PCA and contour
			void drawPCA(cv::Mat& img, const cv::Scalar& color) const{
				
				std::cout << "DRAW PCA" << std::endl;
				//Draw PCA
// 				cv::circle(img, std::get<0>(_pca), 3, CV_RGB(255, 0, 255), 10);
				cv::line(img, std::get<0>(_pca), std::get<1>(_pca) , cv::Scalar(255), 2);
				cv::line(img, std::get<0>(_pca), std::get<2>(_pca) , cv::Scalar(155), 2);	
// 				
// 				std::cout << "One line " << std::get<0>(_pca) << " " << std::get<1>(_pca) << std::endl;
// 				
				
			}
			
			void printLabel(cv::Mat& img) const{
				std::string text;
				text = std::to_string(_value);
				std::string textsi;
				textsi = std::to_string(size());
				std::stringstream precisionValue;
				precisionValue.precision(2);
				precisionValue << _size_classification << std::endl;
				std::stringstream precisionValue2;
				precisionValue2.precision(2);
				precisionValue2 << _pca_classification << std::endl;
// 				text = text + " sd " + precisionValue.str() + " pd " + precisionValue2.str() ;
// 				text = text + ":s:" + textsi;
				cv::putText(img, text, getCentroid(), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255));
			}
			
			///@brief Calculate the principal components of a zone.
			void PCA(){
				
				_flag_PCA = true;
				
// 				std::cout << "PCA" << std::endl;
// 				find countour
				updateContour();
				
// 				cv::Mat copy_tmp;
// 				_zone_mat.copyTo(copy_tmp);
// 				cv::Mat copy =  cv::Scalar::all(255) - _zone_mat;
// 				
// 				
// 				std::vector< std::vector< cv::Point> > contours;
// 				std::vector<cv::Vec4i> hierarchy;
// 				cv::findContours(copy_tmp, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
// 				
// // 				assert(contours.size() >= 1 && "More than one shape in Zone");
// 				
// 				// Calculate the area of each contour
// 				
// 				//Use a lambda function to sort the contours
// 				std::sort(contours.begin(), contours.end(), [](std::vector<cv::Point> &pts, std::vector<cv::Point> &pts2){return pts.size() > pts2.size(); } );
// 				
// 				if(contours.size() < 1)
// 				{
// 					cv::Mat graphmat2 = cv::Mat::zeros(600,600, CV_8U);
// 					draw(graphmat2, cv::Scalar(255));
// // 					std::cout << contours[0].size() << " " << contours[1].size() << std::endl;
// // 					if(contours[0].size() < contours[1].size()){
// // 						cv::Mat graphmat2 = cv::Mat::zeros(600,600, CV_8U);
// // 						for (int i = 0; i < contours[0].size(); ++i)
// // 						{
// // 							graphmat2.at<uchar>(contours[0][i].y, contours[0][i].x) = 255;
// // 						}
// 						cv::imshow("fina", graphmat2);
// // 						cv::waitKey(0);	
// // 						throw std::runtime_error("Calculating PCA with zone not in order");
// // 					}
// 					cv::waitKey(0);
// 					
// 					throw std::runtime_error("MORE THAN ONE CONTOUR !");
// 				}
// 				if(contours.size() == 0 ){
// 					throw std::runtime_error("NO CONTOUR IN ZONE !");
// 				}
// 				std::cout << "Contour size " << contours.size() << std::endl;
// 				_contours = contours[0];
				
				auto contour = _contours[0];
				for(auto it = _contours.begin() ; it != _contours.end() ; ++it){
					if(it-> size() > contour.size()){
						contour = *it;
					}
				}
				
				auto area = cv::contourArea(contour);
				getOrientation(contour);

				
// 				std::cout << "PCAAAAA" << std::endl;
// 				std::cout << "One line " << std::get<0>(_pca) << " " << std::get<1>(_pca) << std::endl;
// 				std::cout << "One line " << std::get<0>(_pca) << " " << std::get<2>(_pca) << std::endl;
// 				std::cout << "Next PCA " << std::endl << std::endl;
// 				cv::imshow("Bobafter", _zone_mat);
// 				cv::imshow("Bob", copy_tmp);
// 				cv::waitKey(0);

			}
			
			void printPCA() const{
				std::cout << "PCAAAAA" << std::endl;
				std::cout << "One line " << std::get<0>(_pca) << " " << std::get<1>(_pca) << std::endl;
				std::cout << "One line " << std::get<0>(_pca) << " " << std::get<2>(_pca) << std::endl;
				std::cout << "Next PCA " << std::endl << std::endl;
			}
			
			///@brief return the length values of the two PC. Must be called after calling PCA at least once !
			std::pair<double, double> getMaxMinPCA() const {
				
				if(_flag_PCA == false){
					throw std::runtime_error("You forgot to call PCA before calling getMaxMin");
				}
				
				double length1_x_own = (std::get<1>(_pca).x - std::get<0>(_pca).x);
				length1_x_own = length1_x_own * length1_x_own;
				double length1_y_own = (std::get<1>(_pca).y - std::get<0>(_pca).y);
				length1_y_own = length1_y_own * length1_y_own;
				double length1_oen = length1_x_own + length1_y_own;
				
				double length2_x_oen = (std::get<2>(_pca).x - std::get<0>(_pca).x);
				length2_x_oen = length2_x_oen * length2_x_oen;
				double length2_y_oen = (std::get<2>(_pca).y - std::get<0>(_pca).y);
				length2_y_oen = length2_y_oen * length2_y_oen;
				double length2_oen = length2_x_oen + length2_y_oen;
				
				double max_own = length1_oen, min_own = length2_oen;
				if(max_own < min_own){
					max_own = length2_oen;
					min_own = length1_oen;
				}
			 	max_own = std::sqrt(max_own);
				min_own = std::sqrt(min_own);
				
				return std::pair<double, double>(max_own, min_own);
			}
			
			/**
			 * @brief return the difference of the pca between 0 and 1.
			 * 
			 * Normalize between zero and 1 by dividing by the max of the two PCA values and doing max - min
			 */
			double getPCADiff() const {
				auto pca_max_min = getMaxMinPCA();
				std::cout << "pca max min " << pca_max_min.first << " " << pca_max_min.second << std::endl;

				
				if(pca_max_min.first > 0){
					double normalizer_own = 1 / pca_max_min.first;
					pca_max_min.second = pca_max_min.second * normalizer_own;
					pca_max_min.first = pca_max_min.first * normalizer_own;
				}
				
				double pca_diff = pca_max_min.first - pca_max_min.second;
				assert(pca_diff >= 0 && "similarity measure of Zone should not be negative");
				assert(pca_diff <= 1 && "similarity measure of Zone should not be mopre than one");
				
				return pca_diff;
				
			}
			
			
			/**
			 * @brief Compare two zones by using the PCA values. the lowest the score the better the matching
			 * 
			 * It normalize the PCA values using the max of each set of values and compare the difference between the max and min of each set.
			 */
			double comparePCA(const ZoneRI& zone_in) const {
				
// 				auto pca_max_min = getMaxMinPCA();
// 				auto pca_max_min_input = zone_in.getMaxMinPCA();
// 				
// // 				std::cout << "max mins "<< pca_max_min.first << " " << pca_max_min.second << " " << pca_max_min_input.first << " " << pca_max_min_input.second  << std::endl;
// 				
// 				if(pca_max_min.first > 0){
// 					double normalizer_own = 1 / pca_max_min.first;
// 					pca_max_min.second = pca_max_min.second * normalizer_own;
// 					pca_max_min.first = pca_max_min.first * normalizer_own;
// 				}
// 				if(pca_max_min_input.first > 0){
// 					double normalizer_input = 1 / pca_max_min_input.first;
// 					pca_max_min_input.second = pca_max_min_input.second * normalizer_input;
// 					pca_max_min_input.first = pca_max_min_input.first * normalizer_input;
// 				}
				
// 				std::cout << " normilzed max mins "<< pca_max_min.first << " " << pca_max_min.second << " " << pca_max_min_input.first << " " << pca_max_min_input.second  << std::endl;
				
				double diff = getPCADiff();
				double diff_input = zone_in.getPCADiff();
				
				double similarity = diff - diff_input;
				similarity = std::abs(similarity);
				
				assert(similarity >= 0 && "similarity measure of Zone should not be negative");
				assert(similarity <= 1 && "similarity measure of Zone should not be mopre than one");
				
				return similarity;
				
			}
			
			
			/** @brief comparison this is where I choose to use PCA or not. The lowest the score, the better the matching. Result is between 0 and 1
			 */
			
			ZoneComparedInterface compare(const AASS::RSI::ZoneRI& zone_in) const;
						
		private: 
			
			void getOrientation(std::vector<cv::Point> &pts){
				std::cout << "getOrinetation" << std::endl;
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
				
// 				std::cout << "ytuple " << pos << std::endl;
			
				//Store the eigenvalues and eigenvectors
				std::vector<cv::Point2d> eigen_vecs(2);
				std::vector<double> eigen_val(2);
				
				if(pca_analysis.eigenvalues.at<double>(0, 0) == 0){
					throw std::runtime_error("No eigen value");
				}
				
				for (int i = 0; i < 2; ++i)
				{
// 					std::cout << "Prints " << pca_analysis.eigenvectors.at<double>(i, 0);
// 					std::cout << " " << pca_analysis.eigenvectors.at<double>(i, 1);
// 					std::cout << " " << pca_analysis.eigenvalues.at<double>(0, i) << std::endl;
					eigen_vecs[i] = cv::Point2d(pca_analysis.eigenvectors.at<double>(i, 0),
											pca_analysis.eigenvectors.at<double>(i, 1));
			
					eigen_val[i] = pca_analysis.eigenvalues.at<double>(0, i);
					
// 					std::cout << "DONE" << std::endl;
				}
				
				std::cout << "ytuple " << pos << " " << pos + 0.02 * cv::Point(eigen_vecs[0].x * eigen_val[0], eigen_vecs[0].y * eigen_val[0]) <<  " " << pos + 0.02 * cv::Point(eigen_vecs[1].x * eigen_val[1], eigen_vecs[1].y * eigen_val[1]) << std::endl;
				
				_pca = std::tuple<cv::Point2i, cv::Point2i, cv::Point2i>(pos, pos + 0.02 * cv::Point(eigen_vecs[0].x * eigen_val[0], eigen_vecs[0].y * eigen_val[0]), pos + 0.02 * cv::Point(eigen_vecs[1].x * eigen_val[1], eigen_vecs[1].y * eigen_val[1]));
			
				_pca_orientation = atan2(eigen_vecs[0].y, eigen_vecs[0].x);
				
				
			}
			
		};
		
		
		inline std::ostream& operator<<(std::ostream& in, const ZoneRI &p){
			
			in << "v " << p.getValue() << " s " << p.size();

			return in;
			
		}
		
		inline std::istream& operator>>(std::istream& in, ZoneRI &p){
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
