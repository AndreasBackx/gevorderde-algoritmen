
#!/usr/bin/env gnuplot

set title 'Dieptes'
set xlabel 'Aantal knopen'
set ylabel 'Diepte'
set grid
set autoscale 
plot "dieptedata.csv" using 5, "dieptedata.csv" using 6 with linespoints
pause -1