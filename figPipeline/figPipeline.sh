font="/usr/share/fonts/TTF/LiberationSans-Bold.ttf"
for i in $(ls *.tif|grep -v crop|grep -v fig)
	do
	tesT=$(echo $i|grep .2_)
	an=`echo $i | cut  -f 1 -d _`
	an=${an:0:1}
	echo $an

	if [ $tesT ]
	    then
		convert $i +repage -resize 646x646 crop-$i;
		convert crop-$i -fill black -box white -font $font -pointsize 92 -gravity NorthWest -annotate +0+0 "$an" crop-$i
	    else
		tesU=$(echo $i|grep .._)
		if [ $tesU ] 
		    then
	    
			convert $i -crop 200x200+818+1294 +repage -resize 646x646 crop-$i 
			
		    else
			convert $i -crop 200x200+525+819 +repage  -resize 646x646 crop-$i 
		fi
	    
	fi
	
	
	convert crop-$i -units PixelsPerInch -density 600 -border 4x4 crop-$i
	
done

montage crop-* -geometry +0+0 -tile 3x6 fig1.png
convert fig1.png -resize 50% fig1.png
convert fig1.png -units PixelsPerInch -density 600 -compress lzw fig1.tif
convert fig1.tif fig1.eps
#~rm crop-*.tif
