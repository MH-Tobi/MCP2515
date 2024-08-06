
#ifndef MCP2515REG_H
#define MCP2515REG_H


/**
 * @brief FILTER n STANDARD IDENTIFIER REGISTER HIGH (ADDRESS= 00h, 04h, 08h, 10h, 14h, 18h)
 * @note The Mask and Filter registers read all ‘0’s when in any mode except Configuration mode.
 *
 * @param n Filternumber 0 - 5
 */
#define REG_RXFnSIDH(n)		(0x00 + (n * 0x04))

/**
 * @brief Standard Identifier Filter bits (R/W-x)
 *
 * These bits hold the filter bits to be applied to bits[10:3] of the Standard Identifier portion of a received message.
 */
#define RXFnSIDH_BIT_SID            0xFF



/**
 * @brief FILTER n STANDARD IDENTIFIER REGISTER LOW (ADDRESS= 01h, 05h, 09h, 11h, 15h, 19h)
 * @note The Mask and Filter registers read all ‘0’s when in any mode except Configuration mode.
 *
 * @param n Filternumber 0 - 5
 */
#define REG_RXFnSIDL(n)		(0x01 + (n * 0x04))

/**
 * @brief Extended Identifier Filter bits (R/W-x)
 *
 * These bits hold the filter bits to be applied to bits[17:16] of the Extended Identifier portion of a received message.
 */
#define RXFnSIDL_BIT_EID            0x03

/**
 * @brief Extended Identifier Enable bit (R/W-x)
 *
 * 1 = Filter is applied only to extended frames
 *
 * 0 = Filter is applied only to standard frames
 */
#define RXFnSIDL_BIT_EXIDE          0x08

/**
 * @brief Standard Identifier Filter bits (R/W-x)
 *
 * These bits hold the filter bits to be applied to bits[2:0] of the Standard Identifier portion of a received message.
 */
#define RXFnSIDL_BIT_SID            0xE0



/**
 * @brief FILTER n EXTENDED IDENTIFIER REGISTER HIGH (ADDRESS= 02h, 06h, 0Ah, 12h, 16h, 1Ah)
 * @note The Mask and Filter registers read all ‘0’s when in any mode except Configuration mode.
 *
 * @param n Filternumber 0 - 5
 */
#define REG_RXFnEID8(n)		(0x02 + (n * 0x04))

/**
 * @brief Extended Identifier bits (R/W-x)
 *
 * These bits hold the filter bits to be applied to bits[15:8] of the Extended Identifier portion of a received
 * message or to Byte 0 in received data if the corresponding RXM[1:0] bits = 00 and EXIDE = 0.
 */
#define RXFnEID8_BIT_EID            0xFF



/**
 * @brief  FILTER n EXTENDED 1 REGISTER LOW (ADDRESS= 03h, 07h, 0Bh, 13h, 17h, 1Bh)
 * @note The Mask and Filter registers read all ‘0’s when in any mode except Configuration mode.
 *
 * @param n Filternumber 0 - 5
 */
#define REG_RXFnEID0(n)		(0x03 + (n * 0x04))		// and Register 0x07, 0x0B, 0x13, 0x17, 0x1B => n = 0..5

/**
 * @brief Extended Identifier bits (R/W-x)
 *
 * These bits hold the filter bits to be applied to bits[7:0] of the Extended Identifier portion of a received
 * message or to Byte 1 in received data if the corresponding RXM[1:0] bits = 00 and EXIDE = 0.
 */
#define RXFnEID0_BIT_EID            0xFF



/**
 * @brief RXnBF PIN CONTROL AND STATUS REGISTER (ADDRESS= 0Ch)
 */
#define REG_BFPCTRL 		0x0C

/**
 * @brief RXnBF Pin Operation mode bit (R/W-0)
 *
 * 1 = Pin is used as an interrupt when a valid message is loaded into RXBn
 *
 * 0 = Digital Output mode
 *
 * @param n Buffernumber 0 - 1
 */
#define BFPCTRL_BIT_BnBFM(n)		(0x01 << n)

/**
 * @brief RXnBF Pin Function Enable bit (R/W-0)
 *
 * 1 = Pin function is enabled, operation mode is determined by the BnBFM bit
 *
 * 0 = Pin function is disabled, pin goes to a high-impedance state
 *
 * @param n Buffernumber 0 - 1
 */
#define BFPCTRL_BIT_BnBFE(n)		(0x04 << n)

