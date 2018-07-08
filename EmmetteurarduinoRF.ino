#include <VirtualWire.h>
int report[2];
void setup()
{
    Serial.begin(9600);  
    Serial.println("setup");
    vw_setup(2000); 
}
void loop()
{
  float temperature;
  float current;
  temperature=analogRead(A1);                  
  current=analogRead(A2);  
    report[0] = temperature;
   report[1] = current;
    vw_send((uint8_t *)report, sizeof(report));
    vw_wait_tx();
    delay(5000);
}
