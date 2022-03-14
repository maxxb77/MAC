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

cbbPalette <- c("#000000", "#E69F00", "#56B4E9", "#009E73", "#F0E442", "#0072B2", "#D55E00", "#CC79A7")



#tell R/GAMS where to find your GDX utilities
igdx("C:/GAMS/win64/24.4")

#specify where the files are stored
gdxdir = "C:/Users/maxwe_000/Documents/gamsdir/projdir/MACCOUT"

#set your working directory to where you have your files
setwd(gdxdir)



ELAS_DE_4 = rgdx.param("ELAS_DE_4","res_ELAS_DE")
ELAS_DE_5 = rgdx.param("ELAS_DE_5","res_ELAS_DE")
ELAS_DE_6 = rgdx.param("ELAS_DE_6","res_ELAS_DE")
ELAS_MW_4 = rgdx.param("ELAS_MW_4","res_ELAS_MW")
ELAS_MW_5 = rgdx.param("ELAS_MW_5","res_ELAS_MW")
ELAS_MW_6 = rgdx.param("ELAS_MW_6","res_ELAS_MW")
ELAS_WS_4 = rgdx.param("ELAS_WS_4","res_ELAS_WS")
ELAS_WS_5 = rgdx.param("ELAS_WS_5","res_ELAS_WS")
ELAS_WS_6 = rgdx.param("ELAS_WS_6","res_ELAS_WS")
OBJ_4 = rgdx.param("OBJ_4","res_OBJ")
OBJ_5 = rgdx.param("OBJ_5","res_OBJ")
OBJ_6 = rgdx.param("OBJ_6","res_OBJ")
pq_DE_4 = rgdx.param("pq_DE_4","res_pq_DE")
pq_DE_5 = rgdx.param("pq_DE_5","res_pq_DE")
pq_DE_6 = rgdx.param("pq_DE_6","res_pq_DE")
pq_MW_4 = rgdx.param("pq_MW_4","res_pq_MW")
pq_MW_5 = rgdx.param("pq_MW_5","res_pq_MW")
pq_MW_6 = rgdx.param("pq_MW_6","res_pq_MW")
pq_WS_4 = rgdx.param("pq_WS_4","res_pq_WS")
pq_WS_5 = rgdx.param("pq_WS_5","res_pq_WS")
pq_WS_6 = rgdx.param("pq_WS_6","res_pq_WS")




dpq_DE_4 = dcast(pq_DE_4,i~j)
dpq_DE_5 = dcast(pq_DE_5,i~j)
dpq_DE_6 = dcast(pq_DE_6,i~j)
dpq_MW_4 = dcast(pq_MW_4,i~j)
dpq_MW_5 = dcast(pq_MW_5,i~j)
dpq_MW_6 = dcast(pq_MW_6,i~j)
dpq_WS_4 = dcast(pq_WS_4,i~j)
dpq_WS_5 = dcast(pq_WS_5,i~j)
dpq_WS_6 = dcast(pq_WS_6,i~j)

gpq_DE_4 = melt(dpq_DE_4[,c(2,3,4,7)],id.vars="rp")
gpq_DE_5 = melt(dpq_DE_5[,c(2,3,4,7)],id.vars="rp")
gpq_DE_6 = melt(dpq_DE_6[,c(2,3,4,7)],id.vars="rp")
gpq_MW_4 = melt(dpq_MW_4[,c(2,3,4,6)],id.vars="rp")
gpq_MW_5 = melt(dpq_MW_5[,c(2,3,4,6)],id.vars="rp")
gpq_MW_6 = melt(dpq_MW_6[,c(2,3,4,6)],id.vars="rp")
gpq_WS_4 = melt(dpq_WS_4[,c(2,3,4,8)],id.vars="rp")
gpq_WS_5 = melt(dpq_WS_5[,c(2,3,4,8)],id.vars="rp")
gpq_WS_6 = melt(dpq_WS_6[,c(2,3,4,8)],id.vars="rp")


gpq_DE_4$value = 2.41301 - gpq_DE_4$value
gpq_DE_5$value = 2.41301 - gpq_DE_5$value
gpq_DE_6$value = 2.41301 - gpq_DE_6$value
gpq_MW_4$value = 2.41301 - gpq_MW_4$value
gpq_MW_5$value = 2.41301 - gpq_MW_5$value
gpq_MW_6$value = 2.41301 - gpq_MW_6$value
gpq_WS_4$value = 2.41301 - gpq_WS_4$value
gpq_WS_5$value = 2.41301 - gpq_WS_5$value
gpq_WS_6$value = 2.41301 - gpq_WS_6$value





ggplot(gpq_WS_6, aes(x = rp, y = value, colour = variable)) + geom_line()
