# Arduino Venturi Meter Analysis

The Arduino code controls a 12V DC fan via PWM from 0 - 100%, while simultaneously measuring the differential pressure of the inlet and throat using the MPXV7002DP sensor, and the barometric pressure and ambient temperature using the BME280 sensor.

The Python code acquires data in real-time from the Arduino board and saves the data into a comma-separated value (CSV) file. The differential pressure and barometric pressure are subsequently plotted as a function of PWM.
