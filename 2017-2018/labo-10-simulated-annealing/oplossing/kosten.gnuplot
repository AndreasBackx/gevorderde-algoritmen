
#!/usr/bin/env gnuplot

set grid
set autoscale
plot "kosten.csv" using 2 title 'Kost' with linespoints
pause -1
