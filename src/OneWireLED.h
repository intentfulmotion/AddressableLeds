#pragma once
#include "AddressableLED.h"

#include <esp_intr_alloc.h>
#include <driver/gpio.h>
#include <freertos/semphr.h>
#include <soc/gpio_sig_map.h>
#include <soc/dport_reg.h>
#include <soc/rmt_struct.h>
#include <driver/rmt.h>
#include "esp_log.h"

struct Timing {
  rmt_item32_t bit0;
  rmt_item32_t bit1;
  uint32_t reset;
};

class OneWireLED : public AddressableLED {
  LEDType _type;
  uint8_t _pin;
  rmt_channel_t _channel;
  LEDTimingParameters _ledParameters;
  intr_handle_t _interruptHandle;
  Timing _timing;

  uint16_t _pixelPosition;
  uint16_t _componentPosition;
  uint16_t _halfIdx;

  static void initChannel(uint8_t channel);
  static bool anyAlive();
  static OneWireLED*& IRAM_ATTR ledForChannel(rmt_channel_t channel);
  static void IRAM_ATTR interruptHandler(void *);
  void IRAM_ATTR copyRmtHalfBlock();
  void startTransmission();
  void pixelToRaw(Rgb *pixel, uint16_t index);

  static uint32_t _clock;

  static void IRAM_ATTR translateToRMT(const void *src, rmt_item32_t *dest, 
    size_t src_size, size_t wanted_num, size_t *translated_size, size_t *item_num, void *context);

  public:
    OneWireLED(LEDType type, uint8_t pin, uint8_t channel, uint16_t count, PixelOrder pixelOrder = PixelOrder::GRB);
    ~OneWireLED();

    bool wait(uint32_t timeout = portMAX_DELAY);
};