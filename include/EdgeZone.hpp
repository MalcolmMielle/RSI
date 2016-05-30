#ifndef RSI_EDGEZONE_27052016
#define RSI_EDGEZONE_27052016

#include <boost/math/distributions/normal.hpp>

#include <Zone.hpp>

namespace AASS{
	
	namespace RSI{
		
		class EdgeElement{
		protected:
			double _diff ;
			
		public:
			EdgeElement() : _diff(0){};
			
			double getDiff(){return _diff;}
			double getDiff() const {return _diff;}
			void setDiff(double diff){_diff = diff;}
						
		};
		
		
		inline int getDirection(const EdgeElement& edg, const Zone& src, const Zone& targ, double thresh = 0){
			if(edg.getDiff() < thresh){
				if(src.getValue() > targ.getValue()){
					//Going down
					return 1;
				}
				else{
					//Going Up
					return 2;
				}
			}
			//Same value
			return 0;
			
		}
		
		
		inline std::ostream& operator<<(std::ostream& in, const EdgeElement &p){
			
			in << "d " << p.getDiff();
			return in;
			
		}
		
		inline std::istream& operator>>(std::istream& in, EdgeElement &p){
			char tmp; 
			in >> tmp;
			std::cout << tmp << " ";
			//TODO change to double
			int input;
			in >> input;
			std::cout << input << std::endl;
			p.setDiff(input);
			return in;
		}
		
		
	}
	
}


#endif