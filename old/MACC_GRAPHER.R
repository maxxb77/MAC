library(grid)
library(gridExtra)

# Multiple plot function
#
# ggplot objects can be passed in ..., or to plotlist (as a list of ggplot objects)
# - cols:   Number of columns in layout
# - layout: A matrix specifying the layout. If present, 'cols' is ignored.
#
# If the layout is something like matrix(c(1,2,3,3), nrow=2, byrow=TRUE),
# then plot 1 will go in the upper left, 2 will go in the upper right, and
# 3 will go all the way across the bottom.
#
multiplot <- function(..., plotlist=NULL, file, cols=1, layout=NULL) {
  library(grid)
  
  # Make a list from the ... arguments and plotlist
  plots <- c(list(...), plotlist)
  
  numPlots = length(plots)
  
  # If layout is NULL, then use 'cols' to determine layout
  if (is.null(layout)) {
    # Make the panel
    # ncol: Number of columns of plots
    # nrow: Number of rows needed, calculated from # of cols
    layout <- matrix(seq(1, cols * ceiling(numPlots/cols)),
                     ncol = cols, nrow = ceiling(numPlots/cols))
  }
  
  if (numPlots==1) {
    print(plots[[1]])
    
  } else {
    # Set up the page
    grid.newpage()
    pushViewport(viewport(layout = grid.layout(nrow(layout), ncol(layout))))
    
    # Make each plot, in the correct location
    for (i in 1:numPlots) {
      # Get the i,j matrix positions of the regions that contain this subplot
      matchidx <- as.data.frame(which(layout == i, arr.ind = TRUE))
      
      print(plots[[i]], vp = viewport(layout.pos.row = matchidx$row,
                                      layout.pos.col = matchidx$col))
    }
  }
}



#contact: maxwell.l.brown@gmail.com

#load in the necessary libraries
#note that you will need to install these packages
#before loading them. The instructions for gdxrrw
#are in the .zip folder. ggplot2 can be installed
#with install.packages("ggplot2"). Also note
#that GDXRRW requires reshape2
library(gdxrrw)
library(ggplot2)
library(maps)
library(reshape2)
library(plotrix)
library(stringr)

cbbPalette <- c("#000000", "blue", "red", "#009E73", "#F0E442", "#0072B2", "#D55E00", "#CC79A7")

#tell R/GAMS where to find your GDX utilities
igdx("C:/GAMS/win64/24.4")

#specify where the files are stored
gdxdir = "C:/Users/maxwe_000/Documents/gamsdir/projdir/MACC_5"

#set your working directory to where you have your files
setwd(gdxdir)

ELAS_DE_4 = rgdx.param("res_ELAS_DE_4","res_ELAS_DE")
ELAS_DE_5 = rgdx.param("res_ELAS_DE_5","res_ELAS_DE")
ELAS_DE_6 = rgdx.param("res_ELAS_DE_6","res_ELAS_DE")
ELAS_MW_4 = rgdx.param("res_ELAS_MW_4","res_ELAS_MW")
ELAS_MW_5 = rgdx.param("res_ELAS_MW_5","res_ELAS_MW")
ELAS_MW_6 = rgdx.param("res_ELAS_MW_6","res_ELAS_MW")
ELAS_WS_4 = rgdx.param("res_ELAS_WS_4","res_ELAS_WS")
ELAS_WS_5 = rgdx.param("res_ELAS_WS_5","res_ELAS_WS")
ELAS_WS_6 = rgdx.param("res_ELAS_WS_6","res_ELAS_WS")
OBJ_4 = rgdx.param("res_OBJ_4","res_OBJ")
OBJ_5 = rgdx.param("res_OBJ_5","res_OBJ")
OBJ_6 = rgdx.param("res_OBJ_6","res_OBJ")

