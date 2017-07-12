#include "GraphZone.hpp"
#include "ZoneCompared.hpp"

void AASS::RSI::GraphZone::drawPartial(cv::Mat& drawmat) const
{
	cv::Mat drawmat_old;
	drawmat.copyTo(drawmat_old);
	
	cv::Scalar color;
	cv::RNG rng(12345);
	std::pair<VertexIteratorZone, VertexIteratorZone> vp;
	//vertices access all the vertix
	for (vp = boost::vertices((*this)); vp.first != vp.second; ++vp.first) {
		std::cout << "VERTEX" << std::endl;
		if(drawmat.channels() == 1){
			color = rng.uniform(50, 255);
		}
		else if(drawmat.channels() == 3){
			color[0] = rng.uniform(50, 255);
			color[1] = rng.uniform(50, 255);
			color[2] = rng.uniform(50, 255);
		}
		
		VertexZone v = *vp.first;
		
		if((*this)[v].getZone().size() > 0){
// 				if(getNumEdges(v) > 1){
			
			cv::Mat copy = cv::Mat::zeros(drawmat_old.rows, drawmat_old.cols, CV_8U);
			for(size_t j = 0 ; j < (*this)[v].getZone().size() ; ++j){
				copy.at<uchar>((*this)[v].getZone()[j].x, (*this)[v].getZone()[j].y) = (*this)[v].getValue();
			}
			draw(drawmat, v, color);
			draw(copy, v, color);
			
			EdgeIteratorZone out_i, out_end;
			EdgeZone e;
			
			for (boost::tie(out_i, out_end) = boost::out_edges(v, (*this)); 
				out_i != out_end; ++out_i) {
				e = *out_i;
				VertexZone src = boost::source(e, (*this)), targ = boost::target(e, (*this));
				if( (*this)[targ].getZone().size() > 100 ){
					cv::line(drawmat, (*this)[src].getCentroid(), (*this)[targ].getCentroid(), color);
					cv::line(copy, (*this)[src].getCentroid(), (*this)[targ].getCentroid(), color);
					for(size_t j = 0 ; j < (*this)[targ].getZone().size() ; ++j){
						copy.at<uchar>((*this)[targ].getZone()[j].x, (*this)[targ].getZone()[j].y) = (*this)[targ].getValue();
					}
				}
			}
			
			cv::imshow("Z", (*this)[v].getZoneMat());
			cv::imshow("Partial" , copy);
			cv::waitKey(0);
		}
		
	}

}

void AASS::RSI::GraphZone::drawEvaluation(cv::Mat& drawmat) const
{
	cv::Mat drawmat_old;
	drawmat.convertTo(drawmat_old, CV_8U);
	
	cv::Scalar color;
	int count = 1;
	std::pair<VertexIteratorZone, VertexIteratorZone> vp;
	//vertices access all the vertix
	for (vp = boost::vertices((*this)); vp.first != vp.second; ++vp.first) {
		
		if(drawmat.channels() == 1){
			color = count;
		}
		else if(drawmat.channels() == 3){
			color[0] = count;
			color[1] = count;
			color[2] = count;
		}
		count++;
		
		VertexZone v = *vp.first;
		
// 		if((*this)[v].getZone().size() > 100){
// 				if(getNumEdges(v) > 1){
			
		drawEvaluation(drawmat, v, color);
	}
}


void AASS::RSI::GraphZone::drawSimple(cv::Mat& drawmat) const
{
	cv::Mat drawmat_old;
	drawmat.convertTo(drawmat_old, CV_8U);
	
	cv::Scalar color;
	cv::RNG rng(12345);
	int nb_zones = this->getNumVertices();
	int color_step = 249 / nb_zones;
	int count = 1;
	std::pair<VertexIteratorZone, VertexIteratorZone> vp;
	//vertices access all the vertix
	for (vp = boost::vertices((*this)); vp.first != vp.second; ++vp.first) {
		
		if(drawmat.channels() == 1){
			color = color_step * count;
		}
		else if(drawmat.channels() == 3){
			color[0] = color_step * count;
			color[1] = color_step * count;
			color[2] = color_step * count;
		}
		count++;
		
		VertexZone v = *vp.first;
		
// 		if((*this)[v].getZone().size() > 100){
// 				if(getNumEdges(v) > 1){
			
		drawSimple(drawmat, v, color);
		
	}
		

}


void AASS::RSI::GraphZone::draw(cv::Mat& drawmat) const
{
	
	cv::Mat drawmat_old;
	drawmat.convertTo(drawmat_old, CV_8U);
	
	cv::Scalar color;
	cv::RNG rng(12345);
	int nb_zones = this->getNumVertices();
	int color_step = 249 / nb_zones;
	int count = 1;
	std::pair<VertexIteratorZone, VertexIteratorZone> vp;
	//vertices access all the vertix
	for (vp = boost::vertices((*this)); vp.first != vp.second; ++vp.first) {
		
		if(drawmat.channels() == 1){
			color = color_step * count;
		}
		else if(drawmat.channels() == 3){
			color[0] = color_step * count;
			color[1] = color_step * count;
			color[2] = color_step * count;
		}
		count++;
		
		VertexZone v = *vp.first;
		
// 		if((*this)[v].getZone().size() > 100){
// 				if(getNumEdges(v) > 1){
			
		draw(drawmat, v, color);
		
		EdgeIteratorZone out_i, out_end;
		EdgeZone e;
		
		for (boost::tie(out_i, out_end) = boost::out_edges(v, (*this)); 
			out_i != out_end; ++out_i) {
			e = *out_i;
			VertexZone src = boost::source(e, (*this)), targ = boost::target(e, (*this));
			if( (*this)[targ].getZone().size() > 100 ){
// 					cv::line(drawmat, (*this)[src].getCentroid(), (*this)[targ].getCentroid(), cv::Scalar(255));
			}
			if( (*this)[e].canRemove() == false ){
				cv::line(drawmat, (*this)[src].getCentroid(), (*this)[targ].getCentroid(), cv::Scalar(255), 2);
			}
			else{
				cv::line(drawmat, (*this)[src].getCentroid(), (*this)[targ].getCentroid(), cv::Scalar(150));
			}
		}
	}
		
	for (vp = boost::vertices((*this)); vp.first != vp.second; ++vp.first) {

		VertexZone v = *vp.first;
		EdgeIteratorZone out_i, out_end;
		EdgeZone e;
		(*this)[v].printLabel(drawmat);
		
	}

}

