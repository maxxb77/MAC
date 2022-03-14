copy MACC_Export_0.xls MACC_export.xls

gams masterpc.gms --swmaccext=4 --swelasext=0
copy masterpc.lst masterpc_4_0.lst

gams masterpc.gms --swmaccext=5 --swelasext=0
copy masterpc.lst masterpc_5_0.lst

gams masterpc.gms --swmaccext=6 --swelasext=0
copy masterpc.lst masterpc_6_0.lst

copy res_pq_DE_4.gdx res_pq_DE_4_0.gdx
copy res_pq_MW_4.gdx res_pq_MW_4_0.gdx
copy res_pq_WS_4.gdx res_pq_WS_4_0.gdx
copy res_pq_DE_5.gdx res_pq_DE_5_0.gdx
copy res_pq_MW_5.gdx res_pq_MW_5_0.gdx
copy res_pq_WS_5.gdx res_pq_WS_5_0.gdx
copy res_pq_DE_6.gdx res_pq_DE_6_0.gdx
copy res_pq_MW_6.gdx res_pq_MW_6_0.gdx
copy res_pq_WS_6.gdx res_pq_WS_6_0.gdx

copy MACC_export.xls MACC_Export_0.xls

copy MACC_Export_1.xls MACC_export.xls

gams masterpc.gms --swmaccext=4 --swelasext=1
copy masterpc.lst masterpc_4_1.lst

gams masterpc.gms --swmaccext=5 --swelasext=1
copy masterpc.lst masterpc_5_1.lst

gams masterpc.gms --swmaccext=6 --swelasext=1
copy masterpc.lst masterpc_6_1.lst

copy res_pq_DE_4.gdx res_pq_DE_4_1.gdx
copy res_pq_MW_4.gdx res_pq_MW_4_1.gdx
copy res_pq_WS_4.gdx res_pq_WS_4_1.gdx
copy res_pq_DE_5.gdx res_pq_DE_5_1.gdx
copy res_pq_MW_5.gdx res_pq_MW_5_1.gdx
copy res_pq_WS_5.gdx res_pq_WS_5_1.gdx
copy res_pq_DE_6.gdx res_pq_DE_6_1.gdx
copy res_pq_MW_6.gdx res_pq_MW_6_1.gdx
copy res_pq_WS_6.gdx res_pq_WS_6_1.gdx

copy MACC_export.xls MACC_Export_1.xls
