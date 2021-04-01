import matplotlib.pyplot as plt
from matplotlib.pyplot import figure

q_density = []
d_density = []
frame_no = []

with open("densities.txt", "r") as file: 
    data = file.readlines() 
    for line in data[1:]: 
        word = line.split() 
        frame_no.append(float(word[0]))
        q_density.append(float(word[1]))
        d_density.append(float(word[2]))
	
plt.figure(figsize=(9.6, 4.9))

plt.plot(frame_no,q_density)
plt.plot(frame_no,d_density)
plt.xlabel('x - axis') 

plt.ylabel('y - axis') 
  

plt.title('Queue and Dynamic density!') 


plt.savefig('graph.png')  

plt.show() 



