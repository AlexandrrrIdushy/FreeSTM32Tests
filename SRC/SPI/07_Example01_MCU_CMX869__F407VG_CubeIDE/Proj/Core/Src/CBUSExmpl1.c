/*
 * CBUSExmpl1.c
 *
 *  Created on: 20 мая 2021 г.
 *      Author: gravitau
 */


//4

//PROJECT: Programmed Tone Tx with CMX868A
//
//PROCESSOR: Atmel AT89C2051
//
// This code programs the CMX868A with four different "tone pair" values.
// This code uses bit-banging over CBUS.
//
// Microcontroller & CMX868A use separate 11.0592MHz xtals
//
// Code occupies 670 bytes of program memory.
#include <REG2051.H>
//CMX868A Register Definitions
#define RESET 0x01
#define GENERALCONTROL 0xE0
#define TXMODE 0xE1
#define RXMODE 0xE2
#define TXDATA1 0xE3
#define TXDATA2 0xE4 //for V.14 implementation
#define RXDATA 0xE5
#define STATUS 0xE6
#define PRGREG 0xE8
#define POWERUP 0x1580 //Pwrup and Reset bits to 1 after power is first
//applied
//Function Declarations
void generalreset(void);
void wr_byte(unsigned char byte);
void wr1(unsigned char address, unsigned char databyte);
void wr2(unsigned char address, unsigned int dataword);
unsigned char rd_byte (void);
unsigned char rd1(unsigned char address);
unsigned int rd2(unsigned char address);
void initialize_uC(void);
void initialize_868(void);
void programtonetx(void);
//C-BUS to Microcontroller Pin Mapping
sbit CSN = P1^7;
sbit CDATA = P1^6;
sbit SCLK = P1^5;
sbit RDATA = P1^4;
sbit IRQ = P3^3; //this uC pin selected because it is an
external IRQ pin
bit pwrupdelay=0, CMX868AIRQ; //flags

//5

//******************************************
// FUNCTION: void initialize_uC()
//
// PURPOSE: Configure the microcontroller.
//
// DESCRIPTION: This function neither takes in nor returns a variable to
// the calling routine.
//
// Timer1 interrupt is enabled so it can be used for the CMX868A powerup
// delay. Timer1 is placed in
// 16-bit timer mode and loaded with 0xB800 to cause a 20ms delay.
//*******************************************
void initialize_uC()
{
IE=0x8C; //interrupts enabled; global IRQ, EX1, T1
IT1=1; //EX1 configured as falling-edge triggered
IRQ
IRQ=1; //write 1 to EX1 to configure as input
TMOD=0x10; //Timer1 - 16bit timer mode
TH1=0xB8; //0xB800=18432counts=20ms delay
TL1=0x00;
}
//******************************************
// FUNCTION: void ex1() interrupt 2
//
// PURPOSE: /INT1 interrupt service routine (ISR).
//
// DESCRIPTION: This function neither takes in nor returns a variable to
// the calling routine.
//
// When a falling-edge signal is sensed on /INT1, this routine sets the
// CMX868AIRQ flag to 1.
// Control then reverts to the previously running function.
//*******************************************
void ex1() interrupt 2
{
CMX868AIRQ=1;
}
//******************************************
// FUNCTION: void timer1() interrupt 3
//
// PURPOSE: Timer1 interrupt service routine (ISR).
//
// DESCRIPTION: This function neither takes in nor returns a variable to
// the calling routine. When Timer1 overflows and interrupts, this routine
// sets the pwrupdelay flag to 1.
// Control then reverts to the previously running function.
//*******************************************
void timer1() interrupt 3
{
pwrupdelay=1;
}

//6

