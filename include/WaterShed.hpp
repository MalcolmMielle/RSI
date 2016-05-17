#ifndef RSI_WATERSHED_17052016
#define RSI_WATERSHED_17052016

#include "FuzzyOpening.hpp"
#include "Zone.hpp"

bool sortFunction(std::pair < size_t, size_t > i, std::pair < size_t, size_t > j){return (i.second > j.second); }


//TODO convert Mat to Eigen !

class Watershed{
	
protected :
	FuzzyOpening _fuzzyOpening;
	std::vector < std::pair < size_t, size_t > > _index_of_zones_to_fuse_after;
	std::deque< Zone > _zones;
	
public:
	Watershed(){
		//Cheat for obstacle being 0
		Zone z; _zones.push_back(z);
	};
	
	size_t size(){return _zones.size();}
	void watershed(cv::Mat& in);
	void makeZones(cv::Mat& input);
	void fuse();
	void isolatedOrNot(int value, cv::Mat& input, cv::Mat& zones_star, int row, int col, std::vector< size_t >& zone_index);
	
	void sortBiggest2Smallest();
	void print(){
		std::cout << "Print watershed" << std::endl;
		
		for(size_t i = 0 ; i < _zones.size() ; ++i){
			std::cout << "Zone " << i << " of size " << _zones[i].size() << " and value " << _zones[i].getValue() << std::endl;
// 			for(size_t j = 0 ; j < _zones[i].size() ; ++j){
// 				std::cout << " x : " << _zones[i][j].x << " y : " << _zones[i][j].y << std::endl;
// 			}
		}
		
		std::cout << "Number of zones " << _zones.size() << std::endl;
		
	}
	
	void printZone(int i){
		for(size_t j = 0 ; j < _zones[i].size() ; ++j){
			std::cout << " x : " << _zones[i][j].x << " y : " << _zones[i][j].y << std::endl;
		}
	}
	
	void draw(cv::Mat& draw, int size){
// 		std::cout << "_zone size " << _zones.size() << std::endl;
		
		for(size_t i = 0 ; i < _zones.size() ; ++i){
			if(_zones[i].size() > size){
				cv::Mat draw_tmp = cv::Mat::zeros(draw.rows, draw.cols, CV_8U);
				for(size_t j = 0 ; j < _zones[i].size() ; ++j){
					draw_tmp.at<uchar>(_zones[i][j].x, _zones[i][j].y) = 255;
				}
				cv::imshow("Zones " , draw_tmp);
				cv::waitKey(0);
				
			}
		
		}
	
	}
	
	void slowdraw(cv::Mat& draw, int size){
		std::cout << "_zone size " << _zones.size() << std::endl;
		cv::Mat draw_tmp = cv::Mat::zeros(draw.rows, draw.cols, CV_8U);
		for(size_t i = 0 ; i < _zones.size() ; ++i){
			if ( i > 1){
				
				if(_zones[i].size() > size){
					
					for(size_t j = 0 ; j < _zones[i].size() ; ++j){
						draw_tmp.at<uchar>(_zones[i][j].x, _zones[i][j].y) = 255;
					}
// 					std::cout << "VALUE " << i << std::endl;
					
				}
				
			}
		}
		if(_zones.size() >1){
			cv::imshow("Zones " , draw_tmp);
			cv::waitKey(1);
		}
	}
};

inline void Watershed::watershed(cv::Mat& in)
{
	//Make zones
	
	makeZones(in);
	
	//Fuse zones with close value
	fuse(); 
	
	std::cout << "Number of zones " << _zones.size() << std::endl;
	//Prune small zones
	
	//Return zones and links

}

