$Title	Read GTAP 7 Basedata and Replicate the Benchmark in MPSGE

*	Trade structure: armtrd, homtrd, or mlztrd
$if not set eistrd $set eistrd mlztrd
$setglobal eistrd %eistrd%

*	Capital Mobility: CET on ssk
$if not set kmob $set kmob 0
$setglobal kmob %kmob%

*	FF supply: multiplier on local fossil-fuel supply elasticities
$if not set ffs $set ffs 1
$setglobal ffs %ffs%

*	Coalition Defined
$if not set coalition $set coalition A1xR

*	Max Number of PE-GE iterations for IRTS decomp method
$if not set pe_ge_itr $set pe_ge_itr 500

*	Convergence plots on PE-GE decomp method
$if not set cnvplt $set cnvplt no

$if not set ds $set ds ham3
$include gtap9data


set	aog(i) All other goods subset /aog/,
	eis(i) Energy intensive goods /crp,nmm,i_s,nfm/,
	m(i)   Melitz heterogeneous-firms goods,
	h(i)   Heckscher-Ohlin Trade goods      /cru/
	coalition(r) Countries with abatement policies;

$if %eistrd%a==armtrda m(i) = no;
$if %eistrd%a==armMtrda m(i) = no; esubd(eis)=5.582;
$if %eistrd%a==armHtrda m(i) = no; esubd(eis)=2*5.582;
*	At esubd(eis) = 41.11 we match the total leakage (gtap7 data)
$if %eistrd%a==armVHtrda m(i) = no; esubd(eis)=100;
$if %eistrd%a==mlztrda m(eis) = yes;
$if %eistrd%a==homtrda m(i) = no; h(eis)=yes;


display m,h;

$if %coalition%==EUR		coalition("EUR") = yes;
$if %coalition%==A1xR		coalition("EUR") = yes; coalition("USA") = yes; coalition("RA1") = yes;
$if %coalition%==A1xR_CHN	coalition("EUR") = yes; coalition("USA") = yes; coalition("RA1") = yes; coalition("CHN") = yes;


parameter sig(i) elasticity of sub for m sectors;
sig(m) = esubd(m);

display sig;

*	Accounts needed for alternative trade formulation:

parameter	
	vafm(i,g,r)	Composite Armington demands,
	rtfa(i,g,r)	Intermediate input tax on Armington demands, 
	rtfa0(i,g,r)	Benchmark intermediate input tax on Armington demands, 
	rta(i,r)	Tariffs on Armington imports,
	rta0(i,r)	Tariffs on Armington imports,
	vam(i,r)	Aggregate Armington supply to all demand categories,
	vxm(i,r)	Exports,
	vtc(j,i,r)	Transport service inputs,
	rtm(i,r)	Import tax rate,
	rtx(i,r)	Export tax rate,
	phit(j,i,r)	Transportation service input coefficient,
	phim(g,r)	Import productivity coefficient,
	phix(i,r)	Export productivity coefficient,
	vhm(i,r)	Value of H-O imports,
	vhx(i,r)	Value of H-O exports,
	vhchk		H-O trade balance check,
	mktchk		H-O market clearance check;


vafm(i,g,r)  = vdfm(i,g,r) + vifm(i,g,r);
rtfa(i,g,r)$vafm(i,g,r) = (rtfd(i,g,r)*vdfm(i,g,r) + rtfi(i,g,r)*vifm(i,g,r)) / vafm(i,g,r);
rtfa0(i,g,r) = rtfa(i,g,r);
vxm(i,r) = sum(s, vxmd(i,r,s));
vam(i,r) = vim(i,r) + vom(i,r)-vst(i,r)-vxm(i,r);
vdm(i,r) = vom(i,r)-vst(i,r)-vxm(i,r);
vtc(j,i,r) = 0;
rta(i,r) = 0;
rta0(i,r) = 0;
rtm(i,r) = 0;
rtx(i,r) = 0;
phit(j,i,r) = 0;
phim(g,r) = 0;
phix(i,r) = 0;

