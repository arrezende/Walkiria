/*
   Curso de Arduino WR Kits Aula 29
   
   www.wrkits.com.br
   facebook.com/wrkits
   youtube.com/user/canalwrkits
   
   Menu para Display LCD 16 x 2
   
   Autor: Eng. Wagner Rambo, Data: Agosto de 2015
*/

// --- Bibliotecas Auxiliares ---
#include <LiquidCrystal.h>                              //Biblioteca para o display LCD

// --- Mapeamento de Hardware ---
#define butUp    12                                     //Botão para selecionar tela acima no digital 12
#define butDown  11                                     //Botão para selecionar tela abaixo no digital 11
#define butP     10                                     //Botão de ajuste mais no digital 10
#define butM      9                                     //Botão de ajuste menos no digital 9
#define Lamp1    A0                                     //Saída para lâmpada 1 no A0 (será usado como digital)
#define Lamp2    A1                                     //Saída para lâmpada 2 no A1 (será usado como digital)
#define LM35     A2                                     //Entrada do LM35 - Sensor de temperatura
#define Buzzer   A5
float temp_atual = 0;
int ADClido = 0;

// -- Desenho --
byte termometro[8] = {B00100, B01010, B01010, B01110, B01110, B11111, B11111, B01110};
byte primeiro[8] = {
  B01110,
  B11011,
  B10001,
  B10001,
  B10001,
  B10001,
  B10001,
  B11111
};
byte segundo[8] = {
  B10001,
  B10001,
  B01110,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};

// --- Musica do Alarme ---

const int c = 261; //do 
const int d = 294; //re 
const int e = 329; //mi 
const int f = 349; //fa 
const int g = 391; //sol
const int gS = 415; //sol sustenido 
const int a = 440; //la 
const int aS = 455; //la sustenido 
const int b = 466; //si 
const int cH = 523; //do maior 
const int cSH = 554; //do maior sustenido 
const int dH = 587; //re maior 
const int dSH = 622; //re maior sustenido  
const int eH = 659; //mi maior sustenido
const int fH = 698; //fa maior 
const int fSH = 740; //fa maior sustenido 
const int gH = 784; //so maior 
const int gSH = 830; //so maior sustenido 
const int aH = 880;  //la maior 



// --- Protótipo das Funções Auxiliares ---
void changeMenu();                                      //Função para modificar o menu atual
void dispMenu();                                        //Função para mostrar o menu atual
void data_hora();                                       //Função do menu1, data e hora
void temperatura();                                     //Função do menu2, temperatura
void lights();                                          //Função do menu3, acionamento de lampadas
void menu4();                                           //Função do menu4
void alarme();

// --- Variáveis Globais ---
char menu = 0x01;                                       //Variável para selecionar o menu
char set1 = 0x00, set2 = 0x00;                          //Controle das lâmpadas
boolean t_butUp, t_butDown, t_butP, t_butM;             //Flags para armazenar o estado dos botões

// --- Hardware do LCD ---
LiquidCrystal disp(7,  //RS no digital 7
                   6,  //EN no digital 6
                   5,  //D4 no digital 5
                   4,  //D5 no digital 4
                   3,  //D6 no digital 3
                   2); //D7 no digital 2

// --- Configurações Iniciais ---
void setup()
{
  disp.begin(16,2);                                      //Inicializa LCD 16 x 2
  
  for(char i=9; i<13; i++) pinMode(i, INPUT_PULLUP);     //Entrada para os botões (digitais 9 a 12) com pull-ups internos
  
  pinMode(Lamp1, OUTPUT);                                //Configura saída para lâmpada 1
  pinMode(Lamp2, OUTPUT);                                //Configura saída para lâmpara 2
  pinMode(Buzzer, OUTPUT);
  t_butUp   = 0x00;                                      //limpa flag do botão Up
  t_butDown = 0x00;                                      //limpa flag do botão Down
  t_butP    = 0x00;                                      //limpa flag do botão P
  t_butM    = 0x00;                                      //limpa flag do botão M
  
  digitalWrite(Lamp1, LOW);                              //Lâmpada 1 inicia apagada
  digitalWrite(Lamp2, LOW);                              //Lâmpada 2 inicia apagada
  disp.createChar(1, primeiro);                          //Seta o desenho que fizemos
  disp.createChar(2, segundo);
} //end setup


// --- Loop Infinito ---
void loop()
{
    
   
   changeMenu();
   dispMenu();

} //end loop

