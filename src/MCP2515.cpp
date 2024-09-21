#include "MCP2515.h"


/***********************************************************************************************************************
 * 									Private Methods
 **********************************************************************************************************************/



/**
 * @brief Get the Filter Standard Identifier High Register (REG_RXFnSIDH) [00h, 04h, 08h, 10h, 14h, 18h]
 * @param FilterNumber 0 - 5
 * @return uint8_t Standard Identifier [10:3]
 *
 * On Error it will return EMPTY_VALUE_8_BIT (Check _lastMcpError).
 * @note The Mask and Filter registers read all ‘0’s when in any mode except Configuration mode.
 *
 * Standard Identifier [2:0] can be get from getFilterStandardIdentifierLow().
 */
uint8_t MCP2515::getFilterStandardIdentifierHigh(uint8_t FilterNumber)
{
  if (FilterNumber > 5)
  {
    this->_lastMcpError = ERROR_MCP2515_VALUE_OUTA_RANGE;
    return EMPTY_VALUE_8_BIT;
  }

  return readInstruction(REG_RXFnSIDH(FilterNumber));

}

/**
 * @brief Set the Filter Standard Identifier High Register (REG_RXFnSIDH) [00h, 04h, 08h, 10h, 14h, 18h]
 * @param FilterNumber 0 - 5
 * @param StandardId_10_3 uint8_t Bit 10-3 of the Standard Identifier
 * @return Result of setting
 *
 * True = Setting successfull
 *
 * False = Error occured during Setting (Check _lastMcpError)
 * @note The Mask and Filter registers read all ‘0’s when in any mode except Configuration mode.
 *
 * Standard Identifier [2:0] can be set in setFilterStandardIdentifierLow().
 */
bool MCP2515::setFilterStandardIdentifierHigh(uint8_t FilterNumber, uint8_t StandardId_10_3)
{
  if (_operationMode != MCP2515_OP_CONFIGURATION)
  {
    this->_lastMcpError = ERROR_MCP2515_FALSE_OPERATION_MODE;
    return false;
  }

  if (FilterNumber > 5 || StandardId_10_3 > 255)
  {
    this->_lastMcpError = ERROR_MCP2515_VALUE_OUTA_RANGE;
    return false;
  }

  writeInstruction(REG_RXFnSIDH(FilterNumber), StandardId_10_3);

  if (_reCheckEnabled)
  {
    if (getFilterStandardIdentifierHigh(FilterNumber) != StandardId_10_3)
    {
      this->_lastMcpError = ERROR_MCP2515_VALUE_NOT_SET;
      return false;
    }
  }

  return true;
}

/**
 * @brief Get the Filter Standard Identifier Low Register (REG_RXFnSIDL) [01h, 05h, 09h, 11h, 15h, 19h]
 * @param FilterNumber 0 - 5
 * @return
 * Bit 7 - 5 -> uint8_t Standard Identifier [2:0]
 *
 * Bit 4 -> bool Extended Identifier Enable bit [EXIDE]
 *
 * 0 = Filter is applied only to standard frames
 *
 * 1 = Filter is applied only to extended frames
 *
 * Bit 1 - 0 -> uint8_t Extended Identifier [17:16]
 *
 * On Error it will return EMPTY_VALUE_8_BIT (Check _lastMcpError).
 * @note The Mask and Filter registers read all ‘0’s when in any mode except Configuration mode.
 *
 * Standard Identifier [10:3] can be get from getFilterStandardIdentifierHigh().
 *
 * Extended Identifier [15:8] and [7:0] can be get from getFilterExtendedIdentifierHigh() and getFilterExtendedIdentifierLow().
 */
uint8_t MCP2515::getFilterStandardIdentifierLow(uint8_t FilterNumber)
{
  if (FilterNumber > 5)
  {
    this->_lastMcpError = ERROR_MCP2515_VALUE_OUTA_RANGE;
    return EMPTY_VALUE_8_BIT;
  }

  return readInstruction(REG_RXFnSIDL(FilterNumber));
}

/**
 * @brief Set the Filter Standard Identifier Low Register (REG_RXFnSIDL) [01h, 05h, 09h, 11h, 15h, 19h]
 * @param FilterNumber 0 - 5
 * @param StandardId_10_3 uint8_t Bit 10-3 of the Standard Identifier
 * @param ExtendedIdEn bool Extended Identifier Enable bit [EXIDE]
 *
 * 0 = Filter is applied only to standard frames
 *
 * 1 = Filter is applied only to extended frames
 * @param ExtendedId_17_16 uint8_t Bit 17-16 of the Extended Identifier
 * @return Result of setting
 *
 * True = Setting successfull
 *
 * False = Error occured during Setting (Check _lastMcpError)
 * @note The Mask and Filter registers read all ‘0’s when in any mode except Configuration mode.
 *
 * Standard Identifier [10:3] can be set in setFilterStandardIdentifierHigh().
 *
 * Extended Identifier [15:8] and [7:0] can be set in setFilterExtendedIdentifierHigh() and setFilterExtendedIdentifierLow().
 */
bool MCP2515::setFilterStandardIdentifierLow(uint8_t FilterNumber, uint8_t StandardId_2_0, bool ExtendedIdEn, uint8_t ExtendedId_17_16)
{
  if (_operationMode != MCP2515_OP_CONFIGURATION)
  {
    this->_lastMcpError = ERROR_MCP2515_FALSE_OPERATION_MODE;
    return false;
  }

  if (FilterNumber > 5 || StandardId_2_0 > 7 || ExtendedId_17_16 > 3 || (ExtendedIdEn != true && ExtendedIdEn != false))
  {
    this->_lastMcpError = ERROR_MCP2515_VALUE_OUTA_RANGE;
    return false;
  }

  uint8_t Data = StandardId_2_0 << 5 | ExtendedIdEn << 3 | ExtendedId_17_16;

  writeInstruction(REG_RXFnSIDL(FilterNumber), Data);

  if (_reCheckEnabled)
  {
    if (getFilterStandardIdentifierLow(FilterNumber) != Data)
    {
      this->_lastMcpError = ERROR_MCP2515_VALUE_NOT_SET;
      return false;
    }
  }

  return true;
}

/**
 * @brief Get the Filter Extended Identifier High Register (REG_RXFnEID8) [02h, 06h, 0Ah, 12h, 16h, 1Ah]
 * @param FilterNumber 0 - 5
 * @return uint8_t Extended Identifier [15:8]
 *
 * On Error it will return EMPTY_VALUE_8_BIT (Check _lastMcpError).
 * @note The Mask and Filter registers read all ‘0’s when in any mode except Configuration mode.
 *
 * Extended Identifier [17:16] and [7:0] can be get from getFilterStandardIdentifierLow() and getFilterExtendedIdentifierLow().
 */
uint8_t MCP2515::getFilterExtendedIdentifierHigh(uint8_t FilterNumber)
{
  if (FilterNumber > 5)
  {
    this->_lastMcpError = ERROR_MCP2515_VALUE_OUTA_RANGE;
    return EMPTY_VALUE_8_BIT;
  }

  return readInstruction(REG_RXFnEID8(FilterNumber));
}

/**
 * @brief Set the Filter Extended Identifier High Register (REG_RXFnEID8) [02h, 06h, 0Ah, 12h, 16h, 1Ah]
 * @param FilterNumber 0 - 5
 * @param ExtendedId_15_8 uint8_t Bit 15-8 of the Extended Identifier
 * @return Result of setting
 *
 * True = Setting successfull
 *
 * False = Error occured during Setting (Check _lastMcpError)
 * @note The Mask and Filter registers read all ‘0’s when in any mode except Configuration mode.
 *
 * Extended Identifier [17:16] and [7:0] can be set in setFilterStandardIdentifierLow() and setFilterExtendedIdentifierLow().
 */
bool MCP2515::setFilterExtendedIdentifierHigh(uint8_t FilterNumber, uint8_t ExtendedId_15_8)
{
  if (_operationMode != MCP2515_OP_CONFIGURATION)
  {
    this->_lastMcpError = ERROR_MCP2515_FALSE_OPERATION_MODE;
    return false;
  }

  if (FilterNumber > 5 || ExtendedId_15_8 > 255)
  {
    this->_lastMcpError = ERROR_MCP2515_VALUE_OUTA_RANGE;
    return false;
  }

  writeInstruction(REG_RXFnEID8(FilterNumber), ExtendedId_15_8);

  if (_reCheckEnabled)
  {
    if (getFilterExtendedIdentifierHigh(FilterNumber) != ExtendedId_15_8)
    {
      this->_lastMcpError = ERROR_MCP2515_VALUE_NOT_SET;
      return false;
    }
  }

  return true;
}

/**
 * @brief Get the Filter Extended Identifier Low Register (REG_RXFnEID0) [03h, 07h, 0Bh, 13h, 17h, 1Bh]
 * @param FilterNumber 0 - 5
 * @return uint8_t Extended Identifier [7:0]
 *
 * On Error it will return EMPTY_VALUE_8_BIT (Check _lastMcpError).
 * @note The Mask and Filter registers read all ‘0’s when in any mode except Configuration mode.
 *
 * Extended Identifier [17:16] and [15:8] can be get from getFilterStandardIdentifierLow() and getFilterExtendedIdentifierHigh().
 */
uint8_t MCP2515::getFilterExtendedIdentifierLow(uint8_t FilterNumber)
{
  if (FilterNumber > 5)
  {
    this->_lastMcpError = ERROR_MCP2515_VALUE_OUTA_RANGE;
    return EMPTY_VALUE_8_BIT;
  }

  return readInstruction(REG_RXFnEID0(FilterNumber));
}

/**
 * @brief Set the Filter Extended Identifier Low Register (REG_RXFnEID0) [03h, 07h, 0Bh, 13h, 17h, 1Bh]
 * @param FilterNumber 0 - 5
 * @param ExtendedId_7_0 uint8_t Bit 7-0 of the Extended Identifier
 * @return Result of setting
 *
 * True = Setting successfull
 *
 * False = Error occured during Setting (Check _lastMcpError)
 * @note The Mask and Filter registers read all ‘0’s when in any mode except Configuration mode.
 *
 * Extended Identifier [17:16] and [15:8] can be set in getFilterStandardIdentifierLow() and getFilterExtendedIdentifierHigh().
 */
bool MCP2515::setFilterExtendedIdentifierLow(uint8_t FilterNumber, uint8_t ExtendedId_7_0)
{
  if (_operationMode != MCP2515_OP_CONFIGURATION)
  {
    this->_lastMcpError = ERROR_MCP2515_FALSE_OPERATION_MODE;
    return false;
  }

  if (FilterNumber > 5 || ExtendedId_7_0 > 255)
  {
    this->_lastMcpError = ERROR_MCP2515_VALUE_OUTA_RANGE;
    return false;
  }

  writeInstruction(REG_RXFnEID0(FilterNumber), ExtendedId_7_0);

  if (_reCheckEnabled)
  {
    if (getFilterExtendedIdentifierLow(FilterNumber) != ExtendedId_7_0)
    {
      this->_lastMcpError = ERROR_MCP2515_VALUE_NOT_SET;
      return false;
    }
  }

  return true;
}

/**
 * @brief Get the Mask Standard Identifier High Register (REG_RXMnSIDH) [20h, 24h]
 * @param MaskNumber 0 - 1
 * @return uint8_t Standard Identifier [10:3]
 *
 * On Error it will return EMPTY_VALUE_8_BIT (Check _lastMcpError).
 * @note The Mask and Filter registers read all ‘0’s when in any mode except Configuration mode.
 *
 * Standard Identifier [2:0] can be get from getMaskStandardIdentifierLow().
 */
uint8_t MCP2515::getMaskStandardIdentifierHigh(uint8_t MaskNumber)
{
  if (MaskNumber > 1)
  {
    this->_lastMcpError = ERROR_MCP2515_VALUE_OUTA_RANGE;
    return EMPTY_VALUE_8_BIT;
  }

  return readInstruction(REG_RXMnSIDH(MaskNumber));
}

/**
 * @brief Set the Mask Standard Identifier High Register (REG_RXMnSIDH) [20h, 24h]
 * @param MaskNumber 0 - 1
 * @param StandardId_10_3 uint8_t Bit 10-3 of the Standard Identifier
 * @return Result of setting
 *
 * True = Setting successfull
 *
 * False = Error occured during Setting (Check _lastMcpError)
 * @note The Mask and Filter registers read all ‘0’s when in any mode except Configuration mode.
 *
 * Standard Identifier [2:0] can be set in setMaskStandardIdentifierLow().
 */
bool MCP2515::setMaskStandardIdentifierHigh(uint8_t MaskNumber, uint8_t StandardId_10_3)
{
  if (_operationMode != MCP2515_OP_CONFIGURATION)
  {
    this->_lastMcpError = ERROR_MCP2515_FALSE_OPERATION_MODE;
    return false;
  }

  if (MaskNumber > 1 || StandardId_10_3 > 255)
  {
    this->_lastMcpError = ERROR_MCP2515_VALUE_OUTA_RANGE;
    return false;
  }

  writeInstruction(REG_RXMnSIDH(MaskNumber), StandardId_10_3);

  if (_reCheckEnabled)
  {
    if (getMaskStandardIdentifierHigh(MaskNumber) != StandardId_10_3)
    {
      this->_lastMcpError = ERROR_MCP2515_VALUE_NOT_SET;
      return false;
    }
  }

  return true;
}

/**
 * @brief Get the Mask Standard Identifier Low Register (REG_RXMnSIDL) [21h, 25h]
 * @param MaskNumber 0 - 1
 * @return
 * Bit 7 - 5 -> uint8_t Standard Identifier [2:0]
 *
 * Bit 1 - 0 -> uint8_t Extended Identifier [17:16]
 *
 * On Error it will return EMPTY_VALUE_8_BIT (Check _lastMcpError).
 * @note The Mask and Filter registers read all ‘0’s when in any mode except Configuration mode.
 *
 * Standard Identifier [10:3] can be get from getMaskStandardIdentifierHigh().
 *
 * Extended Identifier [15:8] and [7:0] can be get from getMaskExtendedIdentifierHigh() and getMaskExtendedIdentifierLow().
 */
uint8_t MCP2515::getMaskStandardIdentifierLow(uint8_t MaskNumber)
{
  if (MaskNumber > 1)
  {
    this->_lastMcpError = ERROR_MCP2515_VALUE_OUTA_RANGE;
    return EMPTY_VALUE_8_BIT;
  }

  return readInstruction(REG_RXMnSIDL(MaskNumber));
}

/**
 * @brief Set the Mask Standard Identifier Low Register (REG_RXMnSIDL) [21h, 25h]
 * @param MaskNumber 0 - 1
 * @param StandardId_2_0 uint8_t Bit 2-0 of the Standard Identifier
 * @param ExtendedId_17_16 uint8_t Bit 17-16 of the Extended Identifier
 * @return Result of setting
 *
 * True = Setting successfull
 *
 * False = Error occured during Setting (Check _lastMcpError)
 * @note The Mask and Filter registers read all ‘0’s when in any mode except Configuration mode.
 *
 * Standard Identifier [10:3] can be set in setMaskStandardIdentifierHigh().
 *
 * Extended Identifier [15:8] and [7:0] can be set in setMaskExtendedIdentifierHigh() and setMaskExtendedIdentifierLow().
 */
bool MCP2515::setMaskStandardIdentifierLow(uint8_t MaskNumber, uint8_t StandardId_2_0, uint8_t ExtendedId_17_16)
{
  if (_operationMode != MCP2515_OP_CONFIGURATION)
  {
    this->_lastMcpError = ERROR_MCP2515_FALSE_OPERATION_MODE;
    return false;
  }

  if (MaskNumber > 1 || StandardId_2_0 > 7 || ExtendedId_17_16 > 3)
  {
    this->_lastMcpError = ERROR_MCP2515_VALUE_OUTA_RANGE;
    return false;
  }

  uint8_t Data = StandardId_2_0 << 5 | ExtendedId_17_16;

  writeInstruction(REG_RXMnSIDL(MaskNumber), Data);

  if (_reCheckEnabled)
  {
    if (getMaskStandardIdentifierLow(MaskNumber) != Data)
    {
      this->_lastMcpError = ERROR_MCP2515_VALUE_NOT_SET;
      return false;
    }
  }

  return true;
}

/**
 * @brief Get the Mask Extended Identifier High Register (REG_RXMnEID8) [22h, 26h]
 * @param MaskNumber 0 - 1
 * @return uint8_t Extended Identifier [15:8]
 *
 * On Error it will return EMPTY_VALUE_8_BIT (Check _lastMcpError).
 * @note The Mask and Filter registers read all ‘0’s when in any mode except Configuration mode.
 *
 * Extended Identifier [17:16] and [7:0] can be get from getMaskStandardIdentifierLow() and getMaskExtendedIdentifierLow().
 */
uint8_t MCP2515::getMaskExtendedIdentifierHigh(uint8_t MaskNumber)
{
  if (MaskNumber > 1)
  {
    this->_lastMcpError = ERROR_MCP2515_VALUE_OUTA_RANGE;
    return EMPTY_VALUE_8_BIT;
  }

  return readInstruction(REG_RXMnEID8(MaskNumber));
}

/**
 * @brief Set the Mask Extended Identifier High Register (REG_RXMnEID8) [22h, 26h]
 * @param MaskNumber 0 - 1
 * @param ExtendedId_15_8 uint8_t Bit 15-8 of the Extended Identifier
 * @return Result of setting
 *
 * True = Setting successfull
 *
 * False = Error occured during Setting (Check _lastMcpError)
 * @note The Mask and Filter registers read all ‘0’s when in any mode except Configuration mode.
 *
 * Extended Identifier [17:16] and [7:0] can be set in setMaskStandardIdentifierLow() and setMaskExtendedIdentifierLow().
 */
bool MCP2515::setMaskExtendedIdentifierHigh(uint8_t MaskNumber, uint8_t ExtendedId_15_8)
{
  if (_operationMode != MCP2515_OP_CONFIGURATION)
  {
    this->_lastMcpError = ERROR_MCP2515_FALSE_OPERATION_MODE;
    return false;
  }

  if (MaskNumber > 1 || ExtendedId_15_8 > 255)
  {
    this->_lastMcpError = ERROR_MCP2515_VALUE_OUTA_RANGE;
    return false;
  }

  writeInstruction(REG_RXMnEID8(MaskNumber), ExtendedId_15_8);

  if (_reCheckEnabled)
  {
    if (getMaskExtendedIdentifierHigh(MaskNumber) != ExtendedId_15_8)
    {
      this->_lastMcpError = ERROR_MCP2515_VALUE_NOT_SET;
      return false;
    }
  }

  return true;
}

/**
 * @brief Get the Mask Extended Identifier Low Register (REG_RXMnEID0) [23h, 27h]
 * @param MaskNumber 0 - 1
 * @return uint8_t Extended Identifier [7:0]
 *
 * On Error it will return EMPTY_VALUE_8_BIT (Check _lastMcpError).
 * @note The Mask and Filter registers read all ‘0’s when in any mode except Configuration mode.
 *
 * Extended Identifier [17:16] and [15:8] can be get from getMaskStandardIdentifierLow() and getMaskExtendedIdentifierHigh().
 */
uint8_t MCP2515::getMaskExtendedIdentifierLow(uint8_t MaskNumber)
{
  if (MaskNumber > 1)
  {
    this->_lastMcpError = ERROR_MCP2515_VALUE_OUTA_RANGE;
    return EMPTY_VALUE_8_BIT;
  }

  return readInstruction(REG_RXMnEID0(MaskNumber));
}

/**
 * @brief Set the Mask Extended Identifier Low Register (REG_RXMnEID0) [23h, 27h]
 * @param MaskNumber 0 - 1
 * @param ExtendedId_7_0 uint8_t Bit 7-0 of the Extended Identifier
 * @return Result of setting
 *
 * True = Setting successfull
 *
 * False = Error occured during Setting (Check _lastMcpError)
 * @note The Mask and Filter registers read all ‘0’s when in any mode except Configuration mode.
 *
 * Extended Identifier [17:16] and [15:8] can be set in setMaskStandardIdentifierLow() and setMaskExtendedIdentifierHigh().
 */
bool MCP2515::setMaskExtendedIdentifierLow(uint8_t MaskNumber, uint8_t ExtendedId_7_0)
{
  if (_operationMode != MCP2515_OP_CONFIGURATION)
  {
    this->_lastMcpError = ERROR_MCP2515_FALSE_OPERATION_MODE;
    return false;
  }

  if (MaskNumber > 1 || ExtendedId_7_0 > 255)
  {
    this->_lastMcpError = ERROR_MCP2515_VALUE_OUTA_RANGE;
    return false;
  }

  writeInstruction(REG_RXMnEID0(MaskNumber), ExtendedId_7_0);

  if (_reCheckEnabled)
  {
    if (getMaskExtendedIdentifierLow(MaskNumber) != ExtendedId_7_0)
    {
      this->_lastMcpError = ERROR_MCP2515_VALUE_NOT_SET;
      return false;
    }
  }

  return true;
}

/**
 * @brief Get the RXnBF Pin Control and Status Register (REG_BFPCTRL) [0Ch]
 * @return
 * Bit 5 -> B1BFS - RX1BF Pin State bit (Digital Output mode only)
 *
 * Reads as ‘0’ when RX1BF is configured as an interrupt pin
 *
 * Bit 4 -> B0BFS - RX0BF Pin State bit (Digital Output mode only)
 *
 * Reads as ‘0’ when RX1BF is configured as an interrupt pin
 *
 * Bit 3 -> B1BFE - RX1BF Pin Function Enable bit
 *
 * 1 = Pin function is enabled, operation mode is determined by the B1BFM bit
 *
 * 0 = Pin function is disabled, pin goes to a high-impedance state
 *
 * Bit 2 -> B0BFE - RX0BF Pin Function Enable bit
 *
 * 1 = Pin function is enabled, operation mode is determined by the B0BFM bit
 *
 * 0 = Pin function is disabled, pin goes to a high-impedance state
 *
 * Bit 1 -> B1BFM - RX1BF Pin Operation mode bit
 *
 * 1 = Pin is used as an interrupt when a valid message is loaded into RXB1
 *
 * 0 = Digital Output mode
 *
 * Bit 0 -> B0BFM - RX0BF Pin Operation mode bit
 *
 * 1 = Pin is used as an interrupt when a valid message is loaded into RXB0
 *
 * 0 = Digital Output mode
 *
 * No Error will be set.
 */
uint8_t MCP2515::getRXnBFPinControl()
{
  return readInstruction(REG_BFPCTRL);
}

