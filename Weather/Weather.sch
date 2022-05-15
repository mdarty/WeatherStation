EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Weather-rescue:ESP32-DEVKITC-32D-ESP32-DEVKITC-32D U1
U 1 1 6058926B
P 2550 2350
F 0 "U1" H 2550 3517 50  0000 C CNN
F 1 "ESP32-DEVKITC-32D" H 2550 3426 50  0000 C CNN
F 2 "ESP32-DEVKITC-32D:MODULE_ESP32-DEVKITC-32D" H 2550 2350 50  0001 L BNN
F 3 "" H 2550 2350 50  0001 L BNN
F 4 "Espressif Systems" H 2550 2350 50  0001 L BNN "MANUFACTURER"
F 5 "4" H 2550 2350 50  0001 L BNN "PARTREV"
	1    2550 2350
	1    0    0    -1  
$EndComp
$Comp
L Connector:RJ12 J3
U 1 1 60690D5C
P 6450 2250
F 0 "J3" H 6120 2254 50  0000 R CNN
F 1 "RJ12 - Rain" H 6120 2345 50  0000 R CNN
F 2 "Connector_RJ:RJ12_Amphenol_54601" V 6450 2275 50  0001 C CNN
F 3 "~" V 6450 2275 50  0001 C CNN
	1    6450 2250
	1    0    0    1   
$EndComp
$Comp
L Connector:RJ12 J2
U 1 1 60692824
P 6450 1100
F 0 "J2" H 6121 1104 50  0000 R CNN
F 1 "RJ12 - Wind" H 6121 1195 50  0000 R CNN
F 2 "Connector_RJ:RJ12_Amphenol_54601" V 6450 1125 50  0001 C CNN
F 3 "~" V 6450 1125 50  0001 C CNN
	1    6450 1100
	1    0    0    1   
$EndComp
Text GLabel 7050 1800 0    50   Input ~ 0
GND
Text GLabel 7900 2650 3    50   Input ~ 0
GND
Text GLabel 8150 2200 1    50   Output ~ 0
Rain
Text GLabel 7200 900  1    50   Output ~ 0
Wdir
Text GLabel 7200 1150 3    50   Output ~ 0
Wspd
Text GLabel 8900 600  2    50   Input ~ 0
3.3v
Text GLabel 8500 1950 2    50   Input ~ 0
3.3v
$Comp
L Device:R R12
U 1 1 606A285C
P 7800 2100
F 0 "R12" H 7870 2146 50  0000 L CNN
F 1 "43k" H 7870 2055 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.30x1.75mm_HandSolder" V 7730 2100 50  0001 C CNN
F 3 "~" H 7800 2100 50  0001 C CNN
	1    7800 2100
	1    0    0    -1  
$EndComp
Connection ~ 7800 2250
$Comp
L Device:C_Small C3
U 1 1 606A5C97
P 7800 2350
F 0 "C3" H 7892 2396 50  0000 L CNN
F 1 "0.1 uF" H 7892 2305 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.33x1.80mm_HandSolder" H 7800 2350 50  0001 C CNN
F 3 "~" H 7800 2350 50  0001 C CNN
	1    7800 2350
	1    0    0    -1  
$EndComp
Wire Wire Line
	7800 2550 7800 2450
$Comp
L Device:R R13
U 1 1 606AAA36
P 7700 1100
F 0 "R13" H 7770 1146 50  0000 L CNN
F 1 "43k" H 7770 1055 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.30x1.75mm_HandSolder" V 7630 1100 50  0001 C CNN
F 3 "~" H 7700 1100 50  0001 C CNN
	1    7700 1100
	0    1    1    0   
$EndComp
$Comp
L Device:C_Small C4
U 1 1 606AB4A0
P 7400 1550
F 0 "C4" H 7492 1596 50  0000 L CNN
F 1 "0.1 uF" H 7492 1505 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.33x1.80mm_HandSolder" H 7400 1550 50  0001 C CNN
F 3 "~" H 7400 1550 50  0001 C CNN
	1    7400 1550
	1    0    0    -1  
$EndComp
Wire Wire Line
	6850 1100 7200 1100
Wire Wire Line
	6850 1200 7100 1200
Wire Wire Line
	7100 1200 7100 1300
Wire Wire Line
	6850 1300 7100 1300
Connection ~ 7100 1300
$Comp
L Device:R R14
U 1 1 606B9005
P 8300 850
F 0 "R14" H 8370 896 50  0000 L CNN
F 1 "10k" H 8370 805 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.30x1.75mm_HandSolder" V 8230 850 50  0001 C CNN
F 3 "~" H 8300 850 50  0001 C CNN
	1    8300 850 
	1    0    0    -1  
$EndComp
Wire Wire Line
	8300 700  8300 600 
Text GLabel 6000 4650 0    50   Input ~ 0
SD_CS
Text GLabel 6050 5150 0    50   Input ~ 0
SPI_CIPO
Text GLabel 6000 5050 0    50   Input ~ 0
GND
Text GLabel 6000 4850 0    50   Input ~ 0
3.3v
Text GLabel 6000 4950 0    50   Input ~ 0
SPI_SCK
Text GLabel 6000 4750 0    50   Input ~ 0
SPI_COPI
Wire Wire Line
	6000 4850 6400 4850
