$STitle a simple scenario to test the decomposition solution technique

ttax(coalition) = yes;
globaltarget = sum(r,emit.l(r)) - 0.2*sum(coalition,emit.l(coalition));


*	Solve the base GE model
gtap.iterlim = 100000;
$include gtap.gen
solve gtap using mcp;
*	PE -- GE solve for Melitz goods
$include DCOMPsolve

$batinclude rpt_bat scn

display rpt7, esubd;

execute_unload 'gdxrpt\%eistrd%_CET%kmob%_ffs%ffs%.gdx', rpt7;