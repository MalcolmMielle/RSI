#include "GraphZone.hpp"

void AASS::RSI::GraphZone::drawPartial(cv::Mat& drawmat) const
{
	cv::Mat drawmat_old;
	drawmat.copyTo(drawmat_old);
	
	cv::Scalar color;
	cv::RNG rng(12345);
	std::pair<VertexIteratorZone, VertexIteratorZone> vp;
	//vertices access all the vertix
	for (vp = boost::vertices((*this)); vp.first != vp.second; ++vp.first) {
		
		if(drawmat.channels() == 1){
			color = rng.uniform(50, 255);
		}
		else if(drawmat.channels() == 3){
			color[0] = rng.uniform(50, 255);
			color[1] = rng.uniform(50, 255);
			color[2] = rng.uniform(50, 255);
		}
		
		VertexZone v = *vp.first;
		
		if((*this)[v].getZone().size() > 100){
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
			
			cv::imshow("Zone", (*this)[v].getZoneMat());
			cv::imshow("Partial" , copy);
			cv::waitKey(0);
		}
		
	}

}

void AASS::RSI::GraphZone::draw(cv::Mat& drawmat) const
{
	cv::Mat drawmat_old;
	drawmat.copyTo(drawmat_old);
	
	cv::Scalar color;
	cv::RNG rng(12345);
	std::pair<VertexIteratorZone, VertexIteratorZone> vp;
	//vertices access all the vertix
	for (vp = boost::vertices((*this)); vp.first != vp.second; ++vp.first) {
		
		if(drawmat.channels() == 1){
			color = 255;
		}
		else if(drawmat.channels() == 3){
			color[0] = 255;
			color[1] = 255;
			color[2] = 255;
		}
		
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
					cv::line(drawmat, (*this)[src].getCentroid(), (*this)[targ].getCentroid(), color);
				}
			}
// 
// 		}
		
	}

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
	cv::circle(m, (*this)[v].getCentroid(), 2, color, -1);
	
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


