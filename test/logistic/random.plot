
set grid
set key outside
set key right top
set key box
set grid
 
set terminal pdf
set output 'MBD_sampling.pdf'
set title "MBD con riferimento - ECG"
set ylabel 'MBD'
set xlabel 'Soggetti'


set style line 1 lt 3 lw 2 lc rgb "yellow" pt 1 ps 0.9
set style line 2 lt 3 lw 2 lc rgb "green" pt 2 ps 0.9
set style line 3 lt 3 lw 2 lc rgb "cyan" pt 3 ps 0.9
set style line 4 lt 3 lw 2 lc rgb "magenta" pt 4 ps 0.9
set style line 5 lt 3 lw 2 lc rgb "blue" pt 5 ps 0.9
set style line 6 lt 3 lw 2 lc rgb "chartreuse" pt 6 ps 0.9
set style line 7 lt 3 lw 2 lc rgb "black" pt 7 ps 0.9
set style line 8 lt 3 lw 2 lc rgb "khaki" pt 8 ps 0.9
set style line 9 lt 3 lw 2 lc rgb "purple" pt 9 ps 0.9
set style line 10 lt 3 lw 2 lc rgb "red" pt 10 ps 0.9
unset key
plot 'mbd.1.dat' ls 1 title 'seed 1', 'mbd.2.dat' ls 2 title 'seed 2', 'mbd.3.dat' ls 3 title 'seed 3', 'mbd.4.dat' ls 4 title 'seed 4', 'mbd.5.dat' ls 5 title 'seed 5', 'mbd.6.dat' ls 6 title 'seed 6', 'mbd.7.dat' ls 7 title 'seed 7', 'mbd.8.dat' ls 8 title 'seed 8', 'mbd.9.dat' ls 9 title 'seed 9','mbd.10.dat' ls 10 title 'seed 10'