pq_DE_4 = rgdx.param("res_pq_DE_4_1","res_pq_DE")
pq_DE_5 = rgdx.param("res_pq_DE_5_1","res_pq_DE")
pq_DE_6 = rgdx.param("res_pq_DE_6_1","res_pq_DE")
pq_MW_4 = rgdx.param("res_pq_MW_4_1","res_pq_MW")
pq_MW_5 = rgdx.param("res_pq_MW_5_1","res_pq_MW")
pq_MW_6 = rgdx.param("res_pq_MW_6_1","res_pq_MW")
pq_WS_4 = rgdx.param("res_pq_WS_4_1","res_pq_WS")
pq_WS_5 = rgdx.param("res_pq_WS_5_1","res_pq_WS")
pq_WS_6 = rgdx.param("res_pq_WS_6_1","res_pq_WS")

pq_DE_4_0 = rgdx.param("res_pq_DE_4_0","res_pq_DE")
pq_DE_5_0 = rgdx.param("res_pq_DE_5_0","res_pq_DE")
pq_DE_6_0 = rgdx.param("res_pq_DE_6_0","res_pq_DE")
pq_MW_4_0 = rgdx.param("res_pq_MW_4_0","res_pq_MW")
pq_MW_5_0 = rgdx.param("res_pq_MW_5_0","res_pq_MW")
pq_MW_6_0 = rgdx.param("res_pq_MW_6_0","res_pq_MW")
pq_WS_4_0 = rgdx.param("res_pq_WS_4_0","res_pq_WS")
pq_WS_5_0 = rgdx.param("res_pq_WS_5_0","res_pq_WS")
pq_WS_6_0 = rgdx.param("res_pq_WS_6_0","res_pq_WS")




allobj4 = rgdx.param("res_OBJ_MOD_4","res_obj_mod")
allobj5 = rgdx.param("res_OBJ_MOD_5","res_obj_mod")
allobj6 = rgdx.param("res_OBJ_MOD_6","res_obj_mod")

allelas4 = rgdx.param("res_ELAS_ALL_4","res_ELAS_ALL")
allelas5 = rgdx.param("res_ELAS_ALL_5","res_ELAS_ALL")
allelas6 = rgdx.param("res_ELAS_ALL_6","res_ELAS_ALL")

dpq_DE_4 = dcast(pq_DE_4,i~j)
dpq_DE_5 = dcast(pq_DE_5,i~j)
dpq_DE_6 = dcast(pq_DE_6,i~j)
dpq_MW_4 = dcast(pq_MW_4,i~j)
dpq_MW_5 = dcast(pq_MW_5,i~j)
dpq_MW_6 = dcast(pq_MW_6,i~j)
dpq_WS_4 = dcast(pq_WS_4,i~j)
dpq_WS_5 = dcast(pq_WS_5,i~j)
dpq_WS_6 = dcast(pq_WS_6,i~j)

dpq_DE_4_0 = dcast(pq_DE_4_0,i~j)
dpq_DE_5_0 = dcast(pq_DE_5_0,i~j)
dpq_DE_6_0 = dcast(pq_DE_6_0,i~j)
dpq_MW_4_0 = dcast(pq_MW_4_0,i~j)
dpq_MW_5_0 = dcast(pq_MW_5_0,i~j)
dpq_MW_6_0 = dcast(pq_MW_6_0,i~j)
dpq_WS_4_0 = dcast(pq_WS_4_0,i~j)
dpq_WS_5_0 = dcast(pq_WS_5_0,i~j)
dpq_WS_6_0 = dcast(pq_WS_6_0,i~j)



gpq_DE_4 = melt(dpq_DE_4[,c(2,3,4,7)],id.vars="rp")
gpq_DE_5 = melt(dpq_DE_5[,c(2,3,4,7)],id.vars="rp")
gpq_DE_6 = melt(dpq_DE_6[,c(2,3,4,7)],id.vars="rp")
gpq_MW_4 = melt(dpq_MW_4[,c(2,3,4,6)],id.vars="rp")
gpq_MW_5 = melt(dpq_MW_5[,c(2,3,4,6)],id.vars="rp")
gpq_MW_6 = melt(dpq_MW_6[,c(2,3,4,6)],id.vars="rp")
gpq_WS_4 = melt(dpq_WS_4[,c(2,3,4,12)],id.vars="rp")
gpq_WS_5 = melt(dpq_WS_5[,c(2,3,4,9)],id.vars="rp")
gpq_WS_6 = melt(dpq_WS_6[,c(2,3,4,12)],id.vars="rp")