/**
 * @brief RXnBF Pin State bit (Digital Output mode only) (R/W-0)
 *
 * Reads as ‘0’ when RXnBF is configured as an interrupt pin
 *
 * @param n Buffernumber 0 - 1
 */
#define BFPCTRL_BIT_BnBFS(n)		(0x10 << n)



/**
 * @brief TXnRTS PIN CONTROL AND STATUS REGISTER (ADDRESS= 0Dh)
 */
#define REG_TXRTSCTRL 		0x0D

/**
 * @brief TXnRTS Pin mode bit (R/W-0)
 *
 * 1 = Pin is used to request message transmission of TXBn buffer (on falling edge)
 *
 * 0 = Digital input
 *
 * @param n Buffernumber 0 - 2
 */
#define TXRTSCTRL_BIT_BnRTSM(n)		(0x01 << n)

/**
 * @brief TXnRTS Pin State bit (R-x)
 *
 * Reads state of TXnRTS pin when in Digital Input mode
 *
 * Reads as ‘0’ when pin is in Request-to-Send mode
 *
 * @param n Buffernumber 0 - 2
 */
#define TXRTSCTRL_BIT_BnRTS(n)		(0x08 << n)



/**
 * @brief CAN STATUS REGISTER (ADDRESS= XEh)
 */
#define REG_CANSTAT 		0x0E

/**
 * @brief Interrupt Flag Code bits (R-0)
 *
 * 000 = No interrupt
 *
 * 001 = Error interrupt
 *
 * 010 = Wake-up interrupt
 *
 * 011 = TXB0 interrupt
 *
 * 100 = TXB1 interrupt
 *
 * 101 = TXB2 interrupt
 *
 * 110 = RXB0 interrupt
 *
 * 111 = RXB1 interrupt
 */
#define CANSTAT_BIT_ICOD			0x0E

/**
 * @brief Operation Mode bits (R-1, R-0, R-0) [at Startup/Reset in Configuration-Mode]
 *
 * 000 = Device is in Normal Operation mode
 *
 * 001 = Device is in Sleep mode
 *
 * 010 = Device is in Loopback mode
 *
 * 011 = Device is in Listen-Only mode
 *
 * 100 = Device is in Configuration mode
 */
#define CANSTAT_BIT_OPMOD			0xE0



/**
 * @brief CAN CONTROL REGISTER (ADDRESS = XFh)
 */
#define REG_CANCTRL 		0x0F

/**
 * @brief CLKOUT Pin Prescaler bits (R/W-1)
 *
 * 00 = FCLKOUT = System Clock/1
 *
 * 01 = FCLKOUT = System Clock/2
 *
 * 10 = FCLKOUT = System Clock/4
 *
 * 11 = FCLKOUT = System Clock/8
 */
#define CANCTRL_BIT_CLKPRE			0x03

/**
 * @brief CLKOUT Pin Enable bit (R/W-1)
 *
 * 1 = CLKOUT pin is enabled
 *
 * 0 = CLKOUT pin is disabled (pin is in high-impedance state)
 */
#define CANCTRL_BIT_CLKEN			0x04

/**
 * @brief One-Shot Mode bit (R/W-0)
 *
 * 1 = Enabled; messages will only attempt to transmit one time
 *
 * 0 = Disabled; messages will reattempt transmission if required
 */
#define CANCTRL_BIT_OSM				0x08

/**
 * @brief Abort All Pending Transmissions bit (R/W-0)
 *
 * 1 = Requests abort of all pending transmit buffers
 *
 * 0 = Terminates request to abort all transmissions
 */
#define CANCTRL_BIT_ABAT            0x10

/**
 * @brief Request Operation Mode bits (R/W-1, R/W-0, R/W-0)
 *
 * 000 = Sets Normal Operation mode
 *
 * 001 = Sets Sleep mode
 *
 * 010 = Sets Loopback mode
 *
 * 011 = Sets Listen-Only mode
 *
 * 100 = Sets Configuration mode
 */
#define CANCTRL_BIT_REQOP			0xE0



/**
 * @brief TRANSMIT ERROR COUNTER REGISTER (ADDRESS = 1Ch)
 */
#define REG_TEC 			0x1C

/**
 * @brief Transmit Error Count bits (R-0)
 */
#define TEC_BIT_TEC			0xFF



/**
 * @brief RECEIVE ERROR COUNTER REGISTER (ADDRESS = 1Dh)
 */
#define REG_REC 			0x1D

/**
 * @brief Receive Error Count bits (R-0)
 */
#define REC_BIT_REC			0xFF