// --- Desenvolvimento das Funções Auxiliares ---
void changeMenu()                                       //Modifica o menu atual
{
   if(!digitalRead(butUp))   t_butUp   = 0x01;          //Botão Up pressionado? Seta flag
   if(!digitalRead(butDown)) t_butDown = 0x01;          //Botão Down pressionado? Seta flag
      
   if(digitalRead(butUp) && t_butUp)                    //Botão Up solto e flag setada?
   {                                                    //Sim...
      t_butUp = 0x00;                                   //Limpa flag
      
      disp.clear();                                     //Limpa display
      menu++;                                           //Incrementa menu
      
      if(menu > 0x04) menu = 0x01;                      //Se menu maior que 4, volta a ser 1
   
   } //end butUp
   
   if(digitalRead(butDown) && t_butDown)                //Botão Down solto e flag setada?
   {                                                    //Sim...
      t_butDown = 0x00;                                 //Limpa flag
      
      disp.clear();                                     //Limpa display
      menu--;                                           //Decrementa menu
      
      if(menu < 0x01) menu = 0x04;                      //Se menu menor que 1, volta a ser 4
   
   } //end butDown   

} //end changeMenu

void dispMenu()                                         //Mostra o menu atual
{
    switch(menu)                                        //Controle da variável menu
    {
      case 0x01:                                       //Caso 1
             disp.setCursor(0,0);                                 //Posiciona cursor na coluna 1, linha 1
             disp.print("Bem Vindo á");
             disp.write(1);
             disp.setCursor(0,1); 
             disp.print("Walkiria 0.1");
             disp.write(2); //escreve o desenho que fizemos
             
             break;    
       case 0x02:                                       //Caso 1
             alarme();                               //Chama a função de relógio
             
             break;                                     //break
       case 0x03:                                       //Caso 2
             temperatura();                             //Chama a função do termômetro
             
             break;                                     //break
       case 0x04:                                       //Caso 3
             lights();                                  //Chama função para acionamento de lâmpadas
            
             break;                                     //break
       case 0x05:                                       //Caso 4
             menu4();                                   //Chama função para o menu4
             
             break;                                     //break
      
    } //end switch menu

} //end dispMenu

void data_hora()                                        //Data e Hora (menu1)
{
    
   disp.setCursor(0,0);                                 //Posiciona cursor na coluna 1, linha 1
   disp.print("Data e Hora");                           //Imprime mensagem
   disp.setCursor(0,1);                                 //Posiciona cursor na coluna 1, linha 2
   
   // Desenvolver uma função de data e hora...
   
   disp.print("27/08/15  15:14");                       //Mostrador (apenas ilustrativo......)

} //end data_hora

void temperatura()                                      //Temperatura (menu2)
{
   disp.setCursor(0,0);                                 //Posiciona cursor na coluna 1, linha 1
   disp.print("Temperatura");                           //Imprime mensagem
   disp.setCursor(1,1);                                 //Posiciona cursor na coluna 2, linha 2
   
   // Desenvolver uma função de data e hora...
   
   //disp.print("25 Celsius");                          //Mostrador (apenas ilustrativo......)   

   ADClido = analogRead(LM35); 
   //temp_atual = ADClido * 0.4887585532746823;           //no Leonardo use 0.4887585532
   temp_atual = (float(analogRead(LM35))*5/(1023))/0.01;
   disp.print(temp_atual);                              //Imprime a temperatura obtida no LCD
   disp.print((char)223);                               // Exibe o caractere º
   disp.print("Celsius"); 
} //end temperatura()
 
void lights()                                           //Acionamento de lâmpadas (menu3)
{
  
   disp.setCursor(0,0);                                 //Posiciona cursor na coluna 1, linha 1
   disp.print("Acionar Lampadas");                      //Imprime mensagem do menu 3
  
  
   if(!digitalRead(butP))    t_butP    = 0x01;          //Botão P pressionado? Seta flag
   if(!digitalRead(butM))    t_butM    = 0x01;          //Botão M pressionado? Seta flag
   
   if(digitalRead(butP) && t_butP)                      //Botão P solto e flag setada?
   {                                                    //Sim...
      t_butP = 0x00;                                    //Limpa flag
      
      set1++;                                           //Incrementa set1
      
      if(set1 > 2) set1 = 0x01;                         //Se maior que 2, volta a ser 1
      
      switch(set1)                                      //Controle do set1
      {
          case 0x01:                                    //Caso 1
                disp.setCursor(0,1);                    //Posiciona cursor na coluna 1, linha 2
                disp.print("L1 on ");                   //Imprime mensagem
                digitalWrite(Lamp1, HIGH);              //Liga lâmpada 1
                break;                                  //Break
          case 0x02:                                    //Caso 2
                disp.setCursor(0,1);                    //Posiciona cursor na coluna 1, linha 2
                disp.print("L1 off");                   //Imprime mensagem
                digitalWrite(Lamp1, LOW);               //Desliga lâmpada 1
                break;                                  //Break
      
      } //end switch set1
   
   } //end butP
   
   if(digitalRead(butM) && t_butM)                      //Botão D solto e flag setada?
   {                                                    //Sim...
      t_butM = 0x00;                                    //Limpa flag
      
      set2++;                                           //Incrementa set2
      
      if(set2 > 2) set2 = 0x01;                         //Se maior que 2, volta a ser 1
      
      switch(set2)                                      //Controle do set2
      {
          case 0x01:                                    //Caso 1
                disp.setCursor(8,1);                    //Posiciona cursor na coluna 8, linha 2
                disp.print("L2 on ");                   //Imprime mensagem
                digitalWrite(Lamp2, HIGH);              //Liga lâmpada 2
                break;                                  //Break
          case 0x02:                                    //Caso 2
                disp.setCursor(8,1);                    //Posiciona cursor na coluna 8, linha 2
                disp.print("L2 off");                   //Imprime mensagem
                digitalWrite(Lamp2, LOW);               //Liga lâmpada 1
                break;                                  //Break
      
      } //end switch set1
   
   } //end butM


} //end lights

