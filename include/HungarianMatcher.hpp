#ifndef RSI_HUNGARIANMATCHER_23102016
#define RSI_HUNGARIANMATCHER_23102016

#include "GraphZoneRI.hpp"
#include "hungarian/hungarian.h"


namespace AASS{
	namespace RSI{
		
		
// 		class Match{
// 		public:
// 			GraphZoneRI::Vertex source;
// 			GraphZoneRI::Vertex target;
// 			int score;
// 
// 			Match(const GraphZoneRI::Vertex& firstt, const GraphZoneRI::Vertex& secondd, int scoree) : source(firstt), target(secondd), score(scoree){}
// 			
// 			//Return a value ranking the matching between 0 (bad) and 100 (perfect)
// 			double getRanking(const GraphZoneRI& gsource, const GraphZoneRI& gtarget){
// 				double uscore =  gsource[source].getUniquenessScore();
// 				double uscore0 = gtarget[target].getUniquenessScore();				
// 				return ((100 * std::min(uscore, uscore0)) + (100 - score)) / 2;
// 			}
// 			
// 		};
		
		
		
		int** array_to_matrix(const std::vector<int>& m, int rows, int cols) {
			int i,j;
			int** r;
			r = (int**)calloc(rows,sizeof(int*));
			for(i=0;i<rows;i++)
			{
			r[i] = (int*)calloc(cols,sizeof(int));
			for(j=0;j<cols;j++)
				r[i][j] = m[i*cols+j];
			}
			return r;
		}
	
		class HungarianMatcher{
		
		private:
			bool is_init_;
		protected:
			hungarian_problem_t p;
			
		public:
			
			HungarianMatcher() : is_init_(false){};
			
			//TODO : make sure all memory is freed
			~HungarianMatcher(){
				/* free used memory */
				if(is_init_ == true){
					hungarian_free(&p);
				}
				
			}
			
			 std::vector< ZoneCompared > match(GraphZoneRI& source, GraphZoneRI& target, std::vector<int>& scores);
			 
			 const hungarian_problem_t& getHungarianProblem() const { return p;}
			 hungarian_problem_t& getHungarianProblem() { return p;}
			 
			 //TODO : cost is not const !!! save it at the begining
			 int** getCostNonConstOptimized(){return p.cost;}
// 			 const int** getCost() const {return p.cost;}
			
		private:
			
			void createCostMatrix(const GraphZoneRI& source, const GraphZoneRI& target);
			
		};
		