/**
 * @brief Set the RXnBF Pin Control and Status Register (REG_BFPCTRL) [0Ch]
 * @param B1BFS RX1BF Pin State bit (Digital Output mode only)
 *
 * Reads as ‘0’ when RX1BF is configured as an interrupt pin
 * @param B0BFS RX0BF Pin State bit (Digital Output mode only)
 *
 * Reads as ‘0’ when RX1BF is configured as an interrupt pin
 * @param B1BFE RX1BF Pin Function Enable bit
 *
 * 1 = Pin function is enabled, operation mode is determined by the B1BFM bit
 *
 * 0 = Pin function is disabled, pin goes to a high-impedance state
 * @param B0BFE RX0BF Pin Function Enable bit
 *
 * 1 = Pin function is enabled, operation mode is determined by the B0BFM bit
 *
 * 0 = Pin function is disabled, pin goes to a high-impedance state
 * @param B1BFM RX1BF Pin Operation mode bit
 *
 * 1 = Pin is used as an interrupt when a valid message is loaded into RXB1
 *
 * 0 = Digital Output mode
 * @param B0BFM RX0BF Pin Operation mode bit
 *
 * 1 = Pin is used as an interrupt when a valid message is loaded into RXB0
 *
 * 0 = Digital Output mode
 * @return Result of setting
 *
 * True = Setting successfull
 *
 * False = Error occured during Setting (Check _lastMcpError)
 */
bool MCP2515::setRXnBFPinControl(bool B1BFS, bool B0BFS, bool B1BFE, bool B0BFE, bool B1BFM, bool B0BFM)
{
  if ((B1BFS != true && B1BFS != false) ||
      (B0BFS != true && B0BFS != false) ||
      (B1BFE != true && B1BFE != false) ||
      (B0BFE != true && B0BFE != false) ||
      (B1BFM != true && B1BFM != false) ||
      (B0BFM != true && B0BFM != false))
  {
    this->_lastMcpError = ERROR_MCP2515_VALUE_OUTA_RANGE;
    return false;
  }

  uint8_t Data = B1BFS << 5 | B0BFS << 4 | B1BFE << 3 | B0BFE << 2 | B1BFM << 1 | B0BFM;

  writeInstruction(REG_BFPCTRL, Data);

  if (_reCheckEnabled)
  {
    if (getRXnBFPinControl() != Data)
    {
      this->_lastMcpError = ERROR_MCP2515_VALUE_NOT_SET;
      return false;
    }
  }

  return true;
}

/**
 * @brief Modify the RXnBF Pin Control and Status Register (REG_BFPCTRL) [0Ch]
 * @param Mask Masked Bits to modify
 *
 * Bit 5 -> B1BFS - RX1BF Pin State bit (Digital Output mode only)
 *
 * Reads as ‘0’ when RX1BF is configured as an interrupt pin
 *
 * Bit 4 -> B0BFS - RX0BF Pin State bit (Digital Output mode only)
 *
 * Reads as ‘0’ when RX1BF is configured as an interrupt pin
 *
 * Bit 3 -> B1BFE - RX1BF Pin Function Enable bit
 *
 * 1 = Pin function is enabled, operation mode is determined by the B1BFM bit
 *
 * 0 = Pin function is disabled, pin goes to a high-impedance state
 *
 * Bit 2 -> B0BFE - RX0BF Pin Function Enable bit
 *
 * 1 = Pin function is enabled, operation mode is determined by the B0BFM bit
 *
 * 0 = Pin function is disabled, pin goes to a high-impedance state
 *
 * Bit 1 -> B1BFM - RX1BF Pin Operation mode bit
 *
 * 1 = Pin is used as an interrupt when a valid message is loaded into RXB1
 *
 * 0 = Digital Output mode
 *
 * Bit 0 -> B0BFM - RX0BF Pin Operation mode bit
 *
 * 1 = Pin is used as an interrupt when a valid message is loaded into RXB0
 *
 * 0 = Digital Output mode
 * @param Value Bitwise 0 or 1
 * @return Result of setting
 *
 * True = Setting successfull
 *
 * False = Error occured during Setting (Check _lastMcpError)
 */
bool MCP2515::modifyRXnBFPinControl(uint8_t Mask, uint8_t Value)
{
  if ((Mask & ~(BFPCTRL_BIT_BnBFS(0) | BFPCTRL_BIT_BnBFS(1) |
                BFPCTRL_BIT_BnBFE(0) | BFPCTRL_BIT_BnBFE(1) |
                BFPCTRL_BIT_BnBFM(0) | BFPCTRL_BIT_BnBFM(1))) != 0x00)
  {
    this->_lastMcpError = ERROR_MCP2515_MASK_NOT_VALID;
    return false;
  }

  if (!bitModifyInstruction(REG_BFPCTRL, Mask, Value))
  {
    this->_lastMcpError = _lastMcpError | ERROR_MCP2515_BITMODIFY_INSTRUCTION;
    return false;
  }

  if (_reCheckEnabled)
  {
    if ((getRXnBFPinControl() & Mask) != (Value & Mask))
    {
      this->_lastMcpError = ERROR_MCP2515_VALUE_NOT_SET;
      return false;
    }
  }

  return true;
}

/**
 * @brief Get the TXnRTS Pin Control and Status Register (REG_TXRTSCTRL) [0Dh]
 * @return
 * Bit 5 -> B2RTS - TX2RTS Pin State bit
 *
 * Reads state of TX2RTS pin when in Digital Input mode
 *
 * Reads as ‘0’ when pin is in Request-to-Send mode
 *
 * Bit 4 -> B1RTS - TX1RTS Pin State bit
 *
 * Reads state of TX1RTS pin when in Digital Input mode
 *
 * Reads as ‘0’ when pin is in Request-to-Send mode
 *
 * Bit 3 -> B0RTS - TX0RTS Pin State bit
 *
 * Reads state of TX0RTS pin when in Digital Input mode
 *
 * Reads as ‘0’ when pin is in Request-to-Send mode
 *
 * Bit 2 -> B2RTSM - TX2RTS Pin mode bit
 *
 * 1 = Pin is used to request message transmission of TXB2 buffer (on falling edge)
 *
 * 0 = Digital input
 *
 * Bit 1 -> B1RTSM - TX1RTS Pin mode bit
 *
 * 1 = Pin is used to request message transmission of TXB1 buffer (on falling edge)
 *
 * 0 = Digital input
 *
 * Bit 0 -> B0RTSM - TX0RTS Pin mode bit
 *
 * 1 = Pin is used to request message transmission of TXB0 buffer (on falling edge)
 *
 * 0 = Digital input
 *
 * No Error will be set.
 */
uint8_t MCP2515::getTXnRTSPinControl()
{
  return readInstruction(REG_TXRTSCTRL);
}

/**
 * @brief Set the TXnRTS Pin Control and Status Register (REG_TXRTSCTRL) [0Dh]
 * @param B2RTSM TX2RTS Pin mode bit
 *
 * 1 = Pin is used to request message transmission of TXB2 buffer (on falling edge)
 *
 * 0 = Digital input
 * @param B1RTSM TX1RTS Pin mode bit
 *
 * 1 = Pin is used to request message transmission of TXB1 buffer (on falling edge)
 *
 * 0 = Digital input
 * @param B0RTSM TX0RTS Pin mode bit
 *
 * 1 = Pin is used to request message transmission of TXB0 buffer (on falling edge)
 *
 * 0 = Digital input
 * @return Result of setting
 *
 * True = Setting successfull
 *
 * False = Error occured during Setting (Check _lastMcpError)
 * @note TXRTSCTRL register only modifiable in Configuration mode.
 */
bool MCP2515::setTXnRTSPinControl(bool B2RTSM, bool B1RTSM, bool B0RTSM)
{
  if (_operationMode != MCP2515_OP_CONFIGURATION)
  {
    this->_lastMcpError = ERROR_MCP2515_FALSE_OPERATION_MODE;
    return false;
  }

  if ((B2RTSM != true && B2RTSM != false) ||
      (B1RTSM != true && B1RTSM != false) ||
      (B0RTSM != true && B0RTSM != false))
  {
    this->_lastMcpError = ERROR_MCP2515_VALUE_OUTA_RANGE;
    return false;
  }

  uint8_t Data = B2RTSM << 2 | B1RTSM << 1 | B0RTSM;

  writeInstruction(REG_TXRTSCTRL, Data);

  if (_reCheckEnabled)
  {
    if ((getTXnRTSPinControl() & (TXRTSCTRL_BIT_BnRTSM(0) | TXRTSCTRL_BIT_BnRTSM(1) | TXRTSCTRL_BIT_BnRTSM(2))) != Data)
    {
      this->_lastMcpError = ERROR_MCP2515_VALUE_NOT_SET;
      return false;
    }
  }

  return true;
}

/**
 * @brief Modify the TXnRTS Pin Control and Status Register (REG_TXRTSCTRL) [0Dh]
 * @param Mask Masked Bits to modify
 *
 * Bit 2 -> B2RTSM - TX2RTS Pin mode bit
 *
 * 1 = Pin is used to request message transmission of TXB2 buffer (on falling edge)
 *
 * 0 = Digital input
 *
 * Bit 1 -> B1RTSM - TX1RTS Pin mode bit
 *
 * 1 = Pin is used to request message transmission of TXB1 buffer (on falling edge)
 *
 * 0 = Digital input
 *
 * Bit 0 -> B0RTSM - TX0RTS Pin mode bit
 *
 * 1 = Pin is used to request message transmission of TXB0 buffer (on falling edge)
 *
 * 0 = Digital input
 * @param Value Bitwise 0 or 1
 * @return Result of setting
 *
 * True = Setting successfull
 *
 * False = Error occured during Setting (Check _lastMcpError)
 * @note TXRTSCTRL register only modifiable in Configuration mode.
 */
bool MCP2515::modifyTXnRTSPinControl(uint8_t Mask, uint8_t Value)
{
  if (_operationMode != MCP2515_OP_CONFIGURATION)
  {
    this->_lastMcpError = ERROR_MCP2515_FALSE_OPERATION_MODE;
    return false;
  }

  if ((Mask & ~(TXRTSCTRL_BIT_BnRTSM(0) | TXRTSCTRL_BIT_BnRTSM(1) | TXRTSCTRL_BIT_BnRTSM(2))) != 0x00)
  {
    this->_lastMcpError = ERROR_MCP2515_MASK_NOT_VALID;
    return false;
  }

  if (!bitModifyInstruction(REG_TXRTSCTRL, Mask, Value))
  {
    this->_lastMcpError = _lastMcpError | ERROR_MCP2515_BITMODIFY_INSTRUCTION;
    return false;
  }

  if (_reCheckEnabled)
  {
    if ((getTXnRTSPinControl() & Mask) != (Value & Mask))
    {
      this->_lastMcpError = ERROR_MCP2515_VALUE_NOT_SET;
      return false;
    }
  }

  return true;
}

/**
 * @brief Get the CAN Status Register (REG_CANSTAT) [-Eh]
 * @return
 * Bit 7 - 5 -> OPMOD - Operation Mode bits
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
 *
 * Bit 3 - 1 -> ICOD - Interrupt Flag Code bits
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
 *
 * No Error will be set.
 */
uint8_t MCP2515::getCanStatus()
{
  return readInstruction(REG_CANSTAT);
}

/**
 * @brief Get the CAN Control Register (REG_CANCTRL) [-Fh]
 * @return
 * Bit 7 - 5 -> REQOP - Request Operation Mode bits
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
 *
 * Bit 4 -> ABAT - Abort All Pending Transmissions bit
 *
 * 1 = Requests abort of all pending transmit buffers
 *
 * 0 = Terminates request to abort all transmissions
 *
 * Bit 3 -> OSM - One-Shot Mode bit
 *
 * 1 = Enabled; messages will only attempt to transmit one time
 *
 * 0 = Disabled; messages will reattempt transmission if required
 *
 * Bit 2 -> CLKEN - CLKOUT Pin Enable bit
 *
 * 1 = CLKOUT pin is enabled
 *
 * 0 = CLKOUT pin is disabled (pin is in high-impedance state)
 *
 * Bit 1 - 0 -> CLKPRE - CLKOUT Pin Prescaler bits
 *
 * 00 = FCLKOUT = System Clock/1
 *
 * 01 = FCLKOUT = System Clock/2
 *
 * 10 = FCLKOUT = System Clock/4
 *
 * 11 = FCLKOUT = System Clock/8
 *
 * No Error will be set.
 */
uint8_t MCP2515::getCanControl()
{
  return readInstruction(REG_CANCTRL);
}

/**
 * @brief Set the CAN Control Register (REG_CANCTRL) [-Fh]
 * @param REQOP Request Operation Mode bits
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
 * @param ABAT Abort All Pending Transmissions bit
 *
 * 1 = Requests abort of all pending transmit buffers
 *
 * 0 = Terminates request to abort all transmissions
 * @param OSM One-Shot Mode bit
 *
 * 1 = Enabled; messages will only attempt to transmit one time
 *
 * 0 = Disabled; messages will reattempt transmission if required
 * @param CLKEN CLKOUT Pin Enable bit
 *
 * 1 = CLKOUT pin is enabled
 *
 * 0 = CLKOUT pin is disabled (pin is in high-impedance state)
 * @param CLKPRE CLKOUT Pin Prescaler bits
 *
 * 00 = FCLKOUT = System Clock/1
 *
 * 01 = FCLKOUT = System Clock/2
 *
 * 10 = FCLKOUT = System Clock/4
 *
 * 11 = FCLKOUT = System Clock/8
 * @return Result of setting
 *
 * True = Setting successfull
 *
 * False = Error occured during Setting (Check _lastMcpError)
 */
bool MCP2515::setCanControl(uint8_t REQOP, bool ABAT, bool OSM, bool CLKEN, uint8_t CLKPRE)
{
  if ((REQOP > 4) ||
      (ABAT != true && ABAT != false) ||
      (OSM != true && OSM != false) ||
      (CLKEN != true && CLKEN != false) ||
      (CLKPRE > 3))
  {
    this->_lastMcpError = ERROR_MCP2515_VALUE_OUTA_RANGE;
    return false;
  }

  uint8_t Data = REQOP << 5 | ABAT << 4 | OSM << 3 | CLKEN << 2 | CLKPRE;

  writeInstruction(REG_CANCTRL, Data);

  if (_reCheckEnabled)
  {
    if (getCanControl() != Data)
    {
      this->_lastMcpError = ERROR_MCP2515_VALUE_NOT_SET;
      return false;
    }
  }

  return true;
}

/**
 * @brief Modify the CAN Control Register (REG_CANCTRL) [-Fh]
 * @param Mask Masked Bits to modify
 *
 * Bit 7 - 5 -> REQOP - Request Operation Mode bits
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
 *
 * Bit 4 -> ABAT - Abort All Pending Transmissions bit
 *
 * 1 = Requests abort of all pending transmit buffers
 *
 * 0 = Terminates request to abort all transmissions
 *
 * Bit 3 -> OSM - One-Shot Mode bit
 *
 * 1 = Enabled; messages will only attempt to transmit one time
 *
 * 0 = Disabled; messages will reattempt transmission if required
 *
 * Bit 2 -> CLKEN - CLKOUT Pin Enable bit
 *
 * 1 = CLKOUT pin is enabled
 *
 * 0 = CLKOUT pin is disabled (pin is in high-impedance state)
 *
 * Bit 1 - 0 -> CLKPRE - CLKOUT Pin Prescaler bits
 *
 * 00 = FCLKOUT = System Clock/1
 *
 * 01 = FCLKOUT = System Clock/2
 *
 * 10 = FCLKOUT = System Clock/4
 *
 * 11 = FCLKOUT = System Clock/8
 * @param Value Bitwise 0 or 1
 * @return Result of setting
 *
 * True = Setting successfull
 *
 * False = Error occured during Setting (Check _lastMcpError)
 */
bool MCP2515::modifyCanControl(uint8_t Mask, uint8_t Value)
{
  if ((Mask & CANCTRL_BIT_REQOP) != 0x00)
  {
    // Check if Result of bitModify results in an invalid Condition. (CANCTRL_BIT_REQOP never > 4)
    if ((((getCanControl() & ~Mask) >> 5) | ((Value & Mask) >> 5)) > 4)
    {
      this->_lastMcpError = ERROR_MCP2515_FALSE_SETTING;
      return false;
    }
  }

  if (!bitModifyInstruction(REG_CANCTRL, Mask, Value))
  {
    this->_lastMcpError = _lastMcpError | ERROR_MCP2515_BITMODIFY_INSTRUCTION;
    return false;
  }

  delayMicroseconds(100);

  if (_reCheckEnabled)
  {
    if ((getCanControl() & Mask) != (Value & Mask))
    {
      this->_lastMcpError = ERROR_MCP2515_VALUE_NOT_SET;
      return false;
    }
  }

  return true;
}

/**
 * @brief Get the Transmit Error Counter Register (REG_TEC) [1Ch]
 * @return uint8_t Transmit Error Count
 *
 * No Error will be set.
 */
uint8_t MCP2515::getTransmitErrorCounter()
{
  return readInstruction(REG_TEC);
}

/**
 * @brief Get the Receive Error Counter Register (REG_REC) [1Dh]
 * @return uint8_t Receive Error Count
 *
 * No Error will be set.
 */
uint8_t MCP2515::getReceiveErrorCounter()
{
  return readInstruction(REG_REC);
}

/**
 * @brief Get the Configuration 3 Register (REG_CNF3) [28h]
 * @return
 * Bit 7 -> SOF - Start-of-Frame signal bit
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
 *
 * Bit 6 -> WAKFIL - Wake-up Filter bit
 *
 * 1 = Wake-up filter is enabled
 *
 * 0 = Wake-up filter is disabled
 *
 * Bit 2 - 0 -> PHSEG2 - PS2 Length bits
 *
 * (PHSEG2 + 1) x TQ. Minimum valid setting for PS2 is 2 TQs.
 *
 * No Error will be set.
 */
uint8_t MCP2515::getConfigurationRegister3()
{
  return readInstruction(REG_CNF3);
}

/**
 * @brief Set the Configuration 3 Register (REG_CNF3) [28h]
 * @param SOF Start-of-Frame signal bit
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
 * @param WAKFIL Wake-up Filter bit
 *
 * 1 = Wake-up filter is enabled
 *
 * 0 = Wake-up filter is disabled
 * @param PHSEG2 PS2 Length bits
 *
 * (PHSEG2 + 1) x TQ. Minimum valid setting for PS2 is 2 TQs.
 * @return Result of setting
 *
 * True = Setting successfull
 *
 * False = Error occured during Setting (Check _lastMcpError)
 * @note CNF3 register only modifiable in Configuration mode.
 */
bool MCP2515::setConfigurationRegister3(bool SOF, bool WAKFIL, uint8_t PHSEG2)
{
  if (_operationMode != MCP2515_OP_CONFIGURATION)
  {
    this->_lastMcpError = ERROR_MCP2515_FALSE_OPERATION_MODE;
    return false;
  }

  if ((SOF != true && SOF != false) ||
      (WAKFIL != true && WAKFIL != false) ||
      (PHSEG2 > 7))
  {
    this->_lastMcpError = ERROR_MCP2515_VALUE_OUTA_RANGE;
    return false;
  }

  uint8_t Data = SOF << 7 | WAKFIL << 6 | PHSEG2;

  writeInstruction(REG_CNF3, Data);

  if (_reCheckEnabled)
  {
    if (getConfigurationRegister3() != Data)
    {
      this->_lastMcpError = ERROR_MCP2515_VALUE_NOT_SET;
      return false;
    }
  }

  return true;
}

/**
 * @brief Modify the Configuration 3 Register (REG_CNF3) [28h]
 * @param Mask Masked Bits to modify
 *
 * Bit 7 -> SOF - Start-of-Frame signal bit
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
 *
 * Bit 6 -> WAKFIL - Wake-up Filter bit
 *
 * 1 = Wake-up filter is enabled
 *
 * 0 = Wake-up filter is disabled
 *
 * Bit 2 - 0 -> PHSEG2 - PS2 Length bits
 *
 * (PHSEG2 + 1) x TQ. Minimum valid setting for PS2 is 2 TQs.
 * @param Value Bitwise 0 or 1
 * @return Result of setting
 *
 * True = Setting successfull
 *
 * False = Error occured during Setting (Check _lastMcpError)
 * @note CNF3 register only modifiable in Configuration mode.
 */
bool MCP2515::modifyConfigurationRegister3(uint8_t Mask, uint8_t Value)
{
  if (_operationMode != MCP2515_OP_CONFIGURATION)
  {
    this->_lastMcpError = ERROR_MCP2515_FALSE_OPERATION_MODE;
    return false;
  }

  if ((Mask & ~(CNF3_BIT_SOF | CNF3_BIT_WAKFIL | CNF3_BIT_PHSEG2)) != 0x00)
  {
    this->_lastMcpError = ERROR_MCP2515_MASK_NOT_VALID;
    return false;
  }

  if (!bitModifyInstruction(REG_CNF3, Mask, Value))
  {
    this->_lastMcpError = _lastMcpError | ERROR_MCP2515_BITMODIFY_INSTRUCTION;
    return false;
  }

  if (_reCheckEnabled)
  {
    if ((getConfigurationRegister3() & Mask) != (Value & Mask))
    {
      this->_lastMcpError = ERROR_MCP2515_VALUE_NOT_SET;
      return false;
    }
  }

  return true;
}

/**
 * @brief Get the Configuration 2 Register (REG_CNF2) [29h]
 * @return
 * Bit 7 -> BTLMODE - PS2 Bit Time Length bit
 *
 * 1 = Length of PS2 is determined by the PHSEG2[2:0] bits of CNF3
 *
 * 0 = Length of PS2 is the greater of PS1 and IPT (2 TQs)
 *
 * Bit 6 -> SAM - Sample Point Configuration bit
 *
 * 1 = Bus line is sampled three times at the sample point
 *
 * 0 = Bus line is sampled once at the sample point
 *
 * Bit 5 - 3 -> PHSEG1 - PS1 Length bits
 *
 * (PHSEG1 + 1) x TQ.
 *
 * Bit 2 - 0 -> PRSEG - Propagation Segment Length bits
 *
 * (PRSEG + 1) x TQ.
 *
 * No Error will be set.
 */
uint8_t MCP2515::getConfigurationRegister2()
{
  return readInstruction(REG_CNF2);
}

/**
 * @brief Set the Configuration 2 Register (REG_CNF2) [29h]
 * @param BTLMODE PS2 Bit Time Length bit
 *
 * 1 = Length of PS2 is determined by the PHSEG2[2:0] bits of CNF3
 *
 * 0 = Length of PS2 is the greater of PS1 and IPT (2 TQs)
 * @param SAM Sample Point Configuration bit
 *
 * 1 = Bus line is sampled three times at the sample point
 *
 * 0 = Bus line is sampled once at the sample point
 * @param PHSEG1 PS1 Length bits
 *
 * (PHSEG1 + 1) x TQ.
 * @param PRSEG Propagation Segment Length bits
 *
 * (PRSEG + 1) x TQ.
 * @return Result of setting
 *
 * True = Setting successfull
 *
 * False = Error occured during Setting (Check _lastMcpError)
 * @note CNF2 register only modifiable in Configuration mode.
 */
