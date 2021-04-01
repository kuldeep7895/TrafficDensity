#include "functions.h"

vector<Point2f> manualPoints;
vector<Point2f> destPoints;
Mat originalImg;
Mat resImg;
Mat homograph;

vector<double> errCalc(string file,string ideal)
{
	
	vector<double> qDensity1,qDensity2,dDensity2,dDensity1;
	double qErr = 0;
	double dErr = 0;
	fstream curFile,idealFile;
	curFile.open(file,ios::in);
	idealFile.open(ideal,ios::in);
	
	if (curFile.is_open())		//checking whether the file is open
	{   				

	      string tp;
	      getline(curFile, tp);
	      int i = 1;

	      while(getline(curFile, tp))
	      { 
	      		
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
			
			while(i<=frame)
			{
				
				qDensity1.push_back(q_density);
				dDensity1.push_back(d_density);
				i++;
			
			}
			

		}
		
	      	curFile.close(); 
   	
   	}
   	
   	if (idealFile.is_open())		//checking whether the file is open
   	{   
	      
	      string tp;
	      getline(idealFile, tp);
	      int i = 1;

	      while(getline(idealFile, tp))
	      { 
	      		
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
			
			while(i<=frame)
			{
				
				qDensity2.push_back(q_density);
				dDensity2.push_back(d_density);
				i++;
			
			}
			

		}
		
	     	idealFile.close(); 
   	
   	}
   	
   	for (int i=0;i<qDensity2.size();i++)
   	{
   		
   		qErr = qErr+(qDensity2[i]-qDensity1[i])*(qDensity2[i]-qDensity1[i]);
   	
   	}
   	
   	qErr = sqrt(qErr/(qDensity2.size()));
   	
   	for (int i=0;i<dDensity2.size();i++)
   	{
   		
   		dErr = dErr+(dDensity2[i]-dDensity1[i])*(dDensity2[i]-dDensity1[i]);
   	
   	}
   	
   	dErr = sqrt(dErr/(dDensity2.size()));
   	//cout<<"done"<<qErr<<"    "<<dErr<<endl;
   	vector<double> res;
   	res.push_back(qErr);
   	res.push_back(dErr);
   	return res;

}