void AASS::RSI::GraphZone::drawUnique(cv::Mat& drawmat) const
{
	
	cv::Mat drawmat_old;
	drawmat.convertTo(drawmat_old, CV_8U);
	
	cv::Scalar color;
	cv::RNG rng(12345);
	std::pair<VertexIteratorZone, VertexIteratorZone> vp;
	//vertices access all the vertix
	for (vp = boost::vertices((*this)); vp.first != vp.second; ++vp.first) {
		
		if(drawmat.channels() == 1){
			color = rng.uniform(0, 230);
		}
		else if(drawmat.channels() == 3){
			color[0] = rng.uniform(0, 230);
			color[1] = rng.uniform(0, 230);
			color[2] = rng.uniform(0, 230);
		}
		
		VertexZone v = *vp.first;
		
		if((*this)[v].isUnique()){
// 				if(getNumEdges(v) > 1){
			
			draw(drawmat, v, color);
			
			EdgeIteratorZone out_i, out_end;
			EdgeZone e;
			
			for (boost::tie(out_i, out_end) = boost::out_edges(v, (*this)); 
				out_i != out_end; ++out_i) {
				e = *out_i;
				VertexZone src = boost::source(e, (*this)), targ = boost::target(e, (*this));
				if( (*this)[targ].getZone().size() > 100 ){
// 					cv::line(drawmat, (*this)[src].getCentroid(), (*this)[targ].getCentroid(), cv::Scalar(255));
				}
			}
// 
		}
	}
		
	for (vp = boost::vertices((*this)); vp.first != vp.second; ++vp.first) {
		
		VertexZone v = *vp.first;
		if((*this)[v].isUnique()){

			EdgeIteratorZone out_i, out_end;
			EdgeZone e;
			(*this)[v].printLabel(drawmat);
			
		}
	}

}


void AASS::RSI::GraphZone::drawEvaluation(cv::Mat& m, const bettergraph::SimpleGraph<Zone, int>::Vertex& v, const cv::Scalar& color) const
{
// 		std::cout << std::endl;
// 			for(size_t i = 0 ; i < (*this)[v].landmarks.size() ; i++){
// 				cv::Point2i point;
// 				point.x = (*this)[v].landmarks[i].first.getX();
// 				point.y = (*this)[v].landmarks[i].first.getY();
// 				cv::circle(m, point, 10, color, 3);
// 			}
// 			cv::drawContours( m, std::vector<std::vector<cv::Point> >(1,(*this)[v].contour), -1, color, 2, 8);
	(*this)[v].drawZone(m, color);
// 	std::cout << "VALUIE " << std::endl;
// 	(*this)[v].printPCA();
	
// 	(*this)[v].printLabel(m);
// 	cv::circle(m, (*this)[v].getCentroid(), 2, 255, -1);
	
// 			cv::Mat draw_tmp = cv::Mat::zeros(m.rows, m.cols, CV_8U);
// 			for(size_t j = 0 ; j < (*this)[v].getZone().size() ; ++j){
// 				draw_tmp.at<uchar>((*this)[v].getZone()[j].x, (*this)[v].getZone()[j].y) = 150;
// 			}
// 			
// 			cv::circle(draw_tmp, (*this)[v].getCentroid(), 10, 255, -1);
// 			
// 			std::cout << "CENTROID " << (*this)[v].getCentroid() << "image size "<<  m.size() << std::endl;
// 			
// 			cv::imshow("Zones " , draw_tmp);
// 			cv::waitKey(0);
		
}

void AASS::RSI::GraphZone::drawSimple(cv::Mat& m, const bettergraph::SimpleGraph<Zone, int>::Vertex& v, const cv::Scalar& color) const
{
	(*this)[v].drawZone(m, color);
	(*this)[v].drawContour(m, color);
}


void AASS::RSI::GraphZone::draw(cv::Mat& m, const bettergraph::SimpleGraph<Zone, int>::Vertex& v, const cv::Scalar& color) const
{
// 		std::cout << std::endl;
// 			for(size_t i = 0 ; i < (*this)[v].landmarks.size() ; i++){
// 				cv::Point2i point;
// 				point.x = (*this)[v].landmarks[i].first.getX();
// 				point.y = (*this)[v].landmarks[i].first.getY();
// 				cv::circle(m, point, 10, color, 3);
// 			}
// 			cv::drawContours( m, std::vector<std::vector<cv::Point> >(1,(*this)[v].contour), -1, color, 2, 8);
	(*this)[v].drawZone(m, color);
	(*this)[v].drawContour(m, color);
// 	(*this)[v].drawPCA(m, color);
// 	(*this)[v].printPCA();
	
// 	(*this)[v].printLabel(m);
// 	cv::circle(m, (*this)[v].getCentroid(), 2, 255, -1);
	
// 			cv::Mat draw_tmp = cv::Mat::zeros(m.rows, m.cols, CV_8U);
// 			for(size_t j = 0 ; j < (*this)[v].getZone().size() ; ++j){
// 				draw_tmp.at<uchar>((*this)[v].getZone()[j].x, (*this)[v].getZone()[j].y) = 150;
// 			}
// 			
// 			cv::circle(draw_tmp, (*this)[v].getCentroid(), 10, 255, -1);
// 			
// 			std::cout << "CENTROID " << (*this)[v].getCentroid() << "image size "<<  m.size() << std::endl;
// 			
// 			cv::imshow("Zones " , draw_tmp);
// 			cv::waitKey(0);
		
}


void AASS::RSI::GraphZone::removeVertexUnderSize(int size, bool preserveEdgeConnectic){
	std::pair<VertexIteratorZone, VertexIteratorZone> vp;
	//vertices access all the vertix
	for (vp = boost::vertices((*this)); vp.first != vp.second;) {
		VertexZone v = *vp.first;
		++vp.first;
// 		std::cout << "Remove under size. Size " << (*this)[v].size() << std::endl;
		if((*this)[v].size() < size){
			
			if(preserveEdgeConnectic == true){
				if(getNumEdges(v) > 0){
					
					//TEST
// 					EdgeIteratorZone out_i, out_end;
// 					VertexZone biggest;
// 					bool init = true;
// 					//First find neighbor with biggest zone
// 					for (boost::tie(out_i, out_end) = boost::out_edges(v, (*this)); 
// 						out_i != out_end; out_i = ++out_i) {
// 						
// 						EdgeZone e_second = *out_i;
// 						VertexZone targ = boost::target(e_second, (*this));
// 				// 				std::cout << "Printing both vertex" << std::endl;
// 				// 				std::cout << "Node 1 " << (*this)[targ] << std::endl;
// 						
// 						EdgeIteratorZone out_i_second;
// 				// 		std::cout << "Number of edges " << getNumEdges(targ) << std::endl;
// 						if(init == true){
// 				// 			std::cout << "INIT" << std::endl;
// 							biggest = targ;
// 							init = false;
// 						}
// 						else{
// 				// 			std::cout << "COMPARING SIZES " << std::endl;
// 							if( (*this)[biggest].size() <(*this)[targ].size() ){
// 								biggest = targ;
// 							}
// 						}
// 					
// 					}
					
					try{
						removeVertexWhilePreservingEdges(v, false);
					}
					catch(std::exception& e){
						std::cout << "Here : " << __LINE__ << " " << __FILE__ << " " << e.what() << std::endl;
// 						cv::Mat graphmat2 = cv::Mat::zeros(600,600, CV_8U);
// 						(*this)[biggest].draw(graphmat2, cv::Scalar(100));
// 						cv::imshow("fused", graphmat2);
// 						cv::waitKey(0);	
						exit(0);
					}
				}
				else{
					removeVertex(v);
				}
			}
			else{
				removeVertex(v);
			}

			
		}
	}
}


