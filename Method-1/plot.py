import matplotlib.pyplot as plt
from matplotlib.pyplot import figure

q_error = []
d_error = []
q_time = []
d_time = []
num = []

with open("method1err.txt", "r") as file: 
    data = file.readlines() 
    for line in data[1:]: 
        word = line.split() 
        num.append(float(word[0]))
        q_error.append(float(word[1]))
        d_error.append(float(word[2]))
        q_time.append(float(word[3])/1000)
        d_time.append(float(word[4])/1000)
	
plt.figure(figsize=(9.6, 4.9))

plt.plot(num,q_error)

plt.xlabel('Number of frames dropped') 

plt.ylabel('Queue density errors in units') 
  

plt.title('Queue and Dynamic errors and runtimes for method-1') 


plt.savefig('q_error.png')  

#plt.show() 


plt.figure(figsize=(9.6, 4.9))

plt.plot(num,d_error)

plt.xlabel('Number of frames dropped') 

plt.ylabel('Dynamic density errors in units') 
  

plt.title('Queue and Dynamic errors and runtimes for method-1') 


plt.savefig('d_error.png')


plt.figure(figsize=(9.6, 4.9))

plt.plot(num,q_time)

plt.xlabel('Number of frames dropped') 

plt.ylabel('Queue density running times in seconds') 
  

plt.title('Queue and Dynamic errors and runtimes for method-1') 


plt.savefig('q_time.png')  


plt.figure(figsize=(9.6, 4.9))

plt.plot(num,d_time)

plt.xlabel('Number of frames dropped') 

plt.ylabel('Dynamic density running times in seconds') 
  

plt.title('Queue and Dynamic errors and runtimes for method-1') 


plt.savefig('d_time.png')  
