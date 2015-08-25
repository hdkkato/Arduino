### 測定波形の比較

- ピアノ
1周期20サンプル (8900 sps)
-> 1/(20/8900) = 440 Hz
![](C:\Users\Hideaki\Pictures\piano_a3.png)


- リコーダー
1周期23サンプル (20000 sps)
-> 1/(23/20000) = 880 Hz
![](C:\Users\Hideaki\Pictures\recorder_a4.png)



- ピアノ(赤)とリコーダー(緑)
![](C:\Users\Hideaki\Pictures\piano_recorder.png)


### FFT結果の比較

- ピアノ
![](C:\Users\Hideaki\Pictures\piano_a3_fft.png)


- リコーダー
![](C:\Users\Hideaki\Pictures\recorder_a4_fft.png)



- ピアノ(赤)とリコーダー(緑)
![](C:\Users\Hideaki\Pictures\piano_recorder_fft.png)

### 金属音の解析


減衰曲線の式は以下であらわされる
$$
\begin{align}

y &= N_{0}e^{-t/\tau} \\

\to log(y) &= log(N_{0}) + (- t/\tau_{1/2}) \\

\end{align}
$$

ただし、tは時間、 $$$ \tau $$$ は時定数である。
半減期$$$\tau_{1/2} = ln2 \cdot \tau$$$を用いると$$$ y = N_{0}2^{-t/\tau_{1/2}}$$$と表すことができる。
つまり、$$$ \tau_{1/2} $$$ sec経過すると信号が半減することを表す。


- 2000 Hz近辺

$$$ y = 665\cdot 2^{-t/\tau_{1/2}}, \tau_{1/2} = 0. [sec] $$$

- 3000 Hz付近

 - $$$ y = 120\cdot2^{-t/\tau_{1/2}}, \tau_{1/2} = 0.139 [sec]  $$$

高周波数の音は約3倍早く減衰する。