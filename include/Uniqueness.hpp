//#ifndef RSI_UNIQUENESS_06112016
//#define RSI_UNIQUENESS_06112016
//
//#include <algorithm>
//#include <boost/math/distributions/normal.hpp>
//
//#include "ZoneExtractor.hpp"
//#include "FuzzyOpening.hpp"
////#include "Kmean.hpp"
//#include "ZoneReducer.hpp"
//#include "hungarian.h"
//#include "HungarianMatcher.hpp"
//
//#include "maoris/Segmentor.hpp"
//
//
//namespace AASS{
//	namespace RSI{
//
//		///@brief create a graph from a file image
//		///TODO : wswitch to Segmentor
//		AASS::maoris::GraphZone makeGraphSketch(cv::Mat& image_in, double t, double margin, double ripples, double doors){
//
////			cv::imshow("IN", image_in);
////			cv::waitKey(0);
//
//			AASS::maoris::GraphZone graph_slam;
//			graph_slam.setThreshold(t);
//			graph_slam.setMargin(margin);
//			graph_slam.setThresholdFusionRipples(ripples);
//			graph_slam.setThresholdFusionDoors(doors);
//
//			double time = 0;
//			// 	makeGraph(slam, graph_slam, time);
//			AASS::maoris::Segmentor segmenteur;
//			time = segmenteur.segmentImage(image_in, graph_slam);
//
//			return graph_slam;
////			cv::Mat segmented_map = segmenteur.getSegmentedMap();
//		}
//
//
//
//
//
//		class Uniqueness{
//
//		protected:
//			double _score;
//
//		public:
//			Uniqueness() : _score(0){};
//
//			/**
//			 * @brief Match a graph onto itself and check the cost of matching one zone onto all the other. Use a normal distribution to know which ones are relevant.
//			 */
//			std::vector<std::pair<GraphZoneRI::Vertex, double> > uniqueness(AASS::RSI::GraphZoneRI& graph_slam) const {
//				std::vector<std::pair<GraphZoneRI::Vertex, double> > out;
//				//Match it onto itself
//// 				AASS::RSI::HungarianMatcher hungmatch;
//
//				//Easier to have int for the nromal distrib
//				std::vector<int> scores;
//// 				auto match = hungmatch.match(graph_slam, graph_slam, scores);
//// 				auto cost = hungmatch.getCostNonConstOptimized();
//
//				auto res = graph_slam.compare(graph_slam);
//				//Get the uniqueness out of the proba distrib
//
//				int count = 0 ;
//				for(int i = 0 ; i < graph_slam.getNumVertices() ; i++) {
//
//					std::vector<int> score_tmp;
//					std::vector<int> sc_tmp;
//// 					std::cout << "exporting" << std::endl;
//
//					for(int j = 0 ; j < graph_slam.getNumVertices() ; j++) {
//						std::cout << res[ (graph_slam.getNumVertices() * i) + j].getSimilarity() << " and " ;
//						score_tmp.push_back(res[ (graph_slam.getNumVertices() * i) + j].getSimilarity() * 100);
//// 						std::cout << cost[j][i] << " : " ;
//// 						sc_tmp.push_back(cost[j][i]);
//					}
//// 					std::cout << std::endl;
//
//					auto mean_v = mean(score_tmp);
//					std::cout << "mean " << mean_v << std::endl;
//					auto variance_v = variance(score_tmp, mean_v);
//					std::cout << "variance " << variance_v << std::endl;
//					auto min = std::min_element<std::vector<int>::iterator>(score_tmp.begin(), score_tmp.end());
//					std::cout << "min " << *min << std::endl;
//
//
//// 					assert((int)(mean_v * 100) == (int) mean_vs);
//// 					assert((int)(variance_v * 100) == (int) variance_vs);
//// 					assert((int)(min * 100) == (int) mins);
//
//					bool uniq = isUnique(variance_v, mean_v, *min);
//					double score;
//
//					if(uniq == true){
//						std::cout << "The zone is unique" << std::endl;
//						++count;
//						score = uniquenesScoreRow(score_tmp, variance_v, mean_v);
//					}
//					else {
//						std::cout << "The zone is NOT unique" << std::endl;
//						score = 0 ;
//					}
//
//					out.push_back(std::pair<GraphZoneRI::Vertex, double>(res[ (graph_slam.getNumVertices() * i)].source, score) );
//					std::cout << "The score of the zone is " << score << std::endl << std::endl;
//
//					graph_slam[res[ (graph_slam.getNumVertices() * i)].source].setUniqueness(uniq, score);
//
//				}
//// 				exit(0);
//				graph_slam.setNumUnique(count);
//				assert(graph_slam.getNumUnique() == count);
//
//				return out;
//
//			}
//
//			std::vector<std::pair<GraphZoneRI::Vertex, double> > uniqueness(cv::Mat& image_in) const {
//
//				//Create the graph
//				auto graph_slam = initGraph(image_in);
//				return uniqueness(graph_slam);
//
//			}
//
//		private:
//
//			AASS::RSI::GraphZoneRI initGraph(cv::Mat& image_in) const {
//
//				//Create the graph
//				//Default param
//				AASS::maoris::GraphZone gz = makeGraphSketch(image_in, 0.25, 0.1, 40, 40);
//				auto graph_slam = AASS::RSI::GraphZoneRI(gz);
//
//				/********** PCA of all zones in Graph and removing the ripples **********/
//
//				graph_slam.updatePCA();
//				return graph_slam;
//				//graph_slam.removeRiplesv2();
//
//			}
//
//			double variance(std::vector<int> input, double mean) const{
//				double variance = 0 ;
//				for(auto it = input.begin(); it != input.end() ; ++it){
//					variance = variance + ( (*it - mean) * (*it - mean) );
//// 					std::cout << "variance " << variance << std::endl;
//				}
//
//				return variance / (double) input.size();
//			}
//
//			double mean(std::vector<int> input) const {
//				double sum = 0;
//				for(auto it = input.begin(); it != input.end() ; ++it){
//					sum = sum + (*it);
//				}
//				return sum / (double) input.size();
//			}
//
//			/**
//			 * @brief return true if the min is at least 1 standard deviation from the mean
//			 */
//			bool isUnique(double variance_model_1, double mean_v, double input) const {
//// 				std::cout << "input : " << variance_model_1 << " " << mean_v << " " << input << std::endl;
//				//standard deviation is square root of variance.
//				double sdeviation = sqrt(variance_model_1);
//// 				boost::math::normal nd(mean_v, sdeviation);
//// 				double var =  boost::math::pdf(nd, input);
//// 				double low_mean =  boost::math::pdf(nd, mean);
//
//// 				std::cout << "Mean " << mean_v << " super mean " << mean_v - ( 3 * sdeviation) << " min gauss " << input << std::endl;
//
//				//ATTENTION Magic number
//				if(input <= mean_v - sdeviation){
//					return true;
//				}
//				else{
//					return false;
//				}
//			}
//
//			/**
//			 * @brief Test if the row represent a unique zone or note and return the associated uniqueness score.
//			 * TODO : better normalizatin of this score for now 0.98 is not good.
//			 *
//			 */
//			double uniquenesScoreRow(std::vector<int> scores, double variance_model_1, double mean_v) const {
//
//				double score_tmp = 1;
//
//				double sdeviation = sqrt(variance_model_1);
//				std::cout << "sd " << sdeviation << "mean " << mean_v << " variance " << variance_model_1 << std::endl;
//				boost::math::normal nd(mean_v, sdeviation);
//
//				bool flag_isUnique = false;
//				for(auto it = scores.begin(); it != scores.end() ; it++){
//					if(*it <= mean_v - sdeviation){
//						flag_isUnique = true;
//						double prob1 =  boost::math::cdf(nd, *it + 0.5);
//						double prob2 =  boost::math::cdf(nd, *it - 0.5);
//						double res = prob1 - prob2 ;
//						if(res > 1 || res < 0){
//							throw std::runtime_error("the cdf in base proba failed and it's not supposed to EVER");
//						}
//						score_tmp = score_tmp - res;
//					}
//				}
//
//				//on sd is 20% of the value on one side, so the max we can remove is 20% close to the sd.
//				//Min value is : 1 - boost::math::cdf(nd, mean_v - sdeviation + 0.5) and max is 1
//				double min = (1 - boost::math::cdf(nd, mean_v - sdeviation + 0.5) );
//
//				assert(score_tmp >= min );
//				assert(score_tmp <= 1 );
//
//				std::cout << "score " << score_tmp << " min " << min << std::endl;
//
//				score_tmp = ( score_tmp -  min ) / (1 - min );
//
//				std::cout << "score " << score_tmp  << std::endl;
//
//// 				if(flag_isUnique == false){
//// 					score_tmp = 0 ;
//// 				}
//				assert(flag_isUnique == true);
//				return score_tmp;
//			}
//
//
//			/**
//			* @brief Calcule the probability based on the normal distribution.
//			* Since it represent the pdf, the probability is given by the integral between two values. Must use the cdf
//			*/
//			double normalDistribution(double variance_model_1, double e1_input, double e_model) const
//			{
//
//
//			// 	boost::math::normal _nd(mean, standard deviation);
//				//standard deviation is square root of variance.
//				double sdeviation = sqrt(variance_model_1);
//	// 			std::cout << "std deviation " << sdeviation << " variance of model " << variance_model_1 << " edge number mkodel" << e_model <<" edge number" << e1_input << std::endl;
//
//				boost::math::normal nd(e_model, sdeviation);
//				//pdf stands for probibility density function
//				//The probability density function is nonnegative everywhere, and its integral over the entire space is equal to one
//				double var =  boost::math::cdf(nd, e1_input + sdeviation);
//				double var2 =  boost::math::cdf(nd, e1_input - sdeviation);
//				double res = var - var2 ;
//				if(res > 1 || res < 0){
//					throw std::runtime_error("the cdf in base proba failed and it's not supposed to EVER");
//				}
//				return res;
//			}
//
//		};
//	}
//}
//
//#endif
