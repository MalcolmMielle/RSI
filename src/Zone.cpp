#include "Zone.hpp"
#include "ZoneCompared.hpp"


AASS::RSI::ZoneComparedInterface AASS::RSI::Zone::compare(const AASS::RSI::Zone& zone_in) const {
	
// // // // 	//DO this outside of the function 
// 	if(zone_in.isUnique() == false || this->isUnique() == false){
// 		//One of the zone is not unique
// 		return 1;
// 	}
	
	AASS::RSI::ZoneComparedInterface zone_out(*this, zone_in);
	
	double score_pca = this->getPCADiff();
	double score_size = this->getSizeClassification();
	double score_pca_target = zone_in.getPCADiff();
	double score_size_target = zone_in.getSizeClassification();
	double res = 1;
// 	
// 	if(diff <= 0.20 || diff_pca <= 0.50){
// 		res = (diff + diff_pca) / 2;
// 	}
// 	else{
// 		res = (diff + 1) / 2;
// 	}
	//Since the Standard deviation is 1
	//PCA within 1 sd
	//Each attribute can remove half of the res. Since the max distance between then is 1, it should not go under 0 
	
// 	std::cout << "score_pca " << score_pca << " score_size " << score_size << " score_pca_target " << score_pca_target << " score_size_target " << score_size_target << std::endl;
	
	
	if(score_size <= score_size_target + 1 && score_size >= score_size_target - 1){
		std::cout << "Similar size" << std::endl;
		res = res - (0.5 - (std::abs(score_size - score_size_target) / 2 ));
		//If pca is 20% similar
// 		if(std::abs(score_pca - score_pca_target) < 0.20){
			std::cout << "Similar pca" << std::endl;
			res = res - (0.5 - (std::abs(score_pca - score_pca_target) / 2 ));
// 		}
	}
	
// 	//Compare their elongation
// 	double simi_zone = comparePCA(zone_in);
// 
// 	// Compare the relative size				
// 	assert(zone_in.getSizeClassification() <=1 && zone_in.getSizeClassification() >=0);
// 	assert(getSizeClassification() <=1 && getSizeClassification() >=0);
// 	
// 	double diff = zone_in.getSizeClassification() - getSizeClassification();
// 	diff = std::abs<double>(diff);
// 	
// // 				std::cout << "Diff " << diff << std::endl;
// 	std::cout << "PCA " << zone_in.getPCAClassification() << std::endl;
// 	assert(zone_in.getPCAClassification() <= 1 && zone_in.getPCAClassification() >= 0);
// 	assert(getPCAClassification() <=1 && getPCAClassification() >=0);
// 	
// 	double diff_pca = zone_in.getPCAClassification() - getPCAClassification();
// 
// 	diff_pca = std::abs<double>(diff_pca);
// 	std::cout << "Diff PCA " << diff_pca << std::endl;
// 	std::cout << "Diff " << diff << std::endl;
// 	
// 
// // 				diff = 1 - diff;
// 	
// // 				std::cout << "Diff " << diff << std::endl;
// 	
// 	double res;
// 	
// 	if(diff <= 0.20 || diff_pca <= 0.50){
// 		res = (diff + diff_pca) / 2;
// 	}
// 	else{
// 		res = (diff + 1) / 2;
// 	}
	assert(res <= 1);
	assert(res >= 0);
					
	//ATTENTION Testing
	zone_out.size_diff = score_size;
	zone_out.pca_diff = score_pca;
	zone_out.size_source = getSizeClassification();
	zone_out.size_target = zone_in.getSizeClassification();
	zone_out.setSimilarity(res);
	
	return zone_out;
	
// 				//If both have similar size, we compare the PCA also
// 				//ATTENTION Magic number
// 				if(diff <= 0.20){
// 					//ATTENTION : compare zone_similarity + diff in size
// // 					assert((simi_zone + diff) / 2 < diff);
// 					size_diff = diff;
// 					pca_diff = simi_zone;
// 					return (simi_zone + diff) / 2;
// 					
// 				}
// 				//Otherwise, we only return the shape comparison since it is pretty bad.
// 				else{
// 					//ATTENTION : compare diff in size
// 					size_diff = diff;
// 					pca_diff = 1;
// 					return (diff + 1) / 2;
// 					
// 				}
	
}
		
void AASS::RSI::Zone::updateContour()
{
	cv::Mat copy_tmp;
	_zone_mat.copyTo(copy_tmp);
	
	std::vector< std::vector< cv::Point> > contours;
	std::vector<cv::Vec4i> hierarchy;
	cv::findContours(copy_tmp, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);
	
// 				assert(contours.size() >= 1 && "More than one shape in Zone");
	
	// Calculate the area of each contour
	
	//Use a lambda function to sort the contours
	std::sort(contours.begin(), contours.end(), [](std::vector<cv::Point> &pts, std::vector<cv::Point> &pts2){return pts.size() > pts2.size(); } );
	
	if(contours.size() > 1)
	{
// 		cv::Mat graphmat2 = cv::Mat::zeros(600,600, CV_8U);
// 		drawZone(graphmat2, cv::Scalar(255));
// 		drawContour(graphmat2, cv::Scalar(255));
// // 					std::cout << contours[0].size() << " " << contours[1].size() << std::endl;
// // 					if(contours[0].size() < contours[1].size()){
// // 						cv::Mat graphmat2 = cv::Mat::zeros(600,600, CV_8U);
// // 						for (int i = 0; i < contours[0].size(); ++i)
// // 						{
// // 							graphmat2.at<uchar>(contours[0][i].y, contours[0][i].x) = 255;
// // 						}
// 			cv::imshow("fina", graphmat2);
// // 						cv::waitKey(0);	
// // 						throw std::runtime_error("Calculating PCA with zone not in order");
// // 					}
// 		cv::waitKey(0);
		
// 		throw ZoneHasMoreThanOneContour("MORE THAN ONE CONTOUR !");
	}
	if(contours.size() == 0 ){
		throw ZoneHasNoContour("NO CONTOUR IN ZONE !");
	}
// 	std::cout << "Contour size " << contours.size() << std::endl;
	_contours = contours;
}

