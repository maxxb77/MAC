$Title	Generate Reports from the Merged GDX


set	d	set on merge set to be dropped,
	eistrd	trade structure /armtrd,armMtrd,armHtrd,armVHtrd,homtrd,mlztrd/,
        scn	scenario  /bau,scn/,
	kmob	cet on capital mobility /0,1,2,3,5/,
	ffs	fossil fuel supply multiplier /1,2,inf/
	item	report item /EV,Cons_NOM,CO2emit,
                             CO2price,Leakage,Output_Q,Output_V,
			     Export_V,Import_V/
	r	region /USA,EUR,RA1,CHN,IND,RUS,EEX,ROW
			ALL,COA,NCOA/
	noncol(r) /CHN,IND,RUS,EEX,ROW/,
	col(r)	  /USA,EUR,RA1/,
	i	goods /AOG,CRP,NMM,I_S,NFM,CRU,COL,GAS,OIL,ELE,ATP,WTP,OTP,xxx/
	eis(i)  energy intensive goods /CRP,NMM,I_S,NFM/;

parameter rpt7	raw results parameter

$gdxin mines.gdx
$load	d=Merged_set_1
$load   rpt7

Parameter rpt(eistrd,scn,kmob,ffs,item,i,r) drop the merged set index;

rpt(eistrd,scn,kmob,ffs,item,i,r)=sum(d,rpt7(d,eistrd,scn,kmob,ffs,item,i,r));

Parameter 
	tblwlf1(r,*) Welfare percent change summary table by region kmob3 ffs1
	tblwlf2(*,*) Welfare moneymetric change summary table by region kmob3 ffs1
	tblout1(*,*) EIS output change
	tblexp1(*,*,*) EIS exports
	tblleak1(*,*) Leakage report 1
	tblpcarb(r,*) Carbon price by region and scenario
;

tblwlf1(r,"BAU ($B)") = rpt("armtrd","bau","3","1","EV","xxx",r);
tblwlf1(r,eistrd)$rpt(eistrd,"bau","3","1","EV","xxx",r)
	 = rpt(eistrd,"scn","3","1","EV","xxx",r)/rpt(eistrd,"bau","3","1","EV","xxx",r) - 1;

tblwlf2(r,"BAU ($B)") = rpt("armtrd","bau","3","1","EV","xxx",r);
tblwlf2(r,eistrd)$rpt(eistrd,"bau","3","1","EV","xxx",r)
	 = rpt(eistrd,"scn","3","1","EV","xxx",r)-rpt(eistrd,"bau","3","1","EV","xxx",r);
tblwlf2("Non-coalition",eistrd)=sum(noncol,tblwlf2(noncol,eistrd));

tblout1(r,"BAU ($B)") = sum(eis,rpt("armtrd","bau","3","1","Output_V",eis,r));
tblout1(r,eistrd) = 
	sum(eis,rpt(eistrd,"scn","3","1","Output_V",eis,r))-
	sum(eis,rpt(eistrd,"bau","3","1","Output_V",eis,r));
tblout1("Non-coalition",eistrd)=sum(noncol,tblout1(noncol,eistrd));
tblout1("Coalition",eistrd)=sum(r$(not noncol(r)),tblout1(r,eistrd));
tblout1("Global",eistrd)   =sum(r                ,tblout1(r,eistrd));
tblout1("Non-coalition","BAU ($B)")=sum(noncol,tblout1(noncol,"BAU ($B)"));
tblout1("Coalition","BAU ($B)")=sum(r$(not noncol(r)),tblout1(r,"BAU ($B)"));
tblout1("Global","BAU ($B)")   =sum(r                ,tblout1(r,"BAU ($B)"));

tblexp1(eis,r,"BAU ($B)") = rpt("armtrd","bau","3","1","Export_V",eis,r);
tblexp1(eis,r,"BAU_HO ($B)") = rpt("homtrd","bau","3","1","Export_V",eis,r);
tblexp1(eis,r,eistrd) = 
	rpt(eistrd,"scn","3","1","Export_V",eis,r)-
	rpt(eistrd,"bau","3","1","Export_V",eis,r);