/**
 * @brief MASK n STANDARD IDENTIFIER REGISTER HIGH (ADDRESS= 20h, 24h)
 * @note The Mask and Filter registers read all ‘0’s when in any mode except Configuration mode.
 *
 * @param n Masknumber 0 - 1
 */
#define REG_RXMnSIDH(n) 	(0x20 + (n * 0x04))

/**
 * @brief Standard Identifier Filter bits (R/W-0)
 *
 * These bits hold the mask bits to be applied to bits[10:3] of the Standard Identifier portion of a received message.
 */
#define RXMnSIDH_BIT_SID            0xFF



/**
 * @brief MASK n STANDARD IDENTIFIER REGISTER LOW (ADDRESS= 21h, 25h)
 * @note The Mask and Filter registers read all ‘0’s when in any mode except Configuration mode.
 *
 * @param n Masknumber 0 - 1
 */
#define REG_RXMnSIDL(n) 	(0x21 + (n * 0x04))

/**
 * @brief  Extended Identifier Mask bits (R/W-0)
 *
 * These bits hold the mask bits to be applied to bits[17:16] of the Extended Identifier portion of a received message.
 */
#define RXMnSIDL_BIT_EID            0x03

/**
 * @brief  Standard Identifier Mask bits (R/W-0)
 *
 * These bits hold the mask bits to be applied to bits[2:0] of the Standard Identifier portion of a received message.
 */
#define RXMnSIDL_BIT_SID            0xE0



/**
 * @brief MASK n EXTENDED IDENTIFIER REGISTER HIGH (ADDRESS= 22h, 26h)
 * @note The Mask and Filter registers read all ‘0’s when in any mode except Configuration mode.
 *
 * @param n Masknumber 0 - 1
 */
#define REG_RXMnEID8(n) 	(0x22 + (n * 0x04))

/**
 * @brief Extended Identifier bits (R/W-0)
 *
 * These bits hold the filter bits to be applied to bits[15:8] of the Extended Identifier portion of a received message.
 *
 * If the corresponding RXM[1:0] bits = 00 and EXIDE = 0, these bits are applied to Byte 0 in received data.
 */
#define RXMnEID8_BIT_EID            0xFF



/**
 * @brief MASK n EXTENDED IDENTIFIER REGISTER LOW (ADDRESS= 23h, 27h)
 * @note The Mask and Filter registers read all ‘0’s when in any mode except Configuration mode
 *
 * @param n Masknumber 0 - 1
 */
#define REG_RXMnEID0(n) 	(0x23 + (n * 0x04))		// and Register 0x27 => n = 0..1

/**
 * @brief Extended Identifier bits (R/W-0)
 *
 * These bits hold the filter bits to be applied to bits[7:0] of the Extended Identifier portion of a received message.
 *
 * If the corresponding RXM[1:0] bits = 00 and EXIDE = 0, these bits are applied to Byte 1 in received data.
 */
#define RXMnEID0_BIT_EID            0xFF



/**
 * @brief CONFIGURATION REGISTER 3 (ADDRESS= 28h)
 */
#define REG_CNF3 			0x28

/**
 * @brief PS2 Length bits (R/W-0)
 *
 * (PHSEG2[2:0] + 1) x TQ. Minimum valid setting for PS2 is 2 TQs.
 */
#define CNF3_BIT_PHSEG2				0x07

/**
 * @brief Wake-up Filter bit (R/W-0)
 *
 * 1 = Wake-up filter is enabled
 *
 * 0 = Wake-up filter is disabled
 */
#define CNF3_BIT_WAKFIL				0x40

/**
 * @brief Start-of-Frame signal bit (R/W-0)
 *
 * If CLKEN (CANCTRL[2]) = 1:
 *
 * 1 = CLKOUT pin is enabled for SOF signal
 *
 * 0 = CLKOUT pin is enabled for clock out function
 *
 * If CLKEN (CANCTRL[2]) = 0:
 *
 * Bit is don’t care.
 */
#define CNF3_BIT_SOF				0x40



/**
 * @brief CONFIGURATION REGISTER 2 (ADDRESS= 29h)
 */
#define REG_CNF2 			0x29

/**
 * @brief Propagation Segment Length bits (R/W-0)
 *
 * (PRSEG[2:0] + 1) x TQ.
 */
#define CNF2_BIT_PRSEG				0x07

/**
 * @brief PS1 Length bits (R/W-0)
 *
 * (PHSEG1[2:0] + 1) x TQ.
 */
#define CNF2_BIT_PHSEG1				0x38

