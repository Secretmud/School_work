#include <LiquidCrystal_I2C.h>
#include <Wire.h>

int trigger = 9;
int echo = 10;
int btn = 2;
int dt = 3;
int clk = 4;
//Used by the ultrasonic sensor
bool calibrated = false;
long distance;
long duration;
//Rotatry encoder
int counter = 0;
int dtState;
int dtLastState;
//Menu arrays
char menu[][16] = {{"Calibration"}, {"Drink!!!"}};
char name[] = {"Drink'o'matic!"};
int calibration();
void menu_fields(int x);
void game(int height);

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup()
{
    pinMode(btn, INPUT_PULLUP);
    pinMode(dt, INPUT);
    pinMode(clk, INPUT);
    pinMode(trigger, OUTPUT);
    pinMode(echo, INPUT);
    lcd.begin();
    lcd.backlight();
    Serial.begin(9600);
    dtLastState = digitalRead(dt);
    lcd.clear();
    for (int x = 0; x < 14; x++) {
        lcd.setCursor(x+1,0);
        lcd.print(name[x]);
        delay(200);
    }
    lcd.setCursor(6,1);
    lcd.print("v0.1");
    delay(1500);
}

void loop()
{
    if (!calibrated) {
        lcd.clear();
        int height = calibration();
        lcd.print("Height:");
        lcd.setCursor(0,1);
        lcd.print(height);
        
    } else {
        dtState = digitalRead(dt);
        if (dtState != dtLastState) {
            if (digitalRead(clk) != dtState) {
                counter++;
            } else {
                counter--;
            }
            if ( counter < 0)
                counter = 0;
            if ( counter > 1)
                counter = 1;
            Serial.print("Position: ");
            Serial.println(counter);
            menu_fields(counter);
        }
        if (digitalRead(btn) == LOW) {
            Serial.println(counter);
            switch(counter) {
                case 0:
                    if (calibrated) {
                        lcd.clear();
                        int height = calibration();
                        lcd.print("Height:");
                        lcd.setCursor(0,1);
                        lcd.print(height);
                    }
                    break;
                case 1:
                    Serial.println("This");
                    lcd.clear();
                    lcd.print("The game");
                    break;
                default:
                    break;
            }
        }       
        dtLastState = dtState;
    }
}

int calibration()
{
    int height = 0;
    digitalWrite(trigger, LOW);
    delayMicroseconds(2);
    digitalWrite(trigger, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigger, LOW);
    duration = pulseIn(echo, HIGH);
    distance= duration*0.034/2;
    Serial.print("Distance: ");
    Serial.println(distance);
    calibrated = true;
    return distance;
}

void menu_fields(int x)
{
    lcd.clear();
    lcd.print(">");
    lcd.setCursor(1,0);
    lcd.print(menu[x]);
    lcd.setCursor(0,1);
    lcd.print(menu[x+1]);
}

void game(int height)
{

}
