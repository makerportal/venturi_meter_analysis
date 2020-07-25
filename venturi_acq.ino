// Pressure differential from MPXV7002DP
// with DC Fan controller via PWM, and a
// BME280 taking pressure, temp measurements
//
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

Adafruit_BME280 bme; // I2C for BME280

// pin allocations for MPXV7002DP and fan PWM
int mpxv_pin = A0; // mpxv analog input pin
int fan_pin = 3; // PWM pin for dc fan

float V_0 = 3.3; // supply voltage to the pressure sensors

// parameters for averaging and offset
int offset = 0;
int offset_size = 10;
int mean_size = 20;
int zero_span = 2;

// dc fan pwm value
int pwm_val = 0; // PWM value for controlling fan speed
int iter_change = 0; // iterations to keep fan at specific speed
bool max_val = false; // when max PWM is reached (255), this sets it back to 0
int pwm_min = 0; // min pwm for loop
int pwm_max = 255; // max pwm for loop

// setup and calculate offset
void setup() {
  analogReference(EXTERNAL); // tell the Arduino to use 3.3V instead of 5V for analog values
  pinMode(fan_pin,OUTPUT); // setup PWM for fan
 
  digitalWrite(fan_pin,0); // make sure fan is off
  delay(2000);
  Serial.begin(9600); // start serial port
  delay(100);
  Serial.println("Acquisition Start"); // lets the data acquisition know you restarted
  Serial.println("PWM Value[0-255],dP[Pa],Pressure [Pa],Temp. [C]"); // header
  for (int ii=0;ii<offset_size;ii++){
    offset += analogRead(mpxv_pin)-(1023/2); // offset for MPXV7002DP
  }
  offset /= offset_size;
 
  unsigned status;
  status = bme.begin(); // make sure BME280 was wired/is working properly  
    if (!status) {
        Serial.println("No BME280 sensor available");
        while (1); // if no BME280, stay in infinite loop
    }
}

void loop() {
  float adc_avg = 0; float delta_P = 0.0; // preallocations for variables
 
// average a few ADC readings for stability
  for (int ii=0;ii<mean_size;ii++){
    adc_avg+= analogRead(mpxv_pin)-offset;
  }
  adc_avg/=mean_size; // average ADC value for MPXV7002DP
 
  // make sure if the ADC reads outside of 512+-zero_span (this increases stability around 0 pressure)
  if (adc_avg>512-zero_span and adc_avg<512+zero_span){
  } else{
    delta_P = 5000.0*((adc_avg/1023.0)-0.5); // actual differential pressure
  }
  Serial.print(pwm_val); // print PWM value
  Serial.print(","); // comma separate
 
  Serial.println(delta_P); // print delta P
  Serial.print(","); // comma separate
 
  Serial.print(bme.readPressure()); // print barometric pressure
  Serial.print(","); // comma separate
 
  Serial.println(bme.readTemperature()); // print temperature

  delay(100); // delay for stability

  // iterating the PWM values
  if (iter_change>20) {
    pwm_val+=10; // this can be changed to desired increase in PWM
    if (pwm_val>=pwm_max and max_val){
      pwm_val = pwm_min; // if we're at max, go to pwm_min
      max_val = false;
    }
    if (pwm_val>pwm_max and pwm_val!=pwm_max){
      pwm_val = pwm_max; // if we overshoot, go to pwm_max
      max_val = true;
    }
    iter_change = 0; // reset to get multiple points at PWM value
    analogWrite(fan_pin,pwm_val); // write the actual PWM value
    delay(1000);
  }
  iter_change+=1; // increase iter every loop
}
