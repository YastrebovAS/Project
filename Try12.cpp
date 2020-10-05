/*
#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
using namespace std;
using namespace cv;
using namespace cv::dnn;


const int POSE_PAIRS[20][2] =
{
	{0,1}, {1,2}, {2,3}, {3,4},         // thumb
	{0,5}, {5,6}, {6,7}, {7,8},         // index
	{0,9}, {9,10}, {10,11}, {11,12},    // middle
	{0,13}, {13,14}, {14,15}, {15,16},  // ring
	{0,17}, {17,18}, {18,19}, {19,20}   // small
};

string protoFile = "B:\\Temp\\Git\\HandDetection\\HandPose\\hand\\pose_deploy.prototxt";
string weightsFile = "B:\\Temp\\Git\\HandDetection\\HandPose\\hand\\pose_iter_102000.caffemodel";

int nPoints = 21;

int main(int argc, char **argv)
{

	cout << "USAGE : ./handPoseImage <imageFile> " << endl;

	string imageFile = "B:\\Temp\\Git\\HandDetection\\HandPose\\рука.jpg";
	// Take arguments from command line
	if (argc == 2)
	{
		imageFile = argv[1];
	}

	float thresh = 0.01f;

	Mat frame = imread(imageFile);
	Mat frameCopy = frame.clone();
	int frameWidth = frame.cols;
	int frameHeight = frame.rows;

	float aspect_ratio = frameWidth / (float)frameHeight;
	int inHeight = 368;
	int inWidth = (int(aspect_ratio*inHeight) * 8) / 8;

	cout << "inWidth = " << inWidth << " ; inHeight = " << inHeight << endl;

	double t = (double)cv::getTickCount();
	Net net = readNetFromCaffe(protoFile, weightsFile);

	Mat inpBlob = blobFromImage(frame, 1.0/255.0, Size(inWidth, inHeight), Scalar(0, 0, 0), false, false);

	net.setInput(inpBlob);

	std::vector<String> aVec = net.getLayerNames();
	//string aStr = aVec[0];
	Mat output = net.forward();
	//Mat output = net.forwardAsync();

	int H = output.size[2];
	int W = output.size[3];

	// find the position of the body parts
	vector<Point> points(nPoints);
	for (int n = 0; n < nPoints; n++)
	{
		// Probability map of corresponding body's part.
		Mat probMap(H, W, CV_32F, output.ptr(0, n));
		resize(probMap, probMap, Size(frameWidth, frameHeight));

		Point maxLoc;
		double prob;
		minMaxLoc(probMap, 0, &prob, 0, &maxLoc);
		if (prob > thresh)
		{
			circle(frameCopy, cv::Point((int)maxLoc.x, (int)maxLoc.y), 8, Scalar(0, 255, 255), -1);
			cv::putText(frameCopy, cv::format("%d", n), cv::Point((int)maxLoc.x, (int)maxLoc.y), cv::FONT_HERSHEY_COMPLEX, 1, cv::Scalar(0, 0, 255), 2);

		}
		points[n] = maxLoc;
	}
	int a = sizeof(POSE_PAIRS);

	int nPairs = sizeof(POSE_PAIRS) / sizeof(POSE_PAIRS[0]);

	for (int n = 0; n < 4; n++)
	{
		// lookup 2 connected body/hand parts
		Point2f partA = points[POSE_PAIRS[n][0]];
		Point2f partB = points[POSE_PAIRS[n][1]];

		if (partA.x <= 0 || partA.y <= 0 || partB.x <= 0 || partB.y <= 0)
			continue;

		line(frame, partA, partB, Scalar(0, 255, 255), 8);
		circle(frame, partA, 8, Scalar(0, 0, 255), -1);
		circle(frame, partB, 8, Scalar(0, 0, 255), -1);
	}
	
	//save points into pattern file
	FILE *apFile = fopen("B:\\Temp\\Git\\HandDetection\\HandPose\\gesture5.txt", "wt");
	if (apFile != nullptr)
	{
		for (int i = 0; i < 20; i += 4)
		{
			for (int n = i; n < i + 4; n++)
			{
				// lookup 2 connected body/hand parts
				Point2f partA = points[POSE_PAIRS[n][0]];
				fprintf(apFile, "%f %f ", partA.x, partA.y);
				if (n == i + 3)
				{
					Point2f partB = points[POSE_PAIRS[n][1]];
					fprintf(apFile, "%f %f \n", partB.x, partB.y);
				}
			}
		}
		fclose(apFile);
	}
	//This opens a sample file and reads it's coordinates
	apFile = fopen("B:\\Temp\\Git\\HandDetection\\HandPose\\gesture5.txt", "rt");
	if (apFile != nullptr)
	{
		char aStr[256];
		vector <vector <Point2f>> aHand;
		while (fgets(aStr, 256, apFile) != nullptr)
		{
			float aPoints[10];
			int aRes = sscanf(aStr, "%f %f %f %f %f %f %f %f %f %f", &aPoints[0],
				&aPoints[1],
				&aPoints[2],
				&aPoints[3],
				&aPoints[4],
				&aPoints[5],
				&aPoints[6],
				&aPoints[7],
				&aPoints[8],
				&aPoints[9]);
			{
				vector <Point2f> points1;
				for (int j = 0; j < 10; j += 2)
					points1.push_back(Point2f(aPoints[j], aPoints[j + 1]));
				aHand.push_back(points1);
			}
		}
		fclose(apFile);
	}

	std::vector <std::vector <std::pair<Point2f, Point2f>>> aPatternVec;

	t = ((double)cv::getTickCount() - t) / cv::getTickFrequency();
	cout << "Time Taken = " << t << endl;
	imshow("Output-Keypoints", frameCopy);
	imshow("Output-Skeleton", frame);
	imwrite("Output-Skeleton.jpg", frame);

	waitKey();

	return 0;
}
*/