All results can be created via runmacc.bat

MasterPC.gms wraps the code together. It calls, in order and with description:
1. init.inc: set up the GTAP data set
2. macc3.inc: sets up the MAC target data table based on SwMACC
3. gtapingams.inc: sets up the default (indicated by 'de') gtap-in-gams structure (Standard) structure. Also replicates the results from the default nesting structure
4. mckwil.inc: sets up the mckibben and wilcoxen (indicated by 'mw') nesting structure (basic)
5. worldscan.inc: sets up the worldscan (indicated by 'ws') nesting structure (extended)
6. cases.inc: adjusts which elasticities are variable and solves each model in turn
7. caseseq.inc: secondary set of cases that solves the standard model's set of variable elasticity permutations