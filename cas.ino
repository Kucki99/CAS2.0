#include <Wire.h>

//Import der LCD Library + Erzeugung des LCD Objekts
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

//Symbol für Anzeige auf LCD Display
byte load[8] = {B11111,B11111,B11111,B11111,B11111,B11111,B11111,B11111};
char z = 1;

//Interrupttaster
const int menu = 2;
const int exponent = 3;

//Digitale Taster
const int less = 4;
const int more = 5;
const int enter = 6;

//Analoger Pin für Joystick
const int X_pin = 3;

//LED Pins für 10er Potenzen
int one = 8;
int ten = 9;
int hundred = 10;

//Variable für Punkte im Rechenquiz
int points = 0;

//Hilfsvariablen
int ex = 0;
int menuk = 0;
boolean menuklicked = false;
double version = 1.0;
long currentmil = 0;

void setup() {
  //Festlegung des LCD Displays auf 2 Zeilen mit jeweils 16 Zeichen
  lcd.begin(16, 2);
  //Benötigung von Pseudorandom-Zahlen für Rechenquiz
  randomSeed(analogRead(2));
  lcd.createChar(z, load);
  //LEDs für 10er Potenzen
  pinMode(one,OUTPUT);
  pinMode(ten,OUTPUT);
  pinMode(hundred,OUTPUT);
  //Digitale Taster
  pinMode(more,INPUT);
  pinMode(less,INPUT);
  pinMode(enter,INPUT);
  //Interrupt des Menütasters
  attachInterrupt(digitalPinToInterrupt(menu),isr1,RISING);
  //Interrupt zur Auswahl der 10er Potenz
  attachInterrupt(digitalPinToInterrupt(exponent),isr2,RISING);
  //Startanimation
  begin();
  
}

