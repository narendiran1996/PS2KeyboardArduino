//#define DEBUG

uint16_t scanCodes_ascii_withShifted[256] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 32352, 0, 0, 0, 0, 0, 0, 20849, 8497, 0, 0, 0, 23162, 21363, 16737, 22391, 16434, 0, 0, 17251, 22648, 17508, 17765, 9268, 9011, 0, 0, 0, 22134, 18022, 21620, 21106, 9525, 0, 0, 20078, 16994, 18536, 18279, 22905, 24118, 0, 0, 0, 19821, 19050, 21877, 9783, 10808, 0, 0, 15404, 19307, 18793, 20335, 10544, 10297, 0, 0, 15918, 16175, 19564, 14907, 20592, 24365, 0, 0, 0, 8743, 0, 31579, 11069, 0, 0, 0, 0, 0, 32093, 0, 31836, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

#define dataPin 8
#define clkPin 3


#ifdef DEBUG
  static volatile uint8_t bufferM[70];
#endif

static volatile uint8_t bitCount = 0;
static volatile uint8_t keyType = 0;
struct myStruct
{
  uint8_t code1;
  uint8_t code2;
  uint8_t code3;
  uint8_t code4;
  uint8_t code5;
  uint8_t code6;
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
        F1.code1 &= ~ (1 << (bitCount-1));
      else
        F1.code1 |= (1 << (bitCount-1));
    }
    else if(bitCount == 9)
    {
      // check it's a shift key
      if(F1.code1 == 0x12 || F1.code1 == 0x59)
      {
        keyType = 1;
      }
      else
      {
        keyType = 0;
      }

    }
    else if(12 <= bitCount && bitCount <= 19)
    {
      if(val == 0)
        F1.code2 &= ~ (1 << (bitCount-12));
      else
        F1.code2 |= (1 << (bitCount-12));
    }
    else if(23 <= bitCount && bitCount <= 30)
    {
      if(val == 0)
        F1.code3 &= ~ (1 << (bitCount-23));
      else
        F1.code3 |= (1 << (bitCount-23));
    }
    else if(bitCount == 33 && keyType == 0)
    {
      bitCount = 0;
      F1.code1 = 0;
      F1.code2 = 0;
      F1.code3 = 0;
      F1.code4 = 0;
      F1.code5 = 0;
      keyType = 0;
      digitalWrite(13, 0);
    }
    else if(34 <= bitCount && bitCount <= 41)
    {
      if(val == 0)
        F1.code4 &= ~ (1 << (bitCount-34));
      else
        F1.code4 |= (1 << (bitCount-34));
    }
    else if(45 <= bitCount && bitCount <= 52)
    {
      if(val == 0)
        F1.code5 &= ~ (1 << (bitCount-45));
      else
        F1.code5 |= (1 << (bitCount-45));
    }
    else if(56 <= bitCount && bitCount <= 63)
    {
      if(val == 0)
        F1.code6 &= ~ (1 << (bitCount-56));
      else
        F1.code6 |= (1 << (bitCount-56));
    }
    else if(bitCount == 66)
    {
      bitCount = 0;
      F1.code1 = 0;
      F1.code2 = 0;
      F1.code3 = 0;
      F1.code4 = 0;
      F1.code5 = 0;
      keyType = 0;
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
  Serial.println("Interface V3");
  attachInterrupt(digitalPinToInterrupt(clkPin), processBits, FALLING);
}
void printIt()
{
  switch(keyType)
  {
    case 0:
    {
      Serial.println((char)(scanCodes_ascii_withShifted[F1.code1] & 0xFF));
      break;
    }
    case 1:
    {
      Serial.println((char)((scanCodes_ascii_withShifted[F1.code2]>>8) & 0xFF));
      break;
    }
    default:
    {
      Serial.println("Something Else");
      break;
    }
  }
    
}
void printCodes()
{
  #ifdef DEBUG
    for(uint8_t i = 0; i < 66; i++)
    {
      Serial.print(bufferM[i]);
    }
    Serial.print(" --- ");
    Serial.print(bitCount);
    Serial.print("  ---  ");
    Serial.print(keyType);
    Serial.print(" --- ");
  #endif
  Serial.print(F1.code1, HEX);
  Serial.print(" --- ");
  Serial.print(F1.code2, HEX);
  Serial.print(" --- ");
  Serial.print(F1.code3, HEX);
  Serial.print(" --- ");
  Serial.print(F1.code4, HEX);
  Serial.print(" --- ");
  Serial.print(F1.code5, HEX);
  Serial.print(" --- ");
  Serial.print(F1.code6, HEX);
  Serial.print("--- ");
  printIt();
  Serial.println();
}

void loop() 
{
  printCodes();
  delay(500);

}
