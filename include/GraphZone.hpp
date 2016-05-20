#ifndef RSI_GRAPHZONE_19052016
#define RSI_GRAPHZONE_19052016

#include <iterator>

#include "Zone.hpp"
#include "Utils.hpp"
#include "bettergraph/SimpleGraph.hpp"

namespace AASS{
	namespace RSI{
		

		//TODO convert Mat to Eigen !

		class GraphZone : public bettergraph::SimpleGraph<Zone, int>{
			
		protected:
			
		public:
			typedef typename bettergraph::SimpleGraph<Zone, int>::GraphType GraphZoneType;
			typedef typename bettergraph::SimpleGraph<Zone, int>::Vertex VertexZone;
			typedef typename bettergraph::SimpleGraph<Zone, int>::Edge EdgeZone;
			typedef typename bettergraph::SimpleGraph<Zone, int>::VertexIterator VertexIteratorZone;
			typedef typename bettergraph::SimpleGraph<Zone, int>::EdgeIterator EdgeIteratorZone;

			GraphZone(){};
			
			void draw(cv::Mat& drawmat) const;
			void draw(cv::Mat& m, const bettergraph::SimpleGraph<Zone, int>::Vertex& v, const cv::Scalar& color) const;
			
			///@brief Remove all vertex with Value value. Do not preserve edges
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
			}
			
			/**
			 * @brief TODO : spurious branches removed on a zone
			 * Method :
			 * -> Finding only node with one link does not work because it maybe be in between two nodes
			 * -> Should use watershed algorithm + SOMETHING
			 */
			void removeRiples(){};
			
			/** @brief Remove all vertex which zone is less than size in size.
			 * @param size size under which the vertex gets removed
			 * @param preserveEdgeConnectic if true the connection between edge is "preserved" i.e if we have Node1 - Node2 - Node3 and we remove Node2 the end result will be Node1 - Node3.
			 */			
			void removeVertexUnderSize(int size, bool preserveEdgeConnectic);
			
		private:
			
		};

	}
}

#endif