#include "GraphZone.hpp"


int main(){
	
	AASS::RSI::GraphZone gz;
	AASS::RSI::Zone zo(cv::Size(10, 10));
	cv::Point2i p; p.x = 1; p.y = 2;
	zo.push_back(p);
	
	AASS::RSI::GraphZone::VertexZone vert;
	gz.addVertex(vert, zo);
	AASS::RSI::Zone zo1(cv::Size(10, 10));
	cv::Point2i p1; p.x = 1; p.y = 2;
	zo.push_back(p1);
	AASS::RSI::GraphZone::VertexZone vert1;
	gz.addVertex(vert1, zo1);
	
	zo1.fuse(zo);
// 	std::cout << (*this)[v] <<std::endl;
	gz.removeVertex(vert);
	zo1.PCA();
	
}