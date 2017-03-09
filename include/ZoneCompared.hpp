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
			double size_source;
			double size_target;
			
			ZoneCompared(const bettergraph::SimpleGraph<Zone, EdgeElement>::Vertex& v, const bettergraph::SimpleGraph<Zone, EdgeElement>::Vertex& v2) : source(v), target(v2), _similarity(-1), zone_size_factor_source(0){};
			
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
			double getRanking(const GraphZone& gsource, const GraphZone& gtarget) const;
			
			void print(){std::cout << " score " << getSimilarity() << " size source " << size_source << " size target " << size_target << " diff size " <<size_diff << " pca diff " << pca_diff ;}
		};
	}
}
#endif
		