#ifndef RSI_GRAPHZONE_19052016
#define RSI_GRAPHZONE_19052016

#include <iterator>
#include <forward_list>

#include "ZoneRI.hpp"
#include "EdgeZone.hpp"
#include "maoris/Utils.hpp"
#include "maoris/GraphZone.hpp"
#include "maoris/Utils.hpp"
#include "bettergraph/SimpleGraph.hpp"
#include "ZoneCompared.hpp"


namespace AASS{
	namespace RSI{
		
		

		//TODO convert Mat to Eigen !
		// ATTENTION: if uniqueness is calculated and then a node is added, then the flag to check if the uniqueness was updated is wrong... Need to do it either every time a node is added but it will need to move the addVertex function here.

		class GraphZoneRI: public maoris::GraphZoneInterface<ZoneRI, maoris::EdgeElement>{
			
		protected:
// 			double _margin_factor;
			
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
			
// 			 ///@param[in] threshold : fraction representing the fraction of the biggest value of cluster until a new cluster must created. If init node got value 100 and threshold = 1/10 then if the new node as 90 or less, it is not fused.
// 			double _threshold;
			
		public:
			typedef typename maoris::GraphZoneInterface<ZoneRI, maoris::EdgeElement>::GraphType GraphZoneRIType;
			typedef typename maoris::GraphZoneInterface<ZoneRI, maoris::EdgeElement>::Vertex VertexZoneRI;
			typedef typename maoris::GraphZoneInterface<ZoneRI, maoris::EdgeElement>::Edge EdgeZoneRI;
			typedef typename maoris::GraphZoneInterface<ZoneRI, maoris::EdgeElement>::VertexIterator VertexIteratorZoneRI;
			typedef typename maoris::GraphZoneInterface<ZoneRI, maoris::EdgeElement>::EdgeIterator EdgeIteratorZoneRI;

			GraphZoneRI(): _nb_of_unique(-1), _pca_classified(false), _sd_away_fom_mean_for_uniqueness(1){};
			
			/**
			 * @brief copy function to get the segmentation from maoris
			 */
			GraphZoneRI(const maoris::GraphZone& graph_in) : _nb_of_unique(-1), _pca_classified(false), _sd_away_fom_mean_for_uniqueness(1){
				
				std::map<maoris::GraphZone::Vertex, VertexZoneRI> vertices;
				
				std::pair<maoris::GraphZone::VertexIteratorZone, maoris::GraphZone::VertexIteratorZone> vp;
				//vertices access all the vertix
				for (vp = boost::vertices(graph_in); vp.first != vp.second;) {
					maoris::GraphZone::VertexZone v = *vp.first; 
					++vp.first;
					
					VertexZoneRI v_out;
					ZoneRI z(graph_in[v]);
					addVertex(v_out, z);
					
					vertices[v] = v_out; 
					// std::cout << "Value " << (*this)[v].getValue() << std::endl;
				}
				
				auto es = boost::edges(graph_in);
				for (auto eit = es.first; eit != es.second; ++eit) {
					maoris::GraphZone::VertexZone src = boost::source(*eit, graph_in);
					maoris::GraphZone::VertexZone targ = boost::target(*eit, graph_in);
					
					VertexZoneRI v_src = vertices[src];
					VertexZoneRI v_tgt = vertices[targ];
					
					EdgeZoneRI e_out;
					addEdge(e_out, v_src, v_tgt, graph_in[*eit]);
				}
				
				
			};
			
			
			void draw(cv::Mat& m, const maoris::GraphZoneInterface<ZoneRI, maoris::EdgeElement>::Vertex& v, const cv::Scalar& color) const
			{
// 				exit(0);
				(*this)[v].drawZone(m, color);
				(*this)[v].drawContour(m, color);	
				(*this)[v].drawPCA(m, color);	
			}
			
			void draw(cv::Mat& drawmat) const
			{
				
				cv::Mat drawmat_old;
				drawmat.convertTo(drawmat_old, CV_8U);
				
				cv::Scalar color;
				cv::RNG rng(12345);
				int nb_zones = this->getNumVertices();
				int color_step = 249 / nb_zones;
				int count = 1;
				std::pair<VertexIteratorZoneRI, VertexIteratorZoneRI> vp;
				//vertices access all the vertix
				for (vp = boost::vertices((*this)); vp.first != vp.second; ++vp.first) {
					
					if(drawmat.channels() == 1){
						color = color_step * count;
					}
					else if(drawmat.channels() == 3){
						color[0] = color_step * count;
						color[1] = color_step * count;
						color[2] = color_step * count;
					}
					count++;
					
					VertexZoneRI v = *vp.first;
					
			// 		if((*this)[v].getZone().size() > 100){
			// 				if(getNumEdges(v) > 1){
						
					draw(drawmat, v, color);
					
					EdgeIteratorZoneRI out_i, out_end;
					EdgeZoneRI e;
					
					for (boost::tie(out_i, out_end) = boost::out_edges(v, (*this)); 
						out_i != out_end; ++out_i) {
						e = *out_i;
						VertexZoneRI src = boost::source(e, (*this)), targ = boost::target(e, (*this));
						if( (*this)[targ].getZone().size() > 100 ){
			// 					cv::line(drawmat, (*this)[src].getCentroid(), (*this)[targ].getCentroid(), cv::Scalar(255));
						}
						if( (*this)[e].canRemove() == false ){
// 							cv::line(drawmat, (*this)[src].getCentroid(), (*this)[targ].getCentroid(), cv::Scalar(255), 2);
						}
						else{
// 							cv::line(drawmat, (*this)[src].getCentroid(), (*this)[targ].getCentroid(), cv::Scalar(150));
						}
					}
				}
					
				for (vp = boost::vertices((*this)); vp.first != vp.second; ++vp.first) {

					VertexZoneRI v = *vp.first;
					EdgeIteratorZoneRI out_i, out_end;
					EdgeZoneRI e;
// 					(*this)[v].printLabel(drawmat);
					
				}

			}
			
			
// 			void setThreshold(double t){if(t >= 0 && t<= 1){_threshold = t;}else{throw std::runtime_error("Threhsold needs to be between 0 and 1");}}
// 			void setMargin(double m){if(m >= 0 && m<= 1){_margin_factor = m;}else{throw std::runtime_error("Margin needs to be between 0 and 1");}}
			
// 			double getT(){return _threshold;}
			
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
			
			
			void drawUnique(cv::Mat& drawmat) const;
			///@brief drawing function to compare the maps to the dataset of Bromman.
			
