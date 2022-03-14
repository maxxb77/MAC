parameter res_elas_de_seq, res_obj_seq;

*Scalar SwFixCO "switch to fix [1] esub_co in the JE model after it is freed up" /0/;
*this would give the JE model an 'unfair' advantage in having more variables
*it is a subtlety but should be tested both way to makes sure there is no change

pcarb_.fx(z,"usa")$ZFEAS(Z) = MACC(z,"p");

alpha_ve_je(z,i,eee,r)$((e(i))$(not co(i))) = ((vafm(i,eee,r)*(1+rtfa(i,eee,r))) / (vafm(i,eee,r)*(1+rtfa(i,eee,r)) - pcarb0(r)*refco2(i,eee,r))) * pa.l(i,r)
                        + refco2(i,eee,r) * pcarb0(r) / ((vafm(i,eee,r)*(1+rtfa(i,eee,r)))-refco2(i,eee,r) * pcarb0(r)) * pcarb_.l(z,r)/pcarb0(r);

alpha_co_je(z,i,eee,r)$co(i) = ((vafm(i,eee,r)*(1+rtfa(i,eee,r))) / (vafm(i,eee,r)*(1+rtfa(i,eee,r)) - pcarb0(r)*refco2(i,eee,r))) * pa.l(i,r)
                        + refco2(i,eee,r) * pcarb0(r) / ((vafm(i,eee,r)*(1+rtfa(i,eee,r)))-refco2(i,eee,r) * pcarb0(r)) * pcarb_.l(z,r)/pcarb0(r);



*Solve the models with no elasticities free
solve ssempec_de using mpec minimizing OBJ_de;
solve ssempec_de using mpec minimizing OBJ_de;
solve ssempec_de using mpec minimizing OBJ_de;
solve ssempec_de using mpec minimizing OBJ_de;
solve ssempec_mw using mpec minimizing OBJ_mw;

res_pq_DE(z,"rp") = MACC(Z,"p");
res_pq_DE(z,"rq") = MACC(Z,"Q");
res_pq_MW(z,"rp") = MACC(Z,"p");
res_pq_MW(z,"rq") = MACC(Z,"Q");

res_pq_DE(z,"base") = (p_de.l(z,"ele","usa")**mu)*SUM(FE,REFCO2(FE,"ELE","USA")*FACDEM_de.l(Z,FE,"ELE","USA"));
res_pq_MW(z,"base") = (p_mw.l(z,"ele","usa")**mu)*SUM(FE,REFCO2(FE,"ELE","USA")*FACDEM_mw.l(Z,FE,"ELE","USA"));

res_obj_seq("base_DE","1") = obj_de.l;
res_obj_seq("base_MW","1") = OBJ_mw.l;



*///////////////////////////////
*//////MW Nesting Structure/////
*///////////////////////////////

res_elas_MW("REF","E") = 0.2;
res_elas_MW("REF","KLEM") = 0.76;


*free up only e nest
esub_e_mw.l("ele","usa") = res_elas_MW("REF","E");
esub_e_mw.lo("ele","usa") = lolim;
esub_e_mw.up("ele","usa") = uplim;
solve ssempec_mw using mpec minimizing OBJ_MW;
res_pq_MW(z,"E") = (p_MW.l(z,"ele","usa")**mu)*sum(FE,REFCO2(FE,"ELE","USA")*FACDEM_MW.l(Z,FE,"ELE","USA"));
res_elas_MW("E","E") = esub_e_mw.l("ele","usa");
res_obj_seq("eo_mw","1") = obj_MW.l;

*fix e and free klem
esub_e_mw.fx("ele","usa") = res_elas_MW("REF","E");
ESUB_KLE_M_MW.lo("ele","usa") = lolim;
ESUB_KLE_M_MW.up("ele","usa") = UPlim;
ESUB_KLE_M_MW.l("ele","usa") = res_elas_MW("REF","KLEM");
solve ssempec_mw using mpec minimizing OBJ_MW;
res_pq_MW(z,"klemo") = (p_MW.l(z,"ele","usa")**mu)*sum(FE,REFCO2(FE,"ELE","USA")*FACDEM_MW.l(Z,FE,"ELE","USA"));
res_elas_MW("klemo","E") = esub_e_mw.l("ele","usa");
res_elas_MW("klemo","KLEM") = esub_KLE_M_mw.l("ele","usa");
res_obj_seq("klemo_mw","1") = obj_MW.l;


