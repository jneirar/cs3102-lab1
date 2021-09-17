set xtics 1000
set ytics 1000

set grid xtics
set grid ytics

set key noautotitle

plot "dataset.txt" with points pointtype 7 pointsize 0.05

set palette model RGB defined (0 "red",1 "blue", 2 "green")
unset colorbox
replot "output.txt" with points pointtype 7 pointsize 0.5 palette
