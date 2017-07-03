#ifndef RSI_GRAPHZONE_19052016
#define RSI_GRAPHZONE_19052016

#include <iterator>

#include "Zone.hpp"
#include "EdgeZone.hpp"
#include "Utils.hpp"
#include "bettergraph/SimpleGraph.hpp"
#include "ZoneCompared.hpp"


namespace AASS{
	namespace RSI{
		
		

		//TODO convert Mat to Eigen !
		// ATTENTION: if uniqueness is calculated and then a node is added, then the flag to check if the uniqueness was updated is wrong... Need to do it either every time a node is added but it will need to move the addVertex function here.

		class GraphZone : public bettergraph::SimpleGraph<Zone, EdgeElement>{
			
		private:
			//TEST
			int _iteration;
			
		protected:
			
			
			///@brief -1 if not init
			int _nb_of_unique;
			
			///@brief mean of pca classification
			double _mean_pca;
			double _variance_pca;
			double _sdeviation_pca;
			
			///@brief just to make sure it was done.
			bool _pca_classified;
			
			///@brief mean of pca classification
			double _mean_size;
			double _variance_size;
			double _sdeviation_size;
			
			///@brief just to make sure it was done.
			bool _size_classified;
			
			int _sd_away_fom_mean_for_uniqueness;
			
			 ///@param[in] threshold : fraction representing the fraction of the biggest value of cluster until a new cluster must created. If init node got value 100 and threshold = 1/10 then if the new node as 90 or less, it is not fused.
			double _threshold;
			
		public:
			typedef typename bettergraph::SimpleGraph<Zone, EdgeElement>::GraphType GraphZoneType;
			typedef typename bettergraph::SimpleGraph<Zone, EdgeElement>::Vertex VertexZone;
			typedef typename bettergraph::SimpleGraph<Zone, EdgeElement>::Edge EdgeZone;
			typedef typename bettergraph::SimpleGraph<Zone, EdgeElement>::VertexIterator VertexIteratorZone;
			typedef typename bettergraph::SimpleGraph<Zone, EdgeElement>::EdgeIterator EdgeIteratorZone;

			GraphZone(): _nb_of_unique(-1), _pca_classified(false), _sd_away_fom_mean_for_uniqueness(1), _threshold(0.25){};
			
			void setThreshold(double t){if(t >= 0 && t<= 1){_threshold = t;}else{throw std::runtime_error("Threhsold needs to be between 0 and 1");}}
			
			void setNumUnique(int n){_nb_of_unique = n;}
			double getNumUnique(){if(_nb_of_unique != -1){return _nb_of_unique;}else{return getNumVertices();}}
			
			void setSDAwayFromMeanForUniqueness(int i){_sd_away_fom_mean_for_uniqueness = i;}
			int getSDAwayFromMeanForUniqueness(){return _sd_away_fom_mean_for_uniqueness;}
			
			bool zoneUniquenessWasCalculated(){
				if(_nb_of_unique == -1){
					return false;
				}
				else{
					return true;
				}
			}
			
			///@brief draw a simple image with no annotations.
			void drawSimple(cv::Mat& drawmat) const;
			void drawSimple(cv::Mat& m, const bettergraph::SimpleGraph<Zone, int>::Vertex& v, const cv::Scalar& color) const;
			///@brief draw map with all info on image
			void draw(cv::Mat& drawmat) const;
			void draw(cv::Mat& m, const bettergraph::SimpleGraph<Zone, int>::Vertex& v, const cv::Scalar& color) const;
			///@brief draw the zone one by one.
			void drawPartial(cv::Mat& drawmat) const;
			void drawUnique(cv::Mat& drawmat) const;
			///@brief drawing function to compare the maps to the dataset of Bromman.
			void drawEvaluation(cv::Mat& drawmat) const;
			void drawEvaluation(cv::Mat& m, const bettergraph::SimpleGraph<Zone, int>::Vertex& v, const cv::Scalar& color) const;
			
			///@brief Remove all vertex with Value value. Do not preserve edges or zones
			void removeVertexValue(int value){
				std::pair<VertexIteratorZone, VertexIteratorZone> vp;
				//vertices access all the vertix
				for (vp = boost::vertices((*this)); vp.first != vp.second;) {
					VertexZone v = *vp.first; 
					++vp.first;
					// std::cout << "Value " << (*this)[v].getValue() << std::endl;
					if((*this)[v].getValue() == value){
						std::cout << "Trying to remove" << std::endl;
						removeVertex(v);
						std::cout << "Removed" << std::endl;
					}
				}
				removeLonelyVertices();
			}
			
