
#!/usr/bin/env gnuplot

set title 'Coordinaten'
set xlabel 'X'
set ylabel 'Y'
set grid
set autoscale 
plot "dieptedata.csv" using 1:2
pause -1