#~TITLE 
#~ OpenCFU /human correlation and subjectivity test
{
#---------------------------------------------------------------------
#---------------------------------------------------------------------
#~~~~~~~~~~~~~~~~~~AIMS~~~~~~~~~~~~~~~~~~~~~~~~~
#~ demonstrate human subjectivity and slowless compared to OpenCFU
#---------------------------------------------------------------------
#~~~~~~~~~~~~~~~~~~PROTOCOL~~~~~~~~~~~~~~~~~~~~~~
#~ 20 plates between 100 and 300 CFU blindly counted
#---------------------------------------------------------------------
#~~~~~~~~~~~~~~~~~~RESULTS~~~~~~~~~~~~~~~~~~~~~~
#~ oneline
#---------------------------------------------------------------------
#~~~~~~~~~~~~~~~~~~CONCLUSION~~~~~~~~~~~~~~~~~~~
#~ oneline
#---------------------------------------------------------------------

}
#~~~~~~~~~~~~~~~~~~RSCRIPT~~~~~~~~~~~~~~~~~~~~~~
#---------------------------------------------------------------------


#~library(snow)
#~library(pvclust)

#~ source();


system("ttf2afm  /usr/share/fonts/TTF/LiberationSans-Regular.ttf > /tmp/LiberationSans-Regular.afm")
system("ttf2afm /usr/share/fonts/TTF/LiberationSans-Italic.ttf > /tmp/LiberationSans-Italic.afm")
system("ttf2afm /usr/share/fonts/TTF/LiberationSans-Bold.ttf > /tmp/LiberationSans-Bold.afm")
system("ttf2afm /usr/share/fonts/TTF/LiberationSans-BoldItalic.ttf > /tmp/LiberationSans-BoldItalic.afm")
Type1Font("test", c(
"/tmp/LiberationSans-Regular.afm","/tmp/LiberationSans-Italic.afm","/tmp/LiberationSans-Bold.afm"," /tmp/LiberationSans-BoldItalic.afm"), encoding = "default")

m<-as.matrix(read.csv("results.csv",sep="\t")[,-1])

#~ml<-as.data.frame(log10(m))
#~superman <- m[,"Superman"]
#~ml <- m/superman
dev <- as.data.frame(100*(m[,]-m[,1])/m[,1])

par(family="test")
try(dev.off())
postscript("/tmp/Rplots.ps",paper="special",width=3.27,height=2.27,pointsize =5)

clr<-c("blue","red","green")
pp<-1:3
mn<-min(c(dev$OCFU3.HD.12,dev$Nice.High.Mask,dev$IJ.205))
mx<-max(c(dev$OCFU3.HD.12,dev$Nice.High.Mask,dev$IJ.205))

plot(1 ,1,pch=1,col=clr[1],xlim=c(0.5,3.5),ylim=c(mn,mx),xlab=expression(log[10]("Median human counts")),ylab="Deviation to Reference(%)")

abline(lm(dev$OCFU3.HD.12 ~ log10(m[,"Superman"])),col=clr[1])
abline(lm(dev$Nice.High.Mask  ~ log10(m[,"Superman"])),col=clr[2])
abline(lm(dev$IJ.205 ~ log10(m[,"Superman"])),col=clr[3])

abline(h=0,lty=2,lwd=1)
points(dev$OCFU3.HD.12 ~ log10(m[,"Superman"]),pch=1,col=clr[1])
points(dev$Nice.High.Mask ~ log10(m[,"Superman"]),pch=2,col=clr[2])
points(dev$IJ.205 ~ log10(m[,"Superman"]),pch=3,col=clr[3])




print(summary(lm(dev$OCFU3.HD.12 ~ log10(m[,"Superman"])),col=clr[1]))
print(summary(lm(dev$Nice.High.Mask  ~ log10(m[,"Superman"])),col=clr[2]))
print(summary(lm(dev$IJ.205 ~ log10(m[,"Superman"])),col=clr[3]))


legend(x="topright",legend=c("OpenCFU","NICE","IJ"),pch=pp,col=clr,cex=0.8,lwd=0.5)
text(-0.05, 105, labels = "A",font=2, xpd=T,cex=2)

try(dev.off())
system("ps2eps -R + -a -f /tmp/Rplots.ps")
system("cp /tmp/Rplots.eps ./fig3A.eps")


absDev<-abs(dev)
l<-list(Pooled_humans = c(absDev$Louise,absDev$Pip,absDev$Jen ,absDev$Becky,absDev$John,absDev$Helen,absDev$Dav  ),Best_human = absDev$Helen, Worst_human = absDev$Jen,
OpenCFU_HD = absDev$OCFU3.HD.12 ,NICE_HD = absDev$Nice.High.Mask, IJM = absDev$IJ.205,
OpenCFU_WC = absDev$OCFU3.WC.7 ,NICE_WC = absDev$Nice.Low.WC, IJM_WC = absDev$IJ.205.WC)
n<-c("Pool","Best","Worst",rep(c("OFU","NICE","IJM"),times=2))
clr<-c(rep("gray",times=3),rep(c("blue","red","green"),times=2))
names(l)<-n
try(dev.off())
postscript("/tmp/Rplots2.ps",paper="special",width=3.27,height=2.27,pointsize =5)
boxplot(l,ylab="Absolute deviation(%)",outline=FALSE,col=clr)
text(-1.1, 30, labels = "B",font=2, xpd=T,cex=2)
dev.off();
for(i in l){print(wilcox.test(l[[1]],i)$p.value)}

system("ps2eps -R + -a -f /tmp/Rplots2.ps")
system("cp /tmp/Rplots2.eps ./fig3B.eps")

