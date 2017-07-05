#include "ZoneComparedLight.hpp"
#include "GraphZoneLight.hpp"

AASS::RSI::ZoneComparedLight::ZoneComparedLight(const bettergraph::SimpleGraph<ZoneLight, EdgeElementLight>::Vertex& v, const bettergraph::SimpleGraph<ZoneLight, EdgeElementLight>::Vertex& v2, const AASS::RSI::GraphZoneLight& graph): ZoneComparedInterfaceLight(graph[v], graph[v2]), source(v), target(v2)
{

}


AASS::RSI::ZoneComparedLight::ZoneComparedLight(const bettergraph::SimpleGraph<ZoneLight, EdgeElementLight>::Vertex& v, const bettergraph::SimpleGraph<ZoneLight, EdgeElementLight>::Vertex& v2, const AASS::RSI::ZoneLight& source, const AASS::RSI::ZoneLight& target): ZoneComparedInterfaceLight(source, target), source(v), target(v2)
{

}		

AASS::RSI::ZoneComparedLight::ZoneComparedLight(const bettergraph::SimpleGraph<ZoneLight, EdgeElementLight>::Vertex& v, const bettergraph::SimpleGraph<ZoneLight, EdgeElementLight>::Vertex& v2, const AASS::RSI::GraphZoneLight& graph, const AASS::RSI::ZoneComparedInterfaceLight& in): ZoneComparedInterfaceLight(graph[v], graph[v2]), source(v), target(v2)
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


double AASS::RSI::ZoneComparedLight::getRanking(const GraphZoneLight& gsource, const GraphZoneLight& gtarget) const{
	double uscore =  gsource[source].getUniquenessScore();
	double uscore0 = gtarget[target].getUniquenessScore();				
	return ((100 * std::min(uscore, uscore0)) + (100 - _similarity)) / 2;
}

double AASS::RSI::ZoneComparedInterfaceLight::getRanking(const ZoneLight& source, const ZoneLight& target) const{
	double uscore =  source.getUniquenessScore();
	double uscore0 = target.getUniquenessScore();				
	return ((100 * std::min(uscore, uscore0)) + (100 - _similarity)) / 2;
}