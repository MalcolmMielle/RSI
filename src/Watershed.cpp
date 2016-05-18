#include "WaterShed.hpp"

void AASS::RSI::Watershed::watershed(cv::Mat& in)
{
	//Make zones
	
	makeZones(in);
	
	//Fuse zones with close value
	fuse(); 
	
	std::cout << "Number of zones " << _zones.size() << std::endl;
	//Prune small zones
	
	//Return zones and links
	
	createGraph();

}

/*Two way : I can either swype one way creating zone
* OR
* I can for every value only keep a certain value -> FloodFill all spaces while marking the pixel that disappeared as a zone
*/
void AASS::RSI::Watershed::makeZones(cv::Mat& input)
{
	std::cout << "Make zone : " << std::endl;
	cv::Mat in;
	input.convertTo(in, CV_32SC1);
	// Mat with 0 when pixel has not been seen or is a wall and a number when it belong to a zone
	cv::Mat zones_star = cv::Mat::zeros(in.rows, in.cols, in.depth());
	
	_index_of_zones_to_fuse_after.clear();
	_graph.clear();

	// Swype in one way and add pixel to zones
	int step = 0 ;
	for(int row = 0 ; row < in.rows ; row++){
		uint32_t* p = in.ptr<uint32_t>(row); //point to each row
		uint32_t* p_zone_star = zones_star.ptr<uint32_t>(row); //point to each row
		for(int col = 0 ; col < in.cols ; col++){
// 			slowdraw(in, 0);
// 			std::cout << "Step : " << step << " row and col " << row << " " << col << std::endl;
			++step;
			
			//If the pixel is not a wall
			
			if(p[col] != 0){
				std::vector<size_t> zone_index;
				std::vector<size_t> zone_edges;
				
				isolatedOrNot(p[col], in, zones_star, row, col, zone_index, zone_edges);
				
				//New zone since the pixel is connected to no  already seen + same value pixel
				if(zone_index.size() == 0){
					Zone new_zone;
					new_zone.setValue(p[col]);
					new_zone.push_back(cv::Point2i(row, col));
					_zones.push_back(new_zone);
					p_zone_star[col] = (int)_zones.size() - 1;					
				}
				//If the pixel is part of (an) already seen zone(s)
				else{
					_zones[ zone_index[0] ].push_back(cv::Point2i(row, col));
					p_zone_star[col] = zone_index [0];
					
					if(_zones[ zone_index[0] ].getValue() != p[col]){
						throw std::runtime_error("Different value between the place and the zone it's added to");
					}
					
					//If more than one zone push the value of other zones to _index_of_zones_to_fuse_after to fuse them later
					if(zone_index.size() > 1){
						bool flag_exist = false;
						for(size_t i = 1 ; i < zone_index.size() ; ++i){
							
							//Sort them by index value
							size_t min = zone_index[0];
							size_t max = zone_index[i];
							if(min > zone_index[i]){
								min = zone_index[i];
								max = zone_index[0];
							}
							
							//Make sure the pair does not already exist
							for(size_t j = 0 ; j < _index_of_zones_to_fuse_after.size() ; ++j){
								if( (min == _index_of_zones_to_fuse_after[j].first &&\
									max == _index_of_zones_to_fuse_after[j].second) ){
									flag_exist = true;
								}
							}
							if(flag_exist == false){
								_index_of_zones_to_fuse_after.push_back(std::pair<size_t, size_t>(min, max) );
							}
						}
						
					}
					
					//Adding edges if needs be done
					if(zone_edges.size() > 1){
						bool flag_exist = false;
						for(size_t i = 0 ; i < zone_edges.size() ; ++i){
							
							//Sort them by index value
							size_t min = p_zone_star[col];
							size_t max = zone_edges[i];
							if(min > zone_edges[i]){
								min = zone_edges[i];
								max = p_zone_star[col];
							}
							if(min == max){
								throw std::runtime_error("linking zone to itself !");
							}
							
							//Make sure the pair does not already exist
							for(size_t j = 0 ; j < _index_of_edges.size() ; ++j){
								if( (min == _index_of_edges[j].first &&\
									max == _index_of_edges[j].second) ){
									flag_exist = true;
								}
							}
							if(flag_exist == false){
								_index_of_edges.push_back(std::pair<size_t, size_t>(min, max) );
							}
						}
						
					}
					
					
				}
				
				
			}
			else{
				p_zone_star[col] = 0;
				_zones[0 ].push_back(cv::Point2i(row, col));
			}
		}
		
	}
	
}

