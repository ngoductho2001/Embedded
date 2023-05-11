/*
* File: SPI_Received.ino
* Author: Ngô Đức Thọ
* Date: 11/05/2023
* Description: This is a sample file for receiving data via SPI protocol
*/

#define SCK D1
#define MOSI D2
#define MISO D6
#define SS D4

/*
* Function: SPI_soft_init
* Description: This function has function to initialize Slave Select, Master Out Slave In, Master In Slave Out, Serial Clock 
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
* Function: Spi_soft_receive
* Description: This function has the function receiving data 
*/
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
* Description: This function help loop to receive data from master
*/
void loop() {
  // Receive data
  uint8_t receivedData = Spi_soft_receive();

  Serial.print("Received: ");
  Serial.println(receivedData, HEX);

  delay(1000);
}
