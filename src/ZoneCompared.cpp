#include "ZoneCompared.hpp"
#include "GraphZone.hpp"

double AASS::RSI::ZoneCompared::getRanking(const GraphZone& gsource, const GraphZone& gtarget) const{
	double uscore =  gsource[source].getUniquenessScore();
	double uscore0 = gtarget[target].getUniquenessScore();				
	return ((100 * std::min(uscore, uscore0)) + (100 - _similarity)) / 2;
}