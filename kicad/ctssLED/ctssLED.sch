EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "CTSS LED Encoder with Uno"
Date "2020-10-11"
Rev "v01"
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 "Author: dtk"
$EndDescr
$Comp
L Arduino:Arduino_Uno_Shield XA?
U 1 1 5F8447AC
P 4600 3150
F 0 "XA?" H 4600 4537 60  0000 C CNN
F 1 "Arduino_Uno_Shield" H 4600 4431 60  0000 C CNN
F 2 "" H 6400 6900 60  0001 C CNN
F 3 "https://store.arduino.cc/arduino-uno-rev3" H 6400 6900 60  0001 C CNN
	1    4600 3150
	1    0    0    -1  
$EndComp
$Comp
L ctssLED:CommSpec-SS32 SS32?
U 1 1 5F84FDFC
P 1800 3150
F 0 "SS32?" H 1800 2635 50  0000 C CNN
F 1 "CommSpec-SS32" H 1800 2726 50  0000 C CNN
F 2 "" H 1650 3250 50  0001 C CNN
F 3 "" H 1650 3250 50  0001 C CNN
	1    1800 3150
	-1   0    0    1   
$EndComp
$Comp
L ctssLED:Modtronics-Dev7 Dev7?
U 1 1 5F851C5B
P 7900 2600
F 0 "Dev7?" H 7825 3415 50  0000 C CNN
F 1 "Modtronics-Dev7" H 7825 3324 50  0000 C CNN
F 2 "" H 7850 2450 50  0001 C CNN
F 3 "" H 7850 2450 50  0001 C CNN
	1    7900 2600
	1    0    0    -1  
$EndComp
Wire Wire Line
	3300 3300 2900 3300
Wire Wire Line
	2900 3300 2900 2950
Wire Wire Line
	2900 2950 2250 2950
Wire Wire Line
	3300 3200 2950 3200
Wire Wire Line
	2950 3200 2950 3050
Wire Wire Line
	2950 3050 2250 3050
Wire Wire Line
	3300 3100 2250 3100
Wire Wire Line
	2250 3100 2250 3150
Wire Wire Line
	3300 3000 3050 3000
Wire Wire Line
	3050 3000 3050 3250
Wire Wire Line
	3050 3250 2250 3250
Wire Wire Line
	3300 2900 3100 2900
Wire Wire Line
	3100 2900 3100 3350
Wire Wire Line
	3100 3350 2250 3350
Wire Wire Line
	3300 3700 2450 3700
Wire Wire Line
	2450 3700 2450 2850
Wire Wire Line
	2450 2850 2250 2850
Wire Wire Line
	5900 3200 6300 3200
Wire Wire Line
	6300 3200 6300 2600
Wire Wire Line
	6300 2600 6850 2600
Wire Wire Line
	5900 3100 6150 3100
Wire Wire Line
	6150 3100 6150 3600
Wire Wire Line
	6150 3600 7950 3600
Wire Wire Line
	7950 3600 7950 2800
Wire Wire Line
	7950 2800 8200 2800
Wire Wire Line
	5900 3000 6200 3000
Wire Wire Line
	6200 3000 6200 3550
Wire Wire Line
	6200 3550 9200 3550
Wire Wire Line
	9200 3550 9200 2700
Wire Wire Line
	9200 2700 8800 2700
Wire Wire Line
	5900 2900 6250 2900
Wire Wire Line
	6250 2900 6250 3500
Wire Wire Line
	6250 3500 7650 3500
Wire Wire Line
	5900 2700 6250 2700
Wire Wire Line
	6250 2700 6250 2500
Wire Wire Line
	6250 2500 6850 2500
Wire Wire Line
	7650 2800 7450 2800
Wire Wire Line
	7650 2800 7650 3500
Wire Wire Line
	5900 2600 6100 2600
Wire Wire Line
	6100 2600 6100 3450
Wire Wire Line
	6100 3450 7600 3450
Wire Wire Line
	7600 3450 7600 3000
Wire Wire Line
	7600 3000 7450 3000
Wire Wire Line
	5900 2500 6050 2500
Wire Wire Line
	6050 2500 6050 3650
Wire Wire Line
	6050 3650 9100 3650
Wire Wire Line
	9100 3650 9100 3000
Wire Wire Line
	9100 3000 8800 3000
Wire Wire Line
	5900 2400 6400 2400
Wire Wire Line
	6400 2400 6400 3400
Wire Wire Line
	6400 3400 8050 3400
Wire Wire Line
	8050 3400 8050 2900
Wire Wire Line
	8050 2900 8200 2900
Wire Wire Line
	5900 2300 6450 2300
Wire Wire Line
	6450 3350 9150 3350
Wire Wire Line
	9150 3350 9150 2900
Wire Wire Line
	9150 2900 8800 2900
Wire Wire Line
	5900 2200 6000 2200
Wire Wire Line
	6000 2200 6000 3700
Wire Wire Line
	6000 3700 7700 3700
Wire Wire Line
	7700 3700 7700 2600
Wire Wire Line
	7700 2600 7450 2600
Text Label 6650 2800 0    50   ~ 0
SW1
Text Label 7500 2800 0    50   ~ 0
SW2
Text Label 6650 2600 0    50   ~ 0
SW3
Text Label 7500 2600 0    50   ~ 0
SW4
Text Label 8850 2700 0    50   ~ 0
SER
Text Label 8000 2800 0    50   ~ 0
SCK
Text Label 6650 2500 0    50   ~ 0
ACK
Text Label 8850 2900 0    50   ~ 0
SEL1
Text Label 8000 2900 0    50   ~ 0
SEL2
Text Label 8850 3000 0    50   ~ 0
SEL3
Text Label 7500 3000 0    50   ~ 0
SEL4
Wire Wire Line
	3300 4100 3050 4100
Wire Wire Line
	3050 4100 3050 4650
Wire Wire Line
	3050 4650 7850 4650
Wire Wire Line
	7850 4650 7850 2500
Wire Wire Line
	7850 2500 8200 2500
Wire Wire Line
	3300 3800 2950 3800
Wire Wire Line
	2950 3800 2950 4700
Wire Wire Line
	2950 4700 7900 4700
Wire Wire Line
	7900 4700 7900 2400
Wire Wire Line
	7900 2400 8200 2400
Wire Wire Line
	5900 2800 6850 2800
Wire Wire Line
	6450 2300 6450 3350
$EndSCHEMATC