		/**
		 * @brief match two graph using the uniqueness and the hungarian.
		 * 
		 * @param[in] source: source graph
		 * @param[in] target: target graph
		 * @param[out] scores: list of score for the matching between 0 and 100. 0 is good and 100 is really bad.
		 * @return vector of paris of matching between vertex.
		 */
		inline std::vector< ZoneCompared > AASS::RSI::HungarianMatcher::match(GraphZoneRI& source, GraphZoneRI& target, std::vector<int>& scores){
			
			cv::Mat imgimg = cv::Mat::zeros(cv::Size(1000,1000), CV_8UC1);
			source.draw(imgimg);
			cv::imshow("in", imgimg);
	
			target.draw(imgimg);
			cv::imshow("innnn", imgimg);
			cv::waitKey(0);
			
			
			std::cout << "TATENTIONDKJ " << source.zoneUniquenessWasCalculated() << " sadoifdsgiouh " << target.zoneUniquenessWasCalculated() << std::endl;
// 			assert(source.zoneUniquenessWasCalculated() == true);
// 			assert(target.zoneUniquenessWasCalculated() == true);
// 			hungarian_problem_t p;
			
			std::cout << "Creating the smilarity" << std::endl;
			auto res = source.compare(target);
			
			std::cout << "Wat" << std::endl;
			for(size_t i = 0 ; i < res.size() ; ++i){
				std::cout << res[i].source << " " << res[i].target << std::endl;
			}
			
			std::cout << "Convertion" << std::endl;
			//Convert all similarity values between 0 and 100.
			std::vector<int> simi;
			auto it = res.begin();
			for ( ; it != res.end() ; ++it){
				int input = it->getSimilarity()*100;
				std::cout << " pushing " << input << " because " << it->getSimilarity()*100 << std::endl;
				assert(input <= 100);
				assert(input >= 0);
				
				simi.push_back(input);
			}
			
			assert(simi.size() == source.getNumUnique()*target.getNumUnique());
			
			std::cout << "OUT source on rows and target on cols" << std::endl;
			int** m = array_to_matrix(simi, source.getNumUnique(), target.getNumUnique());
			
			std::cout << "INit" << std::endl;
			
			int matrix_size = hungarian_init(&p, m , source.getNumUnique(), target.getNumUnique() , HUNGARIAN_MODE_MINIMIZE_COST);
			
			is_init_ = true;
			
			/* some output */
			fprintf(stderr, "cost-matrix:");
			hungarian_print_costmatrix(&p);

			std::cout << "Solving" << std::endl;
			/* solve the assignement problem */
			hungarian_solve(&p);
			std::cout << "Solving done" << std::endl;
			
			/* some output */
			fprintf(stderr, "assignment:");
			hungarian_print_assignment(&p);
			
			/* some output */
			fprintf(stderr, "cost-matrix:");
			hungarian_print_costmatrix(&p);
			
			std::vector< ZoneCompared > out;
			// This depend on which one as more nodes !
			// Goes along the line
			if(source.getNumUnique() <= target.getNumUnique()){
				
				int i,j;
	// 			fprintf(stderr , "\n");
				for(i=0; i<source.getNumUnique(); i++) {
	// 				fprintf(stderr, " [");
					std::cout << " i " << std::endl;
					for(j=0; j<target.getNumUnique(); j++) {
						
						std::cout << " w " << std::endl;
						if(p.assignment[i][j] == 1){
							
							std::cout << "Matching lol2" << i << " with " << j << " cost " << simi.at( ( i*target.getNumUnique() ) + j) << std::endl;
							
							std::cout << "Matching " << i * target.getNumUnique() << " with " << (i * target.getNumUnique() )  + j << std::endl;
							std::cout << res.at(i * target.getNumUnique()).source << " " << res.at(( i * target.getNumUnique() ) + j).target << std::endl;
							
// 							ZoneCompared m(res.at(i * target.getNumUnique()).source, res.at(( i * target.getNumUnique() ) + j).target, simi.at( ( i*target.getNumUnique() ) + j));
							
							assert(res.at(( i * target.getNumUnique() ) + j).source == res.at(i * target.getNumUnique()).source);
							std::cout << "u" << std::endl;
							assert(simi.at( ( i*target.getNumUnique() ) + j) == (int) (res.at(( i * target.getNumUnique() ) + j).getSimilarity() * 100 ));
							std::cout << "u" << std::endl;
							
							out.push_back(res.at(( i * target.getNumUnique() ) + j));
							std::cout << "u" << std::endl;
							scores.push_back(simi.at( ( i*target.getNumUnique() ) + j));
							std::cout << "u" << std::endl;
						}
					}
					
					
	// 				fprintf(stderr, "]\n");
				}
			}
			//Goes down the column
			else{
				assert(out.size() == 0);
				std::cout << "Source more than target" << std::endl;
				int i,j;
	// 			fprintf(stderr , "\n");
				std::cout << "target " << target.getNumUnique() << " source " << source.getNumUnique() << std::endl;
				for(i = 0; i < target.getNumUnique(); i++) {
	// 				fprintf(stderr, " [");
					for(j = 0; j < source.getNumUnique(); j++) {
// 						std::cout << " ass " << p.assignment[j][i] << std::endl;
						if(p.assignment[j][i] == 1){
							
							std::cout << "Matching lol " << j << " with " << i << " cost " << simi.at( j * target.getNumUnique()) + i << std::endl;
							
							std::cout << "Matching " <<j * target.getNumUnique()  +i << std::flush << " with " <<  j * target.getNumUnique() + i << std::endl;
							
							std::cout << res.at(j * target.getNumUnique() + i).source << " " << res.at(j * target.getNumUnique() + i).target << std::endl;
							
// 							ZoneCompared m(res.at((j * target.getNumUnique()) + i).source, 
// 										   res.at((j * target.getNumUnique()) + i).target, 
// 										   simi.at( ( i*source.getNumUnique() ) + j));
// 							out.push_back(m);
// 							out.push_back(std::pair<GraphZoneRI::Vertex, GraphZoneRI::Vertex>(
// 								res.at((j * target.getNumUnique()) + i).source, 
// 								res.at((j * target.getNumUnique()) + i).target)
// 							);
							
// 							std::cout << simi.at( ( i*source.getNumUnique() ) + j) <<"==" << (int) (res.at((j * target.getNumUnique()) + i).getSimilarity() * 100 )<< std::endl;
							
// 							assert(simi.at( ( i*source.getNumUnique() ) + j) == (int) (res.at((j * target.getNumUnique()) + i).getSimilarity() * 100 ) );
							
							out.push_back( res.at((j * target.getNumUnique()) + i) );
							
							scores.push_back(simi.at( ( i*source.getNumUnique() ) + j));
						}
					}
					
					
	// 				fprintf(stderr, "]\n");
				}
			}
// 			fprintf(stderr, "\n");
			std::cout << " out " << std::endl;
//Freeing the memory
			int idx;
			for (idx = 0; idx < source.getNumUnique(); idx += 1) {
				std::cout << "free" << std::endl;
				free(m[idx]);
				std::cout << " afterfree" << std::endl;
			}
			std::cout << "final free" << std::endl;
			free(m);
				
				
			std::cout << "outout" << std::endl;
			std::cout << "return " <<out.size() << std::endl;
			
			for(size_t i = 0 ; i < out.size() ; ++i){
				std::cout << "matching " << i << " : " << out[i].source << " " << out[i].target << std::endl;
			}
			
			
			return out;
		}	
	}
}

#endif