Wire Wire Line
	6400 4950 6000 4950
Wire Wire Line
	6000 4650 6400 4650
Wire Wire Line
	6050 5150 6400 5150
Wire Wire Line
	6000 4750 6400 4750
$Comp
L Connector:Conn_01x04_Female J6
U 1 1 606C5FBB
P 10250 3200
F 0 "J6" H 10278 3176 50  0000 L CNN
F 1 "UV" H 10278 3085 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x04_P2.54mm_Vertical" H 10250 3200 50  0001 C CNN
F 3 "~" H 10250 3200 50  0001 C CNN
	1    10250 3200
	1    0    0    -1  
$EndComp
Text GLabel 1750 1450 0    50   Input ~ 0
3.3v
Text GLabel 3650 2250 2    50   Input ~ 0
SPI_SCK
Wire Wire Line
	3650 2250 3350 2250
Text GLabel 3650 2350 2    50   Input ~ 0
SD_CS
Wire Wire Line
	3650 2350 3350 2350
Text GLabel 3650 1550 2    50   Input ~ 0
SPI_COPI
Wire Wire Line
	3650 1550 3350 1550
Text GLabel 3650 2150 2    50   Input ~ 0
SPI_CIPO
Wire Wire Line
	3650 2150 3350 2150
Text GLabel 3500 1450 2    50   Input ~ 0
GND
Wire Wire Line
	3500 1450 3350 1450
Text GLabel 3500 2050 2    50   Input ~ 0
GND
Text GLabel 1550 2750 0    50   Input ~ 0
GND
Wire Wire Line
	1550 2750 1750 2750
Wire Wire Line
	3500 2050 3350 2050
Text GLabel 3500 1950 2    50   Input ~ 0
SDA
Wire Wire Line
	3500 1950 3350 1950
Text GLabel 3550 1650 2    50   Input ~ 0
SCL
Wire Wire Line
	3550 1650 3350 1650
Text GLabel 9800 3200 0    50   Input ~ 0
GND
Text GLabel 9800 3100 0    50   Input ~ 0
3.3v
Text GLabel 9800 3300 0    50   Input ~ 0
SCL
Text GLabel 9800 3400 0    50   Input ~ 0
SDA
Wire Wire Line
	9800 3400 10050 3400
Wire Wire Line
	10050 3300 9800 3300
Wire Wire Line
	9800 3200 10050 3200
Wire Wire Line
	10050 3100 9800 3100
$Comp
L Connector:Conn_01x05_Male J7
U 1 1 606E70CE
P 10250 3800
F 0 "J7" H 10222 3732 50  0000 R CNN
F 1 "RTC" H 10222 3823 50  0000 R CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x05_P2.54mm_Vertical" H 10250 3800 50  0001 C CNN
F 3 "~" H 10250 3800 50  0001 C CNN
	1    10250 3800
	-1   0    0    1   
$EndComp
$Comp
L Connector:Conn_01x08_Female J9
U 1 1 606E825E
P 10250 5150
F 0 "J9" H 10278 5126 50  0000 L CNN
F 1 "Lightning" H 10278 5035 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x08_P2.54mm_Vertical" H 10250 5150 50  0001 C CNN
F 3 "~" H 10250 5150 50  0001 C CNN
	1    10250 5150
	1    0    0    -1  
$EndComp
Text GLabel 9900 4000 0    50   Input ~ 0
GND
Text GLabel 9900 3600 0    50   Input ~ 0
3.3v
Text GLabel 9900 3900 0    50   Input ~ 0
SDA
Text GLabel 9900 3800 0    50   Input ~ 0
SCL
Text GLabel 9900 3700 0    50   Input ~ 0
MFP
Wire Wire Line
	9900 3600 10050 3600
Wire Wire Line
	10050 3700 9900 3700
Wire Wire Line
	9900 3800 10050 3800
Wire Wire Line
	10050 3900 9900 3900
Wire Wire Line
	9900 4000 10050 4000
Text GLabel 3650 2450 2    50   Input ~ 0
MFP
Wire Wire Line
	3650 2450 3350 2450
Text Notes 4000 2550 0    50   ~ 0
MFP/IRQ is just a interrupt\nand can go on any pin
Text GLabel 9650 4850 0    50   Input ~ 0
3.3v
Text GLabel 9650 4950 0    50   Input ~ 0
GND
Text GLabel 9650 5050 0    50   Input ~ 0
SCL
Text GLabel 9650 5150 0    50   Input ~ 0
SDA
Text GLabel 9650 5350 0    50   Input ~ 0
IRQ
Wire Wire Line
	9650 4950 9750 4950
Wire Wire Line
	9750 5250 9750 4950
Connection ~ 9750 4950
Wire Wire Line
	9750 5550 9750 5250
Connection ~ 9750 5250
Wire Wire Line
	9750 5550 10050 5550
