#ifndef RSI_NEIGHBORHOOD_28052016
#define RSI_NEIGHBORHOOD_28052016

#include "GraphZone.hpp"
#include "editDistance.hpp"

namespace AASS{
	
	namespace RSI{
		
		class Neighbor{
		protected:
			///@brief Zone + Edge from src to Targ
			std::pair <Zone, EdgeElement> _element;
			///@brief VertexZone + VertexEdge
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
			
			void setElement(const GraphZone::VertexZone vertex, const GraphZone::EdgeZone& edge, const Zone& zone, const EdgeElement& edgeelement){
				_element = std::pair <Zone, EdgeElement>(zone, edgeelement);
				_accessor = std::pair <GraphZone::VertexZone, GraphZone::EdgeZone>(vertex, edge);
			}
			
			//Compare the diff in both edges and return a cost of 1 if the diff needs to be change and 0 if not
			double compare(const Neighbor& n1) const {
				if ( ( n1.getEdgeElement().getDiff() > 0 ) == ( _element.second.getDiff() > 0) ){
					return 0;
				}
				else{
					return 1;
				}
			}
			
			
		};
		
		///@brief compare two neighbor objects
		double compareNeighbor(const Neighbor& n1, const Neighbor& n2){
			return n1.compare(n2);
		}
		
		
		class Neighborhood{
		protected:
			std::deque< Neighbor > _neighborhood;
			GraphZone::VertexZone _center;
			Zone _center_zone;
			
		public:
			Neighborhood(){};
			
			void push_back(const GraphZone::VertexZone v, const GraphZone::EdgeZone& edge, const Zone& zone, const EdgeElement& edgeelement){ 
				Neighbor el(v, edge, zone, edgeelement); _neighborhood.push_back(el);
			}
			
			void push_back(const Neighbor& nei){ 
				_neighborhood.push_back(nei);
			}
			
			void push_front(const GraphZone::VertexZone v, const GraphZone::EdgeZone& edge, const Zone& zone, const EdgeElement& edgeelement){
				Neighbor el(v, edge, zone, edgeelement);
				_neighborhood.push_front(el);
			}
			
			void pop_back(){_neighborhood.pop_back();}
			void pop_front(){_neighborhood.pop_front();}
			
			void setCenter(const GraphZone::VertexZone& v, const Zone& zone){_center = v; _center_zone = zone;}
			const GraphZone::VertexZone& getCenter() const {return _center;}
			GraphZone::VertexZone getCenter(){return _center;}
			const std::deque< Neighbor >& getNeighborhood() const {return _neighborhood;}
			std::deque< Neighbor >& getNeighborhood() {return _neighborhood;}
			
			double compare(const Neighborhood& neig) const{
				std::string out;
				double score = levenshteinDistance<Neighbor, Neighbor>(_neighborhood, neig.getNeighborhood(), compareNeighbor, out);
			}
			
			//Not good because we should use the edit distance here as a better comparison mean
// 			double compare(const Neighborhood& neig) const{
// 				auto it_n1 = _neighborhood.begin();
// 				double score = 0 ;
// 				for(it_n1 ; it_n1 != _neighborhood.end() ; ++it_n1){
// 					auto it_n2 = neig.begin();
// 					for(it_n2 ; it_n2 != neig.end() ; it_n2++){
// 						score = score + it_n1->compare(*it_n2);
// 					}
// 				}
// 			}
			
			std::deque< Neighbor>::iterator begin() {
				return _neighborhood.begin();
			}
			std::deque< Neighbor>::const_iterator begin() const{
				return _neighborhood.begin();
			}
			
			std::deque< Neighbor>::iterator end() {
				return _neighborhood.end();
			}
			std::deque< Neighbor>::const_iterator end() const{
				return _neighborhood.end();
			}
			
			
			
		};
		
		
	}
	
}

#endif