*free up both nests
ESUB_KLE_M_MW.l("ele","usa") = res_elas_MW("REF","KLEM");
esub_e_mw.l("ele","usa") = res_elas_MW("REF","E");
esub_e_mw.lo("ele","usa") = lolim;
esub_e_mw.up("ele","usa") = uplim;
ESUB_KLE_M_MW.lo("ele","usa") = lolim;
ESUB_KLE_M_MW.up("ele","usa") = UPlim;   
esub_e_mw.l("ele","usa") = res_elas_MW("REF","E")    ;
ESUB_KLE_M_MW.l("ele","usa")  = res_elas_MW("REF","KLEM") ;
solve ssempec_mw using mpec minimizing OBJ_MW;
res_pq_MW(z,"all") = (p_MW.l(z,"ele","usa")**mu)*sum(FE,REFCO2(FE,"ELE","USA")*FACDEM_MW.l(Z,FE,"ELE","USA"));
res_elas_MW("all","E") = esub_e_mw.l("ele","usa");
res_elas_MW("all","KLEM") = esub_KLE_M_mw.l("ele","usa");
res_obj_seq("all_mw","1") = obj_MW.l;


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


res_elas_DE_seq("REF","E") = 0.5;
res_elas_DE_seq("REF","KLE") = 0.26;
res_elas_DE_seq("REF","KLEM") = 0.1;


*free up only E nest
esub_KL_E_DE.fx("ele","usa") = res_elas_DE_seq("REF","KLE") ;
esub_KLE_M_DE.fx("ele","usa") = res_elas_DE_seq("REF","KLEM");
esub_e_de.fx("ele","usa") = res_elas_DE_seq("REF","E");
*esub_KL_E_DE.lo("ele","usa")  = lolim;
*esub_KL_E_DE.up("ele","usa")  = uplim;
*esub_KLE_M_DE.lo("ele","usa") = lolim;
*esub_KLE_M_DE.up("ele","usa") = uplim;
esub_e_de.lo("ele","usa")     = lolim;
esub_e_de.up("ele","usa")     = uplim;
solve ssempec_de using mpec minimizing OBJ_de;
solve ssempec_de using mpec minimizing OBJ_de;
solve ssempec_de using mpec minimizing OBJ_de;
solve ssempec_de using mpec minimizing OBJ_de;
res_pq_DE(z,"eo") = (p_de.l(z,"ele","usa")**mu)*sum(FE,REFCO2(FE,"ELE","USA")*FACDEM_de.l(Z,FE,"ELE","USA"));
res_elas_DE_seq("eo","E") = esub_E_DE.l("ele","usa");
res_elas_DE_seq("eo","KLE") = esub_KL_E_DE.l("ele","usa");
res_elas_DE_seq("eo","KLEM") = esub_KLE_M_DE.l("ele","usa");
res_obj_seq("eo_DE","1") = obj_de.l;


*free up only kle nest

esub_KL_E_DE.fx("ele","usa") = res_elas_DE_seq("REF","KLE") ;
esub_KLE_M_DE.fx("ele","usa") = res_elas_DE_seq("REF","KLEM");
esub_e_de.fx("ele","usa") = res_elas_DE_seq("REF","E");
esub_KL_E_DE.lo("ele","usa")  = lolim;
esub_KL_E_DE.up("ele","usa")  = uplim;
*esub_KLE_M_DE.lo("ele","usa") = lolim;
*esub_KLE_M_DE.up("ele","usa") = uplim;
*esub_e_de.lo("ele","usa")     = lolim;
*esub_e_de.up("ele","usa")     = uplim;
solve ssempec_de using mpec minimizing OBJ_de;
solve ssempec_de using mpec minimizing OBJ_de;
solve ssempec_de using mpec minimizing OBJ_de;
solve ssempec_de using mpec minimizing OBJ_de;
res_pq_DE(z,"KLEo_DE") = (p_de.l(z,"ele","usa")**mu)*sum(FE,REFCO2(FE,"ELE","USA")*FACDEM_de.l(Z,FE,"ELE","USA"));
res_elas_DE_seq("KLEo","E") = esub_E_DE.l("ele","usa");
res_elas_DE_seq("KLEo","KLE") = esub_KL_E_DE.l("ele","usa");
res_elas_DE_seq("KLEo","KLEM") = esub_KLE_M_DE.l("ele","usa");
res_obj_seq("KLEo_DE","1") = obj_de.l;