/*
void AASS::RSI::GraphZone::removeRipples()
{

}*/



bool AASS::RSI::GraphZone::asVerticesWithNoEdges()
{
	std::pair<VertexIteratorZone, VertexIteratorZone> vp;
	//vertices access all the vertix
	for (vp = boost::vertices((*this)); vp.first != vp.second;) {
		VertexZone v = *vp.first;
		++vp.first;
// 		std::cout << "Edge num " << this->getNumEdges(v) << std::endl;
		if(this->getNumEdges(v) == 0){
			return true;
		}
	}
	return false;
}


// void AASS::RSI::GraphZone::watershed()
// {
// 	//Find all "top node"
// // 	throw std::runtime_error("WTF");
// 	
// 	std::deque<VertexZone> top_vertex;
// 	
// 	std::pair<VertexIteratorZone, VertexIteratorZone> vp;
// 	//vertices access all the vertix
// 	for (vp = boost::vertices((*this)); vp.first != vp.second;) {
// 		VertexZone v = *vp.first;
// 		++vp.first;
// 		EdgeIteratorZone out_i, out_end;
// 		
// 		bool top = true;
// 		for (boost::tie(out_i, out_end) = boost::out_edges(v, (*this)); 
// 			out_i != out_end; out_i = ++out_i) {
// 			
// 			EdgeZone e = *out_i;
// 			VertexZone targ = boost::target(e, (*this));
// 			if((*this)[targ].getValue() > (*this)[v].getValue()){
// 				top = false;
// 			}
// 		}
// 		if(top == true){
// 			top_vertex.push_back(v);
// 		}
// 	}
	
// 	//Find all "bottom node"
// 	
// 	std::deque<VertexZone> bottom_vertex;
// 	
// 	//vertices access all the vertix
// 	for (vp = boost::vertices((*this)); vp.first != vp.second;) {
// 		VertexZone v = *vp.first;
// 		++vp.first;
// 		EdgeIteratorZone out_i, out_end;
// 		
// 		bool top = true;
// 		for (boost::tie(out_i, out_end) = boost::out_edges(v, (*this)); 
// 			out_i != out_end; out_i = ++out_i) {
// 			
// 			EdgeZone e = *out_i;
// 			VertexZone targ = boost::target(e, (*this));
// 			if((*this)[targ].getValue() < (*this)[v].getValue()){
// 				top = false;
// 			}
// 		}
// 		if(top == true){
// 			bottom_vertex.push_back(v);
// 		}
// 	}
	
// 	//Watershed the hell out of it!
// 	
// 	//vertices access all the vertix
// 	for (vp = boost::vertices((*this)); vp.first != vp.second;) {
// 		VertexZone v = *vp.first;
// 		++vp.first;
// 		bool is_visited = false;
// 		bool is_visited_new = false;
// 		for(size_t j = 0 ; j < bottom_vertex.size() ; ++j){
// 			if(v == bottom_vertex[j]){
// 				is_visited = true;
// 			}
// 		}
// 		for(size_t j = 0 ; j < top_vertex.size() ; ++j){
// 			if(v == top_vertex[j]){
// 				is_visited_new = true;
// 			}
// 		}
// 		
// 		if(is_visited == false && is_visited_new == false){
// 			EdgeIteratorZone out_i, out_end;
// 			for (boost::tie(out_i, out_end) = boost::out_edges(v, (*this)); 
// 				out_i != out_end; out_i = ++out_i) {
// 				
// 				
// 				EdgeZone e_second = *out_i;
// 				VertexZone targ = boost::target(e_second, (*this));
// // 				std::cout << "Printing both vertex" << std::endl;
// // 				std::cout << "Node 1 " << (*this)[targ] << std::endl;
// 				
// 				EdgeIteratorZone out_i_second, out_end_second;
// 				std::cout << "Number of edges " << getNumEdges(targ) << std::endl;
// 			
// 				for (boost::tie(out_i_second, out_end_second) = boost::out_edges(targ, (*this) )  ; 
// 					out_i_second != out_end_second; ++out_i_second) {
// 					e_second = *out_i_second;
// 				
// 					VertexZone targ2 = boost::target(e_second, (*this));
// 				
// 					EdgeZone edz;
// 					
// 					std::cout << "Printing both vertex linked" << std::endl;
// 					std::cout << "Node 1 " << (*this)[targ] << std::endl;
// 					std::cout << "Node 2 " << (*this)[targ2] << std::endl;
// 				
// 					addEdge(edz, targ, targ2);
// 				}
// 			}
// 			
// 			std::cout << "Removing" << std::endl;
// 			std::cout << (*this)[v] <<std::endl;
// 			removeVertex(v);
// 			
// 			
// 		}
// 		
// 	}
// 
// }