bool MCP2515::setConfigurationRegister2(bool BTLMODE, bool SAM, uint8_t PHSEG1, uint8_t PRSEG)
{
  if (_operationMode != MCP2515_OP_CONFIGURATION)
  {
    this->_lastMcpError = ERROR_MCP2515_FALSE_OPERATION_MODE;
    return false;
  }

  if ((BTLMODE != true && BTLMODE != false) ||
      (SAM != true && SAM != false) ||
      (PHSEG1 > 7) ||
      (PRSEG > 7))
  {
    this->_lastMcpError = ERROR_MCP2515_VALUE_OUTA_RANGE;
    return false;
  }

  uint8_t Data = BTLMODE << 7 | SAM << 6 | PHSEG1 << 3 | PRSEG;

  writeInstruction(REG_CNF2, Data);

  if (_reCheckEnabled)
  {
    if (getConfigurationRegister2() != Data)
    {
      this->_lastMcpError = ERROR_MCP2515_VALUE_NOT_SET;
      return false;
    }
  }

  return true;
}

/**
 * @brief Modify the Configuration 2 Register (REG_CNF2) [29h]
 * @param Mask Masked Bits to modify
 *
 * Bit 7 -> BTLMODE - PS2 Bit Time Length bit
 *
 * 1 = Length of PS2 is determined by the PHSEG2[2:0] bits of CNF3
 *
 * 0 = Length of PS2 is the greater of PS1 and IPT (2 TQs)
 *
 * Bit 6 -> SAM - Sample Point Configuration bit
 *
 * 1 = Bus line is sampled three times at the sample point
 *
 * 0 = Bus line is sampled once at the sample point
 *
 * Bit 5 - 3 -> PHSEG1 - PS1 Length bits
 *
 * (PHSEG1 + 1) x TQ.
 *
 * Bit 2 - 0 -> PRSEG - Propagation Segment Length bits
 *
 * (PRSEG + 1) x TQ.
 * @param Value Bitwise 0 or 1
 * @return Result of setting
 *
 * True = Setting successfull
 *
 * False = Error occured during Setting (Check _lastMcpError)
 * @note CNF2 register only modifiable in Configuration mode.
 */
bool MCP2515::modifyConfigurationRegister2(uint8_t Mask, uint8_t Value)
{
  if (_operationMode != MCP2515_OP_CONFIGURATION)
  {
    this->_lastMcpError = ERROR_MCP2515_FALSE_OPERATION_MODE;
    return false;
  }

  if (!bitModifyInstruction(REG_CNF2, Mask, Value))
  {
    this->_lastMcpError = _lastMcpError | ERROR_MCP2515_BITMODIFY_INSTRUCTION;
    return false;
  }

  if (_reCheckEnabled)
  {
    if ((getConfigurationRegister2() & Mask) != (Value & Mask))
    {
      this->_lastMcpError = ERROR_MCP2515_VALUE_NOT_SET;
      return false;
    }
  }

  return true;
}

/**
 * @brief Get the Configuration 1 Register (REG_CNF1) [2Ah]
 * @return
 * Bit 7 - 6 -> SJW - Synchronization Jump Width Length bits
 *
 * 11 = Length = 4 x TQ
 *
 * 10 = Length = 3 x TQ
 *
 * 01 = Length = 2 x TQ
 *
 * 00 = Length = 1 x TQ
 *
 * Bit 5 - 0 -> BRP - Baud Rate Prescaler bits
 *
 * TQ = 2 x (BRP + 1)/FOSC.
 *
 * No Error will be set.
 */
uint8_t MCP2515::getConfigurationRegister1()
{
  return readInstruction(REG_CNF1);
}

/**
 * @brief Set the Configuration 1 Register (REG_CNF1) [2Ah]
 * @param SJW Synchronization Jump Width Length bits
 *
 * 11 = Length = 4 x TQ
 *
 * 10 = Length = 3 x TQ
 *
 * 01 = Length = 2 x TQ
 *
 * 00 = Length = 1 x TQ
 * @param BRP Baud Rate Prescaler bits
 *
 * TQ = 2 x (BRP + 1)/FOSC.
 * @return Result of setting
 *
 * True = Setting successfull
 *
 * False = Error occured during Setting (Check _lastMcpError)
 * @note CNF1 register only modifiable in Configuration mode.
 */
bool MCP2515::setConfigurationRegister1(uint8_t SJW, uint8_t BRP)
{
  if (_operationMode != MCP2515_OP_CONFIGURATION)
  {
    this->_lastMcpError = ERROR_MCP2515_FALSE_OPERATION_MODE;
    return false;
  }

  if ((SJW > 3) ||
      (BRP > 63))
  {
    this->_lastMcpError = ERROR_MCP2515_VALUE_OUTA_RANGE;
    return false;
  }

  uint8_t Data = SJW << 6 | BRP;

  writeInstruction(REG_CNF1, Data);

  if (_reCheckEnabled)
  {
    if (getConfigurationRegister1() != Data)
    {
      this->_lastMcpError = ERROR_MCP2515_VALUE_NOT_SET;
      return false;
    }
  }

  return true;
}

/**
 * @brief Modify the Configuration 1 Register (REG_CNF1) [2Ah]
 * @param Mask Masked Bits to modify
 *
 * Bit 7 - 6 -> SJW - Synchronization Jump Width Length bits
 *
 * 11 = Length = 4 x TQ
 *
 * 10 = Length = 3 x TQ
 *
 * 01 = Length = 2 x TQ
 *
 * 00 = Length = 1 x TQ
 *
 * Bit 5 - 0 -> BRP - Baud Rate Prescaler bits
 *
 * TQ = 2 x (BRP + 1)/FOSC.
 * @param Value Bitwise 0 or 1
 * @return Result of setting
 *
 * True = Setting successfull
 *
 * False = Error occured during Setting (Check _lastMcpError)
 * @note CNF1 register only modifiable in Configuration mode.
 */
bool MCP2515::modifyConfigurationRegister1(uint8_t Mask, uint8_t Value)
{
  if (_operationMode != MCP2515_OP_CONFIGURATION)
  {
    this->_lastMcpError = ERROR_MCP2515_FALSE_OPERATION_MODE;
    return false;
  }

  if (!bitModifyInstruction(REG_CNF1, Mask, Value))
  {
    this->_lastMcpError = _lastMcpError | ERROR_MCP2515_BITMODIFY_INSTRUCTION;
    return false;
  }

  if (_reCheckEnabled)
  {
    if ((getConfigurationRegister1() & Mask) != (Value & Mask))
    {
      this->_lastMcpError = ERROR_MCP2515_VALUE_NOT_SET;
      return false;
    }
  }

  return true;
}

/**
 * @brief Get the CAN Interrupt Enable Register (REG_CANINTE) [2Bh]
 * @return
 * Bit 7 -> MERRE - Message Error Interrupt Enable bit
 *
 * 1 = Interrupt on error during message reception or transmission
 *
 * 0 = Disabled
 *
 * Bit 6 -> WAKIE - Wake-up Interrupt Enable bit
 *
 * 1 = Interrupt on CAN bus activity
 *
 * 0 = Disabled
 *
 * Bit 5 -> ERRIE - Error Interrupt Enable bit (multiple sources in EFLG register)
 *
 * 1 = Interrupt on EFLG error condition change
 *
 * 0 = Disabled
 *
 * Bit 4 -> TX2IE - Transmit Buffer 2 Empty Interrupt Enable bit
 *
 * 1 = Interrupt on TXB2 becoming empty
 *
 * 0 = Disabled
 *
 * Bit 3 -> TX1IE - Transmit Buffer 1 Empty Interrupt Enable bit
 *
 * 1 = Interrupt on TXB1 becoming empty
 *
 * 0 = Disabled
 *
 * Bit 2 -> TX0IE - Transmit Buffer 0 Empty Interrupt Enable bit
 *
 * 1 = Interrupt on TXB0 becoming empty
 *
 * 0 = Disabled
 *
 * Bit 1 -> RX1IE - Receive Buffer 1 Full Interrupt Enable bit
 *
 * 1 = Interrupt when message was received in RXB1
 *
 * 0 = Disabled
 *
 * Bit 0 -> RX0IE - Receive Buffer 0 Full Interrupt Enable bit
 *
 * 1 = Interrupt when message was received in RXB0
 *
 * 0 = Disabled
 *
 * No Error will be set.
 */
uint8_t MCP2515::getCanInterruptEnable()
{
  return readInstruction(REG_CANINTE);
}

/**
 * @brief Set the CAN Interrupt Enable Register (REG_CANINTE) [2Bh]
 * @param MERRE - Message Error Interrupt Enable bit
 *
 * 1 = Interrupt on error during message reception or transmission
 *
 * 0 = Disabled
 * @param WAKIE - Wake-up Interrupt Enable bit
 *
 * 1 = Interrupt on CAN bus activity
 *
 * 0 = Disabled
 * @param ERRIE - Error Interrupt Enable bit (multiple sources in EFLG register)
 *
 * 1 = Interrupt on EFLG error condition change
 *
 * 0 = Disabled
 * @param TX2IE - Transmit Buffer 2 Empty Interrupt Enable bit
 *
 * 1 = Interrupt on TXB2 becoming empty
 *
 * 0 = Disabled
 * @param TX1IE - Transmit Buffer 1 Empty Interrupt Enable bit
 *
 * 1 = Interrupt on TXB1 becoming empty
 *
 * 0 = Disabled
 * @param TX0IE - Transmit Buffer 0 Empty Interrupt Enable bit
 *
 * 1 = Interrupt on TXB0 becoming empty
 *
 * 0 = Disabled
 * @param RX1IE - Receive Buffer 1 Full Interrupt Enable bit
 *
 * 1 = Interrupt when message was received in RXB1
 *
 * 0 = Disabled
 * @param RX0IE - Receive Buffer 0 Full Interrupt Enable bit
 *
 * 1 = Interrupt when message was received in RXB0
 *
 * 0 = Disabled
 * @return Result of setting
 *
 * True = Setting successfull
 *
 * False = Error occured during Setting (Check _lastMcpError)
 */
bool MCP2515::setCanInterruptEnable(bool MERRE, bool WAKIE, bool ERRIE, bool TX2IE, bool TX1IE, bool TX0IE, bool RX1IE, bool RX0IE)
{
  if ((MERRE != true && MERRE!= false) ||
      (WAKIE != true && WAKIE!= false) ||
      (ERRIE != true && ERRIE!= false) ||
      (TX2IE != true && TX2IE!= false) ||
      (TX1IE != true && TX1IE!= false) ||
      (TX0IE != true && TX0IE!= false) ||
      (RX1IE != true && RX1IE!= false) ||
      (RX0IE != true && RX0IE!= false))
  {
    this->_lastMcpError = ERROR_MCP2515_VALUE_OUTA_RANGE;
    return false;
  }

  uint8_t Data = MERRE << 7 | WAKIE << 6 | ERRIE << 5 | TX2IE << 4 | TX1IE << 3 | TX0IE << 2 | RX1IE << 1 | RX0IE;

  writeInstruction(REG_CANINTE, Data);

  if (_reCheckEnabled)
  {
    if (getCanInterruptEnable() != Data)
    {
      this->_lastMcpError = ERROR_MCP2515_VALUE_NOT_SET;
      return false;
    }
  }

  return true;
}

/**
 * @brief Modify the CAN Interrupt Enable Register (REG_CANINTE) [2Bh]
 * @param Mask Masked Bits to modify
 *
 * Bit 7 -> MERRE - Message Error Interrupt Enable bit
 *
 * 1 = Interrupt on error during message reception or transmission
 *
 * 0 = Disabled
 *
 * Bit 6 -> WAKIE - Wake-up Interrupt Enable bit
 *
 * 1 = Interrupt on CAN bus activity
 *
 * 0 = Disabled
 *
 * Bit 5 -> ERRIE - Error Interrupt Enable bit (multiple sources in EFLG register)
 *
 * 1 = Interrupt on EFLG error condition change
 *
 * 0 = Disabled
 *
 * Bit 4 -> TX2IE - Transmit Buffer 2 Empty Interrupt Enable bit
 *
 * 1 = Interrupt on TXB2 becoming empty
 *
 * 0 = Disabled
 *
 * Bit 3 -> TX1IE - Transmit Buffer 1 Empty Interrupt Enable bit
 *
 * 1 = Interrupt on TXB1 becoming empty
 *
 * 0 = Disabled
 *
 * Bit 2 -> TX0IE - Transmit Buffer 0 Empty Interrupt Enable bit
 *
 * 1 = Interrupt on TXB0 becoming empty
 *
 * 0 = Disabled
 *
 * Bit 1 -> RX1IE - Receive Buffer 1 Full Interrupt Enable bit
 *
 * 1 = Interrupt when message was received in RXB1
 *
 * 0 = Disabled
 *
 * Bit 0 -> RX0IE - Receive Buffer 0 Full Interrupt Enable bit
 *
 * 1 = Interrupt when message was received in RXB0
 *
 * 0 = Disabled
 * @param Value Bitwise 0 or 1
 * @return Result of setting
 *
 * True = Setting successfull
 *
 * False = Error occured during Setting (Check _lastMcpError)
 */
bool MCP2515::modifyCanInterruptEnable(uint8_t Mask, uint8_t Value)
{
  if (!bitModifyInstruction(REG_CANINTE, Mask, Value))
  {
    this->_lastMcpError = _lastMcpError | ERROR_MCP2515_BITMODIFY_INSTRUCTION;
    return false;
  }

  if (_reCheckEnabled)
  {
    if ((getCanInterruptEnable() & Mask) != (Value & Mask))
    {
      this->_lastMcpError = ERROR_MCP2515_VALUE_NOT_SET;
      return false;
    }
  }

  return true;
}

/**
 * @brief Get the CAN Interrupt Flag Register (REG_CANINTF) [2Ch]
 * @return
 * Bit 7 -> MERRF - Message Error Interrupt Flag bit
 *
 * 1 = Interrupt is pending (must be cleared by MCU to reset the interrupt condition)
 *
 * 0 = No interrupt is pending
 *
 * Bit 6 -> WAKIF - Wake-up Interrupt Flag bit
 *
 * 1 = Interrupt is pending (must be cleared by MCU to reset the interrupt condition)
 *
 * 0 = No interrupt is pending
 *
 * Bit 5 -> ERRIF - Error Interrupt Flag bit (multiple sources in EFLG register)
 *
 * 1 = Interrupt is pending (must be cleared by MCU to reset the interrupt condition)
 *
 * 0 = No interrupt is pending
 *
 * Bit 4 -> TX2IF - Transmit Buffer 2 Empty Interrupt Flag bit
 *
 * 1 = Interrupt is pending (must be cleared by MCU to reset the interrupt condition)
 *
 * 0 = No interrupt is pending
 *
 * Bit 3 -> TX1IF - Transmit Buffer 1 Empty Interrupt Flag bit
 *
 * 1 = Interrupt is pending (must be cleared by MCU to reset the interrupt condition)
 *
 * 0 = No interrupt is pending
 *
 * Bit 2 -> TX0IF - Transmit Buffer 0 Empty Interrupt Flag bit
 *
 * 1 = Interrupt is pending (must be cleared by MCU to reset the interrupt condition)
 *
 * 0 = No interrupt is pending
 *
 * Bit 1 -> RX1IF - Receive Buffer 1 Full Interrupt Flag bit
 *
 * 1 = Interrupt is pending (must be cleared by MCU to reset the interrupt condition)
 *
 * 0 = No interrupt is pending
 *
 * Bit 0 -> RX0IF - Receive Buffer 0 Full Interrupt Flag bit
 *
 * 1 = Interrupt is pending (must be cleared by MCU to reset the interrupt condition)
 *
 * 0 = No interrupt is pending
 *
 * No Error will be set.
 */
uint8_t MCP2515::getCanInterruptFlag()
{
  return readInstruction(REG_CANINTF);
}

/**
 * @brief Set the CAN Interrupt Flag Register (REG_CANINTF) [2Ch]
 * @param MERRF Message Error Interrupt Flag bit
 *
 * 1 = Interrupt is pending (must be cleared by MCU to reset the interrupt condition)
 *
 * 0 = No interrupt is pending
 *
 * @param WAKIF Wake-up Interrupt Flag bit
 *
 * 1 = Interrupt is pending (must be cleared by MCU to reset the interrupt condition)
 *
 * 0 = No interrupt is pending
 *
 * @param ERRIF Error Interrupt Flag bit (multiple sources in EFLG register)
 *
 * 1 = Interrupt is pending (must be cleared by MCU to reset the interrupt condition)
 *
 * 0 = No interrupt is pending
 *
 * @param TX2IF Transmit Buffer 2 Empty Interrupt Flag bit
 *
 * 1 = Interrupt is pending (must be cleared by MCU to reset the interrupt condition)
 *
 * 0 = No interrupt is pending
 *
 * @param TX1IF Transmit Buffer 1 Empty Interrupt Flag bit
 *
 * 1 = Interrupt is pending (must be cleared by MCU to reset the interrupt condition)
 *
 * 0 = No interrupt is pending
 *
 * @param TX0IF Transmit Buffer 0 Empty Interrupt Flag bit
 *
 * 1 = Interrupt is pending (must be cleared by MCU to reset the interrupt condition)
 *
 * 0 = No interrupt is pending
 *
 * @param RX1IF Receive Buffer 1 Full Interrupt Flag bit
 *
 * 1 = Interrupt is pending (must be cleared by MCU to reset the interrupt condition)
 *
 * 0 = No interrupt is pending
 *
 * @param RX0IF Receive Buffer 0 Full Interrupt Flag bit
 *
 * 1 = Interrupt is pending (must be cleared by MCU to reset the interrupt condition)
 *
 * 0 = No interrupt is pending
 * @return Result of setting
 *
 * True = Setting successfull
 *
 * False = Error occured during Setting (Check _lastMcpError)
 */
bool MCP2515::setCanInterruptFlag(bool MERRF, bool WAKIF, bool ERRIF, bool TX2IF, bool TX1IF, bool TX0IF, bool RX1IF, bool RX0IF)
{
  if ((MERRF != true && MERRF!= false) ||
      (WAKIF != true && WAKIF!= false) ||
      (ERRIF != true && ERRIF!= false) ||
      (TX2IF != true && TX2IF!= false) ||
      (TX1IF != true && TX1IF!= false) ||
      (TX0IF != true && TX0IF!= false) ||
      (RX1IF != true && RX1IF!= false) ||
      (RX0IF != true && RX0IF!= false))
  {
    this->_lastMcpError = ERROR_MCP2515_VALUE_OUTA_RANGE;
    return false;
  }

  uint8_t Data = MERRF << 7 | WAKIF << 6 | ERRIF << 5 | TX2IF << 4 | TX1IF << 3 | TX0IF << 2 | RX1IF << 1 | RX0IF;

  writeInstruction(REG_CANINTF, Data);

  if (_reCheckEnabled)
  {
    if (getCanInterruptFlag() != Data)
    {
      this->_lastMcpError = ERROR_MCP2515_VALUE_NOT_SET;
      return false;
    }
  }

  return true;
}

/**
 * @brief Modify the CAN Interrupt Flag Register (REG_CANINTF) [2Ch]
 * @param Mask Masked Bits to modify
 *
 * Bit 7 -> MERRF - Message Error Interrupt Flag bit
 *
 * 1 = Interrupt is pending (must be cleared by MCU to reset the interrupt condition)
 *
 * 0 = No interrupt is pending
 *
 * Bit 6 -> WAKIF - Wake-up Interrupt Flag bit
 *
 * 1 = Interrupt is pending (must be cleared by MCU to reset the interrupt condition)
 *
 * 0 = No interrupt is pending
 *
 * Bit 5 -> ERRIF - Error Interrupt Flag bit (multiple sources in EFLG register)
 *
 * 1 = Interrupt is pending (must be cleared by MCU to reset the interrupt condition)
 *
 * 0 = No interrupt is pending
 *
 * Bit 4 -> TX2IF - Transmit Buffer 2 Empty Interrupt Flag bit
 *
 * 1 = Interrupt is pending (must be cleared by MCU to reset the interrupt condition)
 *
 * 0 = No interrupt is pending
 *
 * Bit 3 -> TX1IF - Transmit Buffer 1 Empty Interrupt Flag bit
 *
 * 1 = Interrupt is pending (must be cleared by MCU to reset the interrupt condition)
 *
 * 0 = No interrupt is pending
 *
 * Bit 2 -> TX0IF - Transmit Buffer 0 Empty Interrupt Flag bit
 *
 * 1 = Interrupt is pending (must be cleared by MCU to reset the interrupt condition)
 *
 * 0 = No interrupt is pending
 *
 * Bit 1 -> RX1IF - Receive Buffer 1 Full Interrupt Flag bit
 *
 * 1 = Interrupt is pending (must be cleared by MCU to reset the interrupt condition)
 *
 * 0 = No interrupt is pending
 *
 * Bit 0 -> RX0IF - Receive Buffer 0 Full Interrupt Flag bit
 *
 * 1 = Interrupt is pending (must be cleared by MCU to reset the interrupt condition)
 *
 * 0 = No interrupt is pending
 * @param Value Bitwise 0 or 1
 * @return Result of setting
 *
 * True = Setting successfull
 *
 * False = Error occured during Setting (Check _lastMcpError)
 */
bool MCP2515::modifyCanInterruptFlag(uint8_t Mask, uint8_t Value)
{
  if (!bitModifyInstruction(REG_CANINTF, Mask, Value))
  {
    this->_lastMcpError = _lastMcpError | ERROR_MCP2515_BITMODIFY_INSTRUCTION;
    return false;
  }

  if (_reCheckEnabled)
  {
    if ((getCanInterruptFlag() & Mask) != (Value & Mask))
    {
      this->_lastMcpError = ERROR_MCP2515_VALUE_NOT_SET;
      return false;
    }
  }

  return true;
}

/**
 * @brief Get the Error Flag Register (REG_EFLG) [2Dh]
 * @return
 * Bit 7 -> RX1OVR - Receive Buffer 1 Overflow Flag bit
 *
 * Sets when a valid message is received for RXB1 and RX1IF (CANINTF[1]) = 1
 *
 * Must be reset by MCU
 *
 * Bit 6 -> RX0OVR - Receive Buffer 0 Overflow Flag bit
 *
 * Sets when a valid message is received for RXB0 and RX0IF (CANINTF[0]) = 1
 *
 * Must be reset by MCU
 *
 * Bit 5 -> TXBO - Bus-Off Error Flag bit
 *
 * Sets when TEC reaches 255
 *
 * Resets after a successful bus recovery sequence
 *
 * Bit 4 -> TXEP - Transmit Error-Passive Flag bit
 *
 * Sets when TEC is equal to or greater than 128
 *
 * Resets when TEC is less than 128
 *
 * Bit 3 -> RXEP - Receive Error-Passive Flag bit
 *
 * Sets when REC is equal to or greater than 128
 *
 * Resets when REC is less than 128
 *
 * Bit 2 -> TXWAR - Transmit Error Warning Flag bit
 *
 * Sets when TEC is equal to or greater than 96
 *
 * Resets when TEC is less than 96
 *
 * Bit 1 -> RXWAR - Receive Error Warning Flag bit
 *
 * Sets when REC is equal to or greater than 96
 *
 * Resets when REC is less than 96
 *
 * Bit 0 -> EWARN - Error Warning Flag bit
 *
 * Sets when TEC or REC is equal to or greater than 96 (TXWAR or RXWAR = 1)
 *
 * Resets when both REC and TEC are less than 96
 *
 * No Error will be set.
 */
