#ifndef RSI_NEIGHBORHOOD_28052016
#define RSI_NEIGHBORHOOD_28052016

#include "GraphZone.hpp"

namespace AASS{
	
	namespace RSI{
		
		class Neighbor{
		protected:
			std::pair <Zone, EdgeElement> _element;
			std::pair <GraphZone::VertexZone, GraphZone::EdgeZone> _accessor;
		public:
			Neighbor(const GraphZone::VertexZone vertex, const GraphZone::EdgeZone& edge, const Zone& zone, const EdgeElement& edgeelement) : _element(std::pair <Zone, EdgeElement>(zone, edgeelement)), _accessor(std::pair <GraphZone::VertexZone, GraphZone::EdgeZone>(vertex, edge)){};
			
			
			GraphZone::VertexZone getVertex(){return _accessor.first;}
			Zone getVertexElement(){return _element.first;}
			GraphZone::Edge getEdge(){return _accessor.second;}
			EdgeElement getEdgeElement(){return _element.second;}
			
			const GraphZone::VertexZone getVertex() const {return _accessor.first;}
			const Zone getVertexElement() const {return _element.first;}
			const GraphZone::Edge getEdge() const {return _accessor.second;}
			const EdgeElement getEdgeElement() const {return _element.second;}
			
		};
		
		
		class Neighborhood : std::deque{
		protected:
			std::deque< Neighbor > _neighborhood;
			GraphZone::VertexZone _center;
			Zone _center_zone;
			
		public:
			Neghborhood(){};
			
			void push_back(const GraphZone::VertexZone v, const GraphZone::EdgeZone& edge, const Zone& zone, const EdgeElement& edgeelement){ 
				Neighbor el(v, edge, zone, edgeelement); _neighborhood.push_back(el);
			}
			
			void push_front(const GraphZone::VertexZone v, const GraphZone::EdgeZone& edge, const Zone& zone, const EdgeElement& edgeelement){
				Neighbor el(v, edge, zone, edgeelement);
				_neighborhood.push_front(el);
			}
			
			void pop_back(){_neighborhood.pop_back();}
			void pop_front(){_neighborhood.pop_front();}
			
			void setCenter(const GraphZone::VertexZone& v, const Zone& zone){_center = v; _center_zone = zone;}
			const GraphZone& getCenter() const {return _center;}
			GraphZone getCenter(){return _center;}
			
			
		};
		
		
		
		//TODO
		inline double compareNeighbor(const Neighbor& n1, const Neighbor& n2){
			
		};
		
		
	}
	
}

#endif