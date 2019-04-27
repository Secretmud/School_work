/*
    _,.---._      ,-,--.             _,.---._           ___      ,----.  ,--.--------.
  ,-.' , -  `.  ,-.'-  _\  _.-.    ,-.' , -  `.  .-._ .'=.'\  ,-.--` , \/==/,  -   , -\
 /==/_,  ,  - \/==/_ ,_.'.-,.'|   /==/_,  ,  - \/==/ \|==|  ||==|-  _.-`\==\.-.  - ,-./
|==|   .=.     \==\  \  |==|, |  |==|   .=.     |==|,|  / - ||==|   `.-. `--`\==\- \
|==|_ : ;=:  - |\==\ -\ |==|- |  |==|_ : ;=:  - |==|  \/  , /==/_ ,    /      \==\_ \
|==| , '='     |_\==\ ,\|==|, |  |==| , '='     |==|- ,   _ |==|    .-'       |==|- |
 \==\ -    ,_ //==/\/ _ |==|- `-._\==\ -    ,_ /|==| _ /\   |==|_  ,`-._      |==|, |
  '.='. -   .' \==\ - , /==/ - , ,/'.='. -   .' /==/  / / , /==/ ,     /      /==/ -/
    `--`--''    `--`---'`--`-----'   `--`--''   `--`./  `--``--`-----``       `--`--`
Gruppe 7:
    Oppgave:
        Vanndispenser for dyr(med litt forandring)
*/

#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <HCSR04.h>
#define menu_len 16
//Pin declaration

int trigger[] = {
    5, 7, 9
};
int echo[] = {
    6, 8, 10
};


int btn = 2;
int dt = 3;
int clk = 4;
int motor1 = 11;
int motor2 = 12;
int led = 13;

//Used by the ultrasonic sensor
bool calibrated = false;
long distance;
long duration;

//Rotatry encoder
int counter = 0;
int dtState;
int dtLastState;

//Arrays
char menu[][menu_len] = {{"Calibration"}, {"Mix: 10/90"},
                         {"Mix: 20/80"}, {"Mix: 30/70"}, 
                         {"Random drink"}, {"System info"}
                        };
char sens[][6] = {{"Front:"}, {"Tank1:"}, {"Tank2:"}};
int out[] = { 5, 7, 9, 11, 12, 13};
int in[] = {3, 4, 6, 8, 10};

//function prototypes
float calibration(float offset, int x);
float calibration_tank(float offset);
void calibration_print(int height, int tank1, int);
void menu_fields(int x);
void menu_switch(int counter, double height, float offset, double box1, double box2);
void game(float height);
void lcd_line_clear(int start, int collumn, int rows);

//Initializing the 16x2 LCD with a I2C connector
LiquidCrystal_I2C lcd(0x27, 16, 2);

//Initializing the sensors
UltraSonicDistanceSensor front(trigger[0], echo[0]);
UltraSonicDistanceSensor tank1(trigger[1], echo[1]);
UltraSonicDistanceSensor tank2(trigger[2], echo[2]);

void setup() {
    pinMode(btn, INPUT_PULLUP);
    for (int x = 0; x < sizeof(in); x++)
        pinMode(in[x], INPUT);
    for (int i = 0; i < sizeof(out); i++)
        pinMode(out[i], OUTPUT);
    lcd.begin();
    lcd.backlight();
    lcd.clear();
    dtLastState = digitalRead(dt);
}

