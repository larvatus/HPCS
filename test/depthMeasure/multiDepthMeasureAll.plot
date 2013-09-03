set title "MBD"
set grid
set style line 1 lt 6 lw 2 lc rgb "red" pt 11 ps 2
set style line 2 lt 6 lw 2 lc rgb "green" pt 9 ps 2
set style line 3 lt 6 lw 2 lc rgb "blue"  pt 7 ps 2
plot bd_I ls 1 with linespoints, bd_II ls 2 with linespoints , mbd ls 3 with linespoints