*free up only klem nest

esub_KL_E_DE.fx("ele","usa") = res_elas_DE_seq("REF","KLE") ;
esub_KLE_M_DE.fx("ele","usa") = res_elas_DE_seq("REF","KLEM");
esub_e_de.fx("ele","usa") = res_elas_DE_seq("REF","E");
*esub_KL_E_DE.lo("ele","usa")  = lolim;
*esub_KL_E_DE.up("ele","usa")  = uplim;
esub_KLE_M_DE.lo("ele","usa") = lolim;

*n.b. with klemo case we need to have less of an upper limit or we get into trouble with higher carbon prices
*trouble here means certain values reach infinity
esub_KLE_M_DE.up("ele","usa") = uplim;
*esub_e_de.lo("ele","usa")     = lolim;
*esub_e_de.up("ele","usa")     = uplim;
solve ssempec_de using mpec minimizing OBJ_de;
solve ssempec_de using mpec minimizing OBJ_de;
solve ssempec_de using mpec minimizing OBJ_de;
solve ssempec_de using mpec minimizing OBJ_de;
res_pq_DE(z,"KLEMo_DE") = (p_de.l(z,"ele","usa")**mu)*sum(FE,REFCO2(FE,"ELE","USA")*FACDEM_de.l(Z,FE,"ELE","USA"));
res_elas_DE_seq("KLEMo","E") = esub_E_DE.l("ele","usa");
res_elas_DE_seq("KLEMo","KLE") = esub_KL_E_DE.l("ele","usa");
res_elas_DE_seq("KLEMo","KLEM") = esub_KLE_M_DE.l("ele","usa");
res_obj_seq("KLEMo_DE","1") = obj_de.l;


*free up only E and KLE nest

esub_KL_E_DE.fx("ele","usa") = res_elas_DE_seq("REF","KLE") ;
esub_KLE_M_DE.fx("ele","usa") = res_elas_DE_seq("REF","KLEM");
esub_e_de.fx("ele","usa") = res_elas_DE_seq("REF","E");
esub_KL_E_DE.lo("ele","usa")  = lolim;
esub_KL_E_DE.up("ele","usa")  = uplim;
*esub_KLE_M_DE.lo("ele","usa") = lolim;
*esub_KLE_M_DE.up("ele","usa") = uplim;
esub_e_de.lo("ele","usa")     = lolim;
esub_e_de.up("ele","usa")     = uplim;
solve ssempec_de using mpec minimizing OBJ_de;
solve ssempec_de using mpec minimizing OBJ_de;
solve ssempec_de using mpec minimizing OBJ_de;
solve ssempec_de using mpec minimizing OBJ_de;
res_pq_DE(z,"E_KLE_o_DE") = (p_de.l(z,"ele","usa")**mu)*sum(FE,REFCO2(FE,"ELE","USA")*FACDEM_de.l(Z,FE,"ELE","USA"));
res_elas_DE_seq("E_KLE_o","E") = esub_E_DE.l("ele","usa");
res_elas_DE_seq("E_KLE_o","KLE") = esub_KL_E_DE.l("ele","usa");
res_elas_DE_seq("E_KLE_o","KLEM") = esub_KLE_M_DE.l("ele","usa");
res_obj_seq("E_KLE_o_DE","1") = obj_de.l;


*free up only E and KLEM nest

