#include "Segmentor.hpp"

double AASS::RSI::Segmentor::segmentImage(cv::Mat& src, AASS::RSI::GraphZone& graph_src, double threshold, double margin)
{
					
	std::cout << "T" << graph_src.getT() << std::endl;
	
//Draw the outer contours
	cv::Mat outer;
	src.copyTo(outer);
	
//Find contours - NOT WORKING TODO. TO SIMPLE. TEST CASE WITH NLB map
// 	std::vector<std::vector<cv::Point> > contours;
// 	std::vector<cv::Vec4i> hierarchy;
// 	cv::findContours( src, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE );
// 
// 	
// 	// iterate through all contours, filling holes
// 	for(int i = 0 ; i < contours.size() ; ++i)
// 	{
// 		
// 		if(contours.size() < 30){
// // 					if(hierarchy[i][3] != -1){
// 			cv::drawContours( outer, contours, i, 255 , CV_FILLED, 8, hierarchy );
// 			cv::imshow("input", outer);
// 	cv::waitKey(0);
// // 					}
// 		}
// 	}
// 	
// 	cv::imshow("input", outer);
// 	cv::waitKey(0);
	
// 	cv::threshold(src, src, 20, 255, cv::THRESH_BINARY);
// 	cv::threshold(src, src, 20, 255, cv::THRESH_BINARY_INV);
	
	double begin_process, end_process, decompose_time;
	std::cout << "/************ FUZZY OPENING*************/ \n";
	AASS::RSI::FuzzyOpening fuzzy_slam;
	fuzzy_slam.fast(false);
	
	cv::Mat out_slam;
	
	begin_process = getTime();	
	fuzzy_slam.fuzzyOpening(outer, out_slam, 500);
	end_process = getTime();	decompose_time = end_process - begin_process;
	double time = decompose_time;
	
	std::cout << "Fuzzy opening time: " << time << std::endl;
	
	out_slam.convertTo(out_slam, CV_8U);
		
	std::cout << "/************ REDUCING THE SPACE OF VALUES *****************/\n";
	cv::Mat out_tmp_slam;
	AASS::RSI::ZoneExtractor zone_maker;
	zone_maker.addValueToIgnore(0);
	
	begin_process = getTime();
	AASS::RSI::reduceZone(out_slam, out_tmp_slam, 5);
	zone_maker.extract(out_tmp_slam);
	end_process = getTime();	decompose_time = end_process - begin_process;
	time = time + decompose_time;
	
	std::cout << "Zone reducing: " << decompose_time << std::endl;
		
	std::cout << "/*********** MAKING AND TRIMMING THE GRAPH ***************/\n";
	
	int size_to_remove2 = 10;
	
	begin_process = getTime();
	graph_src = zone_maker.getGraph();
	graph_src.setThreshold(threshold);
	graph_src.setMargin(margin);
// 	graph_src.removeVertexValue(0);	
	graph_src.removeVertexUnderSize(size_to_remove2, true);

	graph_src.useCvMat(true);
	
// 	cv::Mat copyts;
// 	outer.copyTo(copyts);
// 	graph_src.drawSimple(copyts);
// 	cv::imshow("ss", copyts);
// 	cv::waitKey(0);
	
	
// 	graph_src.updatePCA();
	graph_src.updateContours();
	graph_src.removeRiplesv3();
	
// 	cv::Mat copyt;
// 	outer.copyTo(copyt);
// 	graph_src.drawSimple(copyt);
// 	cv::imshow("s", copyt);
// 	cv::waitKey(0);
// 	
	end_process = getTime();	decompose_time = end_process - begin_process;
	time = time + decompose_time;
	std::cout << "Ripples: " << decompose_time << std::endl;
	
	begin_process = getTime();
// 	graph_src.updatePCA();
	graph_src.updateContours();
	
	//Watershed Algorithm
	graph_src.watershed();

	
	
	int size_to_remove = 100;
	graph_src.removeVertexUnderSize(size_to_remove, true);
	graph_src.removeLonelyVertices();
	end_process = getTime();	decompose_time = end_process - begin_process;
	time = time + decompose_time;
	
	std::cout << "watershed: " << decompose_time << std::endl;
	
	if(graph_src.lonelyVertices())
		throw std::runtime_error("Fuck you lonelyness");	
	
	begin_process = getTime();
	findLimits(outer, graph_src);
// 	addHoles(src, contours, hierarchy, graph_src);
	end_process = getTime();	decompose_time = end_process - begin_process;
	time = time + decompose_time;
	
	std::cout << "Drawing" << std::endl;
	cv::Mat copy;
	outer.copyTo(copy);
	
	begin_process = getTime();
	for(auto it = _limits.begin() ; it != _limits.end() ; ++it){
		cv::line(copy, it->first, it->second, cv::Scalar(100), 1);
	}
	end_process = getTime();	decompose_time = end_process - begin_process;
	time = time + decompose_time;
	
	_segmented = copy;
	
	return time;
	
}


