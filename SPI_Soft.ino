#define SCK D1
#define MOSI D2
#define MISO D5
#define SS D6

void SPI_soft_init() {
  digitalWrite(SCK, LOW);
  digitalWrite(MOSI, LOW);
  digitalWrite(SS, HIGH);
}

void Clock() {
  digitalWrite(SCK, HIGH);
  delayMicroseconds(2);
  digitalWrite(SCK, LOW);
  delayMicroseconds(2);
}

void Spi_soft_transmit(uint8_t mData) {
  digitalWrite(SS, LOW);
  for (int8_t i = 7; i >= 0; i--) {
    digitalWrite(SCK, LOW);
    if (mData & (1 << i))
      digitalWrite(MOSI, HIGH);
    else
      digitalWrite(MOSI, LOW);
    digitalWrite(SCK, HIGH);
  }
  digitalWrite(SCK, LOW);
  digitalWrite(SS, HIGH);
}

uint8_t Spi_soft_receive() {
  uint8_t receivedData = 0;
  digitalWrite(SS, LOW);  // Enable slave device
  delayMicroseconds(5);
  for (int8_t i = 7; i >= 0; i--) {
    receivedData <<= 1;
    digitalWrite(SCK, LOW);
    delayMicroseconds(2);
    receivedData |= digitalRead(MISO);
    digitalWrite(SCK, HIGH);
    delayMicroseconds(2);
  }
  digitalWrite(SS, HIGH); // Disable slave device
  return receivedData;
}

void setup() {
  Serial.begin(19200);
  pinMode(SCK, OUTPUT);
  pinMode(MOSI, OUTPUT);
  pinMode(MISO, INPUT);
  pinMode(SS, OUTPUT);
  SPI_soft_init();
}

void loop() {
  uint8_t sendData = 0x30;

  // Transmit data
  Serial.print("Sending: ");
  Serial.println(sendData, HEX);
  Spi_soft_transmit(sendData);

  delay(1000);

  // Receive data
  uint8_t receivedData = Spi_soft_receive();

  Serial.print("Received: ");
  Serial.println(receivedData, HEX);

  delay(1000);
}
