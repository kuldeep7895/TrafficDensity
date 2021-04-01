#include "functions.h"



vector<Point2f> manualPoints;
vector<Point2f> destPoints;
Mat originalImg;
Mat resImg;


struct thread_data {
   Mat homograph;
   Mat emptyImg;
   vector<Point2f> destPoints;
   Mat frame;
   Rect rect;
   double res;
   int num;
};

struct dynamic_data {
   Mat homograph;
   vector<Point2f> destPoints;
   Mat frame;
   Mat prvs;
   Rect rect;
   Mat res;
   int num;
};


vector<double> errCalc(string file,string ideal){
	vector<double> qDensity1,qDensity2,dDensity2,dDensity1;
	double qErr = 0;
	double dErr = 0;
	fstream curFile,idealFile;
	curFile.open(file,ios::in);
	idealFile.open(ideal,ios::in);
	if (curFile.is_open()){   //checking whether the file is open
	      string tp;
	      getline(curFile, tp);
	      int i = 1;
	      while(getline(curFile, tp)){ 
	      		
			stringstream ss(tp);
			string word;
			int frame;
			double q_density;
			double d_density;
			ss>>word;
			frame = stoi(word);
			ss>>word;
			q_density = stod(word);
			ss>>word;
			d_density = stod(word);
			
			while(i<=frame){
				qDensity1.push_back(q_density);
				dDensity1.push_back(d_density);
				i++;
			}
			

		}
		
	      curFile.close(); 
   	}
   	if (idealFile.is_open()){   //checking whether the file is open
	      string tp;
	      getline(idealFile, tp);
	      int i = 1;
	      while(getline(idealFile, tp)){ 
	      		
			stringstream ss(tp);
			string word;
			int frame;
			double q_density;
			double d_density;
			ss>>word;
			frame = stoi(word);
			ss>>word;
			q_density = stod(word);
			ss>>word;
			d_density = stod(word);
			
			while(i<=frame){
				qDensity2.push_back(q_density);
				dDensity2.push_back(d_density);
				i++;
			}
			

		}
		
	      idealFile.close(); 
   	}
   	for (int i=0;i<qDensity2.size();i++){
   		qErr = qErr+(qDensity2[i]-qDensity1[i])*(qDensity2[i]-qDensity1[i]);
   		//cout << fixed << setprecision(4) << "done1\t" << qErr<<"    "<<dErr<<endl;
   	}
   	
   	qErr = qErr/(qDensity2.size());
   	
   	//cout << fixed << setprecision(4) << "done2\t" << qErr<<"    "<<dErr<<endl;
   	
   	qErr = sqrt(qErr);
   	
   	//cout << fixed << setprecision(4) << "done2\t" << qErr<<"    "<<dErr<<endl;
   	for (int i=0;i<dDensity2.size();i++){
   		dErr = dErr+(dDensity2[i]-dDensity1[i])*(dDensity2[i]-dDensity1[i]);
   		
   		//cout << fixed << setprecision(4) << "done3\t" << qErr<<"    "<<dErr<<endl;
   	}
   	
   	dErr = dErr/(dDensity2.size());
   	//cout << fixed << setprecision(4)<<"done4\t"<<qErr<<"    "<<dErr<<endl;
   	dErr = sqrt(dErr);
   	//cout << fixed << setprecision(4)<<"done4\t"<<qErr<<"    "<<dErr<<endl;
   	vector<double> res;
   	res.push_back(qErr);
   	res.push_back(dErr);
   	return res;
}

void *queueDensity(void *threadarg){

	
	struct thread_data *my_data;
   	my_data = (struct thread_data *) threadarg;
   	
   	Mat homograph = my_data->homograph;
   	Mat emptyImg = my_data->emptyImg;
   	vector<Point2f> destPoints = my_data->destPoints;
   	Mat frame = my_data->frame;
   	Rect rect  = my_data->rect;
	//Mat res1,res2,res,inver,outN,inverQ,emptyWarp;
	Mat res1,outN,cropN,homf;
			absdiff(frame,emptyImg,cropN);
			
			cvtColor(cropN,cropN,COLOR_BGR2GRAY);
			
			threshold( cropN, cropN, 25, 255, THRESH_BINARY );
			
			
			
			double countOn = (double)countNonZero(cropN);
			
			
	my_data->res = countOn;
	
	return NULL;
}


