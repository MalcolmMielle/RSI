#ifndef RSI_NEIGHBORHOOD_28052016
#define RSI_NEIGHBORHOOD_28052016

#include "GraphZone.hpp"

namespace AASS{
	
	namespace RSI{
		
		class Neighborhood : std::deque{
		protected:
			std::deque< std::pair <GraphZone::VertexZone, GraphZone::EdgeZone> > _neighborhood;
			GraphZone::VertexZone _center;
			
		public:
			Neghborhood(){};
			
			void push_back(const std::pair <GraphZone::VertexZone, GraphZone::EdgeZone>& v){ _neighborhood.push_back(v);}
			void push_front(const std::pair <GraphZone::VertexZone, GraphZone::EdgeZone>& v){ _neighborhood.push_front(v);}
			void push_back(const GraphZone::VertexZone v, const GraphZone::EdgeZone& edge){ _neighborhood.push_back(std::pair<GraphZone::VertexZone, GraphZone::EdgeZone>(v, edge));}
			void push_front(const GraphZone::VertexZone v, const GraphZone::EdgeZone& edge){ _neighborhood.push_front(std::pair<GraphZone::VertexZone, GraphZone::EdgeZone>(v, edge));}
			
			void pop_back(){_neighborhood.pop_back();}
			void pop_front(){_neighborhood.pop_front();}
			
			void setCenter(const GraphZone::VertexZone& v){_center = v;}
			const GraphZone& getCenter() const {return _center;}
			GraphZone getCenter(){return _center;}
			
			
		};
		
		
		inline double editDistanceNeighborhood(const Neighborhood& n1, const Neighborhood& n2){
			
			
		}
		
		
		
	}
	
}

#endif