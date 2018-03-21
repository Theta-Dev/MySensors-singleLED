// Software for the MySensors singleLED wireless LED controller
// project by ThetaDev (thdev.org)
// licensed under the CC-BY-SA license
//
// Credits to LastSamurai for the LED fading algorithm

#include <OneButton.h>

// Enable debug prints to serial monitor
//#define MY_DEBUG
//#define MY_BAUD_RATE 38400

// Enable and select radio type attached
#define MY_RADIO_RFM69
#define MY_RFM69_NETWORKID 0
#define MY_IS_RFM69HW

//Security (before uncommenting any of these lines, run the SecurityPersonalizer sketch on your microcontroller)
//#define MY_RFM69_ENABLE_ENCRYPTION
//#define MY_SIGNING_SOFT
//#define MY_SIGNING_SOFT_RANDOMSEED_PIN A5
//#define MY_SIGNING_REQUEST_SIGNATURES

//Status LEDs
#define MY_DEFAULT_LED_BLINK_PERIOD 300
#define MY_WITH_LEDS_BLINKING_INVERSE
#define MY_DEFAULT_ERR_LED_PIN 8  // Error led pin
#define MY_DEFAULT_RX_LED_PIN  7  // Receive led pin
#define MY_DEFAULT_TX_LED_PIN  7  // the PCB, on board LED

#define SN "singleLED"
#define SV "1.0"

#include <MySensors.h>

#define SENSOR_ID 1
#define FADE_DELAY 6  // Delay in ms for each percentage fade up/down (10ms = 1s full-range dim)
#define LED_PIN 9

byte ledLevel;
boolean ledDimWay;

OneButton bt1(3, false);

void setup()
{
  bt1.attachClick(click1);
  bt1.attachDuringLongPress(press1);
  bt1.attachLongPressStart(pstart1);
  bt1.attachLongPressStop(pstop1);
  
  // Pull the gateway's current dim level - restore light level upon sendor node power-up
  request(SENSOR_ID, V_DIMMER);
}

void presentation()
{
  // Register the LED Dimmable Light with the gateway
  present(SENSOR_ID, S_DIMMER);

  sendSketchInfo(SN, SV);
}

void loop()
{
  bt1.tick();
}


void receive(const MyMessage &message)
{
  if (message.type == V_LIGHT || message.type == V_DIMMER) {

    //  Retrieve the power or dim level from the incoming request message
    int requestedLevel = atoi(message.data);

    // Adjust incoming level if this is a V_LIGHT variable update [0 == off, 1 == on]
    requestedLevel *= ( message.type == V_LIGHT ? 100 : 1 );

    // Clip incoming level to valid range of 0 to 100
    requestedLevel = requestedLevel > 100 ? 100 : requestedLevel;
    requestedLevel = requestedLevel < 0   ? 0   : requestedLevel;

    setLED(requestedLevel);
  }
}

void setLED(byte level)
{
  //Fade LED to set level
  int delta = (level - ledLevel) < 0 ? -1 : 1;

  while (ledLevel != level)
  {
    ledLevel += delta;
    analogWrite(LED_PIN, (int)(ledLevel / 100. * 255) );
    delay(FADE_DELAY);
  }
  
  MyMessage dimmerMsg(SENSOR_ID, V_DIMMER);
  MyMessage lightMsg(SENSOR_ID, V_LIGHT);
  
  send(lightMsg.set(ledLevel > 0));
  send(dimmerMsg.set(ledLevel));
}

void writeToLED()
{ 
  analogWrite(LED_PIN, map(ledLevel,0,100,0,255));
}

void click1() {setLED(ledLevel>0 ? 0 : 100); }

void pstart1() {ledDimWay = ledLevel<100; }

void press1() {if(ledDimWay && ledLevel<100) ledLevel++; if(!ledDimWay && ledLevel>5) ledLevel--; writeToLED(); delay(25); }

void pstop1() {setLED(ledLevel); }

