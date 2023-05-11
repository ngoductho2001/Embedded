
/*
* File: SPI_Received.ino
* Author: Ngô Đức Thọ
* Date: 11/05/2023
* Description: This is a sample file for sending data via SPI protocol
*/
#define SCK D1
#define MOSI D2
#define MISO D6
#define SS D4
/*
* Function: SPI_soft_init
* Description: This function has the function of generating a high clock so that 1 second will return to a low level
*/
void SPI_soft_init() {
  digitalWrite(SCK, LOW);
  digitalWrite(MOSI, LOW);
  digitalWrite(SS, HIGH);
  digitalWrite(MISO, LOW);
}

/*
* Function: SPI_soft_init
* Description: This function has the function of generating a high clock so that 1 second will return to a low level
*/
void Clock() {
  digitalWrite(SCK, HIGH);
  delayMicroseconds(1000);
  digitalWrite(SCK, LOW);
  delayMicroseconds(1000);
}

/*
* Function: Spi_soft_transmit
* Description: This function has the function sending data  
*/
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

/*
* Function: setup
* Description: This function has the function of setting the GPIO pins and initializing the SPI interface for communication
*/
void setup() {
  Serial.begin(9600);
  pinMode(SCK, OUTPUT);
  pinMode(MOSI, OUTPUT);
  pinMode(MISO, INPUT);
  pinMode(SS, OUTPUT);
  SPI_soft_init();
}

/*
* Function: loop
* Description: This function help loop to send data to slave
*/
void loop() {
  uint8_t sendData = 0xFF;

  // Transmit data
  Serial.print("Sending: ");
  Serial.println(sendData, HEX);
  Spi_soft_transmit(sendData);

  delay(1000);
  
}