			/**
			 * @brief TODO : spurious branches removed on a zone
			 * Method :
			 * -> Finding only node with one link does not work because it maybe be in between two nodes
			 * -> Should use watershed algorithm + SOMETHING
			 */
// 			void removeRiples(){};
			
			/** @brief Remove all vertex which zone is less than size in size.
			 * @param size size under which the vertex gets removed
			 * @param preserveEdgeConnectic if true the connection between edge is "preserved" i.e if we have Node1 - Node2 - Node3 and we remove Node2 the end result will be Node1 - Node3. Plus the zone of the removed vertex will be fused into the biggest neighbor
			 * TODO : change name to Vertices
			 */			
			void removeVertexUnderSize(int size, bool preserveEdgeConnectic);
			
			bool asVerticesWithNoEdges();
			
			/**
			 * @brief watershed.
			 */
			void watershed(){watershed(_threshold);}
			
			/**
			 * @brief watershed. DO NOT USE DIRECTLY. Indeed, the parameter thrshold is also used by the ripple removal so it should be the same. One should use watershed()
			 * @param[in] threshold : fraction representing the fraction of the biggest value of cluster until a new cluster must created. If init node got value 100 and threshold = 1/10 then if the new node as 90 or less, it is not fused.
			 */
			void watershed(double threshold);			
			
			bool lonelyVertices(){
				std::pair<AASS::RSI::GraphZone::VertexIteratorZone, AASS::RSI::GraphZone::VertexIteratorZone> vp;
				//vertices access all the vertix
				std::cout << "NEW start lonely" << std::endl;
		// 		std::cout << "num of vertices " << getNumVertices() << std::endl; 
				for (vp = boost::vertices(*this); vp.first != vp.second;) {
		// 			std::cout << "Looking up vertex " << std::endl;
					auto v = *vp.first;
					++vp.first;
					if(getNumEdges(v) == 0){
						return true;
					} 
				}
				return false;
			
			}
			
			/**
			 * @brief Remove vertices with no edges
			 */
			void removeLonelyVertices(){
				std::pair<AASS::RSI::GraphZone::VertexIteratorZone, AASS::RSI::GraphZone::VertexIteratorZone> vp;
				//vertices access all the vertix
				// std::cout << "NEW start remove lonely" << std::endl;
				// std::cout << "num of vertices " << getNumVertices() << std::endl; 
				int i = 0;
				for (vp = boost::vertices(*this); vp.first != vp.second;) {

					// std::cout << "num of vertices " << getNumVertices() << std::endl; 
					// std::cout << "Looking up vertex " << i <<std::endl;
					++i;
					auto v = *vp.first;
					++vp.first;
					if(getNumEdges(v) == 0){
						// std::cout << "Remove" << std::endl;
						removeVertex(v);
					}
				}
			}
			
			void updatePCA(){
// 				std::pair<VertexIteratorZone, VertexIteratorZone> vp;
				auto vp = boost::vertices((*this));
				for(vp = boost::vertices((*this)) ; vp.first != vp.second;){
					std::cout <<"Updating PCA "<< std::endl;
					auto v = *vp.first;
					++vp.first;
					
// 					(*this)[v].PCA();
					try{
						(*this)[v].PCA();
					}
					catch(ZoneHasNoContour& nocont){
						//Fuse the zone because no contour
						removeVertexWhilePreservingEdges(v, false);
					}
// 					catch(std::exception& e){
// 						std::cout << "Here : " << e.what() << std::endl;
// 						cv::Mat graphmat2 = cv::Mat::zeros(600,600, CV_8U);
// 						(*this)[v].draw(graphmat2, cv::Scalar(100));
// 						cv::imshow("fused", graphmat2);
// 						cv::waitKey(0);	
// 						exit(0);
// 					}
				}
			}
			
