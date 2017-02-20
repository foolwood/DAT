#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "default_parameters_dat.hpp"
#include "dat_tracker.hpp"

using namespace cv;
using namespace std;

cv::Rect poly2rect(std::vector<cv::Point2f> polygon);
std::vector<cv::Rect> poly2rect(std::vector<std::vector<cv::Point2f> > polygon_v);
std::vector<cv::Rect> getgroundtruth(std::string txt_file);

int main(){

	std::string video_base_path = ".";
	std::string pattern_jpg = video_base_path + "\\sequence\\*.jpg";
	std::string txt_base_path = video_base_path + "\\sequence\\groundtruth.txt";

	std::vector<cv::String> image_files;
	cv::glob(pattern_jpg, image_files);
	if (image_files.size() == 0)
		return -1;
	if (image_files.size() == 0)
		return -1;
	
	std::vector<cv::Rect> groundtruth_rect;
	groundtruth_rect = getgroundtruth(txt_base_path);

	DAT_TRACKER dat;

	cv::Rect location = groundtruth_rect[0];
	cv::Mat image;
	std::vector<cv::Rect> result_rects;
	int64 tic, toc;
	double time = 0;
	bool show_visualization = true;

	for (unsigned int frame = 0; frame < image_files.size(); ++frame) {
		image = cv::imread(image_files[frame]);
		tic = getTickCount();
		if (frame == 0){
			dat.tracker_dat_initialize(image, location);
		}
		else {
			location = dat.tracker_dat_update(image);
		}
		
		toc = cv::getTickCount() - tic;
		time += toc;
		result_rects.push_back(location);

		if (show_visualization) {
			cv::putText(image, to_string(frame + 1), cv::Point(20, 40), 6, 1,
				cv::Scalar(0, 255, 255), 2);
			cv::rectangle(image, groundtruth_rect[frame], cv::Scalar(0, 255, 0), 2);
			cv::rectangle(image, location, cv::Scalar(0, 255, 255), 1);
			cv::imshow("DAT", image);

			char key = cv::waitKey(1);
			if (key == 27 || key == 'q' || key == 'Q')
				break;
		}
	}
	time = time / double(getTickFrequency());
	double fps = double(result_rects.size()) / time;
	std::cout << "fps:" << fps << std::endl;
	cv::destroyAllWindows();

	return 0;
}

cv::Rect poly2rect(std::vector<cv::Point2f> polygon){
// POLY2RECT Convert polygon to rectangle
// Compute axis aligned bounding boxes with correct area and center
	double cx = (polygon[0].x + polygon[1].x + polygon[2].x + polygon[3].x) / 4.;
	double cy = (polygon[0].y + polygon[1].y + polygon[2].y + polygon[3].y) / 4.;
	double x1 = min(min(min(polygon[0].x, polygon[1].x), polygon[2].x), polygon[3].x);
	double x2 = max(max(max(polygon[0].x, polygon[1].x), polygon[2].x), polygon[3].x);
	double y1 = min(min(min(polygon[0].y, polygon[1].y), polygon[2].y), polygon[3].y);
	double y2 = max(max(max(polygon[0].y, polygon[1].y), polygon[2].y), polygon[3].y);
	double A1 = norm(polygon[1] - polygon[2])*norm(polygon[2] - polygon[3]);
	double A2 = (x2 - x1) * (y2 - y1);
	double s = sqrt(A1 / A2);
	double w = s * (x2 - x1) + 1;
	double h = s * (y2 - y1) + 1;
	cv::Rect rect(round(cx - w / 2.) - 1, round(cy - h / 2.) - 1, round(w), round(h));
	return rect;

}


std::vector<cv::Rect> poly2rect(std::vector<std::vector<cv::Point2f> > polygon_v){
// POLY2RECT Convert polygon to rectangle
// Compute axis aligned bounding boxes with correct area and center
	std::vector<cv::Rect> rect_v;
	for (unsigned int i = 0; i < polygon_v.size(); ++i) {
		rect_v.push_back(poly2rect(polygon_v[i]));
	}
	return rect_v;

}

std::vector<cv::Rect> getgroundtruth(std::string txt_file) {
	std::vector<cv::Rect> rects;
	ifstream gt;
	gt.open(txt_file.c_str());
	if (!gt.is_open())
		std::cout << "Ground truth file " << txt_file
		<< " can not be read" << std::endl;
	std::string line;
	float x1, y1, x2, y2, x3, y3, x4, y4;
	while (getline(gt, line)) {
		std::replace(line.begin(), line.end(), ',', ' ');
		stringstream ss;
		ss.str(line);
		ss >> x1 >> y1 >> x2 >> y2 >> x3 >> y3 >> x4 >> y4;
		std::vector<cv::Point2f>polygon;
		polygon.push_back(cv::Point2f(x1, y1));
		polygon.push_back(cv::Point2f(x2, y2));
		polygon.push_back(cv::Point2f(x3, y3));
		polygon.push_back(cv::Point2f(x4, y4));
		rects.push_back(poly2rect(polygon)); //0-index
	}
	gt.close();
	return rects;
}

