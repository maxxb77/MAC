$Title  Read GTAP 9 Basedata and Replicate the Benchmark in MPSGE


$if not set ds $set ds trade
$include gtap9data

scalar  hot     Heckscher-Ohlin Trade Model /0/,
        arm     Armington trade model /0/;

parameter pcarb0(r) "initial price of carbon";


set row(r) "all countries that are not the USA" /CHN, IND, RUS, EUR, RA1, EEX, ROW/;
pcarb0(r) = 1e-6;

$if not set mdl $set mdl arm

$if %mdl%==hot  hot=1;
$if %mdl%==arm  arm=1;

*       Accounts needed for alternative trade formulation:

parameter
        vafm(i,g,r)     Composite Armington demands,
        rtfa(i,g,r)     Interm-ediate input tax on Armington demands,
        rtfa0(i,g,r)    Benchmark intermediate input tax on Armington demands,
        rta(i,r)        Tariffs on Armington imports,
        vam(i,r)        Aggregate Armington supply to all demand categories,
        vxm(i,r)        Exports,
        vtc(j,i,r)      Transport service inputs,
        rtm(i,r)        Import tax rate,
        rtx(i,r)        Export tax rate,
        phit(j,i,r)     Transportation service input coefficient,
        phim(g,r)       Import productivity coefficient,
        phix(i,r)       Export productivity coefficient,
        vhm(i,r)        Value of H-O imports,
        vhx(i,r)        Value of H-O exports,
        vhchk           H-O trade balance check,
        mktchk          H-O market clearance check;


vafm(i,g,r)  = vdfm(i,g,r) + vifm(i,g,r);
rtfa(i,g,r)$vafm(i,g,r) = (rtfd(i,g,r)*vdfm(i,g,r) + rtfi(i,g,r)*vifm(i,g,r)) / vafm(i,g,r);
rtfa0(i,g,r) = rtfa(i,g,r);
vxm(i,r) = sum(s, vxmd(i,r,s));
vam(i,r) = vim(i,r) + vom(i,r)-vst(i,r)-vxm(i,r);
vdm(i,r) = vom(i,r)-vst(i,r)-vxm(i,r);
vtc(j,i,r) = 0;
rta(i,r) = 0;
rtm(i,r) = 0;
rtx(i,r) = 0;
phit(j,i,r) = 0;
phim(g,r) = 0;
phix(i,r) = 0;

if (hot,

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

*       If there are no gross exports, then the net export activity
*       is omitted from the model.  If there are no gross imports in the
*       benchmark, then net import is omitted.
*
        phix(i,r)$(sum(s, vxmd(i,r,s))=0) = 0;
        phim(i,r)$(sum(s, vxmd(i,s,r))=0) = 0;

        vtc(j,i,r) = sum(s,vtwr(j,i,s,r)) - phit(j,i,r)*vhm(i,r);
        rta(i,r) = sum(s, rtms(i,s,r)*((1-rtxs(i,s,r))*vxmd(i,s,r)+sum(j,vtwr(j,i,s,r))) -
                          rtxs(i,r,s)*vxmd(i,r,s));
        vim(i,r) = 0;
*.      vdm(i,r) = vam(i,r) - sum(j,vtc(j,i,r)) - rta(i,r);

        vdm(i,r) =  vom(i,r) - vst(i,r) - vhx(i,r) + vhm(i,r)*phim(i,r);
        rta(i,r)$vdm(i,r) =  (vam(i,r) - sum(j,vtc(j,i,r)) - vdm(i,r))/vdm(i,r);

        esubd(i) = 0;
        vhchk(i) = sum(r, vhx(i,r)*phix(i,r) - vhm(i,r));
        display vhchk;

        mktchk(i,r) = vom(i,r) - vst(i,r) - vhx(i,r) + vhm(i,r)*phim(i,r) - vdm(i,r);
        display mktchk;
);

parameter       bmkreport       Summary of benchmark dataset;
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

*       Subsets of energy commodities for nesting or specific references
* N.B.: Crude oil is rather treated as material input
set     fe(i)           Fossil fuels    /col, oil, gas/,
        co(i)           Only coal and oil for JW /col,oil/,
        fco(i)          Not coal and oil /gas/
        ele(i)          Electricity     /ele/,
        e(i)            Energy inputs   /col,oil,gas,ele/,
        xe(g)           Fossil fuels    /cru, gas, col/;

