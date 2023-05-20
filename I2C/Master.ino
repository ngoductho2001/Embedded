#define SCL_PIN D1
#define SDA_PIN D2

void setup() {
  pinMode(SDA_PIN, INPUT_PULLUP);
  pinMode(SCL_PIN, INPUT_PULLUP);
  Serial.begin(9600);
  delay(100);
}

void loop() {
  byte receivedData;

  int result = I2C_begin(0x13);
  
  if (result == 0) { // Kiểm tra nếu master gửi dữ liệu đến (Master writes - Slave reads)
    receivedData = I2C_readByte();
    Serial.print("Received data: ");
    Serial.println(receivedData);
  }
  else if (result == 1) { // Kiểm tra nếu master yêu cầu đọc dữ liệu (Master reads - Slave writes)
    char dataToSend[] = "[S]abcd"; // Chuỗi ký tự gửi đi
    int result = I2C_write(dataToSend);
    if (result == 1)
      Serial.println("Write successfully!");
    else if (result == -1)
      Serial.println("Fail to write! (Master not respond)");
    else
      Serial.println("Something went wrong??");
  }
  else if (result == 2)
    Serial.println("Wrong address. (Not my Address)");
  else
    Serial.println("Other uncontrolled case!!!");
}

uint8_t I2C_begin(byte address) {
  uint8_t data = 0, i, rw = 0;
  while (true) {
    pinMode(SDA_PIN, INPUT_PULLUP);
    pinMode(SCL_PIN, INPUT_PULLUP);
    while ((digitalRead(SDA_PIN) != 1) || (digitalRead(SCL_PIN) != 1)); // Đợi điều kiện IDEAL-----------
    while (digitalRead(SDA_PIN) == 1); // Đợi cho đến khi SDA = 0
    delayMicroseconds(25);
    if (digitalRead(SCL_PIN) == 1) { // Đây là START CONDITION
      while (digitalRead(SCL_PIN) == 1); // Đợi cho đến khi SCL = 0

      // Đọc địa chỉ, R/W bit
      for (i = 0; i < 8; i++) {
        while (digitalRead(SCL_PIN) == 0); // Đợi cho đến khi SCL = 1
        data = (data << 1) | digitalRead(SDA_PIN);
        while (digitalRead(SCL_PIN) == 1); // Đợi cho đến khi SCL = 0
      }
      rw = data & 1;
      data = data >> 1;
      if (data == address) {
        I2C_sendACK();
        return rw;
      }
      else {
        I2C_sendNACK();
        return 2;
      }
    }
    else {}
  }
}

byte I2C_readByte() {
  byte data = 0, i;
  pinMode(SDA_PIN, INPUT_PULLUP);
  pinMode(SCL_PIN, INPUT_PULLUP);
  for (i = 0; i < 8; i++) {
    while (digitalRead(SCL_PIN) == 0); // Đợi cho đến khi SCL = 1
    data = (data << 1) | digitalRead(SDA_PIN);
    while (digitalRead(SCL_PIN) == 1); // Đợi cho đến khi SCL = 0
  }
  I2C_sendACK();
  return data;
}

void I2C_sendACK() {
  pinMode(SDA_PIN, OUTPUT);
  digitalWrite(SDA_PIN, LOW);
  while (digitalRead(SCL_PIN) == 0); // Đợi cho đến khi SCL = 1
  while (digitalRead(SCL_PIN) == 1); // Đợi cho đến khi SCL = 0
  pinMode(SDA_PIN, INPUT_PULLUP); // Gọi sau khi gọi "pinMode(SDA_PIN, OUTPUT)"
}

void I2C_sendNACK() {
  pinMode(SDA_PIN, OUTPUT);
  digitalWrite(SDA_PIN, HIGH);
  while (digitalRead(SCL_PIN) == 0); // Đợi cho đến khi SCL = 1
  while (digitalRead(SCL_PIN) == 1); // Đợi cho đến khi SCL = 0
  pinMode(SDA_PIN, INPUT_PULLUP); // Gọi sau khi gọi "pinMode(SDA_PIN, OUTPUT)"
}

uint8_t I2C_write(char* data) {
  uint8_t ack;
  while (*data != '\0') {
    I2C_writeByte(*data);
    ack = I2C_readACK();
    if (ack == 1) {
      pinMode(SDA_PIN, INPUT_PULLUP);
      return -1;
    }
    data++;
  }
  pinMode(SDA_PIN, INPUT_PULLUP);
  return 1;
}

void I2C_writeByte(byte data) {
  int i;
  pinMode(SDA_PIN, OUTPUT);
  for (i = 0; i < 8; i++) {
    if ((data & 0x80) == 0x80) {
      digitalWrite(SDA_PIN, HIGH);
    }
    else {
      digitalWrite(SDA_PIN, LOW);
    }
    data = data << 1;
    while (digitalRead(SCL_PIN) == 0); // Đợi cho đến khi SCL = 1
    while (digitalRead(SCL_PIN) == 1); // Đợi cho đến khi SCL = 0
  }
}

uint8_t I2C_readACK() {
  uint8_t ack;
  pinMode(SDA_PIN, INPUT_PULLUP);
  while (digitalRead(SCL_PIN) == 0); // Đợi cho đến khi SCL = 1
  ack = digitalRead(SDA_PIN);
  while (digitalRead(SCL_PIN) == 1); // Đợi cho đến khi SCL = 0
  return ack;
}