gpq_DE_4_0 = melt(dpq_DE_4_0[,c(2,3,4,7)],id.vars="rp")
gpq_DE_5_0 = melt(dpq_DE_5_0[,c(2,3,4,7)],id.vars="rp")
gpq_DE_6_0 = melt(dpq_DE_6_0[,c(2,3,4,7)],id.vars="rp")
gpq_MW_4_0 = melt(dpq_MW_4_0[,c(2,3,4,6)],id.vars="rp")
gpq_MW_5_0 = melt(dpq_MW_5_0[,c(2,3,4,6)],id.vars="rp")
gpq_MW_6_0 = melt(dpq_MW_6_0[,c(2,3,4,6)],id.vars="rp")
gpq_WS_4_0 = melt(dpq_WS_4_0[,c(2,3,4,9)],id.vars="rp")
gpq_WS_5_0 = melt(dpq_WS_5_0[,c(2,3,4,12)],id.vars="rp")
gpq_WS_6_0 = melt(dpq_WS_6_0[,c(2,3,4,12)],id.vars="rp")


gpq_DE_4$Model = " Standard"
gpq_DE_5$Model = " Standard"
gpq_DE_6$Model = " Standard"
gpq_MW_4$Model = " Basic"
gpq_MW_5$Model = " Basic"
gpq_MW_6$Model = " Basic"
gpq_WS_4$Model = "Extended"
gpq_WS_5$Model = "Extended"
gpq_WS_6$Model = "Extended"


gpq_DE_4_0$Model = " Standard"
gpq_DE_5_0$Model = " Standard"
gpq_DE_6_0$Model = " Standard"
gpq_MW_4_0$Model = " Basic"
gpq_MW_5_0$Model = " Basic"
gpq_MW_6_0$Model = " Basic"
gpq_WS_4_0$Model = "Extended"
gpq_WS_5_0$Model = "Extended"
gpq_WS_6_0$Model = "Extended"



gpq_DE_4$Scale = "Scaling Method A"
gpq_DE_5$Scale = "Scaling Method B"
gpq_DE_6$Scale = "Scaling Method C"
gpq_MW_4$Scale = "Scaling Method A"
gpq_MW_5$Scale = "Scaling Method B"
gpq_MW_6$Scale = "Scaling Method C"
gpq_WS_4$Scale = "Scaling Method A"
gpq_WS_5$Scale = "Scaling Method B"
gpq_WS_6$Scale = "Scaling Method C"



gpq_DE_4_0$Scale = "Scaling Method A"
gpq_DE_5_0$Scale = "Scaling Method B"
gpq_DE_6_0$Scale = "Scaling Method C"
gpq_MW_4_0$Scale = "Scaling Method A"
gpq_MW_5_0$Scale = "Scaling Method B"
gpq_MW_6_0$Scale = "Scaling Method C"
gpq_WS_4_0$Scale = "Scaling Method A"
gpq_WS_5_0$Scale = "Scaling Method B"
gpq_WS_6_0$Scale = "Scaling Method C"



k = do.call("rbind", list(gpq_DE_4, gpq_DE_5, gpq_DE_6, gpq_MW_4, gpq_MW_5, gpq_MW_6, gpq_WS_4, gpq_WS_5, gpq_WS_6))
k_0 = do.call("rbind", list(gpq_DE_4_0, gpq_DE_5_0, gpq_DE_6_0, gpq_MW_4_0, gpq_MW_5_0, gpq_MW_6_0, gpq_WS_4_0, gpq_WS_5_0, gpq_WS_6_0))

