#include "Arduino.h"
#include "DCMDriverL298.h"
#include "LiquidCrystal_PCF8574.h"
#include "HX711.h"

// Pin definitions for motors
#define MOTOR_INT1  4
#define MOTOR_ENB 6
#define MOTOR_INT2  7
#define MOTOR_ENA 5
#define MOTOR_INT3  8
#define MOTOR_INT4  10
// Initialization
DCMDriverL298 dcMotorDriverL298(MOTOR_ENA, MOTOR_INT1, MOTOR_INT2, MOTOR_ENB, MOTOR_INT3, MOTOR_INT4);
 
// Pin definitions for scale
#define SCALE_DAT 12
#define SCALE_CLK 11
 
// Initialization
HX711 scale(SCALE_DAT, SCALE_CLK);
#define calibration_factor 2280 //This value is obtained using the SparkFun_HX711_Calibration sketch https://learn.sparkfun.com/tutorials/load-cell-amplifier-hx711-breakout-hookup-guide?_ga=2.77038550.2126325781.1526891300-303225217.1493631967
 
// Definitions for LCD
#define LCD_ADDRESS 0x27
// Define LCD characteristics
#define LCD_ROWS 2
#define LCD_COLUMNS 16
#define SCROLL_DELAY 150
#define BACKLIGHT 255
// Initialization
LiquidCrystal_PCF8574 lcdI2C;
 

void setup() {
    Serial.begin(9600);
    while (!Serial);
    lcdI2C.begin(LCD_COLUMNS, LCD_ROWS, LCD_ADDRESS, BACKLIGHT);
    scale.set_scale(calibration_factor);
    scale.tare(); //Assuming there is no weight on the scale at start up, reset the scale to 0
}
 
void loop() {
    float scaleUnits = scale.get_units() * -1;
    delay(300);
    Serial.println(scaleUnits);
 
    if (scaleUnits >= 70 && scaleUnits <= 150) {
        lcdI2C.clear();
        lcdI2C.print("Proccessing...");
        dcMotorDriverL298.setMotorA(255, 0);
        dcMotorDriverL298.setMotorB(255, 0);
        delay(2400);
        lcdI2C.clear();
        lcdI2C.print("Take your drink");
        dcMotorDriverL298.stopMotors();
        delay(10000);
    }
    
else if (scaleUnits <= 70) {
        lcdI2C.clear();
        lcdI2C.print("Place a glass");
        lcdI2C.setCursor(0,1);
        lcdI2C.print("on a scale");
        dcMotorDriverL298.stopMotors();
    }
}
