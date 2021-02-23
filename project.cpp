#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include "opencv2/calib3d/calib3d.hpp"
#include <iostream>

using namespace std;
using namespace cv;

vector<Point2f> manualPoints;
vector<Point2f> destPoints;
Mat originalImg;
Mat resImg;

static void onMouse( int event, int x, int y, int, void* )
{
	
	if  ( event == EVENT_LBUTTONDOWN )
	{
		
		if(manualPoints.size()<4)
		{

			circle(originalImg,Point(x,y),10,Scalar(0,69,255),FILLED);
			manualPoints.push_back(Point2f(x, y));
		
		}
		
		else
		{
			
			cout<<"You have already selected forur points required"<<endl;
		
		}
		
		cout << "Right button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
	
	}

}


int main(int argc, char **argv)
{
	
	if(argc != 2)
	{
	
		cout << "Image name not given as argument." << endl;
		return 1;
	
	}
	
	string path = strcat(argv[1],".jpg");
	 
	originalImg = imread(path,1);
	
	if(originalImg.data == NULL)
	{
	
		cout << "Image does not exist." << endl;
		return 2;
	
	}
	 
//	originalImg = imread(path,1);
	
	cvtColor(originalImg,originalImg,COLOR_BGR2GRAY);
	Mat orgCopy = originalImg.clone();

//	circle(originalImg,Point2f(100,200),320,Scalar( 0, 55, 255 ),FILLED,LINE_8 );
	
	namedWindow("Original Frame");
	setMouseCallback( "Original Frame", onMouse, 0 );
	
	while(manualPoints.size()<4)		//manual point selection
	{
		
		imshow("Original Frame",originalImg);
		waitKey(5);
	
	}
	
	imshow("Original Frame",originalImg);
	
	destroyWindow("Original Frame");
	
	return 0;

}
