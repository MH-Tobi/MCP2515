#ifndef MCP2515ERROR_H
#define MCP2515ERROR_H

#define ERROR_MCP2515_NOT_INITIALIZED                   0x1000      // Occurs when a Method is called, but the MCP2515 is still not initialized.
#define ERROR_MCP2515_VALUE_OUTA_RANGE                  0x2000      // Occurs when given Value is not in the defined range.
#define ERROR_MCP2515_FALSE_OPERATION_MODE              0x3000      // Occurs when the MCP2515 is in the wrong Operation Mode.
#define ERROR_MCP2515_WRITE_INSTRUCTION                 0x4000      // Occurs when the write-Instruction failed.
#define ERROR_MCP2515_BITMODIFY_INSTRUCTION             0x5000      // Occurs when the BitModify-Instruction failed.
#define ERROR_MCP2515_VALUE_NOT_SET                     0x6000      // Occurs when setting a Register-Value wasn't successfull (recheck Set-Process).
#define ERROR_MCP2515_FALSE_SETTING                     0x7000      // Occurs when a defined Setting-Condition is not complied with.
#define ERROR_MCP2515_MASK_NOT_VALID                    0x8000      // Occurs when a not valid Mask is used.
#define ERROR_MCP2515_UNKNOWN_SWITCH                    0x9000      // Occurs when an unknown Error occurs during a switch-case.
#define ERROR_MCP2515_BAUDRATE_NOT_VALID                0xA000      // Occurs when a Baudrate is choose which is not allowed.
#define ERROR_MCP2515_CLOCKFREQUENCY_NOT_VALID          0xB000      // Occurs when a ClockFrequency is choose which is not allowed.
#define ERROR_MCP2515_CNF_NOT_FOUND                     0xC000      // Occurs when the CNF-Configuration couldn't be found.
#define ERROR_MCP2515_CNF1_NOT_SET                      0xD000      // Occurs when the CNF1-Configuration couldn't be set.
#define ERROR_MCP2515_CNF2_NOT_SET                      0xE000      // Occurs when the CNF2-Configuration couldn't be set.
#define ERROR_MCP2515_CNF3_NOT_SET                      0xF000      // Occurs when the CNF3-Configuration couldn't be set.

#define ERROR_MCP2515_CONFIGURATIONMODE_NOT_SET         0x0100      // Occurs when the Configuration-Mode couldn't be found.
#define ERROR_MCP2515_OPERATION_MODE_NOT_SET            0x0200      // Occurs when the Operation-Mode couldn't be set.
#define ERROR_MCP2515_RESET_INTERRUPT_FLAG              0x0300      // Occurs when the reset of an Interrupt-Flag failed.

#define ERROR_SPI_CONFIGURATION_MODE_NOT_SET            0x1F00      // Occurs when in the Reset-Instruction the Configuration-Mode wasn't set.
#define ERROR_SPI_REGISTER_NOT_ALLOWED                  0x2F00      // Occurs when in the bitModify-Instruction the given Register-Adress is not allowed.
#define ERROR_SPI_VALUE_OUTA_RANGE                      0x3F00      // Occurs when given Value is not in the defined range.

#define ERROR_MCP2515_TRANSMITBUFFER_NOT_FREE           0x10F0      // Occurs when the given TransmitBuffer is not ready to fill.
#define ERROR_MCP2515_TRANSMITBUFFER_ID_FILLING         0x20F0      // Occurs when during filling the ID in TransmitBuffer an Error occured.
#define ERROR_MCP2515_TRANSMITBUFFER_DATA_FILLING       0x30F0      // Occurs when during filling the Data in TransmitBuffer an Error occured.

#define ERROR_MCP2515_INIT_RESET_FAILED                 0xF100      // Occurs when during the initialisation the Reset-Instruction failed.
#define ERROR_MCP2515_INIT_CHANGE_BITTIMING             0xF200      // Occurs when during the initialisation the BitTiming couldn't be changed.
#define ERROR_MCP2515_INIT_TX_PIN_CONTROL               0xF300      // Occurs when during the initialisation the TX-Pins couldn't be set proberly.
#define ERROR_MCP2515_INIT_RX_PIN_CONTROL               0xF400      // Occurs when during the initialisation the RX-Pins couldn't be set proberly.
#define ERROR_MCP2515_INIT_SET_INTERRUPTS               0xF500      // Occurs when during the initialisation the Interrupts couldn't be set proberly.
#define ERROR_MCP2515_INIT_DISABLE_MASKS_FILTERS        0xF600      // Occurs when during the initialisation the Mask and Filter couldn't be disabled.
#define ERROR_MCP2515_INIT_SET_NORMAL_MODE              0xF700      // Occurs when during the initialisation the Normal Operationmode couldn't be set.

#define ERROR_MCP2515_NOT_IMPLEMENTED                   0xFFFF      // Occurs when Method is not implemented yet.

#define EMPTY_VALUE_8_BIT                               0x00
#define EMPTY_VALUE_16_BIT                              0x0000
#define EMPTY_VALUE_32_BIT                              0x00000000
#define EMPTY_VALUE_64_BIT                              0x0000000000000000

#endif