tblexp1(eis,"Non-coalition",eistrd)=sum(noncol,tblexp1(eis,noncol,eistrd));
tblexp1(eis,"Coalition",eistrd)=sum(r$(not noncol(r)),tblexp1(eis,r,eistrd));
tblexp1(eis,"Global",eistrd)   =sum(r                ,tblexp1(eis,r,eistrd));
tblexp1(eis,"Non-coalition","BAU ($B)")=sum(noncol,tblexp1(eis,noncol,"BAU ($B)"));
tblexp1(eis,"Coalition","BAU ($B)")=sum(r$(not noncol(r)),tblexp1(eis,r,"BAU ($B)"));
tblexp1(eis,"Global","BAU ($B)")   =sum(r                ,tblexp1(eis,r,"BAU ($B)"));
tblexp1(eis,"Non-coalition","BAU_HO ($B)")=sum(noncol,tblexp1(eis,noncol,"BAU_HO ($B)"));
tblexp1(eis,"Coalition","BAU_HO ($B)")=sum(r$(not noncol(r)),tblexp1(eis,r,"BAU_HO ($B)"));
tblexp1(eis,"Global","BAU_HO ($B)")   =sum(r                ,tblexp1(eis,r,"BAU_HO ($B)"));

tblexp1("eis",r,"BAU ($B)") = sum(eis,rpt("armtrd","bau","3","1","Export_V",eis,r));
tblexp1("eis",r,"BAU_HO ($B)") = sum(eis,rpt("homtrd","bau","3","1","Export_V",eis,r));
tblexp1("eis",r,eistrd) = 
	sum(eis,rpt(eistrd,"scn","3","1","Export_V",eis,r))-
	sum(eis,rpt(eistrd,"bau","3","1","Export_V",eis,r));
tblexp1("eis","Non-coalition",eistrd)=sum(noncol,tblexp1("eis",noncol,eistrd));
tblexp1("eis","Coalition",eistrd)=sum(r$(not noncol(r)),tblexp1("eis",r,eistrd));
tblexp1("eis","Global",eistrd)   =sum(r                ,tblexp1("eis",r,eistrd));
tblexp1("eis","Non-coalition","BAU ($B)")=sum(noncol,tblexp1("eis",noncol,"BAU ($B)"));
tblexp1("eis","Coalition","BAU ($B)")=sum(r$(not noncol(r)),tblexp1("eis",r,"BAU ($B)"));
tblexp1("eis","Global","BAU ($B)")   =sum(r                ,tblexp1("eis",r,"BAU ($B)"));
tblexp1("eis","Non-coalition","BAU_HO ($B)")=sum(noncol,tblexp1("eis",noncol,"BAU_HO ($B)"));
tblexp1("eis","Coalition","BAU_HO ($B)")=sum(r$(not noncol(r)),tblexp1("eis",r,"BAU_HO ($B)"));
tblexp1("eis","Global","BAU_HO ($B)")   =sum(r                ,tblexp1("eis",r,"BAU_HO ($B)"));

tblleak1(r,eistrd)=rpt(eistrd,"scn","3","1","leakage","xxx",r);
display tblleak1;

tblpcarb(r,eistrd) = rpt(eistrd,"scn","3","1","CO2price","xxx",r);

display tblwlf1,tblwlf2,tblout1,tblexp1,tblleak1,tblpcarb; 

$call 'copy "rpttemp.xlsx" rpt.xlsx';
execute_unload 'rptgen.gdx', tblwlf1, tblwlf2, tblout1, tblexp1,
	tblleak1,tblpcarb;

$onecho >rptgen.rsp
par=tblwlf1 rng="wlf1!b2" merge
par=tblwlf2 rng="wlf2!b2" merge
par=tblout1 rng="out1!b2" merge
par=tblexp1 rng="exp1!b2" merge
par=tblleak1 rng="leak1!b2" merge
par=tblpcarb rng="pcarb!b2"
$offecho

execute 'gdxxrw i=rptgen.gdx o=rpt.xlsx @rptgen.rsp trace=3 log=report.log';