/**
 * @brief Sample Point Configuration bit (R/W-0)
 *
 * 1 = Bus line is sampled three times at the sample point
 *
 * 0 = Bus line is sampled once at the sample point
 */
#define CNF2_BIT_SAM				0x40

/**
 * @brief PS2 Bit Time Length bit (R/W-0)
 *
 * 1 = Length of PS2 is determined by the PHSEG2[2:0] bits of CNF3
 *
 * 0 = Length of PS2 is the greater of PS1 and IPT (2 TQs)
 */
#define CNF2_BIT_BTLMODE			0x80



/**
 * @brief CONFIGURATION REGISTER 1 (ADDRESS= 2Ah)
 */
#define REG_CNF1 			0x2A

/**
 * @brief Baud Rate Prescaler bits (R/W-0)
 *
 * TQ = 2 x (BRP[5:0] + 1)/FOSC.
 */
#define CNF1_BIT_BRP				0x3F

/**
 * @brief Synchronization Jump Width Length bits (R/W-0)
 *
 * 11 = Length = 4 x TQ
 *
 * 10 = Length = 3 x TQ
 *
 * 01 = Length = 2 x TQ
 *
 * 00 = Length = 1 x TQ
 */
#define CNF1_BIT_SJW				0xC0



/**
 * @brief CAN INTERRUPT ENABLE REGISTER (ADDRESS= 2Bh)
 */
#define REG_CANINTE 		0x2B

/**
 * @brief Receive Buffer n Full Interrupt Enable bit (R/W-0)
 *
 * 1 = Interrupt when message was received in RXBn
 *
 * 0 = Disabled
 *
 * @param n Buffernumber 0 - 1
 */
#define CANINTE_BIT_RXnIE(n)		(0x01 << n)

/**
 * @brief Transmit Buffer 0 Empty Interrupt Enable bit (R/W-0)
 *
 * 1 = Interrupt on TXBn becoming empty
 *
 * 0 = Disabled
 *
 * @param n Buffernumber 0 - 2
 */
#define CANINTE_BIT_TXnIE(n)		(0x04 << n)

/**
 * @brief Error Interrupt Enable bit (multiple sources in EFLG register) (R/W-0)
 *
 * 1 = Interrupt on EFLG error condition change
 *
 * 0 = Disabled
 */
#define CANINTE_BIT_ERRIE			0x20

/**
 * @brief Wake-up Interrupt Enable bit (R/W-0)
 *
 * 1 = Interrupt on CAN bus activity
 *
 * 0 = Disabled
 */
#define CANINTE_BIT_WAKIE			0x40

/**
 * @brief Message Error Interrupt Enable bit (R/W-0)
 *
 * 1 = Interrupt on error during message reception or transmission
 *
 * 0 = Disabled
 */
#define CANINTE_BIT_MERRE			0x80



/**
 * @brief CAN INTERRUPT FLAG REGISTER (ADDRESS= 2Ch)
 */
#define REG_CANINTF 		0x2C

/**
 * @brief Receive Buffer n Full Interrupt Flag bit (R/W-0)
 *
 * 1 = Interrupt is pending (must be cleared by MCU to reset the interrupt condition)
 *
 * 0 = No interrupt is pending
 *
 * @param n Buffernumber 0 - 1
 */
#define CANINTF_BIT_RXnIF(n)		(0x01 << n)

/**
 * @brief Transmit Buffer n Empty Interrupt Flag bit (R/W-0)
 *
 * 1 = Interrupt is pending (must be cleared by MCU to reset the interrupt condition)
 *
 * 0 = No interrupt is pending
 *
 * @param n Buffernumber 0 - 2
 */
#define CANINTF_BIT_TXnIF(n)		(0x04 << n)

/**
 * @brief Error Interrupt Flag bit (multiple sources in EFLG register) (R/W-0)
 *
 * 1 = Interrupt is pending (must be cleared by MCU to reset the interrupt condition)
 *
 * 0 = No interrupt is pending
 */
#define CANINTF_BIT_ERRIF			0x20

/**
 * @brief Wake-up Interrupt Flag bit (R/W-0)
 *
 * 1 = Interrupt is pending (must be cleared by MCU to reset the interrupt condition)
 *
 * 0 = No interrupt is pending
 */
#define CANINTF_BIT_WAKIF			0x40

/**
 * @brief Message Error Interrupt Flag bit (R/W-0)
 *
 * 1 = Interrupt is pending (must be cleared by MCU to reset the interrupt condition)
 *
 * 0 = No interrupt is pending
 */
#define CANINTF_BIT_MERRF			0x80



