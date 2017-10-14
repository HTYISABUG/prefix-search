reset
set xlabel 'found data'
set ylabel 'time(cycles)'
set title 'average execute cycles'
set term png enhanced font 'Verdana,10'
set output 'prefix_search_avg.png'
set xtics rotate by 45 right

plot [:][:700000]'prefix_search_avg.txt' using 1:2 with linespoint pt 5 title 'data', \
'' using 1:3 with linespoint pt 5 title 'lower bound', \
'' using 1:4 with linespoint pt 5 title 'upper bound'
