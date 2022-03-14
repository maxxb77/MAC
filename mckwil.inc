Parameter
        theta_k_mw(eee,r)     "value share of k in top nest of MW model",
        theta_l_mw(eee,r)     "value share of l in top nest of MW model",
        theta_ve_mw(i,eee,r)  "value share of i in E nest of ele",
        theta_vm_mw(i,eee,r)  "value share of i in M node of ELE",
        alpha_ve_mw(i,eee,r)  "alpha value for MW model",
        zprfcheck_mw          "Zero profit condition check for MW model",
        e0_mw                 "refernece E nest value",
        theta_pvm_mw          "value share of m in top nest of MW model",
        theta_pe_mw           "value share of E in top nest of MW model",
        refp_mw               "reference unit cost net of output tax",
*note theta co2 is a very small value
        theta_co2_mw(i,eee,r) "value share of CO2 in e nest of MW model";

theta_co2_mw(fe,eee,r) = pcarb0(r)*refco2(fe,eee,r) / (vafm(fe,eee,r)*(1+rtfa0(fe,eee,r)) + pcarb0(r)*refco2(fe,eee,r));

theta_co2_mw("ele",eee,r) = 0;

theta_k_mw(eee,r) =   vkm(eee,r)*(1+rtf0("cap",eee,r)) /
                   ((sum(i$(not e(i)),vafm(i,eee,r)*(1+rtfa0(i,eee,r)))) +
                   ((vlm(eee,r)*(1+rtf0("lab",eee,r)) + vkm(eee,r)*(1+rtf0("cap",eee,r)))
                  +(sum(i$e(i),(1-theta_co2_mw(i,eee,r))*vafm(i,eee,r)*(1+rtfa0(i,eee,r)))+sum(fe,theta_co2_mw(fe,eee,r)*refco2(fe,eee,r)*pcarb0(r))))
                  );


theta_l_mw(eee,r) =    vlm(eee,r)*(1+rtf0("lab",eee,r)) /
                   ((sum(i$(not e(i)),vafm(i,eee,r)*(1+rtfa0(i,eee,r)))) +
                   ((vlm(eee,r)*(1+rtf0("lab",eee,r)) + vkm(eee,r)*(1+rtf0("cap",eee,r)))
                  +(sum(i$e(i),(1-theta_co2_mw(i,eee,r))*vafm(i,eee,r)*(1+rtfa0(i,eee,r)))+sum(fe,theta_co2_mw(fe,eee,r)*refco2(fe,eee,r)*pcarb0(r))))
                  );

theta_pvm_mw(eee,r) = (sum(i$(not e(i)),vafm(i,eee,r)*(1+rtfa0(i,eee,r))))  /
                  ((sum(i$(not e(i)),vafm(i,eee,r)*(1+rtfa0(i,eee,r)))) +
                   ((vlm(eee,r)*(1+rtf0("lab",eee,r)) + vkm(eee,r)*(1+rtf0("cap",eee,r)))
                  +(sum(i$e(i),(1-theta_co2_mw(i,eee,r))*vafm(i,eee,r)*(1+rtfa0(i,eee,r)))+sum(fe,theta_co2_mw(fe,eee,r)*refco2(fe,eee,r)*pcarb0(r))))
                  );


theta_pe_mw(eee,r)   = (sum(i$e(i),(1-theta_co2_mw(i,eee,r))*vafm(i,eee,r)*(1+rtfa0(i,eee,r)))+sum(fe,theta_co2_mw(fe,eee,r)*refco2(fe,eee,r)*pcarb0(r)))/
                  ((sum(i$(not e(i)),vafm(i,eee,r)*(1+rtfa0(i,eee,r)))) +
                   ((vlm(eee,r)*(1+rtf0("lab",eee,r)) + vkm(eee,r)*(1+rtf0("cap",eee,r)))
                  +(sum(i$e(i),(1-theta_co2_mw(i,eee,r))*vafm(i,eee,r)*(1+rtfa0(i,eee,r)))+sum(fe,theta_co2_mw(fe,eee,r)*refco2(fe,eee,r)*pcarb0(r))))
                  );

theta_ve_mw(i,eee,r)$e(i) =  (vafm(i,eee,r)*(1+rtfa0(i,eee,r)) + pcarb0(r)*refco2(i,eee,r)$fe(i)) / (sum(ii$e(ii),vafm(ii,eee,r)*(1+rtfa0(ii,eee,r)))+sum(fe,refco2(fe,eee,r)*pcarb0(r)));

theta_vm_mw(i,eee,r)$(not e(i)) = (vafm(i,eee,r)*(1+rtfa0(i,eee,r))) / (sum(ii$(not e(ii)),vafm(ii,eee,r)*(1+rtfa0(ii,eee,r))));