Wire Wire Line
	9750 5250 10050 5250
Wire Wire Line
	9750 4950 10050 4950
Wire Wire Line
	9650 4850 9900 4850
Wire Wire Line
	9900 4850 9900 5450
Wire Wire Line
	9900 5450 10050 5450
Connection ~ 9900 4850
Wire Wire Line
	9900 4850 10050 4850
Wire Wire Line
	10050 5350 9650 5350
Wire Wire Line
	9650 5150 10050 5150
Wire Wire Line
	10050 5050 9650 5050
Text GLabel 3650 2550 2    50   Input ~ 0
IRQ
Wire Wire Line
	3650 2550 3350 2550
$Comp
L Connector:Conn_01x03_Female J10
U 1 1 60717F0B
P 10250 5950
F 0 "J10" H 10278 5976 50  0000 L CNN
F 1 "Soil" H 10278 5885 50  0000 L CNN
F 2 "TerminalBlock:TerminalBlock_Altech_AK300-3_P5.00mm" H 10250 5950 50  0001 C CNN
F 3 "~" H 10250 5950 50  0001 C CNN
	1    10250 5950
	1    0    0    -1  
$EndComp
Text GLabel 9850 5850 0    50   Input ~ 0
3.3v
Text GLabel 9850 6050 0    50   Input ~ 0
GND
Text GLabel 9850 5950 0    50   Input ~ 0
SOIL
Wire Wire Line
	9850 5950 10050 5950
Wire Wire Line
	10050 5850 9850 5850
Wire Wire Line
	9850 6050 10050 6050
Text GLabel 1650 1850 0    50   Input ~ 0
SOIL
Text GLabel 1650 1950 0    50   Input ~ 0
Wdir
Text GLabel 1650 2050 0    50   Input ~ 0
Wspd
Text GLabel 1650 2150 0    50   Input ~ 0
Rain
Wire Wire Line
	1650 2150 1750 2150
Wire Wire Line
	1750 2050 1650 2050
Wire Wire Line
	1650 1950 1750 1950
Wire Wire Line
	1750 1850 1650 1850
Text GLabel 1550 3250 0    50   Input ~ 0
5v
Wire Wire Line
	1550 3250 1750 3250
$Comp
L Connector:Conn_01x02_Female J1
U 1 1 607500F8
P 1250 4400
F 0 "J1" H 1278 4376 50  0000 L CNN
F 1 "6-40 VDC" H 1278 4285 50  0000 L CNN
F 2 "TerminalBlock:TerminalBlock_Altech_AK300-2_P5.00mm" H 1250 4400 50  0001 C CNN
F 3 "~" H 1250 4400 50  0001 C CNN
	1    1250 4400
	1    0    0    -1  
$EndComp
Text GLabel 850  4400 0    50   Input ~ 0
BAT
Text GLabel 850  4500 0    50   Input ~ 0
GND
Wire Wire Line
	850  4400 1050 4400
Wire Wire Line
	1050 4500 850  4500
Text GLabel 1000 5600 0    50   Input ~ 0
GND
Text GLabel 1000 4900 0    50   Input ~ 0
BAT
$Comp
L Device:R R6
U 1 1 60758279
P 1150 5050
F 0 "R6" H 1220 5096 50  0000 L CNN
F 1 "??" H 1220 5005 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 1080 5050 50  0001 C CNN
F 3 "~" H 1150 5050 50  0001 C CNN
	1    1150 5050
	1    0    0    -1  
$EndComp
$Comp
L Device:R R7
U 1 1 60758D47
P 1150 5450
F 0 "R7" H 1220 5496 50  0000 L CNN
F 1 "??" H 1220 5405 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 1080 5450 50  0001 C CNN
F 3 "~" H 1150 5450 50  0001 C CNN
	1    1150 5450
	1    0    0    -1  
$EndComp
Text GLabel 1450 5250 2    50   Input ~ 0
Chr
Wire Wire Line
	1000 5600 1150 5600
Wire Wire Line
	1150 5200 1150 5250
Wire Wire Line
	1150 5250 1450 5250
Connection ~ 1150 5250
Wire Wire Line
	1150 5250 1150 5300
Text GLabel 1650 2850 0    50   Input ~ 0
Chr
$Comp
L Switch:SW_SPST SW1
U 1 1 6076EF7F
P 7200 3300
F 0 "SW1" H 7200 3535 50  0000 C CNN
F 1 "SW_SPST" H 7200 3444 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 7200 3300 50  0001 C CNN
F 3 "~" H 7200 3300 50  0001 C CNN
	1    7200 3300
	1    0    0    -1  
$EndComp
$Comp
L Device:R R11
U 1 1 60770A90
P 6800 3450
F 0 "R11" H 6730 3404 50  0000 R CNN
F 1 "10k" H 6730 3495 50  0000 R CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.30x1.75mm_HandSolder" V 6730 3450 50  0001 C CNN
F 3 "~" H 6800 3450 50  0001 C CNN
	1    6800 3450
	-1   0    0    1   