void loop() {
  
  menuklicked = false;

  delay(10);
  
  setMenu(menuk);                       /*MENÜAUSWAHL ANFANG*/
  while(digitalRead(enter)==LOW) {    

    if(analogRead(X_pin)<=20) {
      menuk--;
      if(menuk==-1) {
        menuk = 3;
        setMenu(menuk);
      } else {
        setMenu(menuk);
      }
      delay(200);
    } else 
     if(analogRead(X_pin)>=1000) {
      menuk++;
      if(menuk==4) {
        menuk=0;
        setMenu(menuk);
      } else {
        setMenu(menuk);
      }
      
      delay(200);
    }
    
    delay(10);
  }
  lcd.clear();                            /*MENÜAUSWAHL ENDE*/
  delay(150);                         
  while(digitalRead(enter)==LOW) {
    if(menuklicked) {
      return;
    }
    switch(menuk) {
      case 0:                             /*ARITHMETIK ANFANG*/
      {
        int zahl1=0;
        int zahl2=0;
        int selected=0;
        while(digitalRead(enter)==LOW) {   /*Zahl 1 abfragen ANFANG*/
          if(menuklicked) {
            return;
          }
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Zahl 1?");
          lcd.setCursor(0,1);
          lcd.print(zahl1);
          if(digitalRead(more)==HIGH) {
            switch(ex) {
              case 0:
              {
                zahl1++;
                break;
              }
              case 1:
              {
                zahl1=zahl1+10;
                break;
              }
              case 2: 
              {
                zahl1=zahl1+100;
                break;
              }
            }
          }
          if(digitalRead(less)==HIGH) {
           switch(ex) {
            case 0:
            {
              zahl1--;
              break;
            }
            case 1:
            {
              zahl1=zahl1-10;
              break;
            }
            case 2:
            {
              zahl1=zahl1-100;
              break;
            }
           }
          }
          delay(100);
        }                                     /*Zahl 1 abfragen ENDE*/
        delay(200);
        while(digitalRead(enter)==LOW) {      /*Zahl 2 abfragen ANFANG*/
          if(menuklicked) {
            return;
          }
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Zahl 2?");
          lcd.setCursor(0,1);
          lcd.print(zahl2);
          if(digitalRead(more)==HIGH) {
            switch(ex) {
              case 0:
              {
                zahl2++;
                break;
              }
              case 1:
              {
                zahl2=zahl2+10;
                break;
              }
              case 2: 
              {
                zahl2=zahl2+100;
                break;
              }
            }
          }
          if(digitalRead(less)==HIGH) {
           switch(ex) {
            case 0:
            {
              zahl2--;
              break;
            }
            case 1:
            {
              zahl2=zahl2-10;
              break;
            }
            case 2:
            {
              zahl2=zahl2-100;
              break;
            }
           }
          }
          delay(100);
        }                             /*Zahl 2 abfragen ENDE*/
        delay(200);
        while(digitalRead(enter)==LOW) { /*Operator abfragen ANFANG*/
          if(menuklicked) {
            return;
          }
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Operator?");
          lcd.setCursor(0,1);
          switch(selected) {
            case 0:
            {
              lcd.print("+");
              break;
            }
            case 1:
            {
              lcd.print("-");
              break;
            }
            case 2:
            {
              lcd.print("*");
              break;
            }
            case 3:
            {
              lcd.print("/");
              break;
            }
          }
          if(digitalRead(more)==HIGH) {
            if(selected==3) {
              selected=0;
          } else {
            selected++;
          }
        }
        if(digitalRead(less)==HIGH) {
          if(selected==0) {
            selected=3;
          } else {
            selected--;
          }
        }
          delay(150);
        }                 /*Operator abfragen ENDE*/
        delay(200);
        while(digitalRead(enter)==LOW) {
          if(menuklicked) {
            return;
          }
        
        lcd.clear();
        lcd.setCursor(0,0);
        
       
        switch(selected) {
          case 0:
          {
            long ergebnis = 0;
            ergebnis=zahl1+zahl2;
            lcd.print(zahl1);
            lcd.print("+");
            lcd.print(zahl2);
            lcd.print("=");
            lcd.setCursor(0,1);
            lcd.print(ergebnis);            
            break;
          }
          case 1:
          {
            long ergebnis = 0;
            ergebnis=zahl1-zahl2;
            lcd.print(zahl1);
            lcd.print("-");
            lcd.print(zahl2);
            lcd.print("=");
            lcd.setCursor(0,1);
            lcd.print(ergebnis); 
            break;
          }
          case 2:
          {
            long ergebnis = 0;
            ergebnis=(long)zahl1*zahl2;
            lcd.print(zahl1);
            lcd.print("*");
            lcd.print(zahl2);
            lcd.print("=");
            lcd.setCursor(0,1);
            lcd.print(ergebnis);
            break;
          }
          case 3:
          {
            double ergebnis = 0.0;
            ergebnis=(double)zahl1/zahl2;
            lcd.print(zahl1);
            lcd.print("/");
            lcd.print(zahl2);
            lcd.print("=");
            lcd.setCursor(0,1);
            lcd.print(ergebnis);
            break;
          }
        }
        /*while(digitalRead(enter)==LOW) {
          if(menuklicked) {
            return;
          }
          }
          */
          delay(150);
        }
        break;
      }                            /*ARITHMETIK ENDE*/
      case 1:
      {                            /*POTENZIEREN ANFANG*/
        while(digitalRead(enter)==LOW) {
          if(menuklicked) {
           return;
             }
             int x = 0;
             int n = 0;
             int selected = 0;
             while(digitalRead(enter)==LOW) {     /*Basis abfragen ANFANG*/
         if(menuklicked) {
            return;
          }
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Basis?");
          lcd.setCursor(0,1);
          lcd.print(x);
          if(digitalRead(more)==HIGH) {
            switch(ex) {
              case 0:
              {
                x++;
                break;
              }
              case 1:
              {
                x=x+10;
                break;
              }
              case 2: 
              {
                x=x+100;
                break;
              }
            }
          }
          if(digitalRead(less)==HIGH) {
           switch(ex) {
            case 0:
            {
              x--;
              break;
            }
            case 1:
            {
              x=x-10;
              break;
            }
            case 2:
            {
              x=x-100;
              break;
            }
           }
          }
          delay(100);
        }                         /*Basis abfragen ENDE*/

        delay(200);

        while(digitalRead(enter)==LOW) {     /*Exponent abfragen ANFANG*/
         if(menuklicked) {
            return;
          }
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Exponent?");
          lcd.setCursor(0,1);
          lcd.print(n);
          if(digitalRead(more)==HIGH) {
            switch(ex) {
              case 0:
              {
                n++;
                break;
              }
              case 1:
              {
                n=n+10;
                break;
              }
              case 2: 
              {
                n=n+100;
                break;
              }
            }
          }
          if(digitalRead(less)==HIGH) {
           switch(ex) {
            case 0:
            {
              n--;
              break;
            }
            case 1:
            {
              n=n-10;
              break;
            }
            case 2:
            {
              n=n-100;
              break;
            }
           }
          }
          delay(150);
        }                         /*Exponent abfragen ENDE*/
        
      delay(200);


        double ergebnis = 0.0;
        
        if(n>0) {
          for(int i = 0; i<n; i++) {
            if(i==0) {
              ergebnis = x;
            } else {
              ergebnis = ergebnis*x;
            }
          }

          
        } else
        if(n<0) {
            for(int i = 0; i>n; i--) {
              if(i==0) {
                ergebnis = (double)1/(double)(x*-1);
              } else {
                ergebnis = (double)ergebnis*(double)(1/(double)(x*-1));
              }
            }
        } else {

            ergebnis = 1.0;
            
            
          
        }

              
             
        while(digitalRead(enter)==LOW) {
        if(menuklicked) {
          return;  
        }
        
         
         lcd.clear();
         lcd.setCursor(0,0);

        /* for(int i = 0; i<n; i++) {
          if(i==0) {
            ergebnis = x;
          } else {
            ergebnis = ergebnis*x;
          }
                   } */

            lcd.print(x);
            lcd.print("^");
            lcd.print(n);
            lcd.print("=");
            lcd.setCursor(0,1);
            lcd.print(ergebnis);

          
        


        delay(150);
        }
        }
        break;       
      }                            /*POTENZIEREN ENDE*/
      
      
      case 2:                       /*FAKULTAET ANFANG*/
      {
        while(digitalRead(enter)==LOW) {
          if(menuklicked) {
            return;
          }
          int zahl = 0;
          unsigned long fakul= 1;
          while(digitalRead(enter)==LOW) {
            if(menuklicked) {                   /*ZAHL ABFRAGEN ANFANG*/
              return;
            }
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("Fakultaet?");
            lcd.setCursor(0,1);
            lcd.print(zahl);
            if(digitalRead(more)==HIGH) {
            switch(ex) {
              case 0:
              {
                zahl++;
                break;
              }
              case 1:
              {
                zahl=zahl+10;
                break;
              }
              case 2: 
              {
                zahl=zahl+100;
                break;
              }
            }
          }
          if(digitalRead(less)==HIGH) {
           switch(ex) {
            case 0:
            {
              zahl--;
              break;
            }
            case 1:
            {
              zahl=zahl-10;
              break;
            }
            case 2:
            {
              zahl=zahl-100;
              break;
            }
           }
          }
          delay(100);
          }                   /*Zahl ABFRAGEN ENDE*/
          delay(200);

          int i = 1;
          while(i<=zahl) {
            fakul = fakul*i;
            i=i+1;
            delay(1);
          }
        while(digitalRead(enter)==LOW) {
        if(menuklicked) {
          return;
        }
        
        lcd.clear();
        if(zahl>12||zahl<0) {
          lcd.setCursor(0,0);
          lcd.print("Keine Definition");
        } else {
       /*   int i = 1;
          while(i<=zahl) {
            fakul = fakul*i;
            i=i+1;
            delay(1);
          } */

              
              lcd.setCursor(0,0);
              lcd.print(zahl);
              lcd.print("!=");
              lcd.setCursor(0,1);
              lcd.print(fakul);

              
        
             
          }
          delay(150);
        }
        }
        break;
        
      }                             /*FAKULTAET ENDE*/
      case 3:
      {                             /*RECHENQUIZ ANFANG*/
              lcd.clear();
         int op = random(3);
      int zahl1=0,zahl2=0,ergebnis=0,rechnung=0;
      delay(5);
      switch(op) {
        case 0:
         {
            zahl1=random(1,101);
            zahl2=random(1,101);
            ergebnis=zahl1+zahl2;
            lcd.setCursor(0,0);
            lcd.print(zahl1);
            lcd.print("+");
            lcd.print(zahl2);
            lcd.print("?");
           
            break;
            }
        case 1:
        {
          zahl1=random(51,101);
          zahl2=random(1,51);
          ergebnis=zahl1-zahl2;
          lcd.setCursor(0,0);
          lcd.print(zahl1);
          lcd.print("-");
          lcd.print(zahl2);
          lcd.print("?");
         
          break;
        }
        case 2:
        {
          zahl1=random(1,21);
          zahl2=random(1,21);
          ergebnis=zahl1*zahl2;
          lcd.print(zahl1);
          lcd.print("*");
          lcd.print(zahl2);
          lcd.print("?");
          
          break;
        }
        }
   while(digitalRead(enter)==LOW) {
      if(menuklicked) {
        return;
      }
    lcd.setCursor(0,1);
    lcd.print("                ");
    lcd.setCursor(0,1);
    lcd.print(rechnung);
            lcd.setCursor(6,1);
        lcd.print("Points:");
        lcd.print(points);
    if(digitalRead(more)==HIGH) {
     switch(ex) {
              case 0:
              {
                if(rechnung<10000){
                rechnung++;
                }
                break;
              }
              case 1:
              {
                if(rechnung<=9990) {
                rechnung=rechnung+10;
                }
                break;
              }
              case 2: 
              {
                if(rechnung<=9900) {
                rechnung=rechnung+100;
                }
                break;
              }
            }
            
    }

    if(digitalRead(less)==HIGH) {
           switch(ex) {
            case 0:
            {
              if(rechnung>0) {
              rechnung--;
              }
              break;
            }
            case 1:
            {
              if(rechnung>=10) {
              rechnung=rechnung-10;
              }
              break;
            }
            case 2:
            {
              if(rechnung>=100) {
              rechnung=rechnung-100;
              }
              break;
            }
           }
           
          }

    delay(100);
   
     }

   delay(200);
   while(digitalRead(enter)==HIGH) {
    delay(1);
   }
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("=");
   lcd.print(ergebnis);
   lcd.print("(");
   lcd.print(rechnung);
   lcd.print(")");
   if(ergebnis==rechnung) {
    lcd.setCursor(4,1);
    lcd.print("Richtig!");
    points++;
    if(points==1000) {
      points = 0;
    }
   } else {
    lcd.setCursor(4,1);
    lcd.print("Falsch!");
    points = 0;
   }
   
   while(digitalRead(enter)==LOW) {
      if(menuklicked) {
        return;
      }
      delay(100);
   }
   delay(100);
        break;
      }                             /*RECHENQUIZ ENDE*/
      default:
      {
        return;
        break;
      }
    }
    delay(350);
    
  
  }
}