void AASS::RSI::GraphZone::removeVertexUnderSize(int size, bool preserveEdgeConnectic, cv::Mat& test){
	std::pair<VertexIteratorZone, VertexIteratorZone> vp;
	//vertices access all the vertix
	for (vp = boost::vertices((*this)); vp.first != vp.second;) {
		VertexZone v = *vp.first;
		++vp.first;
		std::cout << "Size " << (*this)[v].size() << std::endl;
		if((*this)[v].size() < size){
			
			
			EdgeIteratorZone out_i, out_end;
			for (boost::tie(out_i, out_end) = boost::out_edges(v, (*this)); 
				out_i != out_end; out_i = ++out_i) {
				
				
				EdgeZone e_second = *out_i;
				VertexZone targ = boost::target(e_second, (*this));
// 				std::cout << "Printing both vertex" << std::endl;
// 				std::cout << "Node 1 " << (*this)[targ] << std::endl;
				
				EdgeIteratorZone out_i_second;
				std::cout << "Number of edges " << getNumEdges(targ) << std::endl;
			
				for (out_i_second = std::next(out_i) ; 
					out_i_second != out_end; ++out_i_second) {
					e_second = *out_i_second;
				
					VertexZone targ2 = boost::target(e_second, (*this));
				
					EdgeZone edz;
					
					std::cout << "Printing both vertex linked" << std::endl;
					std::cout << "Node 1 " << (*this)[targ] << std::endl;
					std::cout << "Node 2 " << (*this)[targ2] << std::endl;
				
					addEdge(edz, targ, targ2);
				}
			}
			
			std::cout << "Removing" << std::endl;
			std::cout << (*this)[v] <<std::endl;
			removeVertex(v);
			
// 			cv::Mat copy;
// 			test.copyTo(copy);
// 			
// 			this->draw(copy);
// 			cv::imshow("Partial graph", copy);
// 			cv::waitKey(0);
			
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
		std::cout << "Edge num " << this->getNumEdges(v) << std::endl;
		if(this->getNumEdges(v) == 0){
			return true;
		}
	}
	return false;
}


void AASS::RSI::GraphZone::watershed()
{
	//Find all "top node"
// 	throw std::runtime_error("WTF");
	
	std::deque<VertexZone> top_vertex;
	
	std::pair<VertexIteratorZone, VertexIteratorZone> vp;
	//vertices access all the vertix
	for (vp = boost::vertices((*this)); vp.first != vp.second;) {
		VertexZone v = *vp.first;
		++vp.first;
		EdgeIteratorZone out_i, out_end;
		
		bool top = true;
		for (boost::tie(out_i, out_end) = boost::out_edges(v, (*this)); 
			out_i != out_end; out_i = ++out_i) {
			
			EdgeZone e = *out_i;
			VertexZone targ = boost::target(e, (*this));
			if((*this)[targ].getValue() > (*this)[v].getValue()){
				top = false;
			}
		}
		if(top == true){
			top_vertex.push_back(v);
		}
	}
	
	//Find all "bottom node"
	
	std::deque<VertexZone> bottom_vertex;
	
	//vertices access all the vertix
	for (vp = boost::vertices((*this)); vp.first != vp.second;) {
		VertexZone v = *vp.first;
		++vp.first;
		EdgeIteratorZone out_i, out_end;
		
		bool top = true;
		for (boost::tie(out_i, out_end) = boost::out_edges(v, (*this)); 
			out_i != out_end; out_i = ++out_i) {
			
			EdgeZone e = *out_i;
			VertexZone targ = boost::target(e, (*this));
			if((*this)[targ].getValue() < (*this)[v].getValue()){
				top = false;
			}
		}
		if(top == true){
			bottom_vertex.push_back(v);
		}
	}
	
	//Watershed the hell out of it!
	
	//vertices access all the vertix
	for (vp = boost::vertices((*this)); vp.first != vp.second;) {
		VertexZone v = *vp.first;
		++vp.first;
		bool is_visited = false;
		bool is_visited_new = false;
		for(size_t j = 0 ; j < bottom_vertex.size() ; ++j){
			if(v == bottom_vertex[j]){
				is_visited = true;
			}
		}
		for(size_t j = 0 ; j < top_vertex.size() ; ++j){
			if(v == top_vertex[j]){
				is_visited_new = true;
			}
		}
		
		if(is_visited == false && is_visited_new == false){
			EdgeIteratorZone out_i, out_end;
			for (boost::tie(out_i, out_end) = boost::out_edges(v, (*this)); 
				out_i != out_end; out_i = ++out_i) {
				
				
				EdgeZone e_second = *out_i;
				VertexZone targ = boost::target(e_second, (*this));
// 				std::cout << "Printing both vertex" << std::endl;
// 				std::cout << "Node 1 " << (*this)[targ] << std::endl;
				
				EdgeIteratorZone out_i_second, out_end_second;
				std::cout << "Number of edges " << getNumEdges(targ) << std::endl;
			
				for (boost::tie(out_i_second, out_end_second) = boost::out_edges(targ, (*this) )  ; 
					out_i_second != out_end_second; ++out_i_second) {
					e_second = *out_i_second;
				
					VertexZone targ2 = boost::target(e_second, (*this));
				
					EdgeZone edz;
					
					std::cout << "Printing both vertex linked" << std::endl;
					std::cout << "Node 1 " << (*this)[targ] << std::endl;
					std::cout << "Node 2 " << (*this)[targ2] << std::endl;
				
					addEdge(edz, targ, targ2);
				}
			}
			
			std::cout << "Removing" << std::endl;
			std::cout << (*this)[v] <<std::endl;
			removeVertex(v);
			
			
		}
		
	}

}


//TODO : would crash on self loop ?
void AASS::RSI::GraphZone::getAllNodeRemoved(VertexZone& top_vertex, VertexZone& first_vertex, const std::deque<VertexZone>& top_vertex_visited, int threshold, bool& change){
	
// 	std::cout << "Recursive function" <<std::endl;
	EdgeIteratorZone out_i, out_end;
	int num_edge = getNumEdges(top_vertex), count = 0;
// 	std::cout << "Nume edges " << num_edge << std::endl;
	if(num_edge == 0){
		std::cout << (*this)[top_vertex] << " num of vert " <<getNumVertices() << std::endl;
		print();
		throw std::runtime_error("Node is not linked anymore");
	}
	
	std::deque < EdgeZone > listedge;
	for (boost::tie(out_i, out_end) = boost::out_edges(top_vertex, (*this)); 
		out_i != out_end;) {
		EdgeZone e_second = *out_i;
		listedge.push_back(e_second);
		out_i++;
	}
	
	for (boost::tie(out_i, out_end) = boost::out_edges(top_vertex, (*this)); 
		out_i != out_end;) {
		
		++count;
		EdgeZone e_second = *out_i;
		
		VertexZone targ = boost::target(e_second, (*this));
	
		bool is_old = false;
		for(size_t i = 0; i < listedge.size() ; ++i){
			if(listedge[i] == e_second)
				is_old = true;
		}
	
// 		std::cout << "On this vertex for edge " << (*this)[targ].getValue() << " " << (*this)[top_vertex].getValue() <<" first " << (*this)[first_vertex].getValue() << " " << threshold << std::endl;
	
		//REMOVE TARG
		if( (*this)[targ].getValue() >= (*this)[first_vertex].getValue() - threshold && 
			(*this)[targ].getValue() < (*this)[first_vertex].getValue() && 
			(*this)[targ].getValue() < (*this)[top_vertex].getValue() &&
			is_old == true
		){
			
// 			std::cout << "REMOVE vertex for edge " << (*this)[targ].getValue() << std::endl;
			
			bool is_visited = false;
			for(size_t j = 0 ; j < top_vertex_visited.size() ; ++j){
				if(targ == top_vertex_visited[j]){
					is_visited = true;
				}
			}
			if(is_visited == true){
				std::cout << (*this)[targ].getValue() << " " << (*this)[top_vertex].getValue() <<" " << threshold << std::endl;
				throw std::runtime_error("WE ARE REMOVING A TOP");
			}
			
			//Recursion :(
// 			std::cout << "sending recur " << (*this)[first_vertex].getValue() << std::endl;
			getAllNodeRemoved(targ, first_vertex, top_vertex_visited, threshold, change);
			
			change = true;
			EdgeIteratorZone out_i_second, out_end_second;
			
			int vertnum = getNumVertices();
			int vertedge = getNumEdges(targ);
		
			std::vector<VertexZone> test;
			
			for (boost::tie(out_i_second, out_end_second) = boost::out_edges(targ, (*this) ) ; 
				out_i_second != out_end_second; ++out_i_second) {
				e_second = *out_i_second;
			
				VertexZone targ2 = boost::target(e_second, (*this));
				if(targ2 != first_vertex){
// 					std::cout << "Adding edge with total edges : " << getNumEdges(targ) << std::endl;
					EdgeZone edz;				
					addEdge(edz, first_vertex, targ2);
					if(vertnum != getNumVertices()){
						throw std::runtime_error("Number fo Vertice Change at an abnormal place");
					}
					if(vertedge != getNumEdges(targ)){
						throw std::runtime_error("Number fo Edges Change at an abnormal place");
					}
					
				}
				else{
// 					std::cout << "Not an edge between same node :| " << std::endl;
				}
				
				for(size_t ju = 0 ; ju < test.size() ; ++ju){
					if(targ2 == test[ju]){
						throw std::runtime_error("Same node linked twice with the same edge :( ?");
					}
				}
				test.push_back(targ2);
				
				
			}
// 					std::cout << "REMOVE THE BASE VERTEX EDGE GOING TO NEXT EDGE" << std::endl;
// 					throw std::runtime_error("Not an error, it actually works" );
			//TODO : replace this by a fuse Zone function
			
// 			std::cout << "Removing " <<(*this)[targ] << " with edges " << getNumEdges(targ) << std::endl;
			
			//IMPORTANT THAT'S IT RIGHT BEFORE THE REMOVE
			++out_i;
			removeVertex(targ);
			cv::Mat tmptmp = cv::imread("../Test/labsketch_trimmed.png", CV_LOAD_IMAGE_GRAYSCALE);
			cv::Mat graphmat = cv::Mat::zeros(tmptmp.size(), CV_8U);
			cv::Mat graphmat2 = cv::Mat::zeros(tmptmp.size(), CV_8U);
			draw(graphmat);
			draw(graphmat2, first_vertex, cv::Scalar(255));
			cv::imshow("GRAPH", graphmat);
			cv::imshow("CENTER", graphmat2);
			cv::waitKey(0);
			
// 			std::cout << std::endl;
// 			print();
// 			std::cout << std::endl;
// 			std::cout << "AFTER PRINT" << std::endl;
		}
		else{
			++out_i;
// 			std::cout << "Not to be removed " << std::endl;
		}
	}
	
}



void AASS::RSI::GraphZone::watershed(int threshold)
{
	//Find all "top node"
	
	std::cout << "Starting watershed" << std::endl;
// 	exit(0);
	
	std::deque<VertexZone> top_vertex_visited;
	bool done = false;
	while(done == false){
		
		VertexZone top_vertex;
		bool init = false;
		
		//Find the highest not visited vertex
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
				else if( (*this)[top_vertex].getValue() < (*this)[v].getValue() ){
// 					std::cout << "assigned other" << std::endl;
					top_vertex = v;
				}
			}
		}
		
		std::cout << "watersheding " << (*this)[top_vertex] << std::endl;
		//Watershed the hell out of it!
	
	
		bool change = true;
// 		while(change == true){
// 			change = false;
			
			getAllNodeRemoved(top_vertex, top_vertex, top_vertex_visited, threshold, change);
			
// 			std::cout << "DONE WITH ONE SEED " << std::endl;
			
// 			std::cout << std::endl;
// 			print();
// 			std::cout << std::endl;
// 			EdgeIteratorZone out_i, out_end;
// 			int num_edge = getNumEdges(top_vertex), count = 0;
// 			for (boost::tie(out_i, out_end) = boost::out_edges(top_vertex, (*this)); 
// 				out_i != out_end;) {
// 				
// // 				std::cout << "Edge number " << count << " total " << num_edge << std::endl;
// 				++count;
// 				EdgeZone e_second = *out_i;
// 				++out_i;
// 			
// 				VertexZone targ = boost::target(e_second, (*this));
// 			
// // 				std::cout << "On this vertex for edge " << (*this)[targ].getValue() << " " << (*this)[top_vertex].getValue() <<" " << threshold << std::endl;
// 			
// 				//REMOVE TARG
// 				if( (*this)[targ].getValue() >= (*this)[top_vertex].getValue() - threshold && 
// 					(*this)[targ].getValue() < (*this)[top_vertex].getValue() ){
// 					
// 					bool is_visited = false;
// 					for(size_t j = 0 ; j < top_vertex_visited.size() ; ++j){
// 						if(targ == top_vertex_visited[j]){
// 							is_visited = true;
// 						}
// 					}
// 					if(is_visited == true){
// 						std::cout << (*this)[targ].getValue() << " " << (*this)[top_vertex].getValue() <<" " << threshold << std::endl;
// 						throw std::runtime_error("WE ARE REMOVING A TOP");
// 					}
// 					
// 					change = true;
// 					EdgeIteratorZone out_i_second, out_end_second;
// 				
// 					for (boost::tie(out_i_second, out_end_second) = boost::out_edges(targ, (*this) ) ; 
// 						out_i_second != out_end_second; ++out_i_second) {
// 						e_second = *out_i_second;
// 					
// 						VertexZone targ2 = boost::target(e_second, (*this));
// 						if(targ2 != top_vertex){
// 							EdgeZone edz;					
// 							addEdge(edz, top_vertex, targ2);
// 						}
// 					}
// // 					std::cout << "REMOVE THE BASE VERTEX EDGE GOING TO NEXT EDGE" << std::endl;
// // 					throw std::runtime_error("Not an error, it actually works" );
// 					//TODO : replace this by a fuse Zone function
// 					removeVertex(targ);
// 				}
// 				else{
// // 					std::cout << "Not removing a THING" << std::endl;
// 				}
// 				
// 			}
// 			std::cout << "Got out " << std::endl;
// 		}
		top_vertex_visited.push_back(top_vertex);
		//Stopping condition
		if(top_vertex_visited.size() == getNumVertices()){
			done = true; 
		}
		else if (top_vertex_visited.size() > getNumVertices()){
// 			std::cout << "SIIIIIZE " << top_vertex_visited.size()  << " ALL VERTEX " << getNumVertices() << std::endl;
			throw std::runtime_error("over shoot in the water shed");
		}
		else{
// 			std::cout << "SIIIIIZE " << top_vertex_visited.size()  << " ALL VERTEX " << getNumVertices() << std::endl;
		}
		
				
	}
	

}