$EndComp
Wire Wire Line
	6800 3300 7000 3300
Text GLabel 7400 3300 2    50   Input ~ 0
3.3v
Text GLabel 6800 3600 3    50   Input ~ 0
GND
Text GLabel 6600 3300 0    50   Input ~ 0
SD_safe
Wire Wire Line
	6600 3300 6800 3300
Connection ~ 6800 3300
Text GLabel 3650 2650 2    50   Input ~ 0
SD_safe
Wire Wire Line
	3650 2650 3350 2650
Text GLabel 2250 6850 2    50   Output ~ 0
b_mic
Text GLabel 750  6850 0    50   Input ~ 0
DAC
$Comp
L pspice:CAP C2
U 1 1 619C613E
P 1850 6850
F 0 "C2" V 1535 6850 50  0000 C CNN
F 1 "0.1 uF" V 1626 6850 50  0000 C CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.33x1.80mm_HandSolder" H 1850 6850 50  0001 C CNN
F 3 "~" H 1850 6850 50  0001 C CNN
	1    1850 6850
	0    1    1    0   
$EndComp
$Comp
L pspice:CAP C1
U 1 1 619D03B4
P 1150 7150
F 0 "C1" H 1500 7100 50  0000 R CNN
F 1 "4.7nF" H 1550 7200 50  0000 R CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.33x1.80mm_HandSolder" H 1150 7150 50  0001 C CNN
F 3 "~" H 1150 7150 50  0001 C CNN
	1    1150 7150
	-1   0    0    1   
$EndComp
$Comp
L Device:R_US R1
U 1 1 619D305E
P 900 6850
F 0 "R1" V 695 6850 50  0000 C CNN
F 1 "33K" V 786 6850 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.30x1.75mm_HandSolder" V 940 6840 50  0001 C CNN
F 3 "~" H 900 6850 50  0001 C CNN
	1    900  6850
	0    1    1    0   
$EndComp
$Comp
L Device:R_US R5
U 1 1 619D406B
P 1500 7100
F 0 "R5" H 1432 7054 50  0000 R CNN
F 1 "1K" H 1432 7145 50  0000 R CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.30x1.75mm_HandSolder" V 1540 7090 50  0001 C CNN
F 3 "~" H 1500 7100 50  0001 C CNN
	1    1500 7100
	-1   0    0    1   
$EndComp
Wire Wire Line
	1050 6850 1150 6850
Wire Wire Line
	1500 6850 1500 6950
Connection ~ 1500 6850
Wire Wire Line
	1500 6850 1600 6850
Wire Wire Line
	1150 6850 1150 6900
Connection ~ 1150 6850
Wire Wire Line
	1150 6850 1500 6850
Wire Wire Line
	1150 7400 1350 7400
Wire Wire Line
	1500 7400 1500 7250
Wire Wire Line
	1350 7550 1350 7400
Connection ~ 1350 7400
Wire Wire Line
	1350 7400 1500 7400
$Comp
L Device:R_US R3
U 1 1 619FE84A
P 1300 6200
F 0 "R3" H 1368 6246 50  0000 L CNN
F 1 "3.3K" H 1368 6155 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.30x1.75mm_HandSolder" V 1340 6190 50  0001 C CNN
F 3 "~" H 1300 6200 50  0001 C CNN
	1    1300 6200
	1    0    0    -1  
$EndComp
$Comp
L Device:R_US R2
U 1 1 619FEEC0
P 1150 6050
F 0 "R2" V 945 6050 50  0000 C CNN
F 1 "10K" V 1036 6050 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.30x1.75mm_HandSolder" V 1190 6040 50  0001 C CNN
F 3 "~" H 1150 6050 50  0001 C CNN
	1    1150 6050
	0    1    1    0   
$EndComp
Text GLabel 1550 6050 2    50   Output ~ 0
ADC
Wire Wire Line
	1300 6050 1550 6050
Connection ~ 1300 6050
Text GLabel 1000 6050 0    50   Input ~ 0
B_spkr
Text GLabel 2700 6050 0    50   Input ~ 0
B_spkr
$Comp
L Device:R_US R8
U 1 1 61A0F946
P 2850 6050
F 0 "R8" V 2645 6050 50  0000 C CNN
F 1 "10K" V 2736 6050 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.30x1.75mm_HandSolder" V 2890 6040 50  0001 C CNN
F 3 "~" H 2850 6050 50  0001 C CNN
	1    2850 6050
	0    1    1    0   
$EndComp
$Comp
L Device:R_US R9
U 1 1 61A100C1
P 3000 6200
F 0 "R9" H 2932 6154 50  0000 R CNN
F 1 "22K" H 2932 6245 50  0000 R CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.30x1.75mm_HandSolder" V 3040 6190 50  0001 C CNN
F 3 "~" H 3000 6200 50  0001 C CNN
	1    3000 6200
	-1   0    0    1   
