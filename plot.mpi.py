import numpy as np
import matplotlib.pyplot as pp
from mpl_toolkits.mplot3d.axes3d import Axes3D

which = "64"
rank = 4

mpioutput = "heat_omp." + which +"." + str(rank) + ".rank.0.output.dat"

arr = np.loadtxt(mpioutput,unpack=True)

#for i in range(1,rank):
#    mpioutput = "heat_omp." + which +"." + str(rank) + ".rank." + str(i) + ".output.dat"
#    np.append(arr, np.loadtxt(mpioutput,unpack=True))

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
#pp.xlabel("This is x")

ax = fig.add_subplot(1,1,1,projection='3d')

rstridenum = len(arr)/(30)
cstridenum = len(arr[0])/(20)
rstridenum=4
cstridenum=4

p=ax.plot_wireframe(x,y,arr,rstride=rstridenum,cstride=cstridenum)

stemp = "mpi." + which  + "." + str(rank) + ".pdf"
pp.savefig(stemp)

total = np.sum(arr)
print total
total = total / (float(len(arr)) * float(len(arr[0])))
print total
#pp.show()
