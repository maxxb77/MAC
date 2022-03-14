*kr scaling method 1
if((SwMACC=4),
MACC(Z,pq)=MACC_ORIG(Z,pq);
MACC(Z,"p")$(MACC(Z,"p")<>0) = MACC(Z,"p") + 42.79;
MACC(Z,"q") = (2.539 - MACC("1","q") +  MACC(Z,"q")) / 1000;
MACC("1","p") = 1e-4;
MACC("20",pq) = 0;

);

*kr scaling method 2
*sets all negative prices to zero
if((SwMACC=5),
MACC(Z,pq)=MACC_ORIG(Z,pq);
MACC(Z,"q") = (2.539 - MACC("1","q") +  MACC(Z,"q")) / 1000;
MACC(Z,"p")$(MACC(Z,"p")<0) = 1e-4;
MACC("20",pq) = 0;
);

*kr scaling method 3
*excludes negative prices completely
if((SwMACC=6),
MACC(Z,"p") = macc_orig_nonzero(z,"p");
MACC(Z,"q") = MACC_orig_nonzero(Z,"q") / 1000;
MACC("1","p") = 1e-4;
);