void AASS::RSI::Segmentor::addHoles(const cv::Mat& src, std::vector< std::vector< cv::Point > > contours, std::vector< cv::Vec4i > hierarchy, AASS::RSI::GraphZone& graph_src)
{
				
// 	std::cout << "adding holes" << std::endl;
	//Draw all holes on a Mat
	cv::Mat drawing = cv::Mat::zeros(src.rows, src.cols, CV_8UC1);
	// iterate through all the top-level contours,
	for(int i = 0 ; i < contours.size() ; ++i)
	{
		if(hierarchy[i][3] != -1){
			cv::drawContours( drawing, contours, i, 255 , CV_FILLED, 8, hierarchy );
		}
	}
	
	for(int row = 0; row < drawing.rows; ++row) {
		uchar* p = drawing.ptr(row);
		for(int col = 0; col < drawing.cols; ++col) {
			//If the pixel is part of a hole
			if(p[col] != 0){
				//For all zone remove it if it is part of the zone
				std::pair<AASS::RSI::GraphZone::VertexIteratorZone, AASS::RSI::GraphZone::VertexIteratorZone> vp;
				//vertices access all the vertix
// 				std::cout << "NEW start lonely" << std::endl;
		// 		std::cout << "num of vertices " << getNumVertices() << std::endl; 
				for (vp = boost::vertices(graph_src); vp.first != vp.second;) {
		// 			std::cout << "Looking up vertex " << std::endl;
					auto v = *vp.first;
					++vp.first;
					
					graph_src[v].removePoint(row, col);
					
				}
				
				
			}
		}
	}
	
}


