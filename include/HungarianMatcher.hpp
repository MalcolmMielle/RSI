#ifndef RSI_HUNGARIANMATCHER_23102016
#define RSI_HUNGARIANMATCHER_23102016

#include "GraphZone.hpp"
#include "hungarian/hungarian.h"


namespace AASS{
	namespace RSI{
		
		
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
			
			 std::vector< std::pair<GraphZone::Vertex, GraphZone::Vertex> > match(GraphZone& source, GraphZone& target, std::vector<int>& scores);
			 
			 const hungarian_problem_t& getHungarianProblem() const { return p;}
			 hungarian_problem_t& getHungarianProblem() { return p;}
			 
			 //TODO : cost is not const !!! save it at the begining
			 int** getCostNonConstOptimized(){return p.cost;}
// 			 const int** getCost() const {return p.cost;}
			
		private:
			
			void createCostMatrix(const GraphZone& source, const GraphZone& target);
			
		};
		
		
		inline std::vector< std::pair<GraphZone::Vertex, GraphZone::Vertex> > AASS::RSI::HungarianMatcher::match(GraphZone& source, GraphZone& target, std::vector<int>& scores){
			
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
				simi.push_back(input);
			}
			
			assert(simi.size() == source.getNumUnique()*target.getNumUnique());
			
			std::cout << "OUT source on rows and target on cols" << std::endl;
			int** m = array_to_matrix(simi,source.getNumUnique(), target.getNumUnique());
			
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
			
			std::vector< std::pair<GraphZone::Vertex, GraphZone::Vertex> > out;
			// This depend on which one as more nodes !
			// Goes along the line
			if(source.getNumUnique() <= target.getNumUnique()){
				
				int i,j;
	// 			fprintf(stderr , "\n");
				for(i=0; i<source.getNumUnique(); i++) {
	// 				fprintf(stderr, " [");
					for(j=0; j<target.getNumUnique(); j++) {
						
// 						std::cout << p.cost[i][j] << " " << std::endl;
						if(p.assignment[i][j] == 1){
							
							std::cout << "Matching lol2" << i << " with " << j << " cost " << simi.at( ( i*target.getNumUnique() ) + j) << std::endl;
							
							std::cout << "Matching " << i * target.getNumUnique() << " with " << (i * target.getNumUnique() )  + j << std::endl;
							std::cout << res.at(i * target.getNumUnique()).source << " " << res.at(( i * target.getNumUnique() ) + j).target << std::endl;
							
							out.push_back(std::pair<GraphZone::Vertex, GraphZone::Vertex>(res.at(i * target.getNumUnique()).source, res.at(( i * target.getNumUnique() ) + j).target));
							scores.push_back(simi.at( ( i*target.getNumUnique() ) + j));
						}
					}
					
					
	// 				fprintf(stderr, "]\n");
				}
			}
			//Goes down the column
			else{
				std::cout << "Source more than target" << std::endl;
				int i,j;
	// 			fprintf(stderr , "\n");
				for(i=0; i<target.getNumUnique(); i++) {
	// 				fprintf(stderr, " [");
					for(j=0; j<source.getNumUnique(); j++) {
// 						std::cout << " ass " << p.assignment[j][i] << std::endl;
						if(p.assignment[j][i] == 1){
							
							std::cout << "Matching lol " << j << " with " << i << " cost " << simi.at( j * target.getNumUnique()) + i << std::endl;
							
							std::cout << "Matching " <<j * target.getNumUnique()  +i << std::flush << " with " <<  j * target.getNumUnique() + i << std::endl;
							
							std::cout << res.at(j * target.getNumUnique() + i).source << " " << res.at(j * target.getNumUnique() + i).target << std::endl;
							
							out.push_back(std::pair<GraphZone::Vertex, GraphZone::Vertex>(
								res.at((j * target.getNumUnique()) + i).source, 
								res.at((j * target.getNumUnique()) + i).target)
							);
							
							scores.push_back(simi.at( ( i*source.getNumUnique() ) + j));
						}
					}
					
					
	// 				fprintf(stderr, "]\n");
				}
			}
// 			fprintf(stderr, "\n");

//Freeing the memory
			int idx;
			for (idx=0; idx < 4; idx+=1) {
				free(m[idx]);
			}
			free(m);
				
				
			std::cout << "outout" << std::endl;
			std::cout << "return " <<out.size() << std::endl;
			
			for(size_t i = 0 ; i < out.size() ; ++i){
				std::cout << "matching " << i << " : " << out[i].first << " " << out[i].second << std::endl;
			}
			
			
			return out;
		}	
	}
}

#endif
