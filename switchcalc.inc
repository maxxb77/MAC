if((swlobnd=1),
         PVM_MW.lo(z,eee,r) = loval;
         PVE_MW.lo(z,eee,r) = loval;
         P_MW.lo(z,eee,r) = loval;
*         PCARB_.lo(Z,R) = loval;
         VE_MW.lo(z,eee,r) = loval;
         VM_MW.lo(z,eee,r) = loval;
         Y_MW.lo(z,eee,r) = loval;

         PVM_JE.lo(z,eee,r) = loval;
         PKLE_JE.lo(z,eee,r) = loval;
         PVE_JE.lo(z,eee,r) = loval;
         PVA_JE.lo(z,eee,r) = loval;
         P_JE.lo(z,eee,r) = loval;
*         PCARB_.lo(Z,R) = loval;
         VE_JE.lo(z,eee,r) = loval;
         VKLE_JE.lo(z,eee,r) = loval;
         VA_JE.lo(z,eee,r) = loval;
         VM_JE.lo(z,eee,r) = loval;
         Y_JE.lo(z,eee,r) = loval;
         VCOL_JE.lo(z,eee,r) = loval;
         PCOL_JE.lo(z,eee,r) = loval;

         PVM_de.lo(z,eee,r) = loval;
         PKLE_de.lo(z,eee,r) = loval;
         PVE_de.lo(z,eee,r) = loval;
         PVA_de.lo(z,eee,r) = loval;
         P_de.lo(z,eee,r) = loval;
*         pcarb_.lo(Z,R) = loval;
         VE_de.lo(z,eee,r) = loval;
         VKLE_de.lo(z,eee,r) = loval;
         VA_de.lo(z,eee,r) = loval;
         VM_de.lo(z,eee,r) = loval;
         Y_de.lo(z,eee,r) = loval;
);


*the fixrow switch requires an optfile!
*optfile is used to avoid pretriangular infeasibilities.
*name of file is conopt.opt and it contains a single line:
*Rtnwtr = 1e-6

If((fixrow=1),
         PVM_MW.fx(z,eee,r)$row(r) =  PVM.l(eee,r);
         PVE_MW.fx(z,eee,r)$row(r) =  PVE.l(eee,r);
         P_MW.fx(z,eee,r)$row(r) =  P.l(eee,r);
         PCARB_.fx(Z,R)$row(r) =  PCARB.l(R);
         VE_MW.fx(z,eee,r)$row(r) =  VE.l(eee,r);
         VM_MW.fx(z,eee,r)$row(r) =  VM.l(eee,r);
         Y_MW.fx(z,eee,r)$row(r) =  Y.l(eee,r);

         PVM_JE.fx(z,eee,r)$row(r) =  PVM.l(eee,r);
         PKLE_JE.fx(z,eee,r)$row(r) =  PKLE.l(eee,r);
         PVE_JE.fx(z,eee,r)$row(r) =  PVE.l(eee,r);
         PVA_JE.fx(z,eee,r)$row(r) =  PVA.l(eee,r);
         P_JE.fx(z,eee,r)$row(r) =  P.l(eee,r);
*         PCARB_.fx(Z,R)$row(r) =  PCARB.l(R);
         VE_JE.fx(z,eee,r)$row(r) =  VE.l(eee,r);
         VKLE_JE.fx(z,eee,r)$row(r) =  VKLE.l(eee,r);
         VA_JE.fx(z,eee,r)$row(r) =  VA.l(eee,r);
         VM_JE.fx(z,eee,r)$row(r) =  VM.l(eee,r);
         Y_JE.fx(z,eee,r)$row(r) =  Y.l(eee,r);
         VCOL_JE.fx(z,eee,r)$row(r) = VCO.l(eee,r);
         PCOL_JE.fx(z,eee,r)$row(r) = pco.l(eee,r);

         PVM_de.fx(z,eee,r)$row(r) =  PVM.l(eee,r);
         PKLE_de.fx(z,eee,r)$row(r) =  PKLE.l(eee,r);
         PVE_de.fx(z,eee,r)$row(r) =  PVE.l(eee,r);
         PVA_de.fx(z,eee,r)$row(r) =  PVA.l(eee,r);
*         P_de.fx(z,eee,r)$row(r) =  P.l(eee,r);
         pcarb_.fx(Z,R)$row(r) =  PCARB.l(R);
         VE_de.fx(z,eee,r)$row(r) =  VE.l(eee,r);
         VKLE_de.fx(z,eee,r)$row(r) =  VKLE.l(eee,r);
         VA_de.fx(z,eee,r)$row(r) =  VA.l(eee,r);
         VM_de.fx(z,eee,r)$row(r) =  VM.l(eee,r);
         Y_de.fx(z,eee,r)$row(r) =  Y.l(eee,r);

);

*if((SwYFX=1),
*         Y_MW.fx(z,eee,"usa") = y.l(eee,"usa");
*         Y_JE.fx(z,eee,"usa") = y.l(eee,"usa");
*             Y_DE.fx(z,eee,"usa") = y.l(eee,"usa");
*);
