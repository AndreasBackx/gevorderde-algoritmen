
#!/usr/bin/env gnuplot

set title 'Geshuffelde coordinaten'
set xlabel 'X'
set ylabel 'Y'
set grid
set autoscale
plot "dieptedata.csv" using 3:4
pause -1
