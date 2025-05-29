#include "freqlist.h"


#define PIN_POT A1
#define PIN_1VOCT A0
#define PIN_SYNC 2
#define PIN_OUT 4


struct Osc{
  float phase,step;
  void setFrequency(float f){
    step=(1.0/100000)*(f*256);
  }
  byte update(){
    float f=phase+step;
    if(f>=256) f-=256;
    return phase=f;
  }
};

Osc osc;

#define PRESC 335

ISR(TIMER1_COMPA_vect)
{
  OCR1A += PRESC;
  analogWrite(PIN_OUT,osc.update());
}

void sync(void){
  cli();
  osc.phase=0;
  sei();
}

static const float frequency[4095]={1};

void setup(){
  TCCR1A = 0;
  TCCR1B = B00000001;
  OCR1A = PRESC;
  TIMSK1 |= B00000010;


  attachInterrupt(digitalPinToInterrupt(PIN_SYNC), sync, FALLING);

  analogReference(DEFAULT);
  analogReadResolution(12);
  pinMode(PIN_OUT,ANALOG);
  pinMode(PIN_SYNC,INPUT_PULLUP);
  pinMode(9,OUTPUT);
  osc.setFrequency(110.2f);
  sei();
}

void loop() {
  
  float v2=analogRead(PIN_POT);
  v2/=5;
  float v=analogRead(PIN_1VOCT);
  v+=v2;
  if(v>4095) v=4095;
  osc.setFrequency(v);
}