			std::vector<AASS::RSI::ZoneCompared> compare(GraphZoneRI& target);
			
			void updatePCA(){
// 				std::pair<VertexIteratorZone, VertexIteratorZone> vp;
				auto vp = boost::vertices((*this));
				for(vp = boost::vertices((*this)) ; vp.first != vp.second;){
					std::cout <<"Updating PCA "<< std::endl;
					auto v = *vp.first;
					++vp.first;
					
// 					(*this)[v].PCA();
// 					try{
						(*this)[v].PCA();
// 					}
// 					catch(maoris::ZoneHasNoContour& nocont){
// 						//Fuse the zone because no contour
// 						removeVertexWhilePreservingEdges(v, false);
// 					}
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
			

			
// 			///@brief update all zones and element of the graph: update PCA, remove Ripples, update countours, clasify shapes, clssify PCA
// 			void update(){
// 				updatePCA();
// // 				removeRiplesv2();
// 				updateContours();
// 				updatePCA();
// 				setSizesClassification();
// 				setPCAClassification();
// 			}
// 			
			/**
			 * @brief set if every zone is unique or not
			 * A zone is unique if the size or the pca is at least 1 sd from the mean
			 * The score is 2 - the sum of pdf of size and pca for each unique attribute or 1 if it's non unique.
			 * Thus non unique zone have a score of 0
			 */
			void updateUnique(){
				std::cout << "Update uniqueness" << std::endl;
// 				update();
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
				
				
// 				std::cout << "set PCA Classification " << std::endl;
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
				_mean_pca = maoris::mean<double>(pca_comp);
				_variance_pca = maoris::variance<double>(pca_comp, _mean_pca);
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
					
// 					std::cout << "SCORE" << standardized[i] << std::endl;
					(*this)[v].setPCAClassification(standardized[i]);
					++i ;
				}
				
				_pca_classified = true;
				
// 				std::cout <<"DONE" << std::endl;
			}
			
			
			///@brief force the value of each zone depending on the size from biggest zone to smallest. Normalize it between 0 for the smallest zone and 1 for the largest.
			void setSizesClassification() {
				
// 				std::cout << "set PCA Classification " << std::endl;
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
				_mean_size = maoris::mean<double>(size_comp);
				_variance_size = maoris::variance<double>(size_comp, _mean_size);
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
// 					std::cout << "SCORE" << standardized[i] << std::endl;
					(*this)[v].setSizeClassification(standardized[i]);
					++i ;
				}
				
				_size_classified = true;
				
// 				std::cout <<"DONE" << std::endl;
				
			}
			
			


			//Give all of them score and set all of them to unique
			void updateAllUnique(){

// 				update();
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
					
// 					std::cout << "PCA " <<  score_pca << " mean " << _mean_pca << " standar dev " << _sdeviation_pca << " pca " << (*this)[v].getPCADiff() << std::endl;
					
					(*this)[v].setUniqueness(true, res);
					count++;
				}
								
				_nb_of_unique = count;
			}
			
			

			
		private:
		
// 			double variance (std::vector<double> input, double mean){
// 				double variance = 0 ;
// 				auto it = input.begin();
// 				for(it ; it != input.end() ; ++it){
// 					variance = variance + ( (*it - mean) * (*it - mean) );
// // 					std::cout << "variance " << variance << std::endl;
// 				}
// 				
// 				return variance / (double) input.size();
// 			}
// 				
// 			double mean(std::vector<double> input){
// 				double sum = 0;
// 				auto it = input.begin();
// 				for(it ; it != input.end() ; ++it){
// 					sum = sum + (*it);
// 				}
// 				return sum / (double) input.size();
// 			}
				
			std::vector<double> standardization (std::vector<double> input, double mean, double sdeviation){
				std::vector<double> out;
				auto it = input.begin();
				for(it; it != input.end() ; ++it){
					double v_value = (*it - mean) / sdeviation;
					out.push_back(v_value);
				}
				return out;
			}
						
			
		};
		
		
		

	}
}

#endif
