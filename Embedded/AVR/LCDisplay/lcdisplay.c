/**
 * A simple example how to drive a Crystalfontz LC display.
 */


#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <string.h>

#include "bitsInlineMacro.h"
#include "TWI_master.h"

#define TRUE          1
#define FALSE         0

volatile static uint8_t irqCounter = 0;
volatile static uint8_t doWork = 0;
volatile static uint8_t toggle = 0;

// ISR is called at 61 Hz
ISR(TIMER0_OVF_vect) {
   irqCounter++;
   if (irqCounter >= 122) {
      irqCounter = 0;
      doWork = 1;
   }
}

static uint8_t TWI_buf[TWI_BUFFER_SIZE];     // Transceiver buffer
static uint8_t TWI_msgSize;                  // Number of bytes to be transmitted.
static uint8_t TWI_state = TWI_NO_STATE;     // State byte. Default set to TWI_NO_STATE.

union TWI_statusReg TWI_statusReg = {0};         // TWI_statusReg is defined in TWI_master.h


uint8_t TWI_Transceiver_Busy(void)
{
   return TWCR & (1<<TWIE);                     // IF TWI Interrupt is enabled then the Transceiver is busy
}

/****************************************************************************
This function is the Interrupt Service Routine (ISR), and called when the TWI interrupt is triggered;
that is whenever a TWI event has occurred. This function should not be called directly from the main
application.
****************************************************************************/
ISR(TWI_vect) {
   static uint8_t TWI_bufPtr;

   switch(TWSR) {
      case TWI_START:             // START has been transmitted
      case TWI_REP_START:         // Repeated START has been transmitted
         TWI_bufPtr = 0;          // Set buffer pointer to the TWI Address location
      case TWI_MTX_ADR_ACK:       // SLA+W has been transmitted and ACK received
      case TWI_MTX_DATA_ACK:      // Data byte has been transmitted and ACK received
         if(TWI_bufPtr < TWI_msgSize)
         {
            TWDR = TWI_buf[TWI_bufPtr++];
            TWCR = (1<<TWEN)|                          // TWI Interface enabled
                   (1<<TWIE)|(1<<TWINT)|               // Enable TWI Interrupt and clear the flag to send byte
                   (0<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|
                   (0<<TWWC);
         }
         else                     // Send STOP after last byte
         {
            TWI_statusReg.lastTransOK = TRUE;          // Set status bits to completed successfully.
            TWCR = (1<<TWEN)|                          // TWI Interface enabled
                   (0<<TWIE)|(1<<TWINT)|               // Disable TWI Interrupt and clear the flag
                   (0<<TWEA)|(0<<TWSTA)|(1<<TWSTO)|    // Initiate a STOP condition.
                   (0<<TWWC);
         }
         break;

      case TWI_MRX_DATA_ACK:      // Data byte has been received and ACK transmitted
         TWI_buf[TWI_bufPtr++] = TWDR;
      case TWI_MRX_ADR_ACK:       // SLA+R has been transmitted and ACK received
         if(TWI_bufPtr < (TWI_msgSize-1))              // Detect the last byte to NACK it.
         {
            TWCR = (1<<TWEN)|                          // TWI Interface enabled
                   (1<<TWIE)|(1<<TWINT)|               // Enable TWI Interrupt and clear the flag to read next byte
                   (1<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|    // Send ACK after reception
                   (0<<TWWC);
         }
         else                     // Send NACK after next reception
         {
            TWCR = (1<<TWEN)|                          // TWI Interface enabled
               (1<<TWIE)|(1<<TWINT)|                   // Enable TWI Interrupt and clear the flag to read next byte
               (0<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|        // Send NACK after reception
               (0<<TWWC);
         }
         break;

      case TWI_MRX_DATA_NACK:     // Data byte has been received and NACK transmitted
         TWI_buf[TWI_bufPtr] = TWDR;
         TWI_statusReg.lastTransOK = TRUE;             // Set status bits to completed successfully.
         TWCR = (1<<TWEN)|                             // TWI Interface enabled
                (0<<TWIE)|(1<<TWINT)|                  // Disable TWI Interrupt and clear the flag
                (0<<TWEA)|(0<<TWSTA)|(1<<TWSTO)|       // Initiate a STOP condition.
                (0<<TWWC);
         break;

      case TWI_ARB_LOST:          // Arbitration lost
         TWCR = (1<<TWEN)|                             // TWI Interface enabled
                (1<<TWIE)|(1<<TWINT)|                  // Enable TWI Interrupt and clear the flag
                (0<<TWEA)|(1<<TWSTA)|(0<<TWSTO)|       // Initiate a (RE)START condition.
                (0<<TWWC);
         break;

      case TWI_BUS_ERROR:         // Bus error due to an illegal START or STOP condition
      case TWI_MTX_ADR_NACK:      // SLA+W has been transmitted and NACK received
      case TWI_MRX_ADR_NACK:      // SLA+R has been transmitted and NACK received
      case TWI_MTX_DATA_NACK:     // Data byte has been transmitted and NACK received
//      case TWI_NO_STATE           // No relevant state information available; TWINT = '0'
      default:
         TWI_state = TWSR;                              // Store TWSR and automatically clears no Errors bit.
                                                        // Reset TWI Interface
         TWCR = (1<<TWEN)|                              // Enable TWI-interface and release TWI pins
                (0<<TWIE)|(1<<TWINT)|                   // Disable Interrupt
                (0<<TWEA)|(0<<TWSTA)|(1<<TWSTO)|        // Initiate a STOP condition.
                (0<<TWWC);
   }
}

static void lcdInit() {
   TWBR = TWI_TWBR;                                // Set bit rate register (Baudrate). Defined in header file.
//   TWSR = TWI_TWPS;                                // Set prescaler
   TWDR = 0xFF;                                    // Default content = SDA released.
   TWCR = (1<<TWEN)|                               // Enable TWI-interface and release TWI pins.
          (0<<TWIE)|(0<<TWINT)|                    // Disable Interrupt.
          (0<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|         // No Signal requests.
          (0<<TWWC);
}


static void lcdPrint(const char* text) {
   uint8_t temp;
   while(TWI_Transceiver_Busy());                // Wait until TWI is ready for next transmission.

   TWI_msgSize = strlen(text) + 1;                        // Number of data to transmit.
   TWI_buf[0]  = 0x00; // TODO!!!! msg[0];                         // Store slave address with R/W setting.
 

  // TODO: Need to add the crystalfonts command byte!

   //if(!(msg[0] & (TRUE<<TWI_READ_BIT)))          // If it is a write operation, then also copy data.
  // {
      for(temp = 1; temp < TWI_msgSize; temp += 1)
         TWI_buf[temp] = text[temp-1];
   //}

   TWI_statusReg.all = 0;
   TWI_state = TWI_NO_STATE;
   TWCR = (1<<TWEN)|                             // TWI Interface enabled.
          (1<<TWIE)|(1<<TWINT)|                  // Enable TWI Interrupt and clear the flag.
          (0<<TWEA)|(1<<TWSTA)|(0<<TWSTO)|       // Initiate a START condition.
          (0<<TWWC);
}


int main() {
   lcdInit();
   lcdPrint("Hello World");

   bitSet(DDRL, DDL1);  // output pin
   bitSet(PORTL, PL1);  // green LED

   TCCR0A = 0;
   TCCR0B = _BV(CS02) | _BV(CS00);      // start timer with clk/1024 (15,625 kHz)
   TIMSK0 = _BV(TOIE0);                 // enable timer0 overflow interrupt

   while(1) {
      set_sleep_mode(0); // IDLE mode
      sleep_mode();

      if (doWork) {
         doWork = 0;

         // called every two seconds
         bitToggle(PORTL, PL1);		// green LED blink

         if (toggle) {
            lcdPrint("Hello"); 
         } else {
            lcdPrint("World"); 
         }
         toggle = !toggle;
      }
   }
}
