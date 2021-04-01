#include "functions.h"

void* queueDensity(void *t){

	struct Density *my_data;
   	my_data = (struct Density *) t;
   	
   	string video = my_data->video;
   	int init_frame = my_data->init_frame;
   	int total = my_data->frame_count;
   	
	//Mat res1,res2,res,inver,outN,inverQ,emptyWarp; 

	Mat emptyImg = imread("empty.png");

	vector<pair<int, double>> qDensity;
	
	GaussianBlur(emptyImg, emptyImg, Size(3, 3), 0);
	
	VideoCapture cap(video);			//Queue Density
	
	if (!cap.isOpened())
    	{
        	//error in opening the video input
        	cerr << "Unable to open file!" << endl;
        
        	//pthread_exit(qDensity);
    	
    	}
    	
    	//double fps = cap.get(CAP_PROP_FPS);

	//cout << fps << endl;

	Mat frame,res1,res2,res,inver,outN,inverQ,emptyWarp;
	
	cap.set(cv::CAP_PROP_POS_FRAMES,init_frame);
	
	int count = init_frame;
	
	while(true)
	{
		
		Rect rect = boundingRect(destPoints);
		cap >> frame;
		
		double q_density;
		
		if((count == (total + init_frame)) || (frame.empty()))
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
			
			qDensity.push_back(make_pair(count, q_density));
			
			//output.close();
			
			//cout << q_density << endl;
			
		}
	
		count++;
	
	}
	
	my_data->densities = qDensity;
	
	//pthread_exit(my_data);
	
	return NULL;

}
