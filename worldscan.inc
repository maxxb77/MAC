Parameter
        theta_kl_je(eee,r)        value share of k in KL nest of ele for JE model,
        theta_ve_je(i,eee,r)      value share of i in E nest of KLE for JE model,
        theta_col_je(i,eee,r)     value share of i in CO nest of VE for JE model,
        theta_gasoil_je(i,eee,r)  value share of CO nest in EEE nes for JE modelt
        theta_vm_je(i,eee,r)      value share of i in M node of ELE for JE model,
        alpha_ve_je(z,i,eee,r)    alpha value for E nest for JE model,
        alpha_co_je(z,i,eee,r)    alpha value for coal oil nest for JE model,
        zprfcheck_je              zero profit check for JE model,
        ve0_je                    reference value of E nest for JE model,
        kle0_je                   reference KLE nest value for JE model,
        theta_pvm_je              value share of materials in top nest for JE model,
        theta_kl_e_je             value share of kl in kle nest for JE model,
        theta_e_je                value share of e in kle nest for JE model,
        theta_va_je               value share of VA in top nest for JE model,
        refp_je                   reference unit cost net of output tax for JE model,
        rto1_je(eee,r)            opposite of rto -- unused for JE model,
        theta_co2_je(i,eee,r)     value share of carbon in each fe nest (very small value) for JE model,
        theta_colgasoil_je        value share of cgo in E nest for JE model,
        theta_col_gasoil(eee,r)   value share of col or GO in CGO nest;

theta_co2_je(fe,eee,r) = pcarb0(r)*refco2(fe,eee,r) / (vafm(fe,eee,r)*(1+rtfa0(fe,eee,r)) + pcarb0(r)*refco2(fe,eee,r));

theta_co2_je("ele",eee,r) = 0;

theta_kl_je(eee,r) = vkm(eee,r)*(1+rtf0("cap",eee,r)) / (vlm(eee,r)*(1+rtf0("lab",eee,r)) + vkm(eee,r)*(1+rtf0("cap",eee,r)));

set eeef(eee) /ele/;
set rf(r)     /USA/;


parameter theta_ve0_je "reference value share of energy - used in testing";

*note that the denominator did not need to change since the nest below VE includes the same elements
theta_ve0_je(i,eee,r)$e(i) =  (vafm(i,eee,r)*(1+rtfa0(i,eee,r)) + pcarb0(r)*refco2(i,eee,r)$fe(i))/ (sum(ii$e(ii),vafm(ii,eee,r)*(1+rtfa0(ii,eee,r)))+sum(fe,refco2(fe,eee,r)*pcarb0(r)));


*change to allow for dynamic representation of nest items
set nest1(i) "any index of e(i) in the 1st nest"   /ele/; //default for worldscan is ele
set nest2(i) "any index of e(i) in the 2nd nest"   /col/; //default for worldscan is coal
set nest3(i) "any index of e(i) in the 3rd nest"   /gas, oil/; //default for worldscan is gas and oil

*nest one doesn't change
theta_ve_je(i,eee,r)$nest1(i) = theta_ve0_je(i,eee,r) / sum(ii$e(ii), theta_ve0_je(ii,eee,r));

*nest two does change - notice the denominator here
theta_ve_je(i,eee,r)$nest2(i) = theta_ve0_je(i,eee,r) / sum(ii$(nest2(ii) or nest3(ii)),theta_ve0_je(i,eee,r));

*nest three does change - notice the denominator again
theta_ve_je(i,eee,r)$nest3(i) = theta_ve0_je(i,eee,r) / sum(ii$nest3(ii),theta_ve0_je(ii,eee,r));

*for next two equations, see email FW: This seems to work and gams file 'calibexamp.gms' in dropbox folder
alpha_ve_je(z,i,eee,r)$((e(i))$(not co(i))) = ((vafm(i,eee,r)*(1+rtfa(i,eee,r))) / (vafm(i,eee,r)*(1+rtfa(i,eee,r)) - pcarb0(r)*refco2(i,eee,r))) * pa.l(i,r)
                        + refco2(i,eee,r) * pcarb0(r) / ((vafm(i,eee,r)*(1+rtfa(i,eee,r)))-refco2(i,eee,r) * pcarb0(r)) * pcarb.l(r)/pcarb0(r);


alpha_co_je(z,i,eee,r)$co(i) = ((vafm(i,eee,r)*(1+rtfa(i,eee,r))) / (vafm(i,eee,r)*(1+rtfa(i,eee,r)) - pcarb0(r)*refco2(i,eee,r))) * pa.l(i,r)
                        + refco2(i,eee,r) * pcarb0(r) / ((vafm(i,eee,r)*(1+rtfa(i,eee,r)))-refco2(i,eee,r) * pcarb0(r)) * pcarb.l(r)/pcarb0(r);


theta_vm_je(i,eee,r)$(not e(i)) = (vafm(i,eee,r)*(1+rtfa0(i,eee,r))) / (sum(ii$(not e(ii)),vafm(ii,eee,r)*(1+rtfa0(ii,eee,r))));


zprfcheck_je("1",eee,r) = pve.l(eee,r) - sum(i$e(i),theta_ve_je(i,eee,r)*alpha_ve_je("1",i,eee,r));


theta_pvm_je(eee,r) = (sum(i$(not e(i)),vafm(i,eee,r)*(1+rtfa0(i,eee,r))))  /
                  ((sum(i$(not e(i)),vafm(i,eee,r)*(1+rtfa0(i,eee,r)))) +
                   ((vlm(eee,r)*(1+rtf0("lab",eee,r)) + vkm(eee,r)*(1+rtf0("cap",eee,r)))
                  +(sum(i$e(i),(1-theta_co2_je(i,eee,r))*vafm(i,eee,r)*(1+rtfa0(i,eee,r)))+sum(fe,theta_co2_je(fe,eee,r)*refco2(fe,eee,r)*pcarb0(r))))
                  );


