#define LOG_OUT 1 // use the log output function
#define FHT_N 128 // set to 128 point fht
#include <FHT.h>

#define MIC_PIN   0
  int redPin = 10;
  int greenPin = 9;
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
  int knob = analogRead(2);

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
void scanMusic()
{
  long add = 0;
  int count = 0;
  int average = 100;
  int peak = 100;
  boolean waitFlag = true;
  int wait = 0;
  while(analogRead(2)>800&&analogRead(2)<1000) { // reduces jitter
    for (int i = 0 ; i < FHT_N ; i++)   // save 256 samples
    {
      int sample = analogRead(MIC_PIN);
      fht_input[i] = sample; // put real data into bins
    }
    fht_window(); // window the data for better frequency response
    fht_reorder(); // reorder the data before doing the fht
    fht_run(); // process the data in the fht
    fht_mag_log(); // take the output of the fht
    int output = fht_log_out[0];
    add += output;
    count = (count + 1)%20;
    wait = (wait +1)%20;
    if(wait%5 == 0) waitFlag = true;
    if (count == 0) {average = (add + peak*25)/45-2; add = 0;}
    Serial.print("<");
    Serial.print(FHT_N/2);
    Serial.print(":");
    Serial.print(output);
    Serial.print(" - " );
    Serial.print(average);
    Serial.print(" - " );
    Serial.print(peak);
    Serial.print(" - " );
    Serial.print(waitFlag);
    Serial.println(">");
    if((output>=average || output>=peak )&& output > 70 && waitFlag){
      red = rand()%255;
      blue = rand()%100;
      green = rand()%100;
      setColor(red,green, blue);
      Serial.println("o o o o o o o o o o o o o o o o o o o o o o o");
      delay(20);
      peak = (output + 10 + 146)/2;
      waitFlag = false;
      wait = 0;
      }
    fadeRandom();
  }
}
void white(){
  setColor(255,100,130);
}
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
delay(10);
}
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
