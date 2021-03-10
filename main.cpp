#include "functions.h"

vector<Point2f> manualPoints;
vector<Point2f> destPoints;
Mat originalImg;
Mat resImg;

int main(int argc, char **argv)
{

	if(argc != 2)
	{
	
		cout << "Image name not given as argument." << endl;
		return 1;
	
	}
	
	string path = strcat(argv[1],".jpg");
	 
	originalImg = imread(path);
	
	Mat emptyImg = imread("empty.png");
	
	GaussianBlur(emptyImg, emptyImg, Size(3, 3), 0);
	
	if(originalImg.data == NULL)
	{
	
		cout << "Image does not exist." << endl;
		return 2;
	
	}
	
	cvtColor(originalImg,originalImg,COLOR_BGR2GRAY);
	
	Mat orgCopy = originalImg.clone();

	namedWindow("Original Frame");

	setMouseCallback("Original Frame", onMouse, 0 );

	while(manualPoints.size()<4)
	{
		
		imshow("Original Frame",originalImg);
		waitKey(5);
	
	}
	
	imshow("Original Frame",originalImg);
	
	destroyWindow("Original Frame");

	Mat out;

	destPoints.push_back(Point2f(472,52));
	destPoints.push_back(Point2f(800,52));
	destPoints.push_back(Point2f(800,830));
	destPoints.push_back(Point2f(472,830));

	Mat homograph = findHomography(manualPoints,destPoints);
	
	warpPerspective(orgCopy,out,homograph,originalImg.size());
	warpPerspective(orgCopy,out,homograph,Size(originalImg.size().width-400,originalImg.size().height));

	Rect rect = boundingRect(destPoints);
	
	Mat crop_image = out(rect);
	
	imshow("Cropped Frame", crop_image);
	waitKey(0);
	
	imwrite("Cropped_image.jpg", crop_image);
	destroyWindow("Cropped Frame");
	
    	VideoCapture cap("trafficvideo.mp4");
    	

	Mat frame,res1,res2,res,inver,outN,inverQ,emptyWarp;
	
	count = 0;
	
	while(true)
	{
		
		Rect rect = boundingRect(destPoints);
		cap >> frame;
		
		double q_density;
		
		if (frame.empty())
		    break;
		
		if(true)
		{
			GaussianBlur(frame, frame, Size(3, 3), 0);
			absdiff(frame,emptyImg,res1);
			
			warpPerspective(res1,outN,homograph,Size(originalImg.size().width-400,originalImg.size().height));
			
			Mat cropN = outN(rect);			// Q
			cvtColor(cropN,cropN,COLOR_BGR2GRAY);
			
			threshold( cropN, cropN, 25, 255, THRESH_BINARY );

			int totalPixels = cropN.rows*cropN.cols;
			int countOn = countNonZero(cropN);
			//int countOff = totalPixels-countOn;
			
			q_density = (double)countOn/totalPixels;
			
			ofstream output("queue_density.txt", std::ios::app);
			
			output << q_density <<"\n";
			output.close();
			
		}
	
		count++;
	
	}
	
	return 0;

}
