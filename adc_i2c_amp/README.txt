�E�t�@�C���\��

adc_i2c_amp
��  
��  
����adc_i2c_amp
��      adc_i2c_amp.ino
��      
����adc_i2c_amp_raw
��      adc_i2c_amp_raw.pde
��      
����adc_i2c_amp_rollingmean
        adc_i2c_amp_rollingmean.pde
        
�@adc_i2c_amp.ino��Arduino UNO�̃X�P�b�`�ŁA
adc_i2c_amp_raw.pde�����adc_i2c_amp_rollingmean.pde��processing�̃X�P�b�`�ł���B

adc_i2c_amp.ino�͓d���v�iACS714 -5Ato+5A�j�ɂȂ��ꂽADC�iGROVE - I2C ADC�j����I2C�Ńf�W�^���f�[�^���󂯎��A
������V���A���ʐM�Ńp�\�R���ɑ��M����B
processing�̓V���A���ʐM��1 byte�������f�[�^����M�ł��Ȃ����߁A
���M�f�[�^�̃t�H�[�}�b�g�̓w�b�_�����uH�v�ƃf�W�^���f�[�^2 byte�ŁA
�uHxxHxxHxx...�v�̌`�Ńf�[�^�𑗐M����ixx�̓o�C�g�R�[�h�j�B

adc_i2c_amp_raw.pde�����adc_i2c_amp_rollingmean.pde�̓V���A���ʐM��Arduino�����M�����f�[�^���󂯎��A
�����d���f�[�^�ɕϊ����Ă��̌��ʂ��v���b�g����B
adc_i2c_amp_raw.pde�͕ϊ������d�������̂܂܃v���b�g����v���O�����ŁA
adc_i2c_amp_rollingmean.pde�͈�萔�̃f�[�^�̈ړ����ς��v���b�g����v���O�����ł���B


ADC121C021���̂�12bit��ADC����\�������AGROVE - I2C ADC�̊��œ��͓d����1/2�ɂ��镪����H�������Ă��邽�ߎ���11bit��ADC����\�ł���B
ADC���ŕψ������Va=3.0V�ƂȂ邽�߁AVa/4096=0.73mV�̃��x����

�E50 mA�v���̗�
                                                                              
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