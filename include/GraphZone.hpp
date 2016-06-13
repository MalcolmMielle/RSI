#ifndef RSI_GRAPHZONE_19052016
#define RSI_GRAPHZONE_19052016

#include <iterator>

#include "Zone.hpp"
#include "EdgeZone.hpp"
#include "Utils.hpp"
#include "bettergraph/SimpleGraph.hpp"


namespace AASS{
	namespace RSI{
		
		//TODO convert Mat to Eigen !

		class GraphZone : public bettergraph::SimpleGraph<Zone, EdgeElement>{
			
		protected:
			
		public:
			typedef typename bettergraph::SimpleGraph<Zone, EdgeElement>::GraphType GraphZoneType;
			typedef typename bettergraph::SimpleGraph<Zone, EdgeElement>::Vertex VertexZone;
			typedef typename bettergraph::SimpleGraph<Zone, EdgeElement>::Edge EdgeZone;
			typedef typename bettergraph::SimpleGraph<Zone, EdgeElement>::VertexIterator VertexIteratorZone;
			typedef typename bettergraph::SimpleGraph<Zone, EdgeElement>::EdgeIterator EdgeIteratorZone;

			GraphZone(){};
			
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
			
			
// 			void watershed();
			void watershed(int threshold);
			
			
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
					(*this)[v].PCA();
				}
			}
			
			
			void removeRiplesv2();
			
			
						/**
			 * @brief remove ripples by looking at the pca and the angle between the main component and How much the zone is glued to it. If one of the angle is NOT a ripple then the zone is saved.
			 * 
			 * TODO
			 * 
			 */
			void removeRiples(){
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
			
			
			
			
			
			
			/**
			 * @brief remove ripples by looking at the pca and the angle between the main component and the direct link between the possible ripple and the center of the next zone. If one of the angle is NOT a ripple then the zone is saved.
			 * ATTENTION IMPORTANT BUG -> Limitation : the center of the zone does not represent the actual direction of the link between the two zones.
			 */
// 			void removeRiples(){
// 				updatePCA();
// 				auto vp = boost::vertices((*this));
// 				for(vp = boost::vertices((*this)) ; vp.first != vp.second;){
// 					auto v = *vp.first;
// 					++vp.first;
// 					auto pca = (*this)[v].getPCA();
// 					
// 					//Find bigger direction
// 					std::pair<cv::Point2i, cv::Point2i> line1;
// 					std::pair<cv::Point2i, cv::Point2i> line2;
// 					
// 					double length1_x = (std::get<1>(pca).x - std::get<0>(pca).x);
// 					length1_x = length1_x * length1_x;
// 					double length1_y = (std::get<1>(pca).y - std::get<0>(pca).y);
// 					length1_y = length1_y * length1_y;
// 					double length1 = length1_x + length1_y;
// 					
// 					double length2_x = (std::get<2>(pca).x - std::get<0>(pca).x);
// 					length2_x = length2_x * length2_x;
// 					double length2_y = (std::get<2>(pca).y - std::get<0>(pca).y);
// 					length2_y = length2_y * length2_y;
// 					double length2 = length2_x + length2_y;
// 					
// 					double max = length1, min = length2;
// 					if(max < min){
// 						max = length2;
// 						min = length1;
// 					}
// 					
// 					std::cout << " max and 5 min " << max << " " << min * 5 << std::endl;
// 					
// 					//ATTENTION magic number
// 					if(max > (min * 5)){
// 						
// 						// Might be a riple
// 						
// 						
// 						// FIRST IDEA : try to see if orientation of pca is opposed to gradient.
// 						
// 						auto orientation_pca = (*this)[v].getPCAOrientation();
// 						orientation_pca = orientation_pca * 180 / M_PI;
// 						std::cout << "ORI " << orientation_pca << std::endl;
// 						orientation_pca = (orientation_pca > 0) ? orientation_pca : orientation_pca + 360;
// 						std::cout << "SHould only be positionve " << orientation_pca << std::endl;
// 						
// 						
// 						
// 						bool flag_ripple = true;
// 						
// 						//Maybe only do it with the current vertex seen and then fuse
// 						EdgeIteratorZone out_i, out_end;
// 						for (boost::tie(out_i, out_end) = boost::out_edges(v, (*this)); 
// 							out_i != out_end; out_i = ++out_i) {
// 							
// 							EdgeZone e_second = *out_i;
// 							VertexZone targ = boost::target(e_second, (*this));
// 							VertexZone src = boost::source(e_second, (*this));
// 						
// 							auto p1 = (*this)[src].getCentroid();
// 							auto p2 = (*this)[targ].getCentroid();
// 						
// 							//Putting poitn on center 0
// 							cv::Point2i p3(p2.x - p1.x, p2.y -p1.y);
// 							double orientation_link = atan2(p3.x, p3.y);
// 							
// 							orientation_link = orientation_link * 180 / M_PI;
// 							orientation_link = (orientation_link > 0) ? orientation_link : orientation_link + 360;
// 							
// 							double max_tmp = orientation_link, min_tmp = orientation_pca;
// 							if(max_tmp < min_tmp){
// 								max_tmp = orientation_pca;
// 								min_tmp = orientation_link;
// 							}
// 							
// 							//Get value of both angles
// 							double distance = max_tmp - min_tmp;
// 							double distance2 = min_tmp + (360 - max_tmp);
// 							
// 							std::cout << "Distance " << distance << " distance 2 " << distance2 << " angles " << orientation_pca << " " << orientation_link << std::endl;
// 							//If the distance betwen pca is close to 45 it's a ripple !
// 							if( ( distance > 60 && distance < 110 ) || ( distance2 > 60 && distance2 < 110 ) ){
// 								//It's a ripple
// 							}
// 							else{
// 								std::cout << "NOT A RIPLE" << std::endl;
// 								//Not a ripple ! We have to save it
// 								flag_ripple = false;
// 							}
// 							
// 							
// 						}
// 						
// 						if(flag_ripple == true){
// 							std::cout << "REMOVING " << std::endl;
// 							removeVertexWhilePreservingEdges(v);
// 						}
// 						
// 					}
// 					//For debugging
// // 					else{
// // 						removeVertexWhilePreservingEdges(v);
// // 					}
// 					
// 				}
// 				
// 				
// 			}
			
		private:
			/**
			 * @brief recursively fuse node to be fuse to the top_vertex
			 * @param top_vertex : Vertex currently studied
			 * @param first_vertex : Vertex on the top of the food chain. The initial vertex of the watershed
			 * @param top_vertex_visited : The vertices already visited by the watershed algorithm
			 * @param threshold : Threshold at which the difference between the value of top_vertex and a neighborhood vertex is considered enough for the neighborhood vertex not to be fused and to be considered a new zone.
			 */
			void getAllNodeRemovedWatershed(AASS::RSI::GraphZone::VertexZone& top_vertex, AASS::RSI::GraphZone::VertexZone& first_vertex, const std::deque< AASS::RSI::GraphZone::VertexZone >& top_vertex_visited, int threshold);
			void getAllNodeRemovedRipples(AASS::RSI::GraphZone::VertexZone& base_vertex, const std::deque< AASS::RSI::GraphZone::VertexZone >& top_vertex_visited);
			void removeVertexWhilePreservingEdges(VertexZone v);
			
			///@brief Return true of the zone is ripple
			bool isRipple(const AASS::RSI::GraphZone::VertexZone& base_vertex, const AASS::RSI::GraphZone::VertexZone& might_be_ripple) const;
			
		};

	}
}

#endif