PROJECT: Traffic Density

SUBTASK:  Method-2

AUTHORS: Aryan Chandra and Kuldeep Meena

INSTRUCTIONS FOR USING THE SOFTWARE:

1. Unzip the directory to your desired location.
2. Add a JPG image that you have to work with in the Method-2 folder of that unzipped directory. 
3. Add a video trafficvideo.mp4 that has to be analysed for traffic densities
3. Open the terminal and go to that unzipped directory and then Method-2 folder.
4. Run the following command on the terminal to create an executable "cam":
	make

5. Run the command "./cam <image_name>" to run the executable giving the image that you have to work with. Note that <image_name> should contain the name of the image not with the extension ".jpg". The software will take care of the extension. And, if image name is not given to the executable or the image file or video is not present, then necessary statements will be shown. 

Eg. If you have to work with the image "empty.jpg" the just run the command as:
	./cam empty

6. After running the previous command, the processing of trafficvideo.mp4 for various densities will start.

7. Note: It might take upto several minutes in the processing so have patience

8. After the processing ends all processed data containing frame number, queue density and dynamic queue desnity will be stored in densities.txt text file and the errors and running times of queue and dynamic densities function with parameter in "method2err.txt".

9. We provide a program named plot.py to make the graph plotting task a bit easier 

10. To execute the program, run the command "python3 plot.py" and then all the plots will be stored in "d_error.png", "d_time.png", "q_error.png" and "q_time.png".

11. Note: In order to execute the program you should have *opencv3(C++)*  and *matplotlib(python)* installed in your system

