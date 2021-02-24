PROJECT: Traffic Density

SUBTASK:  Camera angle correction and frame cropping

AUTHORS: Aryan Chandra and Kuldeep Meena

DESCRIPTION: 

The software will take a JPG image from which you want to do camera angle correction and cropping and then a window "Original Frame" will open showing the original image. On selecting four points from the original image, the window containing the original image will close and a new window "Projected Frame" will open displaying top view of the whole original image. After pressing any key, the currently opened window will close and a new window "Cropped Frame" will open displaying the top view of the portion of the original image selected from the 4 points that you chose. After pressing any key, the window currently opened will close and the software will stop the execution.


INSTRUCTIONS FOR USING THE SOFTWARE:

1. Unzip the directory to your desired location.
2. Add a JPG image that you have to work with in that unzipped directory. 
3. Open the terminal and go to that unzipped directory.
4. Run the following command on the terminal to create an executable "cam":
	make

5. Run the command "./cam <image_name>" to run the executable giving the image that you have to work with. Note that <image_name> should contain the name of the image not with the extension ".jpg". The software will take care of the extension. And, if image name is not given to the executable or the image name given is not present, then necessary statements will be shown. 

Eg. If you have to work with the image "empty.jpg" the just run the command as:
	./cam empty

6. After typing the previous command with correct image name, a window "Original Frame" will open displaying the image given to the executable.

7. Select four points from it to later work on that interested portion. The points should be selected in the following manner:

	i) First point to be selected should be on the top left.
	ii) After that, the rest points have to be selected in the clockwise sense i.e.
	
		Top left then Top right then Bottom right then Bottom left.
		

8. After selecting four points, the window "Original Frame" will close and then a new window "Projected Frame" will open displaying top view of the whole original image.

9. After pressing any key, the currently opened window will close and a new window "Cropped Frame" will open displaying the top view of the portion of the original image selected from the 4 points that you chose previously.

10. After pressing any key, the window currently opened will close and the software will stop the execution.
