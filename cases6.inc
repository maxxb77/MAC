

*Scalar SwFixCO "switch to fix [1] esub_co in the JE model after it is freed up" /0/;
*this would give the JE model an 'unfair' advantage in having more variables
*it is a subtlety but should be tested both way to makes sure there is no change


*alternative to fixing demand is to set a very inelastic demand
*doesn't work as well -
if((SwYFX = 1),
    mu = -0.001;
);


pcarb_.fx(z,"usa")$ZFEAS(Z) = MACC(z,"p");

*checking for calibration of alpha parameters here:
alpha_ve_je(z,i,eee,r)$((e(i))$(not co(i))) = ((vafm(i,eee,r)*(1+rtfa(i,eee,r))) / (vafm(i,eee,r)*(1+rtfa(i,eee,r)) - pcarb0(r)*refco2(i,eee,r))) * pa.l(i,r)
                        + refco2(i,eee,r) * pcarb0(r) / ((vafm(i,eee,r)*(1+rtfa(i,eee,r)))-refco2(i,eee,r) * pcarb0(r)) * pcarb_.l(z,r)/pcarb0(r);

alpha_co_je(z,i,eee,r)$co(i) = ((vafm(i,eee,r)*(1+rtfa(i,eee,r))) / (vafm(i,eee,r)*(1+rtfa(i,eee,r)) - pcarb0(r)*refco2(i,eee,r))) * pa.l(i,r)
                        + refco2(i,eee,r) * pcarb0(r) / ((vafm(i,eee,r)*(1+rtfa(i,eee,r)))-refco2(i,eee,r) * pcarb0(r)) * pcarb_.l(z,r)/pcarb0(r);



*Define upper and lower limits for elasticities
*Scalar lolim "Lower limit for elasticity values" /1e-4/;
*Scalar uplim "Lower limit for elasticity values" /1e4/;

*Solve the models with no elasticities free
solve ssempec_de using mpec minimizing OBJ_de;
solve ssempec_je using mpec minimizing OBJ_je;
solve ssempec_mw using mpec minimizing OBJ_mw;

res_pq_DE(z,"rp") = MACC(Z,"p");
res_pq_DE(z,"rq") = MACC(Z,"Q");
res_pq_WS(z,"rp") = MACC(Z,"p");
res_pq_WS(z,"rq") = MACC(Z,"Q");
res_pq_MW(z,"rp") = MACC(Z,"p");
res_pq_MW(z,"rq") = MACC(Z,"Q");


res_pq_DE(z,"base") = (p_de.l(z,"ele","usa")**mu)*SUM(FE,REFCO2(FE,"ELE","USA")*FACDEM_de.l(Z,FE,"ELE","USA"));
res_pq_WS(z,"base") = (p_je.l(z,"ele","usa")**mu)*SUM(FE,REFCO2(FE,"ELE","USA")*FACDEM_je.l(Z,FE,"ELE","USA"));
res_pq_MW(z,"base") = (p_mw.l(z,"ele","usa")**mu)*SUM(FE,REFCO2(FE,"ELE","USA")*FACDEM_mw.l(Z,FE,"ELE","USA"));

res_obj("base_DE","1") = obj_de.l;
res_obj("base_WS","1") = OBJ_je.l;
res_obj("base_MW","1") = OBJ_mw.l;



*///////////////////////////////
*//////MW Nesting Structure/////
*///////////////////////////////

res_elas_MW("REF","E") = esub_e_mw.l("ele","usa");
res_elas_MW("REF","KLEM") = esub_KLE_M_mw.l("ele","usa");

esub_e_mw.lo("ele","usa") = lolim;
esub_e_mw.up("ele","usa") = uplim;
solve ssempec_mw using mpec minimizing OBJ_MW;
res_pq_MW(z,"E") = (p_MW.l(z,"ele","usa")**mu)*sum(FE,REFCO2(FE,"ELE","USA")*FACDEM_MW.l(Z,FE,"ELE","USA"));
res_elas_MW("E","E") = esub_e_mw.l("ele","usa");
res_obj("E_MW","1") = obj_MW.l;

