#ifndef RSI_GRAPHZONE_19052016
#define RSI_GRAPHZONE_19052016

#include "Zone.hpp"
#include "Utils.hpp"
#include "bettergraph/SimpleGraph.hpp"

namespace AASS{
	namespace RSI{
		

		//TODO convert Mat to Eigen !

		class GraphZone : public bettergraph::SimpleGraph<Zone, int>{
			
		protected:
			
		public:
			typedef typename bettergraph::SimpleGraph<Zone, int>::GraphType GraphType;
			typedef typename bettergraph::SimpleGraph<Zone, int>::Vertex VertexPlace;
			typedef typename bettergraph::SimpleGraph<Zone, int>::Edge EdgePlace;
			typedef typename bettergraph::SimpleGraph<Zone, int>::VertexIterator VertexIteratorPlace;
			typedef typename bettergraph::SimpleGraph<Zone, int>::EdgeIterator EdgeIteratorPlace;

			GraphZone(){};
			
			void draw(cv::Mat& draw);
			
		};
		
		
		inline void GraphZone::draw(cv::Mat& draw)
		{
			

		}

	}
}

#endif