/* 
Programa: Control de Acceso a Puertas con Clave y Display LCD
Autor:  Humberto Higinio
Web: www.humbertohiginio.com
Canal de Youtube: https://www.youtube.com/user/HHSolis
Video Exclusivo para mi canal de Youtube
Todos los Derechos Reservados - 2016
Código de Dominio Público
*/

#include <Servo.h> 
#include <Password.h> //Incluimos la libreria Password
#include <Keypad.h> //Incluimos la libreria Keypad
#include <LiquidCrystal.h>  //Incluimos la libreria LiquidCrystal

Password password = Password("00000");  //Definimos el Password
int max_password = 5;                        //Largo del Password
int cont = 0;
String initial_password;
char key;
LiquidCrystal lcd(A0, A1, A2, A3, A4, A5); //Definimos los pines del LCD

String data ="";
Servo seguroPuerta;
int buzzer = 13; //Creamos las Variables de salida
int ledRed = 11; 
int ledGreen = 12;
 
int num_clicks; //Numero de Clicks
 
const byte ROWS = 4; // Cuatro Filas
const byte COLS = 4; // Cuatro Columnas

// Definimos el Keymap
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = { 9,8,7,6 };// Conectar los keypads ROW1, ROW2, ROW3 y ROW4 a esos Pines de Arduino.
byte colPins[COLS] = { 5,4,3,2, };// Conectar los keypads COL1, COL2, COL3 y COL4 a esos Pines de Arduino.
 
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
 
void setup()
{
  Serial.begin(9600);

  
  keypad.addEventListener(keypadEvent);  
  pinMode(ledRed, OUTPUT);  
  pinMode(ledGreen, OUTPUT);
  pinMode(buzzer, OUTPUT);
  seguroPuerta.attach(10);
  seguroPuerta.write(0);
  digitalWrite(ledRed, HIGH);
  digitalWrite(ledGreen, LOW);  
  
  lcd.begin(16, 2);  
  lcd.setCursor(0,0); 
  lcd.print("  *Bienvenido*");
  lcd.setCursor(0,1);
  lcd.print("FAVOR ENTRE PIN");
}
 
void loop()
{  
  keypad.getKey();
  
  if(Serial.available()){   
    data = (String)Serial.readString();
    
    if(data.length()>1){
      for(int i=0; i< data.length();i++){
      password.append(data[i]);
       }
      if(password.evaluate()){
         Serial.print("correcta");
         password.reset();
      }else{
         Serial.print("incorrecta");
         password.reset();
      }
    }else{
      if(data == "O"){
        openDoor();
      }
      if(data == "C"){
        closeDoor();
      }
    }
  }
}
void keypadEvent(KeypadEvent eKey)
{
  switch (keypad.getState())
  {
    case PRESSED:   
      int i;
      for( i = 1; i <= 1; i++ )
      {
        digitalWrite(buzzer, HIGH);  
        delay(200);            
        digitalWrite(buzzer, LOW);  
        delay(100);      
      }    
 
      /*Serial.print("Ingreso: ");
      Serial.println(eKey);*/
 
      switch (eKey)
      {
      
      case '#':              
        openDoor();
        break;    
       
      case '*':
        closeDoor();
        break;
      
       
      default:
      num_clicks=num_clicks+1;
      password.append(eKey);
      }
      if(num_clicks == 1)
      {
        lcd.clear();
        lcd.setCursor(1,0);
        lcd.print("< Contrasena >");
        lcd.setCursor(0,1);
        lcd.print("*_");
      }
      if(num_clicks == 2)
      {
        lcd.clear();
        lcd.setCursor(1,0);
        lcd.print("< Contrasena >");
        lcd.setCursor(0,1);
        lcd.print("**_");
       }
      if(num_clicks == 3)
      {
        lcd.clear();
        lcd.setCursor(1,0);
        lcd.print("< Contrasena >");
        lcd.setCursor(0,1);
        lcd.print("***_");
      }
      if(num_clicks == 4)
      {
        lcd.clear();
        lcd.setCursor(1,0);
        lcd.print("< Contrasena >");
        lcd.setCursor(0,1);
        lcd.print("****_");
      }
      if(num_clicks == 5)
      {
        lcd.clear();
        lcd.setCursor(1,0);
        lcd.print("< Contrasena >");
        lcd.setCursor(0,1);
        lcd.print("*****_");
      }
       
      if(num_clicks == max_password)
      {
        delay(250);
        checkPassword();
        num_clicks = 0;
      }
  }
}

