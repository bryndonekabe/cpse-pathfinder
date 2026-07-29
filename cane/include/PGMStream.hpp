#pragma once
#include <Arduino.h>
#include <Stream.h>
#include <pgmspace.h>

class PGMStream : public Stream {
  const uint8_t *_data;
  size_t _len;
  size_t _pos;

public:
  PGMStream(const uint8_t *data, size_t len)
      : _data(data), _len(len), _pos(0) {}

  size_t write(uint8_t) override {
    return 0; // read-only stream
  }
  size_t write(const uint8_t *buffer, size_t size) override {

    return 0; // read-only stream
  }
  int available() override { return _len - _pos; }
  int read() override {
    if (_pos >= _len)
      return -1;

    return pgm_read_byte(&_data[_pos++]);
  }
  int peek() override {
    if (_pos >= _len)
      return -1;

    return pgm_read_byte(&_data[_pos]);
  }
  void flush() override { _pos = 0; }
  void rewind() { _pos = 0; }
};
