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
   	}
   	qErr = sqrt(qErr/(qDensity2.size()));
   	for (int i=0;i<dDensity2.size();i++){
   		dErr = dErr+(dDensity2[i]-dDensity1[i])*(dDensity2[i]-dDensity1[i]);
   	}
   	dErr = sqrt(dErr/(dDensity2.size()));
   	//cout << fixed << setprecision(4)<<"done"<<qErr<<"    "<<dErr<<endl;
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
	
		ofstream resout;
		resout.open("method3err.txt");
			
		resout << "numParts" << "\t" << "queue error" << "\t\t" << "dynamic error" << "\t\t" << "queue density runtime" <<  "\t\t" << "dynamic density runtime" << endl; 

		for(int numParts = 3;numParts<=10;numParts++)
		{
	
			double width = (originalImg.size().width-400);
			warpPerspective(emptyImg,emptyImg,homograph,Size(width,originalImg.size().height));
			emptyImg = emptyImg(rect);
			//cout<<"hi"<<endl;
			Mat emptyParts[numParts];
			double wid = (double)emptyImg.cols/numParts;
			for(int i = 0;i<numParts;i++){
				Rect grid_rect(i*wid, 0, wid,emptyImg.rows);
				emptyParts[i] = emptyImg(grid_rect);
			}
			
			pthread_t threads[numParts];
			struct thread_data td[numParts];
			
			
			for(int i = 0;i<numParts;i++){
				
				td[i].homograph = homograph;
				td[i].emptyImg = emptyParts[i];
				td[i].destPoints = destPoints;
			
				
			}
			
			
			
			//vector<double> qDensity;
			
			VideoCapture cap("trafficvideo.mp4");			//Queue Density
			
			if (!cap.isOpened())
		    	{
		    		
				//error in opening the video input
				cerr << "Unable to open file!" << endl;
			
				return {};
		    	
		    	}
		    	

			Mat frame,res1,res2,res,inver,outN,inverQ,emptyWarp;
			
			int count = 0;
			auto start = chrono::steady_clock::now();
			while(true)
			{
				
				Rect rect = boundingRect(destPoints);
				
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
					double width = (originalImg.size().width-400);
					warpPerspective(frame,frame,homograph,Size(width,originalImg.size().height));
					frame = frame(rect);
					Mat frameParts[numParts];
		//			double width = (double)frame.cols/numParts;
					for(int i = 0;i<numParts;i++){
						Rect grid_rect(i*(double)frame.cols/numParts, 0,  (double)frame.cols/numParts,frame.rows);
						frameParts[i] = frame(grid_rect);
					}
					
					for(int i = 0;i<numParts;i++){
						td[i].frame = frameParts[i];
						td[i].rect = rect;
						td[i].num = i%2;
						int rc = pthread_create(&threads[i], NULL, queueDensity, (void *)&td[i]); 
						if (rc) {
						   cout << "Error:unable to create thread," << rc << endl;
						   exit(-1);
						} 
					}
					
					
					for(int i = 0;i<numParts;i++){
						pthread_join( threads[i],NULL);
						//q_density+=i;
					}
					
					for(int i = 0;i<numParts;i++){
						//cout<<td[i].res<<"      ";
						q_density+=td[i].res;
					}
					q_density = q_density/(frame.rows*frame.cols);
					
					//cout<<endl;

					ofstream output("queue_density.txt", std::ios::app);
					
					output << fixed << setprecision(4) << q_density <<"\n";
					
					qDensity.push_back(q_density);
					
					output.close();
					
					//cout << fixed << setprecision(4) << count << "\t\t" << q_density << endl;

				}
				count++;

			}

			
			auto end = chrono::steady_clock::now();
			auto q_diff = end - start;
			
			cout << fixed << setprecision(4) << chrono::duration <double, milli> (q_diff).count() << " ms" << endl;
			
			
			
			
			
			// PART 2 DYNAMIC DENSITY 
			
			
			VideoCapture capture("trafficvideo.mp4");		//Dynamic Density
		    	 
		    	if (!capture.isOpened())
		    	{
				cerr << "Unable to open file!" << endl;
		   
				return 0;
		    	
		    	}

		    	
		    	
		    	Mat frame1, prvs;
			capture >> frame1;
			
			//warpPerspective(frame1,prvs,homograph,Size(frame1.size().width-400,frame1.size().height));
					
			//prvs = frame1(rect);
			
			cvtColor(frame1, prvs, COLOR_BGR2GRAY);
			
			Mat prvsParts[numParts];

			for(int i = 0;i<numParts;i++){
				Rect grid_rect(i*(double)prvs.cols/numParts, 0,  (double)prvs.cols/numParts,prvs.rows);
				prvsParts[i] = prvs(grid_rect);
			}
				
			count = 1;
			int i = 0;
			
			cout << "framenum" << "\t" << "queue density" << "\t\t" << "dynamic density" << endl;
			
			ofstream fout;  
		  
			fout.open("densities.txt");
			
			fout << "framenum" << "\t" << "queue density" << "\t\t" << "dynamic density" << endl; 

			pthread_t dthreads[numParts];
			struct dynamic_data dtd[numParts];
			
			
			for(int i = 0;i<numParts;i++){
				
				dtd[i].homograph = homograph;
				
				dtd[i].destPoints = destPoints;
			
				
			}
			
			
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
				
					cvtColor(frame2, next, COLOR_BGR2GRAY);
					
					Mat frameParts[numParts];
		//			double width = (double)frame.cols/numParts;
					for(int i = 0;i<numParts;i++){
						Rect grid_rect(i*(double)next.cols/numParts, 0,  (double)next.cols/numParts,next.rows);
						frameParts[i] = next(grid_rect);
					}
					
					
					for(int i = 0;i<numParts;i++){
						dtd[i].frame = frameParts[i];
						dtd[i].prvs = prvsParts[i];
						//imshow("Original", next);		
		//			imshow("before 1  ",frameParts[i]);
		//			waitKey(5000);
		//			imshow("before 2  ",frameParts[i]);
		//			waitKey(5000);
						dtd[i].rect = rect;
						dtd[i].num = i%2;
						int rc = pthread_create(&dthreads[i], NULL, dynamicDensity, (void *)&dtd[i]); 
						if (rc) {
						   cout << "Error:unable to create thread," << rc << endl;
						   exit(-1);
						} 
					}
					
					
					
					for(int i = 0;i<numParts;i++){
						
						pthread_join( dthreads[i],NULL);
					}
					
					
					
					Mat res;
					if(numParts!=1){
						hconcat(dtd[0].res, dtd[1].res, res);
					
						for(int i = 2;i<numParts;i++){
							hconcat(res, dtd[i].res, res);
						}
						
					}else{
						res = dtd[0].res;
				
					}
					
					double density = 0;
					
		//			for(int i = 0;i<numParts;i++){
		//				density+=dtd[i].res;
		//			}
					Mat bgr;
					warpPerspective(res,res,homograph,Size(frame2.size().width-400,frame2.size().height));
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
					
					for(int i = 0;i<numParts;i++){
						prvsParts[i] = frameParts[i];
					}
					i++;
				}
				
				count++;
		    
		    	}
		    	
		    	fout.close();
		    	end = chrono::steady_clock::now();
			auto d_diff = end - start;
			
			cout << fixed << setprecision(4) << chrono::duration <double, milli> (d_diff).count() << " ms" << endl;
			
			vector<double> resOut = errCalc("densities.txt","ideal.txt");
			resout << fixed << setprecision(4) << numParts << "\t\t" << resOut[0] << "\t\t" << resOut[1] << "\t\t" << chrono::duration <double, milli> (q_diff).count() << "\t\t" << chrono::duration <double, milli> (d_diff).count() << endl;
		
	
		}
	
	return 0;

}

