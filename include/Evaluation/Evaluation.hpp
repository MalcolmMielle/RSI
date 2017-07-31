#ifndef RSI_EVALUATION_31072017
#define RSI_EVALUATION_31072017

#include <iostream>
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <stdexcept>
#include <sstream>
#include <fstream>

#include <boost/filesystem.hpp>
#include <boost/iterator/filter_iterator.hpp>
#include <boost/filesystem/path.hpp>

namespace AASS{
	
	namespace RSI{
		
		typedef std::map <std::vector<int>, std::vector <cv::Point> > match2points;
		typedef std::map <int, std::vector <cv::Point> > tag2points;
		typedef std::map <int, tag2points> tag2tagMapper;
		
		struct results{
			double time;
			double precision;
			double recall;
			double inverse_recall;
		};
		
		inline bool exists_test3 (const std::string& name) {
			struct stat buffer;   
			return (stat (name.c_str(), &buffer) == 0); 
		}

		inline void exportResultsGnuplot(const std::string& file_out, const std::string& file_studied, const AASS::RSI::results& Regions, double max, float proper_size){
			boost::filesystem::path p(file_studied);
			std::string name = p.filename().stem().string();
			
			std::string result_file = file_out;
			std::ofstream myfile;
			if(!exists_test3(result_file)){
				myfile.open (result_file);
				myfile << "precision recall inverse_recall time labels size\n";
			}
			else{
				myfile.open (result_file, std::ios::out | std::ios::app);
			}
			
			if (myfile.is_open())
			{
				myfile << name << " " << Regions.precision << " " << Regions.recall << " " << Regions.inverse_recall << " " << Regions.time << " " << max << " " << proper_size << "\n";
				myfile.close();
			}
			else std::cout << "Unable to open file";
		}
		
		
		inline bool checkAllValue(const cv::Mat& mat_in, const std::vector<int>& all_vals){
	
			std::cout << mat_in.size().width << " " << mat_in.size().height << std::endl;
			
			for(int x=0; x < mat_in.size().width; x++){
				for(int y=0; y < mat_in.size().height; y++){
					cv::Point current_pixel(x,y);
										
					int tag_GT   = (int) mat_in.at<uchar>(current_pixel);
					bool flag = false;
					for (auto it = all_vals.begin(); it != all_vals.end() ; ++it) {
						if(*it == tag_GT){
		// 					std::cout << "wel " << *it << " " << (int) tag_GT << std::endl;
							flag = true;
						}
					}
					if(flag == false){
						std::cout << tag_GT << " Doesn't exist" << std::endl << "List : ";
						for (auto it = all_vals.begin(); it != all_vals.end() ; ++it) {
							std::cout << *it << " " ;
						}
						std::cout << std::endl;
						return false;
					}
					
				}
			}
			return true;
		}

		
		inline cv::Mat segment_Ground_Truth(cv::Mat GroundTruth_BW){
			cv::Mat src = GroundTruth_BW.clone();
			cv::Mat drawing = cv::Mat::zeros(src.rows, src.cols, CV_8UC1);

			src = src > 250;
			
			cv::erode(src, src, cv::Mat(), cv::Point(-1,-1), 1, cv::BORDER_CONSTANT, cv::morphologyDefaultBorderValue() );			// erode ground truth obstacle
			
			std::vector<std::vector<cv::Point> > contours;
			std::vector<cv::Vec4i> hierarchy;
			
			cv::findContours( src, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );

			
			// iterate through all the top-level contours,
			// draw each connected component with its own random color
			int idx = 0;
			//DO NOT CHANGE THAT. WRONG THE COMPARISON LATER OTHERWISE :/
			int color= 249 / contours.size();
			int count = 1;
			std::vector<int> all_vals;
			all_vals.push_back(0);
			assert(checkAllValue(drawing, all_vals) == true);
			for( ; idx >= 0; idx = hierarchy[idx][0] )
			{
				std::cout << "Contour" << std::endl;
				cv::drawContours( drawing, contours, idx, count , CV_FILLED, 8, hierarchy );
		// 		std::cout << drawing << std::endl;
				all_vals.push_back(count);
		// 		cv::imshow("Contours", drawing);
		// 		cv::waitKey(0);
				assert(checkAllValue(drawing, all_vals) == true && "After draw con");
				count++;
				std::cout << "Values " << color*count << std::endl;
				
			}
			assert(checkAllValue(drawing, all_vals) == true);
			cv::dilate(drawing, drawing, cv::Mat(), cv::Point(-1,-1), 1, cv::BORDER_CONSTANT, cv::morphologyDefaultBorderValue() );	
			assert(checkAllValue(drawing, all_vals) == true);		// erode dilate drawing
			return drawing;
		}
		
		
		