set eee(g) electricity as a subset of g /ele/;

*       Re-assign labels for capital and labor inputs
parameter
        vlm(g,r)        Labor inputs,
        vkm(g,r)        Capital inputs,
        vrm(g,r)        Resource inputs for fossil fuel production,
        etas(xe,r)      Fossil fuel supply elasticities,
        thetar(g,r)     Value share of resource-specific input
        esubr(g,r)      Cross-price elasticity of substitution in resource production;

vlm(g,r)        = vfm("lab",g,r);
vkm(g,r)        = vfm("cap",g,r);
vrm(xe,r)       = vkm(xe,r);
vkm(xe,r)       = 0;
evom("cap",r)   = sum(g, vkm(g,r));
display rtf0;


*       Fossil fuel calibration to exogenous supply elasticities
*       etas    =       Elasticity of supply
*       esubr   =       Elasticity of substitution
*       thetar  =       Value share of variable input

*       esubr = etas * thetar/(1-thetar)


*       Elasticities of fossil fuel supply:
etas("cru",r)  = 1; etas("gas",r)  = 1; etas("col",r)  = 4;

parameter       vshare  Benchmark value shares;
vshare(xe,r)$(vom(xe,r)  *(1-rto(xe,r)))    = vrm(xe,r)*(1+rtf("cap",xe,r))/(vom(xe,r)  *(1-rto(xe,r)));
esubr(xe,r)     = etas(xe,r) * vshare(xe,r)/(1-vshare(xe,r));

display xe,vshare;


scalar  ssk             Flag for sector-specific capital        /0/;
*       By default we assume region-specific capital
ssk = 1;

parameter
        esub_kle_m(g)   Elasticity of substitution between KLE and M,
        esub_m(g)       Elasticity of substitution within material composite,
        esub_kl_e(g)    Elasticity of substitution between KL and E,
        esub_k_l(g)     Elasticity of substitution between K and L,
        esub_e(g)       Elasticity of susbstituion within energy composite,
        esub_co(g);

*       Assign default mapping
esub_e(g)       = 0.5;
esub_co(g)      = 0.5;
esub_k_l(g)     = esubva(g);
esub_kle_m(g)   = 0.5;
esub_m(g)       = 0.3;
esub_kl_e(g)    = 0.5;

esub_m("C") = 0.5; esub_e("C")= 0.5; esub_kle_m("C") = 0.3;


*       Impose Leontief-fixed production for investment and public demand
set fixed(g)    /G, I/;

loop(g$fixed(g), esub_e(g) = 0; esub_m(g) = 0; esub_kl_e(g) = 0; esub_kle_m(g) = 0; esub_k_l(g) = 0;esub_co(g) = 0);


*       Armington elasticities are provided by GTAP (with adjustments if
*       values are too high -- e.g. this is the case for gas)
esubm(i)   = min(esubm(i),10);
esubd(i)   = min(esubd(i),esubm(i)/2);


*       Extensions for CO2 emission control policies
parameter
        refco2(i,g,r)   Base-year energy- and sector-specific emission flows (billion tons of CO2),
        co2_bmk(r)      Cumulative base-year CO2 emissions (billion tons of CO2),
        co2lim(r)       Carbon emission limits,
        cc(i,s,r)       Embodied carbon
        globaltarget    Global emission target (if positive we enact a global emission ceiling);

set     rtax(r)         Countries with a regional carbon market,
        ttax(r)         Countries with international carbon trading,
        tariff(i,s,r)   Flag for optional imposition of bilateral tariffs on embodied carbon,
        tariff_ho(i,r)  Flag for optional imposition of tariff on embodied carbon;


*       Convert CO2 emission units in billion tons of CO2 -- then the CO2 price
*       in the model will be readily available as USD per ton of CO2:

refco2(fe,g,r)     = (eco2d(fe,g,r) + eco2i(fe,g,r))/1e+3   ;
co2_bmk(r)         = sum((fe,g), refco2(fe,g,r));
co2lim(r)          = sum((fe,g), refco2(fe,g,r));
globaltarget       = 0;
cc(i,s,r)          = 0;
rtax(r)            = no;
ttax(r)            = no;
tariff(i,s,r)      = no;
tariff_ho(i,r)     = no;