k$variable = gsub("rq"," MACC",k$variable)
k$variable = gsub("base","Unfitted",k$variable)
k$variable = gsub("KLEM_DE","Fitted",k$variable)
k$variable = gsub("KLEM","Fitted",k$variable)
k$variable = gsub("all_WS","Fitted",k$variable)
k$variable = gsub("KLE_WS","Fitted",k$variable)
k$variable = gsub("n123","Fitted",k$variable)

k_0$variable = gsub("rq"," MACC",k_0$variable)
k_0$variable = gsub("base","Unfitted",k_0$variable)
k_0$variable = gsub("KLEM_DE","Fitted",k_0$variable)
k_0$variable = gsub("KLEM","Fitted",k_0$variable)
k_0$variable = gsub("all_WS","Fitted",k_0$variable)
k_0$variable = gsub("KLE_WS","Fitted",k_0$variable)
k_0$variable = gsub("n123","Fitted",k_0$variable)


k$Series = k$variable
k_0$Series = k_0$variable

k$value = 2.41301 - k$value
k_0$value = 2.41301 - k_0$value


ylim1 = -0.03
ylim2 = 150
xlim1 = -0.03
xlim2 = 1.1

breakcol = c(0,0.25,0.5,0.75,1.0,1.25,1.5,1.75,2.0)

p1 = ggplot(k,aes(x=value,y=rp,colour=Series,linetype=Series,shape=Series))+
  facet_grid(Model~Scale)+ 
  ylim(c(ylim1,ylim2))+ 
  scale_x_continuous(breaks=breakcol,limits=c(xlim1,xlim2))+
  scale_colour_manual(values=c("black","red","blue"))+
  ylab("Carbon Price ($ / MTCO2)")+
  xlab("Abatement (Billion MTCO2)")+
  geom_line(size=1.0)+
  scale_linetype_manual(values=c("solid","longdash","dotted"))+
#  geom_point(size=1.25)+
  theme(legend.position="bottom",legend.key.size=unit(1.5,"cm"))


p1_0 = ggplot(k_0,aes(x=value,y=rp,colour=Series,linetype=Series,shape=Series))+
  facet_grid(Model~Scale)+ 
  ylim(c(ylim1,ylim2))+ 
  scale_x_continuous(breaks=breakcol,limits=c(xlim1,xlim2))+
  scale_colour_manual(values=c("black","red","blue"))+
  ylab("Carbon Price ($ / MTCO2)")+
  xlab("Abatement (Billion MTCO2)")+
  geom_line(size=1.0)+
  scale_linetype_manual(values=c("solid","longdash","dotted"))+
  #  geom_point(size=1.25)+
  theme(legend.position="bottom",legend.key.size=unit(1.5,"cm"))

p1
p1_0

#break