ESUB_KLE_M_MW.lo(eee,r) = lolim;
ESUB_KLE_M_MW.up(eee,r) = UPlim;
esub_e_mw.l("ele","usa") = 1.001;
solve ssempec_mw using mpec minimizing OBJ_MW;
res_pq_MW(z,"KLEM") = (p_MW.l(z,"ele","usa")**mu)*sum(FE,REFCO2(FE,"ELE","USA")*FACDEM_MW.l(Z,FE,"ELE","USA"));
res_elas_MW("KLEM","E") = esub_e_mw.l("ele","usa");
res_elas_MW("KLEM","KLEM") = esub_KLE_M_mw.l("ele","usa");
res_obj("KLEM_MW","1") = obj_MW.l;







*///////////////////////////////
*//////DE Nesting Structure/////
*///////////////////////////////

*ESUB_KLE_M_de.fx(eee,r) = 1e-4;
*esub_m_de.fx(eee,r) = 0.391;
*esub_k_l_de.fx(eee,r) = 0.460;
*esub_kl_e_de.fx(eee,r) = 0.256;
*esub_e_de.fx(eee,r) = 0.500;

*ESUB_KLE_M_de.l(eee,r) = 1e-4;
*esub_m_de.l(eee,r) = 0.391;
*esub_k_l_de.l(eee,r) = 0.460;
*esub_kl_e_de.l(eee,r) = 0.256;
*esub_e_de.l(eee,r) = 0.500;


res_elas_DE("REF","E") = esub_E_DE.l("ele","usa");
res_elas_DE("REF","KLE") = esub_KL_E_DE.l("ele","usa");
res_elas_DE("REF","KLEM") = esub_KLE_M_DE.l("ele","usa");


esub_e_de.lo("ele","usa") = lolim;
esub_e_de.up("ele","usa") = uplim;
solve ssempec_de using mpec minimizing OBJ_de;

res_pq_DE(z,"E_DE") = (p_de.l(z,"ele","usa")**mu)*sum(FE,REFCO2(FE,"ELE","USA")*FACDEM_de.l(Z,FE,"ELE","USA"));
res_elas_DE("E","E") = esub_e_DE.l("ele","usa");
res_obj("E_DE","1") = obj_de.l;

*ESUB_KLE_M_de.l(eee,r) = 1e-4;
*esub_m_de.l(eee,r) = 0.391;
*esub_k_l_de.l(eee,r) = 0.460;
*esub_kl_e_de.l(eee,r) = 0.256;
*esub_e_de.l(eee,r) = 0.500;


*esub_kle_m_de.fx("ele","usa") = 1e-4;
esub_kl_e_de.lo("ele","usa") = lolim;
esub_kl_e_de.up("ele","usa") = uplim;
*esub_e_de.l("ele","usa") = 0.5;
esub_e_de.l("ele","usa") = 0.1;
esub_kl_e_de.l("ele","usa") = 0.1;
solve ssempec_de using mpec minimizing OBJ_de;

res_pq_DE(z,"KLE_DE") = (p_de.l(z,"ele","usa")**mu)*sum(FE,REFCO2(FE,"ELE","USA")*FACDEM_de.l(Z,FE,"ELE","USA"));
res_elas_DE("KLE","E") = esub_E_DE.l("ele","usa");
res_elas_DE("KLE","KLE") = esub_KL_E_DE.l("ele","usa");
res_obj("KLE_DE","1") = obj_de.l;


esub_kle_M_de.lo("ele","usa") = lolim;
esub_kle_M_de.up("ele","usa") = uplim;
esub_e_de.l("ele","usa") = 0.0;
esub_kl_e_de.l("ele","usa") = 0.0;
solve ssempec_de using mpec minimizing OBJ_de;

res_pq_DE(z,"KLEM_DE") = (p_de.l(z,"ele","usa")**mu)*sum(FE,REFCO2(FE,"ELE","USA")*FACDEM_de.l(Z,FE,"ELE","USA"));
res_elas_DE("KLEM","E") = esub_E_DE.l("ele","usa");
res_elas_DE("KLEM","KLE") = esub_KL_E_DE.l("ele","usa");
res_elas_DE("KLEM","KLEM") = esub_KLE_M_DE.l("ele","usa");
res_obj("KLEM_DE","1") = obj_de.l;



