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
int height_t = 15;
float dist = 0;
int height = 0;
int tank1 = 0;
int tank2 = 0;
int offset = 1;
bool set = false; //This is used to determine the type of game
bool menu_bool = false;
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
int distance_array[] = {};
//function prototypes
int calibration(int offset, int x);
int calibration_tank(int offset);
int distance_sensor(int x);
void calibration_print();
void menu_fields(int x);
void menu_switch(int x);
void game(int height);
void info(int height, int offset);
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
    for (int x = 0; x < 14; x++) {
        lcd.setCursor(x + 1, 0);
        lcd.print(name[x]);
        delay(200);
    }
    lcd.setCursor(6, 1);
    lcd.print("v0.1");
    delay(1000);*/
}

void loop() {
    digitalWrite(led, HIGH);
    int menu_size = sizeof(menu) / menu_len - 1;
    if (!calibrated) {
        lcd.clear();
        height = calibration(offset, 0);
        tank1 = calibration(offset, 1);
        tank2 = calibration(offset, 2);
        distance_array[0] = height;
        distance_array[1] = tank1;
        distance_array[2] = tank2;
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
                distance_array[0] = height;
                distance_array[1] = tank1;
                distance_array[2] = tank2;
                calibration_print();
            }
            break;
        case 1:
            set = true;
            game(height, set, 10, offset);
            break;
        case 2:
            set = true;
            game(height, set, 20, offset);
            break;
        case 3:
            set = true;
            game(height, set, 30, offset);
            break;
        case 4:
            set = false;
            game(height, set, 0, offset);
            break;
        case 5:
            info(height, offset);
            break;
        default:
            break;
    }
}

int calibration(int offset, int x) {
    digitalWrite(trigger[x], LOW);
    delayMicroseconds(2);
    digitalWrite(trigger[x], HIGH);
    delayMicroseconds(10);
    digitalWrite(trigger[x], LOW);
    duration = pulseIn(echo[x], HIGH);
    distance = duration * 0.034 / 2;
    calibrated = true;
    return distance - offset;
}

int distance_sensor(int x) {
    digitalWrite(trigger[x], LOW);
    delayMicroseconds(2);
    digitalWrite(trigger[x], HIGH);
    delayMicroseconds(10);
    digitalWrite(trigger[x], LOW);
    duration = pulseIn(echo[x], HIGH);
    distance = duration * 0.034 / 2;
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

void game(int height, bool set, float percentage, int offset) {
    int dH;
    if (set) {
        lcd.clear();
        dist = (percentage / 100) * height;
        dH = height - dist;
        game_print(height, dist);
        while(height != 9) {
            digitalWrite(motor1, HIGH);
            height = distance_sensor(0);
        }
        while(height != 7) {
            digitalWrite(motor1, LOW);
            digitalWrite(motor2, HIGH);
        }
        digitalWrite(motor1, LOW);
        digitalWrite(motor2, LOW);
    } else {
        float p_x = rand() % 100;
        dist = (p_x / 100) * height;
        dH = distance_array[0] - dist;
        game_print(height, dist);
        game_print(height, dist);
        while(height != dH) {
            digitalWrite(motor2, HIGH);
            height = distance_sensor(0);
        }
        while(height != distance_array[0]) {
            digitalWrite(motor2, LOW);
            digitalWrite(motor1, HIGH);
        }
        digitalWrite(motor1, LOW);
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

void info(int height, int offset) {
    lcd.clear();
    lcd.print("Height:");
    lcd.setCursor(9, 0);
    lcd.print(height);
    lcd.setCursor(10 + sizeof(height), 0);
    lcd.print("cm");
    calibration_print();
}

void calibration_print() {
    for (int i = 0; i < 3; i++) {
        lcd_line_clear(6, 16, 1);
        lcd.setCursor(0, 1);
        lcd.print(sens[i]);
        lcd.setCursor(7, 1);
        lcd.print(distance_array[i]);
        lcd.setCursor(7 + sizeof(distance_array[i]) , 1);
        lcd.print("cm");
        delay(600);
    }
    calibrated = true;
}

void lcd_line_clear(int start, int rows, int collumn) {
    for (int x = start; x < rows; x++) {
        lcd.setCursor(x, collumn);
        lcd.print(" ");
        delay(10);
    }
}
