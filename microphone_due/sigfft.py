# -*- coding: utf-8 -*-

import sys
import numpy as np
from numpy import fft
import pandas as pd

argv = sys.argv
argc = len(argv)
if (argc != 3):
    print("Two input must be need.")
    quit()

file_in = argv[1]
file_out = file_in.split(sep='.')
file_out = '_fft.'.join(file_out)

# take frequence parameter from command line
fs = int(argv[2])
duration = 1/fs

sig = np.loadtxt(file_in)
sig = sig - sig.mean()
sig_fft = fft.fft(sig)
freq = fft.fftfreq(len(sig), duration)

sig_fft = sig_fft[:(len(sig_fft)/2)]
sig_fft = abs(sig_fft)
freq = freq[:(len(freq)/2)]
result = pd.DataFrame(sig_fft, index=freq)

if __name__ == '__main__':
    result.to_csv(file_out, sep=' ', header=False)
