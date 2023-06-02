#include <16f876a.h>
#device ADC=10
#fuses HS,NOWDT,PROTECT,NOLVP
#use delay(clock=4M)
#define lcd_data_port getenv("sfr:portb")
#define LCD_RS_PIN PIN_B1
#define LCD_RW_PIN PIN_B2
#define LCD_ENABLE_PIN PIN_B3
#include <lcd.c>

int1  SegPar, BtnPediluvio, BtnZapper;
int   Min,Seg/*,Anim*/,i;
int num[61]={0,0,1,2,3,4,5,6,7,8,9,
               10,11,12,13,14,15,16,17,18,19,
               20,21,22,23,24,25,26,27,28,29,
               30,31,32,33,34,35,36,37,38,39,
               40,41,42,43,44,45,46,47,48,49,
               50,51,52,53,54,55,56,57,58,59};
int16 q;
float Amp;

void Pitido()
{
   setup_timer_2(T2_DIV_BY_16,155,2);  // PWM 200.3KHz
   setup_ccp2(CCP_PWM);
   delay_ms(75);
   setup_ccp2(CCP_OFF);
   setup_timer_2(T2_DISABLED,1,1);
}
void Animacion()  // * ZELECTRON CA *
{
   delay_ms(250);
   lcd_gotoxy(1,1);
   printf(lcd_putc,"  Z             ");
   delay_ms(100);
   lcd_gotoxy(1,1);
   printf(lcd_putc,"  ZE            ");
   delay_ms(100);
   lcd_gotoxy(1,1);
   printf(lcd_putc,"  ZEL           ");
   delay_ms(100);
   lcd_gotoxy(1,1);
   printf(lcd_putc,"  ZELE          ");
   delay_ms(100);
   lcd_gotoxy(1,1);
   printf(lcd_putc,"  ZELEC         ");
   delay_ms(100);
   lcd_gotoxy(1,1);
   printf(lcd_putc,"  ZELECT        ");
   delay_ms(100);
   lcd_gotoxy(1,1);
   printf(lcd_putc,"  ZELECTR       ");
   delay_ms(100);
   lcd_gotoxy(1,1);
   printf(lcd_putc,"  ZELECTRO      ");
   delay_ms(100);
   lcd_gotoxy(1,1);
   printf(lcd_putc,"  ZELECTRON     ");
   delay_ms(100);
   lcd_gotoxy(1,1);
   printf(lcd_putc,"  ZELECTRON C   ");
   delay_ms(100);
   lcd_gotoxy(1,1);
   printf(lcd_putc,"  ZELECTRON CA  ");
   delay_ms(1000);
   
/*   printf(lcd_putc,"* ZELECTRON CA *");
   delay_ms(500);
   for(Anim=0;Anim<3;Anim++)  // Animación de Inicio
   {
      printf(lcd_putc,"\n>");
      delay_ms(25);
      printf(lcd_putc,"\n >");
      delay_ms(25);
      printf(lcd_putc,"\n  >");
      delay_ms(25);
      printf(lcd_putc,"\n   >");
      delay_ms(25);
      printf(lcd_putc,"\n    >");
      delay_ms(25);
      printf(lcd_putc,"\n     >");
      delay_ms(25);
      printf(lcd_putc,"\n      >");
      delay_ms(25);
      printf(lcd_putc,"\n       >");
      delay_ms(25);
      printf(lcd_putc,"\n        >");
      delay_ms(25);
      printf(lcd_putc,"\n         >");
      delay_ms(25);
      printf(lcd_putc,"\n          >");
      delay_ms(25);
      printf(lcd_putc,"\n           >");
      delay_ms(25);
      printf(lcd_putc,"\n            >");
      delay_ms(25);
      printf(lcd_putc,"\n             >");
      delay_ms(25);
      printf(lcd_putc,"\n              >");
      delay_ms(25);
      printf(lcd_putc,"\n               >");
      delay_ms(25);
      printf(lcd_putc,"\n                >");
      delay_ms(500);
   }*/
   printf(lcd_putc,"\f");
}
void LeerADC()
{
   q=read_adc();
   Amp=(5.0*q)/1024.0;
}
void Mostrar1()
{
   printf(lcd_putc,"\nTMP %02d:%02d   %1.1fA",num[Min],num[Seg],Amp);
}
void Mostrar2()
{
   printf(lcd_putc,"\nTMP %02d %02d   %1.1fA",num[Min],num[Seg],Amp);
}
void Exceso()
{
   while(Amp>1.4) // EXCESO DE SOLUCION
   {
      lcd_gotoxy(1,1);
      printf(lcd_putc,"EXCESO  SOLUCION");
      LeerADC();
      printf(lcd_putc,"\n      %1.1fA      ",Amp);
      Pitido();
   }
}
void Pediluvio()
{
   BtnPediluvio=0;
   BtnZapper=1;
   lcd_gotoxy(1,1);
   printf(lcd_putc,"*  FOOTSHOWER  *");
   printf(lcd_putc,"\n      %1.1fA      ",Amp);
   Exceso();
}
void Estado()
{
   if(!input(PIN_A1)) // B-Inicio
   {
      while(!input(PIN_A1))
         {
            delay_ms(150);                                                       // Antirrebote
         }
      lcd_gotoxy(1,1);
      printf(lcd_putc,"PROCESO DETENIDO");
      setup_ccp1(CCP_OFF);
      setup_timer_2(T2_DISABLED,1,1);      
      Pitido();
      delay_ms(100);
      Pitido();
      delay_ms(100);
      Pitido();
      delay_ms(2000);
      reset_cpu();
   }
   if(!input(PIN_A2)) // B-Zapper
   {
      Pitido();
      BtnZapper++;
      while(!input(PIN_A2))    // Antirrebote
      {
         delay_ms(150);
      }
   }
}
void PediluvioOperando()
{
   for(Min=60;Min>0;Min--)
   {
      for(Seg=60;Seg>0;Seg--)
      {
         lcd_gotoxy(1,1);
         if(SegPar==1)
         {
            printf(lcd_putc," FOOTSHOWER  ON ");
         }
         if(SegPar==0)
         {
            if(BtnZapper==1)
            {
               printf(lcd_putc," ZAPPER      ON ");
               setup_timer_2(T2_DIV_BY_1,32,1);   // PWM frequency to 30.3KHz
               set_pwm1_duty(128);
               setup_ccp1(CCP_PWM);
            }
            if(BtnZapper==0)
            {
               printf(lcd_putc," ZAPPER     OFF ");
               setup_ccp1(CCP_OFF);
               setup_timer_2(T2_DISABLED,1,1);
            }
         }
         for(i=0;i<10;i++)
         {
            Estado();
            LeerADC();
            Exceso();
            Mostrar1();
            delay_ms(21);
         }
         for(i=0;i<10;i++)
         {
            Estado();
            LeerADC();
            Exceso();
            Mostrar2();
            delay_ms(22);
         }
         SegPar++;
      }
   }
   lcd_gotoxy(1,1);
   printf(lcd_putc,"\f FIN DE TERAPIA ");
   setup_ccp1(CCP_OFF);
   setup_timer_2(T2_DISABLED,1,1);
   Pitido();
   delay_ms(100);
   Pitido();
   delay_ms(100);
   Pitido();
   delay_ms(2000);
   reset_cpu();
}
void main()
{
   lcd_init();
   set_tris_a(0xFF);       // Puerto A como Entrada
   set_tris_b(0x00);       // Puerto B como Salida
   output_b(0x00);         // Puerto B inicia en cero
   set_tris_c(0x00);       // Puerto C como Salida
   output_c(0x00);         // Puerto C inicia en cero
   setup_adc_ports(an0);   // Habilito AN0 como pueto ADC
   setup_adc(adc_clock_internal);   // ADC con fuente de reloj interno
   set_adc_channel(0);
   delay_ms(1);
   Animacion();

   BtnPediluvio=0;
   BtnZapper=1;
   output_high(PIN_C0);
   
   while(true)
   {
//    PEDILUVIO
 
      LeerADC();
      Pediluvio();
      
//    BOTONES

      if(!input(PIN_A1)) // B-Inicio
      {
         //Pitido();
         PediluvioOperando();
      }
   }
}