/*Two way : I can either swype one way creating zone
* OR
* I can for every value only keep a certain value -> FloodFill all spaces while marking the pixel that disappeared as a zone
*/
inline void Watershed::makeZones(cv::Mat& input)
{
	std::cout << "Make zone : " << std::endl;
	cv::Mat in;
	input.convertTo(in, CV_32SC1);
	// Mat with 0 when pixel has not been seen and a nuumber when it belong to a zone
	cv::Mat zones_star = cv::Mat::zeros(in.rows, in.cols, in.depth());
	
	std::cout << "ZONE : " << type2str(zones_star.type()) << std::endl;
// 	exit(0);
	
// 	std::cout << "Make zone : " <<in.rows << " " << in.cols << std::endl;
	
	_index_of_zones_to_fuse_after.clear();
	
// 	std::cout << in << std::endl;
// 	exit(0);
	// Swype in one way and add pixel to zone 
	int step = 0 ;
	for(int row = 0 ; row < in.rows ; row++){
		uint32_t* p = in.ptr<uint32_t>(row); //point to each row
		uint32_t* p_zone_star = zones_star.ptr<uint32_t>(row); //point to each row
		for(int col = 0 ; col < in.cols ; col++){
// 			slowdraw(in, 0);
// 			std::cout << "Step : " << step << " row and col " << row << " " << col << std::endl;
			++step;
			//It's not a wall
			if(p[col] != 0){
				//p[j] <- how to access element
// 						std::cout << (int)p[col]<< std::endl;
				std::vector<size_t> zone_index;
				isolatedOrNot(p[col], in, zones_star, row, col, zone_index);
				
// 				std::cout << "Zone index size " << zone_index.size() << std::endl;
				//New zone
				if(row == 9 && col == 33){
					std::cout <<"NOW for value "<< (int)p[col] <<std::endl;
				}
				if(zone_index.size() == 0){
					if(row == 9 && col == 33){
						std::cout <<"Newzone : "<< _zones.size() <<std::endl;
					}
					Zone new_zone;
// 					std::cout << (int)p[col]<< std::endl;
					new_zone.setValue(p[col]);
					new_zone.push_back(cv::Point2i(row, col));
					_zones.push_back(new_zone);
// 					std::cout << "PUSHING " << _zones.size() << std::endl;
					p_zone_star[col] = (int)_zones.size() - 1;
// 					std::cout << "value pushed compare to size " << (int)p_zone_star[col] << std::endl;
					
				}
				else{
					if(row == 9 && col == 33){
						std::cout <<"Not new Zone"<<std::endl;
					}
					_zones[ zone_index[0] ].push_back(cv::Point2i(row, col));
					p_zone_star[col] = zone_index [0];
					
					if(_zones[ zone_index[0] ].getValue() != p[col]){
						throw std::runtime_error("Different value between the place and the zone it's added to");
					}
					
					//If more than one zone push the fusion later if it doesn't already exist
					if(zone_index.size() > 1){
						bool flag_exist = false;
						for(size_t i = 1 ; i < zone_index.size() ; ++i){
							
							for(size_t j = 0 ; j < _index_of_zones_to_fuse_after.size() ; ++j){
								if( (zone_index[0] == _index_of_zones_to_fuse_after[j].first &&\
									zone_index[i] == _index_of_zones_to_fuse_after[j].second)\
									 || (zone_index[0] == _index_of_zones_to_fuse_after[j].second &&\
									zone_index[i] == _index_of_zones_to_fuse_after[j].first )){
									flag_exist = true;
								}
							}
							if(flag_exist == false){
								size_t min = zone_index[0];
								size_t max = zone_index[i];
								if(min > zone_index[i]){
									min = zone_index[i];
									max = zone_index[0];
								}
								
								_index_of_zones_to_fuse_after.push_back(std::pair<size_t, size_t>(min, max) );
							}
						}
						
					}
					
// 					std::cout << zones_star << std::endl;
				}
				
				
			}
			else{
// 				std::cout << "WALL"<<std::endl;
				p_zone_star[col] = 0;
				_zones[0 ].push_back(cv::Point2i(row, col));
			}
// 			if(row == 9 && col == 33){
// 				exit(0);
// 			}
		}
		
	}
	
// 	std::cout << "TO FUSE " << _index_of_zones_to_fuse_after.size() << std::endl;

}


