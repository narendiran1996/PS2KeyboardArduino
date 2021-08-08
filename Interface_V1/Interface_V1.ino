//#define DEBUG

#define dataPin 8
#define clkPin 3


#ifdef DEBUG
  static volatile uint8_t bufferM[48];
#endif

static volatile uint8_t bitCount = 0;
struct myStruct
{
  uint8_t makeCode;
  uint8_t breakCode1;
  uint8_t breakCode2;
  
}F1;

void processBits(void)
{
    digitalWrite(13, 1);
    bool val = digitalRead(dataPin);

    if(1 <= bitCount && bitCount <= 8)
    {
      if(val == 0)
        F1.makeCode &= ~ (1 << (bitCount-1));
      else
        F1.makeCode |= (1 << (bitCount-1));
    }
    else if(12 <= bitCount && bitCount <= 19)
    {
      if(val == 0)
        F1.breakCode1 &= ~ (1 << (bitCount-12));
      else
        F1.breakCode1 |= (1 << (bitCount-12));
    }
    else if(23 <= bitCount && bitCount <= 30)
    {
      if(val == 0)
        F1.breakCode2 &= ~ (1 << (bitCount-23));
      else
        F1.breakCode2 |= (1 << (bitCount-23));
    }
    else if(bitCount == 33)
    {
      bitCount = 0;
      digitalWrite(13, 0);
    }
    #ifdef DEBUG
      bufferM[bitCount] = val;
    #endif
    bitCount = bitCount + 1;
}

void setup() 
{
  Serial.begin(9600);
  pinMode(dataPin, INPUT);
  pinMode(clkPin, INPUT);  
  pinMode(13, OUTPUT);

  digitalWrite(13, 0);
  Serial.println("Interface V1");
  attachInterrupt(digitalPinToInterrupt(clkPin), processBits, FALLING);
}
void printCodes()
{
  #ifdef DEBUG
    for(uint8_t i = 0; i < 33; i++)
    {
      Serial.print(bufferM[i]);
    }
    Serial.print("   ---   ");
  #endif
  Serial.print(F1.makeCode, HEX);
  Serial.print("   ---   ");
  Serial.print(F1.breakCode1, HEX);
  Serial.print("   ---   ");
  Serial.print(F1.breakCode2, HEX);
  Serial.println();
}
void loop() 
{
  printCodes();
  delay(5000);

}
