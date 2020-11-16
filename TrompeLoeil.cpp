// Include required header files from OpenCV directory 
#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include <cstdlib>
#include <iostream>
#include <chrono>
#include <string>

using namespace std;
using namespace cv;

// Function for Face Detection
Point detectCenter(Mat& img, CascadeClassifier& cascade, int (&prev_c)[2]);
string cascadeName;

int main(int argc, const char** argv)
{
	VideoCapture capture;
	Mat frame, image;

	CascadeClassifier cascade;
	cascade.load("haarcascades/haarcascade_frontalface_default.xml");

	int inc_num[2] = { 30, 20 };
	int block_size[2] = { 1280 / inc_num[0], 720 / inc_num[1] };
	int prev_loc[2] = { 640, 360 };

	// Start Video
	capture.open(0);

	// Set capture resolution to 1280x720
	capture.set(3, 1280.0);
	capture.set(4, 720.0);

	if (capture.isOpened())
	{
		// Capture frames from video and detect faces 
		cout << "Face Detection Started...." << endl;
		while (1)
		{
			// Begin timer for frame
			chrono::steady_clock::time_point begin = chrono::steady_clock::now();

			capture >> frame;
			if (frame.empty())
				break;
			Mat frame1 = frame.clone();
			Point center = detectCenter(frame1, cascade, prev_loc);

			int block[2] = { int(prev_loc[0] / block_size[0]), int(prev_loc[1] / block_size[1]) };
			int frame_num = inc_num[1] - block[1] - 1 + inc_num[1] * (inc_num[0] - block[0]);

			string fpath = "warped/" + to_string(frame_num) + ".jpg";
			//string fpath = "non_warped/" + to_string(frame_num) + ".png";
			Mat img = cv::imread(fpath);

			//cv::resize(img, img, Size(720, 720));
			cv::imshow("Vid", img);


			char c = (char)waitKey(10);

			// Press q to exit from window 
			if (c == 27 || c == 'q' || c == 'Q')
				break;
			
			// End timer for frame and display time
			chrono::steady_clock::time_point end = chrono::steady_clock::now();
			cout << "Time difference = " << chrono::duration_cast<chrono::microseconds>(end - begin).count()/1000 << "[ms]\n";
		}
	}
	else
		cout << "Could not Open Camera";
	return 0;
}

Point detectCenter(Mat& img, CascadeClassifier& cascade, int (&prev_c)[2])
{
	vector<Rect> faces;
	Mat gray;
	Scalar color = Scalar(255, 0, 0); // Box color
	Point center;

	cvtColor(img, gray, COLOR_BGR2GRAY); // Convert to Gray Scale
	// equalizeHist(gray, gray);

	// Detect faces of different sizes using cascade classifier 
	cascade.detectMultiScale(gray, faces, 1.1, 2);

	// Draw boxes around the face
	for (size_t i = 0; i < faces.size(); i++)
	{
		Rect r = faces[i];

		center.x = cvRound(r.x + r.width * 0.5);
		center.y = cvRound(r.y + r.height * 0.5);

		// Check if the face is not too small and that it's close to the previous detection location
		if (r.width < 100 || abs(prev_c[0] - center.x) + abs(prev_c[1] - center.y) > 200) continue;

		// Set the previous center = to the new center
		prev_c[0] = center.x, prev_c[1] = center.y;

		//rectangle(img, cv::Point(r.x, r.y), cv::Point(r.x + r.width - 1, r.y + r.height - 1), color, 3);
		break;
	}

	//cv::imshow("Face Detection", img);
	return center;
}