//******************************************
// FUNCTION: void initialize_868()
//
// PURPOSE: Properly powerup the CMX868A.
//
// DESCRIPTION: This function neither takes in nor returns a variable to
// the calling routine.
//
// A General Reset is first written to the CMX868A, after which a 2-byte
// write is performed to the General Control register with Pwrup & Reset
// bits = 1. A 20ms delay is then observed, and control is passed back to
// the calling routine once the delay is complete.
// At this point the CMX868A can be loaded as necessary for operation.
//*******************************************
void initialize_868()
{
generalreset();
wr2(GENERALCONTROL, POWERUP); //write 1s to Pwrup & Reset bits
TR1=1; //turn on uC Timer1 for 20ms delay
while(pwrupdelay!=1); //wait for xtal osc to stabilize
TR1=0; //turn off uC Timer1
//CMX868A is now powered up and ready for configuration
}
//******************************************
// FUNCTION: void wr_byte(unsigned char byte)
//
// PURPOSE: Write a single byte to the CMX868A CDATA pin.
//
// DESCRIPTION: This function takes in a single-byte corresponding to the
// value to be written to the CDATA line.
//
// A 'for' loop is started and the SCLK line is pulled low. The msb of the
// byte is obtained and written to CDATA. The byte is left-shifted one
// place and SCLK is pulled high to latch the bit into the CMX868A. The
// 'for' loop iterates 7 more times to complete the byte write operation.
//
// This function does not return a variable.
//*******************************************
void wr_byte(unsigned char byte)
{
unsigned char i;
for(i=8; i!=0; i--)
{
SCLK=0;
if(byte & 0x80)
CDATA=1;
else
CDATA=0;
byte <<= 1;
SCLK=1;
//fast processors may need a delay here
}
}

//7


//******************************************
// FUNCTION: void wr1(unsigned char address, unsigned char databyte)
//
// PURPOSE: Write one byte to a CMX868A write register.
//
// DESCRIPTION: This function takes in two single-byte variables
// corresponding to the register address and register contents to be
// written to the CDATA line.
//
// CSN is taken low to start the C-BUS transaction. The address byte is
// written to the CDATA line,and then the register contents are written to
// CDATA. CSN is taken high to complete the C-BUS transaction.
//
// This function does not return a variable.
//*******************************************
void wr1(unsigned char address, unsigned char databyte)
{
CSN=0;
//fast processors may need a delay to observe Tcse
wr_byte(address);
//fast processors may need a delay to observe Tnxt
wr_byte(databyte);
//fast processors may need a delay to observe Tcsh
CSN=1;
}
//******************************************
// FUNCTION: void wr2(unsigned char address, unsigned int dataword)
//
// PURPOSE: Write two bytes to a CMX868A write register.
//
// DESCRIPTION: This function takes in three variables; a single-byte
// variable corresponding to the register address, and a two-byte variable
// corresponding to the register contents, all of which will be written to
// the CDATA line.
//
// CSN is taken low to start the C-BUS transaction. The address byte is
// written to the CDATA line. The register contents are split into two
// single-byte variables, both of which are written to CDATA. CSN is then
// taken high to complete the C-BUS transaction.
//
// This function does not return a variable.
//*******************************************
void wr2(unsigned char address, unsigned int dataword)
{
unsigned char temp;
CSN=0;
//fast processors may need a delay to observe Tcse
wr_byte(address);
//fast processors may need a delay to observe Tnxt
temp=dataword; //get LSB
dataword >>= 8; //shift most significant 8 bits down for MSB
wr_byte(dataword); //write MSB
//fast processors may need a delay to observe Tnxt
wr_byte(temp); //write LSB
//fast processors may need a delay to observe Tcsh
CSN=1;
}

//8