void AASS::RSI::Watershed::isolatedOrNot(int value, cv::Mat& input, cv::Mat& zones_star, int row, int col, std::vector<size_t>& zone_index, std::vector< size_t >& zone_edges)
{

	if(zones_star.size() != input.size()){
		throw std::runtime_error("Zone start and input of different sizes");
	}
	
	for(int row_tmp = row - 1 ; row_tmp <= row + 1 ; ++row_tmp){
		for(int col_tmp = col - 1 ; col_tmp <= col + 1 ; ++col_tmp){
			//Inside mat
			if(row_tmp >= 0 && col_tmp >=0 && row_tmp < input.rows && col_tmp < input.cols){
				//Not the center point
				
				if(row != row_tmp || col != col_tmp){
					uint32_t* p = input.ptr<uint32_t>(row_tmp);
					uint32_t* p_star = zones_star.ptr<uint32_t>(row_tmp);
					//Same value and visited before
					if(value == p[col_tmp] && p_star[col_tmp] > 0){
						
						if(_zones[p_star[col_tmp]].getValue() != value){
							printZone(p_star[col_tmp]);
							std::cout << "Size of matrix : " << zones_star.size() << std::endl;
							std::cout << "At : row col " << row_tmp << " " << col_tmp << " we should not have " << _zones[p_star[col_tmp]].getValue() << " != " << value << std::endl;
							throw std::runtime_error("Value and zone mismatch");
						}
						//SAME ZONE
						bool flag_seen = false;
						for(size_t i = 0 ; i < zone_index.size() ; ++i){
							if(zone_index[i] == p_star[col_tmp]){
								flag_seen = true;
							}
						}
						if(flag_seen == false){
							zone_index.push_back(p_star[col_tmp]);
						}
					}
					//Check for other value bordering
					if(value != p[col_tmp] && p_star[col_tmp] > 0){
						
// 						std::cout << "Was ? " << value << " != " << _zones[p_star[col_tmp]].getValue() << " " << p[col_tmp] << std::endl << "zone : " << p_star[col_tmp] << std::endl;
						
						if(_zones[p_star[col_tmp]].getValue() == value){
// 							printZone(p_star[col_tmp]);
							std::cout << "At : row col " << row_tmp << " " << col_tmp << std::endl;
							throw std::runtime_error("Value and zone are the same in edges");
						}
						//SAME ZONE
						bool flag_seen = false;
						for(size_t i = 0 ; i < zone_edges.size() ; ++i){
							if(zone_edges[i] == p_star[col_tmp]){
								flag_seen = true;
							}
						}
						if(flag_seen == false){
							zone_edges.push_back(p_star[col_tmp]);
						}
					}
				}
			}
		}
	}
}

void AASS::RSI::Watershed::fuse()
{
	
	initMappingAlive();
	
	std::cout << "FUSE" << std::endl;
	
	//Sort the index backward from higest value for second so that we erase the zone from the back without changing the indexes.
	std::sort(_index_of_zones_to_fuse_after.begin(), _index_of_zones_to_fuse_after.end(), sortFunction);
	
// 	for(size_t i = 0 ; i < _zones.size() ; ++i){
// 		std::cout << "Zone " << i << " of size " << _zones[i].size() << std::endl;
// 	}
// 	for(size_t i = 0 ; i < _index_of_zones_to_fuse_after.size() ; ++i){
// 		int base = _index_of_zones_to_fuse_after[i].first;
// 		int to_fuse = _index_of_zones_to_fuse_after[i].second;
// 		std::cout << "Zone to fuse " << _index_of_zones_to_fuse_after[i].first << " " << _index_of_zones_to_fuse_after[i].second << " with values " << _zones[base].getValue() << " " << _zones[to_fuse].getValue() << std::endl;
// 	}
	
	_mapping.clear();	
	//Create the full mapping
	for(size_t i = 0 ; i < _index_of_zones_to_fuse_after.size() ; ++i){
// 		
		int base;
		int to_fuse;
		//Search the value in the mapping and update it
		if ( _mapping.find(_index_of_zones_to_fuse_after[i].first) == _mapping.end() ) {
		// not found
// 			std::cout << "not FOUND base " << _index_of_zones_to_fuse_after[i].first << std::endl;
			base = _index_of_zones_to_fuse_after[i].first;
		} else {
		// found
// 			std::cout << "FOUND base " << _index_of_zones_to_fuse_after[i].first << std::endl;
			base = _mapping[_index_of_zones_to_fuse_after[i].first];
// 			std::cout << "Now base is " << base << std::endl;
		}
		if ( _mapping.find(_index_of_zones_to_fuse_after[i].second) == _mapping.end() ) {
		// not found
// 			std::cout << "not FOUND fuse " << _index_of_zones_to_fuse_after[i].second << std::endl;
			to_fuse = _index_of_zones_to_fuse_after[i].second;
			
// 			mapping[_index_of_zones_to_fuse_after[i].second] = base;
		} else {
		// found
// 			std::cout << "FOUND fuse " << _index_of_zones_to_fuse_after[i].second << std::endl;
			to_fuse = _mapping[_index_of_zones_to_fuse_after[i].second];
// 			std::cout << " now fuse " << to_fuse << std::endl;
// 			mapping[to_fuse] = base;
			
		}
		
		//Always copy to the smallest number
		int max = to_fuse;
		int min = base;
		if(min != max){
			
			if(max < min){
				max = base;
				min = to_fuse;
				
				updateMapping(max, min);
// 				_mapping[max] = min;
				_mapping[_index_of_zones_to_fuse_after[i].first] = min;
			}else{
				updateMapping(max, min);
				_mapping[_index_of_zones_to_fuse_after[i].second] = min;
			}
			
			if(_zones[base].getValue() != _zones[to_fuse].getValue() || base == to_fuse){
				std::cout << min << " " << max << std::endl;
				std::cout << _zones[base].getValue() << " " << _zones[to_fuse].getValue() << std::endl;
				std::ostringstream str_test;
				str_test <<  "Fusing zone that shouldn't be fused at line " << __LINE__ << " in file " << __FILE__ << "." << std::endl;
				throw std::runtime_error(str_test.str() );	
			}
		}
		else{
// 			std::cout << "Already fused" << std::endl;
		}
		
		
	}
	
	std::map<size_t, size_t>::reverse_iterator iter;
	for (iter = _mapping.rbegin(); iter != _mapping.rend(); ++iter) {
		for(size_t j = 0 ; j < _zones[iter->first].size() ; ++j){
			_zones[iter->second].push_back(_zones[ iter->first][j]);
		}
		std::cout << iter->first << " -> " << iter->second << std::endl;
		_zones.erase(_zones.begin() + iter->first );
	}
	std::sort(_zones.begin(), _zones.end(), sortZone);
	
}