alpha_ve_mw(i,eee,r)$e(i) = ((vafm(i,eee,r)*(1+rtfa(i,eee,r))) / (vafm(i,eee,r)*(1+rtfa(i,eee,r)) - pcarb0(r)*refco2(i,eee,r))) *pa.l(i,r)
                        + refco2(i,eee,r) * pcarb0(r) / ((vafm(i,eee,r)*(1+rtfa(i,eee,r))) - refco2(i,eee,r) * pcarb0(r)) * pcarb.l(r)/pcarb0(r);

zprfcheck_mw("1",eee,r) = pve.l(eee,r) - sum(i$e(i),theta_ve_mw(i,eee,r)*alpha_ve_mw(i,eee,r));

refp_mw(eee,r) = (1-rto(eee,r));

rto1_mw(eee,r) = 1-rto(eee,r);

e0_mw(eee,r) =        ((sum(i$(not e(i)),vafm(i,eee,r)*(1+rtfa0(i,eee,r)))) +
                   ((vlm(eee,r)*(1+rtf0("lab",eee,r)) + vkm(eee,r)*(1+rtf0("cap",eee,r)))
                  +(sum(i$e(i),(1-theta_co2_mw(i,eee,r))*vafm(i,eee,r)*(1+rtfa0(i,eee,r)))+sum(fe,theta_co2_mw(fe,eee,r)*refco2(fe,eee,r)*pcarb0(r))))
                  );

*Display theta_k_mw, theta_l_mw, theta_ve_mw, theta_vm_mw, 
*alpha_ve_mw, zprfcheck_mw, e0_mw, theta_pvm_mw, theta_pe_mw, 
*refp_mw, rto1_mw, theta_co2_mw;

Positive Variables
*prices
PVM_MW(z,eee,r) "price of M nest in MW model"
PVE_MW(z,eee,r) "price of E nest in MW model"
P_MW(z,eee,r)   "price of electricity in MW model"
PCARB_(Z,R)     "price of carbon"

*and activity levels
VE_MW(z,eee,r)  "VE in MW  model"
VM_MW(z,eee,r)  "VM in MW  model"
Y_MW(z,eee,r)   "Y in MW  model"

*other variables
FACDEM_MW(Z,FE,EEE,R) "factor demand for each fossil fuel in MW model";

*elasticities as variables (not indexed by z)
ESUB_KLE_M_MW(eee,r) "elasticity of substitution for KLE_M in MW model"
esub_m_MW(eee,r) "elasticity of substitution for m in MW model"
esub_e_MW(eee,r) "elasticity of substitution for e in MW model"
;

Variables
ERROR_MW(Z),
OBJ_MW;


Equations
FACDEMEQ_MW      "factor demand in MW model",
ERROREQ_MW       "error equation in MW model",
ERROREQ_MW_LOG   "experimental log transformation of error in MW model",
OBJEQ_MW         "MW objection function";


        zpr_Y_MW(z,eee,r)    zero profits for Y
        mkt_Y_MW(z,eee,r)    market for Y

        zpr_ve_MW(z,eee,r)   zero profits at the VE node of kle in ele
        mkt_PVE_MW(z,eee,r)  market for PVA

        zpr_vm_MW(z,eee,r)   zero profits at the M node of ele
        mkt_PVM_MW(z,eee,r)  market for PVA
;


*Third -- the model is the same as before except now the necessary variables are indexed by z
*=========================
*Market for Top Nest
*=========================

zpr_Y_MW(z,eee,r)$ZFEAS(Z)..
           refp_mw(EEE,R)*(
                 (theta_pvm_mw(eee,r) *   pvm_mw(z,eee,r) **(1-esub_kle_m_MW(eee,r)))
                +(theta_pe_mw(eee,r)  *   pve_mw(z,eee,r) **(1-esub_kle_m_MW(eee,r)))
                +(theta_k_mw(eee,r)   *  (RK.L(eee,r)*(1+rtf("cap",eee,r))/(1+rtf0("cap",eee,r)))**(1-esub_kle_m_MW(eee,r)))
                +(theta_l_mw(eee,r)   *  (PL.L(r)    *(1+rtf("lab",eee,r))/(1+rtf0("lab",eee,r)))**(1-esub_kle_m_MW(eee,r)))
                        )**(1/(1-esub_kle_m_MW(eee,r)))
           - P_MW(Z,EEE,r)*(1-RTO(EEE,R))

             =g= 0;