int main(int argc, char **argv)
{

	if(argc != 2)
	{
	
		cout << "Image name not given as argument." << endl;
		return 1;
	
	}
	
	string path = strcat(argv[1],".jpg");
	 
	originalImg = imread(path);
	
	//Mat emptyImg = imread("empty.png");
	
	//GaussianBlur(emptyImg, emptyImg, Size(3, 3), 0);
	
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

	//while(manualPoints.size()<4)
	//{
		
	//	imshow("Original Frame",originalImg);
	//	waitKey(5);
	
	//}
	
	imshow("Original Frame",originalImg);
	
	destroyWindow("Original Frame");
	
	manualPoints.push_back(Point2f(961,231));
	manualPoints.push_back(Point2f(1264,219));
	manualPoints.push_back(Point2f(1470,830));
	manualPoints.push_back(Point2f(517,830));

	Mat out;

	destPoints.push_back(Point2f(472,52));
	destPoints.push_back(Point2f(800,52));
	destPoints.push_back(Point2f(800,830));
	destPoints.push_back(Point2f(472,830));

	homograph = findHomography(manualPoints,destPoints);
	
	warpPerspective(orgCopy,out,homograph,originalImg.size());
	warpPerspective(orgCopy,out,homograph,Size(originalImg.size().width-400,originalImg.size().height));
	
	Rect rect = boundingRect(destPoints);
	
	Mat crop_image = out(rect);
	
	//imshow("Cropped Frame", crop_image);
	//waitKey(0);
	
	imwrite("Cropped_image.jpg", crop_image);
	//destroyWindow("Cropped Frame");
	
	string video_name = "trafficvideo.mp4";
	
	//Threading
	
	ofstream resout;
	resout.open("method4err.txt");
			
	resout << "Number of threads" << "\t\t" << "queue error" << "\t\t" << "dynamic error" << "\t\t" << "queue density runtime" <<  "\t\t" << "dynamic density runtime" << endl; 
	
	for(int thread_count = 1; thread_count <= 8; thread_count++)
	{
	
		pthread_t thread_id[thread_count],tid[thread_count];

		pthread_attr_t atr;
		pthread_attr_init(&atr);
		
		VideoCapture cap(video_name);
		
		int total_frames= cap.get(CAP_PROP_FRAME_COUNT);
		
		int init_frame = 1;
		
		int f_count = ceil((double)total_frames/thread_count);
		
		vector<pair<int, double>> qDensity, dDensity;
		
		struct Density t[thread_count], d[thread_count];
		
		auto start = chrono::steady_clock::now();
		
		for(int i = 0; i < thread_count; i++)
		{
			t[i].video = "trafficvideo.mp4";
			
			t[i].init_frame = init_frame;
			t[i].frame_count = f_count;
				
			//t[current].flag = false;

			//pthread_create(&thread_id[0], &atr, queueDensity, &t);
				
			pthread_create(&thread_id[i], NULL, queueDensity, (void *)&t[i]);
				
			//vector<double> qDensity = queueDensity("trafficvideo.mp4");
				
			//pthread_join(thread_id[current], NULL);
				
			init_frame += f_count;
		
		}
		
		for(int i = 0; i < thread_count; i++)
		{
				
			pthread_join(thread_id[i], NULL);
			qDensity.insert(qDensity.end(), t[i].densities.begin(), t[i].densities.end());
		
		}
		
		
		auto end = chrono::steady_clock::now();
		auto q_diff = end - start;
			
		cout << fixed << setprecision(4) << chrono::duration <double, milli> (q_diff).count() << " ms" << endl;
		
		
		init_frame = 1;
		
		start = chrono::steady_clock::now();
		
		for(int i = 0; i < thread_count; i++)
		{
			
			d[i].video = "trafficvideo.mp4";	
			d[i].init_frame = init_frame;
			d[i].frame_count = f_count;

			//t[current].flag = false;

			//pthread_create(&thread_id[0], &atr, queueDensity, &t);
				
			pthread_create(&tid[i], NULL, dynamicDensity, (void *)&d[i]);
				
			//vector<double> qDensity = queueDensity("trafficvideo.mp4");
				
			//pthread_join(thread_id[current], NULL);
				
			init_frame += f_count;
		
		}
		
		for(int i = 0; i < thread_count; i++)
		{
				
			pthread_join(tid[i], NULL);
			dDensity.insert(dDensity.end(), d[i].densities.begin(), d[i].densities.end());
		
		}
	
		end = chrono::steady_clock::now();
		auto d_diff = end - start;
				
		cout << fixed << setprecision(4) << chrono::duration <double, milli> (d_diff).count() << " ms" << endl;
		
		ofstream fout;  
	  
		fout.open("densities.txt");
		
		fout << "framenum" << "\t" << "queue density" << "\t\t" << "dynamic density" << endl; 
		
		//fout << count << "\t\t" << qDensity[i] << "\t\t" << density << endl;
				
		//cout << count << "\t\t" << qDensity[i] << "\t\t" << density << endl;
		
		for(int i = 0; i < qDensity.size(); i++)
		{
		
			cout << fixed << setprecision(4) << qDensity[i].first << "\t\t" << qDensity[i].second << "\t\t"  << dDensity[i].second << endl;
			
			fout << fixed << setprecision(4) << qDensity[i].first << "\t\t" << qDensity[i].second << "\t\t" << dDensity[i].second << endl;
		
		}
		
		fout.close();
		
		vector<double> resOut = errCalc("densities.txt","ideal.txt");
		resout << fixed << setprecision(4) << thread_count << "\t\t\t" << resOut[0] << "\t\t\t" << resOut[1] << "\t\t\t" << chrono::duration <double, milli> (q_diff).count() << "\t\t\t" << chrono::duration <double, milli> (d_diff).count() << endl;
		
	}
	
	//cout << duration.count() << endl;

	return 0;

}
