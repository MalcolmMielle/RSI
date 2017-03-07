#ifndef RSI_UNIQUENESS_06112016
#define RSI_UNIQUENESS_06112016

#include <algorithm>
#include <boost/math/distributions/normal.hpp>

#include "ZoneExtractor.hpp"
#include "FuzzyOpening.hpp"
#include "Kmean.hpp"
#include "ZoneReducer.hpp"
#include "hungarian.h"
#include "HungarianMatcher.hpp"


namespace AASS{
	namespace RSI{
		
		///@brief create a graph from a file image
		void makeGraphSketch(const std::string& file, AASS::RSI::GraphZone& graph_slam){
						
			cv::Mat slam = cv::imread(file, CV_LOAD_IMAGE_GRAYSCALE);
		// 	
		// 	cv::imshow("input", slam);
		// 	cv::waitKey(0);
			
			cv::threshold(slam, slam, 20, 255, cv::THRESH_BINARY);
			cv::threshold(slam, slam, 20, 255, cv::THRESH_BINARY_INV);
			
			std::cout << "/************ FUZZY OPENING*************/ \n";
			AASS::RSI::FuzzyOpening fuzzy_slam;
			fuzzy_slam.fast(false);
			
			cv::Mat out_slam;
		// 	cv::imshow("SLAM", slam);
		// 	cv::waitKey(0);
			fuzzy_slam.fuzzyOpening(slam, out_slam, 500);
			std::cout << "Done opening " << std::endl;
			out_slam.convertTo(out_slam, CV_8U);
			
		// 	std::cout << out << std::endl;
			
			std::cout << "/************ REDUCING THE SPACE OF VALUES *****************/\n";
			cv::Mat out_tmp_slam;
			AASS::RSI::reduceZone(out_slam, out_tmp_slam);
		// 	cv::imshow("REDUCED", out_tmp_slam);
		// 	cv::waitKey(0);
			
			AASS::RSI::ZoneExtractor zone_maker;
			std::cout << "WHATERSHED SLAM" << std::endl;
			zone_maker.extract(out_tmp_slam);
			
			std::cout << "Got the ZONES" << std::endl;

			// 	std::cout << "Getting the graph" << std::endl;
			
			std::cout << "/*********** MAKING AND TRIMMING THE GRAPH ***************/\n";
			graph_slam = zone_maker.getGraph();
			graph_slam.removeVertexValue(0);

			std::cout << "Number of nodes" << graph_slam.getNumVertices() << std::endl;
			
			//Watershed Algorithm
			graph_slam.watershed(0.25);
			
			int size_to_remove = 100;
			graph_slam.removeVertexUnderSize(size_to_remove, true);
			graph_slam.removeLonelyVertices();
			if(graph_slam.lonelyVertices())
				throw std::runtime_error("Fuck you lonelyness");	
			
// 			cv::Mat graphmat2 = cv::Mat::zeros(out_tmp_slam.size(), CV_8U);
// 			graph_slam.draw(graphmat2);
// 			std::string s = std::to_string(i);
// 			cv::imshow(s, graphmat2);
		}
		
		
		
		
		
		class Uniqueness{
			
		protected:
			double _score;
			
		public:
			Uniqueness() : _score(0){};
			
			/**
			 * @brief Match a graph onto itself and check the cost of matching one zone onto all the other. Use a normal distribution to know which ones are relevant.
			 */
			std::vector<std::pair<GraphZone::Vertex, double> > uniqueness(AASS::RSI::GraphZone& graph_slam) const {
				std::vector<std::pair<GraphZone::Vertex, double> > out;
				//Match it onto itself
				AASS::RSI::HungarianMatcher hungmatch;
				std::vector<int> scores;
				auto match = hungmatch.match(graph_slam, graph_slam, scores);
				auto cost = hungmatch.getCostNonConstOptimized();
				//Get the uniqueness out of the proba distrib
				
				int count = 0 ;
				for(int i = 0 ; i < graph_slam.getNumVertices() ; i++) {
					
					std::vector<int> score_tmp;
// 					std::cout << "exporting" << std::endl;
					
					for(int j = 0 ; j < graph_slam.getNumVertices() ; j++) {
// 						std::cout << cost[i][j] << " " ;
						score_tmp.push_back(cost[i][j]);
					}
// 					std::cout << std::endl;
					
					auto mean_v = mean(score_tmp);
					std::cout << "mean " << mean_v << std::endl;
					auto variance_v = variance(score_tmp, mean_v);
					std::cout << "variance " << variance_v << std::endl;
					auto min = std::min_element<std::vector<int>::iterator>(score_tmp.begin(), score_tmp.end());
					std::cout << "min " << *min << std::endl;
					
					bool uniq = isUnique(variance_v, mean_v, *min);
					
					if(uniq == true){
						std::cout << "The zone is unique" << std::endl;
						++count;
					}
					else {
						std::cout << "The zone is NOT unique" << std::endl;
					}
					
					double score = uniquenesScoreRow(score_tmp, variance_v, mean_v);
					
					out.push_back(std::pair<GraphZone::Vertex, double>(match[i].first, score) );
					std::cout << "The score of the zone is " << score << std::endl << std::endl;
					
					graph_slam[match[i].first].setUniqueness(uniq);
					
				}
				graph_slam.setNumUnique(count);
				assert(graph_slam.getNumUnique() == count);
				
				return out;
				
			}
			