#p1 = ggplot(gpq_MW_4, aes(x = value, y = rp, colour = variable,linetype=variable)) + geom_line(size=1.0015)+ scale_x_continuous(breaks=breakcol,limits=c(xlim1,xlim2)) +theme(panel.background = element_rect(colour = "gray85",fill="gray85"),legend.position="none") + scale_colour_manual(values=cbbPalette) + theme(axis.title.x=element_blank(),axis.text.x=element_blank())+ylab("Scaling Method A \n $ / MTCO2E") + ggtitle("Basic") + ylim(c(ylim1,ylim2))
#p2 = ggplot(gpq_MW_5, aes(x = value, y = rp, colour = variable,linetype=variable)) + geom_line(size=1.0015)+ scale_x_continuous(breaks=breakcol,limits=c(xlim1,xlim2)) +theme(panel.background = element_rect(colour = "gray85",fill="gray85"),legend.position="none") + scale_colour_manual(values=cbbPalette) + theme(axis.title.x=element_blank(),axis.text.x=element_blank())+ylab("Scaling Method B \n $ / MTCO2E") + ylim(c(ylim1,ylim2))
#p3 = ggplot(gpq_MW_6, aes(x = value, y = rp, colour = variable,linetype=variable)) + geom_line(size=1.0015)+ scale_x_continuous(name="Abatement (Billion MTCO2E)",limits=c(xlim1,xlim2),breaks=breakcol) +theme(panel.background = element_rect(colour = "gray85",fill="gray85"),legend.position="none") + scale_colour_manual(values=cbbPalette) + ylab("Scaling Method C \n $ / MTCO2E") + ylim(c(ylim1,ylim2))
#p4 = ggplot(gpq_DE_4, aes(x = value, y = rp, colour = variable,linetype=variable)) + geom_line(size=1.0015)+ scale_x_continuous(breaks=breakcol,limits=c(xlim1,xlim2)) +theme(panel.background = element_rect(colour = "gray85",fill="gray85"),legend.position="none") + scale_colour_manual(values=cbbPalette) + theme(axis.title.x=element_blank(),axis.text.x=element_blank())+ theme(axis.title.y=element_blank(),axis.text.y=element_blank())+ggtitle("Standard") + ylim(c(ylim1,ylim2)) 
#p5 = ggplot(gpq_DE_5, aes(x = value, y = rp, colour = variable,linetype=variable)) + geom_line(size=1.0015)+ scale_x_continuous(breaks=breakcol,limits=c(xlim1,xlim2)) +theme(panel.background = element_rect(colour = "gray85",fill="gray85"),legend.position="none") + scale_colour_manual(values=cbbPalette) + theme(axis.title.x=element_blank(),axis.text.x=element_blank())+ theme(axis.title.y=element_blank(),axis.text.y=element_blank()) + ylim(c(ylim1,ylim2)) 
#p6 = ggplot(gpq_DE_6, aes(x = value, y = rp, colour = variable,linetype=variable)) + geom_line(size=1.0015)+ scale_x_continuous(name="Abatement (Billion MTCO2E)",limits=c(xlim1,xlim2),breaks=breakcol) +theme(panel.background = element_rect(colour = "gray85",fill="gray85"),legend.position="none") + scale_colour_manual(values=cbbPalette) + xlab("Billion MTCO2E Abated") + theme(axis.title.y=element_blank(),axis.text.y=element_blank()) + ylim(c(ylim1,ylim2))
#p7 = ggplot(gpq_WS_4, aes(x = value, y = rp, colour = variable,linetype=variable)) + geom_line(size=1.0015)+ scale_x_continuous(breaks=breakcol,limits=c(xlim1,xlim2)) +theme(panel.background = element_rect(colour = "gray85",fill="gray85"),legend.position="none") + scale_colour_manual(values=cbbPalette) + theme(axis.title.x=element_blank(),axis.text.x=element_blank())+ theme(axis.title.y=element_blank(),axis.text.y=element_blank())+ggtitle("Extended") + ylim(c(ylim1,ylim2))
#p8 = ggplot(gpq_WS_5, aes(x = value, y = rp, colour = variable,linetype=variable)) + geom_line(size=1.0015)+ scale_x_continuous(breaks=breakcol,limits=c(xlim1,xlim2)) +theme(panel.background = element_rect(colour = "gray85",fill="gray85"),legend.position="none") + scale_colour_manual(values=cbbPalette) + theme(axis.title.x=element_blank(),axis.text.x=element_blank())+ theme(axis.title.y=element_blank(),axis.text.y=element_blank()) + ylim(c(ylim1,ylim2)) 
#p9 = ggplot(gpq_WS_6, aes(x = value, y = rp, colour = variable,linetype=variable)) + geom_line(size=1.0015)+ scale_x_continuous(name="Abatement (Billion MTCO2E)",limits=c(xlim1,xlim2),breaks=breakcol) +theme(panel.background = element_rect(colour = "gray85",fill="gray85"),legend.position="none") + scale_colour_manual(values=cbbPalette) + xlab("Billion MTCO2E Abated") + theme(axis.title.y=element_blank(),axis.text.y=element_blank()) + ylim(c(ylim1,ylim2))

gpq_MW_4$variable = paste(gpq_MW_4$variable,"_MW",sep="")
gpq_DE_4$variable = paste(gpq_DE_4$variable,"_DE",sep="")
gpq_WS_4$variable = paste(gpq_WS_4$variable,"_WS",sep="")

