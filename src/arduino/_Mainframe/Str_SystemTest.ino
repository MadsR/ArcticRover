/*  Strategy - System Test mode

    Runs system test...
*/

// Start sequence of strategy
void StartStrategySystemTest() {
  DEBUG_PRINTLINE();
  DEBUG_PRINTLN("Strategy (System Test): Starting");
  StrategyStartLed(MODE_SYSTEMTEST);

  SystemDisable();

  AttachSelectButton();

  DEBUG_PRINTLN("Strategy (System Test): Initialized");
  DEBUG_PRINTLINE();
  LedBlinkDoubleShort(BINARY_CODE_LED_GRN);
}

// Main sequence of strategy
bool runTest = false;
void RunStrategySystemTest() {
  // Blink light
  StrategyRunLed(MODE_SYSTEMTEST);
  
  if(runTest){
    DEBUG_PRINTLN("Running Full System Test");

    DEBUG_PRINTLN("  Enabling all systems");
    DEBUG_PRINTLINE();

    SystemEnable();
    DEBUG_PRINTLINE();
    
    DEBUG_PRINTLN("Testing subsystems");
    DEBUG_PRINTLINE();

    SystemTest(true);

    DEBUG_PRINTLINE();
    DEBUG_PRINTLN("Disabeling all systems");
    DEBUG_PRINTLINE();

    unsigned long testResults = ToLong(SystemStatus);
    
    SystemDisable();
    AttachSelectButton();
    
    DEBUG_PRINTLINE();
    DEBUG_PRINTLN("System Test Complete");
    DEBUG_PRINT("  Results: ");
    DEBUG_PRINTLN(String(testResults));
    DEBUG_PRINTLINE();
    runTest = false;
  }
  
}

// End sequence of strategy
void FinishStrategySystemTest() {
  DEBUG_PRINTLINE();
  DEBUG_PRINTLN("Strategy (System Test): Ending");
  
  DetachSelectButton();
  SystemDisable();

  DEBUG_PRINTLN("Strategy (System Test): Finished");
}

// Selecet button function
void SelectFunctionSystemTest(){
  if (millis() - lastMillisSelect > BTN_DEBOUNCE_TIME)
  {
    lastMillisSelect = millis();
    runTest = true;
    DetachSelectButton();
  }

}