void openDoor(){             
  seguroPuerta.write(90);
  delay(1000);
}

void closeDoor(){   
  seguroPuerta.write(0);        
  digitalWrite(buzzer, HIGH);  
  delay(500);            
  digitalWrite(buzzer, LOW);  
  delay(100);      
  digitalWrite(ledGreen, LOW);
  digitalWrite(ledRed, HIGH);    
  seguroPuerta.write(0);        
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("  *Bienvenido*");
  lcd.setCursor(0,1);
  lcd.print("FAVOR ENTRE PIN");  
}

void checkPassword()
{
  if (password.evaluate())
  {
    int i;
    for( i = 1; i <= 3; i++ )
    {
      digitalWrite(buzzer, HIGH);  
      delay(120);            
      digitalWrite(buzzer, LOW);  
      delay(70);      
    }    
    num_clicks = 0;
    password.reset();
    
   // Serial.println("Correcto");    
 
    digitalWrite(ledRed, LOW);
    digitalWrite(ledGreen, HIGH);
    seguroPuerta.write(90);
 
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("<<CONTRASENA>>");    
    lcd.setCursor(0,1);
    lcd.print("<<CORRECTA>>");    
 
    delay(5000);
    
  }  
  else  
  {
    int i;
    for( i = 1; i <= 1; i++ )
    {
      digitalWrite(buzzer, HIGH);  
      delay(300);            
      digitalWrite(buzzer, LOW);  
      delay(100);      
    }  
    num_clicks = 0;  
    password.reset();
 
    //Serial.println("Error");
 
    digitalWrite(ledGreen, LOW);
    digitalWrite(ledRed, HIGH);    
             
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("<<CONTRASENA>> ");    
    lcd.setCursor(0,1);
    lcd.print("<<ERRONEA>>");
    delay(2000);
   
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("  *Bienvenido*");
    lcd.setCursor(0,1);
    lcd.print("FAVOR ENTRE PIN");    
  }
}
/*
void setPassword(){
    lcd.clear();
    lcd.print("Ingrese la nueva ");
    lcd.setCursor(0,1);
    lcd.print("contrasena");  
    do{            
      key  = keypad.getKey();
      if(key != 0){
        initial_password.concat(key);  
        key = 0;  
        cont += 1;
      }            
      if(cont == 1)
      {
        lcd.clear();
        lcd.setCursor(1,0);
        lcd.print("< Contrasena >");
        lcd.setCursor(0,1);
        lcd.print("*_");
      }
      if(cont == 2)
      {
        lcd.clear();
        lcd.setCursor(1,0);
        lcd.print("< Contrasena >");
        lcd.setCursor(0,1);
        lcd.print("**_");
       }
      if(cont == 3)
      {
        lcd.clear();
        lcd.setCursor(1,0);
        lcd.print("< Contrasena >");
        lcd.setCursor(0,1);
        lcd.print("***_");
      }
      if(cont == 4)
      {
        lcd.clear();
        lcd.setCursor(1,0);
        lcd.print("< Contrasena >");
        lcd.setCursor(0,1);
        lcd.print("****_");
      }
      if(cont == 5)
      {
        lcd.clear();
        lcd.setCursor(1,0);
        lcd.print("< Contrasena >");
        lcd.setCursor(0,1);
        lcd.print("*****_");
      }
       
      if(num_clicks == max_password)
      {
        lcd.clear();
        lcd.setCursor(1,0);
        lcd.print("Contrasena establecida");        
        
        password.set(initial_password.c_str());
        
        digitalWrite(buzzer, HIGH);  
        delay(120);            
        digitalWrite(buzzer, LOW);  
        delay(70);   
        initial_password = "";                
        cont = 0;
      }
    }while(cont < 5 );    
   }*/