/**
 * @brief ERROR FLAG REGISTER (ADDRESS= 2Dh)
 */
#define REG_EFLG 			0x2D

/**
 * @brief Error Warning Flag bit (R-0)
 *
 * Sets when TEC or REC is equal to or greater than 96 (TXWAR or RXWAR = 1).
 *
 * Resets when both REC and TEC are less than 96
 */
#define EFLG_BIT_EWARN			0x01

/**
 * @brief Receive Error Warning Flag bit (R-0)
 *
 * Sets when REC is equal to or greater than 96
 *
 * Resets when REC is less than 96
 */
#define EFLG_BIT_RXWAR			0x02

/**
 * @brief Transmit Error Warning Flag bit (R-0)
 *
 * Sets when TEC is equal to or greater than 96
 *
 * Resets when TEC is less than 96
 */
#define EFLG_BIT_TXWAR			0x04

/**
 * @brief Receive Error-Passive Flag bit (R-0)
 *
 * Sets when REC is equal to or greater than 128
 *
 * Resets when REC is less than 128
 */
#define EFLG_BIT_RXEP			0x08

/**
 * @brief Transmit Error-Passive Flag bit (R-0)
 *
 * Sets when TEC is equal to or greater than 128
 *
 * Resets when TEC is less than 128
 */
#define EFLG_BIT_TXEP			0x10

/**
 * @brief Bus-Off Error Flag bit (R-0)
 *
 * Sets when TEC reaches 255
 *
 * Resets after a successful bus recovery sequence
 */
#define EFLG_BIT_TXBO			0x20

/**
 * @brief Receive Buffer n Overflow Flag bit (R/W-0)
 *
 * Sets when a valid message is received for RXBn and RXnIF = 1
 *
 * Must be reset by MCU
 *
 * @param n Buffernumber 0 - 1
 */
#define EFLG_BIT_RXnOVR(n)		(0xC0 << n)



/**
 * @brief TRANSMIT BUFFER n CONTROL REGISTER (ADDRESS= 30h, 40h, 50h)
 *
 * @param n Buffernumber 0 - 2
 */
#define REG_TXBnCTRL(n) 	(0x30 + (n * 0x10))

/**
 * @brief Transmit Buffer Priority bits (R/W-0)
 *
 * 11 = Highest message priority
 *
 * 10 = High intermediate message priority
 *
 * 01 = Low intermediate message priority
 *
 * 00 = Lowest message priority
 */
#define TXBnCTRL_BIT_TXP			0x03

/**
 * @brief Message Transmit Request bit (R/W-0)
 *
 * 1 = Buffer is currently pending transmission (MCU sets this bit to request message be transmitted – bit is automatically cleared when the message is sent)
 *
 * 0 = Buffer is not currently pending transmission (MCU can clear this bit to request a message abort)
 */
#define TXBnCTRL_BIT_TXREQ			0x08

/**
 * @brief Transmission Error Detected bit (R-0)
 *
 * 1 = A bus error occurred while the message was being transmitted
 *
 * 0 = No bus error occurred while the message was being transmitted
 */
#define TXBnCTRL_BIT_TXERR			0x10

/**
 * @brief Message Lost Arbitration bit (R-0)
 *
 * 1 = Message lost arbitration while being sent
 *
 * 0 = Message did not lose arbitration while being sent
 */
#define TXBnCTRL_BIT_MLOA			0x20

/**
 * @brief Message Aborted Flag bit (R-0)
 *
 * 1 = Message was aborted
 *
 * 0 = Message completed transmission successfully
 */
#define TXBnCTRL_BIT_ABTF			0x40



/**
 * @brief TRANSMIT BUFFER n STANDARD IDENTIFIER REGISTER HIGH (ADDRESS= 31h, 41h, 51h)
 *
 * @param n Buffernumber 0 - 2
 */
#define REG_TXBnSIDH(n) 	(0x31 + (n * 0x10))

/**
 * @brief Standard Identifier bits (R/W-x)
 */
#define TXBnSIDH_BIT_SID			0xFF



/**
 * @brief TRANSMIT BUFFER n STANDARD IDENTIFIER REGISTER LOW (ADDRESS= 32h, 42h, 52h)
 *
 * @param n Buffernumber 0 - 2
 */
#define REG_TXBnSIDL(n) 	(0x32 + (n * 0x10))

/**
 * @brief Extended Identifier bits (R/W-x)
 */
#define TXBnSIDL_BIT_EID			0x03