loop(i$h(i),
	rtm(i,r)$sum(s, (vxmd(i,s,r)+sum(j,vtwr(j,i,s,r)))*(1-rtxs(i,s,r)))
		 = sum(s, (vxmd(i,s,r)+sum(j,vtwr(j,i,s,r)))*(1-rtxs(i,s,r))*rtms(i,s,r)) /
		   sum(s, (vxmd(i,s,r)+sum(j,vtwr(j,i,s,r)))*(1-rtxs(i,s,r)));

	rtx(i,r)$sum(s, vxmd(i,r,s)) = -sum(s, vxmd(i,r,s)*rtxs(i,s,r))/sum(s, vxmd(i,r,s));

	phit(j,i,r)$sum(s, vxmd(i,s,r)*(1-rtxs(i,s,r)))
		= sum(s, vtwr(j,i,s,r)) / sum(s, vxmd(i,s,r)*(1-rtxs(i,s,r)));

	phim(i,r) = 1;
	phix(i,r) = 1;

	vhm(i,r) = -min(0, sum(s, vxmd(i,r,s)*(1-rtxs(i,r,s))) - sum(s, vxmd(i,s,r)*(1-rtxs(i,s,r))));
	vhx(i,r) =  max(0, (sum(s, vxmd(i,r,s)*(1-rtxs(i,r,s))) - sum(s, vxmd(i,s,r)*(1-rtxs(i,s,r))))/(1+rtx(i,r)));
	phim(i,r)$vhm(i,r) = (1+sum(j,phit(j,i,r)))*(1+rtm(i,r));
	phix(i,r)$vhx(i,r) = 1+rtx(i,r);

	phix(i,r)$vhm(i,r) = min((1 + rtx(i,r))*0.95,1/phim(i,r));
	phim(i,r)$vhx(i,r) = min((1+rtm(i,r))*(1+sum(j,phit(j,i,r)))*0.95, 
					1/phix(i,r));

*	If there are no gross exports, then the net export activity
*	is omitted from the model.  If there are no gross imports in the
*	benchmark, then net import is omitted.
*	
	phix(i,r)$(sum(s, vxmd(i,r,s))=0) = 0;
	phim(i,r)$(sum(s, vxmd(i,s,r))=0) = 0;

	vtc(j,i,r) = sum(s,vtwr(j,i,s,r)) - phit(j,i,r)*vhm(i,r);
	rta(i,r) = sum(s, rtms(i,s,r)*((1-rtxs(i,s,r))*vxmd(i,s,r)+sum(j,vtwr(j,i,s,r))) -
			  rtxs(i,r,s)*vxmd(i,r,s));
	rta0(i,r)=rta(i,r);
	vim(i,r) = 0;
*.	vdm(i,r) = vam(i,r) - sum(j,vtc(j,i,r)) - rta(i,r);

	vdm(i,r) =  vom(i,r) - vst(i,r) - vhx(i,r) + vhm(i,r)*phim(i,r);
	rta(i,r)$vdm(i,r) =  (vam(i,r) - sum(j,vtc(j,i,r)) - vdm(i,r))/vdm(i,r);
	rta0(i,r)=rta(i,r);

	vhchk(i) = sum(r, vhx(i,r)*phix(i,r) - vhm(i,r));
	display vhchk;

	mktchk(i,r) = vom(i,r) - vst(i,r) - vhx(i,r) + vhm(i,r)*phim(i,r) - vdm(i,r);
	display mktchk;
);

*----~ejb_begin
*	Clean up for the Melitz formulation
parameter
	qrefarm(i,s,r) input coefficient instrument from PE,
	prefarm(i,s,r) ref price on armington inputs
	qrefarm0(i,s,r) benchmark input coefficient instrument from PE,
	prefarm0(i,s,r) benchmark ref price on armington inputs
	expsub(i,s,r)  export subsidy
	imptar0(i,s,r) import tax benchmark
	imptar(i,s,r)  import tax
	trntx(i,s,r)   implied transport tax;

qrefarm(m,s,r) = (vxmd(m,s,r)+sum(g,vdfm(m,g,r))$sameas(r,s));

*	Clean up some of the tax instruments for the irts sectors
expsub(m,s,r) = (rtxs(m,s,r)*vxmd(m,s,r)/(vxmd(m,s,r)+sum(g,vdfm(m,g,r))$sameas(r,s)))$(vxmd(m,s,r)+sum(g,vdfm(m,g,r))$sameas(r,s));
imptar0(m,s,r)= ((rtms(m,s,r)*(1-rtxs(m,s,r)))*vxmd(m,s,r)/(vxmd(m,s,r)+sum(g,vdfm(m,g,r))$sameas(r,s)))$(vxmd(m,s,r)+sum(g,vdfm(m,g,r))$sameas(r,s));
imptar(m,s,r) = ((rtms(m,s,r)*(1-rtxs(m,s,r)))*vxmd(m,s,r)/(vxmd(m,s,r)+sum(g,vdfm(m,g,r))$sameas(r,s)))$(vxmd(m,s,r)+sum(g,vdfm(m,g,r))$sameas(r,s));
trntx(m,s,r)  = (sum(j,vtwr(j,m,s,r)*(pvtwr(m,s,r)))/qrefarm(m,s,r))$qrefarm(m,s,r);
prefarm(m,s,r) = 
	1
	-expsub(m,s,r)
	+imptar(m,s,r)
	+trntx(m,s,r);
qrefarm0(m,s,r)=qrefarm(m,s,r);
prefarm0(m,s,r)=prefarm(m,s,r);
*----~ejb_end

parameter	bmkreport	Summary of benchmark dataset;
bmkreport(i,r,"vom") = vom(i,r);
bmkreport(i,r,"vst") = vst(i,r);
bmkreport(i,r,"vhx") = vhx(i,r);
bmkreport(i,r,"vhm*phim(i,r)") = vhm(i,r)*phim(i,r);
bmkreport(i,r,"vdm") = vdm(i,r);
bmkreport(i,r,"rta") = rta(i,r)*vdm(i,r);
bmkreport(i,r,"vtc") = sum(j, vtc(j,i,r));
bmkreport(i,r,"vtc/vam")$vam(i,r) = sum(j, vtc(j,i,r))/vam(i,r);
bmkreport(i,r,"vam") = vam(i,r);
bmkreport(i,r,"sum(j,vafm(i,j,r))") = sum(j,vafm(i,j,r));
bmkreport(i,r,"vafm(i,c,r)") = vafm(i,"c",r);
bmkreport(i,r,"vafm(i,g,r)") = vafm(i,"g",r);
bmkreport(i,r,"vafm(i,i,r)") = vafm(i,"i",r);



