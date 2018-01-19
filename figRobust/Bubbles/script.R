d<-read.table("result.csv",h=T)

#postscript()


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





plot(1,1,pch='', xlab="Number of bubbles", ylab="Number of detected objects",xlim=c(0,max(d$Npaches)),ylim=c(0,max(d[,-1])))

#blue for OpenCFU x for montage
#~points(F,subset(d,type=="montage"&prog=="OpenCFU"),col='blue',pch='x')
#~points(F,subset(d,type=="montage"&prog=="NICE"),col='red',pch='x')
#~points(F,subset(d,type=="montage"&prog=="NIJ"),col='green',pch='x')

l<-c("OpenCFU","NiceOtsuHigh","IJ.205")
clr<-c("blue","red","green")
pp<-1:3
print("_____________________________")
for(i in l){
    F<-d[,which(colnames(d)==i)] ~ d$Npaches
    colLoc <- clr[which(l==i)];
    ppLoc <- pp[which(l==i)]
    points( F,col=colLoc,pch=ppLoc)
    mod<-lm(F);
    
    
    abline(mod,col=colLoc,lty=1)
    print(i)
    #print(mod$coefficients)
    print(summary(mod))
    print("_____________________________")
}
#~points(d$Nice ~ d$Npaches ,col='red',pch='o')
#~points(d$IJ ~ d$Npaches ,col='green',pch='o')

#~lines(F,subset(d,type=="montage"&prog=="OpenCFU"),col='blue',lty=1)
#~lines(F,subset(d,type=="montage"&prog=="NICE"),col='red',lty=1)
#~lines(F,subset(d,type=="montage"&prog=="IJ"),col='green',lty=1)
#~lines(F,subset(d,type=="scale"&prog=="OpenCFU"),col='blue',lty=2)
#~lines(F,subset(d,type=="scale"&prog=="NICE"),col='red',lty=2)
#~lines(F,subset(d,type=="scale"&prog=="IJ"),col='green',lty=2)

legend(x="topleft",legend=c("OpenCFU","NICE","IJ"),pch=pp,col=clr,cex=0.8,lwd=0.5)
#~title("B\t\t")
text(-3.55, 35, labels = "C",font=2, xpd=T,cex=2)

dev.off()




#system("convert /tmp/Rplots.ps -resize 1962 -rotate 90 -units PixelsPerInch -density 600 ./figSpeed.png")

system("ps2eps -R + -a -f /tmp/Rplots.ps")
system("cp /tmp/Rplots.eps ../fig3C.eps")
dev.off()