void AASS::RSI::Segmentor::findLimits(const cv::Mat& src_mat, AASS::RSI::GraphZone& graph_src)
{
	
// 	auto contacttest = [](int x, int y, int x2, int y2) -> bool{
// 		if(x + 1 >= x2 && x - 1 <= x2){
// 			if(y + 1 >= y2 && y - 1 <= y2){
// 				return true;
// 			}
// 		}
// 		return false;
// 	};
// 	std::cout << "find limits" << std::endl;
	
	auto touchMat = [&src_mat](int x, int y, cv::Point2i& out) -> bool{
		
		int xx;
		for( xx = x - 2 ; xx < x + 3 ; ++xx ){
			int yy;
			for( yy = y - 2 ; yy < y + 3 ; ++yy ){
// 							std::cout << " x y " << xx << " " << yy << std::endl;
				if(src_mat.at<uchar>(yy, xx) == 0){
					out.x = xx;
					out.y = yy;
					return true;
				}
			}
		}
		return false;

	};
	
	//Return closest point to p, on contour vp, that touches a wall
	auto distPointPerimeter = [touchMat](cv::Point2i p, std::vector <std::vector <cv::Point2i> > vp) -> cv::Point2i{
		
		double dist = -1;
		auto it_contour = vp.begin();
		cv::Point2i p_out = (*it_contour)[0];
		for(it_contour ; it_contour != vp.end() ; ++it_contour){
			auto it = it_contour->begin();
			for(it ; it != it_contour->end() ; ++it){
				cv::Point2i out;
				if(touchMat(it->x, it->y, out) == true){
					auto tdist = cv::norm(*it - p);
					if(dist == -1 || dist > tdist){
						dist = tdist;
						p_out = out;
					}
				}
			}
			
		}
		return p_out;

	};
	
	
	std::pair<AASS::RSI::GraphZone::VertexIteratorZone, AASS::RSI::GraphZone::VertexIteratorZone> vp;
	//vertices access all the vertix
// 				std::cout << "NEW start lonely" << std::endl;
// 		std::cout << "num of vertices " << getNumVertices() << std::endl; 
	std::set < AASS::RSI::GraphZone::EdgeZone > visited_edges;
	for (vp = boost::vertices(graph_src); vp.first != vp.second;) {
// 			std::cout << "Looking up vertex " << std::endl;
		auto v = *vp.first;
		++vp.first;
		AASS::RSI::GraphZone::EdgeIteratorZone out_i, out_end;
		//Since we fuse the old zone in biggest we only need to link them to biggest
		for (boost::tie(out_i, out_end) = boost::out_edges(v, graph_src); 
			out_i != out_end; out_i = ++out_i) {
			
			AASS::RSI::GraphZone::EdgeZone e_second = *out_i;
			if(std::find(visited_edges.begin(), visited_edges.end(), e_second) == visited_edges.end()){
			
				visited_edges.insert(e_second);
				
				AASS::RSI::GraphZone::VertexZone targ = boost::target(e_second, graph_src);
				AASS::RSI::GraphZone::VertexZone src = boost::source(e_second, graph_src);
			
				auto contact = graph_src[src].getContactPointSeparated(graph_src[targ]);
				
				assert(contact.size() > 0);
				cv::Point l1, l2;
				for(auto it3 = contact.begin(); it3 != contact.end() ; ++it3){
					for(auto it = it3->begin(); it != it3->end() ; ++it){
						double distance = -1;
						for(auto it2 = it3->begin(); it2 != it3->end() ; ++it2){
							auto tdist = cv::norm(*it - *it2);
		// 					std::cout << distance << " > " << tdist << std::endl;
							if(distance == -1 || distance < tdist){
								l1 = *it;
								l2 = *it2;
								distance = tdist;
							}
						}
					}
					
					//Find point on contour that touch a wall and close to l1 and l2
				
	// 				cv::Mat src_tmp = cv::Mat::zeros(src_mat.size(), CV_8UC1);
	// 				graph_src[src].drawZone(src_tmp, cv::Scalar(255));
	// 				cv::imshow("tmp", src_tmp);
	// 				cv::waitKey(0);
					
					auto contour = graph_src[src].getContour();
					double dist = -1;
					auto l1_f = distPointPerimeter(l1, contour);
					auto l2_f = distPointPerimeter(l2, contour);
					std::cout << "Pushing back " << l1_f << " " << l2_f << std::endl;
					_limits.push_back(std::pair<cv::Point2i, cv::Point2i>(l1_f, l2_f));
				}
			}
			
// 			auto it2 = it+1;
// 			while(touchMat(it->x, it->y) != true  touchMat(it2->x, it2->y) != true){
// 				std::cout << "start " << touchMat(it->x, it->y) << " && " << touchMat(it2->x, it2->y)<< std::endl;
// 				++it;
// 				++it2;
// 				std::cout << "second " << touchMat(it->x, it->y) << " && " << touchMat(it2->x, it2->y)<< std::endl;
// 				if(it2 == contour.end()){
// 					throw std::runtime_error("Terrible contour for limits");
// 				}
// 			}
// 			
// 			std::cout << "start " << touchMat(it->x, it->y) << " && " << touchMat(it2->x, it2->y) << " true is " << true << std::endl;
// 			
// 			cv::Point l1, l2;
// 			bool flag = false;
// 			for( int i = 0 ; i < contour.size() ; ++i){
// 				//True if touch on obstacle
// 				std::cout << touchMat(it->x, it->y) << " && " << !touchMat(it2->x, it2->y) << std::endl;
// 				if(touchMat(it->x, it->y) == true && touchMat(it2->x, it2->y) != true){
// 					l1 = *it;
// 					flag = true;
// 				}
// 				if(touchMat(it->x, it->y) != true && touchMat(it2->x, it2->y) == true){
// 					_limits.push_back(std::pair<cv::Point2i, cv::Point2i>(l1, *it2));
// 					assert(flag == true);
// 					flag = false;
// 				}
// 				++it;
// 				++it2;
// 				if(it == contour.end()) it = contour.begin();
// 				if(it2 == contour.end()) it2 = contour.begin();
// 			}
		
			
			
		}

	}
// 	std::cout << _limits.size() << " == " << graph_src.getNumEdges() << std::endl;
// 	if(_limits.size() != graph_src.getNumEdges()){
// 		throw std::runtime_error("wtf");
// 		std::cout << "SHIT" << std::endl;
// 		exit(0);
// 	}
// 	assert(_limits.size() == graph_src.getNumEdges());

}
