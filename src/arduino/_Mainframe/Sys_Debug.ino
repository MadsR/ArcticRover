/*
  Debug functions

  Custom Debug functions to execute by sending one of two commands via Serial
    <ME23> or <MD23>
*/

// Code in this function is executed by sending <ME23>
bool DebugInitialize() {
  digitalWrite(PO_MOTOR_EN_PWR, true);  // TODO: Remove, temporary 12V relay power
  return true;
}

// Code in this function is executed by sending <MD23>
void DebugTerminate() {
  digitalWrite(PO_MOTOR_EN_PWR, false);  // TODO: Remove, temporary 12V relay power
}