		inline void compare_images(cv::Mat GT_segmentation_in, cv::Mat DuDe_segmentation_in, std::vector<float>& Precisions, std::vector<float>& Recalls, std::vector<float>& inverse_recalls){
					
			std::map<int,int> segmented2GT_tags;
					
			cv::Mat GT_segmentation   = cv::Mat::zeros(GT_segmentation_in.size(),CV_8UC1);
			cv::Mat DuDe_segmentation = cv::Mat::zeros(GT_segmentation_in.size(),CV_8UC1);
			
			
			GT_segmentation_in  .convertTo(GT_segmentation, CV_8UC1);
			DuDe_segmentation_in.convertTo(DuDe_segmentation, CV_8UC1);			
			tag2tagMapper gt_tag2mapper,DuDe_tag2mapper;
			
			std::map<int, std::vector<cv::Point> > dude_points;
			std::map<int, std::vector<cv::Point> > gt_points;
			
			
		// 	cv::imshow("GTTT raw", GT_segmentation);
		// 	cv::Mat img_hist_equalizedgt;
		// 	cv::equalizeHist(GT_segmentation, img_hist_equalizedgt);
		// 	cv::imshow("GT equal", img_hist_equalizedgt);
		// 	cv::imshow("DUDEEE raw", DuDe_segmentation);
		// 	cv::waitKey(0);
		// 	
			
			int nb_of_pixels = 0;
			
			for(int x=0; x < GT_segmentation.size().width; x++){
				for(int y=0; y < GT_segmentation.size().height; y++){
					cv::Point current_pixel(x,y);
										
					int tag_GT   = GT_segmentation.at<uchar>(current_pixel);
					int tag_DuDe  = DuDe_segmentation.at<uchar>(current_pixel);
					
		// 			std::cout << "Tags gt   " << tag_GT << std::endl;
		// 			std::cout << "Tags dude " << tag_DuDe << std::endl;
					
					if(tag_DuDe>0 && tag_GT>0 ){
		// 				std::cout << "Add" << std::endl;
						gt_tag2mapper  [tag_GT][tag_DuDe].push_back(current_pixel);
						DuDe_tag2mapper[tag_DuDe][tag_GT].push_back(current_pixel);
						
						dude_points[tag_DuDe].push_back(current_pixel);
						gt_points[tag_GT].push_back(current_pixel);
						
						++nb_of_pixels;
					}
				}
			}
			
			std::vector<float> recalls_inside;			
			double cum_precision=0, cum_total=0, cum_recall=0, cum_inverse_recall=0;

			std::cout << "Regions in GT or recall: "<< std::endl;
					
			
			for( tag2tagMapper::iterator it = gt_tag2mapper.begin(); it!= gt_tag2mapper.end(); it++ ){
				cv::Mat DuDe_segmentation_draw = cv::Mat::zeros(GT_segmentation.size(),CV_8UC1);
				
		// 		std::cout << "Tag gt " << it->first << std::endl;
		//				std::cout << "   " << it->first << " connected to "<< std::endl;
				tag2points inside = it->second;
				int max_intersection=0; 
						
				int gt_tag_max = -1;
		// 		std::cout << "Tag seg ";
				for( tag2points::iterator it2 = inside.begin(); it2!= inside.end(); it2++ ){
		// 			std::cout << it2->first << " ";
					for(auto it3 = it2->second.begin() ;  it3 != it2->second.end() ; ++it3){
						DuDe_segmentation_draw.at<uchar>(*it3) = 255;
					}
		// 			cv::imshow("mat", DuDe_segmentation_draw);
		// 			cv::waitKey(0);
		// 			total_points += it2->second.size();
					if (it2->second.size() > max_intersection){
						max_intersection = it2->second.size();
						gt_tag_max = it2->first;
					}
		//					std::cout << "      " << it2->first << " with "<< it2->second.size() <<" points" << std::endl;					
				}
		// 		std::cout << std::endl;
				
		// 		cv::imshow("mat final", DuDe_segmentation_draw);
		// 		cv::waitKey(0);		
				int total_points = gt_points[it->first].size();
				
				std::cout << "SIZE " << total_points << " it first " << it->first << std::endl;
				
				
				segmented2GT_tags[gt_tag_max] = it->first;
						std::cout << "   max is " << max_intersection << " that represents " << 100*max_intersection/total_points   << std::endl;
				Recalls.push_back(100*max_intersection/total_points);
				cum_recall += max_intersection;
				cum_total += total_points;
			}	
		// 	pixel_recall = cum_recall/cum_total;
					
			std::vector<float> precisions_inside;	
			cum_total=0;
					std::cout << "Regions in DuDe: "<< std::endl;
			for( tag2tagMapper::iterator it = DuDe_tag2mapper.begin(); it!= DuDe_tag2mapper.end(); it++ ){
				cv::Mat DuDe_segmentation_draw = cv::Mat::zeros(GT_segmentation.size(),CV_8UC1);
		//				std::cout << "   " << it->first << " connected to "<< std::endl;
				tag2points inside = it->second;
				int max_intersection=0; 
				
				
				
				
				for( tag2points::iterator it2 = inside.begin(); it2!= inside.end(); it2++ ){
					
		// 			for(auto it3 = it2->second.begin() ;  it3 != it2->second.end() ; ++it3){
		// 				DuDe_segmentation_draw.at<uchar>(*it3) = 255;
		// 			}
					
		// 			total_points += it2->second.size();
					if (it2->second.size() > max_intersection) max_intersection = it2->second.size();
		//					std::cout << "      " << it2->first << " with "<< it2->second.size() <<" points" << std::endl;					
				}
		//				std::cout << "   max is " << max_intersection << " that represents " << 100*max_intersection/total_points   << std::endl;

				int total_points = dude_points[it->first].size();

				Precisions.push_back(100*max_intersection/total_points);
				cum_precision += max_intersection;
				cum_total += total_points;
		// 		cv::imshow("mat recall", DuDe_segmentation_draw);
		// 		cv::waitKey(0);
			}			
		// 	pixel_precision=cum_precision/cum_total;
			
			
			
			std::vector<float> inverse_recall_inside;	
			cum_total=0;
			
			std::cout << "Regions in GT or inverse recall: "<< std::endl;
			
			for( tag2tagMapper::iterator it = DuDe_tag2mapper.begin(); it!= DuDe_tag2mapper.end(); it++ ){
				cv::Mat DuDe_segmentation_draw = cv::Mat::zeros(GT_segmentation.size(),CV_8UC1);
		//				std::cout << "   " << it->first << " connected to "<< std::endl;
				tag2points inside = it->second;
				auto gt_equivalent_seg = it->second.begin();
				int max_intersection=0;
				
				for( tag2points::iterator it2 = inside.begin(); it2!= inside.end(); it2++ ){
					

		// 			total_points += it2->second.size();
					if (it2->second.size() > max_intersection){
						max_intersection = it2->second.size();
						gt_equivalent_seg = it2;
					}
		//					std::cout << "      " << it2->first << " with "<< it2->second.size() <<" points" << std::endl;					
				}
		//				std::cout << "   max is " << max_intersection << " that represents " << 100*max_intersection/total_points   << std::endl;

				int total_points = dude_points[it->first].size();
				int total_gt_point = gt_points[gt_equivalent_seg->first].size();
				double fp = total_points - max_intersection;
				double tn = nb_of_pixels - (fp - total_gt_point);
				double inv_rec = 100* ( fp/(fp + tn) );
				
// 				std::cout << " fp " << fp << " tn " << tn << " total_points " << total_points << " max_intersection " << max_intersection << " inverse recall " << inv_rec << std::endl;
				

				inverse_recalls.push_back( inv_rec );
				assert(fp/(fp + tn) <= 1);
				assert(fp/(fp + tn) >=0);
				cum_inverse_recall += max_intersection;
				cum_total += total_points;
				
// 				std::cout << "Up" << std::endl;
		// 		cv::imshow("mat recall", DuDe_segmentation_draw);
		// 		cv::waitKey(0);
			}			
			
// 			std::cout << "DONE " << std::endl;
		// 	pixel_inverse_recall=cum_inverse_recall/cum_total;
		}
		