*	Subsets of energy commodities for nesting or specific references
* N.B.: Crude oil is rather treated as material input
set	fe(i)		Fossil fuels	/col, oil, gas/,
	ele(i)		Electricity	/ele/,
	e(i)		Energy inputs	/col,oil,gas,ele/,
	xe(g)		Fossil fuels	/cru, gas, col/;

*	Re-assign labels for capital and labor inputs
parameter
	vlm(g,r)	Labor inputs,
	vkm(g,r)	Capital inputs,
	vrm(g,r)        Resource inputs for fossil fuel production,
	etas(xe,r)	Fossil fuel supply elasticities,
	thetar(g,r)     Value share of resource-specific input
	esubr(g,r)	Cross-price elasticity of substitution in resource production;

vlm(g,r)	= vfm("lab",g,r);
vkm(g,r)	= vfm("cap",g,r);
vrm(xe,r)	= vkm(xe,r);
vkm(xe,r)	= 0;
evom("cap",r)	= sum(g, vkm(g,r));
display rtf0;


*	Fossil fuel calibration to exogenous supply elasticities
*	etas	=	Elasticity of supply
*	esubr	=	Elasticity of substitution
*	thetar	=	Value share of variable input

*	esubr = etas * thetar/(1-thetar)

*	Elasticities of fossil fuel supply:
etas("cru",r)  = 0.5*%ffs%; etas("gas",r)  = 0.5*%ffs%; etas("col",r)  = 1*%ffs%; 
display etas;


parameter	vshare	Benchmark value shares;
vshare(xe,r)    = (vrm(xe,r)*(1+rtf("cap",xe,r))/(vom(xe,r)  *(1-rto(xe,r))))$(vom(xe,r)  *(1-rto(xe,r)));

esubr(xe,r)	= 1;
esubr(xe,r)$vshare(xe,r)	
                = etas(xe,r) * vshare(xe,r)/(1-vshare(xe,r));
display esubr;

parameter
	esub_kle_m(g)   Elasticity of substitution between KLE and M,
	esub_m(g)	Elasticity of substitution within material composite,
	esub_kl_e(g)    Elasticity of substitution between KL and E,
	esub_k_l(g)	Elasticity of substitution between K and L,
	esub_e(g)	Elasticity of susbstituion within energy composite
	cet_ssk		Elasticity of transformation between sectoral capital;	

*	Assign default mapping
esub_e(g)	= 0.5;
esub_k_l(g)	= esubva(g);
esub_kle_m(g)   = 0.5;
esub_m(g)	= 0.3;	
esub_kl_e(g)    = 0.5;
cet_ssk		= %kmob%;

display esubva;

esub_m("C") = 0.5; esub_e("C")= 0.5; esub_kle_m("C") = 0.3;	

*	Impose Leontief-fixed production for investment and public demand
set fixed(g)	/G, I/;

loop(g$fixed(g), esub_e(g) = 0; esub_m(g) = 0; esub_kl_e(g) = 0; esub_kle_m(g) = 0; esub_k_l(g) = 0);	


*	Extensions for CO2 emission control policies
parameter	
	refco2(i,g,r)	Base-year energy- and sector-specific emission flows (billion tons of CO2),
	co2_bmk(r)	Cumulative base-year CO2 emissions (billion tons of CO2), 
	co2lim(r)	Carbon emission limits, 
	cc(i,s,r)	Embodied carbon
	globaltarget	Global emission target (if positive we enact a global emission ceiling);

set	rtax(r)		Countries with a regional carbon market,
	ttax(r)		Countries with international carbon trading,
	tariff(i,s,r)	Flag for optional imposition of bilateral tariffs on embodied carbon,
	tariff_ho(i,r)	Flag for optional imposition of tariff on embodied carbon;


*	Convert CO2 emission units in billion tons of CO2 -- then the CO2 price
*	in the model will be readily available as USD per ton of CO2:

refco2(fe,g,r)     = (eco2d(fe,g,r)+eco2i(fe,g,r))/1e+3;
co2_bmk(r)	   = sum((fe,g), refco2(fe,g,r));
co2lim(r)	   = sum((fe,g), refco2(fe,g,r));
globaltarget	   = 0;
cc(i,s,r)	   = 0;
rtax(r)		   = no;
ttax(r)		   = no;
tariff(i,s,r)	   = no;
tariff_ho(i,r)	   = no;	

$ontext
$model:gtap

$sectors:
	Y(g,r)$vom(g,r)		! Output and Final Demand Activities
	A(i,r)$vam(i,r)		! Composite Supply
	X(i,r)$phix(i,r)	! Homogeneous goods exports
	IM(i,r)$phim(i,r)	! Homogeneous goods imports
	YT(j)$vtw(j)		! Transportation services
	EMIT(r)			! Regional emission flows
	HTRN(i,s,r)$(m(i) and sum(j,vtwr(j,i,s,r))) ! Transport demand Melitz goods
	SSK(r)			! sector specific captial transformation frontier

