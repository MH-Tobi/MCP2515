# Error

## Main Errors

| Error | Value | Occurrence |
| :---- | :--- | :--------- |
| ERROR_MCP2515_BAUDRATE_NOT_VALID | 0x1000 | Occurs when a baudrate that is not allowed is selected. |
| ERROR_MCP2515_CLOCKFREQUENCY_NOT_VALID | 0x1100 | Occurs when a ClockFrequency that is not allowed is selected. |
| ERROR_MCP2515_CNF_NOT_FOUND | 0x1200 | Occurs when the CNF-Configuration could not be found. |
| ERROR_MCP2515_DATA_ORDER_NOT_ALLOWED | 0x1300 | Occurs when a SPI-DataOrder that is not allowed is selected. |
| ERROR_MCP2515_FALSE_OPERATION_MODE | 0x1400 | Occurs when the MCP2515 is in the wrong Operation Mode. |
| ERROR_MCP2515_FALSE_SETTING | 0x1500 | Occurs when a defined Setting-Condition is not complied with. |
| ERROR_MCP2515_INIT_CS_PIN_NOT_DEFINED | 0x1600 | Occurs when no CS-Pin is defined. |
| ERROR_MCP2515_IS_INITIALIZED | 0x1700 | Occurs when a method is called that does not allow MCP2515 to be initialized. |
| ERROR_MCP2515_MASK_NOT_VALID | 0x1800 | Occurs when an invalid mask is used. |
| ERROR_MCP2515_NOT_INITIALIZED | 0x1900 | Occurs when a method is called but the MCP2515 is not yet initialized. |
| ERROR_MCP2515_NO_TRANSMITBUFFER_FREE | 0x1A00 | Occurs when no transmit buffer is free. |
| ERROR_MCP2515_SPI_FREQUENCY_NOT_ALLOWED | 0x1B00 | Occurs when a SPI-Frequency that is not allowed is selected. |
| ERROR_MCP2515_SPI_MODE_NOT_ALLOWED | 0x1C00 | Occurs when a SPI-Mode that is not allowed is selected. |
| ERROR_MCP2515_TRANSMITBUFFER_NOT_FREE | 0x1D00 | Occurs when the selected transmit buffer is not free. |
| ERROR_MCP2515_UNDEFINED | 0x1E00 | Undefined Error (should not occur). |
| ERROR_MCP2515_UNKNOWN_SWITCH | 0x1F00 | Undefined Switch-Case Error (should not occur). |
| ERROR_MCP2515_VALUE_NOT_SET | 0x2000 | Occurs when setting a register value was not successful. |
| ERROR_MCP2515_VALUE_OUTA_RANGE | 0x2100 | Occurs when the specified value is not in the defined range. |
| ERROR_SPI_CONFIGURATION_MODE_NOT_SET | 0x2200 | Occurs if the configuration mode was not set in the Reset-Instruction. |
| ERROR_SPI_NO_TRANSMISSION_INITIATED | 0x2300 | Occurs when no transmission is to be initiated for the RTS-Instruction. |
| ERROR_SPI_REGISTER_NOT_ALLOWED | 0x2400 | Occurs when the specified register address is not allowed in the BitModify-Instruction. |
| ERROR_SPI_VALUE_OUTA_RANGE | 0x2500 | Occurs when the specified value for the SPI-Instruction is not within the defined range. |


## Secondary Errors

| Error | Value | Occurrence |
| :---- | :--- | :--------- |
| ERROR_MCP2515_INIT_RESET_FAILED | 0x0001 | Occurs when the Reset-Instruction failed during initialization. |
| ERROR_MCP2515_INIT_CHANGE_BITTIMING | 0x0002 | Occurs when the BitTiming could not be changed during initialization. |
| ERROR_MCP2515_INIT_TX_PIN_CONTROL | 0x0003 | Occurs when the TX pins could not be set correctly during initialization. |
| ERROR_MCP2515_INIT_RX_PIN_CONTROL | 0x0004 | Occurs when the RX pins could not be set correctly during initialization. |
| ERROR_MCP2515_INIT_SET_INTERRUPTS | 0x0005 | Occurs when the Interrupts could not be set correctly during initialization. |
| ERROR_MCP2515_INIT_SET_TX_BUFFER_FLAGS | 0x0006 | Occurs when the TX buffer flags cannot be set correctly during initialization. |
| ERROR_MCP2515_INIT_DISABLE_MASKS_FILTERS | 0x0007 | Occurs when the mask and filter could not be deactivated during initialization. |
| ERROR_MCP2515_INIT_SET_NORMAL_MODE | 0x0008 | Occurs when the normal operating mode could not be set during initialization. |
| ERROR_MCP2515_BITMODIFY_INSTRUCTION | 0x0010 | Occurs when the BitModify-Instruction failed. |
| ERROR_MCP2515_CNF1_NOT_SET | 0x0020 | Occurs when the CNF1-Configuration could not be set. |
| ERROR_MCP2515_CNF2_NOT_SET | 0x0030 | Occurs when the CNF2-Configuration could not be set. |
| ERROR_MCP2515_CNF3_NOT_SET | 0x0040 | Occurs when the CNF3-Configuration could not be set. |
| ERROR_MCP2515_OPERATION_MODE_NOT_SET | 0x0050 | Occurs when the operating mode could not be set. |
| ERROR_MCP2515_CHANGE_INTERRUPT | 0x0051 | Occurs when the Interrupt-Settings could not be changed. |
| ERROR_MCP2515_CHECK_FREE_TRANSMIT_BUFFER | 0x0052 | Occurs when an error occurs while checking for a free TransmitBuffer. |
| ERROR_MCP2515_TRANSMITBUFFER_ID_FILLING | 0x0053 | Occurs when an error occurs while filling the ID in TransmitBuffer. |
| ERROR_MCP2515_TRANSMITBUFFER_DATA_FILLING | 0x0054 | Occurs when an error occurs while filling the Data in TransmitBuffer. |
| ERROR_MCP2515_INITIATE_SENDING | 0x0055 | Occurs when initiating the sending of a message failed. |
| ERROR_MCP2515_GET_EXTENDED_FLAG | 0x0056 | Occurs when retrieving the Extended-Flag failed. |
| ERROR_MCP2515_GET_STANDARD_ID | 0x0057 | Occurs when retrieving the Standard-ID failed. |
| ERROR_MCP2515_GET_EXTENDED_ID | 0x0058 | Occurs when retrieving the Extended-ID failed. |
| ERROR_MCP2515_RESET_FLAG | 0x0059 | Occurs when resetting an Interrupt-Flag failed. |
| ERROR_MCP2515_GET_DLC | 0x005A | Occurs when retrieving the DLC failed. |
| ERROR_MCP2515_GET_DATA | 0x005B | Occurs when retrieving the Data failed. |
| ERROR_MCP2515_GET_ALL_DATA | 0x005C | Occurs when an error occurs while retrieving all data. |
| ERROR_MCP2515_MESSAGE_SENDING_ABORTED | 0x0060 | Occurs when the send operation was aborted. |
| ERROR_MCP2515_ERROR_DURING_SENDING | 0x0001 | Appended to "ERROR_MCP2515_MESSAGE_SENDING_ABORTED" if the send operation was aborted due to a method error. |
| ERROR_MCP2515_NOT_IMPLEMENTED | 0xFFFF | Occurs when Method is not implemented yet. |
