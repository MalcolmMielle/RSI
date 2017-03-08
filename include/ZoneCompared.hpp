#ifndef RSI_ZONECOMPARED_08032017
#define RSI_ZONECOMPARED_08032017

#include <iterator>

#include "Zone.hpp"
#include "EdgeZone.hpp"
#include "bettergraph/SimpleGraph.hpp"

namespace AASS{
	namespace RSI{
		
		class GraphZone;
				
		class ZoneCompared{
			
		protected:
			//The closer to 0 the better!
			double _similarity;
			
		public:
			bettergraph::SimpleGraph<Zone, EdgeElement>::Vertex source;
			bettergraph::SimpleGraph<Zone, EdgeElement>::Vertex target;
			double zone_similarity;
			double zone_size_factor_source;
			double zone_size_factor_target;
			
			//TESTING
			double pca_diff;
			double size_diff;
			
			ZoneCompared(const bettergraph::SimpleGraph<Zone, EdgeElement>::Vertex& v, const bettergraph::SimpleGraph<Zone, EdgeElement>::Vertex& v2, double simi) : source(v), target(v2), _similarity(simi), zone_size_factor_source(0){};
			
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
			
			double getSimilarity() const {assert(zone_size_factor_source != -1); return _similarity;}
			double getRanking(const GraphZone& gsource, const GraphZone& gtarget) const;
		};
	}
}
#endif
		