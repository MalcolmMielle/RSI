#ifndef RSI_ZONELIGHT_05072016
#define RSI_ZONELIGHT_05072016

#include <iostream>
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <stdexcept>
#include <sstream>

namespace AASS{
	
	namespace RSI{
		
		class ZoneHasMoreThanOneContour : public std::runtime_error {
		public:
			ZoneHasMoreThanOneContour(const std::string& message) 
				: std::runtime_error(message) { };
		};
		class ZoneHasNoContour : public std::runtime_error {
		public:
			ZoneHasNoContour(const std::string& message) 
				: std::runtime_error(message) { };
		};
		
		class ZoneComparedLight;
		class ZoneComparedInterfaceLight;
		
		/**
		 * Zone class with no opencv image to be light in memory. This is to be used in the ZoneExtractor where a lot of zones are created. It used to blow my memory usage out of the zindow otherwise.
		 */
		class ZoneLight{
			
			
			
		private:
			bool _flag_PCA;
			bool _uniqueness_calculated;
			
		protected:
			cv::Size _img_size;
			size_t _value;
			std::deque <cv::Point2i> _zone;
			///@brief sum of all value of x and y of all point in zone. For fast update and get of centroid
			cv::Point2i _sum_of_x_and_y;
			///@brief Zone drawn on the Mat
// 			cv::Mat _zone_mat;
			
			std::tuple<cv::Point, cv::Point, cv::Point> _pca;
			double _pca_orientation;
			
			std::vector< cv::Point> _contours;
			
			bool _isUnique;
			double _uniqueness_score;
			
			
			double _size_classification; //Should be between 0 and 1 to be able to compare to zone size relative to other size in the same graph
			
			//TODO
			double _pca_classification; //Should be between 0 and 1 to be able to compare to zone pcas relative to other pca in the same graph
			
			
		public:
			ZoneLight() : _flag_PCA(false), _uniqueness_calculated(false), _isUnique(true), _uniqueness_score(0), _size_classification(-1), _pca_classification(-1) {};
			ZoneLight(const cv::Size& size) : _img_size(size), _flag_PCA(false), _uniqueness_calculated(false), _isUnique(true), _uniqueness_score(0), _size_classification(-1), _pca_classification(-1){
// 				_zone_mat = cv::Mat::zeros(size, CV_8U);
			};
			ZoneLight(int rows, int cols) : _img_size(rows, cols), _flag_PCA(false), _uniqueness_calculated(false), _isUnique(true), _uniqueness_score(0), _size_classification(-1), _pca_classification(-1){
// 				_zone_mat = cv::Mat::zeros(rows, cols, CV_8U);
			};
			
			void print() const {
				std::cout << "Size classification " <<  _size_classification << " " ;
				std::cout << "pca clasificcation " << _pca_classification << " ";
			}
			
			cv::Point2i getSumOfXAndY() const {return _sum_of_x_and_y;}
			cv::Size getSizeImage() const {return _img_size;}
			
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
				std::cout << "Watwat " << _uniqueness_calculated << std::endl; 
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
			
			void push_back(const cv::Point2i& p){_zone.push_back(p); addPoint(p);}
			void push_front(const cv::Point2i& p){_zone.push_front(p); addPoint(p);}
			
			void pop_back(){removePoint(_zone.size()-1); _zone.pop_back();}
			void pop_front(){removePoint(0); _zone.pop_front();}
			
			bool isEmpty(){return (0 == _zone.size());}
			int size(){return _zone.size();}
// 			void setImageSize(const cv::Mat& in){_zone_mat = cv::Mat::zeros(in.size(), CV_8U);};
			
			size_t size() const {return _zone.size();}
			
			void clear(){
				_zone.clear(); 
				_value = 0; _sum_of_x_and_y.x = 0 ; _sum_of_x_and_y.y = 0; 
// 				_zone_mat = cv::Mat::zeros(_zone_mat.size(), CV_8U);
			}
			
			cv::Point2i& operator[](int i){return _zone[i];};
			const cv::Point2i& operator[](const int i) const {return _zone[i];};
			
			void setValue(size_t i){_value = i;}
			size_t getValue(){return _value;}
			size_t getValue() const {return _value;}
			const std::deque <cv::Point2i >& getZone() const {return _zone;}
		// 	std::deque <cv::Point2i >& getZone(){return _zone;}
// 			const cv::Mat& getZoneMat() const {return _zone_mat;}
			std::tuple<cv::Point, cv::Point, cv::Point> getPCA(){return _pca;}
			const std::tuple<cv::Point, cv::Point, cv::Point>& getPCA() const {return _pca;}
			double getPCAOrientation(){return _pca_orientation;}
			double getPCAOrientation() const {return _pca_orientation;}
			