theta_kl_e_je(eee,r) = 1 - theta_pvm_je(eee,r);


theta_va_je(eee,r)  = (vlm(eee,r)*(1+rtf0("lab",eee,r))  +  vkm(eee,r)*(1+rtf0("cap",eee,r)))/
                      (sum(i$e(i),(1-theta_co2_je(i,eee,r))*vafm(i,eee,r)*(1+rtfa0(i,eee,r))) + SUM(FE,theta_co2_je(fe,eee,r)*refco2(FE,eee,r)*PCARB0(R)) +
                       vlm(eee,r)*(1+rtf0("lab",eee,r)) + vkm(eee,r)*(1+rtf0("cap",eee,r)) );


theta_e_je(eee,r)   = 1-theta_va_je(EEE,R);

parameter checkeleval;

checkeleval(eee,r) = theta_ve_je("ele",eee,r)*theta_e_je(eee,r)*theta_kl_e_je(eee,r);

display checkeleval;

kle0_je(eee,r) = ((vlm(eee,r)*(1+rtf0("lab",eee,r)) + vkm(eee,r)*(1+rtf0("cap",eee,r)))
                 +(sum(i$e(i),(1-theta_co2_je(i,eee,r))*vafm(i,eee,r)*(1+rtfa0(i,eee,r)))+sum(fe,theta_co2_je(fe,eee,r)*refco2(fe,eee,r)*pcarb0(r))));

ve0_je(eee,r) = (sum(i$e(i),(1-theta_co2_je(i,eee,r))*vafm(i,eee,r)*(1+rtfa0(i,eee,r)))+sum(fe,theta_co2_je(fe,eee,r)*refco2(fe,eee,r)*pcarb0(r)));

refp_je(eee,r) = 1-rto(eee,r);

rto1_je(eee,r) = 1-rto(eee,r);


*display theta_col_je, theta_gasoil_je, theta_colgasoil_je,
*        theta_kl_je, theta_ve_je, theta_vm_je, alpha_ve_je, zprfcheck_je, kle0_je, theta_pvm_je, theta_kl_e_je, theta_e_je, theta_va_je, refp_je, rto1_je;

display theta_ve_je,zprfcheck_je;

*Second -- Create new variables and equations that are indexed by MACC data point (z)
Positive Variables
*prices
PVM_JE(z,eee,r)       "price of VM in JE model",
PKLE_JE(z,eee,r)      "price of KLE in JE model",
PVE_JE(z,eee,r)       "price of VE in JE model",
PVA_JE(z,eee,r)       "price of VA in JE model",
P_JE(z,eee,r)         "price of electricity in JE model",
PCARB_(Z,R)           "price of carbon in JE model",
pn1_JE(z,eee,r)       "price of nest n1 in JE model",
pn2_je(z,eee,r)       "price of nest n2 in JE model",
pn3_je(z,eee,r)       "price of nest n3 in JE model",
pcol_je(z,eee,r)      "price of col in JE model",
pgasoil_je(z,eee,r)   "price of go in JE model",

*and activity levels
Y_JE(z,eee,r)         "value (quantity) of Y in JE model",
VE_JE(z,eee,r)        "value (quantity) of VE in JE model",
VKLE_JE(z,eee,r)      "value (quantity) of VKLE in JE model",
VA_JE(z,eee,r)        "value (quantity) of VA in JE model",
VM_JE(z,eee,r)        "value (quantity) of VM in JE model",
vcol_je(z,eee,r)      "value (quantity) of vcol in JE model",
vgasoil_je(z,eee,r)   "value (quantity) of vgasoil in JE model",
vn1_JE(z,eee,r)       "value (quantity) of vn1 in JE model",
vn2_JE(z,eee,r)       "value (quantity) of vn2 in JE model",
vn3_je(z,eee,r)       "value (quantity) of vn3 in JE model"


*elasticities as variables (not indexed by z)
ESUB_KLE_M_JE(eee,r)      "elasticity of KLE_M nest in JE model"
esub_m_JE(eee,r)          "elasticity of m nest in JE model"
esub_k_l_JE(eee,r)        "elasticity of k_l nest in JE model"
esub_kl_e_JE(eee,r)       "elasticity of kl_e nest in JE model"
ESUB_EEE_JE(EEE,R)        "elasticity of EEE nest in JE model"
esub_n1_je(eee,r)         "elasticity of n1 nest in JE model"
esub_n2_je(eee,r)         "elasticity of n2 nest in JE model"
esub_n3_je(eee,r)         "elasticity of n3 nest in JE model"
esub_Fe_je(eee,r)         "elasticity of Fe nest in JE model"

esub_col_je, esub_gasoil_je;
;


Equations
        zpr_Y_JE(z,eee,r)    zero profits for Y
        mkt_Y_JE(z,eee,r)    market for Y

        zpr_kle_JE(z,eee,r)  zero profits at the KLE node of kle in ele
        mkt_kle_JE(z,eee,r)  market for PKLE

        zpr_kl_JE(z,eee,r)   zero profits at the KL node of kle in ele
        mkt_PVA_JE(z,eee,r)  market for PVA

        zpr_vm_JE(z,eee,r)   zero profits at the M node of ele
        mkt_PVM_JE(z,eee,r)  market for PVA

        zpr_n1_JE(z,eee,r)   zero profits at the VE node of kle in ele
        mkt_n1_JE(z,eee,r)  market for PVA

        zpr_n2_JE(z,eee,r) zero profits of n2 in JE model
        mkt_n2_JE(z,eee,r) market for n2

        zpr_n3_JE(z,eee,r) zero profits of n3 in JE model
        mkt_n3_JE(z,eee,r) market for n3