$EndComp
$Comp
L Device:R_US R10
U 1 1 61A10F19
P 3000 6500
F 0 "R10" H 2932 6454 50  0000 R CNN
F 1 "100K" H 2932 6545 50  0000 R CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.30x1.75mm_HandSolder" V 3040 6490 50  0001 C CNN
F 3 "~" H 3000 6500 50  0001 C CNN
	1    3000 6500
	-1   0    0    1   
$EndComp
Text GLabel 3550 6050 2    50   Input ~ 0
trigger
Wire Wire Line
	3000 6050 3450 6050
Connection ~ 3000 6050
Wire Wire Line
	3000 6650 3000 6700
Wire Wire Line
	3000 6700 3150 6700
$Comp
L Weather-rescue:CJ431 U2
U 1 1 61A26B8F
P 3700 6350
F 0 "U2" H 3578 6421 50  0000 L CNN
F 1 "CJ431" H 3578 6330 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-89-3_Handsoldering" H 3700 6350 50  0001 C CNN
F 3 "" H 3700 6350 50  0001 C CNN
	1    3700 6350
	1    0    0    -1  
$EndComp
Wire Wire Line
	3450 6150 3450 6100
Connection ~ 3450 6050
Wire Wire Line
	3450 6050 3550 6050
Wire Wire Line
	3450 6700 3150 6700
Connection ~ 3150 6700
Connection ~ 3450 6100
Wire Wire Line
	3450 6100 3450 6050
Wire Wire Line
	3250 6350 3000 6350
Connection ~ 3000 6350
Wire Wire Line
	3450 6550 3450 6700
Text GLabel 9700 1750 0    50   Input ~ 0
PTT
Text GLabel 1600 2250 0    50   Input ~ 0
PTT
Text GLabel 9800 2000 0    50   Input ~ 0
B_spkr
Text GLabel 9750 1650 0    50   Input ~ 0
b_mic
Text GLabel 1550 1750 0    50   Input ~ 0
trigger
Text GLabel 1550 1650 0    50   Input ~ 0
ADC
Text GLabel 1600 2350 0    50   Input ~ 0
DAC
Wire Wire Line
	1550 1650 1750 1650
Wire Wire Line
	1750 1750 1550 1750
Wire Wire Line
	1600 2350 1750 2350
Wire Wire Line
	1750 2250 1600 2250
Wire Wire Line
	6850 1000 7200 1000
Wire Wire Line
	7800 2550 7900 2550
$Comp
L Connector:AudioJack3 J11
U 1 1 61BAED88
P 10100 2100
F 0 "J11" H 9820 2033 50  0000 R CNN
F 1 "2.5 mm" H 9820 2124 50  0000 R CNN
F 2 "darty_stuff:2.5mm_Stereo_Jack" H 10100 2100 50  0001 C CNN
F 3 "~" H 10100 2100 50  0001 C CNN
	1    10100 2100
	-1   0    0    1   
$EndComp
$Comp
L Connector:AudioJack3 J5
U 1 1 61BB09D9
P 10100 1650
F 0 "J5" H 9820 1583 50  0000 R CNN
F 1 "3.5 mm" H 9820 1674 50  0000 R CNN
F 2 "darty_stuff:3.5mm_Stereo_Jack" H 10100 1650 50  0001 C CNN
F 3 "~" H 10100 1650 50  0001 C CNN
	1    10100 1650
	-1   0    0    1   
$EndComp
Wire Wire Line
	9700 1750 9900 1750
Wire Wire Line
	9750 2200 9900 2200
Wire Wire Line
	9750 1650 9900 1650
Wire Wire Line
	9800 2000 9900 2000
Wire Wire Line
	7650 2350 7650 2550
Wire Wire Line
	7650 2550 7800 2550
Connection ~ 7800 2550
Wire Wire Line
	7900 2550 7900 2650
Connection ~ 7900 2550
Text GLabel 7400 2050 2    50   Input ~ 0
3.3v
Text GLabel 7400 2150 2    50   Input ~ 0
GND
Text GLabel 7400 2450 2    50   Input ~ 0
SCL
Text GLabel 7400 2550 2    50   Input ~ 0
SDA
$Comp
L Diode:BAV99 D1
U 1 1 61C62554
P 8450 2250
F 0 "D1" V 8496 2329 50  0000 L CNN
F 1 "BAV99" V 8405 2329 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 8450 1750 50  0001 C CNN
F 3 "https://assets.nexperia.com/documents/data-sheet/BAV99_SER.pdf" H 8450 2250 50  0001 C CNN
	1    8450 2250
	0    1    -1   0   
$EndComp
Wire Wire Line
	7800 1950 8450 1950
Connection ~ 8450 1950
Wire Wire Line
	8450 1950 8500 1950
Wire Wire Line
	7900 2550 8450 2550
Wire Wire Line
	8250 2250 8150 2250
Wire Wire Line
	8150 2200 8150 2250
Connection ~ 8150 2250
Wire Wire Line
	8150 2250 7800 2250
Wire Wire Line
	7200 900  7200 1000
Connection ~ 7200 1000
Wire Wire Line
	7200 1150 7200 1100
Connection ~ 7200 1100
Wire Wire Line
	7200 1100 7400 1100