			void updateContours(){
// 				std::pair<VertexIteratorZone, VertexIteratorZone> vp;
				auto vp = boost::vertices((*this));
				for(vp = boost::vertices((*this)) ; vp.first != vp.second;){
					std::cout <<"Updating Contours "<< std::endl;
					auto v = *vp.first;
					++vp.first;
					
// 					(*this)[v].PCA();
					try{
						(*this)[v].updateContour();
					}
					catch(std::exception& e){
						std::cout << "Here : " << e.what() << std::endl;
						cv::Mat graphmat2 = cv::Mat::zeros(600,600, CV_8U);
						(*this)[v].drawZone(graphmat2, cv::Scalar(100));
						(*this)[v].drawContour(graphmat2, cv::Scalar(100));
						cv::imshow("fused", graphmat2);
						cv::waitKey(0);	
						exit(0);
					}
				}
			}
			
			///@brief update all zones and element of the graph: update PCA, remove Ripples, update countours, clasify shapes, clssify PCA
			void update(){
				updatePCA();
				removeRiplesv2();
				updateContours();
				updatePCA();
				setSizesClassification();
				setPCAClassification();
			}
			
			/**
			 * @brief set if every zone is unique or not
			 * A zone is unique if the size or the pca is at least 1 sd from the mean
			 * The score is 2 - the sum of pdf of size and pca for each unique attribute or 1 if it's non unique.
			 * Thus non unique zone have a score of 0
			 */
			void updateUnique(){
				std::cout << "Update uniqueness" << std::endl;
				update();
				if(_pca_classified == false){
					throw std::runtime_error("PCA wasn't classified before searching for unique zones");
				}
				if(_size_classified == false){
					throw std::runtime_error("Size wasn't classified before searching for unique zones");
				}
				
				int count = 0;
				
				boost::math::normal nd_pca(_mean_pca, _sdeviation_pca);
				boost::math::normal nd_size(_mean_size, _sdeviation_size);
				auto vp = boost::vertices((*this));
				for(vp ; vp.first != vp.second; ++vp.first){
					
					double res = 2;
					
					auto v = *vp.first;
					double score_pca = (*this)[v].getPCAClassification();
					double score_size = (*this)[v].getSizeClassification();
					
// 					Test PCA TODO
					if(score_pca <= 0 - (_sd_away_fom_mean_for_uniqueness * 1)){
						//Calculate the score. It's under the mean so score is already low
						double prob1 =  boost::math::cdf(nd_pca, score_pca + 0.5);
						double prob2 =  boost::math::cdf(nd_pca, score_pca - 0.5);
						assert(prob1 - prob2 > 0);
						assert(prob1 - prob2 < 1);
						
// 						std::cout << res << " prob " << prob1 - prob2 << std::endl;
						res = res - (prob1 - prob2) ;
						
						if(res > 2 || res < 0){
							throw std::runtime_error("the cdf pca in base proba failed and it's not supposed to EVER");
						}
					}
					else if(score_pca >= 0 + (_sd_away_fom_mean_for_uniqueness * 1)){
						//Score is above so we invert curve
						double prob1 =  1 - boost::math::cdf(nd_pca, score_pca + 0.5);
						double prob2 =  1 - boost::math::cdf(nd_pca, score_pca - 0.5);
						assert(prob2 - prob1 > 0);
						assert(prob2 - prob1 < 1);
						
// 						std::cout << res << " prob " << prob2 - prob1 << std::endl;
						res = res - (prob2 - prob1) ;
						
						if(res > 2 || res < 0){
							throw std::runtime_error("the cdf pca in base proba failed and it's not supposed to EVER");
						}
					}
					else{
						res = res - 1;
					}
					
					//Test SIZE STANDARDIZED SO mean 0 and sd 1
					if(score_size <= 0 - (_sd_away_fom_mean_for_uniqueness * 1)){
						//Calculate the score. It's under the mean so score is already low
						double prob1 =  boost::math::cdf(nd_size, score_size + 0.5);
						double prob2 =  boost::math::cdf(nd_size, score_size - 0.5);
						assert(prob1 - prob2 > 0);
						assert(prob1 - prob2 < 1);
						
						res = res - (prob1 - prob2) ;

// 						std::cout << res << std::endl;
						if(res > 2 || res < 0){
							throw std::runtime_error("the cdf size in base proba failed and it's not supposed to EVER");
						}
					}
					else if(score_size >= 0 + (_sd_away_fom_mean_for_uniqueness * 1)){
						//Score is above so we invert curve
						double prob1 =  1 - boost::math::cdf(nd_size, score_size + 0.5);
						double prob2 =  1 - boost::math::cdf(nd_size, score_size - 0.5);
						assert(prob2 - prob1 > 0);
						assert(prob2 - prob1 < 1);
						
						res = res - (prob2 - prob1);

// 						std::cout << res << std::endl;
						if(res > 2 || res < 0){
							throw std::runtime_error("the cdf size in base proba failed and it's not supposed to EVER");
						}
					}
					else{
						res = res - 1;
					}
					
					std::cout << "PCA " <<  score_pca << " mean " << _mean_pca << " standar dev " << _sdeviation_pca << " pca " << (*this)[v].getPCADiff() << std::endl;
					
					if(res == 0){
						(*this)[v].setUniqueness(false, res);
					}
					else{
						(*this)[v].setUniqueness(true, res);
						count++;
					}

				}
				
// 				exit(0);
								
				_nb_of_unique = count;
				
			}
			
			
			//TODO 
			//THAT DOESN*T MAKE SENSE WE DON*T CARE IF THE PCA ARE THE SAME RELATIVE TO OTHER PCA. This could create matching between relatively different pca. PCA is an "exact measure when calculating the difference"
			///@brief force the value of each zone depending on the uniqueness of the PCA. Normalize it between 0 for the smallest difference and 1 for the largest.
			void setPCAClassification() {
				
				
				std::cout << "set PCA Classification " << std::endl;
				std::vector<double> pca_comp;
// 				std::pair<VertexIteratorZone, VertexIteratorZone> vp;
// 				double max = -1, min = -1;
				auto vp = boost::vertices((*this));
				for(vp ; vp.first != vp.second; ++vp.first){
					auto v = *vp.first;
					double si = (*this)[v].getPCADiff();
					pca_comp.push_back(si);
// 					if(max == -1 || max < si){
// 						max = si;
// 					}
// 					if(min == -1 || min > si){
// 						min = si;
// 					}
				}
				
// 				assert(max != -1);
// 				assert(min != -1);
				assert(pca_comp.size() == getNumVertices());
				_mean_pca = mean(pca_comp);
				_variance_pca = variance(pca_comp, _mean_pca);
				_sdeviation_pca = sqrt(_variance_pca);
				auto standardized = standardization(pca_comp, _mean_pca, _sdeviation_pca);
				
// 				std::cout << "max " << max << " min " << min << std::endl;
// 				double div = (max - min);
// 				assert(div != 0);
// 				std::cout << "div " << div << " min " << min << std::endl;
				
				std::vector<double> all_scores;
// 				std::pair<VertexIteratorZone, VertexIteratorZone> vp;
// 				int max = -1, min = -1;
				vp = boost::vertices((*this));
				int i = 0;
				for(vp ; vp.first != vp.second; ++vp.first){
					auto v = *vp.first;
					
					assert((*this)[v].getPCADiff() == pca_comp[i]);
					std::cout << (pca_comp[i] - _mean_pca) / _sdeviation_pca  << " == " << standardized[i] << std::endl;
					assert(((pca_comp[i] - _mean_pca) / _sdeviation_pca) == standardized[i]);
					
					std::cout << "SCORE" << standardized[i] << std::endl;
					(*this)[v].setPCAClassification(standardized[i]);
					++i ;
				}
				
				_pca_classified = true;
				
				std::cout <<"DONE" << std::endl;
			}
			
			
			///@brief force the value of each zone depending on the size from biggest zone to smallest. Normalize it between 0 for the smallest zone and 1 for the largest.
			void setSizesClassification() {
				
				std::cout << "set PCA Classification " << std::endl;
				std::vector<double> size_comp;
// 				std::pair<VertexIteratorZone, VertexIteratorZone> vp;
// 				double max = -1, min = -1;
				auto vp = boost::vertices((*this));
				for(vp ; vp.first != vp.second; ++vp.first){
					auto v = *vp.first;
					double si = (*this)[v].size();
					size_comp.push_back(si);
// 					if(max == -1 || max < si){
// 						max = si;
// 					}
// 					if(min == -1 || min > si){
// 						min = si;
// 					}
				}
				
// 				assert(max != -1);
// 				assert(min != -1);
				assert(size_comp.size() == getNumVertices());
				_mean_size = mean(size_comp);
				_variance_size = variance(size_comp, _mean_size);
				_sdeviation_size = sqrt(_variance_size);
				auto standardized = standardization(size_comp, _mean_size, _sdeviation_size);
				
// 				std::cout << "max " << max << " min " << min << std::endl;
// 				double div = (max - min);
// 				assert(div != 0);
// 				std::cout << "div " << div << " min " << min << std::endl;
				
				std::vector<double> all_scores;
// 				std::pair<VertexIteratorZone, VertexIteratorZone> vp;
// 				int max = -1, min = -1;
				vp = boost::vertices((*this));
				int i = 0;
				for(vp ; vp.first != vp.second; ++vp.first){
					auto v = *vp.first;
					std::cout << "SCORE" << standardized[i] << std::endl;
					(*this)[v].setSizeClassification(standardized[i]);
					++i ;
				}
				
				_size_classified = true;
				
				std::cout <<"DONE" << std::endl;
				
			}
			
			
			void removeRiplesv2(int dist = -1);
			void removeRiplesv3(int dist = -1);
				
