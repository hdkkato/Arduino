reset
set ytics nomirror
set y2tics
set xrange [0:4000]
set x2range [0:4000]
set yrange [0:1e6]
set y2range [0:1e6]
plot "20150601_due_8900sps_piano_a3_fft.dat" with lines, \
"20150608_due_20ksps_recorder_a4_fft.dat" with lines axes x2y2