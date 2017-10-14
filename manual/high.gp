reset
set xlabel 'data length'
set ylabel 'number'
set title 'performance comparison'
set term png enhanced font 'Verdana,10'
set output 'high.png'
set style fill solid
set xtic 2
set label rotate by 45 right

plot [:][:]'high.txt' using 2 with histogram title ''