			void updateAllEdges(){
				auto vp = boost::vertices((*this));
				for(vp ; vp.first != vp.second; ++vp.first){
					auto v = *vp.first;
					EdgeIteratorZone out_i, out_end;
					//Since we fuse the old zone in biggest we only need to link them to biggest
					for (boost::tie(out_i, out_end) = boost::out_edges(v, (*this)); 
						out_i != out_end; out_i = ++out_i) {
						EdgeZone e_second = *out_i;
						VertexZone targ = boost::target(e_second, (*this));
						(*this)[e_second].setOldScore(v, targ, -2);
						(*this)[e_second].min_toward = -2;
					}
				}
			}

			//Give all of them score and set all of them to unique
			void updateAllUnique(){

				update();
				if(_pca_classified == false){
					throw std::runtime_error("PCA wasn't classified before searching for unique zones");
				}
				if(_size_classified == false){
					throw std::runtime_error("Size wasn't classified before searching for unique zones");
				}
				
				int count = 0;
				
				boost::math::normal nd_pca(_mean_pca, _sdeviation_pca);
				boost::math::normal nd_size(_mean_size, _sdeviation_size);
				auto vp = boost::vertices((*this));
				for(vp ; vp.first != vp.second; ++vp.first){
					
					double res = 2;
					
					auto v = *vp.first;
					double score_pca = (*this)[v].getPCAClassification();
					double score_size = (*this)[v].getSizeClassification();
					
// 					Test PCA TODO
					if(score_pca <= 0 - (_sd_away_fom_mean_for_uniqueness * 1)){
						//Calculate the score. It's under the mean so score is already low
						double prob1 =  boost::math::cdf(nd_pca, score_pca + 0.5);
						double prob2 =  boost::math::cdf(nd_pca, score_pca - 0.5);
						assert(prob1 - prob2 > 0);
						assert(prob1 - prob2 < 1);
						
// 						std::cout << res << " prob " << prob1 - prob2 << std::endl;
						res = res - (prob1 - prob2) ;
						
						if(res > 2 || res < 0){
							throw std::runtime_error("the cdf pca in base proba failed and it's not supposed to EVER");
						}
					}
					else if(score_pca >= 0 + (_sd_away_fom_mean_for_uniqueness * 1)){
						//Score is above so we invert curve
						double prob1 =  1 - boost::math::cdf(nd_pca, score_pca + 0.5);
						double prob2 =  1 - boost::math::cdf(nd_pca, score_pca - 0.5);
						assert(prob2 - prob1 > 0);
						assert(prob2 - prob1 < 1);
						
// 						std::cout << res << " prob " << prob2 - prob1 << std::endl;
						res = res - (prob2 - prob1) ;
						
						if(res > 2 || res < 0){
							throw std::runtime_error("the cdf pca in base proba failed and it's not supposed to EVER");
						}
					}
					else{
						res = res - 1;
					}
					
					//Test SIZE STANDARDIZED SO mean 0 and sd 1
					if(score_size <= 0 - (_sd_away_fom_mean_for_uniqueness * 1)){
						//Calculate the score. It's under the mean so score is already low
						double prob1 =  boost::math::cdf(nd_size, score_size + 0.5);
						double prob2 =  boost::math::cdf(nd_size, score_size - 0.5);
						assert(prob1 - prob2 > 0);
						assert(prob1 - prob2 < 1);
						
						res = res - (prob1 - prob2) ;

// 						std::cout << res << std::endl;
						if(res > 2 || res < 0){
							throw std::runtime_error("the cdf size in base proba failed and it's not supposed to EVER");
						}
					}
					else if(score_size >= 0 + (_sd_away_fom_mean_for_uniqueness * 1)){
						//Score is above so we invert curve
						double prob1 =  1 - boost::math::cdf(nd_size, score_size + 0.5);
						double prob2 =  1 - boost::math::cdf(nd_size, score_size - 0.5);
						assert(prob2 - prob1 > 0);
						assert(prob2 - prob1 < 1);
						
						res = res - (prob2 - prob1);

// 						std::cout << res << std::endl;
						if(res > 2 || res < 0){
							throw std::runtime_error("the cdf size in base proba failed and it's not supposed to EVER");
						}
					}
					else{
						res = res - 1;
					}
					
					std::cout << "PCA " <<  score_pca << " mean " << _mean_pca << " standar dev " << _sdeviation_pca << " pca " << (*this)[v].getPCADiff() << std::endl;
					
					(*this)[v].setUniqueness(true, res);
					count++;
				}
								
				_nb_of_unique = count;
			}
			
			
			/**
			 * @brief remove ripples by looking at the pca and the angle between the main component and How much the zone is glued to it. If one of the angle is NOT a ripple then the zone is saved.
			 * 
			 * TODO
			 * 
			 */
			std::vector<int> removeRiples(){
				updatePCA();
				auto vp = boost::vertices((*this));
				
				std::vector<VertexZone> vertex_zone;
				
				std::vector<VertexZone> seen;
				
				for(vp = boost::vertices((*this)) ; vp.first != vp.second; ++vp.first){
					auto v = *vp.first;
					vertex_zone.push_back(v);
				}
				
				//TODO USE SORTING FUNCTION and then a simple for INSTEAD BUT I DON'T HAVE THE TIME NOW
				
				//TODO : sorting function + for every vertex, as long as no edge can be fused, fuse one and then restart fusing process of neighborhood from scratch (to avoid fucking up the edge iterator)
				while(vertex_zone.size() != 0){
					auto v = vertex_zone[0];
					int count = 0;
					for(size_t i = 1; i < vertex_zone.size() ; ++i){
						if((*this)[vertex_zone[i]].size() > (*this)[v].size()){
							v = vertex_zone[i];
							count = i;
						}
					}

					seen.push_back(v);
					
					auto zone_base = (*this)[v];
					std::cout << "Testing vertex " << std::endl;
					EdgeIteratorZone out_i, out_end;
					for (boost::tie(out_i, out_end) = boost::out_edges(v, (*this)); 
						out_i != out_end; ) {
						
						EdgeZone e_second = *out_i;
						VertexZone targ = boost::target(e_second, (*this));
						VertexZone src = boost::source(e_second, (*this));
					
						out_i = ++out_i;
					
						bool sen = false;
						for(size_t j = 0 ; j < seen.size() ; ++j){
							if(targ == seen[j])
								sen = true;
						}
					
						if(sen == false){
							auto pca = (*this)[targ].getPCA();
						
							//Find bigger direction
							std::pair<cv::Point2i, cv::Point2i> line1;
							std::pair<cv::Point2i, cv::Point2i> line2;
							
							double length1_x = (std::get<1>(pca).x - std::get<0>(pca).x);
							length1_x = length1_x * length1_x;
							double length1_y = (std::get<1>(pca).y - std::get<0>(pca).y);
							length1_y = length1_y * length1_y;
							double length1 = length1_x + length1_y;
							
							double length2_x = (std::get<2>(pca).x - std::get<0>(pca).x);
							length2_x = length2_x * length2_x;
							double length2_y = (std::get<2>(pca).y - std::get<0>(pca).y);
							length2_y = length2_y * length2_y;
							double length2 = length2_x + length2_y;
							
							double max = length1, min = length2;
							if(max < min){
								max = length2;
								min = length1;
							}
							
							std::cout << " max and 5 min " << max << " " << min * 5 << std::endl;
							
							//ATTENTION magic number
							if(max > (min * 5)){
							
								// Might be a riple
								// SECOND IDEA check how much the zone is glued to the first one
								std::cout << "test to remove a ripple" << std::endl;
								auto zoneedge = (*this)[targ];
								auto percentage = zone_base.contactPoint(zoneedge);
								
								if(percentage > 25){
									std::cout << "Removing a ripple because percentage is " << percentage << std::endl;
									//It's a ripple !
									//ATTENTION not sure if I should or not check for highest value and keep that one
									zone_base.fuse(zoneedge);
									(*this)[v] = zone_base;
									removeVertexWhilePreservingEdges(targ, true);
								}
								
								
								
								
								
							}
						}
					
					}
				
				
				}
			}
			
			
			///@brief return source first and target second
			std::vector<ZoneCompared> compare(GraphZone& target);
			

