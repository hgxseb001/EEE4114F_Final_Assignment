#include <Arduino.h>
#include "esp_timer.h"


//!specify filter type:

// #define NO_FILTER
// #define DFT
// #define FIR
#define IIR


//! Choose to implement filters or perform time measurements

#define COMPUTE
// #define MEASURE_TIME




#ifdef DFT
#include "DFT.h"
#endif

#ifdef FIR
#include "FIR.h"
#endif

#ifdef IIR
#include "IIR.h"
#endif

#define ADC_PIN 14

float analogVal = 0;

unsigned long start;
unsigned long end;

// hw_timer_t *timer = NULL;
// void IRAM_ATTR onTimer(){
//   // analogVal = (analogRead(ADC_PIN)* 5 ) / (2047);
//   Serial.println("helloWorld");
// }


void setup(void){
  Serial.begin(115200);
  pinMode(ADC_PIN,INPUT);
  adcAttachPin(ADC_PIN);
  analogReadResolution(11); // from an example, normally 12?
  // analogSetAttenuation(ADC_6db);  // Filtering?

  #ifdef IIR
  initIIRArrays();
  #endif

  // timer = timerBegin(2, 80, true);
  // timerAttachInterrupt(timer, &onTimer, true);
  // timerAlarmWrite(timer, 1000000, true);
  // timerAlarmEnable(timer); //Just Enable
  
  
 
}

#ifdef COMPUTE

void loop(void){  

  #ifdef NO_FILTER
  Serial.println((analogRead(ADC_PIN)* 5.0 ) / (2047));
  #endif

  #ifdef DFT

  for (int i=0; i<len; i++){
    xi[i] = (analogRead(ADC_PIN)* 5.0 ) / (2047);
  }

  calculate_DFT();

  // apply ideal filter
  for (int i=len/2; i<len; i++){
    Xr[i] = 0;
    Xi[i] = 0;
  }

  calculate_IDFT();

  for (int i=0; i<len; i++){
    Serial.println(xo[i]);
  }

  #endif
  
  #ifdef FIR
  
  for (int i=0; i<sampleSize; i++){
    x[i] = (analogRead(ADC_PIN)* 5.0 ) / (2047);
  }
  discreteConvolve();
  
  for (int i=0; i<convSize; i++){
    Serial.println(convOut[i]);
  }

  #endif

  #ifdef IIR

    analogVal = (analogRead(ADC_PIN)* 5.0 ) / (2047);
    Serial.println(compute_next(analogVal));
  
  #endif
  
}

#endif


#ifdef MEASURE_TIME
void loop(void){  


  float Mavg = 0;
  float Cavg = 0;
  
  float temp = 0;

  #ifdef DFT

  for (int j=0; j<10; j++){

    start = micros();
    for (int i=0; i<len; i++){
      // start = micros();
      xi[i] = (analogRead(ADC_PIN)* 5.0 ) / (2047);
      // delayMicroseconds(27);
      // end = micros();
    }
    end = micros();
    Mavg += (end - start);

    start = micros();
    calculate_DFT();

    // apply ideal filter
    for (int i=511; i<len; i++){
      Xr[i] = 0;
      Xi[i] = 0;
    }

    calculate_IDFT();
    end = micros();
    Cavg += (end - start);

    // for (int i=0; i<len; i++){
    //   Serial.println(xo[i]);
    // }

  }
  Mavg /= 10;
  Cavg /= 10;
  Serial.printf("%f, %f\n", Mavg, Cavg);


  

  #endif
  
  #ifdef FIR
  for (int i=0; i<10; i++){

    start = micros();
    for (int i=0; i<sampleSize; i++){
      x[i] = (analogRead(ADC_PIN)* 5.0 ) / (2047);
      // delayMicroseconds(27);
      end = micros();
    }
    Mavg += (end - start);
    

    start = micros();
    discreteConvolve();
    end = micros();
    Cavg += (end - start);
   

    
    // for (int i=0; i<convSize; i++){
    //   Serial.println(convOut[i]);
    // }

  }
  Mavg /= 10;
  Cavg /= 10;
  Serial.printf("%f, %f\n", Mavg, Cavg);
  
  // Serial.println(end-start);
  #endif

  #ifdef IIR

  // analogVal = (analogRead(ADC_PIN)* 5.0 ) / (2047);
  float Mavg = 0;
  float Cavg = 0;
  
  float temp = 0;

  for (int i=0; i< 100; i++){

    start = micros();
    analogVal = (analogRead(ADC_PIN)* 5.0 ) / (2047);
    end = micros();
    Mavg += (end - start);

    start = micros();
    temp = compute_next(analogVal);
    end = micros();
    Cavg += (end - start);
  }
  Mavg /= 100;
  Cavg /= 100;
  // Serial.println(temp);

  Serial.printf("%f, %f\n", Mavg, Cavg);


  delayMicroseconds(27);
  delayMicroseconds(900);
  #endif
  
}
#endif




