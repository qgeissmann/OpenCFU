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

m<-read.csv("../../figAccuracy/results.csv",sep="\t")[,-1]

par(family="test")
try(dev.off())
postscript("/tmp/Rplots.ps",paper="special",width=3.27,height=2.27,pointsize =5)
#cbind(c(m$OCFU.HD.12.shift ,m$OCFU3.HD.12),rep(c(1,2),each=nrow(m)))
l<-list()
ref <-m$OCFU3.HD.12;
test<- m$OCFU.HD.12.shift
l[[1]] <- test -ref
print(t.test( test,ref , paired=T,alternative="greater"))

test <- m$Nice.High.Shift 
ref <- m$Nice.High.Mask
l[[2]] <- test -ref
print(t.test( test,ref , paired=T,alternative="greater"))

test <- m$IJ.205.shift 
ref <- m$IJ.205
l[[3]] <- test-ref
print(t.test( test,ref , paired=T,alternative="greater"))
print("SDs:")
print(sd(l[[1]]))
print(sd(l[[2]]))
print(sd(l[[3]]))
#~d<-data.frame( bias = c(l[[1]],l[[2]],l[[3]]), group = rep(c("OCFU","NICE","IJM"), each= nrow(m)))


names(l)<-c("OpenCFU","NICE","IJM")

boxplot(l,col=c("blue","red","green"),ylab=("Number of false positive"))
text(-0.029,90, labels = "B",font=2, xpd=T,cex=2)
dev.off();

system("ps2eps -R + -a -f /tmp/Rplots.ps")
system("cp /tmp/Rplots.eps ../fig3B.eps")




#~mean(rev(rs[c(-1,-2)])[c(-1,-2)])
#[1] 0.9993706
#~sd(rev(rs[c(-1,-2)])[c(-1,-2)])
#[1] 0.000496734