$commodities:
	PW(i)$h(i)			! World Market Price (HO model)
	P(g,r)$(vom(g,r) or phim(g,r))	! Domestic output price
	PA(i,r)$vam(i,r)		! Armington composite price
	PT(j)$vtw(j)			! Transportation services
        PL(r)				! Wage rate
        RK(g,r)$vkm(g,r)		! Return to sector-specific capital
	RKR(r)				! Return to regional capital
	PR(g,r)$vrm(g,r)		! Resource-specific factor price	
	PCARB(r)			! Price of CO2
	PTCARB$card(ttax)		! Price of tradable CO2
	PRCARB(r)$rtax(r)		! Price of regional CO2
	PDT(i,s,r)$(m(i) and sum(j,vtwr(j,i,s,r))) ! Dummy transport commodity


$consumers:
	RA(r)				! Representative agent
	DTA(i,s,r)$(m(i) and sum(j,vtwr(j,i,s,r))) ! Dummy transport agent

$auxiliary:
	TAU(i,s,r)$tariff(i,s,r)	! Embodied carbon tariffs
	TAU_HO(i,r)$tariff_ho(i,r)	! Embodied carbon tariffs on HO goods
	PHI$globaltarget		! Global carbon accounting
	ressup(xe,r)$(not 1/esubr(xe,r)) ! Constraint to turn off fossil fuel channel

$prod:Y(g,r)$(vom(g,r) and (not xe(g))) s:esub_kle_m(g)   m(s):esub_m(g)   kle(s):esub_kl_e(g)   kl(kle):esub_k_l(g)  
+ e(kle):esub_e(g)  fe.tl(e):0
	o:P(g,r)	q:vom(g,r)	a:RA(r)  t:rto(g,r)
	i:PA(i,r)	q:vafm(i,g,r)	 p:(1+rtfa0(i,g,r))	a:RA(r) t:rtfa(i,g,r)    i.tl:$fe(i)	m:$(not e(i)) e:$(e(i) and (not fe(i)))
	i:PCARB(r)#(fe)	q:refco2(fe,g,r) p:1e-6						 fe.tl:
	i:PL(r)		q:vlm(g,r)	 p:(1+rtf0("lab",g,r))  a:RA(r) t:rtf("lab",g,r) kl: 
	i:RK(g,r)	q:vkm(g,r)	 p:(1+rtf0("cap",g,r))  a:RA(r) t:rtf("cap",g,r) kl:

$prod:Y(g,r)$(vom(g,r) and xe(g))	s:(esubr(g,r)$(1/esubr(g,r)))  id:0
	o:P(g,r)	q:vom(g,r)	a:RA(r)  t:rto(g,r)
	i:PA(i,r)	q:vafm(i,g,r)	 p:(1+rtfa0(i,g,r))     a:RA(r) t:rtfa(i,g,r)	 id:
	i:PCARB(r)#(fe)	q:refco2(fe,g,r) p:1e-6						 id:
	i:PL(r)		q:vlm(g,r)	 p:(1+rtf0("lab",g,r))  a:RA(r) t:rtf("lab",g,r) id:
	i:PR(g,r)	q:vrm(g,r)	 p:(1+rtf0("cap",g,r))	a:RA(r) t:rtf("cap",g,r) 
	

$prod:EMIT(r)
       o:PCARB(r)		q:1
       i:PRCARB(r)$rtax(r)	q:1	
       i:PTCARB$ttax(r)		q:1
       i:PL(r)			q:2e-6


$report:
	V:R_vafm(i,g,r)$vafm(i,g,r)	i:PA(i,r)	prod:Y(g,r)

$prod:YT(j)$vtw(j)  s:1
	o:PT(j)		q:vtw(j)
	i:P(j,r)	q:vst(j,r)

$report:
	V:R_vst(j,r)$vst(j,r)	i:P(j,r)	prod:YT(j)

$prod:X(i,r)$phix(i,r)
	o:PW(i)		q:phix(i,r)
	i:P(i,r)	q:1		a:RA(r)	t:rtx(i,r)

* --------------------------
*	Armington goods (no mm nesting in this formulation)
$prod:A(i,r)$(vam(i,r) and (not m(i) and not h(i)))  s:esubd(i) s.tl:0$vxmd(i,s,r)
	o:PA(i,r)	q:vam(i,r)
	i:P(i,r)	q:vdm(i,r)
	i:P(i,s)	q:vxmd(i,s,r)	p:pvxmd(i,s,r) s.tl: 
+ a:RA(s) t:(-rtxs(i,s,r)) a:RA(r) t:(rtms(i,s,r)*(1-rtxs(i,s,r)))
+ a:RA(r) n:TAU(i,s,r)$tariff(i,s,r) m:cc(i,s,r)$tariff(i,s,r)
	i:PT(j)#(s)	q:vtwr(j,i,s,r) p:pvtwr(i,s,r) s.tl: 
+ a:RA(r) t:rtms(i,s,r)

* --------------------------
* --------------------------
*	Heckscher-Ohlin goods
$prod:A(i,r)$(vam(i,r) and h(i))
	o:PA(i,r)	q:vam(i,r)
	i:PT(j)		q:vtc(j,i,r)
	i:P(i,r)	q:vdm(i,r)  p:(1+rta0(i,r)) a:RA(r) t:rta(i,r)
$prod:IM(i,r)$phim(i,r)
	o:P(i,r)	q:phim(i,r)
	i:PT(j)		q:phit(j,i,r)	a:RA(r) t:rtm(i,r)
	i:PW(i)		q:1		a:RA(r) t:rtm(i,r)