display esub_e, eee;





$ontext
$model:gtap

$sectors:
        Y(g,r)$vom(g,r)                 ! Supply
        X(i,r)$phix(i,r)                ! Homogeneous exports
        A(i,r)$vam(i,r)                 ! Armington supply
        M(i,r)$(vim(i,r)+phim(i,r))     ! Imports
        YT(j)$vtw(j)                    ! Transportation services
        EMIT(r)                         ! Regional emission flows
        VA(eee,r)                       ! value added quantity index on the kl node in ele
        VE(eee,R)
        VCO(eee,R)
        VM(eee,r)
        vkle(eee,r)

$commodities:
        PW(i)$hot                       ! World Market Price (HO model)
        P(g,r)$(vom(g,r) or phim(g,r))  ! Domestic output price
        PA(i,r)$vam(i,r)                ! Armington composite price
        PM(j,r)$(arm and vim(j,r))      ! Import price
        PT(j)$vtw(j)                    ! Transportation services
        PL(r)                           ! Wage rate
        RK(g,r)$(ssk*vkm(g,r))          ! Return to sector-specific capital
        PR(g,r)$vrm(g,r)                ! Resource-specific factor price
        PCARB(r)                        ! Price of CO2
        PTCARB$card(ttax)               ! Price of tradable CO2
        PRCARB(r)$rtax(r)               ! Price of regional CO2
        PVA(eee,r)                      ! Price index on value added in ele
        PVE(eee,R)
        PCO(eee,r)
        PVM(eee,r)
        pkle(eee,r)


$consumers:
        RA(r)                           ! Representative agent

$auxiliary:
        TAU(i,s,r)$tariff(i,s,r)        ! Embodied carbon tariffs
        TAU_HO(i,r)$tariff_ho(i,r)      ! Embodied carbon tariffs on HO goods
        PHI$globaltarget                ! Global carbon accounting


$prod:Y(g,r)$(vom(g,r) and (not xe(g)) and (not eee(g))) s:esub_kle_m(g)   m(s):esub_m(g)   kle(s):esub_kl_e(g)   kl(kle):esub_k_l(g)
+ e(kle):esub_e(g)  fe.tl(e):0
        o:P(g,r)        q:vom(g,r)      a:RA(r)  t:rto(g,r)
        i:PA(i,r)       q:vafm(i,g,r)    p:(1+rtfa0(i,g,r))     a:RA(r) t:rtfa(i,g,r)    i.tl:$fe(i)    m:$(not e(i)) e:$(e(i) and (not fe(i)))
        i:PCARB(r)#(fe) q:refco2(fe,g,r) p:pcarb0(r)                                          fe.tl:
        i:PL(r)         q:vlm(g,r)       p:(1+rtf0("lab",g,r))  a:RA(r) t:rtf("lab",g,r) kl:
        i:RK(g,r)$ssk   q:vkm(g,r)       p:(1+rtf0("cap",g,r))  a:RA(r) t:rtf("cap",g,r) kl:

$prod:Y(eee,r) s:esub_kle_m(eee)
        o:P(eee,r)      q:vom(eee,r)    a:RA(r)  t:rto(eee,r)
        i:PVM(eee,r)    q:(sum(i$(not e(i)),vafm(i,eee,r)*(1+rtfa0(i,eee,r))))
        i:pkle(eee,r)   q:((vlm(eee,r)*(1+rtf0("lab",eee,r)) + vkm(eee,r)*(1+rtf0("cap",eee,r)))+(sum(i$e(i),vafm(i,eee,r)*(1+rtfa0(i,eee,r)))+sum(fe,refco2(fe,eee,r)*pcarb0(r))))


$Report:
        V:SY(eee,r) o:P(eee,r) prod:y(eee,r)

