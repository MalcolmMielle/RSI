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
			
		protected:
			///@brief -1 if not init
			int _nb_of_unique;
			
		public:
			typedef typename bettergraph::SimpleGraph<Zone, EdgeElement>::GraphType GraphZoneType;
			typedef typename bettergraph::SimpleGraph<Zone, EdgeElement>::Vertex VertexZone;
			typedef typename bettergraph::SimpleGraph<Zone, EdgeElement>::Edge EdgeZone;
			typedef typename bettergraph::SimpleGraph<Zone, EdgeElement>::VertexIterator VertexIteratorZone;
			typedef typename bettergraph::SimpleGraph<Zone, EdgeElement>::EdgeIterator EdgeIteratorZone;

			GraphZone(): _nb_of_unique(-1){};
			
			void setNumUnique(int n){_nb_of_unique = n;}
			double getNumUnique(){if(_nb_of_unique != -1){return _nb_of_unique;}else{return getNumVertices();}}
			
			bool zoneUniquenessWasCalculated(){
				if(_nb_of_unique == -1){
					return false;
				}
				else{
					return true;
				}
			}
			
			void draw(cv::Mat& drawmat) const;
			void drawPartial(cv::Mat& drawmat) const;
			void draw(cv::Mat& m, const bettergraph::SimpleGraph<Zone, int>::Vertex& v, const cv::Scalar& color) const;
			
			///@brief Remove all vertex with Value value. Do not preserve edges or zones
			void removeVertexValue(int value){
				std::pair<VertexIteratorZone, VertexIteratorZone> vp;
				//vertices access all the vertix
				for (vp = boost::vertices((*this)); vp.first != vp.second;) {
					VertexZone v = *vp.first; 
					++vp.first;
					std::cout << "Value " << (*this)[v].getValue() << std::endl;
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
			 * @brief watershed
			 * @param[in] threshold : fraction representing the fraction of the biggest value of cluster until a new cluster must created. If init node got value 100 and threshold = 1/10 then if the new node as 90 or less, it is not fused.
			 */
			void watershed(double threshold);
			
			
			bool lonelyVertices(){
				std::pair<AASS::RSI::GraphZone::VertexIteratorZone, AASS::RSI::GraphZone::VertexIteratorZone> vp;
				//vertices access all the vertix
				std::cout << "NEW start" << std::endl;
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
				std::cout << "NEW start" << std::endl;
		// 		std::cout << "num of vertices " << getNumVertices() << std::endl; 
				for (vp = boost::vertices(*this); vp.first != vp.second;) {
		// 			std::cout << "Looking up vertex " << std::endl;
					auto v = *vp.first;
					++vp.first;
					if(getNumEdges(v) == 0){
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
					catch(std::exception& e){
						std::cout << "Here : " << e.what() << std::endl;
						cv::Mat graphmat2 = cv::Mat::zeros(600,600, CV_8U);
						(*this)[v].draw(graphmat2, cv::Scalar(100));
						cv::imshow("fused", graphmat2);
						cv::waitKey(0);	
						exit(0);
					}
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
						(*this)[v].draw(graphmat2, cv::Scalar(100));
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
				setSizesClassification();
				setPCAClassification();
			}
			
			
			//TODO
			///@brief force the value of each zone depending on the uniqueness of the PCA. Normalize it between 0 for the smallest zone and 1 for the largest.
			void setPCAClassification() {
				
				std::cout << "set PCA Classification " << std::endl;
				std::vector<double> pca_comp;
// 				std::pair<VertexIteratorZone, VertexIteratorZone> vp;
				double max = -1, min = -1;
				auto vp = boost::vertices((*this));
				for(vp = boost::vertices((*this)) ; vp.first != vp.second; ++vp.first){
					auto v = *vp.first;
					double si = (*this)[v].getPCADiff();
					pca_comp.push_back(si);
					if(max == -1 || max < si){
						max = si;
					}
					if(min == -1 || min > si){
						min = si;
					}
				}
				
				assert(max != -1);
				assert(min != -1);
				assert(pca_comp.size() == getNumVertices());
				
				std::cout << "max " << max << " min " << min << std::endl;
				double div = (max - min);
				assert(div != 0);
				std::cout << "div " << div << " min " << min << std::endl;
				
				std::vector<double> all_scores;
// 				std::pair<VertexIteratorZone, VertexIteratorZone> vp;
// 				int max = -1, min = -1;
				vp = boost::vertices((*this));
				int i = 0;
				for(vp = boost::vertices((*this)) ; vp.first != vp.second; ++vp.first){
					auto v = *vp.first;
					double score = (pca_comp[i] - min) / div;
					++i ;
					std::cout << "SCORE" << score << std::endl;
					(*this)[v].setPCAClassification(score);
				}
				
				std::cout <<"DONE" << std::endl;
			}
			
			
			///@brief force the value of each zone depending on the size from biggest zone to smallest. Normalize it between 0 for the smallest zone and 1 for the largest.
			void setSizesClassification() {
				
				std::cout << "setSize Classification " << std::endl;
				std::vector<int> sizes;
// 				std::pair<VertexIteratorZone, VertexIteratorZone> vp;
				int max = -1, min = -1;
				auto vp = boost::vertices((*this));
				for(vp = boost::vertices((*this)) ; vp.first != vp.second; ++vp.first){
					auto v = *vp.first;
					int si = (*this)[v].size();
					sizes.push_back(si);
					if(max == -1 || max < si){
						max = si;
					}
					if(min == -1 || min > si){
						min = si;
					}
				}
				
				double div = (max - min);
				
				std::vector<double> all_scores;
// 				std::pair<VertexIteratorZone, VertexIteratorZone> vp;
// 				int max = -1, min = -1;
				vp = boost::vertices((*this));
				int i = 0;
				for(vp = boost::vertices((*this)) ; vp.first != vp.second; ++vp.first){
					auto v = *vp.first;
					double score = (sizes[i] - min) / div;
					++i ;
					std::cout << "SCORE" << score << std::endl;
					(*this)[v].setSizeClassification(score);
				}
				
				std::cout <<"DONE" << std::endl;
				
			}
			
			
			void removeRiplesv2();
			
			
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
									removeVertexWhilePreservingEdges(targ);
								}
								
								
								
								
								
							}
						}
					
					}
				
				
				}
			}
			
			
			///@brief return source first and target second
			std::vector<ZoneCompared> compare(GraphZone& target);
			
			

			
		private:
			/**
			 * @brief recursively fuse node to be fuse to the top_vertex
			 * @param top_vertex : Vertex currently studied
			 * @param first_vertex : Vertex on the top of the food chain. The initial vertex of the watershed
			 * @param top_vertex_visited : The vertices already visited by the watershed algorithm
			 * @param top_vertex_visited_tmp : empty deque used by the algorithm to remember which node were visited in the recursion
			 * @param threshold : Threshold at which the difference between the value of top_vertex and a neighborhood vertex is considered enough for the neighborhood vertex not to be fused and to be considered a new zone. Actually no : fraction the new node must be partt of to not be fyused
			 */
			void getAllNodeRemovedWatershed(AASS::RSI::GraphZone::VertexZone& top_vertex, AASS::RSI::GraphZone::VertexZone& first_vertex, const std::deque< AASS::RSI::GraphZone::VertexZone >& top_vertex_visited, std::deque< AASS::RSI::GraphZone::VertexZone >& top_vertex_visited_tmp, double threshold, int direction);
			void getAllNodeRemovedRipples(AASS::RSI::GraphZone::VertexZone& base_vertex, const std::deque< AASS::RSI::GraphZone::VertexZone >& top_vertex_visited);
			void removeVertexWhilePreservingEdges(AASS::RSI::GraphZone::VertexZone& v);
			void removeVertexWhilePreservingEdges(AASS::RSI::GraphZone::VertexZone& v, AASS::RSI::GraphZone::VertexZone& v_to_fuse_in);
			
			///@brief Return true of the zone is ripple
			bool isRipple(const AASS::RSI::GraphZone::VertexZone& base_vertex, const AASS::RSI::GraphZone::VertexZone& might_be_ripple) const;
			
		};
		
		
		

	}
}

#endif