//ISR des Menütasters
void isr1() {
 if(!menuklicked) {
  menuklicked = true;
 }
}

//ISR der 10er Potenz Auswahl
void isr2() {
  long now = millis();
  if(now>(currentmil+200)) {
  currentmil = millis();
  switch(ex) {
    case 0:
    {
      ex++;
      digitalWrite(one,LOW);
      digitalWrite(ten,HIGH);
      digitalWrite(hundred,LOW);
      break;
    }
    case 1:
    {
      ex++;
      digitalWrite(one,LOW);
      digitalWrite(ten,LOW);
      digitalWrite(hundred,HIGH);
      break;
    }
    case 2:
    {
      ex=0;
      digitalWrite(one,HIGH);
      digitalWrite(ten,LOW);
      digitalWrite(hundred,LOW);
      break;
    }
  }
}
}


//Startanimation
void begin() {
  lcd.backlight();
  digitalWrite(one,HIGH);
  lcd.setCursor(3,0);
  lcd.print("<<CAS2.0>>");
  delay(500);
  lcd.setCursor(0,1);
  lcd.print(z);
  lcd.print(z);
  delay(400);
  lcd.print(z);
  delay(600);
  lcd.print(z);
  lcd.print(z);
  delay(550);
  lcd.print(z);
  lcd.print(z);
  lcd.print(z);
  delay(700);
  lcd.print(z);
  lcd.print(z);
  delay(400);
  lcd.print(z);
  lcd.print(z);
  delay(350);
  lcd.print(z);
  lcd.print(z);
  lcd.print(z);
  lcd.print(z);
  delay(500);
  lcd.clear();
  lcd.setCursor(3,0);
  lcd.print("<<CAS2.0>>");
  lcd.setCursor(2,1);
  lcd.print("Version: ");
  lcd.print(version);
  delay(3000);
  lcd.clear();
}

//Menüanimation
void setMenu(int sel) {
  int selected = sel;
  lcd.clear();
  lcd.backlight();
  switch(selected) {
    case 0:
      lcd.setCursor(0,0);
      lcd.print(" >a+b<x^n n! Q  ");
       lcd.setCursor(3,1);
      lcd.print("Arithmetik");
     return;
    case 1:
      lcd.setCursor(0,0);
      lcd.print("  a+b>x^n<n! Q  ");
      lcd.setCursor(2,1);
      lcd.print("Potenzieren");
     return;
    case 2:
      lcd.setCursor(0,0);
      lcd.print("  a+b x^n>n!<Q  ");
      lcd.setCursor(3,1);
      lcd.print("Fakultaet");
     return;
    case 3:
      lcd.setCursor(0,0);
      lcd.print("  a+b x^n n!>Q<  ");
      lcd.setCursor(3,1);
      lcd.print("Rechenquiz");
     return;
    default:
     return;
    break;
  }
}