$prod:VKLE(eee,r) s:esub_kl_e(eee)
        o:PKLE(eee,r)   q:((vlm(eee,r)*(1+rtf0("lab",eee,r)) + vkm(eee,r)*(1+rtf0("cap",eee,r)))+(sum(i$e(i),vafm(i,eee,r)*(1+rtfa0(i,eee,r)))+sum(fe,refco2(fe,eee,r)*pcarb0(r))))
        i:PVA(eee,r)    q:(vlm(eee,r)*(1+rtf0("lab",eee,r)) + vkm(eee,r)*(1+rtf0("cap",eee,r)))
        i:PVE(eee,R)    q:(sum(i$e(i),vafm(i,eee,r)*(1+rtfa0(i,eee,r)))+sum(fe,refco2(fe,eee,r)*pcarb0(r)))

$prod:VM(EEE,R) s:esub_m(eee)
         o:PVM(eee,r)            q:(sum(i$(not e(i)),vafm(i,eee,r)*(1+rtfa0(i,eee,r))))
         i:PA(i,r)$(not e(i))    q:vafm(i,eee,r)  p:(1+rtfa0(i,eee,r)) a:RA(r) t:Rtfa(i,eee,r)

$prod:VE(eee,r)   s:esub_e(eee) fe.tl(s):0 ele:0
         o:PVE(eee,r)                   q:(sum(i$e(i),vafm(i,eee,r)*(1+rtfa0(i,eee,r)))+sum(fe,refco2(fe,eee,r)*pcarb0(r)))
         i:PCO(eee,r)                   q:(sum(i$co(i),vafm(i,eee,r)*(1+rtfa0(i,eee,r)))+sum(i$co(i),refco2(i,eee,r)*pcarb0(r)))
         i:PA(i,r)$((e(i))$(not co(i)))             q:vafm(i,eee,r)  p:(1+rtfa0(i,eee,r))   a:RA(r) t:rtfa(i,eee,r)    i.tl:$fco(i)
         i:pcarb(r)#(fco)               q:refco2(fco,eee,r)     p:pcarb0(r)     fco.tl:


$prod:VCO(eee,r)   s:esub_co(eee) fe.tl(s):0
         o:PCO(eee,r)                  q:(sum(i$co(i),vafm(i,eee,r)*(1+rtfa0(i,eee,r)))+sum(i$co(i),refco2(i,eee,r)*pcarb0(r)))
         i:PA(i,r)$(co(i))             q:vafm(i,eee,r)  p:(1+rtfa0(i,eee,r))   a:RA(r) t:rtfa(i,eee,r)    i.tl:$co(i)
         i:pcarb(r)#(co)               q:refco2(co,eee,r)     p:pcarb0(r)   co.tl:

$report:
        V:DPVA(eee,r) i:PVA(eee,r) prod:Y(eee,r)
        V:DPVE(eee,r) i:PVE(eee,r) prod:Y(eee,r)
        V:DPVM(eee,r) i:PVM(eee,r) prod:Y(eee,r)
        V:DPVKLE(EEE,R) i:PKLE(eee,r) prod:y(eee,r)

$Prod:VA(eee,r) s:esub_k_l(eee)
        o:PVA(eee,r)    q:(vlm(eee,r)*(1+rtf0("lab",eee,r)) + vkm(eee,r)*(1+rtf0("cap",eee,r)))
        i:PL(r)         q:vlm(eee,r)     p:(1+rtf0("lab",eee,r))  a:RA(r) t:rtf("lab",eee,r)
        i:RK(eee,r)$ssk q:vkm(eee,r)     p:(1+rtf0("cap",eee,r))  a:RA(r) t:rtf("cap",eee,r)

$prod:Y(g,r)$xe(g)      s:esubr(g,r)  id:0
        o:P(g,r)        q:vom(g,r)      a:RA(r)  t:rto(g,r)
        i:PA(i,r)       q:vafm(i,g,r)    p:(1+rtfa0(i,g,r))     a:RA(r) t:rtfa(i,g,r)    id:
        i:PCARB(r)#(fe) q:refco2(fe,g,r) p:pcarb0(r)                                          id:
        i:PL(r)         q:vlm(g,r)       p:(1+rtf0("lab",g,r))  a:RA(r) t:rtf("lab",g,r) id:
        i:PR(g,r)       q:vrm(g,r)       p:(1+rtf0("cap",g,r))  a:RA(r) t:rtf("cap",g,r)


