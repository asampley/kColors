#include <iostream>
#include <sstream>
#include <cstdlib>
#include <algorithm>
#include <string>
#include <cv.hpp>
#include <opencv2/highgui/highgui.hpp>

void printUsage(const char* command) {
	std::cout << "Usage: " << command << "K INFILE OUTFILE [{rgb,hsv,ycrcb,lab}]" << std::endl;
}

enum Mode { RGB, HSV, YCRCB, LAB };

int main(int argc, char* argv[]) {
	if (argc < 4) {
		printUsage(argv[0]);
		return 1;
	}

	// check mode, default rgb
	Mode mode;
	if (argc >= 4) {
		std::string modeString( argv[3] );
		std::transform( modeString.begin(), modeString.end(), modeString.begin(), ::tolower );
		if ( modeString.compare( "rgb" ) == 0 ) {
			mode = Mode::RGB;
		} else if ( modeString.compare( "hsv" ) == 0 ) {
			mode = Mode::HSV;
		} else if ( modeString.compare( "ycrcb" ) == 0 ) {
			mode = Mode::YCRCB;
		} else if ( modeString.compare( "lab" ) == 0 ) {
			mode = Mode::LAB;
		}
	}

	// read the image
	cv::Mat image = cv::imread( argv[2] );

	// create output image of same size as input
	cv::Mat oImage (image.rows, image.cols, CV_8UC3);

	// convert image to specified color space
	switch (mode) {
	case Mode::RGB:
		break;
	case Mode::HSV:
		cv::cvtColor( image, image, cv::COLOR_BGR2HSV );
		break;
	case Mode::YCRCB:
		cv::cvtColor( image, image, cv::COLOR_BGR2YCrCb );
		break;
	case Mode::LAB:
		cv::cvtColor( image, image, cv::COLOR_BGR2Lab );
		break;
	}

	// flatten image for use in kmeans
	// convert to hsv for better distance function
	//std::cout << "(" << image.rows << "," << image.cols << "," << image.channels() << ")" << std::endl;
	image = image.reshape( 1, image.rows * image.cols);
	image.convertTo(image, CV_32F);
	//std::cout << "(" << image.rows << "," << image.cols << "," << image.channels() << ")" << std::endl;
	
	// read k from arguments
	std::stringstream ss (argv[1]);
	int k = 0;
	if (!(ss >> k)) {
		printUsage(argv[0]);
		return 1;
	}

	// do kmeans
	cv::Mat labels, centers;
	cv::TermCriteria criteria = cv::TermCriteria( cv::TermCriteria::EPS, 10, 1.0 );
	cv::kmeans( image, k, labels, criteria, 3, cv::KMEANS_PP_CENTERS, centers );

	centers.convertTo( centers, CV_8UC3 );
	for (int i = 0; i < oImage.rows; ++i) {
		for (int j = 0; j < oImage.cols; ++j) {
			oImage.at<cv::Vec3b>(i, j) = centers.at<cv::Vec3b>(labels.at<int>(i * oImage.cols + j));
		}
	}

	// convert image back to RGB
	switch (mode) {
	case Mode::RGB:
		break;
	case Mode::HSV:
		cv::cvtColor( oImage, oImage, cv::COLOR_HSV2BGR );
		break;
	case Mode::YCRCB:
		cv::cvtColor( oImage, oImage, cv::COLOR_YCrCb2BGR );
		break;
	case Mode::LAB:
		cv::cvtColor( oImage, oImage, cv::COLOR_Lab2LBGR );
		break;
	}

	cv::imwrite( argv[3], oImage );
}
