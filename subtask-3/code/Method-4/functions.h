#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include "opencv2/calib3d/calib3d.hpp"
#include <opencv2/videoio.hpp>
#include <opencv2/video.hpp>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <cstring>
#include <pthread.h>
#include <utility>
#include <chrono>
#include <iomanip>

using namespace std;
using namespace cv;
//using namespace std::chrono;

extern vector<Point2f> manualPoints;
extern vector<Point2f> destPoints;
extern Mat originalImg;
extern Mat resImg;
extern Mat homograph;

extern void onMouse(int, int, int, int, void*);
int main(int, char **);
void* queueDensity(void*);
void* dynamicDensity(void*);

struct Density{

	vector<pair<int, double>> densities;
	string video;
	
	int init_frame;
	int frame_count;

};
