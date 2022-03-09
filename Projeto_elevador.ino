// Projeto de um elevador simulado no Tinkercad.


//**** BIBLIOTECAS ********
#include <LiquidCrystal.h>  // Biblioteca para LCD
#include <Keypad.h> // Biblioteca para controlar o teclado
#include <Adafruit_NeoPixel.h>


#define LIN 1 // Número de linhas do teclado
#define COL 4 // Número de colunas do teclado
#define pwm_1 10
#define pwm_2 11
#define elevador 2  //fita de leds representando o elevador
#define pino_botao A5

LiquidCrystal lcd(4, 5, 6, 7, 8, 9); // Pinos que o LCD precisa para funcionar
Adafruit_NeoPixel strip = Adafruit_NeoPixel(8, elevador, NEO_RGB + NEO_KHZ800);

char teclas[LIN][COL] = {{'1', '2', '3'}}; // Primeira linha
byte linhas[LIN] = {A4}; // Pinos das linhas
byte colunas[COL] = {A3, A2, A1}; // Pinos das colunas
Keypad teclado = Keypad(makeKeymap(teclas), linhas, colunas, LIN, COL); //Obj teclado tipo Kaypad
int reset_cursor = 0;

int pos =1;
int bot=6;
int subir=0;
int LED=6;
int menu=0;
int tecla =0;
int tecla2=0;
int dest;
int dest2=0;

void setup()
{
  // *INICIALIZACAO LCD*
  lcd.begin(16, 2); // Inicia o display LCD 
  lcd.setCursor(0, 0); // Coloca o cursor na coluna 0, linha 0
  lcd.print("ELEVADOR ATIVADO"); //Escreve no display
  delay(500);
  lcd.clear();
  
  // *INICIALIZACAO LEDS*
  strip.begin();
  strip.setPixelColor(6, 255, 0, 0); // ELEVADOR NO TERREO
  strip.show();
  
  
  
  pinMode(10, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(A5, INPUT);
  Serial.begin(9600);
  analogWrite(pwm_1,0);   // motor parado
  analogWrite(pwm_2,0);   //motor parado
           
}

void loop()
{
  
    
  if(LED==bot){     // se elevador já estiver no andar
    if(menu==0){
      atualiza_lcd();              
      }
    
    } else {            // elevador sendo chamado
    levar_elevador();
    analogWrite(pwm_1,0);   // motor PARADO
    analogWrite(pwm_2,0);
    menu=0;
    atualiza_lcd();
    }    
    principal();
    
//    atualiza_pos();

    
 

  
  
  Serial.println(bot);
  Serial.println(LED);
  Serial.println(tecla);
}

void principal(){     
      while(tecla2== NO_KEY && bot==LED){
      tecla2 = teclado.getKey();
      botoes();        
      }             
      tecla=tecla2;
  if(bot !=LED){
     levar_elevador();
    analogWrite(pwm_1,0);   // motor PARADO
    analogWrite(pwm_2,0);
    menu=0;
    atualiza_lcd();    
  }else{
    destino();
  }
      
       
}

void botoes() 
{
   int botao = analogRead(pino_botao); 
  
    if (botao == 731){
      bot=0;
      subir =0;
      
    }else if (botao == 787){
      bot=3;
      subir =1;
    }else if (botao == 852){
      bot=3;
      subir =0;
    }else if (botao==930) {
      bot=6;
      subir =1;
    }
    
}
void levar_elevador(){
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Chamando"); //Escreve no display
      lcd.setCursor(0, 1);
      lcd.print("Elevador..."); //Escreve no display 
      while(bot>LED){
          analogWrite(pwm_1,0);   // motor antihorario
          analogWrite(pwm_2,1);                      
          LED++;
          atualiza_pos();

      }
      while (bot<LED){
          analogWrite(pwm_1,1);   // motor horario
          analogWrite(pwm_2,0);                     
          LED--;
          atualiza_pos();    
      }
}

void atualiza_pos(){ 


    
    if (LED ==bot){
    
    strip.clear();
    strip.setPixelColor(LED, 255, 0, 0); // ELEVADOR NO ANDAR
    strip.show();
    delay(500);
       
    } 
    else{
    
    strip.clear();
    strip.setPixelColor(LED, 0, 255, 0); // ELEVADOR EM MOVIMENTO
    strip.show();
    delay(500);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Chamando"); //Escreve no display 
    lcd.setCursor(0, 1);
    lcd.print("Elevador..."); //Escreve no display   
    }
}

void atualiza_lcd(){
  if(menu==1){
    pos =(-LED/3)+3;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Parado"); 
    lcd.setCursor(0, 1);
    lcd.print("POSICAO: "); // atualiza posicao do elevador 
    lcd.print(pos); // 
    menu=0;
  } else if (menu ==0){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Para qual andar?");
    lcd.setCursor(0, 1);
    lcd.print("Digite ao lado <");
    
  }else if (menu ==3){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Subindo para:");
    lcd.setCursor(0, 1);    
    lcd.print(dest2);
    
  }else if (menu ==4){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Descendo para:");
    lcd.setCursor(0, 1);
    lcd.print(dest2);
  }
  
}
void destino(){
    dest = (-3*tecla)+153;
    dest2=tecla-48;
  if(dest!=LED){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Indo ao destino"); //Escreve no display 
  }
      while(dest>LED){
          analogWrite(pwm_1,0);   // motor antihorario
          analogWrite(pwm_2,1);                      
          LED++;
           menu=4;           
          atualiza_pos2();

      }
      while (dest<LED){
          analogWrite(pwm_1,1);   // motor horario
          analogWrite(pwm_2,0);                     
          LED--;
          menu=3;
          atualiza_pos2();    
      }


//    tecla=0;
    menu=2;
    bot=LED;
    dest=LED;
    tecla2=NO_KEY;
    
    analogWrite(pwm_1,0);   // motor PARADO
    analogWrite(pwm_2,0);
}

void atualiza_pos2(){ 


    
    if (LED ==dest){
    
    strip.clear();
    strip.setPixelColor(LED, 255, 0, 0); // ELEVADOR NO ANDAR
    strip.show();
    delay(500);
    menu=0;
  atualiza_lcd();
    
    
    } 
    else{
    
    strip.clear();
    strip.setPixelColor(LED, 0, 255, 0); // ELEVADOR EM MOVIMENTO
    strip.show();
    delay(500);
    atualiza_lcd();
  
       
    }
}