$Comp
L Diode:BAV99 D3
U 1 1 61CE56F8
P 8600 1000
F 0 "D3" V 8646 1079 50  0000 L CNN
F 1 "BAV99" V 8555 1079 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 8600 500 50  0001 C CNN
F 3 "https://assets.nexperia.com/documents/data-sheet/BAV99_SER.pdf" H 8600 1000 50  0001 C CNN
	1    8600 1000
	0    1    -1   0   
$EndComp
Wire Wire Line
	8300 600  8600 600 
Wire Wire Line
	8600 700  8600 600 
Connection ~ 8600 600 
Wire Wire Line
	8600 600  8900 600 
Wire Wire Line
	8400 1000 8300 1000
Connection ~ 8300 1000
Wire Wire Line
	7200 1000 8300 1000
$Comp
L Diode:BAV99 D2
U 1 1 61D33A34
P 8000 1500
F 0 "D2" V 8046 1579 50  0000 L CNN
F 1 "BAV99" V 7955 1579 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 8000 1000 50  0001 C CNN
F 3 "https://assets.nexperia.com/documents/data-sheet/BAV99_SER.pdf" H 8000 1500 50  0001 C CNN
	1    8000 1500
	0    1    -1   0   
$EndComp
Text GLabel 8100 1100 2    50   Input ~ 0
3.3v
Wire Wire Line
	8000 1100 8000 1200
Wire Wire Line
	7400 1800 8000 1800
Wire Wire Line
	7100 1800 7050 1800
Wire Wire Line
	7100 1300 7100 1800
Wire Wire Line
	7100 1800 7400 1800
Connection ~ 7100 1800
Connection ~ 7400 1800
Wire Wire Line
	7400 1650 7400 1800
Wire Wire Line
	7400 1100 7400 1400
Wire Wire Line
	7400 1400 7800 1400
Wire Wire Line
	7800 1400 7800 1500
Connection ~ 7400 1400
Wire Wire Line
	7400 1400 7400 1450
Wire Wire Line
	7400 1100 7550 1100
Connection ~ 7400 1100
Wire Wire Line
	7850 1100 8000 1100
Wire Wire Line
	8100 1100 8000 1100
Connection ~ 8000 1100
Text GLabel 8600 1500 3    50   Input ~ 0
GND
Wire Wire Line
	8600 1500 8600 1300
Text GLabel 1300 6500 3    50   Input ~ 0
GND
Wire Wire Line
	1300 6500 1300 6350
Text GLabel 3150 6850 3    50   Input ~ 0
GND
Wire Wire Line
	3150 6700 3150 6850
Text GLabel 1350 7550 3    50   Input ~ 0
GND
Text GLabel 9750 2200 0    50   Input ~ 0
GND
Wire Wire Line
	2250 6850 2100 6850
Wire Wire Line
	1650 2850 1750 2850
$Comp
L Connector:Conn_01x08_Female J4
U 1 1 61F2B856
P 6600 4850
F 0 "J4" H 6628 4826 50  0000 L CNN
F 1 "SD_card" H 6628 4735 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x08_P2.54mm_Vertical" H 6600 4850 50  0001 C CNN
F 3 "~" H 6600 4850 50  0001 C CNN
	1    6600 4850
	1    0    0    -1  
$EndComp
Wire Wire Line
	6000 5050 6400 5050
Wire Wire Line
	6850 2250 7800 2250
Wire Wire Line
	6850 2350 7650 2350
Wire Wire Line
	7400 2150 6850 2150
Wire Wire Line
	6850 2050 7400 2050
Wire Wire Line
	6850 2450 7400 2450
Wire Wire Line
	6850 2550 7400 2550
Text GLabel 2350 3700 0    50   Input ~ 0
BAT
Text GLabel 4900 4600 2    50   Input ~ 0
GND
$Comp
L Darty_stuff:XL1509-5.0E1 U3
U 1 1 61A8E715
P 4200 4850
F 0 "U3" H 4300 5515 50  0000 C CNN
F 1 "XL1509-5.0E1" H 4300 5424 50  0000 C CNN
F 2 "darty_stuff:SOIC-8_5x4" H 4200 4850 50  0001 C CNN
F 3 "" H 4200 4850 50  0001 C CNN
	1    4200 4850
	1    0    0    -1  
$EndComp
Wire Wire Line
	4650 4450 4800 4450
Wire Wire Line
	4800 4450 4800 4550
Wire Wire Line
	4800 4750 4650 4750
Wire Wire Line
	4650 4650 4800 4650
Connection ~ 4800 4650
Wire Wire Line
	4800 4650 4800 4750
Wire Wire Line
	4650 4550 4800 4550
Connection ~ 4800 4550
Wire Wire Line
	4800 4550 4800 4600
Wire Wire Line
	4900 4600 4800 4600
Connection ~ 4800 4600
Wire Wire Line
	4800 4600 4800 4650