;


*Third -- the model is the same as before except now the necessary variables are indexed by z
*=========================
*Market for KLE and M nest (top)
*=========================

zpr_Y_JE(z,eee,r)$(rf(r)$eeef(eee)$ZFEAS(Z))..
           refp_je(EEE,R)*(
                 (theta_pvm_je(eee,r)   *PVM_JE(z,eee,r) **(1-esub_kle_m(eee))
               + (1-theta_pvm_je(eee,r))*PKLE_JE(z,eee,r)**(1-esub_kle_m(eee)))**(1/(1-esub_kle_m(eee)))
                        )
           - P_JE(z,eee,r)*(1-RTO(EEE,R))
=g= 0;

*mkt_Y_JE(z,eee,r)$(rf(r)$eeef(eee)$ZFEAS(Z)).. y_JE(z,EEE,R)*vom(eee,r) - SY.L(eee,r) =g=0;
mkt_Y_JE(z,eee,r)$(rf(r)$eeef(eee)$ZFEAS(Z)).. y_JE(z,EEE,R)*vom(eee,r) - SY.L(eee,r)*p_JE(z,eee,r)**mu =g= 0;

*=================
*Market for M nest (second)
*=================



zpr_vm_JE(Z,eee,r)$(rf(r)$eeef(eee)$ZFEAS(Z)).. (sum(i$(not e(i)), theta_vm_je(i,eee,r) * (PA.L(I,R) * (1+RTFA(I,EEE,R)) / (1+RTFA0(I,EEE,R)) ) ** (1-ESUB_m_JE(eee,r))))**(1/(1-ESUB_m_JE(eee,r)))
                 - PVM_JE(z,eee,R) =G= 0;

mkt_PVM_JE(Z,eee,r)$(rf(r)$eeef(eee)$ZFEAS(Z))..
                  VM_JE(z,eee,r)*theta_pvm_je(eee,r)*vom(eee,r)
                  - Y_JE(z,eee,r)*theta_pvm_je(eee,r)*vom(eee,r)*refp_je(eee,r)*(P_JE(Z,eee,r)/(PVM_JE(z,eee,r)))**ESUB_KLE_M_JE(eee,r) =g= 0;

*===============================
*Market for KLE nest (second)
*===============================

zpr_kle_JE(Z,eee,r)$(rf(r)$eeef(eee)$ZFEAS(Z))..
        (theta_e_je(eee,r)*PVE_JE(Z,eee,r)**(1-esub_kl_e_JE(eee,r)) +
         theta_va_je(eee,r)*PVA_JE(z,eee,r)**(1-esub_kl_e_JE(eee,r)))**(1/(1-esub_kl_e_JE(eee,r)))
         - PKLE_JE(z,eee,r) =g= 0;

mkt_kle_JE(Z,eee,r)$(rf(r)$eeef(eee)$ZFEAS(Z))..
        vkle_JE(z,eee,r)*kle0_je(eee,r)
        - Y_JE(z,eee,r)*kle0_je(eee,r)*(P_JE(Z,eee,r)/PKLE_JE(z,eee,r))**ESUB_KLE_m_JE(eee,r) =g= 0;


display theta_e_je;


*=================
*Market for KL nest (third)
*=================

zpr_kl_JE(Z,eee,r)$(rf(r)$eeef(eee)$ZFEAS(Z))..
        (   theta_kl_je(eee,r) * (RK.L(eee,r)*(1+rtf("cap",eee,r))/(1+rtf0("cap",eee,r)))**(1-esub_k_l_JE(eee,r))
        +  (1-theta_kl_je(eee,r))* (PL.L(r)*(1+rtf("lab",eee,r))/(1+rtf0("lab",eee,r)))**(1-esub_k_l_JE(eee,r)))**(1/(1-esub_k_l_JE(eee,r)))
        - PVA_JE(Z,eee,r) =g= 0;


mkt_PVA_JE(Z,eee,r)$(rf(r)$eeef(eee)$ZFEAS(Z))..
        VA_JE(z,eee,r)*theta_va_je(eee,r) * kle0_je(eee,r) -
        vkle_JE(z,eee,r) * theta_va_je(eee,r) * kle0_je(eee,r)*(PKLE_JE(z,eee,r)/PVA_JE(Z,eee,r))**esub_kl_e_JE(eee,r) =g=0;


*=================
*Market for first custom nest (third)
*=================


*calculation method doesn't matter for original values
parameter n1_0, n2_0, n3_0;

n1_0(eee,r) = ve0_je(eee,r);
n2_0(eee,r) = (1-sum(i$nest1(i),theta_ve_je(i,eee,r)))*ve0_je(eee,r);
n3_0(eee,r) = (1-sum(i$nest2(i),theta_ve_je(i,eee,r)))*n2_0(eee,r);

display n1_0, n2_0, n3_0;

