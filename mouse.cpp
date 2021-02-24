#include "functions.h"

void onMouse( int event, int x, int y, int, void*){
	
	if(event == EVENT_LBUTTONDOWN )
	{
	
		if(manualPoints.size()<4)
		{

			circle(originalImg,Point(x,y),10,Scalar(0,69,255),FILLED);
			manualPoints.push_back(Point2f(x, y));
			
			//cout << "Right button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;

		}
		
		else
		{
		
			cout<<"You have already selected four points required"<<endl;
		
		}
		
		
	
	}

}