*///////////////////////////////
*//////JE Nesting Structure/////
*///////////////////////////////


esub_n1_JE.fx(eee,r) = 0.25;
esub_n2_JE.fx(eee,r) = 0.7;
esub_n3_JE.fx(eee,r) = 0.5;

*res_elas_WS("REF","KLE") = esub_KLE_JE.l("ele","usa");
res_elas_WS("REF","n3") = esub_n3_JE.l("ele","usa");
res_elas_WS("REF","n2") = esub_n2_JE.l("ele","usa");
res_elas_WS("REF","n1") = esub_n1_JE.l("ele","usa");


*free only the gasoil elasticity
esub_n3_JE.lo("ele","usa")  = lolim;
esub_n3_JE.up("ele","usa")  = uplim;
solve ssempec_je using mpec minimizing OBJ_je;

res_pq_WS(z,"n3") = (p_je.l(z,"ele","usa")**mu)*SUM(FE,REFCO2(FE,"ELE","USA")*FACDEM_je.l(Z,FE,"ELE","USA"));
res_elas_WS("n3","n3") = esub_n3_JE.l("ele","usa");
res_obj("n3_ws","1") = OBJ_je.l;

esub_n3_JE.fx("ele","usa")  = res_elas_WS("REF","n3");
esub_n1_JE.lo("ele","usa")  = lolim;
esub_n1_JE.up("ele","usa")  = uplim;
*esub_n1_je.l("ele","usa") = 0.01;
solve ssempec_je using mpec minimizing OBJ_je;

res_pq_WS(z,"n1") = (p_je.l(z,"ele","usa")**mu)*SUM(FE,REFCO2(FE,"ELE","USA")*FACDEM_je.l(Z,FE,"ELE","USA"));
res_elas_WS("n1","n1") = esub_n1_JE.l("ele","usa");
res_obj("n1_ws","1") = OBJ_je.l;

*$exit

*free only the col substitution elasticity
esub_n1_je.fx("ele","usa") = res_elas_WS("REF","n1");
esub_n3_JE.fx("ele","usa") = 0.5;
esub_n2_je.lo("ele","usa") = lolim;
esub_n2_je.up("ele","usa") = uplim;
solve ssempec_je using mpec minimizing OBJ_je;

res_pq_WS(z,"n2") = (p_je.l(z,"ele","usa")**mu)*SUM(FE,REFCO2(FE,"ELE","USA")*FACDEM_je.l(Z,FE,"ELE","USA"));
res_elas_WS("on2","n2") = esub_n2_JE.l("ele","usa");
res_obj("n2_WS","1") = OBJ_je.l;

*free up both col and gasoil
esub_n1_je.fx("ele","usa") = res_elas_WS("REF","n1");
esub_n3_JE.lo("ele","usa")  = lolim;
esub_n3_JE.up("ele","usa")  = uplim;
esub_n2_je.l("ele","usa") = 0.7;
solve ssempec_je using mpec minimizing OBJ_je;

res_pq_WS(z,"n23") = (p_je.l(z,"ele","usa")**mu)*SUM(FE,REFCO2(FE,"ELE","USA")*FACDEM_je.l(Z,FE,"ELE","USA"));
res_elas_WS("n23","n2") = esub_n2_JE.l("ele","usa");
res_elas_WS("n23","n3") = esub_n3_JE.l("ele","usa");
res_obj("n23_ws","1") = OBJ_je.l;

*free up all energy nest
esub_n1_JE.lo("ele","usa")  = lolim;
esub_n1_JE.up("ele","usa")  = uplim;
esub_n3_je.l("ele","usa") = 0.5;
esub_n2_je.l("ele","usa") = 0.7;
solve ssempec_je using mpec minimizing OBJ_je;