+ a:RA(r) n:TAU_HO(i,r)$tariff_ho(i,r) m:cc(i,r,r)$tariff_ho(i,r)
* --------------------------
* --------------------------
*	Melitz goods
$prod:A(i,r)$(vam(i,r) and m(i))  s:esubd(i)
	o:PA(i,r)	q:vam(i,r)
	i:P(i,s)	q:qrefarm(i,s,r)	p:prefarm(i,s,r)
+		a:RA(s) t:(-expsub(i,s,r)) 
+		a:RA(r) t:(imptar(i,s,r))
+ a:RA(r) n:TAU(i,s,r)$tariff(i,s,r) m:cc(i,s,r)$tariff(i,s,r)
+		a:DTA(i,s,r)$(sum(j,vtwr(j,i,s,r))) t:(trntx(i,s,r))
* --------------------------

$report:
	v:Q_(i,s,r) i:P(i,s) prod:a(i,r)

$prod:htrn(i,s,r)$(m(i) and sum(j,vtwr(j,i,s,r)))
	o:PDT(i,s,r)	q:(sum(j,vtwr(j,i,s,r)+rtms(i,s,r)*vtwr(j,i,s,r)))
	i:PT(j)		q:vtwr(j,i,s,r) p:pvtwr(i,s,r)
+			a:RA(r) t:rtms(i,s,r)

$demand:DTA(i,s,r)$(m(i) and sum(j,vtwr(j,i,s,r)))
	d:PDT(i,s,r)	q:1

$report:
	V:R_vxmd(i,r,s)$((not h(i)) and vxmd(i,r,s))	i:P(i,r)	prod:IM(i,s)

$Prod:SSK(r)	t:cet_ssk
	o:rk(g,r)	q:vkm(g,r)
	i:rkr(r)	q:evom("cap",r)

$demand:RA(r)
	d:P("c",r)	q:vom("c",r)
	e:P("c",rnum)	q:vb(r)
	e:P("g",r)	q:(-vom("g",r))
	e:P("i",r)	q:(-vom("i",r))
	e:RKR(r)	q:evom("cap",r)
	e:PL(r)		q:evom("lab",r)
	e:PR(xe,r)	q:vrm(xe,r)
	e:PR(xe,r)$(not 1/esubr(xe,r))	q:(-1)	r:ressup(xe,r)
	e:PRCARB(r)$rtax(r)	q:co2lim(r)	r:phi$globaltarget
	e:PTCARB$ttax(r)	q:co2lim(r)	r:phi$globaltarget

*	Constraint to turn off fossil fuel channel
$constraint:ressup(xe,r)$(not 1/esubr(xe,r))
	P(xe,r)-sum(rnum,P("c",rnum)) =e= 0;

$constraint:PHI$globaltarget
	globaltarget =e= sum(r, EMIT(r)); 

$constraint:TAU(i,s,r)$tariff(i,s,r)
	P(i,s)*TAU(i,s,r) =e= PCARB(r);

$constraint:TAU_HO(i,r)$tariff_ho(i,r)
	PW(i)*TAU_HO(i,r) =e= PCARB(r);

$offtext
$sysinclude mpsgeset gtap

*	Set the level values for the H-O sectors
X.L(i,r)  = no;
IM.L(i,r) = no;
X.L(h,r)  = vhx(h,r);
IM.L(h,r) = vhm(h,r);
ressup.l(xe,r) = 0;
ressup.fx(xe,r)$(1/esubr(xe,r)) = 0;

display phix, phim;
parameter phixphim;
phixphim(i,r) = max(phix(i,r)*phim(i,r)-1,0);
display phixphim;


PCARB.l(r)  = 0;
PTCARB.l    = 0;
PRCARB.l(r) = 0;
EMIT.l(r)   = co2lim(r);

gtap.workspace=50;
gtap.iterlim = 0;
$include gtap.gen
solve gtap using mcp;
abort$(gtap.objval > 1e-4) "GE calibration fails";

P.fx("C","USA")=1;

*$onecho >path.opt
*convergence_tolerance=1e-5
*$offecho

*~ejb
Set eq(i) active IRTS sector;
alias(mz,m);
eq(m) = yes;

Parameter
	zeta	PE resource supply elasticity
	xi	PE aggregate demand elasticity
	c0	PE reference unit cost
	aa	Pareto shape parameter
	b	lower bound on productivity
	cif(i,r,s)      "gross markup from taxes and transport (endogenous)"
	fch(i,r,s)	fixed operating cost
	dsc(i,r)	"annualized sunk cost: delta*sc",
	alpha	scale parameter on aggregate demand
	pa0(i,r)	benchmark price index
	lambda	idiosyncratic bilateral parameter
	ice(i,r,s) iceberg cost factor
;

*	We can normalize the iceberg cost factor to one
*	in the benchmark, letting lambda soak up the 
*	bilateral deviations.
ice(m,r,s) = 1;

*From BHR (2011) JIE paper:
aa(m) = 4.582;
b(m) = 0.2;
loop(m,
abort$(aa(m) le (sig(m)-1)) "Violation of finite variance condition."
);

zeta = 1;
xi  =  1;
c0(m,r) = 1;

*	Insert the cif price with markup (this should be the same as prefarm),
*	but it needs to be recalculated like this before the scenario 
*	solve to transfer any change in policy instruments over to the PE.
cif(m,s,r) = (1 
		- expsub(m,s,r) 
                + imptar(m,s,r)
		+ (TAU.l(m,s,r)*cc(m,s,r))$tariff(m,s,r)
		+ trntx(m,s,r));

