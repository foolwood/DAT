#ifndef DAT_TRACKER_HPP_
#define DAT_TRACKER_HPP_
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>
#include <string>
#include <cmath>
#include "default_parameters_dat.hpp"
#include <iostream>

using namespace cv;
using namespace std;

#define PI 3.141592653589793

class DAT_TRACKER
{
public:
	DAT_TRACKER(){ cfg = default_parameters_dat(cfg); };
	~DAT_TRACKER(){}

	void tracker_dat_initialize(cv::Mat I, cv::Rect region);
	
	cv::Rect tracker_dat_update(cv::Mat I);

protected:
	void getNMSRects(cv::Mat prob_map, cv::Size obj_sz, double scale,
		double overlap, double score_frac, cv::Mat dist_map, bool include_inner,
		std::vector<cv::Rect> &top_rects, std::vector<double> &top_vote_scores, std::vector<double> &top_dist_scores);

	void getForegroundBackgroundProbs(cv::Mat frame, cv::Rect obj_rect, int num_bins, cv::Mat bin_mapping, cv::Mat &prob_lut, cv::Mat &prob_map);

	void getForegroundBackgroundProbs(cv::Mat frame, cv::Rect obj_rect, int num_bins, cv::Mat &prob_lut);

	cv::Mat getForegroundDistractorProbs(cv::Mat frame, cv::Rect obj_rect, std::vector<cv::Rect> distractors, int num_bins);

	double getAdaptiveThreshold(cv::Mat prob_map, cv::Rect obj_rect_surr);

	cv::Mat getForegroundProb(cv::Mat frame, cv::Mat prob_lut, cv::Mat bin_mapping);

	cv::Mat CalculateHann(cv::Size sz);

	double intersectionOverUnion(cv::Rect target_rect, cv::Rect candidates);

	void getSubwindowMasked(cv::Mat im, cv::Point pos, cv::Size sz, cv::Mat &out, cv::Mat &mask);

	cv::Point getMotionPrediction(std::vector<cv::Point>values, int maxNumFrames);

	cv::Rect pos2rect(cv::Point obj_center, cv::Size obj_size, cv::Size win_size);

	cv::Rect pos2rect(cv::Point obj_center, cv::Size obj_size);

	cv::Mat getSubwindow(const cv::Mat &frame, cv::Point centerCoor, cv::Size sz);

	cv::Mat getBinMapping(int num_bins);

	dat_cfg default_parameters_dat(dat_cfg cfg);

private:
	dat_cfg cfg;
	double scale_factor_;
	cv::Mat prob_lut_;
	cv::Mat prob_lut_distractor_;
	cv::Mat prob_lut_masked_;
	double adaptive_threshold_;
	std::vector<cv::Point>target_pos_history_;
	std::vector<cv::Size>target_sz_history_;
};

#endif /* DAT_TRACKER_HPP_ */