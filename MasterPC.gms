$TITLE CES CALIBRATION TO MAC CURVES
*GTAP9 Data 
*Maxwell Brown
*Contact: Maxwell.L.Brown@gmail.com
*last updated: 20170418 (Cleaning and annotation)


$ontext
MasterPC.gms wraps the code together. It calls, in order and with description:
1. init.inc: set up the GTAP data set
2. macc3.inc: sets up the MAC target data table based on SwMACC
3. gtapingams.inc: sets up the default (indicated by 'de') gtap-in-gams structure (Standard) structure. Also replicates the results from the default nesting structure
4. mckwil.inc: sets up the mckibben and wilcoxen (indicated by 'mw') nesting structure (basic)
5. worldscan.inc: sets up the worldscan (indicated by 'ws') nesting structure (extended)
6. cases.inc: adjusts which elasticities are variable and solves each model in turn
7. caseseq.inc: secondary set of cases that solves the standard model's set of variable elasticity permutations
$offtext

option decimals = 5;
$eolcom //

Scalar swlobnd           "Switch to have lower bounds on all variables (1) or not (0)"   /1/; //default 1, helps solver but not binding
Scalar fixrow            "Switch to not fix (0) or fix (1) all ROW variables"            /0/; //doesn't matter -- used in testing
Scalar SwYFX             "Switch to fix Y (i.e. assume inelastic demand)"                /1/; //default 1
Scalar SwMATCHCase       "Switch to have the WS match cases for DE and MW structures"    /0/; //default 0 -- See MB b4 running
Scalar SwPRecurse        "Switch to have the price of PA.L(ELE,USA) equal to P(ELE,USA)" /1/; //default 1
Scalar SwMACC /4/; //default 4-6, see note below
Scalar SwExtend          "Switch to extend [1] or keep simple [0] objective function"    /0/; //default 0
*objective function is extended by the difference of each fossil fuel's emissions
*relative to that fossil fuel's emissions in the base case

