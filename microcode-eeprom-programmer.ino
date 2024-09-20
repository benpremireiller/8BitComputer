/**
 * This sketch programs the microcode EEPROMs for the 8-bit breadboard computer
 * Most of this code was adapted from code written by Ben Eater licensed under the MIT License.
 */
 
 // Define pins on the arduino
#define SHIFT_DATA 2
#define SHIFT_CLK 3
#define SHIFT_LATCH 4
#define EEPROM_D0 5
#define EEPROM_D7 12
#define WRITE_EN 13

// These values represent the control word on the 8-bit computer
#define HLT 0b1000000000000000  // Halt clock
#define MI  0b0100000000000000  // Memory address register in
#define RI  0b0010000000000000  // RAM data in
#define RO  0b0001000000000000  // RAM data out
#define IO  0b0000100000000000  // Instruction register data out
#define II  0b0000010000000000  // Instruction register data in
#define AI  0b0000001000000000  // A register in
#define AO  0b0000000100000000  // A register out
#define EO  0b0000000010000000  // ALU out
#define SU  0b0000000001000000  // ALU subtract
#define BI  0b0000000000100000  // B register in
#define OI  0b0000000000010000  // Output register in
#define CE  0b0000000000001000  // Program counter enable
#define CO  0b0000000000000100  // Program counter out
#define J   0b0000000000000010  // Jump (program counter in)

// Define the fetch controls which will be used for each instruction
uint16_t fetch_1 = CO|MI;
uint16_t fetch_2 = RO|II|CE;

// Define the microcode for the OP codes. OP codes values will be stored at address values equal to eachother.
uint16_t data[] = {
fetch_1, fetch_2, 0, 0, 0, 0, 0, 0                  //0000 NOP (No operation)
fetch_1, fetch_2, IO|MI, RO|AI, 0, 0, 0, 0          //0001 MOVA (Move memory to accumulator)
fetch_1, fetch_2, IO|AI, 0, 0, 0, 0, 0,             //0010 MOVI (Move immediate to accumulator)
fetch_1, fetch_2, IO|MI, RO|BI, EO|AI, 0, 0, 0      //0011 ADD (To accumulator)
fetch_1, fetch_2, IO|MI, RO|BI, SU|EO|AI, 0, 0, 0,  //0100 SUB (From accumulator)
fetch_1, fetch_2, IO|MI, AO|RI, 0, 0, 0, 0          //0101 LDA (Load accumulator to memory) 
fetch_1, fetch_2, IO|J, 0, 0, 0, 0, 0,              //0110 JMP (Jump to instruction)
fetch_1, fetch_2, AO|OI, 0, 0, 0, 0, 0,             //0111 OUT (Display ALU to output display)
fetch_1, fetch_2, HLT, 0, 0, 0, 0, 0,               //1000 HLT (Halt the clock)
fetch_1, fetch_2, 0, 0, 0, 0, 0, 0,                 //1001 Unassigned
fetch_1, fetch_2, 0, 0, 0, 0, 0, 0,                 //1010 Unassigned
fetch_1, fetch_2, 0, 0, 0, 0, 0, 0,                 //1011 Unassigned
fetch_1, fetch_2, 0, 0, 0, 0, 0, 0,                 //1100 Unassigned
fetch_1, fetch_2, 0, 0, 0, 0, 0, 0,                 //1101 Unassigned
fetch_1, fetch_2, 0, 0, 0, 0, 0, 0,                 //1110 Unassigned
fetch_1, fetch_2, 0, 0, 0, 0, 0, 0,                 //1111 Unassigned
};


/*
 * Because an arduino nano is used, a shift register is required to output all 11 bits of the address pins
 * Output the address bits and outputEnable signal using shift registers.
 */
void setAddress(int address, bool outputEnable) {
  shiftOut(SHIFT_DATA, SHIFT_CLK, MSBFIRST, (address >> 8) | (outputEnable ? 0x00 : 0x80));
  shiftOut(SHIFT_DATA, SHIFT_CLK, MSBFIRST, address);

  digitalWrite(SHIFT_LATCH, LOW);
  digitalWrite(SHIFT_LATCH, HIGH);
  digitalWrite(SHIFT_LATCH, LOW);
}


/*
 * Read a byte from the EEPROM at the specified address.
 */
byte readEEPROM(int address) {
  for (int pin = EEPROM_D0; pin <= EEPROM_D7; pin += 1) {
    pinMode(pin, INPUT);
  }
  setAddress(address, /*outputEnable*/ true);

  byte data = 0;
  for (int pin = EEPROM_D7; pin >= EEPROM_D0; pin -= 1) {
    data = (data << 1) + digitalRead(pin);
  }
  return data;
}


/*
 * Write a byte to the EEPROM at the specified address.
 */
void writeEEPROM(int address, byte data) {
  setAddress(address, /*outputEnable*/ false);
  for (int pin = EEPROM_D0; pin <= EEPROM_D7; pin += 1) {
    pinMode(pin, OUTPUT);
  }

  for (int pin = EEPROM_D0; pin <= EEPROM_D7; pin += 1) {
    digitalWrite(pin, data & 1);
    data = data >> 1;
  }
  digitalWrite(WRITE_EN, LOW);
  delayMicroseconds(1);
  digitalWrite(WRITE_EN, HIGH);
  delay(10);
}


/*
 * Read the contents of the EEPROM and print them to the serial monitor.
 */
void printContents() {
  for (int base = 0; base <= 255; base += 16) {
    byte data[16];
    for (int offset = 0; offset <= 15; offset += 1) {
      data[offset] = readEEPROM(base + offset);
    }

    char buf[80];
    sprintf(buf, "%03x:  %02x %02x %02x %02x %02x %02x %02x %02x   %02x %02x %02x %02x %02x %02x %02x %02x",
            base, data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7],
            data[8], data[9], data[10], data[11], data[12], data[13], data[14], data[15]);

    Serial.println(buf);
  }
}


void setup() {
  // put your setup code here, to run once:
  pinMode(SHIFT_DATA, OUTPUT);
  pinMode(SHIFT_CLK, OUTPUT);
  pinMode(SHIFT_LATCH, OUTPUT);
  digitalWrite(WRITE_EN, HIGH);
  pinMode(WRITE_EN, OUTPUT);
  Serial.begin(57600);

  // Program data bytes
  Serial.print("Programming EEPROM");

  // Program the 8 high-order bits of microcode into the first 128 bytes of EEPROM
  for (int address = 0; address < sizeof(data)/sizeof(data[0]); address += 1) {
    writeEEPROM(address, data[address] >> 8);

    if (address % 64 == 0) {
      Serial.print(".");
    }
  }

  // Program the 8 low-order bits of microcode into the second 128 bytes of EEPROM
  for (int address = 0; address < sizeof(data)/sizeof(data[0]); address += 1) {
    writeEEPROM(address + 128, data[address]);

    if (address % 64 == 0) {
      Serial.print(".");
    }
  }

  Serial.println(" done");


  // Read and print out the contents of the EERPROM
  Serial.println("Reading EEPROM");
  printContents();
}


void loop() {
  // put your main code here, to run repeatedly:

}