n1_0(eee,r) = sum(i$e(i),(1-theta_co2_je(i,eee,r))*vafm(i,eee,r)*(1+rtfa0(i,eee,r))+theta_co2_je(i,eee,r)*refco2(i,eee,r)*pcarb0(r));
n2_0(eee,r) = sum(i$(nest2(i) or nest3(i)),(1-theta_co2_je(i,eee,r))*vafm(i,eee,r)*(1+rtfa0(i,eee,r))+theta_co2_je(i,eee,r)*refco2(i,eee,r)*pcarb0(r));
n3_0(eee,r) = sum(i$nest3(i),(1-theta_co2_je(i,eee,r))*vafm(i,eee,r)*(1+rtfa0(i,eee,r))+theta_co2_je(i,eee,r)*refco2(i,eee,r)*pcarb0(r));

display n1_0, n2_0, n3_0;


zpr_n1_JE(z,eee,r)$(rf(r)$eeef(eee)$ZFEAS(Z))..
            ((sum(i$nest1(i),theta_ve_je(i,eee,r)*(
                        ((vafm(i,eee,r)*(1+rtfa(i,eee,r))) / (vafm(i,eee,r)*(1+rtfa(i,eee,r)) - pcarb0(r)*refco2(i,eee,r)))
                        * (pa.l(i,r)$(NOT ELE(I)) + PA.L(I,R)$(ELE(I) AND (SWPRecurse = 0))+P_JE(z,"ele","usa")$(ELE(I) AND (SWPRecurse = 1)))
                        + refco2(i,eee,r) * pcarb0(r) / ((vafm(i,eee,r)*(1+rtfa(i,eee,r)))-refco2(i,eee,r) * pcarb0(r)) * pcarb_.l(z,r)/pcarb0(r)
                    )**(1-esub_n1_JE(eee,r))))
                  + (1-sum(i$nest1(i),theta_ve_je(i,eee,r))) * pn2_JE(z,eee,r) ** (1-esub_n1_JE(eee,r))
            )**(1/(1-esub_n1_JE(eee,r)))
          - pve_JE(z,eee,r)
         =G= 0;


mkt_n1_JE(z,eee,r)$(rf(r)$eeef(eee)$ZFEAS(Z))..
        ve_JE(z,eee,r)   * n1_0(eee,r) -
        VKLE_JE(z,eee,r) * n1_0(eee,r) * (PKLE_JE(z,eee,r)/pve_JE(z,eee,r))**esub_kl_e_JE(eee,r) =g= 0;

equation mkt_ve_fe, zpr_ve_fe;

zpr_ve_fe(z,eee,r)$(rf(r)$eeef(eee)$ZFEAS(Z))..
            (sum(i$ele(i),theta_ve_je(i,eee,r)*(
                        ((vafm(i,eee,r)*(1+rtfa(i,eee,r))) / (vafm(i,eee,r)*(1+rtfa(i,eee,r)) - pcarb0(r)*refco2(i,eee,r)))
                        * (pa.l(i,r)$(NOT ELE(I)) + PA.L(I,R)$(ELE(I) AND (SWPRecurse = 0))+P_JE(z,"ele","usa")$(ELE(I) AND (SWPRecurse = 1))
                        + refco2(i,eee,r) * pcarb0(r) / ((vafm(i,eee,r)*(1+rtfa(i,eee,r)))-refco2(i,eee,r) * pcarb0(r)) * pcarb_.l(z,r)/pcarb0(r)
                    ))**(1-esub_eee_JE(eee,r)))
                  + (1-sum(i$ele(i),theta_ve_je(i,eee,r))) * pcol_JE(z,eee,r) ** (1-esub_eee_JE(eee,r))
            )**(1/(1-esub_eee_JE(eee,r)))
          - pve_JE(z,eee,r)
         =G= 0;


mkt_ve_fe(z,eee,r)$(rf(r)$eeef(eee)$ZFEAS(Z))..
        ve_JE(z,eee,r)   * (1-theta_va_je(eee,r)) * kle0_je(eee,r)  -
        VKLE_JE(z,eee,r) * (1-theta_va_je(eee,r)) * kle0_je(eee,r) * (PKLE_JE(z,eee,r)/pve_JE(z,eee,r))**esub_kl_e_JE(eee,r) =g=  0;


*=================
*Market for the second custom Nest (fourth)
*=================

zpr_n2_JE(z,eee,r)$(rf(r)$eeef(eee)$ZFEAS(Z))..
            (sum(i$nest2(i),theta_ve_je(i,eee,r)*(
                        ((vafm(i,eee,r)*(1+rtfa(i,eee,r))) / (vafm(i,eee,r)*(1+rtfa(i,eee,r)) - pcarb0(r)*refco2(i,eee,r)))
                        * (pa.l(i,r)$(NOT ELE(I)) + PA.L(I,R)$(ELE(I) AND (SWPRecurse = 0))+P_JE(z,"ele","usa")$(ELE(I) AND (SWPRecurse = 1))
                        + refco2(i,eee,r) * pcarb0(r) / ((vafm(i,eee,r)*(1+rtfa(i,eee,r)))-refco2(i,eee,r) * pcarb0(r)) * pcarb_.l(z,r)/pcarb0(r)
                    ))**(1-esub_n2_JE(eee,r)))
                  + (1-sum(i$nest2(i),theta_ve_je(i,eee,r))) * pn3_JE(z,eee,r) ** (1-esub_n2_JE(eee,r))
            )**(1/(1-esub_n2_JE(eee,r)))
          - pn2_JE(z,eee,r)
         =G= 0;

mkt_n2_JE(z,eee,r)$(rf(r)$eeef(eee)$ZFEAS(Z))..
        vn2_JE(z,eee,r)* n2_0(eee,r) -
        ve_JE(z,eee,r) * n2_0(eee,r) *(Pve_JE(z,eee,r)/pn2_JE(z,eee,r))**esub_n1_JE(eee,r) =g= 0;


display theta_ve_je, theta_e_je,theta_ve_je;

