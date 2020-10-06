
#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <math.h>
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

	string imageFile = "B:\\Temp\\Git\\HandDetection\\HandPose\\gesture1.jpg";
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

	Mat inpBlob = blobFromImage(frame, 1.0 / 255.0, Size(inWidth, inHeight), Scalar(0, 0, 0), false, false);

	net.setInput(inpBlob);

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

	for (int n = 0; n < 20; n++)
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
	
	//create a sample from photo
	FILE *apFile = fopen("B:\\Temp\\Git\\HandDetection\\HandPose\\gesture1.txt", "wt");
	if (apFile != nullptr)
	{
		float minx = 10000, miny = 10000;
		float maxx = -10000, maxy = -10000;
		for (int i = 0; i < 20; i += 4)
		{
			for (int n = i; n < i + 4; n++)
			{
				Point2f partC = points[POSE_PAIRS[n][0]] - points[POSE_PAIRS[0][0]];
				if (partC.x >= maxx)
					maxx = partC.x;
				if (partC.y >= maxy)
					maxy = partC.y;
				if (partC.x <= minx)
					minx = partC.x;
				if (partC.y <= miny)
					miny = partC.y;
			}
		}
		for (int i = 0; i < 20; i += 4)
		{
			for (int n = i; n < i + 4; n++)
			{
				// write coordinates into a file
				Point2f partA = points[POSE_PAIRS[n][0]]- points[POSE_PAIRS[0][0]];
				float d1 = sqrt((maxx - minx)*(maxx - minx) + (maxy - miny)*(maxy - miny));
					partA.x = partA.x / d1;
					partA.y = partA.y / d1;
				fprintf(apFile, "%f %f ", partA.x, partA.y);
				if (n == i + 3)
				{
					Point2f partB = points[POSE_PAIRS[n][1]] - points[POSE_PAIRS[0][0]];
					partB.x = partB.x / d1;
					partB.y = partB.y / d1;
					fprintf(apFile, "%f %f \n", partB.x, partB.y);
				}
			}
		}
		fclose(apFile);
	}
	
	/*
	float minx = 10000, miny = 10000;
	float maxx = -10000, maxy = -10000;
	for (int i = 0; i < 21; i++)
	{
		if (points[i].x >= maxx)
			maxx = points[i].x;
		if (points[i].y >= maxy)
			maxy = points[i].y;
		if (points[i].x <= minx)
			minx = points[i].x;
		if (points[i].y <= miny)
			miny = points[i].y;
	}
	for (int i = 0; i < 21; i++)
	{
			// write coordinates into a file
			Point2f partA = points[POSE_PAIRS[i][0]] - points[POSE_PAIRS[0][0]];
			float d2 = sqrt((maxx - minx)*(maxx - minx) + (maxy - miny)*(maxy - miny));
			points[i].x = points[i].x / d2;
			points[i].y = points[i].y / d2;

	if (n == i + 3)
				{
					Point2f partA = points[POSE_PAIRS[n][1]] - points[POSE_PAIRS[0][0]];
					partA.x = partA.x / d2;
					partA.y = partA.y / d2;
				}

        }
	//This opens a sample file and reads it's coordinates
	float result[5] = { 0 };
	float minresult = 10000;
	for (int nFile = 1; nFile < 6; nFile++)
	{
		char aFilename[256];
		sprintf(aFilename, "B:\\Temp\\Git\\HandDetection\\HandPose\\gesture%d.txt", nFile);
		FILE* apFile = fopen(aFilename, "rt");
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
					for (int i = 0; i < 10; i += 2)	
						points1.push_back(Point2f(aPoints[i], aPoints[i + 1]));
					aHand.push_back(points1);
				}
			}
			float distance[5];
			float accuracy[5];
			float sum[5] = { 0 };
			int i = 1;
			for (int n = 0; n < 5; n++)
			{
				for (int m = 1; m < 5; m++)
				{
					distance[m] = sqrt((aHand[n][m].x - points[i].x)*(aHand[n][m].x - points[i].x) + (aHand[n][m].y - points[i].y)*(aHand[n][m].y - points[i].y));
					sum[n] += distance[m];
					i++;
				}
				accuracy[n] = sum[n] / 4;
			}
			for (int p = 0; p < 5; p++)
			{
				for (int n = 0; n < 5; n++)
					result[p] += accuracy[n];
			}
			for (int n = 0; n < 5; n++)
			{
				result[n] = result[n] / 5;
				if (result[n] < minresult)
					minresult = result[n];
			}
			fclose(apFile);
		}
		for (int n = 0; n < 5; n++)
		{
			if (minresult == result[n])
				cout << aFilename;
		}
	}

	*/
 	t = ((double)cv::getTickCount() - t) / cv::getTickFrequency();
	cout << "Time Taken = " << t << endl;
	imshow("Output-Keypoints", frameCopy);
	imshow("Output-Skeleton", frame);
	imwrite("Output-Skeleton.jpg", frame);

	waitKey();

	return 0;
	}