$Comp
L Diode:1N5820 D4
U 1 1 61AAD3AF
P 3500 4750
F 0 "D4" V 3454 4830 50  0000 L CNN
F 1 "1N5820" V 3545 4830 50  0000 L CNN
F 2 "Diode_THT:D_DO-201AD_P15.24mm_Horizontal" H 3500 4575 50  0001 C CNN
F 3 "http://www.vishay.com/docs/88526/1n5820.pdf" H 3500 4750 50  0001 C CNN
	1    3500 4750
	0    1    1    0   
$EndComp
$Comp
L pspice:INDUCTOR L1
U 1 1 61AAE717
P 3150 4550
F 0 "L1" H 3150 4765 50  0000 C CNN
F 1 "68uh/2A" H 3150 4674 50  0000 C CNN
F 2 "darty_stuff:inductor_1004_SMD" H 3150 4550 50  0001 C CNN
F 3 "MWSA1004-680MT" H 3150 4550 50  0001 C CNN
	1    3150 4550
	1    0    0    -1  
$EndComp
Text GLabel 3850 4650 0    50   Input ~ 0
5v
Wire Wire Line
	3850 4650 3950 4650
Text GLabel 3500 5050 3    50   Input ~ 0
GND
Wire Wire Line
	3500 5050 3500 5000
Wire Wire Line
	3400 4550 3500 4550
Wire Wire Line
	3500 4550 3500 4600
Connection ~ 3500 4550
Wire Wire Line
	3500 4550 3950 4550
$Comp
L Device:CP1 C5
U 1 1 61AF2717
P 2900 4750
F 0 "C5" H 3015 4796 50  0000 L CNN
F 1 "180uf/35V" H 3015 4705 50  0000 L CNN
F 2 "Capacitor_THT:CP_Radial_D8.0mm_P3.50mm" H 2900 4750 50  0001 C CNN
F 3 "~" H 2900 4750 50  0001 C CNN
	1    2900 4750
	1    0    0    -1  
$EndComp
Wire Wire Line
	2900 4550 2900 4600
Connection ~ 2900 4550
Wire Wire Line
	2900 4900 2900 5000
Wire Wire Line
	2900 5000 3500 5000
Connection ~ 3500 5000
Wire Wire Line
	3500 5000 3500 4900
$Comp
L Device:CP1 C6
U 1 1 61B05FB9
P 3000 4000
F 0 "C6" H 3115 4046 50  0000 L CNN
F 1 "470uf/35V" H 3115 3955 50  0000 L CNN
F 2 "Capacitor_THT:CP_Radial_D10.0mm_P5.00mm" H 3000 4000 50  0001 C CNN
F 3 "~" H 3000 4000 50  0001 C CNN
	1    3000 4000
	1    0    0    -1  
$EndComp
$Comp
L Device:C C7
U 1 1 61B0B249
P 3750 4000
F 0 "C7" H 3865 4046 50  0000 L CNN
F 1 "105" H 3865 3955 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.33x1.80mm_HandSolder" H 3788 3850 50  0001 C CNN
F 3 "~" H 3750 4000 50  0001 C CNN
	1    3750 4000
	1    0    0    -1  
$EndComp
Text GLabel 3500 4250 3    50   Input ~ 0
GND
Wire Wire Line
	3000 4150 3000 4200
Wire Wire Line
	3000 4200 3500 4200
Wire Wire Line
	3500 4200 3500 4250
Wire Wire Line
	3500 4200 3750 4200
Wire Wire Line
	3750 4200 3750 4150
Connection ~ 3500 4200
Wire Wire Line
	2800 3700 3000 3700
Wire Wire Line
	3000 3700 3000 3850
Wire Wire Line
	3000 3700 3750 3700
Wire Wire Line
	3750 3700 3750 3850
Connection ~ 3000 3700
Wire Wire Line
	3750 3700 4050 3700
Wire Wire Line
	4050 3700 4050 4200
Wire Wire Line
	4050 4200 3850 4200
Wire Wire Line
	3850 4200 3850 4450
Wire Wire Line
	3850 4450 3950 4450
Connection ~ 3750 3700
Wire Wire Line
	2500 3700 2350 3700
Wire Wire Line
	1000 4900 1150 4900
$Comp
L Mechanical:MountingHole_Pad H2
U 1 1 61B73766
P 4900 5850
F 0 "H2" H 5000 5899 50  0000 L CNN
F 1 "MountingHole_Pad" H 5000 5808 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3_Pad_Via" H 4900 5850 50  0001 C CNN
F 3 "~" H 4900 5850 50  0001 C CNN
	1    4900 5850
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole_Pad H1
U 1 1 61B74266
P 4700 5600
F 0 "H1" H 4800 5649 50  0000 L CNN
F 1 "MountingHole_Pad" H 4800 5558 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3_Pad_Via" H 4700 5600 50  0001 C CNN
F 3 "~" H 4700 5600 50  0001 C CNN
	1    4700 5600
	1    0    0    -1  
