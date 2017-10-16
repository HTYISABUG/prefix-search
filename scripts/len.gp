reset
set xlabel 'data length'
set ylabel 'number'
set title 'data distribution'
set term png enhanced font 'Verdana,10'
set output 'len.png'
set style fill solid
set xtic 2
set label rotate by 45 right

plot [:][:]'len_cpy.txt' using 2 with histogram title 'cpy', \
           'len_ref.txt' using 2 with histogram title 'ref'