res_pq_WS(z,"n123") = (p_je.l(z,"ele","usa")**mu)*SUM(FE,REFCO2(FE,"ELE","USA")*FACDEM_je.l(Z,FE,"ELE","USA"));
res_elas_WS("n123","n3") = esub_n3_JE.l("ele","usa");
res_elas_WS("n123","n2") = esub_n2_JE.l("ele","usa");
res_elas_WS("n123","n1") = esub_n1_JE.l("ele","usa");
res_obj("n123","1") = OBJ_je.l;


esub_n1_JE.l("ele","usa") = 0.25;
esub_n3_je.l("ele","usa") = 0.5;
esub_n2_je.l("ele","usa") = 0.7;
esub_kl_e_je.lo("ele","usa") = lolim;
esub_kl_e_je.up("ele","usa") = uplim;
solve ssempec_je using mpec minimizing OBJ_je;

res_pq_WS(z,"KLE_WS") = (p_je.l(z,"ele","usa")**mu)*SUM(FE,REFCO2(FE,"ELE","USA")*FACDEM_je.l(Z,FE,"ELE","USA"));
res_elas("WS","KLE","n3") = esub_n3_JE.l("ele","usa");
res_elas("WS","KLE","n2") = esub_n2_JE.l("ele","usa");
res_elas("WS","KLE","n1") = esub_n1_JE.l("ele","usa");
res_elas("WS","KLE","KLE") = esub_KL_E_JE.l("ele","usa");
res_obj("KLE_WS","1") = OBJ_je.l;


ESUB_KLE_M_JE.lo("ele","usa") = lolim;
ESUB_KLE_M_JE.up("ele","usa") = uplim;
esub_n1_JE.l("ele","usa") = 0.25;
esub_n3_je.l("ele","usa") = 0.5;
esub_n2_je.l("ele","usa") = 0.7;
solve ssempec_je using mpec minimizing OBJ_je;

res_pq_WS(z,"KLEM_WS") = (p_je.l(z,"ele","usa")**mu)*SUM(FE,REFCO2(FE,"ELE","USA")*FACDEM_je.l(Z,FE,"ELE","USA"));
res_elas("WS","KLEM","n3") = esub_n3_JE.l("ele","usa");
res_elas("WS","KLEM","n2") = esub_n2_JE.l("ele","usa");
res_elas("WS","KLEM","n1") = esub_n1_JE.l("ele","usa");
res_elas("WS","KLEM","KLE") = esub_KL_E_JE.l("ele","usa");
res_elas("WS","KLEM","KLEM") = esub_KLE_M_JE.l("ele","usa");
res_obj("KLEM_WS","1") = OBJ_je.l;

ESUB_KLE_M_JE.lo("ele","usa") = lolim;
ESUB_n1_JE.lo("ele","usa") = lolim;
esub_n2_JE.lo("ele","usa") = lolim;
esub_n3_JE.lo("ele","usa") = lolim;
ESUB_KLE_M_JE.up("ele","usa") = uplim;
ESUB_n1_JE.up("ele","usa") = uplim;
esub_n2_JE.up("ele","usa") = uplim;
esub_n3_JE.up("ele","usa") = uplim;


solve ssempec_je using mpec minimizing OBJ_je;
res_pq_WS(z,"all_WS") = (p_je.l(z,"ele","usa")**mu)*SUM(FE,REFCO2(FE,"ELE","USA")*FACDEM_je.l(Z,FE,"ELE","USA"));
res_elas("WS","all","n3") = esub_n3_JE.l("ele","usa");
res_elas("WS","all","n2") = esub_n2_JE.l("ele","usa");
res_elas("WS","all","n1") = esub_n1_JE.l("ele","usa");
res_elas("WS","all","KLE") = esub_KL_E_JE.l("ele","usa");
res_elas("WS","all","KLEM") = esub_KLE_M_JE.l("ele","usa");
res_obj("ALL_WS","1") = OBJ_je.l;



$ontext
*fe nesting structure


res_elas_fe("REF","fe") = esub_fe_JE.l("ele","usa");
res_elas_fe("REF","n1") = esub_eee_JE.l("ele","usa");


