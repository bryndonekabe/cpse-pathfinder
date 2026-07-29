#include "../include/config.hpp"
#include "../include/no_respect_raw.hpp"
#include <Arduino.h>

void speaker_setup() {
  // Configure the I2S driver settings
  i2s_config_t i2s_config = {
      .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
      .sample_rate = 16000,
      .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
      .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
      .communication_format = I2S_COMM_FORMAT_STAND_I2S,
      .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
      .dma_buf_count = 8,
      .dma_buf_len = 512,
      .use_apll = true,
      .tx_desc_auto_clear = true,
  };

  // Configure the physical GPIO pin layout
  i2s_pin_config_t pin_config = {.bck_io_num = I2S_BCLK_PIN,
                                 .ws_io_num = I2S_LRC_PIN,
                                 .data_out_num = I2S_DIN_PIN,
                                 .data_in_num = I2S_PIN_NO_CHANGE};

  // Install and start the I2S driver
  i2s_driver_install(I2S_PORT, &i2s_config, 0, NULL);
  i2s_set_pin(I2S_PORT, &pin_config);
}

void playPCM(const unsigned char *ptr, const size_t file_len) {
  int chunk = 512;

  for (size_t i = 0; i < file_len; i += chunk) {
    size_t len = min((size_t)chunk, file_len - i);

    uint8_t buffer[len];

    memcpy_P(buffer, ptr + i, len);

    size_t written;

    i2s_write(I2S_NUM_0, buffer, len, &written, portMAX_DELAY);
  }
}

void speaker_loop() {
  // Generate a continuous raw audio buffer (sine wave tone)
  // int16_t samples[128];
  // for (int i = 0; i < 128; i++) {
  //   // Generate a simple math-based tone
  //   samples[i] = (int16_t)(3000 * sin(i * 2 * PI / 32));
  // }

  playPCM(no_respect_raw, no_respect_raw_len);
  // size_t bytes_written;
  // // Push the data out to the MAX98357A chip
  // i2s_write(I2S_PORT, samples, sizeof(samples), &bytes_written,
  // portMAX_DELAY);
}
