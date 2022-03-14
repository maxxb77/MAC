

Table MCK1(Z,pq) "MACC data points"
      P       Q
1     1       0.80
2     9       0.818
3     20      0.938
4     29      0.968
5     44      1.358
6     50      1.60
;
Table MCK2(Z,pq)
         P         Q
1        1         0.06
2        9         0.13
3        20        0.25
4        29        0.3
5        44        0.59
6        50        0.77
*derived from:   http://www.mckinsey.com/~/media/McKinsey/dotcom/client_service/Sustainability/cost%20curve%20PDFs/US_ghg_final_report.ashx
*pg 59 in combination with  estimates on pg 34
;

Table BLMBRG1(Z,pq)
          P         Q
1         2       0.02
2        25       0.18
3        46       0.27
4        58       0.30
5        69       0.40
6        73       0.47
;
*measured from figure 12 here:
*http://bnef.com/InsightDownload/7100/pdf/

Table BLMBRG2(Z,pq)
          P           Q
1        5.90         0.10
2        23.29        0.33
3        56.52        0.63
4        73.29        0.72
5        81.99        0.81
6        90.99        0.95
;
*Measured from Figure 1 here:
*http://bnef.com/InsightDownload/7100/pdf/

Table MACC_FIN(Z,pq) "Final data points for MACC, from BNEF's 2030 MACC"
               P       Q
1           1e-4      1e-4
2              6      48
3             16      86
4             23     178
5             31     201
6             38     220
7             41     254
8             43     293
9             52     378
10            63     447
11            65     516
12            78     743
13            90     785
14            95     896
15           102     935
16           105    1022
17           113    1036
18           126    1129
*19           157    1206
;


*Table MACC_ORIG(Z,pq)
*       P        Q
*1    -42.79    48.18
*2    -30.82    37.76
*3    -22.30    92.45
*4    -16.89    22.14
*5    -7.21     19.53
*6    -3.28     33.85
*7    -1.04     39.06
*8     1.00     84.64
*9     19.02    69.01
*10    20.82    69.01
*11    22.95    27.86
*12    46.56    71.67
*13    47.05    20.68
*14    56.72    39.06
*15    61.80    87.24
*16    63.61    14.32
*17    77.38    92.45
*18    89.18    76.82
*19    140.00   71.61
*;

Table MACC_ORIG(Z,pq)
       P        Q
1   -30.82  37.76  //Smart grid -- Automated residential systems
2   -22.30  92.45  //Smart grid -- AMI with visual display
3    -7.21  19.53  //Geothermal
4    -3.28  33.85  //Landfill Gas Power Generation
5     1e-4  39.06  //Wind (low-cost)
6    22.95 227.86  //Wind (high cost)
7    35.56  41.67  //Solar PV (utility scale)
8    47.05 110.68  //Nuclear
9    56.72  39.06  //Landfill projects (high-cost)
10   61.80  87.24  //Biomass
11   63.61  14.32  //Gas industry projects (low cost)
12   77.38  92.45  //Solar PV (residential)
13   89.18  76.82  //Coal-gas fuel switch for installed fleet
14  140.00  71.61  //Solar thermal
;




parameter macc_orig_nonzero(z,pq);

macc_orig_nonzero(z,pq) = macc_orig(z,pq);
macc_orig_nonzero(z,pq)$(macc_orig(z,"p")<0) = 0;



Loop(z,
MACC_ORIG(Z,"Q") = MACC_ORIG(Z-1,"Q")+MACC_ORIG(Z,"Q");
macc_orig_nonzero(z,"q") = macc_orig_nonzero(z-1,"Q") + macc_orig_nonzero(z,"Q");
);

macc_orig_nonzero(z,pq)$(macc_orig_nonzero(z,"p")=0) = 0;

display macc_orig, macc_orig_nonzero;

Parameter MACC;
if((SwMACC=0),
MACC(Z,"P")=MCK1(Z,"P");
MACC(Z,"Q")=MCK1(Z,"Q");
);

if((SwMACC=1),
MACC(Z,"P")=MCK2(Z,"P");
MACC(Z,"Q")=MCK2(Z,"Q");
);

if((SwMACC=2),
MACC(Z,"P")=BLMBRG1(Z,"P");
MACC(Z,"Q")=BLMBRG1(Z,"Q");
);

if((SwMACC=3),
MACC(Z,"P")=BLMBRG2(Z,"P");
MACC(Z,"Q")=BLMBRG2(Z,"Q");
);

*kr scaling method 1
if((SwMACC=4),
MACC(Z,pq)=MACC_ORIG(Z,pq);
MACC(Z,"p")$(MACC(Z,"p")<>0) = MACC(Z,"p") + 42.79;
MACC(Z,"q") = (emitele("usa") - MACC("1","q") +  MACC(Z,"q")) / 1000;
MACC("1","p") = 1e-4;
MACC("20",pq) = 0;

);

*kr scaling method 2
*sets all negative prices to zero
if((SwMACC=5),
MACC(Z,pq)=MACC_ORIG(Z,pq);
MACC(Z,"q") = (emitele("usa") - MACC("1","q") +  MACC(Z,"q")) / 1000;
MACC(Z,"p")$(MACC(Z,"p")<0) = 1e-4;
MACC("20",pq) = 0;
);

*kr scaling method 3
*excludes negative prices completely
if((SwMACC=6),
MACC(Z,"p") = macc_orig_nonzero(z,"p");
MACC(Z,"q") = MACC_orig_nonzero(Z,"q") / 1000;
MACC("1","p") = 1e-4;
);