*free only the col substitution elasticity
esub_fe_je.lo("ele","usa") = lolim;
esub_fe_je.up("ele","usa") = uplim;
solve ssempec_jefe using mpec minimizing OBJ_fe;

res_pq_fe(z,"fe") = (p_je.l(z,"ele","usa")**mu)*SUM(FE,REFCO2(FE,"ELE","USA")*FACDEM_fe.l(Z,FE,"ELE","USA"));
res_elas_fe("fe","fe") = esub_fe_JE.l("ele","usa");
res_obj("fe_fe","1") = OBJ_fe.l;


*free up all energy nest
esub_eee_JE.lo("ele","usa")  = lolim;
esub_eee_JE.up("ele","usa")  = uplim;
esub_fe_je.l("ele","usa") = 0.5;
solve ssempec_jefe using mpec minimizing OBJ_fe;

res_pq_fe(z,"ALLE_fe") = (p_je.l(z,"ele","usa")**mu)*SUM(FE,REFCO2(FE,"ELE","USA")*FACDEM_fe.l(Z,FE,"ELE","USA"));
res_elas_fe("ALLE","fe") = esub_fe_JE.l("ele","usa");
res_elas_fe("ALLE","n1") = esub_eee_JE.l("ele","usa");
res_obj("ALLE_fe","1") = OBJ_fe.l;


esub_kl_e_je.lo("ele","usa") = lolim;
esub_kl_e_je.up("ele","usa") = uplim;
esub_EEE_JE.l("ele","usa") = 0.25;
esub_col_je.l("ele","usa") = 0.7;
solve ssempec_jefe using mpec minimizing OBJ_fe;

res_pq_fe(z,"KLE_fe") = (p_je.l(z,"ele","usa")**mu)*SUM(FE,REFCO2(FE,"ELE","USA")*FACDEM_fe.l(Z,FE,"ELE","USA"));
res_elas("fe","KLE","fe") = esub_fe_JE.l("ele","usa");
res_elas("fe","KLE","n1") = esub_eee_JE.l("ele","usa");
res_elas("fe","KLE","KLE") = esub_KL_E_JE.l("ele","usa");
res_obj("KLE_fe","1") = OBJ_fe.l;



ESUB_KLE_M_JE.lo(eee,r) = lolim;
ESUB_KLE_M_JE.up(eee,r) = uplim;
esub_EEE_JE.l("ele","usa") = 0.25;
esub_gasoil_je.l("ele","usa") = 0.5;
esub_col_je.l("ele","usa") = 0.7;
solve ssempec_jefe using mpec minimizing OBJ_fe;

res_pq_fe(z,"KLEM_fe") = (p_je.l(z,"ele","usa")**mu)*SUM(FE,REFCO2(FE,"ELE","USA")*FACDEM_fe.l(Z,FE,"ELE","USA"));
res_elas("fe","KLEM","fe") = esub_fe_JE.l("ele","usa");
res_elas("fe","KLEM","n1") = esub_eee_JE.l("ele","usa");
res_elas("fe","KLEM","KLE") = esub_KL_E_JE.l("ele","usa");
res_elas("fe","KLEM","KLEM") = esub_KLE_M_JE.l("ele","usa");
res_obj("KLEM_fe","1") = OBJ_fe.l;


ESUB_KLE_M_JE.lo("ele","usa") = lolim;
ESUB_EEE_JE.lo("ele","usa") = lolim;
esub_col_JE.lo("ele","usa") = lolim;
esub_gasoil_JE.lo("ele","usa") = lolim;
ESUB_KLE_M_JE.up("ele","usa") = uplim;
ESUB_EEE_JE.up("ele","usa") = uplim;
esub_col_JE.up("ele","usa") = uplim;
esub_gasoil_JE.up("ele","usa") = uplim;

solve ssempec_jefe using mpec minimizing OBJ_fe;