$EndComp
Text GLabel 4900 5950 3    50   Input ~ 0
GND
Text GLabel 4700 5700 3    50   Input ~ 0
GND
$Comp
L Diode:1N5819 D5
U 1 1 61B79CB5
P 2650 3700
F 0 "D5" H 2650 3483 50  0000 C CNN
F 1 "1N5819" H 2650 3574 50  0000 C CNN
F 2 "Diode_THT:D_DO-41_SOD81_P10.16mm_Horizontal" H 2650 3525 50  0001 C CNN
F 3 "http://www.vishay.com/docs/88525/1n5817.pdf" H 2650 3700 50  0001 C CNN
	1    2650 3700
	-1   0    0    1   
$EndComp
Wire Wire Line
	2600 4550 2900 4550
Text GLabel 2600 4550 0    50   Input ~ 0
5v
$Comp
L Connector:Conn_01x04_Female J8
U 1 1 61BF0FFE
P 8650 4300
F 0 "J8" H 8678 4276 50  0000 L CNN
F 1 "Snow" H 8678 4185 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x04_P2.54mm_Vertical" H 8650 4300 50  0001 C CNN
F 3 "~" H 8650 4300 50  0001 C CNN
	1    8650 4300
	1    0    0    -1  
$EndComp
Text GLabel 8200 4200 0    50   Input ~ 0
3.3v
Text GLabel 8200 4500 0    50   Input ~ 0
GND
Wire Wire Line
	8200 4200 8450 4200
Wire Wire Line
	8450 4500 8200 4500
Text GLabel 8200 4300 0    50   Input ~ 0
Trig
Text GLabel 8200 4400 0    50   Input ~ 0
Echo
Wire Wire Line
	8200 4400 8450 4400
Wire Wire Line
	8450 4300 8200 4300
Text GLabel 3500 2850 2    50   Input ~ 0
Trig
Text GLabel 3500 2750 2    50   Input ~ 0
Echo
Wire Wire Line
	3350 2750 3500 2750
Wire Wire Line
	3500 2850 3350 2850
Text Notes 8700 4550 0    50   ~ 0
HC-SR04
$Comp
L Connector:Conn_01x04_Female J12
U 1 1 61C3E305
P 8550 5350
F 0 "J12" H 8578 5326 50  0000 L CNN
F 1 "GPS" H 8578 5235 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x04_P2.54mm_Vertical" H 8550 5350 50  0001 C CNN
F 3 "~" H 8550 5350 50  0001 C CNN
	1    8550 5350
	1    0    0    -1  
$EndComp
Text GLabel 3550 1750 2    50   Input ~ 0
TX
Wire Wire Line
	3550 1750 3350 1750
Text GLabel 3550 1850 2    50   Input ~ 0
RX
Wire Wire Line
	3550 1850 3350 1850
Text GLabel 8050 5350 0    50   Input ~ 0
RX
Text GLabel 8050 5450 0    50   Input ~ 0
TX
Text GLabel 8050 5550 0    50   Input ~ 0
GND
Text GLabel 8050 5250 0    50   Input ~ 0
3.3v
Wire Wire Line
	8050 5250 8350 5250
Wire Wire Line
	8350 5350 8050 5350
Wire Wire Line
	8050 5450 8350 5450
Wire Wire Line
	8350 5550 8050 5550
Text Notes 8600 5600 0    50   ~ 0
GY-NEO6MV2
Text Notes 10300 3450 0    50   ~ 0
TSL2561
$Comp
L Device:R R4
U 1 1 61D82F34
P 8400 3500
F 0 "R4" H 8470 3546 50  0000 L CNN
F 1 "??" H 8470 3455 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 8330 3500 50  0001 C CNN
F 3 "~" H 8400 3500 50  0001 C CNN
	1    8400 3500
	1    0    0    -1  
$EndComp
$Comp
L Device:R R15
U 1 1 61D84BD9
P 8650 3500
F 0 "R15" H 8720 3546 50  0000 L CNN
F 1 "??" H 8720 3455 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 8580 3500 50  0001 C CNN
F 3 "~" H 8650 3500 50  0001 C CNN
	1    8650 3500
	1    0    0    -1  
$EndComp
Text GLabel 8400 3200 1    50   Input ~ 0
3.3v
Text GLabel 8650 3200 1    50   Input ~ 0
3.3v
Text GLabel 8400 3800 3    50   Input ~ 0
SCL
Text GLabel 8650 3800 3    50   Input ~ 0
SDA
Wire Wire Line
	8650 3800 8650 3650
Wire Wire Line
	8400 3650 8400 3800
Wire Wire Line
	8400 3200 8400 3350
Wire Wire Line
	8650 3200 8650 3350
$Comp
L Mechanical:MountingHole_Pad H3
U 1 1 61DBD995
P 5100 6100
F 0 "H3" H 5200 6149 50  0000 L CNN
F 1 "MountingHole_Pad" H 5200 6058 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3_Pad_Via" H 5100 6100 50  0001 C CNN
F 3 "~" H 5100 6100 50  0001 C CNN
	1    5100 6100
	1    0    0    -1  
$EndComp
Text GLabel 5100 6200 3    50   Input ~ 0
GND
$EndSCHEMATC
