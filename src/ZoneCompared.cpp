#include "ZoneCompared.hpp"
#include "GraphZone.hpp"

AASS::RSI::ZoneCompared::ZoneCompared(const bettergraph::SimpleGraph<Zone, EdgeElement>::Vertex& v, const bettergraph::SimpleGraph<Zone, EdgeElement>::Vertex& v2, const AASS::RSI::GraphZone& graph): ZoneComparedInterface(graph[v], graph[v2]), source(v), target(v2)
{

}


AASS::RSI::ZoneCompared::ZoneCompared(const bettergraph::SimpleGraph<Zone, EdgeElement>::Vertex& v, const bettergraph::SimpleGraph<Zone, EdgeElement>::Vertex& v2, const AASS::RSI::Zone& source, const AASS::RSI::Zone& target): ZoneComparedInterface(source, target), source(v), target(v2)
{

}		

AASS::RSI::ZoneCompared::ZoneCompared(const bettergraph::SimpleGraph<Zone, EdgeElement>::Vertex& v, const bettergraph::SimpleGraph<Zone, EdgeElement>::Vertex& v2, const AASS::RSI::GraphZone& graph, const AASS::RSI::ZoneComparedInterface& in): ZoneComparedInterface(graph[v], graph[v2]), source(v), target(v2)
{
	
	_similarity = in.getSimilarity();
	_source_zone = in.getSourceZone();
	_target_zone = in.getTargetZone();
	zone_similarity = in.zone_similarity;
	zone_size_factor_source = in. zone_size_factor_source;
	zone_size_factor_target = in. zone_size_factor_target;
	pca_diff = in.pca_diff;
	size_diff = in.size_diff;
	size_source = in.size_source;
	size_target = in.size_target;
		

}


double AASS::RSI::ZoneCompared::getRanking(const GraphZone& gsource, const GraphZone& gtarget) const{
	double uscore =  gsource[source].getUniquenessScore();
	double uscore0 = gtarget[target].getUniquenessScore();				
	return ((100 * std::min(uscore, uscore0)) + (100 - _similarity)) / 2;
}

double AASS::RSI::ZoneComparedInterface::getRanking(const Zone& source, const Zone& target) const{
	double uscore =  source.getUniquenessScore();
	double uscore0 = target.getUniquenessScore();				
	return ((100 * std::min(uscore, uscore0)) + (100 - _similarity)) / 2;
}