/**
 * @brief Extended Identifier Enable bit (R/W-x)
 *
 * 1 = Message will transmit Extended Identifier
 *
 * 0 = Message will transmit Standard Identifier
 */
#define TXBnSIDL_BIT_EXIDE			0x08

/**
 * @brief Standard Identifier bits (R/W-x)
 */
#define TXBnSIDL_BIT_SID			0xE0



/**
 * @brief TRANSMIT BUFFER n EXTENDED IDENTIFIER 8 REGISTER HIGH (ADDRESS= 33h, 43h, 53h)
 *
 * @param n Buffernumber 0 - 2
 */
#define REG_TXBnEID8(n) 	(0x33 + (n * 0x10))

/**
 * @brief Extended Identifier bits (R/W-x)
 */
#define TXBnEID8_BIT_EID			0xFF



/**
 * @brief TRANSMIT BUFFER n EXTENDED IDENTIFIER 0 REGISTER LOW (ADDRESS= 34h, 44h, 54h)
 *
 * @param n Buffernumber 0 - 2
 */
#define REG_TXBnEID0(n) 	(0x34 + (n * 0x10))

/**
 * @brief Extended Identifier bits (R/W-x)
 */
#define TXBnEID0_BIT_EID			0xFF



/**
 * @brief TRANSMIT BUFFER n DATA LENGTH CODE REGISTER (ADDRESS= 35h, 45h, 55h)
 *
 * @param n Buffernumber 0 - 2
 */
#define REG_TXBnDLC(n) 		(0x35 + (n * 0x10))

/**
 * @brief Data Length Code bits (R/W-x)
 *
 * Sets the number of data bytes to be transmitted (0 to 8 bytes).
 * @note It is possible to set the DLC[3:0] bits to a value greater than eight; however, only eight bytes are transmitted.
 */
#define TXBnDLC_BIT_DLC			    0x0F

/**
 * @brief Remote Transmission Request bit (R/W-x)
 *
 * 1 = Transmitted message will be a remote transmit request
 *
 * 0 = Transmitted message will be a data frame
 */
#define TXBnDLC_BIT_RTR			    0x40



/**
 * @brief TRANSMIT BUFFER n DATA BYTE 0 REGISTER (ADDRESS= 0x46, 0x56)
 *
 * @param n Buffernumber 0 - 2
 */
#define REG_TXBnD0(n) 		(0x36 + (n * 0x10))

/**
 * @brief TRANSMIT BUFFER n DATA BYTE 1 REGISTER (ADDRESS= 0x47, 0x57)
 *
 * @param n Buffernumber 0 - 2
 */
#define REG_TXBnD1(n) 		(0x37 + (n * 0x10))

/**
 * @brief TRANSMIT BUFFER n DATA BYTE 2 REGISTER (ADDRESS= 0x48, 0x58)
 *
 * @param n Buffernumber 0 - 2
 */
#define REG_TXBnD2(n) 		(0x38 + (n * 0x10))

/**
 * @brief TRANSMIT BUFFER n DATA BYTE 3 REGISTER (ADDRESS= 0x49, 0x59)
 *
 * @param n Buffernumber 0 - 2
 */
#define REG_TXBnD3(n) 		(0x39 + (n * 0x10))

/**
 * @brief TRANSMIT BUFFER n DATA BYTE 4 REGISTER (ADDRESS= 0x4A, 0x5A)
 *
 * @param n Buffernumber 0 - 2
 */
#define REG_TXBnD4(n) 		(0x3A + (n * 0x10))

/**
 * @brief TRANSMIT BUFFER n DATA BYTE 5 REGISTER (ADDRESS= 0x4B, 0x5B)
 *
 * @param n Buffernumber 0 - 2
 */
#define REG_TXBnD5(n) 		(0x3B + (n * 0x10))

/**
 * @brief TRANSMIT BUFFER n DATA BYTE 6 REGISTER (ADDRESS= 0x4C, 0x5C)
 *
 * @param n Buffernumber 0 - 2
 */
#define REG_TXBnD6(n) 		(0x3C + (n * 0x10))

/**
 * @brief TRANSMIT BUFFER n DATA BYTE 7 REGISTER (ADDRESS= 0x4D, 0x5D)
 *
 * @param n Buffernumber 0 - 2
 */
#define REG_TXBnD7(n) 		(0x3D + (n * 0x10))

/**
 * @brief Transmit Buffer n Data Field Byte m bits (R/W-x)
 */
#define TXBnDm_BIT_TXBnDm		    0xFF



/**
 * @brief RECEIVE BUFFER n CONTROL REGISTER (ADDRESS= 0x60, 0x70)
 *
 * @param n Buffernumber 0 - 1
 */
