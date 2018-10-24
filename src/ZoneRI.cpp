#include "ZoneRI.hpp"
#include "ZoneCompared.hpp"


AASS::RSI::ZoneComparedInterface AASS::RSI::ZoneRI::compare(const AASS::RSI::ZoneRI& zone_in) const {
	
// // // // 	//DO this outside of the function 
// 	if(zone_in.isUnique() == false || this->isUnique() == false){
// 		//One of the zone is not unique
// 		return 1;
// 	}
	
	AASS::RSI::ZoneComparedInterface zone_out(*this, zone_in);
	
	std::cout << "PCA diff " << std::endl;
	double score_pca = this->getPCADiff();
	std::cout << "Size diff " << std::endl;
	double score_size = this->getSizeClassification();
	std::cout << "PCA diff " << std::endl;
	double score_pca_target = zone_in.getPCADiff();
	std::cout << "Size diff " << std::endl;
	double score_size_target = zone_in.getSizeClassification();
	double similarity = 1;
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

		double sum_difference = std::abs(score_size - score_size_target);
		sum_difference = sum_difference + std::abs(score_pca - score_pca_target);
		similarity = sum_difference / 2;

//		res = res - (0.5 - (std::abs(score_size - score_size_target) / 2 ));
//		//If pca is 20% similar
//// 		if(std::abs(score_pca - score_pca_target) < 0.20){
//			std::cout << "Similar pca" << std::endl;
//			res = res - (0.5 - (std::abs(score_pca - score_pca_target) / 2 ));
//// 		}
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
	assert(similarity <= 1);
	assert(similarity >= 0);
					
	//ATTENTION Testing
	zone_out.size_diff = score_size;
	zone_out.pca_diff = score_pca;
	zone_out.size_source = getSizeClassification();
	zone_out.size_target = zone_in.getSizeClassification();
	zone_out.setSimilarity(similarity);
	
	std::cout << "OUT" << std::endl;
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
		