uint8_t MCP2515::getErrorFlag()
{
  return readInstruction(REG_EFLG);
}

/**
 * @brief Set the Error Flag Register (REG_EFLG) [2Dh]
 * @param RX1OVR Receive Buffer 1 Overflow Flag bit
 *
 * Sets when a valid message is received for RXB1 and RX1IF (CANINTF[1]) = 1
 *
 * Must be reset by MCU
 *
 * @param RX0OVR Receive Buffer 0 Overflow Flag bit
 *
 * Sets when a valid message is received for RXB0 and RX0IF (CANINTF[0]) = 1
 *
 * Must be reset by MCU
 * @return Result of setting
 *
 * True = Setting successfull
 *
 * False = Error occured during Setting (Check _lastMcpError)
 */
bool MCP2515::setErrorFlag(bool RX1OVR, bool RX0OVR)
{
  if ((RX1OVR != true && RX1OVR!= false) ||
      (RX0OVR != true && RX0OVR!= false))
  {
    this->_lastMcpError = ERROR_MCP2515_VALUE_OUTA_RANGE;
    return false;
  }

  uint8_t Data = RX1OVR << 7 | RX0OVR << 6;

  writeInstruction(REG_EFLG, Data);

  if (_reCheckEnabled)
  {
    if ((getErrorFlag() & (EFLG_BIT_RXnOVR(0) | EFLG_BIT_RXnOVR(1))) != Data)
    {
      this->_lastMcpError = ERROR_MCP2515_VALUE_NOT_SET;
      return false;
    }
  }

  return true;
}

/**
 * @brief Modify the Error Flag Register (REG_EFLG) [2Dh]
 * @param Mask Masked Bits to modify
 *
 * Bit 7 -> RX1OVR - Receive Buffer 1 Overflow Flag bit
 *
 * Sets when a valid message is received for RXB1 and RX1IF (CANINTF[1]) = 1
 *
 * Must be reset by MCU
 *
 * Bit 6 -> RX0OVR - Receive Buffer 0 Overflow Flag bit
 *
 * Sets when a valid message is received for RXB0 and RX0IF (CANINTF[0]) = 1
 *
 * Must be reset by MCU
 * @param Value Bitwise 0 or 1
 * @return Result of setting
 *
 * True = Setting successfull
 *
 * False = Error occured during Setting (Check _lastMcpError)
 */
bool MCP2515::modifyErrorFlag(uint8_t Mask, uint8_t Value)
{
  if ((Mask & ~(EFLG_BIT_RXnOVR(0) | EFLG_BIT_RXnOVR(1))) != 0x00)
  {
    this->_lastMcpError = ERROR_MCP2515_MASK_NOT_VALID;
    return false;
  }

  if (!bitModifyInstruction(REG_EFLG, Mask, Value))
  {
    this->_lastMcpError = _lastMcpError | ERROR_MCP2515_BITMODIFY_INSTRUCTION;
    return false;
  }

  if (_reCheckEnabled)
  {
    if ((getErrorFlag() & Mask) != (Value & Mask))
    {
      this->_lastMcpError = ERROR_MCP2515_VALUE_NOT_SET;
      return false;
    }
  }

  return true;
}

/**
 * @brief Get the Transmit Buffer Control Register (REG_TXBnCTRL) [30h, 40h, 50h]
 * @param BufferNumber 0 - 2
 * @return
 * Bit 6 -> ABTF - Message Aborted Flag bit
 *
 * 1 = Message was aborted
 *
 * 0 = Message completed transmission successfully
 *
 * Bit 5 -> MLOA - Message Lost Arbitration bit
 *
 * 1 = Message lost arbitration while being sent
 *
 * 0 = Message did not lose arbitration while being sent
 *
 * Bit 4 -> TXERR - Transmission Error Detected bit
 *
 * 1 = A bus error occurred while the message was being transmitted
 *
 * 0 = No bus error occurred while the message was being transmitted
 *
 * Bit 3 -> TXREQ - Message Transmit Request bit
 *
 * 1 = Buffer is currently pending transmission (MCU sets this bit to request message be transmitted – bit is automatically cleared when the message is sent)
 *
 * 0 = Buffer is not currently pending transmission (MCU can clear this bit to request a message abort)
 *
 * Bit 1 - 0 -> TXP - Transmit Buffer Priority bits
 *
 * 11 = Highest message priority
 *
 * 10 = High intermediate message priority
 *
 * 01 = Low intermediate message priority
 *
 * 00 = Lowest message priority
 *
 * On Error it will return EMPTY_VALUE_8_BIT (Check _lastMcpError).
 */
uint8_t MCP2515::getTransmitBufferControl(uint8_t BufferNumber)
{
  if (BufferNumber > 2)
  {
    this->_lastMcpError = ERROR_MCP2515_VALUE_OUTA_RANGE;
    return EMPTY_VALUE_8_BIT;
  }

  return readInstruction(REG_TXBnCTRL(BufferNumber));
}

/**
 * @brief Set the Transmit Buffer Control Register (REG_TXBnCTRL) [30h, 40h, 50h]
 * @param BufferNumber 0 - 2
 * @param TXREQ Message Transmit Request bit
 *
 * 1 = Buffer is currently pending transmission (MCU sets this bit to request message be transmitted – bit is automatically cleared when the message is sent)
 *
 * 0 = Buffer is not currently pending transmission (MCU can clear this bit to request a message abort)
 * @param TXP Transmit Buffer Priority bits
 *
 * 11 = Highest message priority
 *
 * 10 = High intermediate message priority
 *
 * 01 = Low intermediate message priority
 *
 * 00 = Lowest message priority
 * @return Result of setting
 *
 * True = Setting successfull
 *
 * False = Error occured during Setting (Check _lastMcpError)
 */
bool MCP2515::setTransmitBufferControl(uint8_t BufferNumber, bool TXREQ, uint8_t TXP)
{
  if ((BufferNumber > 2) ||
      (TXREQ != true && TXREQ!= false) ||
      (TXP > 3))
  {
    this->_lastMcpError = ERROR_MCP2515_VALUE_OUTA_RANGE;
    return false;
  }

  uint8_t Data = TXREQ << 3 | TXP;

  writeInstruction(REG_TXBnCTRL(BufferNumber), Data);

  if (_reCheckEnabled)
  {
    if ((getTransmitBufferControl(BufferNumber) & (TXBnCTRL_BIT_TXREQ | TXBnCTRL_BIT_TXP)) != Data)
    {
      this->_lastMcpError = ERROR_MCP2515_VALUE_NOT_SET;
      return false;
    }
  }

  return true;
}

/**
 * @brief Modify the Transmit Buffer Control Register (REG_TXBnCTRL) [30h, 40h, 50h]
 * @param Mask Masked Bits to modify
 *
 * Bit 3 -> TXREQ - Message Transmit Request bit
 *
 * 1 = Buffer is currently pending transmission (MCU sets this bit to request message be transmitted – bit is automatically cleared when the message is sent)
 *
 * 0 = Buffer is not currently pending transmission (MCU can clear this bit to request a message abort)
 *
 * Bit 1 - 0 -> TXP - Transmit Buffer Priority bits
 *
 * 11 = Highest message priority
 *
 * 10 = High intermediate message priority
 *
 * 01 = Low intermediate message priority
 *
 * 00 = Lowest message priority
 * @param Value Bitwise 0 or 1
 * @return Result of setting
 *
 * True = Setting successfull
 *
 * False = Error occured during Setting (Check _lastMcpError)
 */
bool MCP2515::modifyTransmitBufferControl(uint8_t BufferNumber, uint8_t Mask, uint8_t Value)
{
  if (BufferNumber > 2)
  {
    this->_lastMcpError = ERROR_MCP2515_VALUE_OUTA_RANGE;
    return false;
  }

  if ((Mask & ~(TXBnCTRL_BIT_TXREQ | TXBnCTRL_BIT_TXP)) != 0x00)
  {
    this->_lastMcpError = ERROR_MCP2515_MASK_NOT_VALID;
    return false;
  }

  if (!bitModifyInstruction(REG_TXBnCTRL(BufferNumber), Mask, Value))
  {
    this->_lastMcpError = _lastMcpError | ERROR_MCP2515_BITMODIFY_INSTRUCTION;
    return false;
  }

  if (_reCheckEnabled)
  {
    if ((getTransmitBufferControl(BufferNumber) & Mask) != (Value & Mask))
    {
      this->_lastMcpError = ERROR_MCP2515_VALUE_NOT_SET;
      return false;
    }
  }

  return true;
}

/**
 * @brief Get the Transmit Buffer Standard Identifier High Register (REG_TXBnSIDH) [31h, 41h, 51h]
 * @param BufferNumber 0 - 2
 * @return uint8_t Standard Identifier [10:3]
 *
 * On Error it will return EMPTY_VALUE_8_BIT (Check _lastMcpError).
 * @note Standard Identifier [2:0] can be get from getTransmitBufferStandardIdentifierLow().
 */
uint8_t MCP2515::getTransmitBufferStandardIdentifierHigh(uint8_t BufferNumber)
{
  if (BufferNumber > 2)
  {
    this->_lastMcpError = ERROR_MCP2515_VALUE_OUTA_RANGE;
    return EMPTY_VALUE_8_BIT;
  }

  return readInstruction(REG_TXBnSIDH(BufferNumber));
}

/**
 * @brief Set the Transmit Buffer Standard Identifier High Register (REG_TXBnSIDH) [31h, 41h, 51h]
 * @param BufferNumber 0 - 2
 * @param StandardId_10_3 uint8_t Bit 10-3 of the Standard Identifier
 * @return Result of setting
 *
 * True = Setting successfull
 *
 * False = Error occured during Setting (Check _lastMcpError)
 * @note Standard Identifier [2:0] can be set in setTransmitBufferStandardIdentifierLow().
 */
bool MCP2515::setTransmitBufferStandardIdentifierHigh(uint8_t BufferNumber, uint8_t StandardId_10_3)
{
  if ((BufferNumber > 2) ||
      (StandardId_10_3 > 255))
  {
    this->_lastMcpError = ERROR_MCP2515_VALUE_OUTA_RANGE;
    return false;
  }

  writeInstruction(REG_TXBnSIDH(BufferNumber), StandardId_10_3);

  if (_reCheckEnabled)
  {
    if (getTransmitBufferStandardIdentifierHigh(BufferNumber) != StandardId_10_3)
    {
      this->_lastMcpError = ERROR_MCP2515_VALUE_NOT_SET;
      return false;
    }
  }

  return true;
}

/**
 * @brief Get the Transmit Buffer Standard Identifier Low Register (REG_TXBnSIDL) [32h, 42h, 52h]
 * @param BufferNumber 0 - 2
 * @return
 * Bit 7 - 5 -> SID - Standard Identifier [2:0]
 *
 * Bit 3 -> EXIDE - Extended Identifier Enable bit
 *
 * 1 = Message will transmit Extended Identifier
 *
 * 0 = Message will transmit Standard Identifier
 *
 * Bit 1 - 0 -> EID - Extended Identifier bits [17:16]
 *
 * On Error it will return EMPTY_VALUE_8_BIT (Check _lastMcpError).
 * @note Standard Identifier [10:3] can be get from getTransmitBufferStandardIdentifierHigh().
 *
 * Extended Identifier [15:8] and [7:0] can be get from getTransmitBufferExtendedIdentifierHigh() and getTransmitBufferExtendedIdentifierLow().
 */
uint8_t MCP2515::getTransmitBufferStandardIdentifierLow(uint8_t BufferNumber)
{
  if (BufferNumber > 2)
  {
    this->_lastMcpError = ERROR_MCP2515_VALUE_OUTA_RANGE;
    return EMPTY_VALUE_8_BIT;
  }

  return readInstruction(REG_TXBnSIDL(BufferNumber));
}

/**
 * @brief Set the Transmit Buffer Standard Identifier Low Register (REG_TXBnSIDL) [32h, 42h, 52h]
 * @param BufferNumber 0 - 2
 * @param StandardId_2_0 uint8_t Bit 2-0 of the Standard Identifier
 * @param ExtendedIdEn bool Extended Identifier Enable bit
 *
 * 1 = Message will transmit Extended Identifier
 *
 * 0 = Message will transmit Standard Identifier
 * @param ExtendedId_17_16 uint8_t Bit 17-16 of the Extended Identifier
 * @return Result of setting
 *
 * True = Setting successfull
 *
 * False = Error occured during Setting (Check _lastMcpError)
 * @note Standard Identifier [10:3] can be set in setTransmitBufferStandardIdentifierHigh().
 *
 * Extended Identifier [15:8] and [7:0] can be set in setTransmitBufferExtendedIdentifierHigh() and setTransmitBufferExtendedIdentifierLow().
 */
bool MCP2515::setTransmitBufferStandardIdentifierLow(uint8_t BufferNumber, uint8_t StandardId_2_0, bool ExtendedIdEn, uint8_t ExtendedId_17_16)
{
  if ((BufferNumber > 2) ||
      (StandardId_2_0 > 7) ||
      (ExtendedIdEn != true && ExtendedIdEn != false) ||
      (ExtendedId_17_16 > 3))
  {
    this->_lastMcpError = ERROR_MCP2515_VALUE_OUTA_RANGE;
    return false;
  }

  uint8_t Data = StandardId_2_0 << 5 | ExtendedIdEn << 3 | ExtendedId_17_16;

  writeInstruction(REG_TXBnSIDL(BufferNumber), Data);

  if (_reCheckEnabled)
  {
    if (getTransmitBufferStandardIdentifierLow(BufferNumber) != Data)
    {
      this->_lastMcpError = ERROR_MCP2515_VALUE_NOT_SET;
      return false;
    }
  }

  return true;
}

/**
 * @brief Get the Transmit Buffer Extended Identifier High Register (REG_TXBnEID8) [33h, 43h, 53h]
 * @param BufferNumber 0 - 2
 * @return uint8_t Extended Identifier [15:8]
 *
 * On Error it will return EMPTY_VALUE_8_BIT (Check _lastMcpError).
 * @note Extended Identifier [17:16] and [7:0] can be get from getTransmitBufferStandardIdentifierLow() and getTransmitBufferExtendedIdentifierLow().
 */
uint8_t MCP2515::getTransmitBufferExtendedIdentifierHigh(uint8_t BufferNumber)
{
  if (BufferNumber > 2)
  {
    this->_lastMcpError = ERROR_MCP2515_VALUE_OUTA_RANGE;
    return EMPTY_VALUE_8_BIT;
  }

  return readInstruction(REG_TXBnEID8(BufferNumber));
}

/**
 * @brief Set the Transmit Buffer Extended Identifier High Register (REG_TXBnEID8) [33h, 43h, 53h]
 * @param BufferNumber 0 - 2
 * @param Extended_15_8 uint8_t Bit 15-8 of the Extended Identifier
 * @return Result of setting
 *
 * True = Setting successfull
 *
 * False = Error occured during Setting (Check _lastMcpError)
 * @note Extended Identifier [17:16] and [7:0] can be set in setTransmitBufferStandardIdentifierLow() and setTransmitBufferExtendedIdentifierLow().
 */
bool MCP2515::setTransmitBufferExtendedIdentifierHigh(uint8_t BufferNumber, uint8_t ExtendedId_15_8)
{
  if ((BufferNumber > 2) ||
      (ExtendedId_15_8 > 255))
  {
    this->_lastMcpError = ERROR_MCP2515_VALUE_OUTA_RANGE;
    return false;
  }

  writeInstruction(REG_TXBnEID8(BufferNumber), ExtendedId_15_8);

  if (_reCheckEnabled)
  {
    if (getTransmitBufferExtendedIdentifierHigh(BufferNumber) != ExtendedId_15_8)
    {
      this->_lastMcpError = ERROR_MCP2515_VALUE_NOT_SET;
      return false;
    }
  }

  return true;
}

/**
 * @brief Get the Transmit Buffer Extended Identifier Low Register (REG_TXBnEID0) [34h, 44h, 54h]
 * @param BufferNumber 0 - 2
 * @return uint8_t Extended Identifier [7:0]
 *
 * On Error it will return EMPTY_VALUE_8_BIT (Check _lastMcpError).
 * @note Extended Identifier [17:16] and [15:8] can be get from getTransmitBufferStandardIdentifierLow() and getTransmitBufferExtendedIdentifierHigh().
 */
uint8_t MCP2515::getTransmitBufferExtendedIdentifierLow(uint8_t BufferNumber)
{
  if (BufferNumber > 2)
  {
    this->_lastMcpError = ERROR_MCP2515_VALUE_OUTA_RANGE;
    return EMPTY_VALUE_8_BIT;
  }

  return readInstruction(REG_TXBnEID0(BufferNumber));
}

/**
 * @brief Set the Transmit Buffer Extended Identifier Low Register (REG_TXBnEID0) [34h, 44h, 54h]
 * @param BufferNumber 0 - 2
 * @param Extended_7_0 uint8_t Bit 7-0 of the Extended Identifier
 * @return Result of setting
 *
 * True = Setting successfull
 *
 * False = Error occured during Setting (Check _lastMcpError)
 * @note Extended Identifier [17:16] and [15:8] can be set in setTransmitBufferStandardIdentifierLow() and setTransmitBufferExtendedIdentifierHigh().
 */
bool MCP2515::setTransmitBufferExtendedIdentifierLow(uint8_t BufferNumber, uint8_t ExtendedId_7_0)
{
  if ((BufferNumber > 2) ||
      (ExtendedId_7_0 > 255))
  {
    this->_lastMcpError = ERROR_MCP2515_VALUE_OUTA_RANGE;
    return false;
  }

  writeInstruction(REG_TXBnEID0(BufferNumber), ExtendedId_7_0);

  if (_reCheckEnabled)
  {
    if (getTransmitBufferExtendedIdentifierLow(BufferNumber) != ExtendedId_7_0)
    {
      this->_lastMcpError = ERROR_MCP2515_VALUE_NOT_SET;
      return false;
    }
  }

  return true;
}

/**
 * @brief Get the Transmit Buffer Data Length Code Register (REG_TXBnDLC) [35h, 45h, 55h]
 * @param BufferNumber 0 - 2
 * @return
 * Bit 6 -> RTR - Remote Transmission Request bit
 *
 * 1 = Transmitted message will be a remote transmit request
 *
 * 0 = Transmitted message will be a data frame
 *
 * Bit 3 - 0 -> DLC - Data Length Code bits. Sets the number of data bytes to be transmitted (0 to 8 bytes).
 *
 * On Error it will return EMPTY_VALUE_8_BIT (Check _lastMcpError).
 * @note It is possible to set the DLC[3:0] bits to a value greater than eight; however, only eight bytes are transmitted.
 */
uint8_t MCP2515::getTransmitBufferDataLengthCode(uint8_t BufferNumber)
{
  if (BufferNumber > 2)
  {
    this->_lastMcpError = ERROR_MCP2515_VALUE_OUTA_RANGE;
    return EMPTY_VALUE_8_BIT;
  }

  return readInstruction(REG_TXBnDLC(BufferNumber));
}

/**
 * @brief Set the Transmit Buffer Data Length Code Register (REG_TXBnDLC) [35h, 45h, 55h]
 * @param BufferNumber 0 - 2
 * @param RTR Remote Transmission Request bit
 *
 * 1 = Transmitted message will be a remote transmit request
 *
 * 0 = Transmitted message will be a data frame
 * @param DLC Data Length Code bits. Sets the number of data bytes to be transmitted (0 to 8 bytes).
 * @return Result of setting
 *
 * True = Setting successfull
 *
 * False = Error occured during Setting (Check _lastMcpError)
 * @note It is possible to set the DLC[3:0] bits to a value greater than eight; however, only eight bytes are transmitted.
 */
bool MCP2515::setTransmitBufferDataLengthCode(uint8_t BufferNumber, bool RTR, uint8_t DLC)
{
  if ((RTR != true && RTR != false) ||
      (DLC > 8))
  {
    this->_lastMcpError = ERROR_MCP2515_VALUE_OUTA_RANGE;
    return false;
  }

  uint8_t Data = RTR << 6 | DLC;
  writeInstruction(REG_TXBnDLC(BufferNumber), Data);

  if (_reCheckEnabled)
  {
    if (getTransmitBufferDataLengthCode(BufferNumber) != Data)
    {
      this->_lastMcpError = ERROR_MCP2515_VALUE_NOT_SET;
      return false;
    }
  }

  return true;
}

/**
 * @brief Get the Transmit Buffer Data Byte Register (REG_TXBnDm) [36h-3Dh, 46h-4Dh, 56h-5Dh]
 * @param BufferNumber 0 - 2
 * @param ByteNumber 0 -7
 * @return uint8_t Data
 *
 * On Error it will return EMPTY_VALUE_8_BIT (Check _lastMcpError).
 */
uint8_t MCP2515::getTransmitBufferDataByte(uint8_t BufferNumber, uint8_t ByteNumber)
{
  if (BufferNumber > 2 || ByteNumber > 7)
  {
    this->_lastMcpError = ERROR_MCP2515_VALUE_OUTA_RANGE;
    return EMPTY_VALUE_8_BIT;
  }

  switch (ByteNumber)
  {
  case 0:
    return readInstruction(REG_TXBnD0(BufferNumber));
    break;
  case 1:
    return readInstruction(REG_TXBnD1(BufferNumber));
    break;
  case 2:
    return readInstruction(REG_TXBnD2(BufferNumber));
    break;
  case 3:
    return readInstruction(REG_TXBnD3(BufferNumber));
    break;
  case 4:
    return readInstruction(REG_TXBnD4(BufferNumber));
    break;
  case 5:
    return readInstruction(REG_TXBnD5(BufferNumber));
    break;
  case 6:
    return readInstruction(REG_TXBnD6(BufferNumber));
    break;
  case 7:
    return readInstruction(REG_TXBnD7(BufferNumber));
    break;
  default:
    this->_lastMcpError = ERROR_MCP2515_UNKNOWN_SWITCH;
    return EMPTY_VALUE_8_BIT;
    break;
  }
}

/**
 * @brief Set the Transmit Buffer Data Byte Register (REG_TXBnDm) [36h-3Dh, 46h-4Dh, 56h-5Dh]
 * @param BufferNumber 0 - 2
 * @param ByteNumber 0 -7
 * @param Data uint8_t Data
 * @return Result of setting
 *
 * True = Setting successfull
 *
 * False = Error occured during Setting (Check _lastMcpError)
 */