gpq_MW_5$variable = paste(gpq_MW_5$variable,"_MW",sep="")
gpq_DE_5$variable = paste(gpq_DE_5$variable,"_DE",sep="")
gpq_WS_5$variable = paste(gpq_WS_5$variable,"_WS",sep="")

gpq_MW_6$variable = paste(gpq_MW_6$variable,"_MW",sep="")
gpq_DE_6$variable = paste(gpq_DE_6$variable,"_DE",sep="")
gpq_WS_6$variable = paste(gpq_WS_6$variable,"_WS",sep="")

m1 = rbind(gpq_MW_4,gpq_DE_4,gpq_WS_4)
m2 = rbind(gpq_MW_5,gpq_DE_5,gpq_WS_5)
m3 = rbind(gpq_MW_6,gpq_DE_6,gpq_WS_6)

m1=subset(m1,variable!="rq_MW" & variable!="rq_WS")
m2=subset(m2,variable!="rq_MW" & variable!="rq_WS")
m3=subset(m3,variable!="rq_MW" & variable!="rq_WS")

m1$variable <- gsub('rq_DE', 'MACC', m1$variable)
m1$variable <- gsub('base_DE', ' Standard - Reference', m1$variable)
m1$variable <- gsub('base_MW', ' Basic - Reference', m1$variable)
m1$variable <- gsub('base_WS', ' Expanded - Reference', m1$variable)
m1$variable <- gsub('KLE_WS_WS', ' Expanded - Fitted', m1$variable)
m1$variable <- gsub('KLEM_DE_DE', ' Standard - Fitted', m1$variable)
m1$variable <- gsub('KLEM_MW',    ' Basic - Fitted', m1$variable)
m1$variable <- gsub('all_WS_WS',  ' Expanded - Fitted', m1$variable)


m2$variable <- gsub('rq_DE', 'MACC', m2$variable)
m2$variable <- gsub('base_DE', ' Standard - Reference', m2$variable)
m2$variable <- gsub('base_MW', ' Basic - Reference', m2$variable)
m2$variable <- gsub('base_WS', ' Expanded - Reference', m2$variable)
m2$variable <- gsub('KLE_WS_WS', ' Expanded - Fitted', m2$variable)
m2$variable <- gsub('KLEM_DE_DE', ' Standard - Fitted', m2$variable)
m2$variable <- gsub('KLEM_MW',    ' Basic - Fitted', m2$variable)
m2$variable <- gsub('all_WS_WS',  ' Expanded - Fitted', m2$variable)
m2$variable <- gsub('n123',  ' Expanded - Fitted', m2$variable)

m3$variable <- gsub('rq_DE', ' z MACC', m3$variable)
m3$variable <- gsub('base_DE', ' Standard - Reference', m3$variable)
m3$variable <- gsub('base_MW', ' Basic - Reference', m3$variable)
m3$variable <- gsub('base_WS', ' Expanded - Reference', m3$variable)
m3$variable <- gsub('KLE_WS_WS', ' Expanded - Fitted', m3$variable)
m3$variable <- gsub('KLEM_DE_DE', ' Standard - Fitted', m3$variable)
m3$variable <- gsub('KLEM_MW',    ' Basic - Fitted', m3$variable)
m3$variable <- gsub('all_WS_WS',  ' Expanded - Fitted', m3$variable)
m3$variable <- gsub('n123',  ' Expanded - Fitted', m3$variable)


newpal = c("red","red","grey40","grey40","blue","blue","black")

ltyp = c(2,3,2,3,2,3,1)

m1$Series = m1$variable
m2$Series = m2$variable
m3$Series = m3$variable


m1$value = 2.41301 - m1$value
m2$value = 2.41301 - m2$value
m3$value = 2.41301 - m3$value