$ontext
         0: McKinsey first-cut
         1: McKinsey power cluster
         2: Bloomberg
                 Specifically: 2030 US MAC curve (accounting for improving carbon
                 intensity, key recent polciies, and sector specific discount rates)
         3: Bloomberg
                 Specifically: 2020 MAC curve (scenario 4 -- accounting for improving
                 carbon intensity, key recent poliies, and sector-specific discount rate
                 adjustments
         4: Scaling Method A
         5: Scaling Method B
         6: Scaling Method C
$offtext

*set switches based on global definitions
SwMACC = %swmaccext%;
SwYFX = %swelasext%;

*Define upper and lower limits for elasticities
Scalar lolim "Lower limit for elasticity values" /0/; //default 0
Scalar uplim "Upper limit for elasticity values" /500/; //default 500. stable to 1e5+


*bounds on the variables (not elasticities)
scalar loval "lower bound on endogenous prices and quantities" /1e-3/; //default 1e-3, stable except DE to 1e-7
scalar hival "upper bound on endogenous prices and quantities"  /1e4/; //doesn't matter beyond 1e4

scalar mu "elasticity of electricity demand WRT price" /-0.5/; //default -0.5


*Include the file which holds all the information in the GTAP dataset
*this also solves a model based on the JE structure
*The JE structure was chosen since it contains all the information for
*all the models; that is, it replicates the GTAP dataset but also
*has the parameters necessary to compute the CO nest
*Since it replicates the GTAP dataset, the other level values used
*in the other models' calculations remain the same
$include init.inc


set z "index of MACC curve points, could contain many more than data contains" /1*30/;
set zfeas(z) "feasible carbon price scenarios";
set pq "set of values on the MACC, price and quantity" /p,q/;


PARAMETER EMITELE "REFERENCE LEVEL OF CARBON EMISSIONS FROM ELECTRICITY SECTOR";
EMITELE(R) = SUM((FE,EEE),REFCO2(FE,EEE,R)*Y.L(EEE,R));


*Retrive the information from the MACC curve based on the selected cases data in the MACC parameter.
*|current file: macc.inc|
$include macc.inc


*the first feasible set is set as sometimes MACC("1","Q") equals zero
zfeas("1") = yes;
ZFEAS(Z)$((MACC(Z,"Q")>0)$(MACC(Z,"P")>0)) = yes;
*ZFEAS(Z)$((MACC(Z,"Q")>0)) = yes;


*MACC Q must be taken as a reduction from the baseline
MACC(Z,"Q")$zfeas(z) = emitele("usa")-macc(z,"q");
display zfeas, MACC;

*Set up the model (SSEMPEC_DE) from the default nesting structure
*this also tests to see if the algebraic representation matches the MPSGE model (1st solve)
$include gtapingams.inc

*the de model includes the gtap representation from which we take the
*reference amount of carbone missions. The MACC 'Q' , as used in the model,
*is the amount of aggregate emissions

*Set up the model (SSEMPEC_JE) from the Jorgensen et al. nesting structure
$include mckwil.inc

*Set up the model (SSEMPEC_MW) from the McKibbin and Wilcoxen nesting structure
$include worldscan.inc

*Fix input prices to output values:
PA.FX(I,R)   = PA.L(I,R);
RK.FX(EEE,R) = RK.L(EEE,R);
PL.FX(R)     = PL.L(R);

*include the file which changes settings depending on switches defined above (excluding SwMACC)
$include switchcalc.inc

parameter res_pq_DE,res_pq_MW, res_pq_WS, res_elas_DE, res_elas_MW, res_elas_WS, res_obj;

display macc;
parameter res_elas_all, res_obj_mod;
parameter res_pq, res_elas, res_obj;
parameter res_elas_fe, res_pq_fe;

ESUB_KLE_M_de.fx(eee,r) = 0.1;
esub_m_de.fx(eee,r) = 0.391;
esub_k_l_de.fx(eee,r) = 0.460;
esub_kl_e_de.fx(eee,r) = 0.256;
esub_e_de.fx(eee,r) = 0.500;


table macc_de_op(z,*)
         PB        QB
1        1E-04     2.549334
2        10        2.269493
3        20        2.107244
4        30        1.994301
5        40        1.907772
6        50        1.83754
7        60        1.778333
8        70        1.727083
9        80        1.681856
10       90        1.641358
11      100        1.604677
12      110        1.57115
13      120        1.540276
14      130        1.511666
15      140        1.485017
16      150        1.460081
;

If((SwMATCHCASE = 1),
macc(z,"p") = macc_de_op(z,"pb");
macc(z,"q") = macc_de_op(z,"qb");
zfeas(z) = no;
zfeas(z)$(macc(z,"q")>0) = yes;
);

Table MAC_CPP(z,*)

   TAX   ABA
1  1e-4  1e-2
2  10    0.22
3  20    0.54
4  30    0.83
5  40    1.01
6  50    1.12
7  60    1.17
8  70    1.22
9  80    1.23
10 90    1.25
11 100   1.26
;

Table MAC_cpp2(z,*)
          TAX       ABA
1         1e-4      1e-2
2         10        0.268
3         20        0.540
4         30        0.798
5         40        0.897
6         50        0.994
7         60        1.042
8         70        1.089
9         80        1.102
10        90        1.119
11        100       1.132
;
MAC_CPP(z,"ABA") = MAC_cpp2(z,"aba");

MAC_CPP(z,"ABA") = emitele("usa") - MAC_CPP(z,"ABA");

If((SwMATCHCASE = 2),
macc(z,"p") = MAC_CPP(z,"TAX");
macc(z,"q") = MAC_CPP(z,"ABA");
zfeas(z) = no;
zfeas(z)$((macc(z,"q")>0)$(macc(z,"p")>0)) = yes;
);

display macc,zfeas, refco2;


$include cases.inc

DISPLAY         macc, res_pq_DE,
                res_pq_MW,
                res_pq_WS,
                res_elas_DE,
                res_elas_MW,
                res_elas_WS,
                res_elas,
                res_obj;

If(SwMACC=4,
execute_unload "res_pq_DE_4.gdx" res_pq_DE;

execute_unload "res_pq_MW_4.gdx" res_pq_MW;

execute_unload "res_pq_WS_4.gdx" res_pq_WS;

execute_unload "res_ELAS_DE_4.gdx" res_ELAS_DE;

execute_unload "res_ELAS_MW_4.gdx" res_ELAS_MW;

execute_unload "res_ELAS_WS_4.gdx" res_ELAS_WS;

execute_unload "res_OBJ_4.gdx" res_OBJ;

execute_unload "res_OBJ_MOD_4.gdx" res_OBJ_MOD;

execute_unload "res_ELAS_ALL_4.gdx" RES_ELAS_ALL;

Execute 'GDXXRW.EXE res_ELAS_ALL_4.gdx O=MACC_Export.xls par=res_elas_all rng=res4!A1:K30 rdim=2 cdim=1';

Execute 'GDXXRW.EXE res_OBJ_MOD_4.gdx O=MACC_Export.xls par=res_obj_mod rng=obj4!A1:K30 rdim=1 cdim=1';

);


If(SwMACC=5,

execute_unload "res_pq_DE_5.gdx" res_pq_DE;

execute_unload "res_pq_MW_5.gdx" res_pq_MW;

execute_unload "res_pq_WS_5.gdx" res_pq_WS;

execute_unload "res_ELAS_DE_5.gdx" res_ELAS_DE;

execute_unload "res_ELAS_MW_5.gdx" res_ELAS_MW;

execute_unload "res_ELAS_WS_5.gdx" res_ELAS_WS;

execute_unload "res_OBJ_5.gdx" res_OBJ;

execute_unload "res_OBJ_MOD_5.gdx" res_OBJ_MOD;

execute_unload "res_ELAS_ALL_5.gdx" RES_ELAS_ALL;

Execute 'GDXXRW.EXE res_ELAS_ALL_5.gdx O=MACC_Export.xls par=res_elas_all rng=res5!A1:K30 rdim=2 cdim=1';

Execute 'GDXXRW.EXE res_OBJ_MOD_5.gdx O=MACC_Export.xls par=res_obj_mod rng=obj5!A1:K30 rdim=1 cdim=1';

);


If(SwMACC=6,

execute_unload "res_pq_DE_6.gdx" res_pq_DE;

execute_unload "res_pq_MW_6.gdx" res_pq_MW;

execute_unload "res_pq_WS_6.gdx" res_pq_WS;

execute_unload "res_ELAS_DE_6.gdx" res_ELAS_DE;

execute_unload "res_ELAS_MW_6.gdx" res_ELAS_MW;

execute_unload "res_ELAS_WS_6.gdx" res_ELAS_WS;

execute_unload "res_OBJ_6.gdx" res_OBJ;

execute_unload "res_OBJ_MOD_6.gdx" res_OBJ_MOD;

execute_unload "res_ELAS_ALL_6.gdx" RES_ELAS_ALL;

Execute 'GDXXRW.EXE res_ELAS_ALL_6.gdx O=MACC_Export.xls par=res_elas_all rng=res6!A1:K30 rdim=2 cdim=1';

Execute 'GDXXRW.EXE res_OBJ_MOD_6.gdx O=MACC_Export.xls par=res_obj_mod rng=obj6!A1:K30 rdim=1 cdim=1';

);


$include caseseq.inc

If(SwMACC=4,

execute_unload "res_elas_seq_4.gdx" res_elas_de_seq;
execute_unload "res_obj_seq_4.gdx" res_obj_seq;

Execute 'GDXXRW.EXE res_elas_seq_4.gdx O=MACC_Export.xls par=res_elas_de_seq rng=resseq4!A1:K30 rdim=1 cdim=1';
Execute 'GDXXRW.EXE res_obj_seq_4.gdx O=MACC_Export.xls par=res_obj_seq rng=resseq4!A31:K60 rdim=1 cdim=1';

);


If(SwMACC=5,

execute_unload "res_elas_seq_5.gdx" res_elas_de_seq;
execute_unload "res_obj_seq_5.gdx" res_obj_seq;

Execute 'GDXXRW.EXE res_elas_seq_5.gdx O=MACC_Export.xls par=res_elas_de_seq rng=resseq5!A1:K30 rdim=1 cdim=1';
Execute 'GDXXRW.EXE res_obj_seq_5.gdx O=MACC_Export.xls par=res_obj_seq rng=resseq5!A31:K60 rdim=1 cdim=1';

);


If(SwMACC=6,
execute_unload "res_elas_seq_6.gdx" res_elas_de_seq;
execute_unload "res_obj_seq_6.gdx" res_obj_seq;

Execute 'GDXXRW.EXE res_elas_seq_6.gdx O=MACC_Export.xls par=res_elas_de_seq rng=resseq6!A1:K30 rdim=1 cdim=1';
Execute 'GDXXRW.EXE res_obj_seq_6.gdx O=MACC_Export.xls par=res_obj_seq rng=resseq6!A31:K60 rdim=1 cdim=1';

);