bool MCP2515::setTransmitBufferDataByte(uint8_t BufferNumber, uint8_t ByteNumber, uint8_t Data)
{
  if (BufferNumber > 2 || ByteNumber > 7)
  {
    this->_lastMcpError = ERROR_MCP2515_VALUE_OUTA_RANGE;
    return EMPTY_VALUE_8_BIT;
  }

  switch (ByteNumber)
  {
  case 0:
    writeInstruction(REG_TXBnD0(BufferNumber), Data);
    break;
  case 1:
    writeInstruction(REG_TXBnD1(BufferNumber), Data);
    break;
  case 2:
    writeInstruction(REG_TXBnD2(BufferNumber), Data);
    break;
  case 3:
    writeInstruction(REG_TXBnD3(BufferNumber), Data);
    break;
  case 4:
    writeInstruction(REG_TXBnD4(BufferNumber), Data);
    break;
  case 5:
    writeInstruction(REG_TXBnD5(BufferNumber), Data);
    break;
  case 6:
    writeInstruction(REG_TXBnD6(BufferNumber), Data);
    break;
  case 7:
    writeInstruction(REG_TXBnD7(BufferNumber), Data);
    break;
  default:
    this->_lastMcpError = ERROR_MCP2515_UNKNOWN_SWITCH;
    return false;
    break;
  }

  if (_reCheckEnabled)
  {
    if (getTransmitBufferDataByte(BufferNumber, ByteNumber) != Data)
    {
      this->_lastMcpError = ERROR_MCP2515_VALUE_NOT_SET;
      return false;
    }
  }

  return true;
}

/**
 * @brief Get the Receiver Buffer 0 Control Register (REG_RXB0CTRL) [60h]
 * @return
 * Bit 6 - 5 -> RXM - Receive Buffer Operating mode bits
 *
 * 11 = Turns mask/filters off; receives any message
 *
 * 10 = Reserved
 *
 * 01 = Reserved
 *
 * 00 = Receives all valid messages using either Standard or Extended Identifiers that meet filter criteria;
 *
 * Extended ID Filter registers, RXFnEID8:RXFnEID0, are applied to the first two bytes of data in the messages with standard IDs
 *
 * Bit 3 -> RXRTR - Received Remote Transfer Request bit
 *
 * 1 = Remote Transfer Request received
 *
 * 0 = No Remote Transfer Request received
 *
 * Bit 2 -> BUKT - Rollover Enable bit
 *
 * 1 = RXB0 message will roll over and be written to RXB1 if RXB0 is full
 *
 * 0 = Rollover is disabled
 *
 * Bit 1 -> BUKT1 - Read-Only Copy of BUKT bit (used internally by the MCP2515)
 *
 * Bit 0 -> FILHIT0 - Filter Hit bit (indicates which acceptance filter enabled reception of message)(1)
 *
 * 1 = Acceptance Filter 1 (RXF1)
 *
 * 0 = Acceptance Filter 0 (RXF0)
 *
 * No Error will be set.
 * @note If a rollover from RXB0 to RXB1 occurs, the FILHIT0 bit will reflect the filter that accepted the message that rolled over.
 */
uint8_t MCP2515::getReceiveBuffer0Control()
{
  return readInstruction(REG_RXBnCTRL(0));
}

/**
 * @brief Set the Receiver Buffer 0 Control Register (REG_RXB0CTRL) [60h]
 * @param RXM Receive Buffer Operating mode bits
 *
 * 11 = Turns mask/filters off; receives any message
 *
 * 10 = Reserved
 *
 * 01 = Reserved
 *
 * 00 = Receives all valid messages using either Standard or Extended Identifiers that meet filter criteria;
 *
 * Extended ID Filter registers, RXFnEID8:RXFnEID0, are applied to the first two bytes of data in the messages with standard IDs
 * @param BUKT Rollover Enable bit
 *
 * 1 = RXB0 message will roll over and be written to RXB1 if RXB0 is full
 *
 * 0 = Rollover is disabled
 * @return Result of setting
 *
 * True = Setting successfull
 *
 * False = Error occured during Setting (Check _lastMcpError)
 * @note If a rollover from RXB0 to RXB1 occurs, the FILHIT0 bit will reflect the filter that accepted the message that rolled over.
 */
bool MCP2515::setReceiveBuffer0Control(uint8_t RXM, bool BUKT)
{
  if ((RXM != 0 && RXM != 3) ||
      (BUKT != true && BUKT != false))
  {
    this->_lastMcpError = ERROR_MCP2515_VALUE_OUTA_RANGE;
    return false;
  }

  uint8_t Data = RXM << 5 | BUKT << 2;

  writeInstruction(REG_RXBnCTRL(0), Data);

  if (_reCheckEnabled)
  {
    if ((getReceiveBuffer0Control() & (RXBnCTRL_BIT_RXM | RXBnCTRL_BIT_BUFFER0_BUKT)) != Data)
    {
      this->_lastMcpError = ERROR_MCP2515_VALUE_NOT_SET;
      return false;
    }
  }

  return true;
}

/**
 * @brief Modify the Receiver Buffer 0 Control Register (REG_RXB0CTRL) [60h]
 * @param Mask Masked Bits to modify
 *
 * Bit 6 - 5 -> RXM - Receive Buffer Operating mode bits
 *
 * 11 = Turns mask/filters off; receives any message
 *
 * 10 = Reserved
 *
 * 01 = Reserved
 *
 * 00 = Receives all valid messages using either Standard or Extended Identifiers that meet filter criteria;
 *
 * Extended ID Filter registers, RXFnEID8:RXFnEID0, are applied to the first two bytes of data in the messages with standard IDs
 *
 * Bit 2 -> BUKT - Rollover Enable bit
 *
 * 1 = RXB0 message will roll over and be written to RXB1 if RXB0 is full
 *
 * 0 = Rollover is disabled
 * @param Value Bitwise 0 or 1
 * @return Result of setting
 *
 * True = Setting successfull
 *
 * False = Error occured during Setting (Check _lastMcpError)
 */
bool MCP2515::modifyReceiveBuffer0Control(uint8_t Mask, uint8_t Value)
{
  if ((Mask & ~(RXBnCTRL_BIT_RXM | RXBnCTRL_BIT_BUFFER0_BUKT)) != 0x00)
  {
    this->_lastMcpError = ERROR_MCP2515_MASK_NOT_VALID;
    return false;
  }

  if (!bitModifyInstruction(REG_RXBnCTRL(0), Mask, Value))
  {
    this->_lastMcpError = _lastMcpError | ERROR_MCP2515_BITMODIFY_INSTRUCTION;
    return false;
  }

  if (_reCheckEnabled)
  {
    if ((getReceiveBuffer0Control() & Mask) != (Value & Mask))
    {
      this->_lastMcpError = ERROR_MCP2515_VALUE_NOT_SET;
      return false;
    }
  }

  return true;
}

/**
 * @brief Get the Receiver Buffer 1 Control Register (REG_RXB1CTRL) [70h]
 * @return
 * Bit 6 - 5 -> RXM - Receive Buffer Operating mode bits
 *
 * 11 = Turns mask/filters off; receives any message
 *
 * 10 = Reserved
 *
 * 01 = Reserved
 *
 * 00 = Receives all valid messages using either Standard or Extended Identifiers that meet filter criteria
 *
 * Bit 3 -> RXRTR - Received Remote Transfer Request bit
 *
 * 1 = Remote Transfer Request received
 *
 * 0 = No Remote Transfer Request received
 *
 * Bit 2 - 0 -> FILHIT - Filter Hit bits (indicates which acceptance filter enabled reception of message)
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
 *
 * No Error will be set.
 */
uint8_t MCP2515::getReceiveBuffer1Control()
{
  return readInstruction(REG_RXBnCTRL(1));
}

/**
 * @brief Set the Receiver Buffer 1 Control Register (REG_RXB1CTRL) [70h]
 * @param RXM Receive Buffer Operating mode bits
 *
 * 11 = Turns mask/filters off; receives any message
 *
 * 10 = Reserved
 *
 * 01 = Reserved
 *
 * 00 = Receives all valid messages using either Standard or Extended Identifiers that meet filter criteria
 * @return Result of setting
 *
 * True = Setting successfull
 *
 * False = Error occured during Setting (Check _lastMcpError)
 * @note If a rollover from RXB0 to RXB1 occurs, the FILHIT0 bit will reflect the filter that accepted the message that rolled over.
 */
bool MCP2515::setReceiveBuffer1Control(uint8_t RXM)
{
  if (RXM != 0 && RXM != 3)
  {
    this->_lastMcpError = ERROR_MCP2515_VALUE_OUTA_RANGE;
    return false;
  }

  uint8_t Data = RXM << 5;

  writeInstruction(REG_RXBnCTRL(1), Data);

  if (_reCheckEnabled)
  {
    if ((getReceiveBuffer1Control() & RXBnCTRL_BIT_RXM) != Data)
    {
      this->_lastMcpError = ERROR_MCP2515_VALUE_NOT_SET;
      return false;
    }
  }

  return true;
}

/**
 * @brief Modify the Receiver Buffer 1 Control Register (REG_RXB1CTRL) [70h]
 * @param Mask Masked Bits to modify
 *
 * Bit 6 - 5 -> RXM - Receive Buffer Operating mode bits
 *
 * 11 = Turns mask/filters off; receives any message
 *
 * 10 = Reserved
 *
 * 01 = Reserved
 *
 * 00 = Receives all valid messages using either Standard or Extended Identifiers that meet filter criteria
 * @param Value Bitwise 0 or 1
 * @return Result of setting
 *
 * True = Setting successfull
 *
 * False = Error occured during Setting (Check _lastMcpError)
 */
bool MCP2515::modifyReceiveBuffer1Control(uint8_t Mask, uint8_t Value)
{
  if ((Mask & ~RXBnCTRL_BIT_RXM) != 0x00)
  {
    this->_lastMcpError = ERROR_MCP2515_MASK_NOT_VALID;
    return false;
  }

  if (!bitModifyInstruction(REG_RXBnCTRL(1), Mask, Value))
  {
    this->_lastMcpError = _lastMcpError | ERROR_MCP2515_BITMODIFY_INSTRUCTION;
    return false;
  }

  if (_reCheckEnabled)
  {
    if ((getReceiveBuffer1Control() & Mask) != (Value & Mask))
    {
      this->_lastMcpError = ERROR_MCP2515_VALUE_NOT_SET;
      return false;
    }
  }

  return true;
}

/**
 * @brief Get the Receive Buffer Standard Identifier High Register (REG_RXBnSIDH) [61h, 71h]
 * @param BufferNumber 0 - 1
 * @return uint8_t Standard Identifier [10:3]
 *
 * On Error it will return EMPTY_VALUE_8_BIT (Check _lastMcpError).
 * @note Standard Identifier [2:0] can be get from getReceiveBufferStandardIdentifierLow().
 */
uint8_t MCP2515::getReceiveBufferStandardIdentifierHigh(uint8_t BufferNumber)
{
  if (BufferNumber > 1)
  {
    this->_lastMcpError = ERROR_MCP2515_VALUE_OUTA_RANGE;
    return EMPTY_VALUE_8_BIT;
  }

  return readInstruction(REG_RXBnSIDH(BufferNumber));
}
/**
 * @brief Get the Receive Buffer Standard Identifier Low Register (REG_RXBnSIDL) [62h, 72h]
 * @param BufferNumber 0 - 1
 * @return
 * Bit 7 - 5 -> SID - Standard Identifier [2:0]
 *
 * Bit 4 -> SRR - Standard Frame Remote Transmit Request bit (valid only if IDE bit = 0)
 *
 * 1 = Standard frame Remote Transmit Request received
 *
 * 0 = Standard data frame received
 *
 * Bit 3 -> IDE - Extended Identifier Flag bit
 *
 * This bit indicates whether the received message was a standard or an extended frame.
 *
 * 1 = Received message was an extended frame
 *
 * 0 = Received message was a standard frame
 *
 * Bit 1 - 0 -> EID - Extended Identifier bits [17:16]
 *
 * On Error it will return EMPTY_VALUE_8_BIT (Check _lastMcpError).
 * @note Standard Identifier [10:3] can be get from getReceiveBufferStandardIdentifierHigh().
 *
 * Extended Identifier [15:8] and [7:0] can be get from getReceiveBufferExtendedIdentifierHigh() and getReceiveBufferExtendedIdentifierLow().
 */
uint8_t MCP2515::getReceiveBufferStandardIdentifierLow(uint8_t BufferNumber)
{
  if (BufferNumber > 1)
  {
    this->_lastMcpError = ERROR_MCP2515_VALUE_OUTA_RANGE;
    return EMPTY_VALUE_8_BIT;
  }

  return readInstruction(REG_RXBnSIDL(BufferNumber));
}
/**
 * @brief Get the Receive Buffer Extended Identifier High Register (REG_RXBnEID8) [63h, 73h]
 * @param BufferNumber 0 - 1
 * @return uint8_t Extended Identifier [15:8]
 *
 * On Error it will return EMPTY_VALUE_8_BIT (Check _lastMcpError).
 * @note Extended Identifier [17:16] and [7:0] can be get from getReceiveBufferStandardIdentifierLow() and getReceiveBufferExtendedIdentifierLow().
 */
uint8_t MCP2515::getReceiveBufferExtendedIdentifierHigh(uint8_t BufferNumber)
{
  if (BufferNumber > 1)
  {
    this->_lastMcpError = ERROR_MCP2515_VALUE_OUTA_RANGE;
    return EMPTY_VALUE_8_BIT;
  }

  return readInstruction(REG_RXBnEID8(BufferNumber));
}
/**
 * @brief Get the Receive Buffer Extended Identifier Low Register (REG_RXBnEID0) [64h, 74h]
 * @param BufferNumber 0 - 1
 * @return uint8_t Extended Identifier [7:0]
 *
 * On Error it will return EMPTY_VALUE_8_BIT (Check _lastMcpError).
 * @note Extended Identifier [17:16] and [15:8] can be get from getReceiveBufferStandardIdentifierLow() and getReceiveBufferExtendedIdentifierHigh().
 */
uint8_t MCP2515::getReceiveBufferExtendedIdentifierLow(uint8_t BufferNumber)
{
  if (BufferNumber > 1)
  {
    this->_lastMcpError = ERROR_MCP2515_VALUE_OUTA_RANGE;
    return EMPTY_VALUE_8_BIT;
  }

  return readInstruction(REG_RXBnEID0(BufferNumber));
}
/**
 * @brief Get the Receive Buffer Data Length Code Register (REG_RXBnDLC) [65h, 75h]
 * @param BufferNumber 0 - 1
 * @return
 * Bit 6 -> RTR - Extended Frame Remote Transmission Request bit (valid only when IDE (RXBnSIDL[3]) = 1)
 *
 * 1 = Extended frame Remote Transmit Request received
 *
 * 0 = Extended data frame received
 *
 * Bit 3 - 0 -> DLC - Data Length Code bits.
 *
 * On Error it will return EMPTY_VALUE_8_BIT (Check _lastMcpError).
 * @note It is possible to set the DLC[3:0] bits to a value greater than eight; however, only eight bytes are transmitted.
 */
uint8_t MCP2515::getReceiveBufferDataLengthCode(uint8_t BufferNumber)
{
  if (BufferNumber > 1)
  {
    this->_lastMcpError = ERROR_MCP2515_VALUE_OUTA_RANGE;
    return EMPTY_VALUE_8_BIT;
  }

  return readInstruction(REG_RXBnDLC(BufferNumber));
}

/**
 * @brief Get the Receive Buffer Data Byte Register (REG_RXBnDm) [66h-6Dh, 76h-7Dh]
 * @param BufferNumber 0 - 1
 * @param ByteNumber 0 -7
 * @return uint8_t Data
 *
 * On Error it will return EMPTY_VALUE_8_BIT (Check _lastMcpError).
 */
uint8_t MCP2515::getReceiveBufferDataByte(uint8_t BufferNumber, uint8_t ByteNumber)
{
  if (BufferNumber > 1 || ByteNumber > 7)
  {
    this->_lastMcpError = ERROR_MCP2515_VALUE_OUTA_RANGE;
    return EMPTY_VALUE_8_BIT;
  }

  switch (ByteNumber)
  {
  case 0:
    return readInstruction(REG_RXBnD0(BufferNumber));
    break;
  case 1:
    return readInstruction(REG_RXBnD1(BufferNumber));
    break;
  case 2:
    return readInstruction(REG_RXBnD2(BufferNumber));
    break;
  case 3:
    return readInstruction(REG_RXBnD3(BufferNumber));
    break;
  case 4:
    return readInstruction(REG_RXBnD4(BufferNumber));
    break;
  case 5:
    return readInstruction(REG_RXBnD5(BufferNumber));
    break;
  case 6:
    return readInstruction(REG_RXBnD6(BufferNumber));
    break;
  case 7:
    return readInstruction(REG_RXBnD7(BufferNumber));
    break;
  default:
    this->_lastMcpError = ERROR_MCP2515_UNKNOWN_SWITCH;
    return EMPTY_VALUE_8_BIT;
    break;
  }
}

/**
 * @brief Reinitialize the internal registers of the MCP2515 and set the Configuration mode.
 * @note Doc p. 65; Kap. 12.2
 * @return true on success, false when not (Check _lastMcpError)
 */
bool MCP2515::resetInstruction()
{
  SPI.beginTransaction(_spiSettings);
  digitalWrite(_csPin, LOW);
  SPI.transfer(MCP2515_SPI_INSTRUCTION_RESET);
  digitalWrite(_csPin, HIGH);
  SPI.endTransaction();

  delayMicroseconds(10);

  // Check if Configuration-Mode is set
  if (_reCheckEnabled)
  {
    if (((getCanStatus() & CANSTAT_BIT_OPMOD) >> 5) != MCP2515_OP_CONFIGURATION) {
      this->_lastMcpError = ERROR_SPI_CONFIGURATION_MODE_NOT_SET;
      return false;
    }
  }

  this->_operationMode = MCP2515_OP_CONFIGURATION;
  return true;
}

/**
 * @brief Write the given Data in the given Register.
 * @note Doc p. 65; Kap. 12.5
 * @param Address Register-Address
 * @param Value Register-Data
 * @return Returns only true (no Error will be set).
 */
bool MCP2515::writeInstruction(uint8_t Address, uint8_t Value)
{
  SPI.beginTransaction(_spiSettings);
  digitalWrite(_csPin, LOW);
  SPI.transfer(MCP2515_SPI_INSTRUCTION_WRITE);
  SPI.transfer(Address);
  SPI.transfer(Value);
  digitalWrite(_csPin, HIGH);
  SPI.endTransaction();
  delayMicroseconds(10);

  return true;
}

/**
 * @brief Setting or clearing individual bits in specific status and control registers.
 *
 * Allowed Register-Adresses are:
 *
 * BFPCTRL, TXRTSCTRL, CANCTRL, CNF3, CNF2, CNF1, CANINTE, CANINTF, EFLG, TXB0CTRL, TXB1CTRL, TXB2CTRL, RXB0CTRL, RXB1CTRL
 * @note Doc p. 66; Kap. 12.10
 * @param Address Register-Adress
 * @param Mask Mask
 * @param Value Value
 * @return true on success, false when not (Check _lastMcpError)
 */
bool MCP2515::bitModifyInstruction(uint8_t Address, uint8_t Mask, uint8_t Value)
{
  if (Address==REG_BFPCTRL or Address==REG_TXRTSCTRL or Address==REG_CANCTRL or Address==REG_CNF3 or
      Address==REG_CNF2 or Address==REG_CNF1 or Address==REG_CANINTE or Address==REG_CANINTF or
      Address==REG_EFLG or Address==REG_TXBnCTRL(0) or Address==REG_TXBnCTRL(1) or Address==REG_TXBnCTRL(2) or
      Address==REG_RXBnCTRL(0) or Address==REG_RXBnCTRL(1))
  {
    SPI.beginTransaction(_spiSettings);
    digitalWrite(_csPin, LOW);
    SPI.transfer(MCP2515_SPI_INSTRUCTION_BIT_MODIFY);
    SPI.transfer(Address);
    SPI.transfer(Mask);
    SPI.transfer(Value);
    digitalWrite(_csPin, HIGH);
    SPI.endTransaction();

    return true;
  }

  this->_lastMcpError = ERROR_SPI_REGISTER_NOT_ALLOWED;
  return false;
}

/**
 * @brief Sets the Address Pointer to one of six addresses to quickly write to a transmit buffer that points to the “ID” or “data” address of any of the three transmit buffers.
 *
 * a - b - c - Address Points to - Address
 *
 * 0 - 0 - 0 - TX Buffer 0, Start at TXB0SIDH - 0x31
 *
 * 0 - 0 - 1 - TX Buffer 0, Start at TXB0D0 - 0x36
 *
 * 0 - 1 - 0 - TX Buffer 1, Start at TXB1SIDH - 0x41
 *
 * 0 - 1 - 1 - TX Buffer 1, Start at TXB1D0 - 0x46
 *
 * 1 - 0 - 0 - TX Buffer 2, Start at TXB2SIDH - 0x51
 *
 * 1 - 0 - 1 - TX Buffer 2, Start at TXB2D0 - 0x56
 * @note Doc p. 65; Kap. 12.6
 * @param a TX-Buffer Adress-Pointer (false or true)
 * @param b TX-Buffer Adress-Pointer (false or true)
 * @param c TX-Buffer Adress-Pointer (false or true)
 * @param Value Data
 * @return true on success, false when not (Check _lastMcpError)
 */
bool MCP2515::loadTxBufferInstruction(uint8_t Value, bool a, bool b, bool c)
{
  if ((a != false && a != true) ||
      (b != false && b != true) ||
      (c != false && c != true))
  {
    this->_lastMcpError = ERROR_SPI_VALUE_OUTA_RANGE;
    return false;
  }

  uint8_t Instruction = MCP2515_SPI_INSTRUCTION_LOAD_TX_BUFFER | a << 2 | b << 1 | c;

  SPI.beginTransaction(_spiSettings);
  digitalWrite(_csPin, LOW);
  SPI.transfer(Instruction);
  SPI.transfer(Value);
  digitalWrite(_csPin, HIGH);
  SPI.endTransaction();

  return true;
}

/**
 * @brief Initiate message transmission for one or more of the transmit buffers.
 * @note Doc p. 65; Kap. 12.7
 * @param TXBuffer_0 Initiate transmission for TX-Buffer 0 (false or true)
 * @param TXBuffer_1 Initiate transmission for TX-Buffer 1 (false or true)
 * @param TXBuffer_2 Initiate transmission for TX-Buffer 2 (false or true)
 * @return true on success, false when not (Check _lastMcpError)
 */
