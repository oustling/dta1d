# This generates simulation of the crossplane profile measured during gantry rotation
# the field is some narrow stripe i.e. 1cm x 40cm (1cm is x size)
# from gantry at 0 degrees the field has 1cm of width, but what size will be, when 
# gantry turns? it will increase, get infinity when gantry is at 90 degrees.

from math import pi, sin, cos
from numpy import convolve

A = []              # our profile will be here
num_of_points = 301 # the points are place every 0.1cm, so this defines the width of profile 
a = 1               # stripe width
ssd = 100           # source surface distance = source izocenter distance
num_of_angles = 200 # num of checked angles between 0 and π/2 

for i in range( 0, num_of_points ): A.append(0.0)

for i in range( 0 , num_of_angles - 1): # that -1 is here because π/2 cause problems
    angle = (i*pi)/(2*num_of_angles)
    x = ssd/(ssd*cos(angle)/a - sin(angle))
    for j in range( 0, num_of_points ):
        x_checked = (j-num_of_points/2+0.5)*0.1
        if(x_checked >= -x and x_checked <= x): A[j] = A[j]+1.0 

for i in range( 0, num_of_points ):
     x = (i-num_of_points/2+0.5)*0.1
     print( x,", ", A[i] )
