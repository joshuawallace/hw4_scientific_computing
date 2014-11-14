import numpy as np
import matplotlib.pyplot as pp
from mpl_toolkits.mplot3d.axes3d import Axes3D

which = "128"

serialoutput = "heat_serial." + which + ".output.dat"

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

rstridenum = len(arr)/(30)
cstridenum = len(arr[0])/(20)

p=ax.plot_wireframe(x,y,arr,rstride=rstridenum,cstride=cstridenum)

stemp = "serial." + which  + ".pdf"
pp.savefig(stemp)

total = np.sum(arr)
print total
total = total / (float(len(arr)) * float(len(arr[0])))
print total
#pp.show()
