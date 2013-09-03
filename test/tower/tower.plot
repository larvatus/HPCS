set title "MBD"
set grid
set style line 1 lt 6 lw 2 lc rgb "red" pt 11 ps 2
set style line 2 lt 6 lw 2 lc rgb "green" pt 9 ps 2
set style line 3 lt 6 lw 2 lc rgb "blue"  pt 7 ps 2
plot mbd1 ls 1 with linespoints title 'J=2', mbd2 ls 2 with linespoints title 'J=3', mbd3 ls 3 with linespoints title 'J=4'