//TODO : would crash on self loop ?
///Recurisve function to find all node to be fused to the original node by the watershed !
void AASS::RSI::GraphZone::getAllNodeRemovedWatershed(AASS::RSI::GraphZone::VertexZone& top_vertex, AASS::RSI::GraphZone::VertexZone& first_vertex, const std::deque< AASS::RSI::GraphZone::VertexZone >& top_vertex_visited, std::deque< AASS::RSI::GraphZone::VertexZone >& top_vertex_visited_tmp, double threshold, std::deque<VertexZone>& to_be_removed){

	
// 	std::cout << "Recursive function" <<std::endl;
	EdgeIteratorZone out_i, out_end;
	int num_edge = getNumEdges(top_vertex), count = 0;
// 	std::cout << "Nume eddges " << num_edge << std::endl;
	
	
	//Can be isloated rooms without this condition
// 	if(num_edge == 0){
// 		std::cout << (*this)[top_vertex] << " num of vert " <<getNumVertices() << " tieration  " << _iteration << std::endl;
// 		print();
// 		cv::Mat graphmat2 = cv::Mat::zeros(600,600, CV_8U);
// 		(*this)[top_vertex].draw(graphmat2, cv::Scalar(100));
// 		cv::imshow("fused", graphmat2);
// 		cv::waitKey(0);	
// 		throw std::runtime_error("Node is not linked anymore");
// 	}
	
	std::deque < EdgeZone > listedge;
	for (boost::tie(out_i, out_end) = boost::out_edges(top_vertex, (*this)); 
		out_i != out_end;) {
		EdgeZone e_second = *out_i;
		listedge.push_back(e_second);
		out_i++;
	}
	
	top_vertex_visited_tmp.push_back(top_vertex);
	
// 	std::cout << "NEW NODE VISITED " << top_vertex << " from " << first_vertex << std::endl;
	
	for (boost::tie(out_i, out_end) = boost::out_edges(top_vertex, (*this)); 
		out_i != out_end;) {
		
		++count;
		EdgeZone e_second = *out_i;

		if((*this)[e_second].canRemove()){
		
			VertexZone targ = boost::target(e_second, (*this));
		
			//Needed to not consider the new added vertex since they are supposed to be stopping point of the recursion.
			//This is a marker for node that are here seen the begining
			bool is_old = false;
			for(size_t i = 0; i < listedge.size() ; ++i){
				if(listedge[i] == e_second)
					is_old = true;
			}
			
			//Do not visit already seen nodes
			//Mark nodes that are already visited
			bool is_visited = false;
			for(size_t j = 0 ; j < top_vertex_visited.size() ; ++j){
				if(targ == top_vertex_visited[j]){
					// std::cout << "SEEN " << j << std::endl;
					is_visited = true;
				}
			}
			
			//Already visited node in this recursion mode. Needed if we don't do a watershed and no direction is kept.
			//It's simply not go backward in the exploration
			bool is_visited_tmp = false;
			for(size_t j = 0 ; j < top_vertex_visited_tmp.size() ; ++j){
				if(targ == top_vertex_visited_tmp[j]){
					// std::cout << "SEEN " << j << std::endl;
					is_visited_tmp = true;
				}
			}

			double targ_value = (*this)[targ].getValue();
			double first_vertex_value = (*this)[top_vertex].getValue();
// 			double first_vertex_value = (*this)[first_vertex].getValue();
			
			//Comparison using the biggest space as a reference
			double max_value = first_vertex_value;
			double min_value = targ_value;
			if(max_value < min_value){
				max_value = targ_value;
				min_value = first_vertex_value;
			}
			
			if(first_vertex_value == 36 || targ_value == 36){
				std::cout << "Max " << max_value << " min " << min_value << " thre " << threshold << " margin " << _margin_factor << std::endl;
				std::cout << min_value << " >= " << max_value - ( (double) max_value * (threshold + _margin_factor)) << std::endl;
			}
		
			//REMOVE TARG
			if( 
	// 			( (int) (*this)[targ].getValue() ) >= ((int)(*this)[first_vertex].getValue()) - threshold && 
	// 			Decided to remove that line since testing the difference between the very first one and next one is not very significant. Indeed if it progressively go down for them going up again, the K-mean wil create a new zone if it gets small enough and then watershed will not be able to go up again after ! So the corridor will be indeed separated in two even without this condition which is super abitrary
				
	// 			(*this)[targ].getValue() < (*this)[first_vertex].getValue() &&
				//Condition for going down the watershed
	// 			(*this)[targ].getValue() < (*this)[top_vertex].getValue() &&
				//Condition of threshold up and down
				min_value >= max_value - ( (double) max_value * (threshold + _margin_factor)) &&
				
				//Min always less than max so should not influence
// 				min_value <= max_value + ( (double) max_value * threshold) &&
				is_old == true && is_visited == false && is_visited_tmp == false
			){
				
				//Clearly the same
				if(min_value >= max_value - ( (double) max_value * threshold)){
					getAllNodeRemovedWatershed(targ, first_vertex, top_vertex_visited, top_vertex_visited_tmp, threshold, to_be_removed);
						
					try{
						// removeVertexWhilePreservingEdges(targ, first_vertex);
						to_be_removed.push_back(targ);
					}
					catch(std::exception& e){
						std::cout << "Zone had more than one shape. It's fine at this point in the proccess. Continue" << std::endl;
						//Catch the error that PCA stopped because of 
					}
				}
				//It's close but not close enough for us to be sure so we look at the neighbors of the target to see if one is close to the top_vertex
				else{
					
					std::cout << "Close but not enough" << std::endl;
					
					//Check if targ as neighbor close to top
					auto closeNeigh = [this, threshold](const VertexZone& top, const VertexZone& targ) -> bool{
							
						double f_v_value = (*this)[top].getValue();
						
						EdgeIteratorZone out_i_tmp, out_end_tmp;
						for (boost::tie(out_i_tmp, out_end_tmp) = boost::out_edges(targ, (*this)); out_i_tmp != out_end_tmp; ++out_i_tmp) {
							EdgeZone e_second_tmp = *out_i_tmp;
							
							//Only check neighbor with breakable links
							if((*this)[e_second_tmp].canRemove()){
								VertexZone targ_second_wave = boost::target(e_second_tmp, (*this));
								
								//As long as it's not the original we check for closeness
								if(targ_second_wave != top){
									double targ_sw_value = (*this)[targ_second_wave].getValue();								
									double max_sw = std::max(targ_sw_value, f_v_value);
									double min_sw = std::min(targ_sw_value, f_v_value);
									
									if(min_sw >= max_sw - ( (double) max_sw * threshold)){
										std::cout << "Close nieghbor" << std::endl;
										return true;
									}
								}
							}
						}
						return false;
					};
					
					bool close_value_neighb = closeNeigh(top_vertex, targ);
					if(close_value_neighb == false){
						close_value_neighb = closeNeigh(targ, top_vertex);
					}
					
					
					//If they were indeed close, they we remove it
					if(close_value_neighb){
						getAllNodeRemovedWatershed(targ, first_vertex, top_vertex_visited, top_vertex_visited_tmp, threshold, to_be_removed);
						
						try{
							// removeVertexWhilePreservingEdges(targ, first_vertex);
							to_be_removed.push_back(targ);
						}
						catch(std::exception& e){
							std::cout << "Zone had more than one shape. It's fine at this point in the proccess. Continue" << std::endl;
							//Catch the error that PCA stopped because of 
						}
					}
					else{
						std::cout << "Not good " << first_vertex_value << " " << targ_value << std::endl;
						++out_i;
					}
					
				}
					
			}
			else{
				++out_i;	
			}
		}
		else{
			++out_i;
		}
	}
	
}



