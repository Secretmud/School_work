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
        Vanndispenser for dyr(++)
*/

#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <HCSR04.h>
#define menu_len 16
# define full_glass 12000
//Pin declaration

int trigger[] = {5, 7, 9};
int echo[] = {6, 8, 10};

int btn = 2;
int dt = 3;
int clk = 4;
int motor1 = 11;
int motor2 = 12;
int led = 13;

bool set = false;
bool rand_set = false;

double height;
double box1;
double box2;
double glass;

//Used by the ultrasonic sensor
bool calibrated = false;
long distance;
long duration;

//Rotatry encoder
int counter = 0;
int dtState;
int dtLastState;

//Arrays
char menu[][menu_len] = {{"Calibration"}, {"Mix: 10/90"}, {"Mix: 20/80"},
                         {"Mix: 30/70"}, {"Random drink"}, {"Cat-mode"},
                         {"System info"}};
int out[] = {5, 7, 9,
             11, 12, 13};
int in [] = {3, 4, 6,
             8, 10};

//function prototypes
void calibration_print(double height, double tank1, double tank2);
void game_print(double height, double dist);
void menu_fields(int x);
void menu_switch(int counter, double height, float offset, double box1, double box2);
void programs(float height, bool set, float percentage, int offset);
void lcd_line_clear(int start, int collumn, int rows);
void tank_low(int tank);

//Initializing the 16x2 LCD with a I2C connector
LiquidCrystal_I2C lcd(0x27, 16, 2);

//Initializing the sensors
UltraSonicDistanceSensor front(trigger[0], echo[0]);
UltraSonicDistanceSensor tank1(trigger[1], echo[1]);
UltraSonicDistanceSensor tank2(trigger[2], echo[2]);

void setup() {
    pinMode(btn, INPUT_PULLUP);
    for (int x = 0; x < 5; x++) //Using the for loop to assign INPUT and OUTPUT pins
        pinMode( in [x], INPUT);
    for (int i = 0; i < 6; i++)
        pinMode(out[i], OUTPUT);
    lcd.init(); //Starting the lcd module and setting it to use a backling
    lcd.backlight();
    lcd.clear();
    dtLastState = digitalRead(dt);
}

void loop() {
    int height_t = 7;
    float offset = 0;
    bool measurement = true;
    int menu_size = sizeof(menu) / menu_len - 1; //calculating the amount of menu_items and zero indexing them
    if (!calibrated) {//Initial calibration if it hasn't been done before
        lcd.clear();
        height = front.measureDistanceCm();
        box1 = tank1.measureDistanceCm();
        box2 = tank2.measureDistanceCm();
        calibration_print(height - glass, box1, box2);
        calibrated = true;
    } else {
        //Checking the states and appending a 1 or subtracting a one to counter depending on the placement of the rotary encoder
        dtState = digitalRead(dt);
        if (dtState != dtLastState) { 
            if (digitalRead(clk) != dtState) {
                counter++;
            } else {
                counter--;
            }
            //These two if's makes sure that you can't scrollpast the items in the menu
            if (counter < 0)
                counter = 0;
            if (counter > menu_size)
                counter = menu_size;
            menu_fields(counter);
        }

        //Checking for "LOW" since pinMode for btn is INPUT_PULLUP
        if (digitalRead(btn) == LOW) {
            menu_switch(counter, height, glass, box1, box2);
        }

        dtLastState = dtState;
    }
}

void menu_switch(int counter, double height, double glass, double box1, double box2) {
    //Checking the distance to the water in the two water tanks
    if (tank1.measureDistanceCm() > 22) {
        tank_low(0);
    } else if (tank2.measureDistanceCm() > 22) {
        tank_low(1);
    } else {
        /*Switch case to chose a function depending on the placement of the rotary. You'll be able to see which item you pick
         *from the menu_fields(int counter) function.
         */
        switch (counter) {
        case 0:
            if (calibrated) {
                lcd.clear();
                height = front.measureDistanceCm();
                box1 = tank1.measureDistanceCm();
                box2 = tank2.measureDistanceCm();
                calibration_print(height, box1, box2);
            }
            break;
        case 1:
            if (tank1.measureDistanceCm() >= 25) {
                digitalWrite(led, HIGH);
                break;
            }
            programs(height, true, false, 10, glass);
            break;
        case 2:
            programs(height, true, false, 20, glass);
            break;
        case 3:
            programs(height, true, false, 30, glass);
            break;
        case 4:
            programs(height, false, true, 0, glass);
            break;
        case 5:
            programs(height, false, false, 0, glass);
            break;
        case 6:
            calibration_print(height, box1, box2);
            break;
        default:
            break;
        }
    }
}