bool MCP2515::rtsInstruction(bool TXBuffer_0, bool TXBuffer_1, bool TXBuffer_2)
{
  if ((TXBuffer_0 != false && TXBuffer_0 != true) ||
      (TXBuffer_1 != false && TXBuffer_1 != true) ||
      (TXBuffer_2 != false && TXBuffer_2 != true))
  {
    this->_lastMcpError = ERROR_SPI_VALUE_OUTA_RANGE;
    return false;
  } else if (TXBuffer_0 || TXBuffer_1 || TXBuffer_2)
  {
    uint8_t Instruction = MCP2515_SPI_INSTRUCTION_RTS | TXBuffer_2 << 2 | TXBuffer_1 << 1 | TXBuffer_0;
    SPI.beginTransaction(_spiSettings);
    digitalWrite(_csPin, LOW);
    SPI.transfer(Instruction);
    digitalWrite(_csPin, HIGH);
    SPI.endTransaction();
    return true;
  } else {
    this->_lastMcpError = ERROR_SPI_NO_TRANSMISSION_INITIATED;
    return false;
  }
}

/**
 * @brief Determine which filter matched the message and message type (standard, extended, remote).
 * @note Doc p. 66; Kap. 12.9
 * @return
 *  7 | 6 | Received Message  || 4 | 3 | Msg Type Received     || 2 | 1 | 0 | Filter Match
 *
 *  0 | 0 |  No RX message    || 0 | 0 | Standard data frame   || 0 | 0 | 0 | RXF0
 *
 *  0 | 1 | Message in RXB0   || 0 | 1 | Standard remote frame || 0 | 0 | 1 | RXF1
 *
 *  1 | 0 | Message in RXB1   || 1 | 0 | Extended data frame   || 0 | 1 | 0 | RXF2
 *
 *  1 | 1 | Messages in both buffers^ || 1 | 1 | Extended remote frame || 0 | 1 | 1 | RXF3
 *
 *
 *  RXnIF (CANINTF) bits are  ||  The extended ID bit is       || 1 | 0 | 0 | RXF4
 *
 *  mapped to bits 7 and 6.   ||  mapped to bit 4.             || 1 | 0 | 1 | RXF5
 *
 *                            ||   The RTR bit is mapped to    || 1 | 1 | 0 | RXF0 (rollover to RXB1)
 *
 *                            ||   bit 3.                      || 1 | 1 | 1 | RXF1 (rollover to RXB1)
 *
 * ^Buffer 0 has higher priority; therefore, RXB0 status is reflected in bits[4:0].
 *
 * No Error will be set.
 */
uint8_t MCP2515::rxStatusInstruction()
{
  uint8_t value;
  SPI.beginTransaction(_spiSettings);
  digitalWrite(_csPin, LOW);
  SPI.transfer(MCP2515_SPI_INSTRUCTION_RX_STATUS);
  value = SPI.transfer(0x00);
  digitalWrite(_csPin, HIGH);
  SPI.endTransaction();

  return value;
}

/**
 * @brief Read the Value of the given Register-Address.
 * @note Doc p. 65; Kap. 12.3
 * @param Address Register-Address
 * @return Register-Value
 *
 * No Error will be set.
 */
uint8_t MCP2515::readInstruction(uint8_t Address)
{
  uint8_t value;

  SPI.beginTransaction(_spiSettings);
  digitalWrite(_csPin, LOW);
  SPI.transfer(MCP2515_SPI_INSTRUCTION_READ);
  SPI.transfer(Address);
  value = SPI.transfer(0x00);
  digitalWrite(_csPin, HIGH);
  SPI.endTransaction();

  return value;
}

/**
 * @brief Single instruction access to some of the often used status bits for message reception and transmission.
 * @note Doc p. 66; Kap. 12.8
 * @returns
 * Bit 0 - RX0IF (CANINTF[0])   - Receive Buffer 0 Full Interrupt Flag bit
 *
 * Bit 1 - RX1IF (CANINTF[1])   - Receive Buffer 1 Full Interrupt Flag bit
 *
 * Bit 2 - TXREQ (TXB0CNTRL[3]) - Message Transmit Request bit Buffer 0
 *
 * Bit 3 - TX0IF (CANINTF[2])   - Transmit Buffer 0 Empty Interrupt Flag bit
 *
 * Bit 4 - TXREQ (TXB1CNTRL[3]) - Message Transmit Request bit Buffer 1
 *
 * Bit 5 - TX1IF (CANINTF[3])   - Transmit Buffer 1 Empty Interrupt Flag bit
 *
 * Bit 6 - TXREQ (TXB2CNTRL[3]) - Message Transmit Request bit Buffer 2
 *
 * Bit 7 - TX2IF (CANINTF[4])   - Transmit Buffer 2 Empty Interrupt Flag bit
 *
 * No Error will be set.
 */
uint8_t MCP2515::readStatusInstruction()
{
  uint8_t value;

  SPI.beginTransaction(_spiSettings);
  digitalWrite(_csPin, LOW);
  SPI.transfer(MCP2515_SPI_INSTRUCTION_READ_STATUS);
  value = SPI.transfer(0x00);
  digitalWrite(_csPin, HIGH);
  SPI.endTransaction();

  return value;
}

/**
 * @brief Quickly address a receive buffer for reading.
 *
 * n - m - Address Points to - Address - Flag
 *
 * 0 - 0 - Receive Buffer 0, Start at RXB0SIDH - 0x61 - RX0IF
 *
 * 0 - 1 - Receive Buffer 0, Start at RXB0D0 - 0x66 - RX0IF
 *
 * 1 - 0 - Receive Buffer 1, Start at RXB1SIDH - 0x71 - RX1IF
 *
 * 1 - 1 - Receive Buffer 1, Start at RXB1D0 - 0x76 - RX1IF
 *
 * The associated receive flag will be cleared automatically.
 * @note Doc p. 65; Kap. 12.4
 * @param n RX-Buffer Adress-Pointer (false or true)
 * @param m RX-Buffer Adress-Pointer (false or true)
 * @return Buffer-Value
 *
 * On Error it will return EMPTY_VALUE_8_BIT (Check _lastMcpError).
 */
uint8_t MCP2515::readRxBufferInstruction(bool n, bool m)
{
  uint8_t value;

  if ((n != false && n != true) ||
      (m != false && m != true))
  {
    this->_lastMcpError = ERROR_SPI_VALUE_OUTA_RANGE;
    return false;
  }

  uint8_t Instruction = MCP2515_SPI_INSTRUCTION_READ_RX_BUFFER | n << 2 | m << 1;

  SPI.beginTransaction(_spiSettings);
  digitalWrite(_csPin, LOW);
  SPI.transfer(Instruction);
  value = SPI.transfer(0x00);
  digitalWrite(_csPin, HIGH);
  SPI.endTransaction();

  return value;
}

/**
 * @brief Changing the Bit-Timing according to the Baudrate and ClockFrequency.
 * @param targetBaudrate possible Baudrates= (5E3, 10E3, 20E3, 40E3, 50E3, 80E3, 100E3, 125E3, 200E3, 250E3, 500E3, 1000E3)
 * @param targetClockFrequency possible ClockFrequencys= (8E6, 16E6, 25E6, 40E6)
 * @return true when success, false on any error (Check _lastMcpError)
 */
bool MCP2515::changeBitTiming(uint32_t targetBaudRate, uint32_t targetClockFrequency)
{
  uint8_t OperationMode = _operationMode;

  if (targetBaudRate != 5E3 && targetBaudRate != 10E3 && targetBaudRate != 20E3 &&
      targetBaudRate != 40E3 && targetBaudRate != 50E3 && targetBaudRate != 80E3 &&
      targetBaudRate != 100E3 && targetBaudRate != 125E3 && targetBaudRate != 200E3 &&
      targetBaudRate != 250E3 && targetBaudRate != 500E3 && targetBaudRate != 1000E3)
  {
    this->_lastMcpError = ERROR_MCP2515_BAUDRATE_NOT_VALID;
    return false;
  }

  if (targetClockFrequency != (uint32_t)8E6 && targetClockFrequency != 16E6 && targetClockFrequency != 25E6 && targetClockFrequency != 40E6)
  {
    this->_lastMcpError = ERROR_MCP2515_CLOCKFREQUENCY_NOT_VALID;
    return false;
  }

  // Values calculated with the BitTimeCalculator (see ..\docs\BitTimeCalculator.xlsx)
  const struct {
    uint32_t clockFrequency;
    uint32_t baudRate;
    uint8_t cnf[3];
  } CNF_MAPPER[] = {
    //{  (uint32_t)8E6, (uint32_t)1000E3, { 0x00, 0x80, 0x01 } }, // not possible, Prescaler out of range
    {  (uint32_t)8E6,  (uint32_t)500E3, { 0x40, 0x89, 0x02 } },
    {  (uint32_t)8E6,  (uint32_t)250E3, { 0xc0, 0xa4, 0x04 } },
    {  (uint32_t)8E6,  (uint32_t)200E3, { 0xc0, 0xad, 0x06 } },
    {  (uint32_t)8E6,  (uint32_t)125E3, { 0xc1, 0xa4, 0x04 } },
    {  (uint32_t)8E6,  (uint32_t)100E3, { 0xc1, 0xad, 0x06 } },
    {  (uint32_t)8E6,   (uint32_t)80E3, { 0xc1, 0xbf, 0x07 } },
    {  (uint32_t)8E6,   (uint32_t)50E3, { 0xc3, 0xad, 0x06 } },
    {  (uint32_t)8E6,   (uint32_t)40E3, { 0xc3, 0xbf, 0x07 } },
    {  (uint32_t)8E6,   (uint32_t)20E3, { 0xc7, 0xbf, 0x07 } },
    {  (uint32_t)8E6,   (uint32_t)10E3, { 0xcf, 0xbf, 0x07 } },
    {  (uint32_t)8E6,    (uint32_t)5E3, { 0xdf, 0xbf, 0x07 } },

    { (uint32_t)16E6, (uint32_t)1000E3, { 0x40, 0x89, 0x02 } },
    { (uint32_t)16E6,  (uint32_t)500E3, { 0xc0, 0xa4, 0x04 } },
    { (uint32_t)16E6,  (uint32_t)250E3, { 0xc1, 0xa4, 0x04 } },
    { (uint32_t)16E6,  (uint32_t)200E3, { 0xc1, 0xad, 0x06 } },
    { (uint32_t)16E6,  (uint32_t)125E3, { 0xc3, 0xa4, 0x04 } },
    { (uint32_t)16E6,  (uint32_t)100E3, { 0xc3, 0xad, 0x06 } },
    { (uint32_t)16E6,   (uint32_t)80E3, { 0xc3, 0xbf, 0x07 } },
    { (uint32_t)16E6,   (uint32_t)50E3, { 0xc7, 0xad, 0x06 } },
    { (uint32_t)16E6,   (uint32_t)40E3, { 0xc7, 0xbf, 0x07 } },
    { (uint32_t)16E6,   (uint32_t)20E3, { 0xcf, 0xbf, 0x07 } },
    { (uint32_t)16E6,   (uint32_t)10E3, { 0xdf, 0xbf, 0x07 } },
    { (uint32_t)16E6,    (uint32_t)5E3, { 0xff, 0xbf, 0x07 } },

    { (uint32_t)25E6, (uint32_t)1000E3, { 0x80, 0x9a, 0x03 } }, // Attention!!! 40ns faster than the regular Bittime (1000ns) -> not recommended
    { (uint32_t)25E6,  (uint32_t)500E3, { 0xc0, 0xbf, 0x07 } },
    { (uint32_t)25E6,  (uint32_t)250E3, { 0xc1, 0xbf, 0x07 } },
    { (uint32_t)25E6,  (uint32_t)200E3, { 0xc2, 0xb5, 0x06 } }, // Attention!!! 40ns slower than the regular Bittime (5000ns)
    { (uint32_t)25E6,  (uint32_t)125E3, { 0xc3, 0xbf, 0x07 } },
    { (uint32_t)25E6,  (uint32_t)100E3, { 0xc4, 0xbf, 0x07 } },
    { (uint32_t)25E6,   (uint32_t)80E3, { 0x8b, 0x9b, 0x03 } }, // Attention!!! 20ns faster than the regular Bittime (12500ns)
    { (uint32_t)25E6,   (uint32_t)50E3, { 0xc9, 0xbf, 0x07 } },
    { (uint32_t)25E6,   (uint32_t)40E3, { 0xcc, 0xbe, 0x07 } }, // Attention!!! 40ns faster than the regular Bittime (25000ns)
    { (uint32_t)25E6,   (uint32_t)20E3, { 0xd8, 0xbf, 0x07 } },
    { (uint32_t)25E6,   (uint32_t)10E3, { 0xf1, 0xbf, 0x07 } },
    //{ (uint32_t)25E6,    (uint32_t)5E3, { 0xff, 0xbf, 0x07 } }, // not possible, Prescaler out of range

    { (uint32_t)40E6, (uint32_t)1000E3, { 0xc0, 0xad, 0x06 } },
    { (uint32_t)40E6,  (uint32_t)500E3, { 0xc1, 0xad, 0x06 } },
    { (uint32_t)40E6,  (uint32_t)250E3, { 0xc3, 0xad, 0x06 } },
    { (uint32_t)40E6,  (uint32_t)200E3, { 0xc3, 0xbf, 0x07 } },
    { (uint32_t)40E6,  (uint32_t)125E3, { 0xc7, 0xad, 0x06 } },
    { (uint32_t)40E6,  (uint32_t)100E3, { 0xc7, 0xbf, 0x07 } },
    { (uint32_t)40E6,   (uint32_t)80E3, { 0xc9, 0xbf, 0x07 } },
    { (uint32_t)40E6,   (uint32_t)50E3, { 0xcf, 0xbf, 0x07 } },
    { (uint32_t)40E6,   (uint32_t)40E3, { 0xd3, 0xbf, 0x07 } },
    { (uint32_t)40E6,   (uint32_t)20E3, { 0xe7, 0xbf, 0x07 } },
    //{ (uint32_t)40E6,   (uint32_t)10E3, { 0xf1, 0xbf, 0x07 } }, // not possible, Prescaler out of range
    //{ (uint32_t)40E6,    (uint32_t)5E3, { 0xff, 0xbf, 0x07 } }, // not possible, Prescaler out of range
  };

  const uint8_t* cnf = NULL;

  for (unsigned int i = 0; i < (sizeof(CNF_MAPPER) / sizeof(CNF_MAPPER[0])); i++) {
    if ((CNF_MAPPER[i].clockFrequency == targetClockFrequency) && (CNF_MAPPER[i].baudRate == targetBaudRate)) {
      cnf = CNF_MAPPER[i].cnf;
      break;
    }
  }

  if (cnf == NULL) {
    this->_lastMcpError = ERROR_MCP2515_CNF_NOT_FOUND;
    return false;
  }

  if (OperationMode != MCP2515_OP_CONFIGURATION)
  {
    if (!setConfigurationMode())
    {
      // Error will be set in setConfigurationMode()
      return false;
    }
  }

  if (!modifyConfigurationRegister1(0xFF, cnf[0]))
  {
    this->_lastMcpError = _lastMcpError | ERROR_MCP2515_CNF1_NOT_SET;
      return false;
  }

  if (!modifyConfigurationRegister2(0xFF, cnf[1]))
  {
    this->_lastMcpError = _lastMcpError | ERROR_MCP2515_CNF2_NOT_SET;
      return false;
  }

  if (!modifyConfigurationRegister3(0xC7, cnf[2]))
  {
    this->_lastMcpError = _lastMcpError | ERROR_MCP2515_CNF3_NOT_SET;
      return false;
  }

  delayMicroseconds(10);

  if (OperationMode != _operationMode)
  {
    switch (OperationMode)
    {
    case MCP2515_OP_LISTEN:
      return setListenOnlyMode();
      break;
    case MCP2515_OP_LOOPBACK:
      return setLoopbackMode();
      break;
    case MCP2515_OP_NORMAL:
      return setNormalMode();
      break;
    case MCP2515_OP_SLEEP:
      return setSleepMode();
      break;
    case MCP2515_OP_CONFIGURATION:
      return setConfigurationMode();
      break;
    default:
      this->_lastMcpError = ERROR_MCP2515_UNKNOWN_SWITCH;
      return false;
      break;
    }
  }
  return true;
}




/***********************************************************************************************************************
 * 									Public Methods
 **********************************************************************************************************************/


/**
 * @brief ClassConstructor
 */
MCP2515::MCP2515() :
  _spiSettings(MCP2515_MAX_SPI_SPEED, MCP2515_SPI_DATA_ORDER, MCP2515_SPI_MODE),
  _csPin(0),
  _spiMode(MCP2515_SPI_MODE),
  _dataOrder(MCP2515_SPI_DATA_ORDER),
  _spiFrequency(MCP2515_MAX_SPI_SPEED),
  _clockFrequency(MCP2515_DEFAULT_CLOCK_FREQUENCY),
  _operationMode(0xFF),
  _baudRate(MCP2515_DEFAULT_BAUDRATE),
  _isInitialized(false),
  _lastMcpError(EMPTY_VALUE_16_BIT),
  _lastSpiError(EMPTY_VALUE_16_BIT),
  _reCheckEnabled(true)
{
}

/**
 * @brief Deconstructor
 */
MCP2515::~MCP2515()
{
}

/**
 * @brief Returns the last MCP-Error.
 *
 * The last MCP-Error will always been reset at the beginning of a Method.
 * @return uint16_t MCP-Error
 *
 * 0x0000 = no Error
 */
uint16_t MCP2515::getLastMCPError()
{
	return _lastMcpError;
}

/**
 * @brief Set the ReCheck-Enabler.
 *
 * When it's enabled, after Setting some bit it will check if the bit is set successfully.
 * @param reCheckEnabler bool
 * @return bool true when success, false on any error
 */
bool MCP2515::setReCheckEnabler(bool reCheckEnabler)
{
  this->_lastMcpError = EMPTY_VALUE_16_BIT;

  if (!_isInitialized)
  {
    this->_lastMcpError = ERROR_MCP2515_NOT_INITIALIZED;
    return false;
  }

  if (reCheckEnabler != true && reCheckEnabler != false)
  {
    this->_lastMcpError = ERROR_MCP2515_VALUE_OUTA_RANGE;
    return false;
  }

  this->_reCheckEnabled = reCheckEnabler;

  return true;
}

/**
 * @brief Set the CS (ChipSelect) Pin.
 * @note Use this Method only before the Initialisation of the MCP2515.
 * @param cs ChipSelect-Pin
 * @return true when success, false on any error
 */
bool MCP2515::setSpiPins(uint8_t cs)
{
  this->_lastMcpError = EMPTY_VALUE_16_BIT;

  if (_isInitialized)
  {
    this->_lastMcpError = ERROR_MCP2515_IS_INITIALIZED;
    return false;
  }

  this->_csPin = cs;

  return true;
}

/**
 * @brief Setting the SPI-Mode.
 * @note Use this Method only before the Initialisation of the MCP2515.
 * @param SpiMode Allowed Values for the MCP2515 are SPI_MODE0 and SPI_MODE3.
 * @return true when success, false on any error
 */
bool MCP2515::setSpiMode(uint8_t SpiMode)
{
  this->_lastMcpError = EMPTY_VALUE_16_BIT;

  if (_isInitialized)
  {
    this->_lastMcpError = ERROR_MCP2515_IS_INITIALIZED;
    return false;
  }

  if (SpiMode != SPI_MODE0 && SpiMode != SPI_MODE3)
  {
    this->_lastMcpError = ERROR_MCP2515_SPI_MODE_NOT_ALLOWED;
    return false;
  }

  this->_spiMode = SpiMode;
  SPISettings _spiSettings(_spiFrequency, _dataOrder, SpiMode);

  return true;
}

/**
 * @brief Set the DataOrder.
 * @note Use this Method only before the Initialisation of the MCP2515.
 * @param DataOrder Allowed Value for the MCP2515 is MSBFIRST.
 * @return true when success, false on any error
 */
bool MCP2515::setDataOrder(uint8_t DataOrder)
{
  this->_lastMcpError = EMPTY_VALUE_16_BIT;

  if (_isInitialized)
  {
    this->_lastMcpError = ERROR_MCP2515_IS_INITIALIZED;
    return false;
  }

  if (DataOrder != MSBFIRST)
  {
    this->_lastMcpError = ERROR_MCP2515_DATA_ORDER_NOT_ALLOWED;
    return false;
  }

  this->_dataOrder = DataOrder;
  SPISettings _spiSettings(_spiFrequency, DataOrder, _spiMode);

  return true;
}

/**
 * @brief Set the SPI-Clock-Frequency.
 * @note Use this Method only before the Initialisation of the MCP2515.
 * @param Frequency max. allowed Value for the MCP2515 is 10e6 Hz.
 * @return true when success, false on any error
 */
bool MCP2515::setSpiFrequency(uint32_t Frequency)
{
  this->_lastMcpError = EMPTY_VALUE_16_BIT;

  if (_isInitialized)
  {
    this->_lastMcpError = ERROR_MCP2515_IS_INITIALIZED;
    return false;
  }

  if (((uint32_t)Frequency > (uint32_t)MCP2515_MAX_SPI_SPEED) ||
      ((uint32_t)Frequency < (uint32_t)1e6))
  {
    this->_lastMcpError = ERROR_MCP2515_SPI_FREQUENCY_NOT_ALLOWED;
    return false;
  }

  this->_spiFrequency = Frequency;
  SPISettings _spiSettings(Frequency, _dataOrder, _spiMode);

  return true;
}

/**
 * @brief Set the MCP2515 Clock-Frequency.
 * @note Use this Method only before the Initialisation of the MCP2515.
 * @param ClockFrequency max. allowed Value for the MCP2515 is 40e6 Hz.
 * @return true when success, false on any error
 */
bool MCP2515::setClockFrequency(uint32_t ClockFrequency)
{
  if (_isInitialized)
  {
    this->_lastMcpError = ERROR_MCP2515_IS_INITIALIZED;
    return false;
  }

  if ((uint32_t)ClockFrequency > (uint32_t)MCP2515_MAX_CLOCK_FREQUENCY)
  {
    this->_lastMcpError = ERROR_MCP2515_CLOCK_FREQUENCY_NOT_ALLOWED;
    return false;
  }

  this->_clockFrequency = ClockFrequency;

  return true;
}

/**
 * @brief Set the MCP2515 in the Configuration-Mode.
 * @note Doc p. 59; Kap. 10.1
 * @return true when success, false on any error (Check _lastMcpError)
 */
bool MCP2515::setConfigurationMode()
{
  this->_lastMcpError = EMPTY_VALUE_16_BIT;

  if (!_isInitialized)
  {
    this->_lastMcpError = ERROR_MCP2515_NOT_INITIALIZED;
    return false;
  }

  int8_t counter = 100;

  while (!modifyCanControl(CANCTRL_BIT_REQOP, (MCP2515_OP_CONFIGURATION << 5)) && counter > 0)
  {
    counter--;
    delayMicroseconds(10);
  }

  if (counter <= 0)
  {
    this->_lastMcpError = ERROR_MCP2515_OPERATION_MODE_NOT_SET | _lastMcpError;
    return false;
  }

  this->_operationMode = MCP2515_OP_CONFIGURATION;
  return true;
}

/**
 * @brief Set the MCP2515 in the Normal-Mode.
 * @note Doc p. 60; Kap. 10.5
 * @return true when success, false on any error (Check _lastMcpError)
 */