void AASS::RSI::GraphZone::watershed(double threshold)
{
	std::cout << "Starting watershedd" << std::endl;
	
	_iteration = 0;
	std::vector<std::pair<std::deque<VertexZone>, VertexZone > > all_to_remove_and_in_what;
	std::deque<VertexZone> top_vertex_visited;
	//Find all "top node"
	
	std::list<VertexZone> all_vertex;
	std::pair<VertexIteratorZone, VertexIteratorZone> vp;
		//vertices access all the vertix
// 		std::cout << "NEW start" << std::endl;
// 		std::cout << "num of vertices " << getNumVertices() << std::endl; 
	for (vp = boost::vertices((*this)); vp.first != vp.second;) {
		VertexZone v = *vp.first;
		++vp.first;
		all_vertex.push_front(v);
	}
	
	//Sort by smallest to biggest and hightes tpixel value to smallest if size are equal
	all_vertex.sort([this](VertexZone a, VertexZone b)
    {
		if((*this)[a].getValue() == (*this)[b].getValue()){
			//Return biggest value when equal
			return (*this)[a].size() > (*this)[b].size();
		}
		return (*this)[a].getValue() > (*this)[b].getValue();
    });

	for(auto it = all_vertex.begin() ; it != all_vertex.end() ; ++it){
		
		top_vertex_visited.push_back(*it);
	
		// std::cout << "NEW VERTEX.............................:" << std::endl;
		std::deque<VertexZone> top_vertex_visited_tmp;
		top_vertex_visited_tmp.push_back(*it);

		std::deque<VertexZone> to_be_removed;

		int num_edge = getNumEdges(*it), count = 0;
// 		std::cout << "Nume edges " << num_edge << std::endl;
		if(num_edge > 0){
			getAllNodeRemovedWatershed(*it, *it, top_vertex_visited, top_vertex_visited_tmp, threshold, to_be_removed);
			all_to_remove_and_in_what.push_back(std::pair<std::deque<VertexZone>, VertexZone >(to_be_removed, *it));
			
			//Remove vertex to be remove from inspection
			for(auto it_rem = to_be_removed.begin(); it_rem != to_be_removed.end() ; ++it_rem){	
				all_vertex.remove(*it_rem);
			}
		}
	}
	
	for(auto it_rem = all_to_remove_and_in_what.begin() ; it_rem != all_to_remove_and_in_what.end() ; ++it_rem){
		for(auto it = it_rem->first.begin() ; it != it_rem->first.end() ; ++it){
			removeVertexWhilePreservingEdges(*it, it_rem->second, false);
		}
	}
	
	std::cout << "DONE Watershed" << std::endl;
// 	exit(0);
	

}


void AASS::RSI::GraphZone::removeVertexWhilePreservingEdges(AASS::RSI::GraphZone::VertexZone& v, AASS::RSI::GraphZone::VertexZone& v_to_fuse_in, bool createUnBreakableLinks)
{
	assert(v != v_to_fuse_in);
	
	EdgeIteratorZone out_i, out_end;

// 	std::cout << "Getting the value" << std::endl;

	int diff = (*this)[v_to_fuse_in].getValue() - (*this)[v].getValue();
	bool up = true;
	bool same = false;
	if (diff < 0){
		up = false;
	}
	else if (diff == 0){
		same = true;
	}

	//Since we fuse the old zone in biggest we only need to link them to biggest
	for (boost::tie(out_i, out_end) = boost::out_edges(v, (*this)); 
		out_i != out_end; out_i = ++out_i) {
		
		
		EdgeZone e_second = *out_i;
		VertexZone targ = boost::target(e_second, (*this));
// 				std::cout << "Printing both vertex" << std::endl;
// 				std::cout << "Node 1 " << (*this)[targ] << std::endl;
		
		EdgeIteratorZone out_i_second;
// 		std::cout << "Number of edges " << getNumEdges(targ) << std::endl;
		
		if(v_to_fuse_in != targ){
			EdgeZone edz;
			EdgeElement ed_el((*this)[e_second]);
			
			double min_val = (*this)[v].getValue();
			if(min_val >= (*this)[e_second].min_toward && (*this)[e_second].min_toward != -2){
				min_val = (*this)[e_second].min_toward;
			}
			
			//Keep the best minimum score in edge
			if(boost::edge(v_to_fuse_in, targ, (*this)).second == true){
				EdgeZone edz_tmp;
				EdgeElement ed_el_tmp;
				(*this).getEdge(v_to_fuse_in, targ, edz_tmp);
				ed_el_tmp = (*this)[edz_tmp];
				double min_val_tmp = ed_el_tmp.min_toward;
				if(min_val_tmp == -2){
// 					min_val = min_val_tmp;
				}
				else if(min_val_tmp <= min_val){
					min_val = min_val_tmp;
				}
			}
			ed_el.min_toward = min_val;

				
			assert(ed_el.size() == (*this)[e_second].size());
			ed_el.setOldScore(v_to_fuse_in, targ, (*this)[v].getValue());
			
			if((*this)[e_second].canRemove() == false){
				ed_el.makeUnbreakable();
			}

			addEdge(edz, targ, v_to_fuse_in, ed_el);
		}
	}
	
	(*this)[v_to_fuse_in].fuse((*this)[v]);
// 	std::cout << (*this)[v] <<std::endl;
	removeVertex(v);
	
// 	(*this)[v_to_fuse_in].PCA();
	
}



void AASS::RSI::GraphZone::removeVertexWhilePreservingEdges(AASS::RSI::GraphZone::VertexZone& v, bool createUnBreakableLinks)
{
// 	if(getNumEdges(v) == 0){
// 		cv::Mat graphmat = cv::Mat::zeros(500, 500, CV_8U);
// 		draw(graphmat);
// 		cv::imshow("BUG", graphmat);
// 		cv::Mat graphmat3 = cv::Mat::zeros(500, 500, CV_8U);
// 		draw(graphmat3,v, cv::Scalar(255));
// 		cv::imshow("BUGZONE", graphmat3);
// 		cv::waitKey(0);
// 		throw std::runtime_error("Fuck you lonelyness");
// 	}
// 	std::cout << "Yop"<<std::endl;
	assert(getNumEdges(v) > 0 && "Node without edges Oo");
	//Find Closest valued neighbor vertex for fusion of zone
	VertexZone closest;
	bool init = true;
	EdgeIteratorZone out_i, out_end;
	//First find neighbor with biggest zone
	for (boost::tie(out_i, out_end) = boost::out_edges(v, (*this)); 
		out_i != out_end; out_i = ++out_i) {
		
		EdgeZone e_second = *out_i;
		VertexZone targ = boost::target(e_second, (*this));
// 				std::cout << "Printing both vertex" << std::endl;
// 				std::cout << "Node 1 " << (*this)[targ] << std::endl;
		
		EdgeIteratorZone out_i_second;
// 		std::cout << "Number of edges " << getNumEdges(targ) << std::endl;
		if(init == true){
// 			std::cout << "INIT" << std::endl;
			closest = targ;
			init = false;
		}
		else{
// 			std::cout << "COMPARING SIZES " << std::endl;
			if( std::abs((*this)[closest].getValue() - (*this)[v].getValue() ) > std::abs( (*this)[targ].getValue() - (*this)[v].getValue()) ){
				closest = targ;
			}
		}
	
	}
	
	removeVertexWhilePreservingEdges(v, closest, createUnBreakableLinks);
}