*other representations presented here
*mkt_Y_MW(z,eee,r)$ZFEAS(Z).. Y_MW(Z,EEE,R)*vom(eee,r) - SY.L(eee,r) =g=0;
*mkt_Y_MW(z,eee,r)$ZFEAS(Z).. Y_MW(Z,EEE,R)*vom(eee,r) - p_MW(z,"ele","usa")**mu =g=0;
mkt_Y_MW(z,eee,r)$ZFEAS(Z).. Y_MW(Z,EEE,R)*vom(eee,r) - SY.L(eee,r)*p_MW(z,"ele","usa")**mu=g=0;


*=================
*Market for M nest (second)
*=================

zpr_vm_MW(z,eee,r)$ZFEAS(Z).. (sum(i$(not e(i)), theta_vm_mw(i,eee,r) * (PA.L(I,R) * (1+RTFA(I,EEE,R)) / (1+RTFA0(I,EEE,R)) ) ** (1-ESUB_m_MW(EEE,r))))**(1/(1-esub_m_MW(eee,r)))
                 - pvm_mw(z,eee,R) =G= 0;

mkt_PVM_MW(z,eee,r)$ZFEAS(Z)..
*        VM_MW(Z,EEE,r)*(sum(i$(not e(i)),vafm(i,eee,r)*(1+rtfa0(i,eee,r))))
         VM_MW(Z,EEE,r)*theta_pvm_mw(eee,r)*vom(eee,r)*refp_mw(eee,r)
        - Y_MW(Z,EEE,r)*theta_pvm_mw(eee,r)*vom(eee,r)*refp_mw(eee,r)*(P_MW(Z,EEE,r)/pvm_mw(z,eee,r))**esub_kle_m_MW(eee,r) =g= 0;

*=================
*Market for E nest
*=================

zpr_ve_MW(z,eee,r)$ZFEAS(Z)..
         sum(i$e(i),theta_ve_mw(i,eee,r)*((
                    ((vafm(i,eee,r)*(1+rtfa(i,eee,r))) / (vafm(i,eee,r)*(1+rtfa(i,eee,r)) - pcarb0(r)*refco2(i,eee,r))) *
                        (pa.l(i,r)$(not ele(i))+pa.l(i,r)$(ele(i)$(SwPRecurse = 0)) + P_MW(Z,"ele","USA")$(ele(i)$(SwPRecurse = 1)))
                        + refco2(i,eee,r) * pcarb0(r) / ((vafm(i,eee,r)*(1+rtfa(i,eee,r))) - refco2(i,eee,r) * pcarb0(r)) * pcarb_.l(z,r)/pcarb0(r)
                        )**(1-esub_e_MW(eee,r))))**(1/(1-esub_e_MW(eee,r))) - pve_mw(z,eee,r)
         =G= 0;

mkt_PVE_MW(z,eee,r)$ZFEAS(Z)..
        VE_MW(Z,EEE,r)*theta_pe_mw(eee,r)*e0_mw(eee,r) -
         Y_MW(Z,EEE,r)*theta_pe_mw(eee,r)*e0_mw(eee,r)*(P_MW(Z,EEE,r)/pve_mw(z,eee,r))**esub_kle_m_MW(eee,r) =g= 0;


ESUB_KLE_M_MW.fx(eee,r) = ESUB_KLE_M(eee);
esub_m_MW.fx(eee,r) = esub_m(eee);
esub_e_MW.fx(eee,r) = esub_e(eee);

*level values are those from the previous solve
PVM_MW.l(z,eee,r) =  PVM.l(eee,r);
PVE_MW.l(z,eee,r) =  PVE.l(eee,r);
P_MW.l(z,eee,r) =  P.l(eee,r);
PCARB_.l(Z,R) =  PCARB.l(R);
VE_MW.l(z,eee,r) =  VE.l(eee,r);
VM_MW.l(z,eee,r) =  VM.l(eee,r);
Y_MW.l(z,eee,r) =  Y.l(eee,r);



