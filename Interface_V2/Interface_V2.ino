//#define DEBUG

uint8_t scanCodes[256] = { 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 96, 255, 255, 255, 255, 255, 255, 113, 49, 255, 255, 255, 122, 115, 97, 119, 50, 255, 255, 99, 120, 100, 101, 52, 51, 255, 255, 255, 118, 102, 116, 114, 53, 255, 255, 110, 98, 104, 103, 121, 54, 255, 255, 255, 109, 106, 117, 55, 56, 255, 255, 44, 107, 105, 111, 48, 57, 255, 255, 46, 47, 108, 59, 112, 45, 255, 255, 255, 39, 255, 91, 61, 255, 255, 255, 255, 255, 93, 255, 92, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 };

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
    
    bool val = digitalRead(dataPin);
    if(bitCount == 0)
    {
      digitalWrite(13, 1);
    }
    else if(1 <= bitCount && bitCount <= 8)
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
void printCharacter()
{
  Serial.println((char)scanCodes[F1.makeCode]);
}

void loop() 
{
  printCodes();
  printCharacter();
  delay(200);

}