*=================
*Market for GASOIL nest (SIXTH)
*=================

*mkt_PVA_de(Z,eee,r)$ZFEAS(Z)..
*        VA_de(z,eee,r)*(vlm(eee,r)*(1+rtf0("lab",eee,r)) + vkm(eee,r)*(1+rtf0("cap",eee,r))) -
*        vkle_de(z,eee,r)*theta_va_de(eee,r)*kle0_de(eee,r)*(PKLE_de(z,eee,r)/PVA_de(Z,eee,r))**esub_kl_e_de(eee,r) =g=0;


zpr_n3_JE(z,eee,r)$(rf(r)$eeef(eee)$ZFEAS(Z))..
            (sum(i$nest3(i),theta_ve_je(i,eee,r)*(
                        ((vafm(i,eee,r)*(1+rtfa(i,eee,r))) / (vafm(i,eee,r)*(1+rtfa(i,eee,r)) - pcarb0(r)*refco2(i,eee,r)))
                        * (pa.l(i,r)$(NOT ELE(I)) + PA.L(I,R)$(ELE(I) AND (SWPRecurse = 0))+P_JE(z,"ele","usa")$(ELE(I) AND (SWPRecurse = 1))
                        + refco2(i,eee,r) * pcarb0(r) / ((vafm(i,eee,r)*(1+rtfa(i,eee,r)))-refco2(i,eee,r) * pcarb0(r)) * pcarb_.l(z,r)/pcarb0(r)
                    ))**(1-esub_n3_JE(eee,r)))
            )**(1/(1-esub_n3_JE(eee,r)))
          - pn3_JE(z,eee,r)
         =G= 0;

mkt_n3_JE(z,eee,r)$(rf(r)$eeef(eee)$ZFEAS(Z))..
        vn3_JE(z,eee,r) * n3_0(eee,r) -
        vn2_JE(z,eee,r) * n3_0(eee,r)*(Pn2_JE(z,eee,r)/pn3_JE(z,eee,r))**esub_n2_JE(eee,r) =g= 0;


*separate model structure, only has ELE and FE

equation zpr_fe_je(z,eee,r), mkt_fe_je(z,eee,r);
positive variable pfe_je(z,eee,r), vfe_je(z,eee,r);

parameter theta_fe_je(i,eee,r);

theta_fe_je(i,eee,r)$fe(i) =  (vafm(i,eee,r)*(1+rtfa0(i,eee,r)) + pcarb0(r)*refco2(i,eee,r)$fe(i)) / (sum(ii$fe(ii),vafm(ii,eee,r)*(1+rtfa0(ii,eee,r)))+sum(fe,refco2(fe,eee,r)*pcarb0(r)));

zpr_fe_JE(z,eee,r)$(rf(r)$eeef(eee)$ZFEAS(Z))..
                        (sum(i$fe(i),theta_fe_je(i,eee,r)*(
                                                    ((vafm(i,eee,r)*(1+rtfa(i,eee,r))) / (vafm(i,eee,r)*(1+rtfa(i,eee,r)) - pcarb0(r)*refco2(i,eee,r))) * pa.l(i,r)
                                                    + refco2(i,eee,r) * pcarb0(r) / ((vafm(i,eee,r)*(1+rtfa(i,eee,r)))-refco2(i,eee,r) * pcarb0(r)) * pcarb_.l(z,r)/pcarb0(r)
                                                            )**(1-esub_fe_JE(eee,r))) )**(1/(1-esub_fe_JE(eee,r)))
                        - PCOL_JE(z,eee,r)
         =G= 0;


*note that the theta_col_je("col",eee,r) is still the share of fossil fuels in the ELE-COLGASOIL nest
mkt_fe_JE(z,eee,r)$(rf(r)$eeef(eee)$ZFEAS(Z))..
        VCOL_JE(z,eee,r)*(1-theta_ve_je("ele",eee,r))*theta_e_je(eee,r)*kle0_je(eee,r) -
        VE_JE(z,eee,r)*(1-theta_ve_je("ele",eee,r))*theta_e_je(eee,r)*kle0_je(eee,r)*(PvE_JE(z,eee,r)/PCOL_JE(z,eee,r))**esub_eee_JE(eee,r) =g= 0;
;

*display theta_col_je;

VFE_JE.l(z,eee,r) = 1;
PFE_JE.l(z,eee,r) = 1;

*for now, the elasticities are fixed at their respective parameter values
ESUB_KLE_M_JE.fx(eee,r) = ESUB_KLE_M(eee);
esub_m_JE.fx(eee,r) = esub_m(eee);
esub_k_l_JE.fx(eee,r) = esub_k_l(eee);
esub_kl_e_JE.fx(eee,r) = esub_kl_e(eee);
esub_EEE_JE.fx(eee,r) = esub_e(eee);
esub_FE_JE.fx(eee,r) = 0.5;
esub_COL_JE.fx(eee,r) = esub_e(eee);
esub_GASOIL_JE.fx(eee,r) = esub_e(eee);

esub_n1_JE.fx(eee,r) = 0.25;
esub_n2_JE.fx(eee,r) = 0.7;
esub_n3_JE.fx(eee,r) = 0.5;

ESUB_KLE_M_JE.fx(eee,r) = 0.01;
esub_m_JE.fx(eee,r) = 0.6;
esub_k_l_JE.fx(eee,r) = 0.85;
esub_kl_e_JE.fx(eee,r) = 0.5;
esub_EEE_JE.fx(eee,r) = 0.25;
esub_COL_JE.fx(eee,r) = 0.7;
esub_GASOIL_JE.fx(eee,r) = 0.5;