			///Overwrite
			virtual void removeVertex(Vertex& v){
				EdgeIteratorZone out_i, out_end;	
				for (boost::tie(out_i, out_end) = boost::out_edges(v, (*this)); 
					out_i != out_end;) {
					EdgeZone e_second = *out_i;
					boost::remove_edge(e_second, (*this));
					boost::tie(out_i, out_end) = boost::out_edges(v, (*this));
				}
				boost::remove_vertex(v, (*this));
				
			}
			virtual void removeEdge(Edge& e){
				if(!(*this)[e].canRemove()){
					throw std::runtime_error("Can't remove unbreakable edge");
				}
				boost::remove_edge(e, (*this));
			}
			

			
		private:
			/**
			 * @brief recursively fuse node to be fuse to the top_vertex
			 * @param top_vertex : Vertex currently studied
			 * @param first_vertex : Vertex on the top of the food chain. The initial vertex of the watershed
			 * @param top_vertex_visited : The vertices already visited by the watershed algorithm
			 * @param top_vertex_visited_tmp : empty deque used by the algorithm to remember which node were visited in the recursion
			 * @param threshold : Threshold at which the difference between the value of top_vertex and a neighborhood vertex is considered enough for the neighborhood vertex not to be fused and to be considered a new zone. Actually no : fraction the new node must be partt of to not be fyused
			 */
			void getAllNodeRemovedWatershed(AASS::RSI::GraphZone::VertexZone& top_vertex, AASS::RSI::GraphZone::VertexZone& first_vertex, const std::deque< AASS::RSI::GraphZone::VertexZone >& top_vertex_visited, std::deque< AASS::RSI::GraphZone::VertexZone >& top_vertex_visited_tmp, double threshold, int direction, std::deque<VertexZone>& to_be_removed);

