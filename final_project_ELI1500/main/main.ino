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

   ,-,--.  ,--.--------.   _,.---._
 ,-.'-  _\/==/,  -   , -\,-.' , -  `.   .-.,.---.     _..---.  ,--.-.  .-,--.
/==/_ ,_.'\==\.-.  - ,-./==/_,  ,  - \ /==/  `   \  .' .'.-. \/==/- / /=/_ /
\==\  \    `--`\==\- \ |==|   .=.     |==|-, .=., |/==/- '=' /\==\, \/=/. /
 \==\ -\        \==\_ \|==|_ : ;=:  - |==|   '='  /|==|-,   '  \==\  \/ -/
 _\==\ ,\       |==|- ||==| , '='     |==|- ,   .' |==|  .=. \  |==|  ,_/
/==/\/ _ |      |==|, | \==\ -    ,_ /|==|_  . ,'. /==/- '=' ,| \==\-, /
\==\ - , /      /==/ -/  '.='. -   .' /==/  /\ ,  )==|   -   /  /==/._/
 `--`---'       `--`--`    `--`--''   `--`-`--`--'`-._`.___,'   `--`-`

              .-._         .=-.-.       ,-.-.    ,----.                ,-,--.   .=-.-.
 .--.-. .-.-./==/ \  .-._ /==/_ /,--.-./=/ ,/ ,-.--` , \  .-.,.---.  ,-.'-  _\ /==/_ /
/==/ -|/=/  ||==|, \/ /, /==|, |/==/, ||=| -||==|-  _.-` /==/  `   \/==/_ ,_.'|==|, |,--.--------.
|==| ,||=| -||==|-  \|  ||==|  |\==\,  \ / ,||==|   `.-.|==|-, .=., \==\  \   |==|  /==/,  -   , -\
|==|- | =/  ||==| ,  | -||==|- | \==\ - ' - /==/_ ,    /|==|   '='  /\==\ -\  |==|- \==\.-.  - ,-./
|==|,  \/ - ||==| -   _ ||==| ,|  \==\ ,   ||==|    .-' |==|- ,   .' _\==\ ,\ |==| ,|`--`--------`
|==|-   ,   /|==|  /\ , ||==|- |  |==| -  ,/|==|_  ,`-._|==|_  . ,'./==/\/ _ ||==|- |
/==/ , _  .' /==/, | |- |/==/. /  \==\  _ / /==/ ,     //==/  /\ ,  )==\ - , //==/. /
`--`..---'   `--`./  `--``--`-`    `--`--'  `--`-----`` `--`-`--`--' `--`---' `--`-`
 ,--.--------.    ,----.  ,--.--------.    ,----.  ,--.--------.
/==/,  -   , -\,-.--` , \/==/,  -   , -\,-.--` , \/==/,  -   , -\
\==\.-.  - ,-./==|-  _.-`\==\.-.  - ,-./==|-  _.-`\==\.-.  - ,-./
 `--`\==\- \  |==|   `.-. `--`\==\- \  |==|   `.-. `--`\==\- \
      \==\_ \/==/_ ,    /      \==\_ \/==/_ ,    /      \==\_ \
      |==|- ||==|    .-'       |==|- ||==|    .-'       |==|- |
      |==|, ||==|_  ,`-._      |==|, ||==|_  ,`-._      |==|, |
      /==/ -//==/ ,     /      /==/ -//==/ ,     /      /==/ -/
      `--`--``--`-----``       `--`--``--`-----``       `--`--`

Gruppe 7:
    Oppgave:
        Vanndispenser for dyr(med litt forandring)

*/
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#define menu_len 16
//Pin declaration
int trigger[] = {5, 7, 9};
int echo[] = {6, 8, 10};
int btn = 2;
int dt = 3;
int clk = 4;
int motor1 = 11;
int motor2 = 12;
//Variables needed for the program to run
int height = 0;
int tank1 = 0;
int tank2 = 0;
int offset = 3;
bool set = false; //This is used to determine the type of game
int distance_array[] = {};
//Used by the ultrasonic sensor
bool calibrated = false;
long distance;
long duration;
//Rotatry encoder
int counter = 0;
int dtState;
int dtLastState;
//Menu arrays
char menu[][menu_len] = {
                         {"Calibration"}, {"Mix: 10%"}, {"Mix: 20%"},
                         {"Mix: 30%"}, {"Random drink"}, {"System info"}
                        };
