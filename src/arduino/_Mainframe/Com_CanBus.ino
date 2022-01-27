/*
  GeoRover CanBUS communication protocols for openCAN communication (Motors)
  https://github.com/autowp/arduino-mcp2515

  Mads Rosenhøj Jepepsen
  Aarhus University
  2021
*/

#include <mcp2515.h>

struct can_frame canMsgLeft;
struct can_frame canMsgRight;
struct can_frame canMsg;
struct can_frame canTestMsg;

MCP2515 mcp2515(PO_SPISS_CANBUS);

bool InitializeCanBus() {
  SPI.begin(); //Begins SPI communication
  // mcp2515.reset();
  mcp2515.setBitrate(CANBBUS_SPEED, MCP_8MHZ);
  mcp2515.setNormalMode();

  canMsgLeft.can_dlc = CANBUS_DATA_LENGTH;
  canMsgLeft.can_id = CANBUS_ID_MOTOR1;

  canMsgRight.can_dlc = CANBUS_DATA_LENGTH;
  canMsgRight.can_id = CANBUS_ID_MOTOR2;

  delay(20);

  return mcp2515.checkReceive();

}

void TerminateCanBus() {
  SPI.end();
}

void CanBusProcess(){
  if (millis() - CanBusTxLast < CANBUS_TX_PERIOD){
    mcp2515.sendMessage(&canMsgLeft);
    mcp2515.sendMessage(&canMsgRight);
    CanBusTxLast = millis();
  }
  
  if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK) {
    status = ParseData();
  }
}

bool CanBusTest(){
  bool status = false;
  DEBUG_PRINTLN("------- CAN Read ----------");
  DEBUG_PRINTLN("ID  DLC   DATA");

  canTestMsg.can_id = 0x12;
  canTestMsg.can_dlc = 3;
  canTestMsg.data[0] = 0;
  canTestMsg.data[1] = 0;
  canTestMsg.data[2] = 0;

  mcp2515.sendMessage(&canTestMsg);

  long CanBusTxLast = millis();
  long CanBusTestStart = millis();
  
  while (millis() - CanBusTestStart < COM_TEST_PERIOD)
  {
    if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK) {
      StreamData();
      status = ParseData();

      Serial.println();      
    }
    else if (millis() - CanBusTxLast < CANBUS_TX_PERIOD)
    {
      mcp2515.sendMessage(&canTestMsg);
      CanBusTxLast = millis();
    }
  }

  return status;
  
}

// Stream all data in raw HEX
void StreamData(){
  Serial.print(canMsg.can_id, HEX); // print ID
  Serial.print(" "); 
  Serial.print(canMsg.can_dlc, HEX); // print DLC
  Serial.print(" ");
  
  for (int i = 0; i<canMsg.can_dlc; i++)  {  // print the data
    Serial.print(canMsg.data[i],HEX);
    Serial.print(" ");
  }
  
  Serial.println();     
}

bool ParseData(){
  char buffer[100];
  bool status = false;

  if(canMsg.can_id == 0x64){
    int control_value = (int)((canMsg.data[1] << 8) | canMsg.data[0]);
    int motor_state = (int)(canMsg.data[3] >> 6); 
    int rpm = (int)((canMsg.data[6] << 8) | canMsg.data[5]) / 10.0f;
    int temperature = (int)canMsg.data[7];
    
    Serial.print("Control value: ");
    Serial.print(control_value);
    Serial.print("\t Motor State: ");
    Serial.print(motor_state);
    Serial.print("\t rpm: ");
    Serial.print(rpm);
    Serial.print("\t Temperature: ");
    Serial.println(temperature);

    status = true;
  }
  
  if(canMsg.can_id == 0x65){
    // inv peak current
    // Motor power
  }
  
  // Warning
  if(canMsg.can_id == 0x66){
    bool warning[64];
    
    for (size_t i = 0; i < 8; i++)
    {
      for (size_t j = 0; j < 8; j++)
      {
        warning[i*8+j] = canMsg.data[i] >> j & 1;
      }
    }
    Serial.print("Warning: ");
    for (size_t i = 0; i < 64; i++)
    {
      if(warning[i] == 1){
        Serial.print(i);
        Serial.print("\t");
      }
    }

    // for (int i = 0; i<canMsg.can_dlc; i++)  {  // print the data
    //   Serial.print(canMsg.data[i],HEX);
    //   Serial.print(" ");
    // }
    Serial.println();
  }
  
  // Error
  if(canMsg.can_id == 0x67){
    bool error[64];
    
    for (size_t i = 0; i < 8; i++)
    {
      for (size_t j = 0; j < 8; j++)
      {
        error[i*8+j] = canMsg.data[i] >> j & 1;
      }
    }
    Serial.print("Error: ");
    for (size_t i = 0; i < 64; i++)
    {
      if(error[i] == 1){
        Serial.print(i);
        Serial.print("\t");
      }
    }
    
    // for (int i = 0; i<canMsg.can_dlc; i++)  {  // print the data
    //   Serial.print(canMsg.data[i],HEX);
    //   Serial.print(" ");
    // }

    Serial.println();
  }

  return status;
}

void CanBusTransmit(struct can_frame canMsg) {
  DEBUG_PRINT("Transmitting CAN Bus message... ");

  mcp2515.sendMessage(&canMsg); //Sends the CAN message

  DEBUG_PRINTLN("CAN Bus message sent.");
}


void PopulateCanMsg(struct can_frame canMsg, unsigned int _data) {
  uint8_t dataArray[CANBUS_DATA_LENGTH];

  for (int i = 0; i < CANBUS_DATA_LENGTH; i++) {
    canMsg.data[i] = (_data >> 8 * i) & 0xFF;
  }
}

bool InitializeMotor(bool motor){
  bool status = true;

  return status;
}