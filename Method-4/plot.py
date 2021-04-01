import matplotlib.pyplot as plt
from matplotlib.pyplot import figure

q_error = []
d_error = []
q_time = []
d_time = []
numThreads = []

with open("method4err.txt", "r") as file: 
    data = file.readlines() 
    for line in data[1:]: 
        word = line.split() 
        numThreads.append(float(word[0]))
        q_error.append(float(word[1]))
        d_error.append(float(word[2]))
        q_time.append(float(word[3])/1000)
        d_time.append(float(word[4])/1000)
	
plt.figure(figsize=(9.6, 4.9))

plt.plot(numThreads,q_error)

plt.xlabel('Number of Threads') 

plt.ylabel('Queue density errors in units') 
  

plt.title('Queue and Dynamic errors and runtimes for method-4') 


plt.savefig('q_error.png')  

#plt.show() 


plt.figure(figsize=(9.6, 4.9))

plt.plot(numThreads,d_error)

plt.xlabel('Number of Threads') 

plt.ylabel('Dynamic density errors in units') 
  

plt.title('Queue and Dynamic errors and runtimes for method-4') 


plt.savefig('d_error.png')


plt.figure(figsize=(9.6, 4.9))

plt.plot(numThreads,q_time)

plt.xlabel('Number of Threads') 

plt.ylabel('Queue density running times in seconds') 
  

plt.title('Queue and Dynamic errors and runtimes for method-4') 


plt.savefig('q_time.png')  


plt.figure(figsize=(9.6, 4.9))

plt.plot(numThreads,d_time)

plt.xlabel('Number of Threads') 

plt.ylabel('Dynamic density running times in seconds') 
  

plt.title('Queue and Dynamic errors and runtimes for method-4') 


plt.savefig('d_time.png')  