//Htis is n!*m n is node number and m is max number of edges
void AASS::RSI::GraphZone::removeRiplesv3(int dist)
{
	updateAllEdges();
	
	std::list<VertexZone> all_vertex;
// 	std::vector<VertexZone> removed_vertices;
	
// 	auto wasRemoved = [](const VertexZone& v, const std::vector<VertexZone>& removed) -> bool{
// 		auto it = removed.begin();
// 		for( ; it != removed.end() ; ++it){
// 			if(v == *it){
// 				std::cout << "Removed " << std::endl;
// 				return true;
// 			}
// 		}
// 		return false;
// 	};
	
	
	std::pair<VertexIteratorZone, VertexIteratorZone> vp;
		//vertices access all the vertix
// 		std::cout << "NEW start" << std::endl;
// 		std::cout << "num of vertices " << getNumVertices() << std::endl; 
	for (vp = boost::vertices((*this)); vp.first != vp.second;) {
		VertexZone v = *vp.first;
		++vp.first;
		all_vertex.push_front(v);
	}
	
	//Sort by smallest to biggest and hightes tpixel value to smallest if size are equal
	all_vertex.sort([this](VertexZone a, VertexZone b)
    {
		if((*this)[a].getValue() == (*this)[b].getValue()){
			//Return biggest value when equal
			return (*this)[a].size() < (*this)[b].size();
		}
		return (*this)[a].getValue() > (*this)[b].getValue();
    });

	for(auto it = all_vertex.begin() ; it != all_vertex.end() ;){
		
// 		std::cout << "Step " << std::distance(all_vertex.begin(), it) << " size " << std::distance(all_vertex.begin(), all_vertex.end()) << std::endl;
		//Remove all ripples
// 		if(wasRemoved(*it, removed_vertices) == false){
// 			std::cout << " Go " << std::endl;
			VertexZone to_fuse_in;
			if(checkAndReplaceRipple(*it, to_fuse_in)){
				try{
					std::deque<VertexZone> to_check;
					this->getAllVertexLinked(*it, to_check);
// 					removed_vertices.push_back(*it);
					
					int add=0;
					//Check the neighbor for ripples so we add right after in the list
					for(int i = 0; i < to_check.size() ; ++i){
						all_vertex.push_back(to_check[i]);
						++add;
					}
// 					std::cout << "Added " << add << std::endl;
	// 				all_vertex.erase(it);
	// 				it = all_vertex.begin();
				}
				catch(std::exception& e){
					std::cout << "Here : " << e.what() << std::endl;
					cv::Mat graphmat2 = cv::Mat::zeros(600,600, CV_8U);
					(*this)[*it].drawZone(graphmat2, cv::Scalar(100));
					(*this)[*it].drawContour(graphmat2, cv::Scalar(100));
					cv::imshow("fused 222", graphmat2);
					cv::waitKey(0);	
					exit(0);
				}
				
				removeVertexWhilePreservingEdges(*it, to_fuse_in, true);
				(*this)[to_fuse_in].updateContour();
				
				auto it_cpy = it;
				while(*it_cpy == *it){
					++it;
				}
				all_vertex.remove(*it_cpy);
			}
			else{
// 				std::cout << "Not a ripple" << std::endl;
				++it;
			}
// 		}
		
	}
	for(auto it = all_vertex.begin() ; it != all_vertex.end() ; ++it){
		makeAllUnbreakableEdges(*it);
	}
		
}

bool AASS::RSI::GraphZone::checkAndReplaceRipple(AASS::RSI::GraphZone::VertexZone& might_be_ripple, AASS::RSI::GraphZone::VertexZone& to_fuse_in)
{
	EdgeIteratorZone out_i, out_end;
	int num_edge = getNumEdges(might_be_ripple);
	
	bool out = false;
	double value = -1;
	
	for (boost::tie(out_i, out_end) = boost::out_edges(might_be_ripple, (*this)); 
		out_i != out_end;) {
		
		EdgeZone e_second = *out_i;
		++out_i;
	
		if((*this)[e_second].canRemove()){
			VertexZone targ = boost::target(e_second, (*this));
			
			if(isRipple(targ, might_be_ripple)){
				//Keep the closest vertex ripple
				if(value == -1 || std::abs(value - (*this)[might_be_ripple].getValue()) > std::abs((*this)[targ].getValue() - (*this)[might_be_ripple].getValue())){
					to_fuse_in = targ;
					out = true;
					value = (*this)[targ].getValue();
				}
			}
			
		}
	}
	return out;
}



//This is n*m! 
void AASS::RSI::GraphZone::removeRiplesv2(int dist)
{
	
// 	assert(true == false && "do not use. Old bug version");
	
	updateAllEdges();
	std::cout << "Starting watershed" << std::endl;
// 	exit(0);
	
	std::deque<VertexZone> top_vertex_visited;
	bool done = false;
	while(done == false){
		
		VertexZone top_vertex;
		bool init = false;
		
		///Find biggest vertex not visited
		
		std::pair<VertexIteratorZone, VertexIteratorZone> vp;
		//vertices access all the vertix
// 		std::cout << "NEW start" << std::endl;
// 		std::cout << "num of vertices " << getNumVertices() << std::endl; 
		for (vp = boost::vertices((*this)); vp.first != vp.second;) {
// 			std::cout << "Looking up vertex " << std::endl;
			VertexZone v = *vp.first;
			++vp.first;
// 			std::cout << "assind vertex " <<(*this)[v]<< std::endl;
			bool is_visited = false;
			for(size_t j = 0 ; j < top_vertex_visited.size() ; ++j){
				if(v == top_vertex_visited[j]){
					is_visited = true;
				}
			}
// 			std::cout << "checked visit " << is_visited << std::endl;
			if(is_visited == false){
// 				std::cout << "Is it init : "<< init << std::endl;
				if(init == false){
// 					std::cout << "assigned init" << std::endl;
					top_vertex = v;
					init = true;
				}
				else if( (*this)[top_vertex].size() < (*this)[v].size() ){
// 					std::cout << "assigned other" << std::endl;
					top_vertex = v;
				}
			}
		}
		
		//Remove all ripples
// 		std::cout << "Value " << (*this)[top_vertex].getValue() << std::endl;
		auto tmp_test_value = (*this)[top_vertex].getValue();
		
// 		cv::Mat graphmat2 = cv::Mat::zeros(600,600, CV_8U);
// 		(*this)[top_vertex].draw(graphmat2, cv::Scalar(100));
// 		(*this)[top_vertex].printLabel(graphmat2);
// 		cv::imshow("fused 222", graphmat2);
// 		cv::waitKey(0);	
		
		getAllNodeRemovedRipples(top_vertex, top_vertex_visited, dist);
		
		auto tmp_test_value2 = (*this)[top_vertex].getValue();
		assert(tmp_test_value == tmp_test_value2);
		
// 		graphmat2 = cv::Mat::zeros(600,600, CV_8U);
// 		(*this)[top_vertex].draw(graphmat2, cv::Scalar(100));
// 		(*this)[top_vertex].printLabel(graphmat2);
// 		cv::imshow("fused 222", graphmat2);
// 		cv::waitKey(0);	
		
		//End condition
		top_vertex_visited.push_back(top_vertex);
		//Stopping condition
		if(top_vertex_visited.size() >= getNumVertices()){
			done = true; 
		}
		else if (top_vertex_visited.size() > getNumVertices()){
// 			std::cout << "SIIIIIZE " << top_vertex_visited.size()  << " ALL VERTEX " << getNumVertices() << std::endl;
			throw std::runtime_error("over shoot in the remove ripples");
		}
		else{
// 			std::cout << "SIIIIIZE " << top_vertex_visited.size()  << " ALL VERTEX " << getNumVertices() << std::endl;
		}
		
		
	}

}