			std::vector<std::pair<GraphZone::Vertex, double> > uniqueness(const std::string& file) const {
				
				//Create the graph
				AASS::RSI::GraphZone graph_slam;
				initGraph(file, graph_slam);
						
				
				return uniqueness(graph_slam);
				
				
			}
			
		private:
			
			bool initGraph(const std::string& file, AASS::RSI::GraphZone& graph_slam) const {
				
				//Create the graph
				makeGraphSketch(file, graph_slam);
				
				/********** PCA of all zones in Graph and removing the ripples **********/
				
				graph_slam.updatePCA();
				graph_slam.removeRiplesv2();
								
			}
			
			double variance(std::vector<int> input, double mean) const{
				double variance = 0 ;
				auto it = input.begin();
				for(it ; it != input.end() ; ++it){
					variance = variance + ( (*it - mean) * (*it - mean) );
// 					std::cout << "variance " << variance << std::endl;
				}
				
				return variance / (double) input.size();
			}
			
			double mean(std::vector<int> input) const {
				double sum = 0;
				auto it = input.begin();
				for(it ; it != input.end() ; ++it){
					sum = sum + (*it);
				}
				return sum / (double) input.size();
			}
			
			/** 
			 * @brief return true if the min is at least 1 standard deviation from the mean
			 */
			bool isUnique(double variance_model_1, double mean_v, int input) const {
// 				std::cout << "input : " << variance_model_1 << " " << mean_v << " " << input << std::endl;
				//standard deviation is square root of variance.
				double sdeviation = sqrt(variance_model_1);
// 				boost::math::normal nd(mean_v, sdeviation);
// 				double var =  boost::math::pdf(nd, input);
// 				double low_mean =  boost::math::pdf(nd, mean);
				
// 				std::cout << "Mean " << mean_v << " super mean " << mean_v - ( 3 * sdeviation) << " min gauss " << input << std::endl;
				
				//ATTENTION Magic number
				if(input <= mean_v - sdeviation){
					return true;
				}
				else{
					return false;
				}
			}
			
			/**
			 * @brief Test if the row represent a unique zone or note and return the associated uniqueness score.
			 * TODO : better normalizatin of this score for now 0.98 is not good.
			 * 
			 */
			double uniquenesScoreRow(std::vector<int> scores, double variance_model_1, double mean_v) const {
				
				double score_tmp = 1;
				
				double sdeviation = sqrt(variance_model_1);
				std::cout << "sd " << sdeviation << std::endl;
				boost::math::normal nd(mean_v, sdeviation);
				
				bool flag_isUnique = false;
				auto it = scores.begin();
				for(it; it != scores.end() ; it++){
					if(*it <= mean_v - sdeviation){
						flag_isUnique = true;
						double prob1 =  boost::math::cdf(nd, *it + 0.5);
						double prob2 =  boost::math::cdf(nd, *it - 0.5);
						double res = prob1 - prob2 ;
						if(res > 1 || res < 0){
							throw std::runtime_error("the cdf in base proba failed and it's not supposed to EVER");
						}
						score_tmp = score_tmp - res;
					}
				}
				
				if(flag_isUnique == false){
					score_tmp = 0 ;
				}
				return score_tmp;
			}
			
			
			/**
			* @brief Calcule the probability based on the normal distribution.
			* Since it represent the pdf, the probability is given by the integral between two values. Must use the cdf
			*/
			double normalDistribution(double variance_model_1, double e1_input, int e_model) const 
			{

				
			// 	boost::math::normal _nd(mean, standard deviation); 
				//standard deviation is square root of variance.
				double sdeviation = sqrt(variance_model_1);
	// 			std::cout << "std deviation " << sdeviation << " variance of model " << variance_model_1 << " edge number mkodel" << e_model <<" edge number" << e1_input << std::endl;
				
				boost::math::normal nd(e_model, sdeviation);
				//pdf stands for probibility density function
				//The probability density function is nonnegative everywhere, and its integral over the entire space is equal to one
				double var =  boost::math::cdf(nd, e1_input + sdeviation);
				double var2 =  boost::math::cdf(nd, e1_input - sdeviation);
				double res = var - var2 ;
				if(res > 1 || res < 0){
					throw std::runtime_error("the cdf in base proba failed and it's not supposed to EVER");
				}
				return res;
			}
			
		};
	}
}

#endif