*Fourth -- demand for individual inputs into the E nest (col, gas, oil, ele)
*are computed using shepherds lemma on the inputs' expenditure functions (i.e. partial zpr_ve_ / partial pa.l )
FACDEMEQ_MW(z,fe,eee,r)$ZFEAS(Z).. facdem_MW(z,fe,eee,r) =e=
(
  (
    (theta_ve_mw(fe,eee,r) * vafm(fe,eee,r)*(1+rtfa(fe,eee,r)) ) / (vafm(fe,eee,r)*(1+rtfa(fe,eee,r)) - pcarb0(r)*refco2(fe,eee,r))
  )
 *(
        (
                vafm(fe,eee,r)*(1+rtfa(fe,eee,r)) * PA.L(fe,r) / (vafm(fe,eee,r)*(1+rtfa(fe,eee,r)) - pcarb0(r)*refco2(fe,eee,r))
                + refco2(fe,eee,r)*pcarb_.l(z,r) / ((vafm(fe,eee,r)*(1+rtfa(fe,eee,r)) - pcarb0(r)*refco2(fe,eee,r)))
         )**(-esub_e_MW(eee,r))
  )
 *(
   (sum(i$e(i),theta_ve_mw(i,eee,r)*(
                        ((vafm(i,eee,r)*(1+rtfa(i,eee,r))) / (vafm(i,eee,r)*(1+rtfa(i,eee,r)) - pcarb0(r)*refco2(i,eee,r))) *(pa.l(i,r)$(not ele(i))+pa.l(i,r)$(ele(i)$(SwPRecurse = 0)) + P_MW(Z,"ele","USA")$(ele(i)$(SwPRecurse = 1)))
                        + refco2(i,eee,r) * pcarb0(r) / ((vafm(i,eee,r)*(1+rtfa(i,eee,r)))-refco2(i,eee,r) * pcarb0(r)) * pcarb_.l(z,r)/pcarb0(r)
                                  )**(1-esub_e_MW(eee,r)))
    )**((1/(1-esub_e_MW(eee,r)))-1)
))/ theta_ve_mw(fe,eee,r)
;

*Fifth -- error is computed as the difference between the endogenous level of
*electricity carbon activity (i.e. sum over all factor demands multiplied by their
*reference carbon levels) less the reference level of carbon from the MACC curve


ERROREQ_MW(Z)$ZFEAS(Z).. ERROR_MW(Z) =e= 
  (p_MW(z,"ele","usa")**mu)*SUM(FE,REFCO2(FE,"ELE","USA")*FACDEM_MW(Z,FE,"ELE","USA")) - MACC(Z,"Q");

ERROREQ_MW_LOG(Z)$ZFEAS(Z).. ERROR_MW(Z) =e= 
  LOG(abs(((p_MW(z,"ele","usa")**mu)*SUM(FE,REFCO2(FE,"ELE","USA")*FACDEM_MW(Z,FE,"ELE","USA")) - MACC(Z,"Q"))));


*ERROREQ(Z).. ERROR(Z) =e= SUM(FE,REFCO2(FE,"ELE","USA")*FACDEM(Z,FE,"ELE","USA")) - MACC(Z,"Q");

*Sixth -- the objective is to minimize the squared errors
OBJEQ_MW.. OBJ_MW =e= SUM(Z$ZFEAS(Z),power(error_MW(z),2)) 
  + SwExtend * sum((z,fe)$zfeas(z),power(REFCO2(FE,"ELE","USA")*FACDEM_MW(Z,FE,"ELE","USA") - emit0(fe,"ele","usa"),2));


model ssempec_mw
 /
        zpr_Y_MW.y_MW
        mkt_Y_MW.p_MW

        zpr_ve_MW.ve_MW,
        mkt_PVE_MW.PVE_MW,

        zpr_vm_MW.vm_MW,
        mkt_PVM_MW.PVM_MW


        FACDEMEQ_MW
        ERROREQ_MW
        OBJEQ_MW

/;


model ssempec_mw_log
 /
         zpr_Y_MW.y_MW
        mkt_Y_MW.p_MW

        zpr_ve_MW.ve_MW,
        mkt_PVE_MW.PVE_MW,

        zpr_vm_MW.vm_MW,
        mkt_PVM_MW.PVM_MW


        FACDEMEQ_MW
        ERROREQ_MW_LOG
        OBJEQ_MW
/;


*for now, the elasticities are fixed at their respective parameter values
ESUB_KLE_M_MW.fx(eee,r) = ESUB_KLE_M(eee);
esub_m_MW.fx(eee,r) = esub_m(eee);
esub_e_MW.fx(eee,r) = esub_e(eee);

*fix elasticities of substitution at reference values
ESUB_KLE_M_MW.fx(eee,r) = 0.7634;
esub_m_MW.fx(eee,r) = 1.001;
esub_e_MW.fx(eee,r) = 0.2;

*level values are those from the previous solve
PVM_MW.l(z,eee,r) =  PVM.l(eee,r);
PVE_MW.l(z,eee,r) =  PVE.l(eee,r);
P_MW.l(z,eee,r) =  P.l(eee,r);
PCARB_.l(Z,R) =  PCARB.l(R);
VE_MW.l(z,eee,r) =  VE.l(eee,r);
VM_MW.l(z,eee,r) =  VM.l(eee,r);
Y_MW.l(z,eee,r) =  Y.l(eee,r);

ssempec_mw.optfile = 1;
ssempec_mw_log.optfile = 1;
