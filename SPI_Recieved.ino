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

uint8_t Spi_soft_receive() {
  uint8_t receivedData = 0;
  digitalWrite(SS, LOW);  // Enable slave device
  delayMicroseconds(100);
  for (int8_t i = 7; i >= 0; i--) {
    receivedData <<= 1;
    digitalWrite(SCK, HIGH);
    delayMicroseconds(100);
    receivedData |= digitalRead(MISO);
    digitalWrite(SCK, LOW);
    delayMicroseconds(100);
  }
  digitalWrite(SS, HIGH); // Disable slave device
  return receivedData;
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
  // Receive data
  uint8_t receivedData = Spi_soft_receive();

  Serial.print("Received: ");
  Serial.println(receivedData, HEX);

  delay(1000);
}