g1a = ggplot(m1,aes(x=value,y=rp,colour=Series,linetype=Series))+geom_point(aes(shape=Series),size=1.7) +geom_line(size=1.05) + scale_colour_manual(values=newpal) + xlab("Billion MTCO2E Abated")+ scale_x_continuous(name="Abatement (Billion MTCO2E)",limits=c(xlim1,1.25),breaks=c(0,0.25,0.5,0.75,1.0,1.25,1.5))+ylab("$ / MTCO2E")+scale_linetype_manual(values = ltyp) +scale_shape_manual(values = c(1,1,2,2,3,3,4)) +theme_bw()
g2b = ggplot(m2,aes(x=value,y=rp,colour=Series,linetype=Series))+geom_point(aes(shape=Series),size=1.7) +geom_line(size=1.05) + scale_colour_manual(values=newpal) + xlab("Billion MTCO2E Abated")+ scale_x_continuous(name="Abatement (Billion MTCO2E)",limits=c(xlim1,xlim2),breaks=c(0,0.25,0.5,0.75,1.0,1.25))+ylab("$ / MTCO2E")+scale_linetype_manual(values = ltyp) +scale_shape_manual(values    = c(1,1,2,2,3,3,4)) +theme_bw()
g3c = ggplot(m3,aes(x=value,y=rp,colour=Series,linetype=Series))+geom_point(aes(shape=Series),size=1.7) +geom_line(size=1.05) + scale_colour_manual(values=newpal) + xlab("Billion MTCO2E Abated")+ scale_x_continuous(name="Abatement (Billion MTCO2E)",limits=c(xlim1,xlim2),breaks=c(0,0.25,0.5,0.75,1.0,1.25))+ylab("$ / MTCO2E")+scale_linetype_manual(values = ltyp) +scale_shape_manual(values    = c(1,1,2,2,3,3,4)) +theme_bw()



g1a

#break

theme(legend.key = element_rect(size = 5),legend.key.size = unit(1.5, 'lines'))

g6 = melt(dpq_WS_5[,c(2,3,6,8,9,11,12)],id.vars="rp")

colnames(g6)= c("rp","Series","value")

g6$Series <- gsub('rq', 'MACC', g6$Series)
g6$Series <- gsub('n2', 'Base', g6$Series)
g6$Series <- gsub('all_WS', 'c3', g6$Series)
g6$Series <- gsub('n123', 'c321', g6$Series)
g6$Series <- gsub('KLEM_WS', 'c32', g6$Series)
g6$Series <- gsub('Base3', 'ckle', g6$Series)


gnames=c(expression(paste(hat(sigma)," = ",bar(sigma))),
         expression(sigma['GO']),
         expression(paste("                  ",sigma['GO'],",",sigma['CGO'])),
         expression(paste(sigma['GO'],",",sigma['CGO'],",",sigma['E'])),
         expression(paste(sigma['GO'],",",sigma['CGO'],",",sigma['E'],",",sigma['KLE'])),
         'MACC')


g6$value = 2.41301 - g6$value

newpal2 = c("red","darkgreen","orange","blue","purple","black")

gp = ggplot(g6,aes(x=value,y=rp,group=Series,colour=Series,linetype=Series,shape=Series))+geom_line(size=1.042)+
    geom_point(size=3)+
    xlab("Billion MTCO2E Abated")+ 
    ylab("$ / MTCO2E")+
    theme_bw() +
    scale_colour_manual(values=newpal2,labels=gnames)+
    scale_linetype_manual(values=c(2,3,4,5,6,1),labels=gnames)+
    scale_shape_manual(values=c(1,2,3,4,5,6),labels=gnames)+
    scale_x_continuous(breaks=breakcol,limits=c(xlim1,1.0))
    

gp

#setwd("C:\\Users\\maxwe_000\\Dropbox\\Dissertation\\img")
setwd("C:\\Users\\maxwe_000\\Desktop\\mac_mb9\\newimg")

gwd = 7
ght = 5

ggsave("inelas_fig7.png",g2b,width=gwd,height=ght)
ggsave("inelas_fig8.png",g3c,width=gwd,height=ght)
ggsave("ingrid.png",p1,width=gwd,height=ght)
ggsave("allgrid.png",p1_0,width=gwd,height=ght)

p1