void AASS::RSI::Watershed::createGraph(){
	
	std::cout << "Create graph " << std::endl;
	std::map<size_t, int>::iterator iter;
	for (iter = _mapping_of_node_alive.begin(); iter != _mapping_of_node_alive.end(); ++iter) {
		std::cout << iter->first << " -> " << iter->second << std::endl;
	}
	
	std::cout << "Number of edges " << _index_of_edges.size() << std::endl;
	
	std::vector <Vertex> vertices_zones;
	
	//Adding all vertices
	for(size_t i = 0 ; i < _zones.size() ; ++i){
		std::cout << "Adding a vertex " << i << std::endl;
		Vertex v;
		_graph.addVertex(v, _zones[i]);
		vertices_zones.push_back(v);
	}
	
	std::cout << "adding the edges" << std::endl;
	
	//Adding all edge
	for(size_t i = 0 ; i < _index_of_edges.size() ; ++i){
		std::cout << " edge : "<< _index_of_edges[i].first << " " << _index_of_edges[i].second << std::endl;
		int base;
		int destination;
		//Search the value in the mapping and update it
		if ( _mapping.find(_index_of_edges[i].first) == _mapping.end() ) {
		// not found
// 			std::cout << "not FOUND base " << _index_of_zones_to_fuse_after[i].first << std::endl;
			base = _index_of_edges[i].first;
		} else {
		// found
// 			std::cout << "FOUND base " << _index_of_zones_to_fuse_after[i].first << std::endl;
			base = _mapping[_index_of_edges[i].first];
// 			std::cout << "Now base is " << base << std::endl;
		}
		if ( _mapping.find(_index_of_edges[i].second) == _mapping.end() ) {
		// not found
// 			std::cout << "not FOUND fuse " << _index_of_zones_to_fuse_after[i].second << std::endl;
			destination = _index_of_edges[i].second;
			
// 			mapping[_index_of_zones_to_fuse_after[i].second] = base;
		} else {
		// found
// 			std::cout << "FOUND fuse " << _index_of_zones_to_fuse_after[i].second << std::endl;
			destination = _mapping[_index_of_edges[i].second];
// 			std::cout << " now fuse " << to_fuse << std::endl;
// 			mapping[to_fuse] = base;
		}
		Edge e;
		std::cout << "actual edge : "<< base << " " << destination << " " << _mapping[24] << std::endl;
		if ( _mapping_of_node_alive[base] > 0) {
		// found
// 			std::cout << "not FOUND base " << _index_of_zones_to_fuse_after[i].first << std::endl;
			base =  _mapping_of_node_alive[base];
		}
		if ( _mapping_of_node_alive[destination] > 0) {
		// found
// 			std::cout << "not FOUND base " << _index_of_zones_to_fuse_after[i].first << std::endl;
			destination =  _mapping_of_node_alive[destination];
		}
		
		std::cout << "actual actual edge : "<< base << " " << destination << " " << _mapping[24] << std::endl;
		_graph.addEdge(e, vertices_zones[ base ], vertices_zones[ destination ]);
	}
	
	std::cout << "Graph vertices : " << _graph.getNumVertices() << " egdes : " << _graph.getNumEdges() << std::endl;
	
	
}