//******************************************
// FUNCTION: unsigned char rd_byte(void)
//
// PURPOSE: Read a single byte from the CMX868A RDATA pin.
//
// DESCRIPTION: This function does not take in a variable.
//
// The SCLK line is pulled low and a 'for' loop is started. The SCLK line
// is pulled high to latch out the latest data bit, and this bit is
// appended to any previously received bits. The received bits are left-
// shifted one place and SCLK is pulled low. (The left-shift is at the top
// of the loop due to the decrementing counter.) The 'for' loop iterates 7
// more times to complete the read operation.
//
// This function returns the received byte to the calling routine.
//*******************************************
unsigned char rd_byte(void)
{
unsigned char byte=0x00, i;
for(i=8; i != 0; i--)
{
byte <<= 1;
SCLK=1;
if(RDATA) //append 1 to byte if RDATA=1
byte |= 0x01;
else //else, append 0 to byte
byte &= 0xFE;
SCLK=0;
//fast processors may need a delay here
}
return(byte);
}
//******************************************
// FUNCTION: unsigned char rd1(unsigned char address)
//
// PURPOSE: Read a single byte from a CMX868A read register.
//
// DESCRIPTION: This function receives a variable corresponding to the
// register address to be read.
//
// The CSN line is pulled low to start the C-BUS transaction.
// The read register address is written to the CMX868A, and then the
// register contents are read out. The C-BUS transaction ends when the CSN
// line is pulled high.
//
// This function returns the received byte to the calling routine.
//*******************************************
unsigned char rd1(unsigned char address)
{
unsigned char rbyte;
CSN=0;
//fast processors may need a delay to observe Tcse
wr_byte(address);
//fast processors may need a delay to observe Tnxt
rbyte=rd_byte();
//fast processors may need a delay to observe Tcsh
CSN=1;
return(rbyte);


//9

//******************************************
// FUNCTION: unsigned int rd2(unsigned char address)
//
// PURPOSE: Read two bytes from a CMX868A read register.
//
// DESCRIPTION: This function receives a variable corresponding to the
// register address to be read.
//
// The CSN line is pulled low to start the C-BUS transaction.
// The read register address is written to the CMX868A, and then the
// register contents are read out.
// The register contents are read out 8-bits at a time and are combined to
// form the 16-bit word.
// The C-BUS transaction ends when the CSN line is pulled high.
//
// This function returns the received word (16 bits) to the calling
routine.
//*******************************************
unsigned int rd2(unsigned char address)
{
unsigned int rword=0x0000;
CSN=0;
//fast processors may need a delay to observe Tcse
wr_byte(address);
//fast processors may need a delay to observe Tnxt
//SCLK=1 at end of wr_byte
SCLK=0; //RDATA becomes active here
rword = rd_byte(); //8 bits returned into 16-bit variable (only least
8 significant bits copied)
rword <<= 8; //left-shift bits into most significant position
rword |= rd_byte(); //append next 8 bits onto existing 16-bit
variable
CSN=1;
return(rword);
}
//******************************************
// FUNCTION: void generalreset(void)
//
// PURPOSE: Issue General Reset to CMX868A.
//
// DESCRIPTION: This function neither receives nor takes in a variable.
//
// The CSN line is pulled low to start the C-BUS transaction.
// The General Reset byte (0x01) is written to the CMX868A.
// The C-BUS transaction ends when the CSN line is pulled high.
//*******************************************
void generalreset(void) //issues General Reset to chip
{
CSN=0;
//fast processors may need a delay to observe Tcse
wr_byte(RESET);

//10

CSN=1;
}
//**************************************
// FUNCTION: void programtonetx(void)
//
// PURPOSE: Write to the Programming Register to configure user-defined Tx
tones.
//
// DESCRIPTION: The function does not take in a variable from the calling
// routine. Variables are declared, including an 17-member int array that
// contains the values to be written to the Programming Register.
//
// A loop is entered that first waits until the Programming Flag in the
// Status Register is set. Once this bit is set, a value is written to the
// Programming Register and the loop iterates. This loop runs 17 times to
// allow 17 values to be written to the Programming Register. If not all
// Tone Pairs are required to be programmed, this loop can be shortened as
// needed.
//
// No variable is returned to the calling function.
//***************************************
void programtonetx(void)
{
unsigned char i;
unsigned int pgmtonetxwords[17]={0x8000, //Increment pointer
0x12AC, 0x24A2, 0x0000, 0x0000, //Tone pair A (one tone),
1400Hz @ 0.5Vrms
0x1556, 0x24A2, 0x0000, 0x0000, //Tone pair B (one tone),
1600Hz @ 0.5Vrms
0x1EAC, 0x24A2, 0x0000, 0x0000, //Tone pair C (one tone),
2300Hz @ 0.5Vrms
0x2157, 0x24A2, 0x0000, 0x0000}; //Tone pair D (one tone),
2500Hz @ 0.5Vrms
CMX868AIRQ=0;
rd2(STATUS); //read Status register to clear
any IRQs
for(i=0; i<17; i++) //17 writes required to program
all Tx tones
{
wr2(PRGREG, pgmtonetxwords[i]);
while(CMX868AIRQ!=1); //wait for IRQ, Pgm Flag is only IRQ
that can happen here
CMX868AIRQ=0; //reset IRQ flag bit
rd2(STATUS); //read Status register to enable next
IRQ
}
}
//******************************************
// FUNCTION: void main()
//
// PURPOSE: Main routine of program.
//
// DESCRIPTION: After the variables are declared, the uC and CMX868A are
// initialized.
// The General Control register is initialized, and the Programming
// Register is loaded for programmed tone tx. Next, the Tx Mode Register
// is configured for programmed tone
// Tx, and the program then enters an infinite loop.

//11


//*******************************************
void main()
{
unsigned int status=0;
initialize_uC();
initialize_868();
wr2(GENERALCONTROL, 0x1550); //11.0592MHz xtal, normal pwr, IRQ
enabled, Pgm Flag IRQ enabled
programtonetx();
wr2(TXMODE, 0x1E0F); //Tx tone pair TD, 0dB attenuation.
while(1);
}
