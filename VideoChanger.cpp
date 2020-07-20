#include <iostream>
#include <algorithm>
#include <string>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;
using namespace std;

int main() {
	
	VideoCapture bgCap;
	bgCap.open("C:\\Users\\arvutimees\\Desktop\\student_mas\\student_mas\\student_mas\\background.mp4");
	VideoCapture fgCap;
	fgCap.open("C:\\Users\\arvutimees\\Desktop\\student_mas\\student_mas\\student_mas\\foreground.mp4");
	
	if (!(bgCap.isOpened() && fgCap.isOpened())) {
		return -1;
	}

	Mat foreground, background, fgMask, fgCopy;
	int frameCount = 1;
	int maxFrames = 100;
	vector<Mat> frames;
	while (1) {
		fgCap >> foreground;
		bgCap >> background;
		if (foreground.empty() || background.empty() || maxFrames == 0)
			break;
		resize(foreground, foreground, Size(1920, 1080));
		resize(background, background, Size(1920, 1080));

		cvtColor(foreground, fgCopy, COLOR_BGR2HSV);
		inRange(foreground, Scalar(0, 25,0), Scalar(215, 185, 255), fgMask);

        int largest_area = 0;
        int largest_contour_index = 0;
        Rect bounding_rect;
        vector<vector<Point>> contours; // Vector for storing contour
        vector<Vec4i> hierarchy;
		findContours(fgMask, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
        // iterate through each contour.
        for (int i = 0; i < contours.size(); i++)
        {
            //  Find the area of contour
            double a = contourArea(contours[i], false);
            if (a > largest_area) {
                largest_area = a;
                // Store the index of largest contour
                largest_contour_index = i;
                // Find the bounding rectangle for biggest contour
                bounding_rect = boundingRect(contours[i]);
            }
        }

		
		Mat black(foreground.rows, foreground.cols, CV_8U, Scalar(255, 255, 255));
        cv::drawContours(black, contours, largest_contour_index, Scalar(0,0,0), FILLED, 8, hierarchy);
		black = ~black;
		//rectangle(foreground, bounding_rect, Scalar(0, 255, 0), 2, 8, 0);
		foreground.copyTo(background, black);
		frames.push_back(background);
		//namedWindow("Display window", WINDOW_AUTOSIZE);
		//imshow("Display window", background);
		//waitKey(100000);
		maxFrames--;
		cout << maxFrames;
	}

	VideoWriter video("outcpp.avi", cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 10, Size(1920, 1080));
	for (Mat x : frames)
		video.write(x);

	bgCap.release();
	fgCap.release();
	video.release();
	destroyAllWindows();

	
	waitKey(0);
	return 0;
}