void loop() {
    double height;
    double box1;
    double box2;
    int height_t = 7;
    float offset = 0;
    bool set = false;
    bool measurement = true;
    int menu_size = sizeof(menu) / menu_len - 1;
    if (!calibrated) {
        lcd.clear();
        while (measurement) {
            lcd.clear();
            lcd.print("Paper over cup.");
            lcd.setCursor(0,1);
            lcd.print("press button");
            if (digitalWrite(btn) == LOW)
                measurement = false
        } 
         height = calibration(offset, 0);
         box1 = calibration(offset, 1);
         box2 = calibration(offset, 2);
        calibration_print(height, box1, box2);

    } else {
        dtState = digitalRead(dt);
        if (dtState != dtLastState) {
            if (digitalRead(clk) != dtState) {
                counter++;
            } else {
                counter--;
            }
            if (counter < 0)
                counter = 0;
            if (counter > menu_size)
                counter = menu_size;
           menu_fields(counter);
        }
        //Checking for "LOW" since pinMode for btn is INPUT_PULLUP
        if (digitalRead(btn) == LOW) { 
           menu_switch(counter, height, offset, box1, box2);
        }
        dtLastState = dtState;
    }
}

void menu_switch(int counter, double height, float offset, double tank1, double tank2) {
    switch (counter) {
        case 0:
            if (calibrated) {
                lcd.clear();
                do
                {
                    
                } while (digitalWrite != LOW);
                
                double height = front.measureDistanceCm();
                double box1 = tank1.measureDistanceCm();
                double box2 = tank2.measureDistanceCm();
                calibration_print(height, box1, box2);
            }
            break;
        case 1:
            programs(height, true, 10, offset);
            break;
        case 2:
            programs(height, true, 20, offset);
            break;
        case 3:
            programs(height, true, 30, offset);
            break;
        case 4:
            programs(height, false, 0, offset);
            break;
        case 5:
            calibration_print(height, tank1, tank2);
            break;
        default:
            break;
    }
}

void menu_fields(int counter) {
    lcd.clear();
    lcd.print(">");
    lcd.setCursor(1, 0);
    lcd.print(menu[counter]);
    lcd.setCursor(1, 1);
    lcd.print(menu[counter + 1]);
}

void programs(float height, bool set, float percentage, int offset) {
    float dH;
    float dist = 0;
    if (set) {
        lcd.clear();
        dist = (percentage / 100) * height;
        dH = height - dist;
        game_print(height, dist);
        delay(1000);
        lcd.clear();
        while(1) {
            if (digitalRead(btn) == LOW)
                break;
            if (front.measureDistanceCm() >= dH && tank1.measureDistanceCm() >= 20) {
                lcd.clear();
                lcd.print(calibration(offset, 0));
                digitalWrite(motor1, HIGH);
                delay(50);
             } else {
                digitalWrite(motor1, LOW);
                break;
            }
        }
    } else {
        float p_x = rand() % 100;
        dist = (p_x / 100) * height;
        dH = height - dist;
        game_print(height, dist);
        game_print(height, dist);
        while(1) {
            if (digitalRead(btn) == LOW)
                break;
            if (front.measureDistanceCm() >= dH && tank1.measureDistanceCm() >= 20) {
                lcd.clear();
                lcd.print(calibration(offset, 0));
                digitalWrite(motor1, HIGH);
                delay(50);
             } else {
                digitalWrite(motor1, LOW);
                break;
            }
        }
        
    }
}

void game_print(int height, int x) {
    lcd.clear();
    lcd.print("height cm:");
    lcd.setCursor(13, 0);
    lcd.print(height);
    lcd.setCursor(0, 1);
    lcd.print("Liquid1 cm:");
    lcd.setCursor(13, 1);
    lcd.print(x);
}

void calibration_print(int height, int tank1, int tank2) {
    int arr[] = {height, tank1, tank2};
    lcd.clear();
    lcd.print("Height:");
    for (int i = 0; i < 3; i++) {
        lcd_line_clear(6, 16, 1);
        lcd.setCursor(0, 1);
        lcd.print(sens[i]);
        lcd.setCursor(7, 1);
        lcd.print(arr[i]);
        lcd.setCursor(7 + sizeof(arr[i]) , 1);
        lcd.print("cm");
        delay(600);
    }
}

void lcd_line_clear(int start, int rows, int collumn) {
    for (int x = start; x < rows; x++) {
        lcd.setCursor(x, collumn);
        lcd.print(" ");
        delay(10);
    }
}
