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
		
		protected:
			hungarian_problem_t p;
			
		public:
			
			HungarianMatcher(){};
			
			//TODO : make sure all memory is freed
			~HungarianMatcher(){
				/* free used memory */
				hungarian_free(&p);
				
				
			}
			
			 std::vector< std::pair<GraphZone::Vertex, GraphZone::Vertex> > match(GraphZone& source, GraphZone& target, std::vector<int>& scores);
			 
			 const hungarian_problem_t& getHungarianProblem() const { return p;}
			 hungarian_problem_t& getHungarianProblem() { return p;}
			 
			 int** getCost(){return p.cost;}
// 			 const int** getCost() const {return p.cost;}
			
		private:
			
			void createCostMatrix(const GraphZone& source, const GraphZone& target);
			
		};
		
		
		inline std::vector< std::pair<GraphZone::Vertex, GraphZone::Vertex> > AASS::RSI::HungarianMatcher::match(GraphZone& source, GraphZone& target, std::vector<int>& scores){
			
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
				std::cout << " pushing " << input << " becaue " << it->getSimilarity()*100 << std::endl;
				simi.push_back(input);
			}
			
			std::cout << "OUT source on rows and target on cols" << std::endl;
			int** m = array_to_matrix(simi,source.getNumVertices(), target.getNumVertices());
			
			std::cout << "INit" << std::endl;
			
			int matrix_size = hungarian_init(&p, m , source.getNumVertices(), target.getNumVertices() , HUNGARIAN_MODE_MINIMIZE_COST);
			
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
			
			std::vector< std::pair<GraphZone::Vertex, GraphZone::Vertex> > out;
			// This depend on which one as more nodes !
			// Goes along the line
			if(source.getNumVertices() <= target.getNumVertices()){
				
				int i,j;
	// 			fprintf(stderr , "\n");
				for(i=0; i<source.getNumVertices(); i++) {
	// 				fprintf(stderr, " [");
					for(j=0; j<target.getNumVertices(); j++) {
						if(p.assignment[i][j] == 1){
							std::cout << "Matching " << i << " with " << j << std::endl;
							std::cout << "Matching " << i * target.getNumVertices() << " with " << (i * target.getNumVertices() )  + j << std::endl;
							
							out.push_back(std::pair<GraphZone::Vertex, GraphZone::Vertex>(res.at(i * target.getNumVertices()).source, res.at(( i * target.getNumVertices() ) + j).target));
							scores.push_back(p.cost[i][j]);
						}
					}
					
					
	// 				fprintf(stderr, "]\n");
				}
			}
			//Goes down the collomns
			else{
				int i,j;
	// 			fprintf(stderr , "\n");
				for(i=0; i<target.getNumVertices(); i++) {
	// 				fprintf(stderr, " [");
					for(j=0; j<source.getNumVertices(); j++) {
						if(p.assignment[j][i] == 1){
							
							std::cout << "Matching " << i << " with " << j << std::endl;
							std::cout << "Matching " << (j * target.getNumVertices()) + i << std::flush << " with " << i * target.getNumVertices() << std::endl;
							
							out.push_back(std::pair<GraphZone::Vertex, GraphZone::Vertex>(res.at((j * target.getNumVertices()) + i).source, res.at(( i * target.getNumVertices() )).target));
							scores.push_back(p.cost[j][i]);
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
			return out;
		}	
	}
}

#endif