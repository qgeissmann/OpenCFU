#!/bin/bash

# to scale image between 100 and 400%
for i in $(seq 100 50 1000)
	do
	convert miniSample.tif -resize $i'%' ./Scale$i.tif 
	done

# to make a mosaic from the same image recursively:

#A=1


#B=11
#  11

#C=111
#  111
#  111 and so on
for j in  4 9 16 25 36 49 64 81 100
	do
	A=$(seq -s "miniSample.tif " $(($j+1)) | sed 's/[0-9]//g')
	montage $A -geometry +0+0 ./Mont$j.tif
	done
