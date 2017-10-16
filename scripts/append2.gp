reset
set xlabel 'data distribution'
set ylabel 'time(cycles)'
set title 'execute cycles'
set term png enhanced font 'Verdana,10'
set output 'append2.png'
set format x "%10.0f"
set xtic 20000
set xtics rotate by 45 right

plot [:][-5000:100000]'data_cpy.txt' using 1:2 with points title 'data cpy', \
                      'ldata_cpy.txt' using 1:2 with points title 'long data cpy', \
                      'data_ref.txt' using 1:2 with points title 'data ref', \
                      'ldata_ref.txt' using 1:2 with points title 'long data ref'

