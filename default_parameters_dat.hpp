#ifndef DEFAULT_PARAMETERS_DAT_HPP_
#define DEFAULT_PARAMETERS_DAT_HPP_
#include <string>
#include <vector>
#include <algorithm>
#include <opencv2/opencv.hpp>


class dat_cfg {
public:
	bool show_figures = false;
	int  img_scale_target_diagonal	= 75;
	double search_win_padding = 2;
	double surr_win_factor = 1.9;
	int color_space = 1; //1rgb 2lab 3hsv 4gray
	int num_bins = 16;
	cv::Mat bin_mapping; //getBinMapping(cfg.num_bins);
	double prob_lut_update_rate = 0.05;
	bool distractor_aware = true;
	std::vector<double> adapt_thresh_prob_bins; // 0:0.05 : 1;
	int motion_estimation_history_size = 5;

	int nms_scale = 1;
	double nms_overlap = 0.9;
	double nms_score_factor = 0.5;
	double nms_include_center_vote = true;

};


#endif /* DEFAULT_PARAMETERS_DAT_HPP_ */