*level values are those from the previous solve
PVM_JE.l(z,eee,r)$ZFEAS(Z) =  PVM.l(eee,r);
PKLE_JE.l(z,eee,r)$ZFEAS(Z) =  PKLE.l(eee,r);
PVE_JE.l(z,eee,r)$ZFEAS(Z) =  PVE.l(eee,r);
PVA_JE.l(z,eee,r)$ZFEAS(Z) =  PVA.l(eee,r);
P_JE.l(z,eee,r)$ZFEAS(Z) =  P.l(eee,r);
PCARB_.l(Z,R)$ZFEAS(Z) =  PCARB.l(R);
VE_JE.l(z,eee,r)$ZFEAS(Z) =  VE.l(eee,r);
VKLE_JE.l(z,eee,r)$ZFEAS(Z) =  VKLE.l(eee,r);
VA_JE.l(z,eee,r)$ZFEAS(Z) =  VA.l(eee,r);
VM_JE.l(z,eee,r)$ZFEAS(Z) =  VM.l(eee,r);
Y_JE.l(z,eee,r)$ZFEAS(Z) =  Y.l(eee,r);
*vcol_JE.l(z,eee,r)$ZFEAS(Z) = vcol_je.l(eee,r);
*pcol_JE.l(z,eee,r)$ZFEAS(Z) = pcol_je.l(eee,r);
*vgasoil_JE.l(z,eee,r)$ZFEAS(Z) = vgasoil.l(eee,r);
*pgasoil_JE.l(z,eee,r)$ZFEAS(Z) = pgasoil.l(eee,r);

vn1_JE.l(z,eee,r) = n1_0(eee,r);
vn2_JE.l(z,eee,r) = n2_0(eee,r);
vn3_JE.l(z,eee,r) = n3_0(eee,r);

vn1_JE.l(z,eee,r) = 1;
vn2_JE.l(z,eee,r) = 1;
vn3_JE.l(z,eee,r) = 1;

pn1_je.l(z,eee,r) = 1;
pn2_je.l(z,eee,r) = 1;
pn3_je.l(z,eee,r) = 1;


vn1_JE.lo(z,eee,r) = loval;
vn2_JE.lo(z,eee,r) = loval;
vn3_JE.lo(z,eee,r) = loval;

pn1_je.lo(z,eee,r) = loval;
pn2_je.lo(z,eee,r) = loval;
pn3_je.lo(z,eee,r) = loval;

pcol_je.lo(z,eee,r) = loval;
vcol_je.lo(z,eee,r) = loval;


*Begin regression framework
Equations
FACDEMEQNC_JE,
FACDEMEQCO_JE,
ERROREQ_JE,
ERROREQ_JE_LOG,
OBJEQ_JE;

positive VARIABLES
FACDEM_JE(Z,FE,EEE,R),
FACDEM_FE(Z,FE,EEE,R);

Variables
ERROR_JE(Z),
OBJ_JE;

parameter nc_dum,co_dum;

set gasoil(fe) /gas, oil/;
set col(fe) /col/;

nc_dum(fe)$(gasoil(fe)) = 1;
co_dum(fe)$col(fe) = 1;

*Fourth -- demand for individual inputs into the E nest (col, gas, oil, ele)
*are computed using shepherds lemma on the inputs' expenditure functions (i.e. partial zpr_ve_ / partial pa.l )

equation FACDEMEQN1_JE, FACDEMEQN2_JE, FACDEMEQN3_JE, FACDEMEQ_FE;

FACDEMEQN1_JE(z,fe,eee,r)$(ZFEAS(Z)$nest1(fe)$rf(r)$eeef(eee))..
    facdem_JE(z,fe,eee,r) =e=
(
  (
    (theta_ve_je(fe,eee,r) * vafm(fe,eee,r)*(1+rtfa(fe,eee,r)) ) / (vafm(fe,eee,r)*(1+rtfa(fe,eee,r)) - pcarb0(r)*refco2(fe,eee,r))
  )
 *(
        (
                vafm(fe,eee,r)*(1+rtfa(fe,eee,r)) * PA.L(fe,r) / (vafm(fe,eee,r)*(1+rtfa(fe,eee,r)) - pcarb0(r)*refco2(fe,eee,r))
                + refco2(fe,eee,r)*pcarb_.l(z,r) / ((vafm(fe,eee,r)*(1+rtfa(fe,eee,r)) - pcarb0(r)*refco2(fe,eee,r)))
         )**(-esub_n1_je(eee,r))
  )
 *(
   (
      sum(i$nest1(i),theta_ve_je(i,eee,r)*(
              ((vafm(i,eee,r)*(1+rtfa(i,eee,r))) / (vafm(i,eee,r)*(1+rtfa(i,eee,r)) - pcarb0(r)*refco2(i,eee,r)))
              * (pa.l(i,r)$(NOT ELE(I)) + PA.L(I,R)$(ELE(I) AND (SWPRecurse = 0))+P_JE(z,"ele","usa")$(ELE(I) AND (SWPRecurse = 1)))
              + refco2(i,eee,r) * pcarb0(r) / ((vafm(i,eee,r)*(1+rtfa(i,eee,r)))-refco2(i,eee,r) * pcarb0(r)) * pcarb_.l(z,r)/pcarb0(r)
                    )**(1-esub_n1_JE(eee,r))))
                  + (1-sum(i$nest1(i),theta_ve_je(i,eee,r))) * pn2_JE(z,eee,r) ** (1-esub_n1_JE(eee,r))
    )**((1/(1-esub_n1_je(eee,r)))-1)
)/ theta_ve_je(fe,eee,r)
;