$prod:EMIT(r)
       o:PCARB(r)               q:1
       i:PRCARB(r)$rtax(r)      q:1
       i:PTCARB$ttax(r)         q:1
       i:PL(r)                  q:2e-6


$report:
        V:R_vafm(i,g,r)$vafm(i,g,r)     i:PA(i,r)       prod:Y(g,r)

$prod:YT(j)$vtw(j)  s:1
        o:PT(j)         q:vtw(j)
        i:P(j,r)        q:vst(j,r)

$report:
        V:R_vst(j,r)$vst(j,r)   i:P(j,r)        prod:YT(j)

$prod:X(i,r)$phix(i,r)
        o:PW(i)         q:phix(i,r)
        i:P(i,r)        q:1             a:RA(r) t:rtx(i,r)

$prod:M(i,r)$phim(i,r)
        o:P(i,r)        q:(phim(i,r))
        i:PT(j)         q:phit(j,i,r)   a:RA(r) t:rtm(i,r)
        i:PW(i)         q:1             a:RA(r) t:rtm(i,r)
+ a:RA(r) n:TAU_HO(i,r)$tariff_ho(i,r) m:cc(i,r,r)$tariff_ho(i,r)



$prod:M(i,r)$(vim(i,r) and arm) s:esubm(i)  s.tl:0$vxmd(i,s,r)
        o:PM(i,r)       q:vim(i,r)
        i:P(i,s)        q:(vxmd(i,s,r)) p:pvxmd(i,s,r) s.tl:
+ a:RA(s) t:(-rtxs(i,s,r)) a:RA(r) t:(rtms(i,s,r)*(1-rtxs(i,s,r)))
+ a:RA(r) n:TAU(i,s,r)$tariff(i,s,r) m:cc(i,s,r)$tariff(i,s,r)

        i:PT(j)#(s)     q:vtwr(j,i,s,r) p:pvtwr(i,s,r) s.tl:
+               a:RA(r) t:rtms(i,s,r)

$prod:A(i,r)$vam(i,r)  s:esubd(i)
        o:PA(i,r)       q:vam(i,r)
        i:PT(j)         q:vtc(j,i,r)
        i:PM(i,r)$arm   q:vim(i,r)
        i:P(i,r)        q:vdm(i,r)  a:RA(r) t:rta(i,r)

$report:
        V:R_vxmd(i,r,s)$(arm and vxmd(i,r,s))   i:P(i,r)        prod:M(i,s)

$demand:RA(r)
        d:P("c",r)      q:vom("c",r)
        e:P("c",rnum)   q:vb(r)
        e:P("g",r)      q:(-vom("g",r))
        e:P("i",r)      q:(-vom("i",r))
        e:RK(g,r)$ssk   q:vkm(g,r)
        e:PL(r)         q:evom("lab",r)
        e:PR(xe,r)      q:vrm(xe,r)
        e:PRCARB(r)$rtax(r)     q:co2lim(r)     r:phi$globaltarget
        e:PTCARB$ttax(r)        q:co2lim(r)     r:phi$globaltarget


$constraint:PHI$globaltarget
        globaltarget =e= sum(r, EMIT(r));

$constraint:TAU(i,s,r)$tariff(i,s,r)
        P(i,s)*TAU(i,s,r) =e= PCARB(r);

$constraint:TAU_HO(i,r)$tariff_ho(i,r)
        PW(i)*TAU_HO(i,r) =e= PCARB(r);

$offtext
$sysinclude mpsgeset gtap

if (hot,
        X.L(i,r) = vhx(i,r);
        M.L(i,r) = vhm(i,r);
);

display phix, phim;
parameter phixphim;
phixphim(i,r) = max(phix(i,r)*phim(i,r)-1,0);
display phixphim;


PCARB.l(r)  = 0;
PTCARB.l    = 0;
PRCARB.l(r) = 0;
EMIT.l(r)   = co2lim(r);


gtap.workspace=50;
$onecho >path.opt
convergence_tolerance=1e-5
$offecho

gtap.optfile = 1;
gtap.iterlim = 0;
$include gtap.gen
solve gtap using mcp;
Abort$(gtap.objval gt 1e-3) "Benchmark MPSGE model is not balanced";

alias(i,ii)