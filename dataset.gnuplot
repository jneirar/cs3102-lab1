set xtics 1000
set ytics 1000

set grid xtics
set grid ytics

set key noautotitle

plot "dataset.txt" with points pointtype 7 pointsize 0.05