bool MCP2515::setNormalMode()
{
  this->_lastMcpError = EMPTY_VALUE_16_BIT;

  if (!_isInitialized)
  {
    this->_lastMcpError = ERROR_MCP2515_NOT_INITIALIZED;
    return false;
  }

  int8_t counter = 100;

  while (!modifyCanControl(CANCTRL_BIT_REQOP, (MCP2515_OP_NORMAL << 5)) && counter > 0)
  {
    counter--;
    delayMicroseconds(10);
  }

  if (counter <= 0)
  {
    this->_lastMcpError = ERROR_MCP2515_OPERATION_MODE_NOT_SET | _lastMcpError;
    return false;
  }

  this->_operationMode = MCP2515_OP_NORMAL;
  return true;
}

/**
 * @brief Set the MCP2515 in the Sleep-Mode.
 * @note Doc p. 59; Kap. 10.2
 * @return true when success, false on any error (Check _lastMcpError)
 */
bool MCP2515::setSleepMode()
{
  this->_lastMcpError = EMPTY_VALUE_16_BIT;

  if (!_isInitialized)
  {
    this->_lastMcpError = ERROR_MCP2515_NOT_INITIALIZED;
    return false;
  }

  int8_t counter = 100;

  while (!modifyCanControl(CANCTRL_BIT_REQOP, (MCP2515_OP_SLEEP << 5)) && counter > 0)
  {
    counter--;
    delayMicroseconds(10);
  }

  if (counter <= 0)
  {
    this->_lastMcpError = ERROR_MCP2515_OPERATION_MODE_NOT_SET | _lastMcpError;
    return false;
  }

  this->_operationMode = MCP2515_OP_SLEEP;
  return true;
}

/**
 * @brief Set the MCP2515 in the Listen-Only-Mode.
 * @note Doc p. 59; Kap. 10.3
 * @return true when success, false on any error (Check _lastMcpError)
 */
bool MCP2515::setListenOnlyMode()
{
  this->_lastMcpError = EMPTY_VALUE_16_BIT;

  if (!_isInitialized)
  {
    this->_lastMcpError = ERROR_MCP2515_NOT_INITIALIZED;
    return false;
  }

  int8_t counter = 100;

  while (!modifyCanControl(CANCTRL_BIT_REQOP, (MCP2515_OP_LISTEN << 5)) && counter > 0)
  {
    counter--;
    delayMicroseconds(10);
  }

  if (counter <= 0)
  {
    this->_lastMcpError = ERROR_MCP2515_OPERATION_MODE_NOT_SET | _lastMcpError;
    return false;
  }

  this->_operationMode = MCP2515_OP_LISTEN;
  return true;
}

/**
 * @brief Set the MCP2515 in the Loopback-Mode.
 * @note Doc p. 60; Kap. 10.4
 * @return true when success, false on any error (Check _lastMcpError)
 */
bool MCP2515::setLoopbackMode()
{
  this->_lastMcpError = EMPTY_VALUE_16_BIT;

  if (!_isInitialized)
  {
    this->_lastMcpError = ERROR_MCP2515_NOT_INITIALIZED;
    return false;
  }

  int8_t counter = 100;

  while (!modifyCanControl(CANCTRL_BIT_REQOP, (MCP2515_OP_LOOPBACK << 5)) && counter > 0)
  {
    counter--;
    delayMicroseconds(10);
  }

  if (counter <= 0)
  {
    this->_lastMcpError = ERROR_MCP2515_OPERATION_MODE_NOT_SET | _lastMcpError;
    return false;
  }

  this->_operationMode = MCP2515_OP_LOOPBACK;
  return true;
}

/**
 * @brief Initialize the MCP2515.
 *
 * - Reset the MCP2515
 *
 * - Set the BitTiming for the given BaudRate
 *
 * - Setting the PinControl for RX-Pins and TX-Pins
 *
 * - Enable Receive-Interrupts
 *
 * - Disable Mask and Filter-Settings
 *
 * - Set the OperationMode Normal
 * @param BaudRate Baudrate of the CAN-Communication
 * @param reCheckEnabled bool true if after Set-Processes a recheck of setted Values has to be execute
 * @return true when success, false on any error (Check _lastMcpError)
 */
bool MCP2515::init(uint32_t BaudRate, bool reCheckEnabled)
{
  this->_lastMcpError = EMPTY_VALUE_16_BIT;

  if (_isInitialized)
  {
    this->_lastMcpError = ERROR_MCP2515_IS_INITIALIZED;
    return false;
  }

  this->_reCheckEnabled = reCheckEnabled;

  if (_csPin == 0)
  {
    this->_lastMcpError = ERROR_MCP2515_INIT_CS_PIN_NOT_DEFINED;
    return false;
  }

  pinMode(_csPin, OUTPUT);
  this->_isInitialized = true;

  // start SPI
  SPI.begin();

  // Reset the MCP2515
  if (!resetInstruction()) {
    this->_isInitialized = false;
    this->_lastMcpError = _lastMcpError | ERROR_MCP2515_INIT_RESET_FAILED;
    return false;
  }

  // Change the Bit-Timing
  if (!changeBaudRate(BaudRate)) {
    this->_isInitialized = false;
    this->_lastMcpError = _lastMcpError | ERROR_MCP2515_INIT_CHANGE_BITTIMING;
    return false;
  }

  // Set TXnRTS-Pin to Digital Input
  if (!modifyTXnRTSPinControl(TXRTSCTRL_BIT_BnRTSM(0) | TXRTSCTRL_BIT_BnRTSM(1) | TXRTSCTRL_BIT_BnRTSM(2), 0x00))
  {
    this->_isInitialized = false;
    this->_lastMcpError = _lastMcpError | ERROR_MCP2515_INIT_TX_PIN_CONTROL;
    return false;
  }

  // Disable the RXnBF-Pins
  if (!modifyRXnBFPinControl(BFPCTRL_BIT_BnBFE(0) | BFPCTRL_BIT_BnBFE(1), 0x00))
  {
    this->_isInitialized = false;
    this->_lastMcpError = _lastMcpError | ERROR_MCP2515_INIT_RX_PIN_CONTROL;
    return false;
  }

  // Disable all Interrupts
  if (!setCanInterruptEnable(false, false, false, false, false, false, false, false))
  {
    this->_isInitialized = false;
    this->_lastMcpError = _lastMcpError | ERROR_MCP2515_INIT_SET_INTERRUPTS;
    return false;
  }

  // Set the TX-Buffer empty Flags
  if (!modifyCanInterruptFlag(0x1C, 0x1C))
  {
    this->_isInitialized = false;
    this->_lastMcpError = _lastMcpError | ERROR_MCP2515_INIT_SET_TX_BUFFER_FLAGS;
    return false;
  }

  // Disable the Mask und Filter
  if (!disableFilterMask(0) || !disableFilterMask(1))
  {
    this->_isInitialized = false;
    this->_lastMcpError = _lastMcpError | ERROR_MCP2515_INIT_DISABLE_MASKS_FILTERS;
    return false;
  }

  // Change to Operation-Mode Normal
  if (!setNormalMode()) {
    this->_isInitialized = false;
    this->_lastMcpError =_lastMcpError | ERROR_MCP2515_INIT_SET_NORMAL_MODE;
    return false;
  }

  return true;
}

/**
 * @brief Deinitialize the MCP2515.
 */
void MCP2515::deinit()
{
  this->_isInitialized = false;
  SPI.end();
}

/**
 * @brief Changing the Baudrate.
 * @param targetBaudrate possible Baudrates= (5E3, 10E3, 20E3, 40E3, 50E3, 80E3, 100E3, 125E3, 200E3, 250E3, 500E3, 1000E3)
 * @return true when success, false on any error (Check _lastMcpError)
 * @note If a Baudrate is allowed depends on the ClockFrequency.
 */
bool MCP2515::changeBaudRate(uint32_t targetBaudRate)
{
  this->_lastMcpError = EMPTY_VALUE_16_BIT;

  if (!_isInitialized)
  {
    this->_lastMcpError = ERROR_MCP2515_NOT_INITIALIZED;
    return false;
  }

  if (targetBaudRate != _baudRate)
  {
    if (!changeBitTiming(targetBaudRate, _clockFrequency))
    {
      // Error will be set in changeBitTiming()
      return false;
    }
    this->_baudRate = targetBaudRate;
  }
  return true;
}

/**
 * @brief Enable/Disable an Interrupt.
 * @param value bool
 *
 * True = Enable Interrupt
 *
 * False = Disable Interrupt
 * @param Interrupt 0 - 7
 *
 * 7 = MERRE - Message Error Interrupt
 *
 * 6 = WAKIE - Wake-up Interrupt
 *
 * 5 = ERRIE - Error Interrupt (multiple sources in EFLG register)
 *
 * 4 = TX2IE - Transmit Buffer 2 Empty Interrupt
 *
 * 3 = TX1IE - Transmit Buffer 1 Empty Interrupt
 *
 * 2 = TX0IE - Transmit Buffer 0 Empty Interrupt
 *
 * 1 = RX1IE - Receive Buffer 1 Full Interrupt
 *
 * 0 = RX0IE - Receive Buffer 0 Full Interrupt
 * @return true when success, false on any error (Check _lastMcpError)
 */
bool MCP2515::changeInterruptSetting(bool value, uint8_t Interrupt)
{
  this->_lastMcpError = EMPTY_VALUE_16_BIT;

  if (!_isInitialized)
  {
    this->_lastMcpError = ERROR_MCP2515_NOT_INITIALIZED;
    return false;
  }

  if ((Interrupt > 7) ||
      (value != false && value != true))
  {
    this->_lastMcpError = ERROR_MCP2515_VALUE_OUTA_RANGE;
    return false;
  }

  uint8_t InterruptBit = 0x01 << Interrupt;

  uint8_t EnablerValue = (value) ? InterruptBit : 0x00;

  if (!modifyCanInterruptEnable(InterruptBit, EnablerValue))
  {
    this->_lastMcpError = _lastMcpError | ERROR_MCP2515_CHANGE_INTERRUPT;
    return false;
  }

  return true;
}

/**
 * @brief Enables the Filter/Mask for the given Receive-Buffer.
 * @param buffer Receive-Buffer-Number (0 - 1)
 * @return true when success, false on any error (Check _lastMcpError)
 */
bool MCP2515::enableFilterMask(uint8_t buffer)
{
  this->_lastMcpError = EMPTY_VALUE_16_BIT;

  if (!_isInitialized)
  {
    this->_lastMcpError = ERROR_MCP2515_NOT_INITIALIZED;
    return false;
  }

  if (buffer > 1)
  {
    this->_lastMcpError = ERROR_MCP2515_VALUE_OUTA_RANGE;
    return false;
  }

  switch (buffer)
  {
  case 0:
    return modifyReceiveBuffer0Control(RXBnCTRL_BIT_RXM, 0x00);
    break;
  case 1:
    return modifyReceiveBuffer1Control(RXBnCTRL_BIT_RXM, 0x00);
    break;
  default:
    this->_lastMcpError = ERROR_MCP2515_UNKNOWN_SWITCH;
    return false;
    break;
  }
}

/**
 * @brief Disables the Filter/Mask for the given Receive-Buffer.
 * @param buffer Receive-Buffer-Number (0 - 1)
 * @return true when success, false on any error
 */
bool MCP2515::disableFilterMask(uint8_t buffer)
{
  this->_lastMcpError = EMPTY_VALUE_16_BIT;

  if (!_isInitialized)
  {
    this->_lastMcpError = ERROR_MCP2515_NOT_INITIALIZED;
    return false;
  }

  if (buffer > 1)
  {
    this->_lastMcpError = ERROR_MCP2515_VALUE_OUTA_RANGE;
    return false;
  }

  switch (buffer)
  {
  case 0:
    return modifyReceiveBuffer0Control(RXBnCTRL_BIT_RXM, RXBnCTRL_BIT_RXM);
    break;
  case 1:
    return modifyReceiveBuffer1Control(RXBnCTRL_BIT_RXM, RXBnCTRL_BIT_RXM);
    break;
  default:
    this->_lastMcpError = ERROR_MCP2515_UNKNOWN_SWITCH;
    return false;
    break;
  }
}

/**
 * @brief Check if a Transmit-Buffer is free
 * @return 0xFF if no Transmit-Buffer is free
 *
 * 0x0n - n = Number of the free Buffer
 */
uint8_t MCP2515::check4FreeTransmitBuffer()
{
  this->_lastMcpError = EMPTY_VALUE_16_BIT;

  if (!_isInitialized)
  {
    this->_lastMcpError = ERROR_MCP2515_NOT_INITIALIZED;
    return 0xFF;
  }

  uint8_t Value = readStatusInstruction();

  if ((Value & 0xA8) == 0x00)
  {
    this->_lastMcpError = ERROR_MCP2515_NO_TRANSMITBUFFER_FREE;
    return 0xFF;
  }

  for (size_t i = 0; i < 3; i++)
  {
    if ((Value & (0x08 << (2 * i))) != 0 )
    {
      return i;
      break;
    }
  }

  this->_lastMcpError = ERROR_MCP2515_UNDEFINED;
  return 0xFF;
}

/**
 * @brief Fill the given Transmit-Buffer with the given Data.
 * @param BufferNumber 0 - 2
 * @param ID Message-ID
 * @param Extended bool true if it is a Extended Frame
 * @param RTR bool true if it is a Remote Transmit request Message
 * @param DLC Data Length Code of the Message
 * @param Data 8-Byte Data-Array (in total)
 * @return true when success, false on any error (check _lastMcpError)
 */
bool MCP2515::fillTransmitBuffer(uint8_t BufferNumber, uint32_t ID, bool Extended, bool RTR, uint8_t DLC, uint8_t Data[8])
{
  this->_lastMcpError = EMPTY_VALUE_16_BIT;

  if (!_isInitialized)
  {
    this->_lastMcpError = ERROR_MCP2515_NOT_INITIALIZED;
    return false;
  }

  if ((BufferNumber > 2) ||
      (ID > 0x1FFFFFFF) ||
      (Extended != false && Extended != true) ||
      (RTR != false && RTR != true) ||
      (DLC > 8))
  {
    this->_lastMcpError = ERROR_MCP2515_VALUE_OUTA_RANGE;
    return false;
  }

  if ((check4InterruptFlags() & (0x04 << BufferNumber)) != (0x04 << BufferNumber))
  {
    if (_lastMcpError != EMPTY_VALUE_16_BIT)
    {
      this->_lastMcpError = _lastMcpError | ERROR_MCP2515_CHECK_FREE_TRANSMIT_BUFFER;
      return false;
    }

    this->_lastMcpError = ERROR_MCP2515_TRANSMITBUFFER_NOT_FREE;
    return false;
  }

  uint8_t ErrorCount = 0;
  if (Extended) {
    ErrorCount = ErrorCount + (setTransmitBufferStandardIdentifierHigh(BufferNumber, (ID >> 21))) ? 0 : 1;
    ErrorCount = ErrorCount + (setTransmitBufferStandardIdentifierLow(BufferNumber, ((ID >> 18) & 0x07), Extended, ((ID >> 16) & 0x03))) ? 0 : 1;
    ErrorCount = ErrorCount + (setTransmitBufferExtendedIdentifierHigh(BufferNumber, ((ID >> 8) & 0xFF))) ? 0 : 1;
    ErrorCount = ErrorCount + (setTransmitBufferExtendedIdentifierLow(BufferNumber, (ID & 0xFF))) ? 0 : 1;
  } else {
    ErrorCount = ErrorCount + (setTransmitBufferStandardIdentifierHigh(BufferNumber, (ID >> 3))) ? 0 : 1;
    ErrorCount = ErrorCount + (setTransmitBufferStandardIdentifierLow(BufferNumber, (ID & 0x03), Extended, 0x00)) ? 0 : 1;
    ErrorCount = ErrorCount + (setTransmitBufferExtendedIdentifierHigh(BufferNumber, 0x00)) ? 0 : 1;
    ErrorCount = ErrorCount + (setTransmitBufferExtendedIdentifierLow(BufferNumber, 0x00)) ? 0 : 1;
  }

  if (ErrorCount > 0)
  {
    this->_lastMcpError = _lastMcpError | ERROR_MCP2515_TRANSMITBUFFER_ID_FILLING;
    return false;
  }

  if (RTR) {
    ErrorCount = ErrorCount + (setTransmitBufferDataLengthCode(BufferNumber, RTR, DLC)) ? 0 : 1;
  } else {
    ErrorCount = ErrorCount + (setTransmitBufferDataLengthCode(BufferNumber, RTR, DLC)) ? 0 : 1;

    for (int i = 0; i < DLC; i++) {
      ErrorCount = ErrorCount + (setTransmitBufferDataByte(BufferNumber, i, Data[i])) ? 0 : 1;
    }
  }

  if (ErrorCount > 0)
  {
    this->_lastMcpError = _lastMcpError | ERROR_MCP2515_TRANSMITBUFFER_DATA_FILLING;
    return false;
  }

  if (!resetInterruptFlag((0x02 + BufferNumber)))
  {
    this->_lastMcpError = _lastMcpError | ERROR_MCP2515_RESET_INTERRUPT_FLAG;
    return false;
  }

  return true;
}

/**
 * @brief Initiates a transmission for the given TX-Buffer
 * @param BufferNumber 0 - 2
 * @param Priority Message-Priority 0 - 3
 * @return true when success, false on any error (check _lastMcpError)
 */
bool MCP2515::sendMessage(uint8_t BufferNumber, uint8_t Priority)
{
  this->_lastMcpError = EMPTY_VALUE_16_BIT;

  if (!_isInitialized)
  {
    this->_lastMcpError = ERROR_MCP2515_NOT_INITIALIZED;
    return false;
  }

  if ((BufferNumber > 2) ||
      (Priority > 3))
  {
    this->_lastMcpError = ERROR_MCP2515_VALUE_OUTA_RANGE;
    return false;
  }

  if (!modifyTransmitBufferControl(BufferNumber, 0x0B, (0x08 | Priority)))
  {
    this->_lastMcpError = _lastMcpError | ERROR_MCP2515_INITIATE_SENDING;
    return false;
  }

  bool aborted = false;
  bool error_during_sending = false;
  uint16_t ErrorValue = EMPTY_VALUE_16_BIT;

  // While the Message Transmit Request bit is set
  while (getTransmitBufferControl(BufferNumber) & TXBnCTRL_BIT_TXREQ) {
    // Check if an error occures during the transmission
    if ((getTransmitBufferControl(BufferNumber) & TXBnCTRL_BIT_TXERR) || (_lastMcpError != EMPTY_VALUE_16_BIT)) {

      if (_lastMcpError != EMPTY_VALUE_16_BIT){
        error_during_sending = true;
        ErrorValue = ERROR_MCP2515_ERROR_DURING_SENDING;
      }
      // set aborted to true
      aborted = true;

      // In case of an error during the transmission,
      // set the "Abort All Pending Transmissions"-bit
      modifyCanControl(CANCTRL_BIT_ABAT, CANCTRL_BIT_ABAT);

      // In case of "Abort All Pending Transmissions" the while-loop
      // ended automaticaly when from the MCP2515 the Transmit Request bit is resetted.
    }

    yield();
  }

  if (aborted) {
    // when the abortion is successfull reset the "Abort All Pending Transmissions"-bit
    modifyCanControl(CANCTRL_BIT_ABAT, 0x00);
    modifyCanInterruptFlag(0x1B, 0x1B);
    this->_lastMcpError = _lastMcpError | ERROR_MCP2515_MESSAGE_SENDING_ABORTED;
    return false;
  }

  return true;
}

/**
 * @brief Check for the given Message-ID a RTR-Frame was received.
 * @param ID Message-ID
 * @param Extended bool true if it is a Extended Frame
 * @return True if a RemoteTransmissionRequest for the Message was received, False when not (or on Error check _lastMcpError)
 */
bool MCP2515::check4Rtr(uint32_t ID, bool Extended)
{
  this->_lastMcpError = EMPTY_VALUE_16_BIT;

  if (!_isInitialized)
  {
    this->_lastMcpError = ERROR_MCP2515_NOT_INITIALIZED;
    return false;
  }

  if ((ID > 0x1FFFFFFF) ||
      (Extended != false && Extended != true))
  {
    this->_lastMcpError = ERROR_MCP2515_VALUE_OUTA_RANGE;
    return false;
  }

  for (size_t i = 0; i < 2; i++)
  {
    if ((rxStatusInstruction() & (0x40 + i * 0x40)) != 0)
    {
      bool RTR = false;
      switch (i)
      {
      case 0:
        if ((getReceiveBuffer0Control() & RXBnCTRL_BIT_RXRTR) == RXBnCTRL_BIT_RXRTR)
        {
          RTR = true;
        }
        break;
      case 1:
        if ((getReceiveBuffer1Control() & RXBnCTRL_BIT_RXRTR) == RXBnCTRL_BIT_RXRTR)
        {
          RTR = true;
        }
        break;
      default:
        this->_lastMcpError = ERROR_MCP2515_UNKNOWN_SWITCH;
        return false;
        break;
      }

      if (!RTR)
      {
        continue;
      }

      if ((getReceiveBufferStandardIdentifierLow(i) & RXBnSIDL_BIT_IDE) == RXBnSIDL_BIT_IDE)
      {
        if (!Extended)
        {
            continue;
        }
      }else{
        // Because getReceiveBufferStandardIdentifierLow() returns EMPTY_VALUE_8_BIT on Error,
        // only to check here if Error occurs.
        if (_lastMcpError != EMPTY_VALUE_16_BIT)
        {
          this->_lastMcpError = _lastMcpError | ERROR_MCP2515_GET_EXTENDED_FLAG;
          return false;
        }

        if (Extended)
        {
            continue;
        }
      }

      uint32_t RTR_ID = ((getReceiveBufferStandardIdentifierHigh(i) << 3) & 0x07F8) |
                        ((getReceiveBufferStandardIdentifierLow(i) >> 5) & 0x07);

      if (_lastMcpError != EMPTY_VALUE_16_BIT)
      {
        this->_lastMcpError = _lastMcpError | ERROR_MCP2515_GET_STANDARD_ID;
        return false;
      }

      if (Extended)
      {
        RTR_ID = ((RTR_ID << 18) & 0x1FFC0000) |
                 ((((getReceiveBufferStandardIdentifierLow(i) & RXBnSIDL_BIT_EID) << 8) << 8) & 0x30000) |
                 ((getReceiveBufferExtendedIdentifierHigh(i) << 8) & 0xFF00) |
                 getReceiveBufferExtendedIdentifierLow(i);

        if (_lastMcpError != EMPTY_VALUE_16_BIT)
        {
          this->_lastMcpError = _lastMcpError | ERROR_MCP2515_GET_EXTENDED_ID;
          return false;
        }
      }

      if (ID != RTR_ID)
      {
        continue;
      }

      if (!modifyCanInterruptFlag(CANINTF_BIT_RXnIF(i), 0x00))
      {
        this->_lastMcpError = _lastMcpError | ERROR_MCP2515_RESET_FLAG;
      }

      return true;
    }
  }
  return false;
}

