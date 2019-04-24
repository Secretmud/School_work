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
int trigger = 9;
int echo = 10;
int btn = 2;
int dt = 3;
int clk = 4;
//Variables needed for the program to run
int height = 0;
int offset = 3;
bool set = false; //This is used to determine the type of game
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
                         {"Calibration"},
                         {"Drink menu"},
                         {"System info"}
                        };
char menu_sub[][16] = {
                       {"Mix: 10%"}, {"Mix: 20%"},
                       {"Mix: 30%"}, {"Random drink"},
                       {"back"}
                      };
char name[] = {"Drink'o'matic!"};
//function prototypes
int calibration(int offset);
void menu_fields(int x);
void menu_switch(int x);
void menu_sub_switch(void);
void game(int height);
void info(int height, int offset);
//Initializing the 16x2 LCD with a I2C connector
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
    int menu_size = sizeof(menu)/menu_len -1;
    if (!calibrated) {
        lcd.clear();
        height = calibration(offset);
        lcd.print("Height:");
        lcd.setCursor(1,1);
        lcd.print(height);
        lcd.setCursor(1+sizeof(height),1);
        lcd.print("cm");
        
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
            Serial.print("Position: ");
            Serial.println(counter);
            menu_fields(counter);
        }
        if (digitalRead(btn) == LOW) {
            Serial.println(counter);
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
            menu_sub_switch();
        case 2:
            info(height, offset);
            break;
        default:
            break;
    }

}

void menu_sub_switch()
{
    int menu_size = sizeof(menu_sub)/menu_len -1;
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
        Serial.print("Position: ");
        Serial.println(counter);
        switch(counter)
        {
        case 0:
            set = false;
            game(height, set, 0);
            break;
        case 1:
            set = true;
            game(height, set, 10);
            break;
        case 2:
            set = true;
            game(height, set, 20);
            break;
        case 3:
            set = true;
            game(height, set, 30);
            break;
        }
    }
    if (digitalRead(btn) == LOW) {
        Serial.println(counter);
        menu_switch(counter);
    }       
    dtLastState = dtState;
}

int calibration(int offset)
{
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

void game(int height, bool set, int percentage)
{
    if (set) {
        int p_x = percentage;
        int x = (p_x/100)*height;
        game_print(x);
    } else {
        int p_x = rand() % 100;
        int x = (p_x/100)*height;
        game_print(x);
    }
}

void game_print(int x)
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
    lcd.clear();
    lcd.print("Height:");
    lcd.setCursor(9,0);
    lcd.print(height - offset);
    lcd.setCursor(9+sizeof(height),0);
    lcd.print("cm");
    lcd.setCursor(0,1);
    lcd.print("Offset:");
    lcd.setCursor(9,1);
    lcd.print(offset);
    lcd.setCursor(9,1);
    lcd.print("+");
    lcd.setCursor(10+sizeof(offset),1);
    lcd.print("cm");
}
