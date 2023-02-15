#include <Keypad.h>
#include <LiquidCrystal.h>
#include <String.h>
#include <Time.h>

LiquidCrystal lcd (13,12,11,10,9,8);       //(rs, en, d4, d5, d6, d7)

char keys[4][3] = 
 {{'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}};

byte rowPins[4] = {A0,A1,A2,A3}; //connect to the row pinouts of the keypad
byte colPins[3] = {7, 6, 5};     //connect to the column pinouts of the keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, 4, 3 );

volatile int counter = 0;
byte attempts = 3;
int EPass [4];
int pos = 0;
int *getRandPass ();
int *Pass = getRandPass ();


void setup() 
{
  pinMode (4,OUTPUT);
  pinMode (3,OUTPUT); 
  
  TCCR1A = 0;
  TCCR1B = 0x04;
  TCNT1 = 3035;
  TIMSK1 = 1;
  
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("   ***WELCOME!***");
  delay(1000);   lcd.clear();
  lcd.print("* -> Enter Password");
  lcd.setCursor(0, 1);
  lcd.print("# -> Clean Password");
  delay(1000);   lcd.clear();
  
  lcd.setCursor(12, 1);
  lcd.print("H: ");
  for (int i = 0; i < 4; ++i)
    {
       lcd.print(Pass[i]);
    }
  
  lcd.setCursor(0, 0);
  lcd.print("Password: ");
  
}



void loop() 
{
  char key = keypad.getKey();
  
  if(key != NO_KEY)
       {
        
        if (key == '*')
          {
            lcd.setCursor(0, 1);
            if ( isEqual (EPass , Pass , 4))
              {
                lcd.print("Opened");
                delay(1000);
                lcd.setCursor(10, 0);   lcd.print("     ");  
                lcd.setCursor(0, 1);    lcd.print("         ");
                digitalWrite(4,LOW);   digitalWrite(3,HIGH);
                attempts = 3;
              }
            else
             {
              lcd.print("Wrong");
              delay(1000);
              lcd.setCursor(10, 0);   lcd.print("     ");
              lcd.setCursor(0, 1);    lcd.print("      ");
              lcd.setCursor(0, 1);
              lcd.print("Attempt:");  lcd.print(--attempts);
              delay(1000);   
              lcd.setCursor(0, 1);    lcd.print("         ");
              if (attempts == 0)
                    {
                      digitalWrite(3,LOW); digitalWrite(4,HIGH);
                      attempts = 3;      
                    }
              pos = 0;
              EPass[0] = NULL;
              lcd.setCursor(10, 0);
             }
          }
          
        else if (key == '#')
          {
              lcd.setCursor(10, 0);   lcd.print("     ");
              pos = 0;
              EPass[0] = NULL;
              lcd.setCursor(10, 0);
          }
          
         else
           {
             EPass[pos++] = key - '0';
             lcd.print('*');
           }
       }
}


ISR (TIMER1_OVF_vect)
{
  TCNT1 = 3035;
  counter++;
  if (counter == 5)
  {
    counter = 0;
    Pass = getRandPass();
    lcd.setCursor(12, 1);
    lcd.print("H: ");
    for (int i = 0; i < 4; ++i)
     {
       lcd.print(Pass[i]);
     }
    lcd.setCursor(10, 0);
  }
}

int *getRandPass ()
{
    static int array [4];
    for (int i = 0; i < 4; ++i)
     array[i] = random(0,9); 
     
    return array;
}


bool isEqual (const int *a1 ,const int *a2, int size)
{
    int counter = 0;

    for (int i = 0; i < size; ++i)
        if (*a1++ == *a2++)
            counter++;

    return (counter == size);
}