FACDEMEQN2_JE(z,fe,eee,r)$(ZFEAS(Z)$nest2(fe)$rf(r)$eeef(eee))..
    facdem_JE(z,fe,eee,r) =e=
(
  (
    (theta_ve_je(fe,eee,r) * vafm(fe,eee,r)*(1+rtfa(fe,eee,r)) ) / (vafm(fe,eee,r)*(1+rtfa(fe,eee,r)) - pcarb0(r)*refco2(fe,eee,r))
  )
 *(
        (
                vafm(fe,eee,r)*(1+rtfa(fe,eee,r)) * PA.L(fe,r) / (vafm(fe,eee,r)*(1+rtfa(fe,eee,r)) - pcarb0(r)*refco2(fe,eee,r))
                + refco2(fe,eee,r)*pcarb_.l(z,r) / ((vafm(fe,eee,r)*(1+rtfa(fe,eee,r)) - pcarb0(r)*refco2(fe,eee,r)))
         )**(-esub_n2_je(eee,r))
  )
 *(
   (
      sum(i$nest2(i),theta_ve_je(i,eee,r)*(
              ((vafm(i,eee,r)*(1+rtfa(i,eee,r))) / (vafm(i,eee,r)*(1+rtfa(i,eee,r)) - pcarb0(r)*refco2(i,eee,r)))
              * (pa.l(i,r)$(NOT ELE(I)) + PA.L(I,R)$(ELE(I) AND (SWPRecurse = 0))+P_JE(z,"ele","usa")$(ELE(I) AND (SWPRecurse = 1)))
              + refco2(i,eee,r) * pcarb0(r) / ((vafm(i,eee,r)*(1+rtfa(i,eee,r)))-refco2(i,eee,r) * pcarb0(r)) * pcarb_.l(z,r)/pcarb0(r)
                    )**(1-esub_n2_JE(eee,r))))
                  + (1-sum(i$nest2(i),theta_ve_je(i,eee,r))) * pn3_JE(z,eee,r) ** (1-esub_n2_JE(eee,r))
    )**((1/(1-esub_n2_je(eee,r)))-1)
)/ theta_ve_je(fe,eee,r)
;


FACDEMEQN3_JE(z,fe,eee,r)$(ZFEAS(Z)$nest3(fe)$rf(r)$eeef(eee))..
    facdem_JE(z,fe,eee,r) =e=
(
  (
    (theta_ve_je(fe,eee,r) * vafm(fe,eee,r)*(1+rtfa(fe,eee,r)) ) / (vafm(fe,eee,r)*(1+rtfa(fe,eee,r)) - pcarb0(r)*refco2(fe,eee,r))
  )
 *(
        (
                vafm(fe,eee,r)*(1+rtfa(fe,eee,r)) * PA.L(fe,r) / (vafm(fe,eee,r)*(1+rtfa(fe,eee,r)) - pcarb0(r)*refco2(fe,eee,r))
                + refco2(fe,eee,r)*pcarb_.l(z,r) / ((vafm(fe,eee,r)*(1+rtfa(fe,eee,r)) - pcarb0(r)*refco2(fe,eee,r)))
         )**(-esub_n3_je(eee,r))
  )
 *(
   (
      sum(i$nest3(i),theta_ve_je(i,eee,r)*(
              ((vafm(i,eee,r)*(1+rtfa(i,eee,r))) / (vafm(i,eee,r)*(1+rtfa(i,eee,r)) - pcarb0(r)*refco2(i,eee,r)))
              * (pa.l(i,r)$(NOT ELE(I)) + PA.L(I,R)$(ELE(I) AND (SWPRecurse = 0))+P_JE(z,"ele","usa")$(ELE(I) AND (SWPRecurse = 1)))
              + refco2(i,eee,r) * pcarb0(r) / ((vafm(i,eee,r)*(1+rtfa(i,eee,r)))-refco2(i,eee,r) * pcarb0(r)) * pcarb_.l(z,r)/pcarb0(r)
                    )**(1-esub_n3_JE(eee,r))))
    )**((1/(1-esub_n3_je(eee,r)))-1)
)/ theta_ve_je(fe,eee,r)
;



FACDEMEQ_FE(z,fe,eee,r)$(ZFEAS(Z)$rf(r)$eeef(eee))..
    facdem_FE(z,fe,eee,r) =e=
(
  (
    (theta_fe_je(fe,eee,r) * vafm(fe,eee,r)*(1+rtfa(fe,eee,r)) ) / (vafm(fe,eee,r)*(1+rtfa(fe,eee,r)) - pcarb0(r)*refco2(fe,eee,r))
  )
 *(
        (
                vafm(fe,eee,r)*(1+rtfa(fe,eee,r)) * PA.L(fe,r) / (vafm(fe,eee,r)*(1+rtfa(fe,eee,r)) - pcarb0(r)*refco2(fe,eee,r))
                + refco2(fe,eee,r)*pcarb_.l(z,r) / ((vafm(fe,eee,r)*(1+rtfa(fe,eee,r)) - pcarb0(r)*refco2(fe,eee,r)))
         )**(-esub_fe_je(eee,r))
  )
 *(
                        (sum(i$fe(i),theta_fe_je(i,eee,r)*(
                                                    ((vafm(i,eee,r)*(1+rtfa(i,eee,r))) / (vafm(i,eee,r)*(1+rtfa(i,eee,r)) - pcarb0(r)*refco2(i,eee,r))) * pa.l(i,r)
                                                    + refco2(i,eee,r) * pcarb0(r) / ((vafm(i,eee,r)*(1+rtfa(i,eee,r)))-refco2(i,eee,r) * pcarb0(r)) * pcarb_.l(z,r)/pcarb0(r)
                                                            )**(1-esub_fe_JE(eee,r))) )
    )**((1/(1-esub_fe_je(eee,r)))-1)
)/ theta_fe_je(fe,eee,r)
;