void *dynamicDensity(void *threadarg){

	
	struct dynamic_data *my_data;
   	my_data = (struct dynamic_data *) threadarg;
   	
   	Mat homograph = my_data->homograph;
   	Mat prvs = my_data->prvs;
   	vector<Point2f> destPoints = my_data->destPoints;
   	Mat next = my_data->frame;
   	Rect rect  = my_data->rect;
	//Mat res1,res2,res,inver,outN,inverQ,emptyWarp;
	Mat res1,outN,cropN,homf;

	//cout<<"lknlk"<<endl;

//					
//			imshow("next  ",next);
//			waitKey(5000);
//			imshow("prvs  ",prvs);
//			waitKey(5000);
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
		
			
//			cvtColor(bgr, bgr, COLOR_BGR2GRAY);
//			
//			threshold( bgr, bgr, 5, 255, 3 );	

//			int totalPixels = bgr.rows*bgr.cols;
//			int countOn = countNonZero(bgr);

			
	my_data->res = bgr;

	return NULL;
}



int main(int argc, char **argv)
{

	//errCalc("densities.txt","ideal.txt");
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

	//setMouseCallback("Original Frame", onMouse, 0 );
	manualPoints.push_back(Point2f(961,231));
	manualPoints.push_back(Point2f(1264,219));
	manualPoints.push_back(Point2f(1470,830));
	manualPoints.push_back(Point2f(517,830));

	
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
	//waitKey(0);
	
	imwrite("Cropped_image.jpg", crop_image);
	destroyWindow("Cropped Frame");
	
	
		double ar = 0.5;
		ofstream resout;
		resout.open("method2err.txt");
			
		resout << "ar" << "\t" << "queue error" << "\t\t" << "dynamic error" << "\t\t" << "queue density runtime" <<  "\t\t" << "dynamic density runtime" << endl; 
		for(;ar>=0.5;ar-=0.1){
			for(int i = 0;i<manualPoints.size();i++){
			manualPoints[i].x*=ar;
			manualPoints[i].y*=ar;
			manualPoints[i].x=round(manualPoints[i].x);
			manualPoints[i].y=round(manualPoints[i].y);
			}
			for(int i = 0;i<destPoints.size();i++){
				destPoints[i].x*=ar;
				destPoints[i].y*=ar;
				destPoints[i].x=round(destPoints[i].x);
				destPoints[i].y=round(destPoints[i].y);
			}
			
			Mat homograph = findHomography(manualPoints,destPoints);
			cv::resize(emptyImg, emptyImg, cv::Size(0, 0), ar, ar);
			Rect rect = boundingRect(destPoints);
			double width = (originalImg.size().width);
			warpPerspective(emptyImg,emptyImg,homograph,Size(width,originalImg.size().height));
			emptyImg = emptyImg(rect);
			
	//		imshow("kjnk",emptyImg);
	//		waitKey(5000);
			
			pthread_t threads;
			struct thread_data td;
			td.homograph = homograph;
			td.emptyImg = emptyImg;
			td.destPoints = destPoints;

			
			VideoCapture cap("trafficvideo.mp4");			//Queue Density
			
			if (!cap.isOpened())
		    	{
		    		
				//error in opening the video input
				cerr << "Unable to open file!" << endl;
			
				return {};
		    	
		    	}
		    	

			Mat frame,res1,res2,res,inver,outN,inverQ,emptyWarp;
			
			ofstream output("queue_density.txt");
			
			int count = 0;
			auto start = chrono::steady_clock::now();
			
			while(true)
			{
				
				
				cap >> frame;
				
				double q_density;
			
				
				if (frame.empty()){
					cout<<"empty "<<endl;    
				    break;
				}
				
				if(count % 5 == 0)
				{
					//cout<<"frame "<<count<<endl;
					GaussianBlur(frame, frame, Size(3, 3), 0);
					
					cv::resize(frame, frame, cv::Size(0, 0), ar, ar);
					warpPerspective(frame,frame,homograph,Size(width,originalImg.size().height));
					
					frame = frame(rect);
	//						imshow("kjnk",frame);
	//						waitKey(1);
					
		//			double width = (double)frame.cols/numParts;
					
					
					
					td.frame = frame;
					td.rect = rect;
					td.num = 0;
					int rc = pthread_create(&threads, NULL, queueDensity, (void *)&td); 
					if (rc) {
					   cout << "Error:unable to create thread," << rc << endl;
					   exit(-1);
					} 
					
					
					
					
					pthread_join( threads,NULL);
					//q_density+=i;
					
					
					
					q_density+=td.res;
					
					q_density = q_density/(frame.rows*frame.cols);
					
					//cout<<endl;cl

					
					output << fixed << setprecision(4) << q_density <<"\n";
					
					qDensity.push_back(q_density);
					
					
					
					//cout << fixed << setprecision(4) << count << "\t\t" << q_density << endl;

				}
				count++;

			}

			
	//		auto squareError = [](double a, double b) {
	//		    auto e = a-b;
	//		    return e*e;
	//		};
	//		double sum = transform_reduce(A.begin(), A.end(), B.begin(), 0, std::plus<>(), squareError);
	//		double rmse = sqrt(sum / a.size());
	//				
			
			auto end = chrono::steady_clock::now();
			auto q_diff = end - start;
			
			cout << fixed << setprecision(4) << chrono::duration <double, milli> (q_diff).count() << " ms" << endl;
			
			output.close();
			
			
			// PART 2 DYNAMIC DENSITY 
			
			
			VideoCapture capture("trafficvideo.mp4");		//Dynamic Density
		    	 
		    	if (!capture.isOpened())
		    	{
				cerr << "Unable to open file!" << endl;
		   
				return 0;
		    	
		    	}

		    	
		    	
		    	Mat frame1, prvs;
			capture >> frame1;
			
			cv::resize(frame1, frame1, cv::Size(0, 0), ar, ar);
			cvtColor(frame1, prvs, COLOR_BGR2GRAY);
			
				
			count = 1;
			int i = 0;
			
			cout << "framenum" << "\t" << "queue density" << "\t\t" << "dynamic density" << endl;
			
			ofstream fout;  
		  
			fout.open("densities.txt");
			
			fout << "framenum" << "\t" << "queue density" << "\t\t" << "dynamic density" << endl; 

			pthread_t dthreads;
			struct dynamic_data dtd;
		
				
			dtd.homograph = homograph;
			dtd.destPoints = destPoints;
			
		
			
			start = chrono::steady_clock::now();
			while(true)
			{

				Mat frame2, next;
				capture >> frame2;
				
				if (frame2.empty())
				    break;
				
				if(count % 5 == 0)
				{
				
		//			warpPerspective(frame2,frame2,homograph,Size(frame2.size().width-400,frame2.size().height));
		//			
		//			frame2 = frame2(rect);
					cv::resize(frame2, frame2, cv::Size(0, 0), ar, ar);
					cvtColor(frame2, next, COLOR_BGR2GRAY);
					
					
					dtd.frame = next;
					dtd.prvs = prvs;
						//imshow("Original", next);		
		//			imshow("before 1  ",frameParts[i]);
		//			waitKey(5000);
		//			imshow("before 2  ",frameParts[i]);
		//			waitKey(5000);
					dtd.rect = rect;
					dtd.num = 0;
					int rc = pthread_create(&dthreads, NULL, dynamicDensity, (void *)&dtd); 
					if (rc) {
					   cout << "Error:unable to create thread," << rc << endl;
					   exit(-1);
					} 
					
					
			
						
					pthread_join( dthreads,NULL);
					
					
					
					
					Mat res;
					
					res = dtd.res;
				
					
					
					double density = 0;

					Mat bgr;
					warpPerspective(res,res,homograph,Size(frame2.size().width,frame2.size().height));
					res = res(rect);
					cvtColor(res, bgr, COLOR_BGR2GRAY);
					
					threshold( bgr, bgr, 5, 255, 3 );
					int totalPixels = bgr.rows*bgr.cols;
		//			imshow("before 1  ",bgr);
		//			waitKey(1);
					
					
					int countOn = countNonZero(bgr);
		//			
					//cout<<countOn<<endl;

					density = (double)countOn/(totalPixels);
					
					
					fout << fixed << setprecision(4) << count << "\t\t" << qDensity[i] << "\t\t" << density << endl;
					
					cout << fixed << setprecision(4) << count << "\t\t" << qDensity[i] << "\t\t" << density << endl;
					
					
					prvs = next;
					
					i++;
				}
				
				count++;
		    
		    	}
		    	
		    	fout.close();
		    	end = chrono::steady_clock::now();
			auto d_diff = end - start;
			
			cout << fixed << setprecision(4) << chrono::duration <double, milli> (d_diff).count() << " ms" << endl;
			
			//errCalc("densities.txt","ideal.txt");


			vector<double> resOut = errCalc("densities.txt","ideal.txt");
			resout << fixed << setprecision(4) << ar << "\t\t" << resOut[0] << "\t\t" << resOut[1] << "\t\t" << chrono::duration <double, milli> (q_diff).count() << "\t\t" << chrono::duration <double, milli> (d_diff).count() << endl;
		}
			
	
	return 0;

}

