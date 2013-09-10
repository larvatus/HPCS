set title "MBD - ECG"
set grid
set termoption dash
set multiplot layout 2,1
set style line 1 lt 3 lw 2 lc rgb "yellow" pt 11 ps 0.5
set style line 2 lt 3 lw 2 lc rgb "orange" pt 9 ps 0.5
set style line 3 lt 3 lw 2 lc rgb "red" pt 9 ps 0.5
set style line 4 lt 3 lw 2 lc rgb "blue" pt 9 ps 0.5
#plot mbd1 ls 1 with linespoints title 'J=2', mbd2 ls 2 with linespoints title 'J=3', mbd3 ls 3 with linespoints title 'J=4', mbd4 ls 4 with linespoints title 'J=5'
#plot rks1 ls 1 title 'J=2', rks2 ls 2 title 'J=3', rks3 ls 3 title 'J=4', rks4 ls 4 title 'J=5'
plot mbd1 ls 1 with linespoints title 'J=2'
plot rks1 ls 1 with linespoints title 'J=2'
unset multiplot