#define REG_RXBnCTRL(n)		(0x60 + (n * 0x10))

/**
 * @brief Filter Hit bit (indicates which acceptance filter enabled reception of message) (R-0)
 *
 * FOR BUFFER 0 ONLY
 *
 * 1 = Acceptance Filter 1 (RXF1)
 *
 * 0 = Acceptance Filter 0 (RXF0)
 *
 * @note If a rollover from RXB0 to RXB1 occurs, the FILHIT0 bit will reflect the filter that accepted the message that rolled over.
 */
#define RXBnCTRL_BIT_BUFFER0_FILHIT0    0x01

/**
 * @brief Filter Hit bits (indicates which acceptance filter enabled reception of message) (R-0)
 *
 * FOR BUFFER 1 ONLY
 *
 * 101 = Acceptance Filter 5 (RXF5)
 *
 * 100 = Acceptance Filter 4 (RXF4)
 *
 * 011 = Acceptance Filter 3 (RXF3)
 *
 * 010 = Acceptance Filter 2 (RXF2)
 *
 * 001 = Acceptance Filter 1 (RXF1) (only if the BUKT bit is set in RXB0CTRL)
 *
 * 000 = Acceptance Filter 0 (RXF0) (only if the BUKT bit is set in RXB0CTRL)
 * @note If a rollover from RXB0 to RXB1 occurs, the FILHIT0 bit will reflect the filter that accepted the message that rolled over.
 */
#define RXBnCTRL_BIT_BUFFER1_FILHIT0    0x07

/**
 * @brief Rollover Enable bit (R/W-0)
 *
 * FOR BUFFER 0 ONLY
 *
 * 1 = RXB0 message will roll over and be written to RXB1 if RXB0 is full
 *
 * 0 = Rollover is disabled
 */
#define RXBnCTRL_BIT_BUFFER0_BUKT    0x07

/**
 * @brief Received Remote Transfer Request bit (R-0)
 *
 * 1 = Remote Transfer Request received
 *
 * 0 = No Remote Transfer Request received
 */
#define RXBnCTRL_BIT_RXRTR    0x08

/**
 * @brief Receive Buffer Operating mode bits (R/W-0)
 *
 * 11 = Turns mask/filters off; receives any message
 *
 * 10 = Reserved
 *
 * 01 = Reserved
 *
 * 00 = Receives all valid messages using either Standard or Extended Identifiers that meet filter criteria;
 * Extended ID Filter registers, RXFnEID8:RXFnEID0, are applied to the first two bytes of data in the messages with standard IDs
 */
#define RXBnCTRL_BIT_RXM    0x60



/**
 * @brief RECEIVE BUFFER n STANDARD IDENTIFIER REGISTER HIGH (ADDRESS= 61h, 71h)
 *
 * @param n Buffernumber 0 - 1
 */
#define REG_RXBnSIDH(n) 	(0x61 + (n * 0x10))

/**
 * @brief Standard Identifier bits (R-x)
 *
 * These bits contain the eight Most Significant bits of the Standard Identifier for the received message.
 */
#define RXBnSIDH_BIT_SID    0xFF



/**
 * @brief RECEIVE BUFFER n STANDARD IDENTIFIER REGISTER LOW (ADDRESS= 62h, 72h)
 *
 * @param n Buffernumber 0 - 1
 */
#define REG_RXBnSIDL(n) 	(0x62 + (n * 0x10))

/**
 * @brief Standard Identifier bits (R-x)
 *
 * These bits contain the eight Most Significant bits of the Standard Identifier for the received message.
 */
#define RXBnSIDL_BIT_EID    0x03

/**
 * @brief Extended Identifier Flag bit (R-x)
 *
 * This bit indicates whether the received message was a standard or an extended frame.
 *
 * 1 = Received message was an extended frame
 *
 * 0 = Received message was a standard frame
 */
#define RXBnSIDL_BIT_IDE    0x08

/**
 * @brief Extended Identifier Flag bit (R-x)
 *
 * Standard Frame Remote Transmit Request bit (valid only if IDE bit = 0)
 *
 * 1 = Standard frame Remote Transmit Request received
 *
 * 0 = Standard data frame received
 */
#define RXBnSIDL_BIT_SRR    0x10

/**
 * @brief Standard Identifier bits (R-x)
 *
 * These bits contain the three Least Significant bits of the Standard Identifier for the received message.
 */
#define RXBnSIDL_BIT_SID    0xE0



