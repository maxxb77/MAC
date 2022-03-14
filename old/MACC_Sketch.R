mac = as.matrix(read.csv("c:/users/maxwe_000/desktop/imp.csv"))

setwd("c:/users/maxwe_000/documents/gamsdir/projdir")

#png("macc.png",height=1080,width=1920)

heights = as.numeric(mac[,3])
widths = as.numeric(mac[,4])

px = as.numeric(mac[,5])
py = as.numeric(mac[,6])

par(mar=c(16,5,7,1))

barplot(heights, widths, space=0, 
        col = "grey",xlab="",ylab="$/tCO2e",
        names.arg=mac[,1],las=2)
axis(1,seq(0, 1277, 250),cex=0.5,pos=250)
mtext("tCO2e", side=3, line=1)
points(x=py,y=px,type="o")

#dev.off()