#include "GraphZone.hpp"

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
			
			cv::imshow("Partial" , copy);
			cv::waitKey(0);
		}
		
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
	cv::circle(m, (*this)[v].getCentroid(), 10, 255, -1);
	
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
		std::cout << "Size " << (*this)[v].size() << std::endl;
		if((*this)[v].size() < size){
			
			
			EdgeIteratorZone out_i, out_end;
			EdgeZone e;
			for (boost::tie(out_i, out_end) = boost::out_edges(v, (*this)); 
				out_i != out_end; out_i = ++out_i) {
				
				
				EdgeZone e_second = *out_i;
				VertexZone targ = boost::target(e_second, (*this));
				std::cout << "Printing both vertex" << std::endl;
				std::cout << "Node 1 " << (*this)[targ] << std::endl;
				
				EdgeIteratorZone out_i_second;
			
				for (out_i_second = std::next(out_i) ; 
					out_i_second != out_end; ++out_i_second) {
					e_second = *out_i_second;
				
					VertexZone targ2 = boost::target(e_second, (*this));
				
					EdgeZone edz;
					
					std::cout << "Printing both vertex" << std::endl;
					std::cout << "Node 1 " << (*this)[targ] << std::endl;
					std::cout << "Node 2 " << (*this)[targ2] << std::endl;
				
					addEdge(edz, targ, targ2);
				}
			}
			
			
			
			
			std::cout << "Trying to remove" << std::endl;
			removeVertex(v);
			std::cout << "Removed" << std::endl;
		}
	}
}


/*
void AASS::RSI::GraphZone::removeRipples()
{

}*/
