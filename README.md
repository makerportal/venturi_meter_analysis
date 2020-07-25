# Arduino Venturi Meter Analysis

Codes that accompany the experiments outlined in: 

[The Design and Characterization of a 3D Printed Venturi Tube](https://makersportal.com/s/the_design_and_characterization_of_a_3d_printed_venturi_tube_joshua_hrisko.pdf)

#
The Arduino code controls a 12V DC fan via PWM from 0 - 100%, while simultaneously measuring the differential pressure of the inlet and throat using the MPXV7002DP sensor, and the barometric pressure and ambient temperature using the BME280 sensor.

![](https://static1.squarespace.com/static/59b037304c0dbfb092fbe894/t/5f1b8602637dc12007ef1306/1595639317097/arduino_dc_fan_bme280_mpxv7002dp_wiring_12V+copy.png?format=1500w)

##
The Python code acquires data in real-time from the Arduino board and saves the data into a comma-separated value (CSV) file. The differential pressure and barometric pressure are subsequently plotted as a function of PWM.
##
![](https://static1.squarespace.com/static/59b037304c0dbfb092fbe894/t/5f1b74707b6273648c202b45/1595634868218/venturi_meter_setup_image.JPG?format=1500w)