			/**
			* @brief get all ripples
			* base_vertex is the vertex to fuse in
			* top_vertex_visited is all the vertext visited since the beginning
			* dist is the maximum distance between the base and a ripple to be fused
			*/
			void getAllNodeRemovedRipples(AASS::RSI::GraphZone::VertexZone& base_vertex, const std::deque< AASS::RSI::GraphZone::VertexZone >& top_vertex_visited, int dist = -1);
			void removeVertexWhilePreservingEdges(AASS::RSI::GraphZone::VertexZone& v, bool createUnBreakableLinks);
			void removeVertexWhilePreservingEdges(AASS::RSI::GraphZone::VertexZone& v, AASS::RSI::GraphZone::VertexZone& v_to_fuse_in, bool createUnBreakableLinks);
			
			///@brief Return true of the zone is ripple
			bool isRipple(const AASS::RSI::GraphZone::VertexZone& base_vertex, const AASS::RSI::GraphZone::VertexZone& might_be_ripple) const;
			
			///@brief Check if the zone is a ripple of a neighborhood and removes it
			bool checkAndReplaceRipple(AASS::RSI::GraphZone::VertexZone& might_be_ripple, AASS::RSI::GraphZone::VertexZone& to_fuse_in);
			
			double variance (std::vector<double> input, double mean){
				double variance = 0 ;
				auto it = input.begin();
				for(it ; it != input.end() ; ++it){
					variance = variance + ( (*it - mean) * (*it - mean) );
// 					std::cout << "variance " << variance << std::endl;
				}
				
				return variance / (double) input.size();
			}
				
			double mean(std::vector<double> input){
				double sum = 0;
				auto it = input.begin();
				for(it ; it != input.end() ; ++it){
					sum = sum + (*it);
				}
				return sum / (double) input.size();
			}
				
			std::vector<double> standardization (std::vector<double> input, double mean, double sdeviation){
				std::vector<double> out;
				auto it = input.begin();
				for(it; it != input.end() ; ++it){
					double v_value = (*it - mean) / sdeviation;
					out.push_back(v_value);
				}
				return out;
			}
			
			
			void makeAllUnbreakableEdges(VertexZone& base_vertex);
			
			
		};
		
		
		

	}
}

#endif
