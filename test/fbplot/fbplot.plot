set title "ECG"
set grid
set termoption dash
set multiplot layout 2,1
set style line 1 lt 3 lw 2 lc rgb "orange" pt 11 ps 0.5
set style line 2 lt 3 lw 2 lc rgb "red" pt 9 ps 0.5
set style line 3 lt 3 lw 2 lc rgb "yellow" pt 9 ps 0.5
set style line 4 lt 3 lw 2 lc rgb "blue" pt 9 ps 0.5
set key box
set key outside
set key top right
plot mbd ls 1 title 'J=2'
plot rks ls 2 title 'J=2'
unset multiplot
