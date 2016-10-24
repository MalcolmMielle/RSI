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
			
			
		public:
			
			HungarianMatcher(){};
			
			 std::vector< std::pair<GraphZone::Vertex, GraphZone::Vertex> > match(const GraphZone& source, const GraphZone& target);
			
		private:
			
			void createCostMatrix(const GraphZone& source, const GraphZone& target);
			
		};
		
		
		inline std::vector< std::pair<GraphZone::Vertex, GraphZone::Vertex> > AASS::RSI::HungarianMatcher::match(const GraphZone& source, const GraphZone& target){
			
			hungarian_problem_t p;
			
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
				int input = it->similarity*100;
				std::cout << " pushing " << input << it->source << std::endl;
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
			int i,j;
// 			fprintf(stderr , "\n");
			for(i=0; i<source.getNumVertices(); i++) {
// 				fprintf(stderr, " [");
				for(j=1; j<target.getNumVertices(); j++) {
					if(p.assignment[i][j] == 1){
						std::cout << "Matching " << i << " with " << j << std::endl;
						std::cout << "Matching " << res[i * target.getNumVertices()].source << " with " << res[( i * target.getNumVertices() ) + j].target << std::endl;
						out.push_back(std::pair<GraphZone::Vertex, GraphZone::Vertex>(res[i * target.getNumVertices()].source, res[( i * target.getNumVertices() ) + j].target));
					}
				}
				
				
// 				fprintf(stderr, "]\n");
			}
// 			fprintf(stderr, "\n");
			
			

			/* free used memory */
			hungarian_free(&p);
			
			int idx;
			for (idx=0; idx < 4; idx+=1) {
				free(m[idx]);
			}
			free(m);
			
			return out;
		}	
	}
}

#endif