//TODO : would crash on self loop ?
///Recurisve function to find all node to be fused to the original node by the watershed !
void AASS::RSI::GraphZone::getAllNodeRemovedRipples(VertexZone& base_vertex, const std::deque<VertexZone>& top_vertex_visited, int dist){
	
// 	std::cout << "Recursive function" <<std::endl;
	EdgeIteratorZone out_i, out_end;
	int num_edge = getNumEdges(base_vertex);
// 	std::cout << "Nume edges " << num_edge << std::endl;
	
	//Can be isloated rooms without this condition
// 	if(num_edge == 0){
// 		std::cout << (*this)[base_vertex] << " num of vert " <<getNumVertices() << std::endl;
// 		print();
// 		throw std::runtime_error("Node is not linked anymore");
// 	}
	
	for (boost::tie(out_i, out_end) = boost::out_edges(base_vertex, (*this)); 
		out_i != out_end;) {
		
		EdgeZone e_second = *out_i;
	
		++out_i;
	
		if((*this)[e_second].canRemove()){
			VertexZone targ = boost::target(e_second, (*this));
			
// 			int upp;
// 			
// 			double max;
// 			//Min is either the edge variable OR the direct value
// 			double min;
// 			if((*this)[e_second].getOldScore(base_vertex) == -1){
// 				max = std::max( (double)(*this)[base_vertex].getValue(), (double)(*this)[targ].getValue());
// 				min = std::min( (double)(*this)[base_vertex].getValue(), (double)(*this)[targ].getValue());
// 				upp = (double)(*this)[base_vertex].getValue() - (double)(*this)[targ].getValue();
// 			}
// 			else{
// 				max = std::max( (double)(*this)[e_second].getOldScore(base_vertex), (double)(*this)[targ].getValue());
// 				min = std::min( (double)(*this)[e_second].getOldScore(base_vertex), (double)(*this)[targ].getValue());
// 				upp = (double)(*this)[e_second].getOldScore(base_vertex) - (double)(*this)[targ].getValue();
// 			}
// 			
// // 			assert(max <= std::max( (double)(*this)[base_vertex].getValue(), (double)(*this)[targ].getValue()) );
// // 			assert(min <= std::min( (double)(*this)[base_vertex].getValue(), (double)(*this)[targ].getValue()) );
// 				
// 			std::cout << "Top vs " << top_vertex_visited.size() << std::endl;
// 			std::cout << "NEED Values " << isRipple(base_vertex, targ) << " is " << true << " or( " << min  << " >= " << max - (max * _threshold)<< " && " << upp << " >= " << 0 << ") max and min" << max << " " << min << std::endl;
// 			
// 			if((*this)[targ].getValue() == 96 || (*this)[base_vertex].getValue() == 96){
// 	// 			std::cout << "Here : " <<  std::endl;
// 				cv::Mat graphmat2 = cv::Mat::zeros(600,600, CV_8U);
// 				(*this)[targ].drawZone(graphmat2, cv::Scalar(100));
// 				cv::imshow("fused", graphmat2);
// 
// 				cv::Mat graphmat22 = cv::Mat::zeros(600,600, CV_8U);
// 				(*this)[base_vertex].drawZone(graphmat22, cv::Scalar(100));
// 				cv::imshow("fused base", graphmat22);
// 
// 				std::cout << "Is ripple : " << isRipple(base_vertex, targ) << " true is : " << true << std::endl;
// 				
// 				cv::waitKey(0);
// 			}
			
			
			
			if(isRipple(base_vertex, targ) == true /*|| ( min >= max - (max * _threshold) && upp >= 0 )*/){
				
				bool is_visited = false;
				for(size_t j = 0 ; j < top_vertex_visited.size() ; ++j){
					if(targ == top_vertex_visited[j]){
						is_visited = true;
					}
				}
				// if(is_visited == true){
				// 	std::cout << (*this)[targ].getValue() << " " << (*this)[base_vertex].getValue() << std::endl;

				// 	// ATTENTION uncomment this
				// 	// throw std::runtime_error("WE ARE REMOVING A TOP");
				// 	std::cout << "Here : " <<  std::endl;
				// 	cv::Mat graphmat2 = cv::Mat::zeros(600,600, CV_8U);
				// 	(*this)[targ].draw(graphmat2, cv::Scalar(100));
				// 	cv::imshow("fused", graphmat2);

				// 	cv::Mat graphmat22 = cv::Mat::zeros(600,600, CV_8U);
				// 	(*this)[base_vertex].draw(graphmat22, cv::Scalar(100));
				// 	cv::imshow("fused base", graphmat22);

				// 	cv::waitKey(0);	
				// }
				// else{
				
					//Removing the ripple
					// (*this)[base_vertex].fuse((*this)[targ]);
					
					try{
						removeVertexWhilePreservingEdges(targ, base_vertex, true);
					}
					catch(std::exception& e){
						std::cout << "Here : " << e.what() << std::endl;
						cv::Mat graphmat2 = cv::Mat::zeros(600,600, CV_8U);
						(*this)[targ].drawZone(graphmat2, cv::Scalar(100));
						(*this)[targ].drawContour(graphmat2, cv::Scalar(100));
						cv::imshow("fused 222", graphmat2);
						cv::waitKey(0);	
						exit(0);
					}
					(*this)[base_vertex].PCA();
					(*this)[base_vertex].updateContour();
				
					//Need to restart from the begining since the contour may have changed and some that were not ripples might be now.
					boost::tie(out_i, out_end) = boost::out_edges(base_vertex, (*this));
// 					out_end = boost::out_edges(base_vertex, (*this)).second;
					
				// }
				
// 				cv::Mat graphmat2 = cv::Mat::zeros(600,600, CV_8U);
// 				draw(graphmat2, base_vertex, cv::Scalar(100));
// 				cv::imshow("fina", graphmat2);
// 				cv::waitKey(0);
				
			}
// 			else{
// 				++out_i;
// 			}
		}
// 		else{
// 			++out_i;
// 		}
	}
	
	makeAllUnbreakableEdges(base_vertex);
	
	
}