display theta_fe_je;

*Fifth -- error is computed as the difference between the endogenous level of
*electricity carbon activity (i.e. sum over all factor demands multiplied by their
*reference carbon levels) less the reference level of carbon from the MACC curve

equation erroreq_fe(z), objeq_fe;

variable error_fe(z);

*scalar mu /-0.5/;
ERROREQ_JE(Z)$ZFEAS(Z)..     ERROR_JE(Z) =e= (p_JE(z,"ele","usa")**mu)*SUM(FE,REFCO2(FE,"ELE","USA")*FACDEM_JE(Z,FE,"ELE","USA")) - MACC(Z,"Q");
ERROREQ_FE(Z)$ZFEAS(Z)..     ERROR_FE(Z) =e= (p_JE(z,"ele","usa")**mu)*SUM(FE,REFCO2(FE,"ELE","USA")*FACDEM_FE(Z,FE,"ELE","USA")) - MACC(Z,"Q");
ERROREQ_JE_LOG(Z)$ZFEAS(Z).. ERROR_JE(Z) =e= log(abs((p_JE(z,"ele","usa")**mu)*SUM(FE,REFCO2(FE,"ELE","USA")*FACDEM_JE(Z,FE,"ELE","USA")) - MACC(Z,"Q")));

*ERROREQ(Z).. ERROR(Z) =e= SUM(FE,REFCO2(FE,"ELE","USA")*FACDEM(Z,FE,"ELE","USA")) - MACC(Z,"Q");

variable obj_fe;

objeq_fe.. obj_fe =e= SUM(Z$ZFEAS(Z),power(error_FE(z),2)) + SwExtend * sum((z,fe)$zfeas(z),power(REFCO2(FE,"ELE","USA")*FACDEM_FE(Z,FE,"ELE","USA") - emit0(fe,"ele","usa"),2));
*==
*Sixth -- the objective is to minimize the squared errors
OBJEQ_JE.. OBJ_JE =e= SUM(Z$ZFEAS(Z),power(error_JE(z),2)) + SwExtend * sum((z,fe)$zfeas(z),power(REFCO2(FE,"ELE","USA")*FACDEM_JE(Z,FE,"ELE","USA") - emit0(fe,"ele","usa"),2));

model checkje
    /
        zpr_Y_JE.y_JE
        mkt_Y_JE.p_JE

        zpr_kle_JE.vkle_JE
        mkt_kle_JE.pkle_JE

        zpr_kl_JE.va_JE,
        mkt_PVA_JE.PVA_JE,

        zpr_vm_JE.vm_JE,
        mkt_PVM_JE.PVM_JE,

        zpr_n1_JE.ve_JE,
        mkt_n1_JE.pve_JE,

        zpr_n2_JE.vn2_JE,
        mkt_n2_JE.pn2_JE,

        zpr_n3_JE.vn3_JE,
        mkt_n3_JE.pn3_JE,
    /;


display theta_ve_je, theta_ve0_je;

*checkje.iterlim = 0;

*solve checkje using mcp

*$exit

model checkfe
/
        zpr_Y_JE.y_JE
        mkt_Y_JE.p_JE

        zpr_kle_JE.vkle_JE
        mkt_kle_JE.pkle_JE

        zpr_kl_JE.va_JE,
        mkt_PVA_JE.PVA_JE,

        zpr_ve_fe.ve_JE,
        mkt_n1_JE.PVE_JE,

        zpr_vm_JE.vm_JE,
        mkt_PVM_JE.PVM_JE,

        zpr_fe_JE.vcol_JE,
        mkt_fe_JE.Pcol_JE

/;

checkfe.iterlim = 0;

*solve checkfe using mcp;

model ssempec_JE
 /
        zpr_Y_JE.y_JE
        mkt_Y_JE.p_JE

        zpr_kle_JE.vkle_JE
        mkt_kle_JE.pkle_JE

        zpr_kl_JE.va_JE,
        mkt_PVA_JE.PVA_JE,

        zpr_vm_JE.vm_JE,
        mkt_PVM_JE.PVM_JE,

        zpr_n1_JE.ve_JE,
        mkt_n1_JE.pve_JE,

        zpr_n2_JE.vn2_JE,
        mkt_n2_JE.pn2_JE,

        zpr_n3_JE.vn3_JE,
        mkt_n3_JE.pn3_JE,

        FACDEMEQN1_JE,
        FACDEMEQN2_JE,
        FACDEMEQN3_JE,
        ERROREQ_JE,
        OBJEQ_JE



/;

ssempec_JE.optfile=1;


model ssempec_JEFE
 /
        zpr_Y_JE.y_JE
        mkt_Y_JE.p_JE

        zpr_kle_JE.vkle_JE
        mkt_kle_JE.pkle_JE

        zpr_kl_JE.va_JE,
        mkt_PVA_JE.PVA_JE,

        zpr_ve_fe.ve_JE,
        mkt_n1_JE.PVE_JE,

        zpr_vm_JE.vm_JE,
        mkt_PVM_JE.PVM_JE,

        zpr_fe_JE.vcol_JE,
        mkt_fe_JE.Pcol_JE

        FACDEMEQ_FE
        ERROREQ_FE
        OBJEQ_FE


/;

ssempec_JEFE.optfile=1;