fch(m,r,s)  = 0.1;

*	Equilibrium industrial-organization variables:
POSITIVE VARIABLES
	AD(i,r)		Aggregate demand
	MM(i,r)		Entered firms
	NN(i,r,s)	Number of operating firms
	PFT(i,r,s)	Price of average firm
	RFT(i,r,s)	Revenue of average firm
	VARPHIT(i,r,s)	Productivity of avg firm
	PI(i,r,s)	Shadow value (rents) on capacity constraint,
*	The following are already declared in the mpsge model
	P(g,r)		Unit cost of firm inputs
	PA(i,r)		Composite price index
;

Equations
	Demand	Aggregate Demand (centered each iteration),
	Entry   Zero economic profits,
	ZCP	Zero cutoff profits,
	PAdef	Dixit Stiglitz price index,
	Markup  Optimal firm pricing
	qtdef   Firm level demand
	phitdef  relation between avg productivity and n to m ratio
	capacity operating firms cannot exceed entered firms,
	Supply  Input market;

Demand(m,r)$eq(m)..
	AD(m,r) =e= sum(g,vafm(m,g,r))*A.l(m,r)*(pa0(m,r)/PA(m,r))**xi;

Entry(m,r)$eq(m)..
*krugman should look like
*	P(m,r)*fc(m,r) =e= sum(s,RFT(m,r,s)/(cif(m,r,s)*sig(m)));
*Melitz
	P(m,r)*MM(m,r)*dsc(m,r)
		=E= sum(s,NN(m,r,s)*(RFT(m,r,s)/cif(m,r,s))*(1-1/sig(m))/aa(m));
ZCP(m,r,s)$(eq(m) and q_.l(m,r,s))..	
	P(m,r)*fch(m,r,s) + PI(m,r,s) =E=
	(RFT(m,r,s)/cif(m,r,s))*(aa(m)-(sig(m)-1))/(aa(m)*sig(m));

PAdef(m,r)$eq(m)..
*krugman should look like
*	PA(m,s) =e= (sum(r,lambda(m,r,s)*NN(m,r)*PFT(m,r,s)**(1-sig(m))))**(1/(1-sig(m)));
*Melitz
	PA(m,r) =e= (sum(s, lambda(m,s,r)*NN(m,s,r) * PFT(m,s,r)**(1-sig(m))))**(1/(1-sig(m)));

Markup(m,r,s)$(eq(m) and q_.l(m,r,s))..
*krugman should look like
*	P(m,r) - (1-1/sig(m))*PFT(m,r,s)/cif(m,r,s) =e= 0;
*Melitz (orientation?)
*	PFT(m,r,s)*((1-1/sig(m))*PHIT(m,r,s)) =e= 
*	cif(m,r,s)*P(m,r);
	cif(m,r,s)*ice(m,r,s)*P(m,r)-PFT(m,r,s)*(1-1/sig(m))*VARPHIT(m,r,s) =e= 0;
	;

qtdef(m,r,s)$(eq(m) and q_.l(m,r,s))..
	RFT(m,r,s) =e= lambda(m,r,s)*AD(m,s)*PA(m,s)**sig(m) * PFT(m,r,s)**(1-sig(m));

phitdef(m,r,s)$(eq(m)  and q_.l(m,r,s))..	
	VARPHIT(m,r,s) * (NN(m,r,s)/MM(m,r))**(1/aa(m)) =e= (aa(m)/(aa(m)+1-sig(m)))**(1/(sig(m)-1)) * b(m);

Supply(m,r)$eq(m)..
*krugman should look like
*	(vom(m,r)*Y.l(m,r) - R_vst.l(m,r))*(P(m,r)/c0(m,r))**zeta =e= 
*	NN(m,r)*(fc(m,r) + sum(s,RFT(m,r,s)/PFT(m,r,s)));
*Melitz
*	GE output
 	(vom(m,r)*Y.L(m,r) 
*	less any transport demand				
 	- R_vst.l(m,r) 
 	)* 
 	(P(m,r)/c0(m,r))**zeta =e= 
 	(MM(m,r)*dsc(m,r) + 
 	sum(s$q_.l(m,r,s), NN(m,r,s) * (fch(m,r,s)+(RFT(m,r,s)/PFT(m,r,s))*ice(m,r,s)/VARPHIT(m,r,s))));

capacity(m,r,s)$(eq(m) and q_.l(m,r,s))..	MM(m,r) =g= NN(m,r,s);
	
MODEL PE /Demand.PA,
          Entry.MM,
	  ZCP.NN,
	  PAdef.AD,
	  Markup.RFT,
	  qtdef.PFT,
	  phitdef.varphit,
	  Supply.P,
	  capacity.pi/;

*	Set lowerbounds to prevent bad function calls
MM.lo(m,r)	=1e-8;
NN.lo(m,r,s)	=1e-8;
AD.lo(m,r)	=1e-8;
PFT.lo(m,r,s)	=1e-8;
RFT.lo(m,r,s)	=1e-8;
VARPHIT.lo(m,r,s)	=1e-8;

