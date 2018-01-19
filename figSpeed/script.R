d<-read.table("results.csv",h=T)

#postscript()
d$size <- (d$size^2)/1000000
F<-log10(t)~size
system("ttf2afm  /usr/share/fonts/TTF/LiberationSans-Regular.ttf > /tmp/LiberationSans-Regular.afm")
system("ttf2afm /usr/share/fonts/TTF/LiberationSans-Italic.ttf > /tmp/LiberationSans-Italic.afm")
system("ttf2afm /usr/share/fonts/TTF/LiberationSans-Bold.ttf > /tmp/LiberationSans-Bold.afm")
system("ttf2afm /usr/share/fonts/TTF/LiberationSans-BoldItalic.ttf > /tmp/LiberationSans-BoldItalic.afm")

Type1Font("test", c(
"/tmp/LiberationSans-Regular.afm"
,"/tmp/LiberationSans-Italic.afm"
,"/tmp/LiberationSans-Bold.afm"
," /tmp/LiberationSans-BoldItalic.afm"
), encoding = "default")

par(family="test")
#try(dev.off())
#arials=c("~/arial.afm", "~/ariali.afm","~/arialbd.afm","~/arialbi.afm")
postscript("/tmp/Rplots.ps",paper="special",width=3.27,height=2.27,pointsize =5)
plot(F,d,pch='', ylab=expression(log[10]("time")*"(ms)"), xlab=expression("Image size"*"(Mpixel)"))

#blue for OpenCFU x for montage
points(F,subset(d,type=="montage"&prog=="OpenCFU"),col='blue',pch='x')
points(F,subset(d,type=="montage"&prog=="NICE"),col='red',pch='x')
points(F,subset(d,type=="montage"&prog=="NIJ"),col='green',pch='x')
points(F,subset(d,type=="scale"&prog=="OpenCFU"),col='blue',pch='o')
points(F,subset(d,type=="scale"&prog=="NICE"),col='red',pch='o')
points(F,subset(d,type=="scale"&prog=="IJ"),col='green',pch='o')

lines(F,subset(d,type=="montage"&prog=="OpenCFU"),col='blue',lty=1)
lines(F,subset(d,type=="montage"&prog=="NICE"),col='red',lty=1)
lines(F,subset(d,type=="montage"&prog=="IJ"),col='green',lty=1)
lines(F,subset(d,type=="scale"&prog=="OpenCFU"),col='blue',lty=2)
lines(F,subset(d,type=="scale"&prog=="NICE"),col='red',lty=2)
lines(F,subset(d,type=="scale"&prog=="IJ"),col='green',lty=2)

legend(x="bottomright",legend=c("Program | image","OpenCFU | tiled","OpenCFU | scaled","NICE | tiled","NICE | scaled","IJM | tiled","IJM | scaled"),pch=c('','x','o','x','o','x','o'),lty=c(0,1,2,1,2,1,2),col=c('black','blue','blue','red','red','green','green'),cex=0.8,lwd=0.5)
#~title("B\t\t")
text(-2.8, 4.5, labels = "B",font=2, xpd=T,cex=2)

dev.off()




#system("convert /tmp/Rplots.ps -resize 1962 -rotate 90 -units PixelsPerInch -density 600 ./figSpeed.png")

system("ps2eps -R + -a -f /tmp/Rplots.ps")
system("cp /tmp/Rplots.eps ./fig2B.eps")
dev.off()
