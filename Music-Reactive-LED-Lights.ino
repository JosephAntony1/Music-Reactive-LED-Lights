#define LOG_OUT 1 // use the log output function
#define FHT_N 128 // set to 128 point fht
#include <FHT.h>

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
void scanMusic()
{
  long add = 0;
  int count = 0;
  int average = 100;
  int peak = 100;
  boolean waitFlag = true;
  int wait = 0;
  while(knob>800&&knob<1000) { // reduces jitter
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

    int AVG_SAMPLE = 20;
    add += output;
    
    count = (count + 1)%AVG_SAMPLE;
    wait = (wait +1)%20;
    if(wait%5 == 0) waitFlag = true;
    if (count == 0) {average = (add)/AVG_SAMPLE + 2; add = 0;}
//     FOR DEBUGGING
    Serial.print("<");
    Serial.print(FHT_N/2);
    Serial.print(": Output: ");
    Serial.print(output);
    Serial.print(" - Average: " );
    Serial.print(average);
    Serial.print(" - Peak: " );
    Serial.print(peak);
    Serial.print(" - Waiting?: " );
    Serial.print(waitFlag);
    Serial.println(">");
    
    if((output>average || output>=peak )&& output > 70 && waitFlag){
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
delay(40);

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
