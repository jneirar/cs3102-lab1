#set grid x2tics
#set x2tics 100 format "" scale 0

set xtics 100
set ytics 100

set grid xtics
set grid ytics

set key noautotitle

plot "build/tests/dataset.txt" with points pointtype 7 pointsize 0.5
pause -1 

set palette model RGB defined (0 "red",1 "blue", 2 "green")
unset colorbox
replot "build/tests/output.txt" with points pointtype 7 pointsize 1 palette
