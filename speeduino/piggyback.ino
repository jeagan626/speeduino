/* Theroy of opperation for piggyback mode: 
Piggyback mode replaces the primary fuel and spark tables,
and uses the signals taken from the factory ecu to determine the fuel and spark values
fuel is simple as the factory pulsewidth sent to injectors is measured then back caclulated to determine a ve value to use 
instead of the ve value stored in the primary table. The AFR can be measured by a lambda sensor,
but the default value should be to avoid incorperate AFR. In theroy this would also let users reverse engineer the factory VE table
The utility of the piggyback mode is the use of the secondary tables and corrections.
This allows the user to modify or completely override the factory ecu behavior under boost pressure ethonol content ect.
*/
#include "globals.h"
#include "speeduino.h"
#include "piggyback.h"

void piggybackInjPulse()
{
  if(READ_FLEX() == true)
  {
    unsigned long tempPW = (micros() - injPulseStartTime); //Calculate the pulse width
    flexPulseWidth = ADC_FILTER(tempPW, configPage4.FILTER_FLEX, flexPulseWidth);
    ++flexCounter;
  }
  else
  {
    flexStartTime = micros(); //Start pulse width measurement.
  }
}
// determine the factory fuel injection pulse width 
//(perhaps it is best to sample the last cylinder in the firing order and base calculations on that?)
uint16_t piggybackPW = 1000; // filler value of 1000us (write function to find this later)

// float calculation to find PW
//PW = (REQ_FUEL * (float)(VE/100.0) * (float)(MAP/100.0) * (float)(TPS/100.0) * (float)(corrections/100.0) + injOpen);
// find the equilivelent VE based on PW
byte piggybackVE = (float)(piggybackPW - inj_opentime_uS) / req_fuel_uS;
// consider adding a reqFuel based on the oe Specs to allow for injector auto scaling