		inline void extract_results(results& Regions, std::vector<float>& Precisions, std::vector<float>& Recalls, std::vector<float>& inverse_recall){
		// 	pixel.precision = 100*pixel_precision;
		// 	pixel.recall    = 100*pixel_recall;
			
			std::cout << "Extract res" << std::endl;
			
			float cum_precision=0;
			float cum_recall=0;
			float cum_inverse_recall=0;
			int size_precision=0, size_recall=0, size_inverse_recall = 0;

			for(int j=0; j < Precisions.size();j++){
				cum_precision += Precisions[j];
				size_precision++;
			}
			std::cout << "Done1" << std::endl;
			for(int j=0; j < Recalls.size();j++){
				cum_recall    += Recalls[j];
				size_recall++;
			}
			std::cout << "Done2" << std::endl;
			for(int j=0; j < inverse_recall.size();j++){
				cum_inverse_recall    += inverse_recall[j];
				size_inverse_recall++;
			}
			std::cout << "Done3" << std::endl;
			Regions.precision = cum_precision/Precisions.size();
			Regions.recall    = cum_recall/Recalls.size();
			Regions.inverse_recall    = cum_inverse_recall/inverse_recall.size();

			std::cout << "Done" << std::endl;

		//			Precisions.clear();
		//			Recalls.clear();
		}
		