NN.fx(m,r,s)$(q_.l(m,r,s) eq 0)		= 0;
PFT.fx(m,r,s)$(q_.l(m,r,s) eq 0)	= 0;
RFT.fx(m,r,s)$(q_.l(m,r,s) eq 0)	= 0;
VARPHIT.fx(m,r,s)$(q_.l(m,r,s) eq 0)	= 0;

*PE Calibration
*	Calibration sequence when we know the bilateral fixed costs:
	RFT.l(m,r,s) = 	c0(m,r)*fch(m,r,s)*cif(m,r,s)/((aa(m)-(sig(m)-1))/(aa(m)*sig(m)));
	NN.l(m,r,s)  = cif(m,r,s)*P.l(m,r)*Q_.l(m,r,s)/RFT.l(m,r,s);
	MM.l(m,r)    = 2*smax(s,NN.l(m,r,s));
        dsc(m,r)     = sum(s,NN.l(m,r,s)*(RFT.l(m,r,s)/cif(m,r,s))*(1-1/sig(m))/aa(m))
		       /(c0(m,r)*MM.l(m,r));
	PI.l(m,r,s)=0;

	VARPHIT.l(m,r,s)$NN.l(m,r,s) = b(m)*(aa(m)/(aa(m)+1-sig(m)))**(1/(sig(m)-1)) * (MM.l(m,r)/NN.l(m,r,s))**(1/aa(m));
	PFT.l(m,r,s)$NN.l(m,r,s) = c0(m,r)*cif(m,r,s)/((1-1/sig(m))*VARPHIT.l(m,r,s));
	pa0(m,r)=PA.l(m,r);
	AD.l(m,r)= sum(g,vafm(m,g,r))*a.l(m,r);
	lambda(m,s,r)=RFT.l(m,s,r)/AD.l(m,r)   * PA.l(m,r)**(-sig(m)) *PFT.l(m,s,r)**(-1+sig(m));

PE.iterlim = 0;

*	Check the PE calibration if the set is not empty
if (sum(m,1),
Solve PE using MCP;
abort$(PE.objval > 1e-4) "PE calibration is not consistent";
    );

*	IRTS solve machinery

set		itr	Solution iterations /itr0*itr%pe_ge_itr%/;
parameter	itrlog	Iteration log;
alias(itr,it);
file status; put status; 

parameter	
	abs_dev	input and output deviation at handoff to PE
	rel_dev deviation as a percent of benchmark levels
	itrflag flag to halt iterations once sum of squared deviations is below tol
	flaglbl
	tol	error tolerance					/1e-7/,
	pltdat  plot data,
	prftchk(itr,i,r) zero profit on new calibrated armington activity
	imktchk(itr,i,r) input market check
	omktchk(itr,i,r) output market check;
prftchk(itr,i,r)=no;imktchk(itr,i,r)=no;omktchk(itr,i,r)=no;

$onecho >DCOMPsolve.gms
eq(m) = no;
PE.iterlim = 10000;
itrflag(m) = 10000;
loop(itr$(sum(m,1) and (smax(m,itrflag(m))) > tol),
	flaglbl = (smax(m,itrflag(m))-tol)/tol;
*	Recenter supply and demand on the last GE solution point

	c0(m,r) = P.L(m,r);
	pa0(m,r)= PA.l(m,r);

*	Recalculate the cif markup based on new distortions
	cif(m,s,r) = (1 
		- expsub(m,s,r) 
                + imptar(m,s,r)
		+ (TAU.l(m,s,r)*cc(m,s,r))$tariff(m,s,r)
		+ trntx(m,s,r));

*	Solve one PE for each commodity
	loop(mz,
	eq(m) = no;
	eq(mz)= yes;
	put status; put_utility 'title' /'Commodity:',mz.tl,itr.tl,flaglbl/;

	solve PE using mcp;
	itrlog(itr,"PE") = PE.iterusd;
	abort$(PE.solvestat<>1) "Model does not solve.";
	);

*	Recalibrate the Armington demand function to the current point:

	prefarm(m,s,r)
		= P.l(m,s)*cif(m,s,r);

	qrefarm(m,s,r) = 
		  (RFT.l(m,s,r)*NN.l(m,s,r)/prefarm(m,s,r))/
		  (AD.l(m,r)/(sum(g,vafm(m,g,r))))
		  ;

*	Display "Cross check on zero profit in the ARM blocks:";
	gtap.iterlim = 0;
$include gtap.gen
	solve gtap using mcp;
	prftchk(itr,m,r) = A.M(m,r);
	imktchk(itr,m,r) = P.M(m,r);
	omktchk(itr,m,r) = PA.M(m,r);
	itrflag(m) = smax(r,SQR(imktchk(itr,m,r))) + smax(r,SQR(omktchk(itr,m,r)));
	pltdat(itr,m) = itrflag(m);

*	Solve the GE 
	gtap.iterlim = 100000;
$include gtap.gen
	solve gtap using mcp;
	itrlog(itr,"cge") = gtap.iterusd;
);

display itrlog;
display prftchk,imktchk,omktchk;

$if %cnvplt%a==yesa $batinclude plot pltdat
pltdat(itr,m)$(ord(itr) le (card(itr)-20)) = no;
$if %cnvplt%a==yesa $batinclude plot pltdat

abort$((smax(m,itrflag(m))) ge tol) "Convergence failure at this tolerance level";

$offecho


*	Set up the basic reports

