reset
set xlabel 'data length'
set ylabel 'number'
set title 'performance comparison'
set term png enhanced font 'Verdana,10'
set output 'len.png'
set style fill solid
set xtic 2
set label rotate by 45 right

plot [:][:]'len.txt' using 2 with histogram title 'cities.txt'
