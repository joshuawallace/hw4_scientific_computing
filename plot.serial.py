import numpy as np
import matplotlib.pyplot as pp
from mpl_toolkits.mplot3d.axes3d import Axes3D

serialoutput = "heat_serial.output.dat"

arr = np.loadtxt(serialoutput)#,unpack=True)

x = []
y=[]
print len(arr)
for i in range(len(arr)):
    x.append( (float(i) + .5) * np.pi/float(len(arr)))
    y.append( (float(i) + .5) * np.pi/float(len(arr)))

x,y = np.meshgrid(x,y)

#for i in range(len(arr)):
#    for j in range(len(arr[0])):
 
fig = pp.figure(figsize=(np.pi,np.pi))
pp.xlabel("This is x")

ax = fig.add_subplot(1,1,1,projection='3d')

p=ax.plot_wireframe(x,y,arr,rstride=2,cstride=3)

pp.savefig("serial.pdf")

total = np.sum(arr)
print total
total = total / (float(len(arr)) * float(len(arr[0])))
print total
#pp.show()