Parameter
	pnum	numeraire for calculating nominal reports,
	rpt7	reports with seven dimensions including scn and structure tags;

$onechov >rpt_bat.gms
pnum  = sum(s, p.l("c",s) * y.l("c",s)* vom("c",s)) / sum(s, y.l("c",s)*vom("c",s));

rpt7("%eistrd%","%1","%kmob%","%ffs%","EV","xxx",r) = y.l("c",r)*vom("c",r);

*	Nominal consumption (BUSD) evaluated using wtd avg pnum
rpt7("%eistrd%","%1","%kmob%","%ffs%","Cons_NOM","xxx",r) = p.l("c",r) * y.l("c",r) * vom("c",r)/ pnum;
rpt7("%eistrd%","%1","%kmob%","%ffs%","Cons_NOM","xxx","ALL") = sum(r,p.l("c",r) * y.l("c",r) * vom("c",r))/ pnum;
rpt7("%eistrd%","%1","%kmob%","%ffs%","Cons_NOM","xxx","COA") = sum(r$coalition(r),p.l("c",r) * y.l("c",r) * vom("c",r))/ pnum;
rpt7("%eistrd%","%1","%kmob%","%ffs%","Cons_NOM","xxx","NCOA") = sum(r$(not coalition(r)),p.l("c",r) * y.l("c",r) * vom("c",r))/ pnum;

*	Emissions (MtCO2):
rpt7("%eistrd%","%1","%kmob%","%ffs%","CO2emit","xxx",r) = emit.l(r)*1000;
rpt7("%eistrd%","%1","%kmob%","%ffs%","CO2emit","xxx","ALL") = sum(r,emit.l(r))*1000;
rpt7("%eistrd%","%1","%kmob%","%ffs%","CO2emit","xxx","COA") = sum(r$coalition(r),emit.l(r))*1000;
rpt7("%eistrd%","%1","%kmob%","%ffs%","CO2emit","xxx","NCOA") = sum(r$(not coalition(r)),emit.l(r))*1000;

*	CO2 price (USD/ton) using pnum as numeraire
rpt7("%eistrd%","%1","%kmob%","%ffs%","CO2price","xxx",r)$coalition(r)  = (PCARB.l(r)$rtax(r) + PTCARB.l$ttax(r))/pnum;
rpt7("%eistrd%","%1","%kmob%","%ffs%","CO2price","xxx","COA")$ptcarb.l = (ptcarb.l)/pnum; 

**	Leakage (%):
rpt7("%eistrd%","%1","%kmob%","%ffs%","Leakage","xxx",s)$(not coalition(s) and 
                                     sum(r$(    coalition(r)), rpt7("%eistrd%","bau","%kmob%","%ffs%","CO2emit","xxx",r) - rpt7("%eistrd%","%1","%kmob%","%ffs%","CO2emit","xxx",r)))  
				=100*                         (rpt7("%eistrd%","%1","%kmob%","%ffs%","CO2emit","xxx",s) - rpt7("%eistrd%","bau","%kmob%","%ffs%","CO2emit","xxx",s))/
	                             sum(r$(    coalition(r)), rpt7("%eistrd%","bau","%kmob%","%ffs%","CO2emit","xxx",r) - rpt7("%eistrd%","%1","%kmob%","%ffs%","CO2emit","xxx",r));
rpt7("%eistrd%","%1","%kmob%","%ffs%","Leakage","xxx","ALL")$    sum(r$(    coalition(r)), rpt7("%eistrd%","bau","%kmob%","%ffs%","CO2emit","xxx",r) - rpt7("%eistrd%","%1","%kmob%","%ffs%","CO2emit","xxx",r))  
				=100*sum(r$(not coalition(r)), rpt7("%eistrd%","%1","%kmob%","%ffs%","CO2emit","xxx",r) - rpt7("%eistrd%","bau","%kmob%","%ffs%","CO2emit","xxx",r))/
	                             sum(r$(    coalition(r)), rpt7("%eistrd%","bau","%kmob%","%ffs%","CO2emit","xxx",r) - rpt7("%eistrd%","%1","%kmob%","%ffs%","CO2emit","xxx",r));

*	Sector performance:
rpt7("%eistrd%","%1","%kmob%","%ffs%","Output_Q",i,r)	  =          y.l(i,r)*vom(i,r)             ;
rpt7("%eistrd%","%1","%kmob%","%ffs%","Output_V",i,r)	  = p.l(i,r)*y.l(i,r)*vom(i,r) / pnum;
*	Armington and Metitz value of trade
rpt7("%eistrd%","%1","%kmob%","%ffs%","Export_V",i,r)  = sum(s$(not sameas(r,s)), p.l(i,r)*Q_.l(i,r,s))/ pnum;
rpt7("%eistrd%","%1","%kmob%","%ffs%","Import_V",i,r)  = sum(s$(not sameas(r,s)), p.l(i,s)*Q_.l(i,s,r))/ pnum;
*	Homogeneous goods trade
rpt7("%eistrd%","%1","%kmob%","%ffs%","Export_V",h,r)	  = pw.l(h)*phix(h,r)*X.l(h,r)/ pnum;
rpt7("%eistrd%","%1","%kmob%","%ffs%","Import_V",h,r)	  = pw.l(h)*IM.l(h,r)/ pnum;


$offecho

$batinclude rpt_bat bau