void menu4()                                            //Função genérica para um quarto menu...
{
   disp.setCursor(0,0);                                 //Posiciona cursor na coluna 1, linha 1
   disp.print("Menu 4");                                //Imprime mensagem
   disp.setCursor(7,1);                                 //Posiciona cursor na coluna 8, linha 2
   
   // Desenvolver uma função de data e hora...
   
   disp.print("WR Kits");                               //Créditos  


} //end menu4

void alarme()                                            //Função genérica para um quarto menu...
{
   disp.setCursor(0,0);                                 //Posiciona cursor na coluna 1, linha 1
   disp.print("Alarme");                                //Imprime mensagem
   disp.setCursor(7,1);                                 //Posiciona cursor na coluna 8, linha 2
    
    if(!digitalRead(butP))    t_butP    = 0x01;          //Botão P pressionado? Seta flag
   if(!digitalRead(butM))    t_butM    = 0x01;          //Botão M pressionado? Seta flag
   
   if(digitalRead(butP) && t_butP)                      //Botão P solto e flag setada?
   {                                                    //Sim...
      t_butP = 0x00;                                    //Limpa flag
      
      set1++;                                           //Incrementa set1
      
      if(set1 > 2) set1 = 0x01;                         //Se maior que 2, volta a ser 1
      
      switch(set1)                                      //Controle do set1
      {
          case 0x01:                                    //Caso 1
                disp.setCursor(0,1);                    //Posiciona cursor na coluna 1, linha 2
                disp.print("L1 on ");                   //Imprime mensagem
                //digitalWrite(Lamp1, HIGH);              //Liga lâmpada 1
                 //toca a primeira parte
                parte1();
                //toca a segunda parte
                parte2();  
                variacao();
                break;                                  //Break
          case 0x02:                                    //Caso 2
                disp.setCursor(0,1);                    //Posiciona cursor na coluna 1, linha 2
                disp.print("L1 off");                   //Imprime mensagem
                //digitalWrite(Lamp1, LOW);               //Desliga lâmpada 1
                noTone(Buzzer);
                break;                                  //Break
      
      } //end switch set1
   
   } //end butP 
   
   
   //disp.print("WR Kits");                               //Créditos  


} //end menu4
void beep(int nota, int duracao)
{
//toca a nota no pino 3
  tone(Buzzer, nota, duracao);
 

   delay(duracao);

//Para o tom no pino do alto falante
  noTone(Buzzer);
 
  delay(50);

}

void parte1()
{
  beep(a, 500);
  beep(a, 500);    
  beep(a, 500);
  beep(f, 350);
  beep(cH, 150);  
  beep(a, 500);
  beep(f, 350);
  beep(cH, 150);
  beep(a, 650);
 
  delay(500);
 
  beep(eH, 500);
  beep(eH, 500);
  beep(eH, 500);  
  beep(fH, 350);
  beep(cH, 150);
  beep(gS, 500);
  beep(f, 350);
  beep(cH, 150);
  beep(a, 650);
 
  delay(500);
}

//segunda parte
void parte2()
{
  beep(aH, 500);
  beep(a, 300);
  beep(a, 150);
  beep(aH, 500);
  beep(gSH, 325);
  beep(gH, 175);
  beep(fSH, 125);
  beep(fH, 125);    
  beep(fSH, 250);
 
  delay(325);
 
  beep(aS, 250);
  beep(dSH, 500);
  beep(dH, 325);  
  beep(cSH, 175);  
  beep(cH, 125);  
  beep(b, 125);  
  beep(cH, 250);  
 
  delay(350);
}

//variacao
void variacao(){
//Variacao 1
beep(f, 250);  
  beep(gS, 500);  
  beep(f, 350);  
  beep(a, 125);
  beep(cH, 500);
  beep(a, 375);  
  beep(cH, 125);
  beep(eH, 650);
 
  delay(500);

//repete a segunda parte
  parte2();

//segunda variacao da musica
  beep(f, 250);  
  beep(gS, 500);  
  beep(f, 375);  
  beep(cH, 125);
  beep(a, 500);  
  beep(f, 375);  
  beep(cH, 125);
  beep(a, 650);  
 
  delay(650);
}

   //pode-se implementar quantos menus quiser
   //............
