#!/bin/bash

font="/usr/share/fonts/TTF/LiberationSans-Bold.ttf"


for i in $(ls *.tiff | grep ..\.tif |grep -v valid )
	do
	
		convert $i -resize 646x646 valid-$i
		l=$(echo $i|sed s/.tiff//)
		
		echo $l
		l=${l:0:1}
		echo $l
		convert valid-$i -fill black -box white -font $font -pointsize 92 -gravity NorthWest -annotate +0+0 "$l" valid-$i
		#l=$(echo $i|sed s/.tiff//)
	done

for i in $(ls *.tiff | grep ..\.tif -v |grep -v valid)
	do
		convert $i -resize 646x646 valid-$i
	done

montage valid-*.tiff  -border 4x4 -geometry +0+0 -tile 2x6 figVersa.png
convert figVersa.png -units PixelsPerInch -density 600 -compress lzma	  -resize 33% figVersa.tif

#~ convert figVersa.png -units PixelsPerInch -density 600 figVersa.png
convert figVersa.tif figVersa.eps
	