char name[] = {"Drink'o'matic!"};
//function prototypes
int calibration(int offset);
int calibration_tank(int offset);
//int distance_sensor(int x);
int distance_tank(int tank);
void menu_fields(int x);
void menu_switch(int x);
//void menu_sub_switch(void);
void game(int height);
void info(int height, int offset);
//Initializing the 16x2 LCD with a I2C connector
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup()
{
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
    dtLastState = digitalRead(dt);
    for (int x = 0; x < 14; x++) {
        lcd.setCursor(x+1,0);
        lcd.print(name[x]);
        delay(200);
    }
    lcd.setCursor(6,1);
    lcd.print("v0.1");
}

void loop()
{
    int menu_size = sizeof(menu)/menu_len -1;
    if (!calibrated) {
        lcd.clear();
        height = calibration(offset);
        tank1 = distance_tank(1);
        tank2 = distance_tank(2);
        lcd.print("Height:");
        lcd.setCursor(1,1);
        lcd.print(height);
        lcd.setCursor(1+sizeof(height),1);
        lcd.print("cm");
        delay(300);
        lcd.print(tank1);
        lcd.setCursor(1+sizeof(tank1),1);
        lcd.print("cm");
        delay(300);
        lcd.print(tank2);
        lcd.setCursor(1+sizeof(tank2),1);
        lcd.print("cm");
        delay(300);
        distance_array[0] = height;
        distance_array[1] = tank1;
        distance_array[2] = tank2;
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
            if ( counter > menu_size)
                counter = menu_size;
            menu_fields(counter);
        }
        if (digitalRead(btn) == LOW) {
            menu_switch(counter);
        }       
        dtLastState = dtState;
    }
}

void menu_switch(int counter)
{
    switch(counter) {
        case 0:
            if (calibrated) {
                lcd.clear();
                height = calibration(offset);
                lcd.setCursor(0,0);
                lcd.print("Height:");
                lcd.setCursor(1,1);
                lcd.print(height);
                lcd.setCursor(1+sizeof(height),1);
                lcd.print("cm");
            }
            break;
        case 1:
            set = false;
            game(height, set, 0, offset);
            break;
        case 2:
            set = true;
            game(height, set, 10, offset);
            break;
        case 3:
            set = true;
            game(height, set, 20, offset);
            break;
        case 4:
            set = true;
            game(height, set, 30, offset);
            break;
        case 5:
            info(height, offset);
            break;
        default:
            break;
    }

}

int calibration(int offset)
{
    digitalWrite(trigger[0], HIGH);
    delayMicroseconds(10);
    digitalWrite(trigger[0], LOW);
    duration = pulseIn(echo[0], HIGH);
    distance= duration*0.034/2;
    calibrated = true;
    distance_array[0] = distance + offset;
    return distance + offset;
}

int distance_tank(int tank)
{
    digitalWrite(trigger[tank], LOW);
    delayMicroseconds(2);
    digitalWrite(trigger[tank], HIGH);
    delayMicroseconds(10);
    digitalWrite(trigger[tank], LOW);
    duration = pulseIn(echo[tank], HIGH);
    distance= duration*0.034/2;
    return distance + offset;

}

int distance_sensor(int x, int offset)
{
    digitalWrite(trigger[x], LOW);
    delayMicroseconds(2);
    digitalWrite(trigger[x], HIGH);
    delayMicroseconds(10);
    digitalWrite(trigger[x], LOW);
    duration = pulseIn(echo[x], HIGH);
    distance= duration*0.034/2;
    return distance + offset;
}

void menu_fields(int current)
{
    lcd.clear();
    lcd.print(">");
    lcd.setCursor(1,0);
    lcd.print(menu[current]);
    lcd.setCursor(1,1);
    lcd.print(menu[current+1]);
}

void game(int height, bool set, int percentage, int offset)
{
    if (set) {
        int dist = (percentage/100)*height;
        game_print(height, dist);
        if(distance_sensor(0, offset)) 
            digitalWrite(motor1, HIGH);
        else if (distance_sensor(0, offset) > dist && distance_sensor(0, offset) < height)
            digitalWrite(motor2, HIGH);
    } else {
        int p_x = rand() % 100;
        int dist = (p_x/100)*height;
        game_print(height, dist);
    }
}

void game_print(int height, int x)
{
    lcd.clear();
    lcd.print("height cm:");
    lcd.setCursor(13,0);
    lcd.print(height);
    lcd.setCursor(0,1);
    lcd.print("Liquid1 cm:");
    lcd.setCursor(13,1);
    lcd.print(x);
}

void info(int height, int offset)
{
    for (int y = 0; y < sizeof(distance_array)/sizeof(int); y++) {

    }
    int x = height - offset;
    lcd.clear();
    lcd.print("Height:");
    lcd.setCursor(9,0);
    lcd.print(x);
    lcd.setCursor(10+sizeof(height),0);
    lcd.print("cm");
}
