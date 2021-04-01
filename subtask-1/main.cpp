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
	
	if(originalImg.data == NULL)
	{
	
		cout << "Image does not exist." << endl;
		return 2;
	
	}
	
	cvtColor(originalImg,originalImg,COLOR_BGR2GRAY);
	
	//resize(originalImg, originalImg, Size(originalImg.cols/2, originalImg.rows/2));
	
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
	
	//warpPerspective(orgCopy,out,homograph,originalImg.size());
	warpPerspective(orgCopy,out,homograph,Size(originalImg.size().width-400,originalImg.size().height));
	imshow("Projected Frame",out);
	waitKey(0);
	
	imwrite("Projected_image.jpg", out);
	
	destroyWindow("Projected Frame");
	
	Rect rect = boundingRect(destPoints);
	
	Mat crop_image = out(rect);
	
	imshow("Cropped Frame", crop_image);
	waitKey(0);
	
	imwrite("Cropped_image.jpg", crop_image);
	
	return 0;

}