		template<typename T>
		inline T mean(std::vector<T>& in){
			T sum = 0 ;
			for(auto it = in.begin() ; it != in.end() ; ++it){
				sum = sum + *it;
			}
			sum = sum / in.size();
			return sum;
		}
			
		template<typename T>
		inline T variance(std::vector<T>& in, T mean){
			T vari = 0 ;
			for(auto it = in.begin() ; it != in.end() ; ++it){
				T temp_el = *it - mean;
				temp_el = temp_el * temp_el;
				vari = vari + temp_el;
			}
			vari = vari / (in.size() - 1);
			return vari;
		}
		
		template<typename T>
		inline T sd(T variance){
			return std::sqrt(variance);
		}
		
		
		class Evaluation{
		protected:
			std::vector<double> _precision;
			std::vector<double> _recall;
			std::vector<double> _inverse_recall;
			std::vector<double> _time;
			std::vector<std::string> _name;
			std::vector<float> _proper_size;
			std::vector<double> _labels;
			
			double _mean_p;
			double _mean_r;
			double _mean_ir;
			double _sd_p;
			double _sd_r;
			double _sd_ir;
			
		public:
			Evaluation(){}
			
			void addPrecision(double p){_precision.push_back(p);}
			void addRecall(double p){_recall.push_back(p);}
			void addInverseRecall(double p){_inverse_recall.push_back(p);}
			void addTime(double p){_time.push_back(p);}
			void addName(std::string& m){_name.push_back(m);}
			void clear(){_precision.clear(); _recall.clear(); _inverse_recall.clear();}
			
			void calculate(){
				_mean_p = mean<double>(_precision);
				_mean_r = mean<double>(_recall);
				_mean_ir = mean<double>(_inverse_recall);
				
				_sd_p = sd<double>(variance<double>(_precision, _mean_p));
				_sd_r = sd<double>(variance<double>(_recall, _mean_r));
				_sd_ir = sd<double>(variance<double>(_inverse_recall, _mean_ir));
			}
			
			size_t size(){return _precision.size();}
			
			
			void compare(const cv::Mat& seg, const cv::Mat& GT_segmentation, double time, const std::string& file){
				assert(seg.rows == GT_segmentation.rows);
				assert(seg.cols == GT_segmentation.cols);
							
				std::vector<float> Precisions, Recalls, inverse_recall;
				AASS::RSI::compare_images(GT_segmentation, seg, Precisions, Recalls, inverse_recall);
				
				std::cout << "Compared" << std::endl;
				
			// 	std::cout << "pixel prec " << pixel_precision << std::endl;
				
				AASS::RSI::results Regions;
				Regions.time = time;
				std::cout << "Go for extract" << std::endl;
				
				extract_results(Regions, Precisions, Recalls, inverse_recall);
					
				double min, max;
				cv::minMaxLoc(GT_segmentation,&min,&max);
				
				float rows = GT_segmentation.rows;
				float cols = GT_segmentation.cols;
				float proper_size = rows*cols/1000;
				proper_size = proper_size/1000;

				std::cout << " No_Furniture Precision: " << Regions.precision << " Recall: "<< Regions.recall << " Inverse recall " << Regions.inverse_recall << " time: "<< Regions.time <<" Labels " << max <<"  size " << proper_size << std::endl;
				
				addPrecision(Regions.precision);
				addRecall(Regions.recall);
				addInverseRecall(Regions.inverse_recall);
				addTime(Regions.time);
				boost::filesystem::path p(file);
				std::string name = p.filename().stem().string();
				addName(name);
				_proper_size.push_back(proper_size);
				_labels.push_back(max);
			}
			
			
			void exportAll(const std::string& file_out){
				
				calculate();
				
				std::string result_file = file_out;
				std::ofstream myfile;
				if(!exists_test3(result_file)){
					myfile.open (result_file);
					myfile << "precision recall inverse_recall time labels size\n";
				}
				else{
					myfile.open (result_file, std::ios::out | std::ios::app);
				}
				
				if (myfile.is_open())
				{
					for(int i = 0 ; i < _precision.size() ; ++i){
						
						myfile << _name[i] << " " << _precision[i] << " " << _recall[i] << " " << _inverse_recall[i] << " " << _time[i] << " " << _labels[i] << " " << _proper_size[i] << "\n";
						
					}
					
					myfile << "\n\nprecision_mean recall_mean inverse_recall_mean precision_sd recall_sd inverse_recall_sd time_mean \n";
					myfile << _mean_p << " " << _mean_r << " " << _mean_ir << " " << _sd_p << " " << _sd_r << " " << _sd_ir << " " << mean<double>(_time) << "\n";
					myfile.close();
					
					
				}
				else std::cout << "Unable to open file";
				
			}
			
			
		};

		
	}
}

#endif