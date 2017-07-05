#ifndef RSI_ZONECOMPAREDLIGHT_05072017
#define RSI_ZONECOMPAREDLIGHT_05072017

#include <iterator>

#include "ZoneLight.hpp"
#include "EdgeZoneLight.hpp"
#include "bettergraph/SimpleGraph.hpp"

namespace AASS{
	namespace RSI{
		
		class GraphZoneLight;
		
		/**
		 * @brief store the result of the comparison between two zone_similarity. Can be used without the graph structure at hand
		 */
		class ZoneComparedInterfaceLight{
			protected:
			//The closer to 0 the better!
			double _similarity;
			ZoneLight _source_zone;
			ZoneLight _target_zone;
			
		public:
			double zone_similarity;
			double zone_size_factor_source;
			double zone_size_factor_target;
			
			//TESTING
			double pca_diff;
			double size_diff;
			double size_source;
			double size_target;
			
			ZoneComparedInterfaceLight(const ZoneLight& source, const ZoneLight& target) : _source_zone(source), _target_zone(target), _similarity(-1), zone_size_factor_source(0){};
			
			const ZoneLight& getSourceZone() const {return _source_zone;}
			const ZoneLight& getTargetZone() const {return _target_zone;}
			ZoneLight& getSourceZone(){return _source_zone;}
			ZoneLight& getTargetZone(){return _target_zone;}
			
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
			double getRanking(const ZoneLight& source, const ZoneLight& target) const;
			
			virtual void print(){std::cout << " score " << getSimilarity() << " size source " << size_source << " size target " << size_target << " diff size " <<size_diff << " pca diff " << pca_diff ;}
		};
		
		
		
				
		/**
		 * @brief store the result of the comparison between two zone_similarity. Also as some interface for the graph in RSI
		 */
		class ZoneComparedLight : public ZoneComparedInterfaceLight{
			
		public:
			bettergraph::SimpleGraph<ZoneLight, EdgeElementLight>::Vertex source;
			bettergraph::SimpleGraph<ZoneLight, EdgeElementLight>::Vertex target;
			
			
			ZoneComparedLight(const bettergraph::SimpleGraph<ZoneLight, EdgeElementLight>::Vertex& v, const bettergraph::SimpleGraph<ZoneLight, EdgeElementLight>::Vertex& v2, const ZoneLight& source, const ZoneLight& target);
			
			ZoneComparedLight(const bettergraph::SimpleGraph<ZoneLight, EdgeElementLight>::Vertex& v, const bettergraph::SimpleGraph<ZoneLight, EdgeElementLight>::Vertex& v2, const GraphZoneLight& graph);
			
			ZoneComparedLight(const bettergraph::SimpleGraph<ZoneLight, EdgeElementLight>::Vertex& v, const bettergraph::SimpleGraph<ZoneLight, EdgeElementLight>::Vertex& v2, const AASS::RSI::GraphZoneLight& graph, const AASS::RSI::ZoneComparedInterfaceLight& in);
			
			double getRanking(const GraphZoneLight& gsource, const GraphZoneLight& gtarget) const;
			
			virtual void print(){std::cout << " score " << getSimilarity() << " size source " << size_source << " size target " << size_target << " diff size " <<size_diff << " pca diff " << pca_diff ;}
		};
	}
}
#endif
		