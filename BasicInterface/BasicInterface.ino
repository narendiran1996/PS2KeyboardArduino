#define dataPin 8
#define clkPin 3

static volatile uint8_t bufferM[48];
static volatile uint8_t bufferIndx = 0;

void process11bits(void)
{
  static uint8_t bitCount = 0;
  
  if(bitCount == 33)
  {
    bitCount = 0;
  }
  else
  {

    if(bufferIndx < 48)
    {
      bufferM[bufferIndx] = digitalRead(dataPin);
      bufferIndx = bufferIndx + 1;
    }
    
    
    bitCount = bitCount + 1;
  }

 
}
void initBuffer()
{
  for(uint8_t i=0;i<48;i++)
  {
    bufferM[i] = 45;
  }
}
void setup() 
{
  Serial.begin(9600);
  initBuffer();
  pinMode(dataPin, INPUT);
  pinMode(clkPin, INPUT);  
  pinMode(13, OUTPUT);

  digitalWrite(13, 0);
  Serial.println("This is naren");
  attachInterrupt(digitalPinToInterrupt(clkPin), process11bits, FALLING);
}

void loop() 
{
  for(uint8_t i = 0; i < 34; i++)
  {
    Serial.print(bufferM[i]);
  }
  Serial.println();
  delay(5000);

}
