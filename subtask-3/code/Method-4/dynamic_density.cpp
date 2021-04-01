#include "functions.h"

void* dynamicDensity(void *d){
	
	struct Density *my_data;
   	my_data = (struct Density *) d;
	
	string video = my_data->video;
	
	int init_frame = my_data->init_frame;
   	int total = my_data->frame_count;
	
	vector<pair<int, double>> dDensity;
	
	Rect rect = boundingRect(destPoints);
	
	VideoCapture capture(video);		//Dynamic Density
    	 
    	if (!capture.isOpened())
    	{
        	//error in opening the video input
        	cerr << "Unable to open file!" << endl;
        
        	return NULL;
    	
    	}
	
	capture.set(cv::CAP_PROP_POS_FRAMES,init_frame - 1);
	
    	Mat frame1, prvs;
	capture >> frame1;
	cvtColor(frame1, prvs, COLOR_BGR2GRAY);
	
	int count = init_frame;
	
	while(true)
	{

		Mat frame2, next;
		capture >> frame2;
		
		if((count == (total + init_frame)) || (frame2.empty()))
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
			
			dDensity.push_back(make_pair(count, density));

			prvs = next;
		
		}
		
		count++;
    
    	}
    	
    	my_data->densities = dDensity;
    	
    	return NULL;
    	
}
