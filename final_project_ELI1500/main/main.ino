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
//Variables needed for the program to run
int height_t = 7;
float dist = 0;
float height = 0;
float tank1 = 0;
float tank2 = 0;
float offset = 0;
bool set = false;

//Used by the ultrasonic sensor
bool calibrated = false;
long distance;
long duration;

//Rotatry encoder
int counter = 0;
int dtState;
int dtLastState;

//Arrays
char menu[][menu_len] = {
    {"Calibration"},
    {"Mix: 10/90"},
    {"Mix: 20/80"},
    {"Mix: 30/70"},
    {"Random drink"},
    {"System info"}
};
char name[] = {
    "Drink'o'matic!"
};
char sens[][6] = {
    {"Front:"},
    {"Tank1:"},
    {"Tank2:"}
};

//function prototypes
float calibration(float offset, int x);
float calibration_tank(float offset);
float distance_sensor();
void calibration_print();
void menu_fields(int x);
void menu_switch(int x);
void game(float height);
void info(float height, float offset);
void lcd_line_clear(int start, int collumn, int rows);

//Initializing the 16x2 LCD with a I2C connector
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
    pinMode(led, OUTPUT);
    pinMode(motor1, OUTPUT);
    pinMode(motor2, OUTPUT);
    pinMode(btn, INPUT_PULLUP);
    pinMode(dt, INPUT);
    pinMode(clk, INPUT);
    for (int pin_set = 0; pin_set < 2; pin_set++) {
        pinMode(trigger[pin_set], OUTPUT);
        pinMode(echo[pin_set], INPUT);
    }
    lcd.begin();
    lcd.backlight();
    lcd.clear();
    dtLastState = digitalRead(dt);/*
    for (int x = 1; x < 15; x++) {
        lcd.setCursor(x, 0);
        lcd.print(name[x]);
        delay(200);
    }
    lcd.setCursor(6, 1);
    lcd.print("v0.1");
    delay(1000);*/
}

void loop() {
    int menu_size = sizeof(menu) / menu_len - 1;
    if (!calibrated) {
        lcd.clear();
        height = calibration(offset, 0);
        tank1 = calibration(offset, 1);
        tank2 = calibration(offset, 2);
        calibration_print();
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
        if (digitalRead(btn) == LOW) {
           menu_switch(counter);
        }
        dtLastState = dtState;
    }
}

void menu_switch(int counter) {
    switch (counter) {
        case 0:
            if (calibrated) {
                lcd.clear();
                height = calibration(offset, 0);
                tank1 = calibration(offset, 1);
                tank2 = calibration(offset, 2);
                calibration_print();
            }
            break;
        case 1:
            game(height, true, 10, offset);
            break;
        case 2:
            game(height, true, 20, offset);
            break;
        case 3:
            game(height, true, 30, offset);
            break;
        case 4:
            game(height, false, 0, offset);
            break;
        case 5:
            info(height, offset);
            break;
        default:
            break;
    }
}

float calibration(float offset, int x) {
    digitalWrite(trigger[x], LOW);
    delayMicroseconds(2);
    digitalWrite(trigger[x], HIGH);
    delayMicroseconds(10);
    digitalWrite(trigger[x], LOW);
    duration = pulseIn(echo[x], HIGH);
    distance = duration/29/2;
    calibrated = true;
    return distance - offset;
}

float distance_sensor() {
    digitalWrite(trigger[0], LOW);
    delayMicroseconds(2);
    digitalWrite(trigger[0], HIGH);
    delayMicroseconds(10);
    digitalWrite(trigger[0], LOW);
    duration = pulseIn(echo[0], HIGH);
    distance = (duration)/29/2;
    return distance;
}

void menu_fields(int current) {
    lcd.clear();
    lcd.print(">");
    lcd.setCursor(1, 0);
    lcd.print(menu[current]);
    lcd.setCursor(1, 1);
    lcd.print(menu[current + 1]);
}

void game(float height, bool set, float percentage, int offset) {
    float dH;
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
            if (distance_sensor() >= dH) {
                lcd.clear();
                lcd.print(distance_sensor());
                digitalWrite(motor1, HIGH);
                delay(50);
             } else {
                digitalWrite(motor1, LOW);
                break;
            }
        }
        digitalWrite(motor1, LOW);/*
        while(height  <= dH && height > height_t ) {
            digitalWrite(motor2, HIGH);
            height = distance_sensor(0);
        }
        digitalWrite(motor2, LOW);*/
    } else {
        float p_x = rand() % 100;
        dist = (p_x / 100) * height;
        dH = height - dist;
        game_print(height, dist);
        game_print(height, dist);
        while(height > dH) {
            digitalWrite(motor1, HIGH);
            height = distance_sensor();
        }
        digitalWrite(motor1, LOW);
        while(height  <= dH && height > 6) {
            digitalWrite(motor2, HIGH);
            height = distance_sensor();
        }
        digitalWrite(motor2, LOW);
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

void info(float height, float offset) {
    lcd.clear();
    lcd.print("Height:");
    calibration_print();
}

void calibration_print() {
    int arr[] = {height, tank1, tank2};
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
