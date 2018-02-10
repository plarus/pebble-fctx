#pragma once

/**
 * Decode the next byte of a UTF-8 byte stream.
 * Initialize state to 0 the before calling this function for the first
 * time for a given stream.  If the returned value is 0, then cp has been
 * set to a valid code point.  Other return values indicate that a multi-
 * byte sequence is in progress, or there was a decoding error.
 *
 * @param byte the byte to decode.
 * @state the current state of the decoder.
 * @cp the decoded unitcode code point.
 * @return the state of the decode process after decoding the byte.
 */
uint16_t utf8_decode_byte(uint8_t byte, uint16_t* state, uint16_t* cp);