inline void Watershed::isolatedOrNot(int value, cv::Mat& input, cv::Mat& zones_star, int row, int col, std::vector<size_t>& zone_index)
{
// 	std::cout << zones_star << std::endl;
// 	std::cout << "Value " << value << " ";
// 	std::cout << "row col " << row << " " << col <<  " " ;
	for(int row_tmp = row - 1 ; row_tmp <= row + 1 ; ++row_tmp){
		for(int col_tmp = col - 1 ; col_tmp <= col + 1 ; ++col_tmp){
			//Inside mat
// 			std::cout << "row_t col_t " << row_tmp << " " << col_tmp <<  " " ;
			if(row_tmp >= 0 && col_tmp >=0 && row_tmp <= input.rows && col_tmp <= input.cols){
				//Not the center point
				
// 				std::cout << "YES " ;
				if(row != row_tmp || col != col_tmp){
					uint32_t* p = input.ptr<uint32_t>(row_tmp);
					uint32_t* p_star = zones_star.ptr<uint32_t>(row_tmp);
					//Same value and visited before
// 					std::cout << "Visited before " << (int)p_star[col_tmp] << std::endl;
					if(value == p[col_tmp] && p_star[col_tmp] > 0){
						
// 						std::cout << "Same for zone? " << (int)p_star[col_tmp] << " -> " << _zones[p_star[col_tmp]].getValue() <<"  " << value << " value in mat " <<  (int)p[col_tmp] << std::endl;
						if(_zones[p_star[col_tmp]].getValue() != value){
// 							std::cout << input << std::endl;
							printZone(p_star[col_tmp]);
							std::cout << "At : row col " << row_tmp << " " << col_tmp << std::endl;
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
				}
			}
		}
	}
	
// 	std::cout << "Found " << zone_index.size() << " neighb" << std::endl;
// 	if(zone_index.size() > 1){
// 		exit(0);
// 	}
}


inline void Watershed::fuse()
{
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
// 	exit(0);
	
	
	
	std::map<size_t, size_t> mapping;
	
	for(size_t i = 0 ; i < _index_of_zones_to_fuse_after.size() ; ++i){
// 		
		int base;
		int to_fuse;
// 		std::cout << std::endl;
		//Search the value in the mapping and update it
		if ( mapping.find(_index_of_zones_to_fuse_after[i].first) == mapping.end() ) {
		// not found
// 			std::cout << "not FOUND base " << _index_of_zones_to_fuse_after[i].first << std::endl;
			base = _index_of_zones_to_fuse_after[i].first;
		} else {
		// found
// 			std::cout << "FOUND base " << _index_of_zones_to_fuse_after[i].first << std::endl;
			base = mapping[_index_of_zones_to_fuse_after[i].first];
// 			std::cout << "Now base is " << base << std::endl;
		}
		if ( mapping.find(_index_of_zones_to_fuse_after[i].second) == mapping.end() ) {
		// not found
// 			std::cout << "not FOUND fuse " << _index_of_zones_to_fuse_after[i].second << std::endl;
			to_fuse = _index_of_zones_to_fuse_after[i].second;
			
// 			mapping[_index_of_zones_to_fuse_after[i].second] = base;
		} else {
		// found
// 			std::cout << "FOUND fuse " << _index_of_zones_to_fuse_after[i].second << std::endl;
			to_fuse = mapping[_index_of_zones_to_fuse_after[i].second];
// 			std::cout << " now fuse " << to_fuse << std::endl;
// 			mapping[to_fuse] = base;
			
		}
		
		//Always copy to the smalles number
		int max = to_fuse;
		int min = base;
		if(max < min){
			max = base;
			min = to_fuse;
			mapping[max] = min;
			mapping[_index_of_zones_to_fuse_after[i].first] = min;
		}else{
			mapping[max] = min;
			mapping[_index_of_zones_to_fuse_after[i].second] = min;
		}
		
		
		
		if(_zones[base].getValue() != _zones[to_fuse].getValue() || base == to_fuse){
			std::ostringstream str_test;
			str_test <<  "Fusing zone that shouldn't be fused at line " << __LINE__ << " in file " << __FILE__ << "." << std::endl;
			throw std::runtime_error(str_test.str() );	
		}
		
		for(size_t j = 0 ; j < _zones[max].size() ; ++j){
// 			std::cout << " j : " << j << " with size " << _zones[to_fuse].size() << std::endl;
			_zones[min].push_back(_zones[max][j]);
		}
		
		
	}
	
	std::map<size_t, size_t>::reverse_iterator iter;
// 	std::cout << "PRINTTT" << std::endl;
	for (iter = mapping.rbegin(); iter != mapping.rend(); ++iter) {
// 		std::cout << iter->first << " " << iter->second << "\n";
		_zones.erase(_zones.begin() + iter->first);
	}
	
// 	for(size_t i = 0 ; i < _index_of_zones_to_fuse_after.size() ; ++i){
// 		int to_fuse = _index_of_zones_to_fuse_after[i].second;
// 		_zones.erase(_zones.begin() + to_fuse);
// 	}
	
}





#endif