void AASS::RSI::GraphZone::makeAllUnbreakableEdges(VertexZone& base_vertex){
	EdgeIteratorZone out_i, out_end;
//Make unbreakable edges
	
	for (boost::tie(out_i, out_end) = boost::out_edges(base_vertex, (*this)); 
		out_i != out_end;) {
		
		EdgeZone e_second = *out_i;
		VertexZone targ = boost::target(e_second, (*this));
		++out_i;
	
		double val_base = (*this)[base_vertex].getValue();
		double val_targ = (*this)[targ].getValue();
	
		double min_value =  std::min(val_base, val_targ);
		double max_value =  std::max(val_base, val_targ);
	
		if((*this)[e_second].wasRipple()){
			
// 			if(min_value >= max_value - ( (double) max_value * _threshold)){
				
				if((*this)[e_second].shouldBeUnbreakable((*this)[base_vertex].getValue(), base_vertex, (*this)[targ].getValue(), targ, _threshold)){
					(*this)[e_second].makeUnbreakable();
	// 					cv::Mat graphmat2 = cv::Mat::zeros(600,600, CV_8U);
	// 					draw(graphmat2, base_vertex, cv::Scalar(100));
	// 					cv::imshow("fina", graphmat2);
	// 					cv::waitKey(0);
				}
// 			}
		}
		
		
	}
}


bool AASS::RSI::GraphZone::isRipple(const VertexZone& base_vertex, const VertexZone& might_be_ripple) const
{
	
	
	//ATTENTION magic number
// 	if(max > (min * 5)){
// 	if(true == true){
		
	Zone z_ripple = (*this)[might_be_ripple];
	Zone z_base = (*this)[base_vertex];
// 		std::cout << "PERCET : " << z_ripple.contactPoint(z_base) << std::endl;
// 		cv::Mat graphmat2 = cv::Mat::zeros(600,600, CV_8U);
// 		z_ripple.draw(graphmat2, cv::Scalar(255));
// 		cv::imshow("ripple", graphmat2);
// 		cv::waitKey(0);
// 			
// 			It's a ripple !
	//ATTENTION Second magic number
	int nb_contact = z_ripple.contactPoint(z_base);
	
// 	std::cout << "Contact percent " << nb_contact << std::endl;
	//BEST FOR SKETCHMAPS
	//Check that the object is not enterely circled by the zone. i.e a windows or a object in the room
	if(nb_contact >= 40){
// 		std::cout << "it is a ripple: PERCENT " << z_ripple.contactPoint(z_base) << std::endl;
		return true;
	}

		//It might be a contained object so we need to check pca now
	else if(nb_contact >= 60){
// 		std::cout << "NO :(. We have something with more than 60percent contact. Press any key and enter to continue" << std::endl;
		int tmp_a;
		std::cin >> tmp_a;

		auto pca_ripple = (*this)[might_be_ripple].getPCA();
		
		//Find bigger direction
		std::pair<cv::Point2i, cv::Point2i> line1;
		std::pair<cv::Point2i, cv::Point2i> line2;
		
		double length1_x = (std::get<1>(pca_ripple).x - std::get<0>(pca_ripple).x);
		length1_x = length1_x * length1_x;
		double length1_y = (std::get<1>(pca_ripple).y - std::get<0>(pca_ripple).y);
		length1_y = length1_y * length1_y;
		double length1 = length1_x + length1_y;
		
		double length2_x = (std::get<2>(pca_ripple).x - std::get<0>(pca_ripple).x);
		length2_x = length2_x * length2_x;
		double length2_y = (std::get<2>(pca_ripple).y - std::get<0>(pca_ripple).y);
		length2_y = length2_y * length2_y;
		double length2 = length2_x + length2_y;
		
		double max = length1, min = length2;
		if(max < min){
			max = length2;
			min = length1;
		}
		
// 		std::cout << " max and 5 min " << max << " " << min * 10 << std::endl;
		//if it is very alongated
		if(max > (min * 10)){
			return true;
		}
	}
// 	}

	return false;

}



std::vector<AASS::RSI::ZoneCompared> AASS::RSI::GraphZone::compare(GraphZone& target) {
	
	//Update the classification of all zones
	setSizesClassification();
	target.setSizesClassification();
	
	std::vector<ZoneCompared> out;
	std::pair<VertexIteratorZone, VertexIteratorZone> vp;
	//vertices access all the vertix
	for (vp = boost::vertices((*this)); vp.first != vp.second; ++vp.first) {
		auto v = *vp.first;
// 		std::cout << "new source" << std::endl;
		std::pair<VertexIteratorZone, VertexIteratorZone> vp_target;
		//vertices access all the vertix
		for (vp_target = boost::vertices(target); vp_target.first != vp_target.second; ++vp_target.first) {
			auto v_target = *vp_target.first;
			
			//Compare only if both zone are not non unique
			if((*this)[v].isUnique() == true && target[v_target].isUnique() == true){
			
// 				std::cout << "source " << v << std::endl;
				double diff_size;
				double pca_diff;
// 				ZoneCompared zoneout(v, v_target, *this);
				auto zoneinterface = (*this)[v].compare(target[v_target]);
				ZoneCompared zoneout(v, v_target, *this, zoneinterface);
				
				cv::Mat graphmat2 = cv::Mat::zeros(600,600, CV_8U);
				(*this)[v].drawZone(graphmat2, cv::Scalar(100));
				(*this)[v].drawContour(graphmat2, cv::Scalar(100));
				cv::imshow("firstzone", graphmat2);
				cv::Mat graphmat22 = cv::Mat::zeros(600,600, CV_8U);
				target[v_target].drawZone(graphmat22, cv::Scalar(100));
				target[v_target].drawContour(graphmat22, cv::Scalar(100));
				cv::imshow("secondzone", graphmat22);
				
				std::cout << "RES " << zoneout.getSimilarity() << std::endl;
				
				cv::waitKey(0);	
				
				
				//Create the zone comparison element with all values
// 				ZoneCompared zoneout(v, v_target, similarity);
// 				
// 				//ATTENTION Testing
// 				zoneout.size_diff = diff_size;
// 				zoneout.pca_diff = pca_diff;
// 				zoneout.size_source = (*this)[v].getSizeClassification();
// 				zoneout.size_target = target[v_target].getSizeClassification();
				
	// 			zoneout.source = v;
	// 			zoneout.target = v_target;
	// 			zoneout.zone_similarity = similarity;
	// 			std::cout << "WHYYYY" << (*this)[v].getSizeClassification() << std::endl;
	// 			zoneout.zone_size_factor_source = (*this)[v].getSizeClassification();
	// 			zoneout.zone_size_factor_target = target[v_target].getSizeClassification();
				
				//Create the zoneout similarity value
	// 			zoneout.update();
				out.push_back(zoneout);
			}
			else{
				
				std::cout << "NOT UNIQUE" << std::endl;
			}
			
		}
	}
	
// 	for(size_t i = 0 ; i < out.size() ; ++i){
// 		
// 		out[i].update();
// 	}
	
	std::cout << out.size() << "==" << this->getNumUnique() << "*" << target.getNumUnique() << std::endl;
	assert(out.size() == (this->getNumUnique() * target.getNumUnique()));
	return out;
	
}
