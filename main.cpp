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
	
	vector<double> qDensity;
	
	Mat emptyImg = imread("empty.png");
	
	GaussianBlur(emptyImg, emptyImg, Size(3, 3), 0);
	
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
	
	warpPerspective(orgCopy,out,homograph,originalImg.size());
	warpPerspective(orgCopy,out,homograph,Size(originalImg.size().width-400,originalImg.size().height));
	//imshow("Projected Frame",out);
	//waitKey(0);
	
	//imwrite("Projected_image.jpg", out);
	
	//destroyWindow("Projected Frame");
	
	Rect rect = boundingRect(destPoints);
	
	Mat crop_image = out(rect);
	
	imshow("Cropped Frame", crop_image);
	waitKey(0);
	
	imwrite("Cropped_image.jpg", crop_image);
	destroyWindow("Cropped Frame");
	
	VideoCapture cap("trafficvideo.mp4");			//Queue Density
	
	if (!cap.isOpened())
    	{
        	//error in opening the video input
        	cerr << "Unable to open file!" << endl;
        
        	return 0;
    	
    	}

	Mat frame,res1,res2,res,inver,outN,inverQ,emptyWarp;
	
	int count = 0;
	
	while(true)
	{
		
		Rect rect = boundingRect(destPoints);
		cap >> frame;
		
		double q_density;
		
		if (frame.empty())
		    break;
		
		if(count % 5 == 0)
		{
			
			GaussianBlur(frame, frame, Size(3, 3), 0);
			absdiff(frame,emptyImg,res1);
			
			warpPerspective(res1,outN,homograph,Size(originalImg.size().width-400,originalImg.size().height));
			
			Mat cropN = outN(rect);			// Q
			cvtColor(cropN,cropN,COLOR_BGR2GRAY);
			
			threshold( cropN, cropN, 25, 255, THRESH_BINARY );

			int totalPixels = cropN.rows*cropN.cols;
			int countOn = countNonZero(cropN);
			
			q_density = (double)countOn/totalPixels;
			
			//ofstream output("queue_density.txt", std::ios::app);
			
			//output << q_density <<"\n";
			
			qDensity.push_back(q_density);
			
			//output.close();
			
		}
	
		count++;
	
	}
	
	VideoCapture capture("trafficvideo.mp4");		//Dynamic Density
    	 
    	if (!capture.isOpened())
    	{
        	//error in opening the video input
        	cerr << "Unable to open file!" << endl;
        
        	return 0;
    	
    	}

    	Mat frame1, prvs;
	capture >> frame1;
	cvtColor(frame1, prvs, COLOR_BGR2GRAY);
	
	count = 1;
	int i = 0;
	
	cout << "framenum" << "\t" << "queue density" << "\t\t" << "dynamic density" << endl;
	
	ofstream fout;  
  
	fout.open("densities.txt");
	
	fout << "framenum" << "\t" << "queue density" << "\t\t" << "dynamic density" << endl; 

	while(true)
	{

		Mat frame2, next;
		capture >> frame2;
		
		if (frame2.empty())
		    break;
		
		if(count % 5 == 0)
		{
		
			cvtColor(frame2, next, COLOR_BGR2GRAY);

			//imshow("Original", next);		
			
			Mat flow(prvs.size(), CV_32FC2);
			calcOpticalFlowFarneback(prvs, next, flow, 0.5, 3, 15, 3, 5, 1.2, 0);
			//calcOpticalFlowFarneback(prvs, next, flow, 0.4, 1, 12, 2, 8, 1.2, 0);

			// visualization
			Mat flow_parts[2];
			split(flow, flow_parts);
			Mat magnitude, angle, magn_norm;
			cartToPolar(flow_parts[0], flow_parts[1], magnitude, angle, true);
			normalize(magnitude, magn_norm, 0.0f, 1.0f, NORM_MINMAX);
			angle *= ((1.f / 360.f) * (180.f / 255.f));

			//build hsv image
			Mat _hsv[3], hsv, hsv8, bgr;
			_hsv[0] = angle;
			_hsv[1] = Mat::ones(angle.size(), CV_32F);
			_hsv[2] = magn_norm;
			merge(_hsv, 3, hsv);
			hsv.convertTo(hsv8, CV_8U, 255.0);
			cvtColor(hsv8, bgr, COLOR_HSV2BGR);
			
			warpPerspective(bgr,bgr,homograph,Size(frame2.size().width-400,frame2.size().height));
			
			bgr = bgr(rect);
			
			cvtColor(bgr, bgr, COLOR_BGR2GRAY);

			//imshow("frame", bgr);
			//waitKey(5);
			
			threshold( bgr, bgr, 5, 255, 3 );	

			int totalPixels = bgr.rows*bgr.cols;
			int countOn = countNonZero(bgr);
			
			double density = ((double)countOn)/((double)totalPixels);
			
			fout << count << "\t\t" << qDensity[i] << "\t\t" << density << endl;
			
			cout << count << "\t\t" << qDensity[i] << "\t\t" << density << endl;

			prvs = next;
			
			i++;
		
		}
		
		count++;
    
    	}
    	
    	fout.close();
	
	return 0;

}