void menu_fields(int counter) {
    //Prints the menu and adds a > infront of the item you're picking
    lcd.clear();
    lcd.print(">");
    lcd.setCursor(1, 0);
    lcd.print(menu[counter]);
    lcd.setCursor(1, 1);
    lcd.print(menu[counter + 1]);
}

void programs(double height, bool set, bool rand_set, double percentage, double glass) {
    //This is where all the program logic is placed, we're using booleans or the lack thereof to choose which program to run.
    double dist;
    double ho;
    if (set) {
        lcd.clear();
        dist = (percentage / 100);
        game_print(height, dist * 100);
        delay(1000);
        while (1) {
            digitalWrite(motor1, HIGH);
            delay(full_glass * dist + 700);
            digitalWrite(motor1, LOW);
            digitalWrite(motor2, HIGH);
            delay(full_glass * (1 - dist) + 700);
            digitalWrite(motor2, LOW);
            break;
        }
    } else if (rand_set) {
        lcd.clear();
        ho = rand() % 50 + 10;
        dist = (ho / 100);
        game_print(height, dist * 100);
        delay(1000);
        while (1) {
            digitalWrite(motor1, HIGH);
            delay(full_glass * dist + 700);
            digitalWrite(motor1, LOW);
            digitalWrite(motor2, HIGH);
            delay(full_glass * (1 - dist) + 700);
            digitalWrite(motor2, LOW);
            break;
        }
    } else {
        lcd.clear();
        lcd.print("Cat-mode!");
        lcd.setCursor(0, 1);
        lcd.print("Button to exit");
        while (1) {
            if (front.measureDistanceCm() <= 13) {
                digitalWrite(motor2, LOW);
                if (digitalRead(dt) == HIGH)
                    break;
            }
            if (front.measureDistanceCm() >= 13) {
                digitalWrite(motor2, HIGH);
                delay(400);
            }

        }
    }
}

void game_print(double height, double x) {
    lcd.clear();
    lcd.print("height cm:");
    lcd.setCursor(13, 0);
    lcd.print(height);
    lcd.setCursor(0, 1);
    lcd.print("Mix:");
    lcd.setCursor(5, 1);
    lcd.print(x);
    lcd.setCursor(5+sizeof(x),1);
    lcd.print("\%");
}

void calibration_print(double height, double tank1, double tank2) {
    double arr[] = {
        height,
        tank1,
        tank2
    };
    char name[][6] = {{"Glass"}, {"Tank1"}, {"Tank2"}};
    lcd.clear();
    lcd.print("Height:");
    for (int i = 0; i < 3; i++) {
        lcd_line_clear(0, 16, 1);
        lcd.setCursor(0, 1);
        lcd.print(name[i]);
        lcd.setCursor(7, 1);
        lcd.print(arr[i]);
        lcd.setCursor(7 + sizeof(arr[i]), 1);
        lcd.print("cm");
        delay(600);
    }
}

void lcd_line_clear(int start, int rows, int collumn) {
    for (int x = start; x < rows; x++) {
        lcd.setCursor(x, collumn);
        lcd.print(" ");
    }
}

void tank_low(int tank) {
    char tank_number[][5] = {{"Tank1"}, {"Tank2"}};
    lcd.clear();
    lcd.print("Low volume in");
    lcd.setCursor(0, 1);
    lcd.print(tank_number[tank]);
    while (1) {
        digitalWrite(led, HIGH);
        delay(300);
        digitalWrite(led, LOW);
        delay(300);
        if (tank1.measureDistanceCm() <= 15 && tank2.measureDistanceCm() <= 15) {
            lcd.clear();
            lcd.print("Twist to enter");
            lcd.setCursor(0,1);
            lcd.print("the menu");
            break;
        }
    }
}
