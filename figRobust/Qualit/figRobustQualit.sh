#

figName="fig3A"
for i in $(ls *.tif|grep -v crop|grep -v fig)
	do
	convert $i  -resize 646x646 -units PixelsPerInch -density 600 -bordercolor white  -border 4x4  crop-$i
	
done

montage  crop-* -geometry +0+0 $figName.png
convert $figName.png -rotate 90 $figName.png
#
convert $figName.png  -bordercolor white  -border 646x646  $figName.png
#convert fig2.png -border 200x200  fig2.png

convert +repage $figName.png -crop 3000x3000+258+258 -resize 33% $figName.png

convert $figName.png  -units PixelsPerInch -density 600   $figName.tif
convert $figName.tif ../$figName.eps
rm crop-*.tif