esub_KL_E_DE.fx("ele","usa") = res_elas_DE_seq("REF","KLE") ;
esub_KLE_M_DE.fx("ele","usa") = res_elas_DE_seq("REF","KLEM");
esub_e_de.fx("ele","usa") = res_elas_DE_seq("REF","E");
*esub_KL_E_DE.lo("ele","usa")  = lolim;
*esub_KL_E_DE.up("ele","usa")  = uplim;
esub_KLE_M_DE.lo("ele","usa") = lolim;
esub_KLE_M_DE.up("ele","usa") = uplim;
esub_e_de.lo("ele","usa")     = lolim;
esub_e_de.up("ele","usa")     = uplim;
solve ssempec_de using mpec minimizing OBJ_de;
solve ssempec_de using mpec minimizing OBJ_de;
solve ssempec_de using mpec minimizing OBJ_de;
solve ssempec_de using mpec minimizing OBJ_de;
res_pq_DE(z,"E_KLEM_o_DE") = (p_de.l(z,"ele","usa")**mu)*sum(FE,REFCO2(FE,"ELE","USA")*FACDEM_de.l(Z,FE,"ELE","USA"));
res_elas_DE_seq("E_KLEM_o","E") = esub_E_DE.l("ele","usa");
res_elas_DE_seq("E_KLEM_o","KLE") = esub_KL_E_DE.l("ele","usa");
res_elas_DE_seq("E_KLEM_o","KLEM") = esub_KLE_M_DE.l("ele","usa");
res_obj_seq("E_KLEM_DE","1") = obj_de.l;



*free up only KLE and KLEM nest

esub_KL_E_DE.fx("ele","usa") = res_elas_DE_seq("REF","KLE") ;
esub_KLE_M_DE.fx("ele","usa") = res_elas_DE_seq("REF","KLEM");
esub_e_de.fx("ele","usa") = res_elas_DE_seq("REF","E");
esub_KL_E_DE.lo("ele","usa")  = lolim;
esub_KL_E_DE.up("ele","usa")  = uplim;
esub_KLE_M_DE.lo("ele","usa") = lolim;
esub_KLE_M_DE.up("ele","usa") = uplim;
*esub_e_de.lo("ele","usa")     = lolim;
*esub_e_de.up("ele","usa")     = uplim;
solve ssempec_de using mpec minimizing OBJ_de;
solve ssempec_de using mpec minimizing OBJ_de;
solve ssempec_de using mpec minimizing OBJ_de;
solve ssempec_de using mpec minimizing OBJ_de;
res_pq_DE(z,"KLE_KLEM_DE") = (p_de.l(z,"ele","usa")**mu)*sum(FE,REFCO2(FE,"ELE","USA")*FACDEM_de.l(Z,FE,"ELE","USA"));
res_elas_DE_seq("KLE_KLEM","E") = esub_E_DE.l("ele","usa");
res_elas_DE_seq("KLE_KLEM","KLE") = esub_KL_E_DE.l("ele","usa");
res_elas_DE_seq("KLE_KLEM","KLEM") = esub_KLE_M_DE.l("ele","usa");
res_obj_seq("KLE_KLEM_DE","1") = obj_de.l;


*free up all nests

esub_KL_E_DE.fx("ele","usa") = res_elas_DE_seq("REF","KLE") ;
esub_KLE_M_DE.fx("ele","usa") = res_elas_DE_seq("REF","KLEM");
esub_e_de.fx("ele","usa") = res_elas_DE_seq("REF","E");
esub_KL_E_DE.lo("ele","usa")  = lolim;
esub_KL_E_DE.up("ele","usa")  = uplim;
esub_KLE_M_DE.lo("ele","usa") = lolim;
esub_KLE_M_DE.up("ele","usa") = uplim;
esub_e_de.lo("ele","usa")     = lolim;
esub_e_de.up("ele","usa")     = uplim;
solve ssempec_de using mpec minimizing OBJ_de;
solve ssempec_de using mpec minimizing OBJ_de;
solve ssempec_de using mpec minimizing OBJ_de;
solve ssempec_de using mpec minimizing OBJ_de;
res_pq_DE(z,"all") = (p_de.l(z,"ele","usa")**mu)*sum(FE,REFCO2(FE,"ELE","USA")*FACDEM_de.l(Z,FE,"ELE","USA"));
res_elas_DE_seq("all","E") = esub_E_DE.l("ele","usa");
res_elas_DE_seq("all","KLE") = esub_KL_E_DE.l("ele","usa");
res_elas_DE_seq("all","KLEM") = esub_KLE_M_DE.l("ele","usa");
res_obj_seq("all_DE","1") = obj_de.l;

display res_pq_mw, res_elas_mw, res_pq_de, res_elas_de_seq, res_obj_seq;