/**
 * @brief Check for the given Message-ID if a Message was received.
 *
 * If a Message is received it would fill the given Databuffer.
 * @param ID Message-ID
 * @param Extended bool true if it is a Extended Frame
 * @param DLC Data-Length-Code of the Message
 * @param DataBuffer Address-Pointer to the DataBuffer of the Message
 * @return True if a Message was received, False when not (or on Error check _lastMcpError)
 */
bool MCP2515::check4Receive(uint32_t ID, bool Extended, uint8_t DLC, uint8_t (&DataBuffer)[8])
{
  this->_lastMcpError = EMPTY_VALUE_16_BIT;

  if (!_isInitialized)
  {
    this->_lastMcpError = ERROR_MCP2515_NOT_INITIALIZED;
    return false;
  }

  if ((ID > 0x1FFFFFFF) ||
      (Extended != false && Extended != true) ||
      (DLC > 8))
  {
    this->_lastMcpError = ERROR_MCP2515_VALUE_OUTA_RANGE;
    return false;
  }

  for (size_t i = 0; i < 2; i++)
  {
    if ((rxStatusInstruction() & (0x40 + i * 0x40)) != 0)
    {
      if ((getReceiveBufferStandardIdentifierLow(i) & RXBnSIDL_BIT_IDE) == RXBnSIDL_BIT_IDE)
      {
        if (!Extended)
        {
            continue;
        }
      }else{
        // Because getReceiveBufferStandardIdentifierLow() returns EMPTY_VALUE_8_BIT on Error,
        // only to check here if Error occurs.
        if (_lastMcpError != EMPTY_VALUE_16_BIT)
        {
          this->_lastMcpError = _lastMcpError | ERROR_MCP2515_GET_EXTENDED_FLAG;
          return false;
        }

        if (Extended)
        {
            continue;
        }
      }

      uint32_t Message_ID = ((getReceiveBufferStandardIdentifierHigh(i) << 3) & 0x07F8) |
                        ((getReceiveBufferStandardIdentifierLow(i) >> 5) & 0x07);

      if (_lastMcpError != EMPTY_VALUE_16_BIT)
      {
        this->_lastMcpError = _lastMcpError | ERROR_MCP2515_GET_STANDARD_ID;
        return false;
      }

      if (Extended)
      {
        Message_ID = ((Message_ID << 18) & 0x1FFC0000) |
                     ((((getReceiveBufferStandardIdentifierLow(i) & RXBnSIDL_BIT_EID) << 8) << 8) & 0x30000) |
                     ((getReceiveBufferExtendedIdentifierHigh(i) << 8) & 0xFF00) |
                     getReceiveBufferExtendedIdentifierLow(i);

        if (_lastMcpError != EMPTY_VALUE_16_BIT)
        {
          this->_lastMcpError = _lastMcpError | ERROR_MCP2515_GET_EXTENDED_ID;
          return false;
        }
      }

      if (ID != Message_ID)
      {
        continue;
      }

      uint8_t MessageDLC = (getReceiveBufferDataLengthCode(i) & RXBnDLC_BIT_DLC);

      if (_lastMcpError != EMPTY_VALUE_16_BIT)
      {
        this->_lastMcpError = _lastMcpError | ERROR_MCP2515_GET_DLC;
        return false;
      }

      if (MessageDLC != DLC)
      {
        continue;
      }

      for (size_t m = 0; m < DLC; m++)
      {
        DataBuffer[m] = getReceiveBufferDataByte(i, m);

        if (_lastMcpError != EMPTY_VALUE_16_BIT)
        {
          this->_lastMcpError = _lastMcpError | ERROR_MCP2515_GET_DATA;
          return false;
        }
      }

      if (!modifyCanInterruptFlag(CANINTF_BIT_RXnIF(i), 0x00))
      {
        this->_lastMcpError = _lastMcpError | ERROR_MCP2515_RESET_FLAG;
      }

      return true;
    }
  }
  return false;
}

/**
 * @brief Get the Message-ID from the given RX-Buffer
 * @param BufferNumber 0 - 1
 * @return uint32_t Message-ID
 *
 * On Error it will return EMPTY_VALUE_32_BIT (Check _lastMcpError).
 */
uint32_t MCP2515::getIdFromReceiveBuffer(uint8_t BufferNumber)
{
  this->_lastMcpError = EMPTY_VALUE_16_BIT;

  if (!_isInitialized)
  {
    this->_lastMcpError = ERROR_MCP2515_NOT_INITIALIZED;
    return false;
  }

  if (BufferNumber > 1)
  {
    this->_lastMcpError = ERROR_MCP2515_VALUE_OUTA_RANGE;
    return EMPTY_VALUE_32_BIT;
  }

  uint32_t Message_ID = ((getReceiveBufferStandardIdentifierHigh(BufferNumber) << 3) & 0x07F8) |
                        ((getReceiveBufferStandardIdentifierLow(BufferNumber) >> 5) & 0x07);

  if (_lastMcpError != EMPTY_VALUE_16_BIT)
  {
    this->_lastMcpError = _lastMcpError | ERROR_MCP2515_GET_STANDARD_ID;
    return EMPTY_VALUE_32_BIT;
  }

  if ((getReceiveBufferStandardIdentifierLow(BufferNumber) & RXBnSIDL_BIT_IDE) == RXBnSIDL_BIT_IDE)
  {
    Message_ID = ((Message_ID << 18) & 0x1FFC0000) |
                 ((((getReceiveBufferStandardIdentifierLow(BufferNumber) & RXBnSIDL_BIT_EID) << 8) << 8) & 0x30000) |
                 ((getReceiveBufferExtendedIdentifierHigh(BufferNumber) << 8) & 0xFF00) |
                 getReceiveBufferExtendedIdentifierLow(BufferNumber);

    if (_lastMcpError != EMPTY_VALUE_16_BIT)
    {
      this->_lastMcpError = _lastMcpError | ERROR_MCP2515_GET_EXTENDED_ID;
      return EMPTY_VALUE_32_BIT;
    }
  }

  if (_lastMcpError != EMPTY_VALUE_16_BIT)
  {
    this->_lastMcpError = _lastMcpError | ERROR_MCP2515_GET_EXTENDED_FLAG;
    return EMPTY_VALUE_32_BIT;
  }

  return Message_ID;
}

/**
 * @brief Get the DLC from the given RX-Buffer
 * @param BufferNumber 0 - 1
 * @return uint8_t Message-DLC
 *
 * On Error it will return EMPTY_VALUE_8_BIT (Check _lastMcpError).
 */
uint8_t MCP2515::getDlcFromReceiveBuffer(uint8_t BufferNumber)
{
  this->_lastMcpError = EMPTY_VALUE_16_BIT;

  if (BufferNumber > 1)
  {
    this->_lastMcpError = ERROR_MCP2515_VALUE_OUTA_RANGE;
    return EMPTY_VALUE_8_BIT;
  }

  return (getReceiveBufferDataLengthCode(BufferNumber) & RXBnDLC_BIT_DLC);
}

/**
 * @brief Get the Frame from the given RX-Buffer
 * @param BufferNumber 0 - 1
 * @return uint8_t Frame
 *
 * 0 = Standard Frame
 *
 * 1 = Extended Frame
 *
 * On Error it will return EMPTY_VALUE_8_BIT (Check _lastMcpError).
 */
uint8_t MCP2515::getFrameFromReceiveBuffer(uint8_t BufferNumber)
{
  this->_lastMcpError = EMPTY_VALUE_16_BIT;

  if (BufferNumber > 1)
  {
    this->_lastMcpError = ERROR_MCP2515_VALUE_OUTA_RANGE;
    return EMPTY_VALUE_8_BIT;
  }

  if ((getReceiveBufferStandardIdentifierLow(BufferNumber) & RXBnSIDL_BIT_IDE) == RXBnSIDL_BIT_IDE)
  {
    return 1;
  }else{
    // Because getReceiveBufferStandardIdentifierLow() returns EMPTY_VALUE_8_BIT on Error,
    // only to check here if Error occurs.
    if (_lastMcpError != EMPTY_VALUE_16_BIT)
    {
      this->_lastMcpError = _lastMcpError | ERROR_MCP2515_GET_EXTENDED_FLAG;
    }

    return 0;
  }
}

/**
 * @brief Get the Remote-Transmission-Request-Bit from the given RX-Buffer
 * @param BufferNumber 0 - 1
 * @return uint8_t RTR
 *
 * 0 = Message is not a Remote-Transmission-Request
 *
 * 1 = Message is a Remote-Transmission-Request
 *
 * On Error it will return EMPTY_VALUE_8_BIT (Check _lastMcpError).
 */
uint8_t MCP2515::getRtrFromReceiveBuffer(uint8_t BufferNumber)
{
  this->_lastMcpError = EMPTY_VALUE_16_BIT;

  if (BufferNumber > 1)
  {
    this->_lastMcpError = ERROR_MCP2515_VALUE_OUTA_RANGE;
    return EMPTY_VALUE_8_BIT;
  }

  switch (BufferNumber)
  {
  case 0:
    return ((getReceiveBuffer0Control() & RXBnCTRL_BIT_RXRTR) == RXBnCTRL_BIT_RXRTR) ? 1 : 0;
    break;
  case 1:
    return ((getReceiveBuffer1Control() & RXBnCTRL_BIT_RXRTR) == RXBnCTRL_BIT_RXRTR) ? 1 : 0;
    break;
  default:
    this->_lastMcpError = ERROR_MCP2515_UNKNOWN_SWITCH;
    return EMPTY_VALUE_8_BIT;
    break;
  }
  return EMPTY_VALUE_8_BIT;
}

/**
 * @brief Get the Data from the given RX-Buffer
 * @param BufferNumber 0 - 1
 * @param DLC expected Data Length Code
 * @param DataBuffer Message-Data-Buffer which has to be filled
 * @return true when success, false on any error (check _lastMcpError)
 */
bool MCP2515::getDataFromReceiveBuffer(uint8_t BufferNumber, uint8_t DLC, uint8_t (&DataBuffer)[8])
{
  this->_lastMcpError = EMPTY_VALUE_16_BIT;

  if (!_isInitialized)
  {
    this->_lastMcpError = ERROR_MCP2515_NOT_INITIALIZED;
    return false;
  }

  if (BufferNumber > 1 || DLC > 8)
  {
    this->_lastMcpError = ERROR_MCP2515_VALUE_OUTA_RANGE;
    return false;
  }

  for (size_t m = 0; m < DLC; m++)
  {
    DataBuffer[m] = getReceiveBufferDataByte(BufferNumber, m);

    if (_lastMcpError != EMPTY_VALUE_16_BIT)
    {
      this->_lastMcpError = _lastMcpError | ERROR_MCP2515_GET_DATA;
      return false;
    }
  }

  return true;
}

/**
 * @brief Get all Information from the given RX-Buffer.
 *
 * When all Information are collected it would be release the given RX-Buffer.
 * @param BufferNumber 0 - 1
 * @param ID Message-ID Variable which has to be filled
 * @param Frame Frame Variable which has to be filled (true if Extended Frame, false if Standard Frame)
 * @param RTR RTR Variable which has to be filled (true if RTR, false if not)
 * @param DLC DLC Variable which has to be filled
 * @param DataBuffer Message-Data-Buffer which has to be filled
 * @return true when success, false on any error (check _lastMcpError)
 */
bool MCP2515::getAllFromReceiveBuffer(uint8_t BufferNumber, uint32_t (&ID), bool (&Frame), bool (&RTR), uint8_t (&DLC), uint8_t (&DataBuffer)[8])
{
  this->_lastMcpError = EMPTY_VALUE_16_BIT;

  if (!_isInitialized)
  {
    this->_lastMcpError = ERROR_MCP2515_NOT_INITIALIZED;
    return false;
  }

  if (BufferNumber > 1)
  {
    this->_lastMcpError = ERROR_MCP2515_VALUE_OUTA_RANGE;
    return false;
  }

  // Collect Data from the given RX-Buffer
  uint8_t Data_RX_Controller = (BufferNumber == 0) ? getReceiveBuffer0Control() : getReceiveBuffer1Control();
  uint8_t Data_Standard_High = getReceiveBufferStandardIdentifierHigh(BufferNumber);
  uint8_t Data_Standard_Low = getReceiveBufferStandardIdentifierLow(BufferNumber);
  uint8_t Data_Extended_High = getReceiveBufferExtendedIdentifierHigh(BufferNumber);
  uint8_t Data_Extended_Low = getReceiveBufferExtendedIdentifierLow(BufferNumber);
  uint8_t Data_DLC = getReceiveBufferDataLengthCode(BufferNumber);
  uint8_t Data_Bytes[8];

  for (size_t i = 0; i < 8; i++)
  {
    Data_Bytes[i] = getReceiveBufferDataByte(BufferNumber, i);
  }

  if (_lastMcpError != EMPTY_VALUE_16_BIT)
  {
    this->_lastMcpError = _lastMcpError | ERROR_MCP2515_GET_ALL_DATA;
    return false;
  }

  if (!modifyCanInterruptFlag(CANINTF_BIT_RXnIF(BufferNumber), 0x00))
  {
    this->_lastMcpError = _lastMcpError | ERROR_MCP2515_RESET_FLAG;
  }

  bool RTR_Message = ((Data_RX_Controller & RXBnCTRL_BIT_RXRTR) == RXBnCTRL_BIT_RXRTR) ? true : false;
  bool Extended_Frame = ((Data_Standard_Low & RXBnSIDL_BIT_IDE) == RXBnSIDL_BIT_IDE) ? true : false;

  uint32_t ID_Message = ((Data_Standard_High << 3) & 0x07F8) |
                        ((Data_Standard_Low >> 5) & 0x07);

  if (Extended_Frame)
  {
    ID_Message = ((ID_Message << 18) & 0x1FFC0000) |
                 ((((Data_Standard_Low & RXBnSIDL_BIT_EID) << 8) << 8) & 0x30000) |
                 ((Data_Extended_High << 8) & 0xFF00) |
                 Data_Extended_Low;
  }

  // Fill given Variables
  ID = ID_Message;
  Frame = Extended_Frame;
  RTR = RTR_Message;

  if (RTR_Message)
  {
    DLC = 0;
    for (size_t m = 0; m < 8; m++)
    {
      DataBuffer[m] = 0;
    }
  } else {
    DLC = Data_DLC & RXBnDLC_BIT_DLC;
    for (size_t m = 0; m < DLC; m++)
    {
      DataBuffer[m] = Data_Bytes[m];
    }
  }

  return true;
}

/**
 * @brief Release the given RX-Buffer
 * @param BufferNumber 0 - 1
 * @return true when success, false on any error (check _lastMcpError)
 */
bool MCP2515::releaseReceiveBuffer(uint8_t BufferNumber)
{
  this->_lastMcpError = EMPTY_VALUE_16_BIT;

  if (!_isInitialized)
  {
    this->_lastMcpError = ERROR_MCP2515_NOT_INITIALIZED;
    return false;
  }

  if (BufferNumber > 1)
  {
    this->_lastMcpError = ERROR_MCP2515_VALUE_OUTA_RANGE;
    return false;
  }

  return modifyCanInterruptFlag(CANINTF_BIT_RXnIF(BufferNumber), 0x00);
}

/**
 * @brief Check all Interrupts
 * @return Bitwise
 * Bit 7 -> MERRF - Message Error Interrupt Flag bit
 *
 * 1 = Interrupt is pending (must be cleared by MCU to reset the interrupt condition)
 *
 * 0 = No interrupt is pending
 *
 * Bit 6 -> WAKIF - Wake-up Interrupt Flag bit
 *
 * 1 = Interrupt is pending (must be cleared by MCU to reset the interrupt condition)
 *
 * 0 = No interrupt is pending
 *
 * Bit 5 -> ERRIF - Error Interrupt Flag bit (multiple sources in EFLG register)
 *
 * 1 = Interrupt is pending (must be cleared by MCU to reset the interrupt condition)
 *
 * 0 = No interrupt is pending
 *
 * Bit 4 -> TX2IF - Transmit Buffer 2 Empty Interrupt Flag bit
 *
 * 1 = Interrupt is pending (must be cleared by MCU to reset the interrupt condition)
 *
 * 0 = No interrupt is pending
 *
 * Bit 3 -> TX1IF - Transmit Buffer 1 Empty Interrupt Flag bit
 *
 * 1 = Interrupt is pending (must be cleared by MCU to reset the interrupt condition)
 *
 * 0 = No interrupt is pending
 *
 * Bit 2 -> TX0IF - Transmit Buffer 0 Empty Interrupt Flag bit
 *
 * 1 = Interrupt is pending (must be cleared by MCU to reset the interrupt condition)
 *
 * 0 = No interrupt is pending
 *
 * Bit 1 -> RX1IF - Receive Buffer 1 Full Interrupt Flag bit
 *
 * 1 = Interrupt is pending (must be cleared by MCU to reset the interrupt condition)
 *
 * 0 = No interrupt is pending
 *
 * Bit 0 -> RX0IF - Receive Buffer 0 Full Interrupt Flag bit
 *
 * 1 = Interrupt is pending (must be cleared by MCU to reset the interrupt condition)
 *
 * 0 = No interrupt is pending
 *
 * On Error it will return EMPTY_VALUE_8_BIT (Check _lastMcpError).
 */
uint8_t MCP2515::check4InterruptFlags()
{
  this->_lastMcpError = EMPTY_VALUE_16_BIT;

  if (!_isInitialized)
  {
    this->_lastMcpError = ERROR_MCP2515_NOT_INITIALIZED;
    return EMPTY_VALUE_8_BIT;
  }

  return getCanInterruptFlag();
}

/**
 * @brief Resets the given Interrupt-Flag (only)
 * @param Flag
 * 7 = MERRF - Message Error Interrupt Flag
 *
 * 6 = WAKIF - Wake-up Interrupt Flag
 *
 * 5 = ERRIF - Error Interrupt Flag
 *
 * 4 = TX2IF - Transmit Buffer 2 Empty Interrupt Flag
 *
 * 3 = TX1IF - Transmit Buffer 1 Empty Interrupt Flag
 *
 * 2 = TX0IF - Transmit Buffer 0 Empty Interrupt Flag
 *
 * 1 = RX1IF - Receive Buffer 1 Full Interrupt Flag
 *
 * 0 = RX0IF - Receive Buffer 0 Full Interrupt Flag
 * @return true when success, false on any error (check _lastMcpError)
 */
bool MCP2515::resetInterruptFlag(uint8_t Flag)
{
  this->_lastMcpError = EMPTY_VALUE_16_BIT;

  if (!_isInitialized)
  {
    this->_lastMcpError = ERROR_MCP2515_NOT_INITIALIZED;
    return EMPTY_VALUE_8_BIT;
  }

  if (Flag > 7)
  {
    this->_lastMcpError = ERROR_MCP2515_VALUE_OUTA_RANGE;
    return false;
  }

  return modifyCanInterruptFlag((0x01 << Flag), 0x00);
}

/**
 * @brief Sets the given Interrupt-Flag (only)
 * @param Flag
 * 7 = MERRF - Message Error Interrupt Flag
 *
 * 6 = WAKIF - Wake-up Interrupt Flag
 *
 * 5 = ERRIF - Error Interrupt Flag
 *
 * 4 = TX2IF - Transmit Buffer 2 Empty Interrupt Flag
 *
 * 3 = TX1IF - Transmit Buffer 1 Empty Interrupt Flag
 *
 * 2 = TX0IF - Transmit Buffer 0 Empty Interrupt Flag
 *
 * 1 = RX1IF - Receive Buffer 1 Full Interrupt Flag
 *
 * 0 = RX0IF - Receive Buffer 0 Full Interrupt Flag
 * @return true when success, false on any error (check _lastMcpError)
 */
bool MCP2515::setInterruptFlag(uint8_t Flag)
{
  this->_lastMcpError = EMPTY_VALUE_16_BIT;

  if (!_isInitialized)
  {
    this->_lastMcpError = ERROR_MCP2515_NOT_INITIALIZED;
    return EMPTY_VALUE_8_BIT;
  }

  if (Flag > 7)
  {
    this->_lastMcpError = ERROR_MCP2515_VALUE_OUTA_RANGE;
    return false;
  }

  return modifyCanInterruptFlag((0x01 << Flag), (0x01 << Flag));
}

/**
 * @brief Get the choosen ChipSelect-Pin
 * @return uint8_t Number of the CS-Pin
 */
uint8_t MCP2515::getCsPin()
{
  return _csPin;
}

/**
 * @brief Get the SPI-Mode
 * @return uint8_t
 *
 * 0 = SPI-Mode 0
 *
 * 4 = SPI-Mode 1
 *
 * 8 = SPI-Mode 2
 *
 * 12 = SPI-Mode 3
 * @note The MCP2515 only allows SPI-Mode 0 and 3
 */
uint8_t MCP2515::getSpiMode()
{
  return _spiMode;
}

/**
 * @brief Get the DataOrder for the SPI-Communication
 * @return uint8_t
 *
 * 0 = LSB-First
 *
 * 1 = MSB-First
 * @note The MCP2515 only allows MSB-First
 */
uint8_t MCP2515::getDataOrder()
{
  return _dataOrder;
}

/**
 * @brief Get the SPI-Frequency
 * @return uint32_t Value of the SPI-Frequency in Hz
 * @note The MCP2515 only allows SPI-Frequencys up to 10MHz
 */
uint32_t MCP2515::getFrequency()
{
  return _spiFrequency;
}

/**
 * @brief Get the Clock-Frequency of the MCP2515
 * @return uint32_t Value of the Clock-Frequency in Hz
 * @note The MCP2515 only allows Clock-Frequencys up to 40MHz
 */
uint32_t MCP2515::getClockFrequency()
{
  return _clockFrequency;
}

/**
 * @brief Get the Operation-Mode of the MCP2515
 * @return uint8_t
 *
 * 0 = Normal-Mode
 *
 * 1 = Sleep-Mode
 *
 * 2 = Loopback-Mode
 *
 * 3 = Listen-Only-Mode
 *
 * 4 = Configuration-Mode
 */
uint8_t MCP2515::getOperationMode()
{
  return _operationMode;
}

/**
 * @brief Get the Baudrate of the CAN-Communication
 * @return uint32_t Value of the Baudrate in Bd (Baud)
 */
uint32_t MCP2515::getBaudRate()
{
  return _baudRate;
}

/**
 * @brief Get the Initialisation-Status of the MCP2515
 * @return bool
 *
 * True = MCP2515 is initialized
 *
 * False = MCP2515 is not initialized
 */
bool MCP2515::getIsInitialized()
{
  return _isInitialized;
}

/**
 * @brief Get the Recheck-Status of the MCP2515
 * @return bool
 *
 * True = ReCheck is enabled
 *
 * False = ReCheck is disabled
 */
bool MCP2515::getReCheckEnabled()
{
  return _reCheckEnabled;
}
