#define LOG_OUT 1 // use the log output function
#define FHT_N 128 // set to 128 point fht
#include <FHT.h>
#include <stdio.h> // for function sprintf

#define MIC_PIN   0
  int redPin = 9;
  int greenPin = 10;
  int bluePin = 11;
  
  int red = 255;
  int blue = 100;
  int green = 100;
  
  int red2 = 255;
  int green2 = 0;
  int blue2 = 0;
  int knob;
void setup()
{
  Serial.begin(115200);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  setColor(rand()%255,rand()%255, rand()%255);

}
  
  void loop() {
  knob = 900;

    if(knob > 1000)
      white();
    else if(knob>800)
       scanMusic();
    else if(knob>200)
      fadeRandom(); 

    else
      nightMode();  
      delay(1);
 }
// the loop routine runs over and over again forever:

//Analyzes the music attempting to locate the bass drum

int count = 0;
long add = 0;
int average = 100;
int peak = 0;
boolean isWaiting = false;
int wait = 0;

void scanMusic()
{
    for (int i = 0 ; i < FHT_N ; i++)   // save 256 samples
    {
      int sample = analogRead(MIC_PIN);
      fht_input[i] = sample; // put real data into bins
    }
    fht_window(); // window the data for better frequency response
    fht_reorder(); // reorder the data before doing the fht
    fht_run(); // process the data in the fht
    fht_mag_log(); // take the output of the fht
    int output = fht_log_out[3];

    int delayTime = 10;
    int AVG_SAMPLE = 40;
    add += output;

    count = (count + 1)% AVG_SAMPLE;
    wait = (wait +1)%delayTime;
    if(wait == 0) {isWaiting = false;}
    if (count == 0) {average = (add)/AVG_SAMPLE + 5; add = 0;}
//     FOR DEBUGGING
//    for (int z=0; z<5; z++){
//     char buf [3];
//     sprintf(buf, "%03d", fht_log_out[z]);
//      Serial.print(buf); Serial.print(" ") ;
//     }
    Serial.print(" ");
    Serial.print(FHT_N/2);
    Serial.print(" ");
    Serial.print(output);
    Serial.print(" " );
    Serial.print(average);
    Serial.print(" " );
    Serial.print(peak);
    Serial.print(" " );
    Serial.print(isWaiting);
    Serial.println();
    
    if((output>average+3 || output>=peak )&& output >=70 && !isWaiting){
      red = rand()%255;
      blue = rand()%100;
      green = rand()%100;
      setColor(red,green, blue);
      //Serial.println("o o o o o o o o o o o o o o o o o o o o o o o");
      delay(20);
      peak = (output);
      isWaiting = true;
      wait = 1;
      average = (output + average) / 2;
      }
    fadeRandom();
  
}

//Homework Mode
void white(){
  setColor(225,255,255);
}

//Not listen to music, just fade randomly
void fadeRandom(){
if(red!=red2) 
    red = red + ((red2-red)/abs(red2-red));

 if(green!=green2)
    green = green + ((green2-green)/abs(green2-green));

 if(blue!=blue2)
  blue = blue + ((blue2-blue)/abs(blue2-blue));

 else if(red == red2 && green == green2 && blue == blue2){
    red2=(red2+200)%205;
    green2=(green2+143)%100;
    blue2 = (blue2+231)%50;
  }
   setColor(red, green, blue);
delay(5);

}

//Designed to Help you sleep better, unknown if it actually works
void nightMode(){
  boolean blank = false;
  int redFade = 30;
while (analogRead(2)<400){
if(blank)
  redFade ++;
else
  redFade --;

if(redFade == 0){
  blank = true;
  delay(1500);
}
if(redFade == 30)
blank = false;
  
  setColor(redFade, 0, 0);

  delay(70);
}
}

 //the real mvp
 void setColor(int red, int green, int blue)
{
  if(red<0)
  red=0;

  if(green<0)
  green=0;

  if(blue<0)
  blue=0;
  
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);  
}
