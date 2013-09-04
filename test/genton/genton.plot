set title "MBD"
set grid
set termoption dash
set style line 1 lt 3 lw 2 lc rgb "red" pt 11 ps 2
set style line 2 lt 3 lw 2 lc rgb "green" pt 9 ps 2
plot bdGen ls 1 with linespoints title 'Genton', bdDir ls 2 with linespoints title 'Direct'