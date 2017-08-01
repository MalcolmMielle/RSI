#ifndef RSI_EVALUATIONPARAM_31072017
#define RSI_EVALUATIONPARAM_31072017

#include "Evaluation.hpp"

namespace AASS{
	
	namespace RSI{
		
		class EvaluationParam{
		protected:
			
			std::vector<double> _mean_p;
			std::vector<double> _mean_r;
			std::vector<double> _f1_score;
			std::vector<double> _mean_ir;
			std::vector<double> _sd_p;
			std::vector<double> _sd_r;
			std::vector<double> _sd_ir;
			std::vector<double> _t_value;
			std::vector<double> _m_value;
			std::vector<double> _mathiewCC;
			std::vector<double> _gscore;
			std::vector<double> _dor;
			
		public:
			EvaluationParam(){}
			
			void add(double p, double r, double ir, double sdp, double sdr, double sdir, double mCC, double gscore, double f1score, double t, double m){
				_mean_p.push_back(p);
			    _mean_r.push_back(r);
			    _mean_ir.push_back(ir);
			    _sd_p.push_back(sdp);
				_sd_r.push_back(sdr);
				_sd_ir.push_back(sdir);
				_t_value.push_back(t);
				_m_value.push_back(m);
				
				_mathiewCC.push_back(mCC);
				_gscore.push_back(gscore);
				_f1_score.push_back(f1score);
				
			}
			
			void add(Evaluation eval, double t, double m){
				_mean_p.push_back(eval.getMeanPrecision());
			    _mean_r.push_back(eval.getMeanRecall());
			    _mean_ir.push_back(eval.getMeanInverseRecall());
			    _sd_p.push_back(eval.getSDPrecision());
				_sd_r.push_back(eval.getSDRecall());
				_sd_ir.push_back(eval.getSDInverseRecall());
				_t_value.push_back(t);
				_m_value.push_back(m);
				
				_mathiewCC.push_back(eval.getMatthewsCC());
				_gscore.push_back(eval.getGscore());
				_f1_score.push_back(eval.getFscore());
				_dor.push_back(eval.getDOR());
			}
			
			size_t size(){return _mean_p.size();}
			
			void exportAll(const std::string& file_out){
								
				std::string result_file = file_out;
				std::ofstream myfile;
				if(!exists_test3(result_file)){
					myfile.open (result_file);
					myfile << "# mean_p mean_r mean_ir sd_p sd_r sd_ir f1_score g_score dor matthewsCC t m\n";
				}
				else{
					myfile.open (result_file, std::ios::out | std::ios::app);
				}
				
				if (myfile.is_open())
				{
					for(int i = 0 ; i < _mean_p.size() ; ++i){
						
						myfile << _mean_p[i] << " " << _mean_r[i] << " " << _mean_ir[i] << " " << _sd_p[i] << " " << _sd_r[i] << " " << _sd_ir[i] << " " << _f1_score[i] << " " << _gscore[i] << " " << _dor[i] << " " << _mathiewCC[i] << " " << _t_value[i] << " " << _m_value[i] <<"\n";
						
					}
					
					myfile.close();
					
					
				}
				else std::cout << "Unable to open file";
				
			}
			
			
			
				
			
		};
		
	}
}


#endif