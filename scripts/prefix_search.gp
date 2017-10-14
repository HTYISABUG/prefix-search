reset
set xlabel 'found data'
set ylabel 'time(cycles)'
set title 'execute cycles'
set term png enhanced font 'Verdana,10'
set output 'prefix_search.png'
set format x "%10.0f"
set xtics rotate by 45 right

plot [:][:700000]'prefix_search.txt' using 3:2 with point title 'data'
