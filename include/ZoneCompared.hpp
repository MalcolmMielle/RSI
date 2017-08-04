#ifndef RSI_ZONECOMPARED_08032017
#define RSI_ZONECOMPARED_08032017

#include <iterator>

#include "ZoneRI.hpp"
#include "maoris/EdgeZone.hpp"
// #include "EdgeZone.hpp"
#include "bettergraph/SimpleGraph.hpp"

namespace AASS{
	namespace RSI{
		
		class GraphZoneRI;
		
		/**
		 * @brief store the result of the comparison between two zone_similarity. Can be used without the graph structure at hand
		 */
		class ZoneComparedInterface{
			protected:
			//The closer to 0 the better!
			double _similarity;
			ZoneRI _source_zone;
			ZoneRI _target_zone;
			
		public:
			double zone_similarity;
			double zone_size_factor_source;
			double zone_size_factor_target;
			
			//TESTING
			double pca_diff;
			double size_diff;
			double size_source;
			double size_target;
			
			ZoneComparedInterface(const ZoneRI& source, const ZoneRI& target) : _source_zone(source), _target_zone(target), _similarity(-1), zone_size_factor_source(0){};
			
			const ZoneRI& getSourceZone() const {return _source_zone;}
			const ZoneRI& getTargetZone() const {return _target_zone;}
			ZoneRI& getSourceZone(){return _source_zone;}
			ZoneRI& getTargetZone(){return _target_zone;}
			
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
			double getRanking(const ZoneRI& source, const ZoneRI& target) const;
			
			virtual void print(){std::cout << " score " << getSimilarity() << " size source " << size_source << " size target " << size_target << " diff size " <<size_diff << " pca diff " << pca_diff ;}
		};
		
		
		
				
		/**
		 * @brief store the result of the comparison between two zone_similarity. Also as some interface for the graph in RSI
		 */
		class ZoneCompared : public ZoneComparedInterface{
			
		public:
			bettergraph::SimpleGraph<ZoneRI, maoris::EdgeElement>::Vertex source;
			bettergraph::SimpleGraph<ZoneRI, maoris::EdgeElement>::Vertex target;
			
			
			ZoneCompared(const bettergraph::SimpleGraph<ZoneRI, maoris::EdgeElement>::Vertex& v, const bettergraph::SimpleGraph<ZoneRI, maoris::EdgeElement>::Vertex& v2, const ZoneRI& source, const ZoneRI& target);
			
			ZoneCompared(const bettergraph::SimpleGraph<ZoneRI, maoris::EdgeElement>::Vertex& v, const bettergraph::SimpleGraph<ZoneRI, maoris::EdgeElement>::Vertex& v2, const GraphZoneRI& graph);
			
			ZoneCompared(const bettergraph::SimpleGraph<ZoneRI, maoris::EdgeElement>::Vertex& v, const bettergraph::SimpleGraph<ZoneRI, maoris::EdgeElement>::Vertex& v2, const AASS::RSI::GraphZoneRI& graph, const AASS::RSI::ZoneComparedInterface& in);
			
			double getRanking(const GraphZoneRI& gsource, const GraphZoneRI& gtarget) const;
			
			virtual void print(){std::cout << " score " << getSimilarity() << " size source " << size_source << " size target " << size_target << " diff size " <<size_diff << " pca diff " << pca_diff ;}
		};
	}
}
#endif
		