res_pq_fe(z,"all_fe") = (p_je.l(z,"ele","usa")**mu)*SUM(FE,REFCO2(FE,"ELE","USA")*FACDEM_fe.l(Z,FE,"ELE","USA"));
res_elas("fe","all","gasoil") = esub_gasoil_JE.l("ele","usa");
res_elas("fe","all","fe") = esub_fe_JE.l("ele","usa");
res_elas("fe","all","n1") = esub_eee_JE.l("ele","usa");
res_elas("fe","all","KLE") = esub_KL_E_JE.l("ele","usa");
res_elas("fe","all","KLEM") = esub_KLE_M_JE.l("ele","usa");
res_obj("ALL_fe","1") = OBJ_fe.l;


$offtext




*res_elas_all("WS","REF","KLE") = res_elas_WS("REF","KLE");
res_elas_all("WS","REF","n3") = res_elas_WS("REF","n3");
res_elas_all("WS","REF","n2") = res_elas_WS("REF","n2");
res_elas_all("WS","REF","n1") = res_elas_WS("REF","n1");
res_elas_all("WS","n3","n3") = res_elas_WS("n3","n3");
res_elas_all("WS","n2","n2") = res_elas_WS("n2","n2");
res_elas_all("WS","n23","n2") = res_elas_WS("n23","n2");
res_elas_all("WS","n23","n3") = res_elas_WS("n23","n3");
res_elas_all("WS","n123","n3") = res_elas_WS("n123","n3");
res_elas_all("WS","n123","n2") = res_elas_WS("n123","n2");
res_elas_all("WS","n123","n1") = res_elas_WS("n123","n1");
res_elas_all("DE","REF","E") = res_elas_DE("REF","E");
res_elas_all("DE","REF","KLE") = res_elas_DE("REF","KLE");
res_elas_all("DE","REF","KLEM") = res_elas_DE("REF","KLEM");
res_elas_all("DE","E","E") = res_elas_DE("E","E");
res_elas_all("DE","KLE","E") = res_elas_DE("KLE","E");
res_elas_all("DE","KLE","KLE") = res_elas_DE("KLE","KLE");
res_elas_all("DE","KLEM","E") = res_elas_DE("KLEM","E");
res_elas_all("DE","KLEM","KLE") = res_elas_DE("KLEM","KLE");
res_elas_all("DE","KLEM","KLEM") = res_elas_DE("KLEM","KLEM");
res_elas_all("MW","REF","E") = res_elas_MW("REF","E");
res_elas_all("MW","REF","KLEM") = res_elas_MW("REF","KLEM");
res_elas_all("MW","E","E") = res_elas_MW("E","E");
res_elas_all("MW","KLEM","E") = res_elas_MW("KLEM","E");
res_elas_all("MW","KLEM","KLEM") = res_elas_MW("KLEM","KLEM");


res_elas_all("WS","all","n3") = res_elas("WS","all","n3");
res_elas_all("WS","all","n2") = res_elas("WS","all","n2");
res_elas_all("WS","all","n1") = res_elas("WS","all","n1");
res_elas_all("WS","all","KLE") = res_elas("WS","all","KLE");
res_elas_all("WS","all","KLEM") = res_elas("WS","all","KLEM");


res_obj_mod("DE","base") = res_obj("base_DE","1");
res_obj_mod("WS","base") = res_obj("base_WS","1");
res_obj_mod("MW","base") = res_obj("base_MW","1");
res_obj_mod("WS","n3") = res_obj("n3_ws","1");
res_obj_mod("WS","n2") = res_obj("n2_WS","1");
res_obj_mod("WS","n23") = res_obj("n23_ws","1");
res_obj_mod("WS","n123") = res_obj("n123","1");
res_obj_mod("WS","all") = res_obj("n123","1");
*res_obj_mod("WS","KLE") = res_obj("KLE_WS","1");
res_obj_mod("DE","E") = res_obj("E_DE","1");
res_obj_mod("DE","KLE") = res_obj("KLE_DE","1");
res_obj_mod("DE","KLEM") = res_obj("KLEM_DE","1");
res_obj_mod("MW","E") = res_obj("E_MW","1");
res_obj_mod("MW","KLEM") = res_obj("KLEM_MW","1");