/**
 * @brief RECEIVE BUFFER n EXTENDED IDENTIFIER REGISTER HIGH (ADDRESS= 63h, 73h)
 *
 * @param n Buffernumber 0 - 1
 */
#define REG_RXBnEID8(n) 	(0x63 + (n * 0x10))

/**
 * @brief Extended Identifier bits (R-x)
 *
 * These bits hold bits 15 through 8 of the Extended Identifier for the received message
 */
#define RXBnEID8_BIT_EID    0xFF



/**
 * @brief RECEIVE BUFFER n EXTENDED IDENTIFIER REGISTER HIGH (ADDRESS= 64h, 74h)
 *
 * @param n Buffernumber 0 - 1
 */
#define REG_RXBnEID0(n) 	(0x64 + (n * 0x10))

/**
 * @brief Extended Identifier bits (R-x)
 *
 * These bits hold the Least Significant eight bits of the Extended Identifier for the received message.
 */
#define RXBnEID0_BIT_EID    0xFF



/**
 * @brief RECEIVE BUFFER n DATA LENGTH CODE REGISTER (ADDRESS= 65h, 75h)
 *
 * @param n Buffernumber 0 - 1
 */
#define REG_RXBnDLC(n) 		(0x65 + (n * 0x10))

/**
 * @brief Data Length Code bits (R-x)
 *
 * Indicates the number of data bytes that were received.
 */
#define RXBnDLC_BIT_DLC    0x0F

/**
 * @brief Extended Frame Remote Transmission Request bit (valid only when IDE (RXBnSIDL[3]) = 1) (R-x)
 *
 * 1 = Extended frame Remote Transmit Request received
 *
 * 0 = Extended data frame received
 */
#define RXBnDLC_BIT_RTR    0x40



/**
 * @brief RECEIVE BUFFER n DATA BYTE 0 REGISTER (ADDRESS= 0x66, 0x76)
 *
 * @param n Buffernumber 0 - 1
 */
#define REG_RXBnD0(n) 		(0x66 + (n * 0x10))		// and Register 0x76 => n = 0..1

/**
 * @brief RECEIVE BUFFER n DATA BYTE 1 REGISTER (ADDRESS= 0x67, 0x77)
 *
 * @param n Buffernumber 0 - 1
 */
#define REG_RXBnD1(n) 		(0x67 + (n * 0x10))		// and Register 0x77 => n = 0..1

/**
 * @brief RECEIVE BUFFER n DATA BYTE 2 REGISTER (ADDRESS= 0x68, 0x78)
 *
 * @param n Buffernumber 0 - 1
 */
#define REG_RXBnD2(n) 		(0x68 + (n * 0x10))		// and Register 0x78 => n = 0..1

/**
 * @brief RECEIVE BUFFER n DATA BYTE 3 REGISTER (ADDRESS= 0x69, 0x79)
 *
 * @param n Buffernumber 0 - 1
 */
#define REG_RXBnD3(n) 		(0x69 + (n * 0x10))		// and Register 0x79 => n = 0..1

/**
 * @brief RECEIVE BUFFER n DATA BYTE 4 REGISTER (ADDRESS= 0x6A, 0x7A)
 *
 * @param n Buffernumber 0 - 1
 */
#define REG_RXBnD4(n) 		(0x6A + (n * 0x10))		// and Register 0x7A => n = 0..1

/**
 * @brief RECEIVE BUFFER n DATA BYTE 5 REGISTER (ADDRESS= 0x6B, 0x7B)
 *
 * @param n Buffernumber 0 - 1
 */
#define REG_RXBnD5(n) 		(0x6B + (n * 0x10))		// and Register 0x7B => n = 0..1

/**
 * @brief RECEIVE BUFFER n DATA BYTE 6 REGISTER (ADDRESS= 0x6C, 0x7C)
 *
 * @param n Buffernumber 0 - 1
 */
#define REG_RXBnD6(n) 		(0x6C + (n * 0x10))		// and Register 0x7C => n = 0..1

/**
 * @brief RECEIVE BUFFER n DATA BYTE 7 REGISTER (ADDRESS= 0x6D, 0x7D)
 *
 * @param n Buffernumber 0 - 1
 */
#define REG_RXBnD7(n) 		(0x6D + (n * 0x10))		// and Register 0x7D => n = 0..1

/**
 * @brief Receive Buffer n Data Field Bytes m bits (R-x)
 *
 * Eight bytes containing the data bytes for the received message.
 */
#define RXBnDm_BIT_RBnD		    0xFF


#endif
