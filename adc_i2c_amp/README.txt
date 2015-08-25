・ファイル構造

adc_i2c_amp
│  
│  
├─adc_i2c_amp
│      adc_i2c_amp.ino
│      
├─adc_i2c_amp_raw
│      adc_i2c_amp_raw.pde
│      
└─adc_i2c_amp_rollingmean
        adc_i2c_amp_rollingmean.pde
        
　adc_i2c_amp.inoはArduino UNOのスケッチで、
adc_i2c_amp_raw.pdeおよびadc_i2c_amp_rollingmean.pdeはprocessingのスケッチである。

adc_i2c_amp.inoは電流計（ACS714 -5Ato+5A）につながれたADC（GROVE - I2C ADC）からI2Cでデジタルデータを受け取り、
それをシリアル通信でパソコンに送信する。
processingはシリアル通信で1 byteずつしかデータを受信できないため、
送信データのフォーマットはヘッダ文字「H」とデジタルデータ2 byteで、
「HxxHxxHxx...」の形でデータを送信する（xxはバイトコード）。

adc_i2c_amp_raw.pdeおよびadc_i2c_amp_rollingmean.pdeはシリアル通信でArduinoが送信したデータを受け取り、
それを電流データに変換してその結果をプロットする。
adc_i2c_amp_raw.pdeは変換した電流をそのままプロットするプログラムで、
adc_i2c_amp_rollingmean.pdeは一定数のデータの移動平均をプロットするプログラムである。


ADC121C021自体は12bitのADC分解能を持つが、GROVE - I2C ADCの基板上で入力電圧を1/2にする分圧回路を持っているため実質11bitのADC分解能である。
ADC基板上で変圧されてVa=3.0Vとなるため、Va/4096=0.73mVのレベルで

・50 mA計測の例
                                                                              
 Raspberry Pi                Ammeter                  ADC                     
+-----------+           (ACS714 -5Ato+5A)       (GROVE -I2C ADC)              
|           |             +-----------+        +-----------------+            
|           |             |           |        |              SDA+----------+ 
|       5V  +-------------|+Vin   Vout+--------+SIG              |          | 
|           |             |           |        |              SCL+-------+  | 
|           |  +-------+  |       Vcc +--------+Vcc              |       |  | 
|       GND +--|100 ohm|--|-Vin       |        |              Vcc+-----+ |  | 
|           |  +-------+  |       GND +--------+GND              |     | |  | 
|           |             |           |        |              GND+---+ | |  | 
|           |             +-----------+        +-----------------+   | | |  | 
|           |                                                        | | |  | 
+-----------+                                                        | | |  | 
+-----------------------------------+                Arduino         | | |  | 
|    Computer                       |          +------------------+  | | |  | 
|                 +--------------+  |          |                  |  | | |  | 
|                 |              |  |          |              GND +--+ | |  | 
|                 |              |  | 'Hxxxx'  |                  |    | |  | 
|                 |  processing  <-------------+              5V  +----+ |  | 
| +--------------+|              |  |          |                  |      |  | 
| |              ||              |  |          |              SCL +------+  | 
| |              ||              |  |          |                  |         | 
| |     plot     |+------+-------+  |          |              SDA +---------+ 
| |              |       |          |          +------------------+           
| |              <-------+          |                                         
| +--------------+                  |                                         
+-----------------------------------+                                         