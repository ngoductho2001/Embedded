// File: spi_sender.ino

#define SCK D1
#define MOSI D2
#define MISO D6
#define SS D4

void SPI_soft_init() {
  digitalWrite(SCK, LOW);
  digitalWrite(MOSI, LOW);
  digitalWrite(SS, HIGH);
  digitalWrite(MISO, LOW);
}

void Clock() {
  digitalWrite(SCK, HIGH);
  delayMicroseconds(1000);
  digitalWrite(SCK, LOW);
  delayMicroseconds(1000);
}

void Spi_soft_transmit(uint8_t mData) {
  digitalWrite(SS, LOW);
  for (int8_t i = 7; i >= 0; i--) {
    digitalWrite(SCK, LOW);
    if (mData | (1 << i))
      digitalWrite(MOSI, HIGH);
    else
      digitalWrite(MOSI, LOW);
    digitalWrite(SCK, HIGH);
  }
  digitalWrite(SCK, LOW);
  digitalWrite(SS, HIGH);
}

void setup() {
  Serial.begin(9600);
  pinMode(SCK, OUTPUT);
  pinMode(MOSI, OUTPUT);
  pinMode(MISO, INPUT);
  pinMode(SS, OUTPUT);
  SPI_soft_init();
}

void loop() {
  uint8_t sendData = 0xFF;

  // Transmit data
  Serial.print("Sending: ");
  Serial.println(sendData, HEX);
  Spi_soft_transmit(sendData);

  delay(1000);
  
}
