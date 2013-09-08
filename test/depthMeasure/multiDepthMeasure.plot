set title "MBD"
set grid
set termoption dash
set style line 1 lt 3 lw 2 lc rgb "red" pt 11 ps 2
set style line 2 lt 3 lw 2 lc rgb "green" pt 9 ps 2
set style line 3 lt 3 lw 2 lc rgb "blue"  pt 7 ps 2
plot bd_I ls 1, bd_II ls 2, mbd ls 3
