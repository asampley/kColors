#include <iostream>
#include <sstream>
#include <cstdlib>
#include <cv.hpp>
#include <opencv2/highgui/highgui.hpp>

void printUsage(const char* command) {
	std::cout << "Usage: " << command << "<k> <infile> <outfile>" << std::endl;
}

int main(int argc, char* argv[]) {
	if (argc != 4) {
		printUsage(argv[0]);
		return 1;
	}

	cv::Mat image = cv::imread( argv[2] );
	cv::Mat oImage (image.rows, image.cols, CV_8UC3);
	std::cout << "(" << image.rows << "," << image.cols << "," << image.channels() << ")" << std::endl;
	image = image.reshape( 1, image.rows * image.cols);
	image.convertTo(image, CV_32F);
	std::cout << "(" << image.rows << "," << image.cols << "," << image.channels() << ")" << std::endl;
	
	std::stringstream ss (argv[1]);
	int k = 0;
	if (!(ss >> k)) {
		printUsage(argv[0]);
		return 1;
	}

	cv::Mat labels, centers;
	cv::TermCriteria criteria = cv::TermCriteria( cv::TermCriteria::EPS, 10, 1.0 );
	cv::kmeans( image, k, labels, criteria, 3, cv::KMEANS_PP_CENTERS, centers );

	centers.convertTo( centers, CV_8UC3 );
	for (int i = 0; i < oImage.rows; ++i) {
		for (int j = 0; j < oImage.cols; ++j) {
			oImage.at<cv::Vec3b>(i, j) = centers.at<cv::Vec3b>(labels.at<int>(i * oImage.cols + j));
		}
	}

	cv::imwrite( argv[3], oImage );
}