			///@brief return the center of the zone by doing the mean of all the points
			cv::Point2i getCentroid(){
				if( _zone.size() == 0 ){
					throw std::runtime_error("zone is empty");
				}
				return cv::Point2i(_sum_of_x_and_y.y / _zone.size(), _sum_of_x_and_y.x / _zone.size());	
			}
			///@brief return the center of the zone by doing the mean of all the points
			const cv::Point2i getCentroid() const {
				if( _zone.size() == 0 ){
					throw std::runtime_error("zone is empty");
				}
				return cv::Point2i(_sum_of_x_and_y.y / _zone.size(), _sum_of_x_and_y.x / _zone.size());		
			}
			
			
			void fuse(const ZoneLight& input){
// 				std::cout << "Fuse " << std::endl;
// 				assert(input.getValue() < _value);
				for(size_t i = 0 ; i < input.size() ; ++i){
					this->push_back(input.getZone()[i]);
				}
				
				//Fuse the perimeters
				auto contact = [](cv::Point2i pt, const std::vector<cv::Point2i>& points) -> bool{
					int count = 0;
					auto it = points.begin();
					for( ; it != points.end() ; ++it){
						if(pt.x + 1 >= it->x && pt.x -1 <= it->x){
							if(pt.y + 1 >= it->y && pt.y -  1 <= it->y){
								++count;
							}
						}
					}
					if(count == 3){
						return true;
					}
					return false;
				};
				
				std::vector<cv::Point> new_perim;
				auto inputC = input.getContourNoUpdate();
				for(auto it = _contours.begin() ; it != _contours.end() ; ++it){
// 					Keep all point not in contact
					if( !contact(*it, inputC) ){
						new_perim.push_back(*it);
					}
				}
				for(auto it = inputC.begin() ; it != inputC.end() ; ++it){
// 					Keep all point not in contact
					if( !contact(*it, _contours) ){
						new_perim.push_back(*it);
					}
				}
				
				_contours = new_perim;
				
// 				std::cout << "Fuse DONE " << std::endl;
				
// 				cv::Mat graphmat2 = cv::Mat::zeros(600,600, CV_8U);
// 				draw(graphmat2, cv::Scalar(100));
// 				cv::imshow("fused", graphmat2);
// 				cv::waitKey(0);	
// 				std::cout << "Fused now pca " << std::endl;
				//Do not updated PCA because sometime when doing recursive fusion, we must wait until all zones have been fused in the initial to perform fusion OR use throw mechanism but it's complex for no reason
// 				PCA();
			}
			
			void drawZone(cv::Mat& img, const cv::Scalar& color) const{
				img.convertTo(img, CV_8U);
				uchar colllo = color[0];
				for(int i = 0; i < _zone.size() ; ++i){
					img.at<uchar>(_zone[i].x, _zone[i].y) = colllo;
				}				
			}
			
			///@brief simply draw the map
			void drawContour(cv::Mat& img, const cv::Scalar& color) const{				
				for (int i = 0; i < _contours.size(); ++i)
				{
					img.at<uchar>(_contours[i].x, _contours[i].y) = 255;
				}
			}
			
