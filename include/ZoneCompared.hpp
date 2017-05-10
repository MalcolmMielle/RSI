#ifndef RSI_ZONECOMPARED_08032017
#define RSI_ZONECOMPARED_08032017

#include <iterator>

#include "Zone.hpp"
#include "EdgeZone.hpp"
#include "bettergraph/SimpleGraph.hpp"

namespace AASS{
	namespace RSI{
		
		class GraphZone;
		
		/**
		 * @brief store the result of the comparison between two zone_similarity. Can be used without the graph structure at hand
		 */
		class ZoneComparedInterface{
			protected:
			//The closer to 0 the better!
			double _similarity;
			Zone _source_zone;
			Zone _target_zone;
			
		public:
			double zone_similarity;
			double zone_size_factor_source;
			double zone_size_factor_target;
			
			//TESTING
			double pca_diff;
			double size_diff;
			double size_source;
			double size_target;
			
			ZoneComparedInterface(const Zone& source, const Zone& target) : _source_zone(source), _target_zone(target), _similarity(-1), zone_size_factor_source(0){};
			
			const Zone& getSourceZone() const {return _source_zone;}
			const Zone& getTargetZone() const {return _target_zone;}
			Zone& getSourceZone(){return _source_zone;}
			Zone& getTargetZone(){return _target_zone;}
			
			///@brief calculate the similarity value depending on all attributes
// 			void update(){
// 				assert(zone_size_factor_source <=1 && zone_size_factor_source >=0);
// 				assert(zone_size_factor_target <=1 && zone_size_factor_target >=0);
// 				
// 				std::cout << "s " << zone_size_factor_source << std::endl;
// 				std::cout << "s " << zone_size_factor_target << std::endl;
// 				
// 				double diff = zone_size_factor_source - zone_size_factor_target;
// 				std::cout << "Diff " << diff << std::endl;
// 
// 				diff = std::abs<double>(diff);
// 				std::cout << "Diff " << diff << std::endl;
// 
// // 				diff = 1 - diff;
// 				
// 				std::cout << "Diff " << diff << std::endl;
// 				
// 				//ATTENTION : compare zone_similarity + diff in size
// 				_similarity = (zone_similarity + diff) / 2;
// 				//ATTENTION : compare diff in size
// // 				_similarity = diff;
// 			};
			
			void setSimilarity(double si){_similarity = si;}
			double getSimilarity() const {assert(_similarity != -1); return _similarity;}
			/**
			 * @brief use the uniqueness score of the zones AND the similarity score to give a goodness of matching between the two.
			 */
			double getRanking(const Zone& source, const Zone& target) const;
			
			virtual void print(){std::cout << " score " << getSimilarity() << " size source " << size_source << " size target " << size_target << " diff size " <<size_diff << " pca diff " << pca_diff ;}
		};
		
		
		
				
		/**
		 * @brief store the result of the comparison between two zone_similarity. Also as some interface for the graph in RSI
		 */
		class ZoneCompared : public ZoneComparedInterface{
			
		public:
			bettergraph::SimpleGraph<Zone, EdgeElement>::Vertex source;
			bettergraph::SimpleGraph<Zone, EdgeElement>::Vertex target;
			
			
			ZoneCompared(const bettergraph::SimpleGraph<Zone, EdgeElement>::Vertex& v, const bettergraph::SimpleGraph<Zone, EdgeElement>::Vertex& v2, const Zone& source, const Zone& target);
			
			ZoneCompared(const bettergraph::SimpleGraph<Zone, EdgeElement>::Vertex& v, const bettergraph::SimpleGraph<Zone, EdgeElement>::Vertex& v2, const GraphZone& graph);
			
			ZoneCompared(const bettergraph::SimpleGraph<Zone, EdgeElement>::Vertex& v, const bettergraph::SimpleGraph<Zone, EdgeElement>::Vertex& v2, const AASS::RSI::GraphZone& graph, const AASS::RSI::ZoneComparedInterface& in);
			
			double getRanking(const GraphZone& gsource, const GraphZone& gtarget) const;
			
			virtual void print(){std::cout << " score " << getSimilarity() << " size source " << size_source << " size target " << size_target << " diff size " <<size_diff << " pca diff " << pca_diff ;}
		};
	}
}
#endif
		