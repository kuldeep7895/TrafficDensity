import matplotlib.pyplot as plt
from matplotlib.pyplot import figure

g = open("queue_density.txt", "r")

a = []
b = []
i = 0

for k in g:
	a = a + [float(k.split()[0])]
	b = b + [i]
	i =  i+1
	
plt.figure(figsize=(9.6, 4.9))

plt.plot(b,a)
plt.xlabel('x - axis') 

plt.ylabel('y - axis') 
  

plt.title('Queue and Dynamic density!') 


plt.savefig('graph.png')  

plt.show() 