			///@rbief draw the map and info such as PCA and contour
			void drawPCA(cv::Mat& img, const cv::Scalar& color) const{
				
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
			
			///Update the contour before giving it to always be up to date
			std::vector< cv::Point > getContour(){
				updateContour();
				return _contours;
			}
			
			std::vector< cv::Point > getContourNoUpdate() const {
				return _contours;
			}
			
			///Do not update the contour since it's unmmutable. Need to call update before to be sure.
			std::vector< cv::Point > getContour() const {
// 				updateContour();
				return _contours;
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
				auto area = cv::contourArea(_contours);
				getOrientation(_contours);

				
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
			
			
			//Recursive border function. We check the point, if its a border we add to the border add check all neighbor
			void isBorder (cv::Point2i pt, const std::deque<cv::Point2i>& points, std::vector<cv::Point2i>& border, std::vector<cv::Point2i>& seen)const {
				
				assert(border.size() <= points.size());
// 				std::cout << "In with perim of " << border.size() << std::endl;
				std::vector<cv::Point2i> neigh;
				auto it = points.begin();
				for( ; it != points.end() ; ++it){
					if(pt.x + 1 >= it->x && pt.x -1 <= it->x){
						if(pt.y + 1 >= it->y && pt.y -  1 <= it->y){
							neigh.push_back(*it);
						}
					}
				}
				if(neigh.size()!=9){
					border.push_back(pt);
					auto it = neigh.begin();
					for( ; it != neigh.end() ; ++it){
						auto seen_it = seen.begin();
						bool seen_flag = false;
						for( ; seen_it != seen.end() ; ++seen_it){
							if(*seen_it == *it){
								seen_flag = true;
							}
						}
						if(seen_flag == false){
							seen.push_back(*it);
							isBorder(*it, points, border, seen);
						}
						
					}
				}
			}
			
			///@brief return all points that are part of the perimeter
			std::vector<cv::Point2i> getPerimeter() const {

				assert(_zone.size() > 0);
				std::vector<cv::Point2i> perim;
				std::vector<cv::Point2i> to_explore;
// 				std::cout << "Getting perim " <<_zone.size() << std::endl;
				//Find one 
				for(auto it = _zone.begin() ; it != _zone.end() ; ++it){
					int nb_neighb = 0;
					for(auto it2 = _zone.begin() ; it2 != _zone.end() ; ++it2){
						if(it2->x + 1 >= it->x && it2->x -1 <= it->x){
							if(it2->y + 1 >= it->y && it2->y - 1 <= it->y){
// 								std::cout << it2->x << " " << it2->y << " and " << it->x << " " << it->y <<std::endl;
								nb_neighb++;
							}
						}
					}
// 					std::cout << "Nd_neighb " << nb_neighb << std::endl;
					assert (nb_neighb <= 9);
					assert (nb_neighb > 0);
					if(nb_neighb != 9){
						to_explore.push_back(*it);
						it = _zone.end() - 1 ;
					}
				}
				
				assert(to_explore.size() >= 1);
// 				std::cout << "start the recursion" << std::endl;
				isBorder(to_explore[0], _zone, perim, to_explore);
				
				if(perim.size() == 0){
					std::cout << "WEHAT " << std::endl;
					for(auto it = _zone.begin() ; it != _zone.end() ; ++it){
						std::cout << *it << std::endl;
					}
				}
// 				std::cout << "end fg perim" << std::endl;
				return perim;
			}
			
			///@brief Return the number of contact points in percent compared to size of contour. NEED AN updated contour beforehand !!!!.
			int contactPoint(ZoneLight& zone, bool updateContour = true){
				int whitepix = 0 ;
				int final_size = 0;
				int test_size = 0 ;

// 				std::cout << "contact" << std::endl;
				
				auto contact = [](cv::Point2i pt, const std::vector<cv::Point2i>& points) -> bool{
					
					auto it = points.begin();
					for( ; it != points.end() ; ++it){
						if(pt.x + 1 >= it->x && pt.x -1 <= it->x){
							if(pt.y + 1 >= it->y && pt.y -  1 <= it->y){
								return true;
							}
						}
					}
					return false;
				};
				
				if(updateContour==true){
					std::cout << "Update countour" << std::endl;
					this->updateContour();
					std::cout << "Update countour2" << std::endl;
					zone.updateContour();
				}
				for(auto it = _contours.begin() ; it != _contours.end() ; ++it){
					
// 					std::cout << "getval" << std::endl;
					if( contact(*it, zone.getContourNoUpdate()) ){
						whitepix++;
					}
					
				}
// 				std::cout << "Out" << std::endl;
				final_size = _contours.size();
				
				auto percent = whitepix * 100 / final_size;
// 				std::cout << "Percent " << percent << std::endl;
// 				assert(percent < 50 && "Percentage can't be more than 50% because that means the shape is flat");
				return percent;
				
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
			double comparePCA(const ZoneLight& zone_in) const {
				
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
			
			ZoneComparedInterfaceLight compare(const AASS::RSI::ZoneLight& zone_in) const;
			
			void updateContour();
			
		private: 
			void addPoint(const cv::Point2i& p){
				_sum_of_x_and_y.x = _sum_of_x_and_y.x + p.x;
				_sum_of_x_and_y.y = _sum_of_x_and_y.y + p.y;
				//TODO drawing function of new point
// 				_zone_mat.at<uchar>(p.x, p.y) = 255;
			}
			void removePoint(int i){
				_sum_of_x_and_y.x = _sum_of_x_and_y.x - _zone[i].x;
				_sum_of_x_and_y.y = _sum_of_x_and_y.y - _zone[i].y;
				//TODO un-drawing function of new point
// 				_zone_mat.at<uchar>(_zone[i].x, _zone[i].y) = 0;
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
				
// 				std::cout << "ytuple " << pos << " " << pos + 0.02 * cv::Point(eigen_vecs[0].x * eigen_val[0], eigen_vecs[0].y * eigen_val[0]) <<  " " << pos + 0.02 * cv::Point(eigen_vecs[1].x * eigen_val[1], eigen_vecs[1].y * eigen_val[1]) << std::endl;
				
				_pca = std::tuple<cv::Point2i, cv::Point2i, cv::Point2i>(pos, pos + 0.02 * cv::Point(eigen_vecs[0].x * eigen_val[0], eigen_vecs[0].y * eigen_val[0]), pos + 0.02 * cv::Point(eigen_vecs[1].x * eigen_val[1], eigen_vecs[1].y * eigen_val[1]));
			
				_pca_orientation = atan2(eigen_vecs[0].y, eigen_vecs[0].x);
				
				
			}
			
			
			

			
		};
		
		
		inline std::ostream& operator<<(std::ostream& in, const ZoneLight &p){
			
			in << "v " << p.getValue() << " s " << p.size();

			return in;
			
		}
		
		inline std::istream& operator>>(std::istream& in, ZoneLight &p){
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
