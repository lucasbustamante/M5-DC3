#include <M5StickCPlus.h>
#include <driver/i2s.h>
#include <HardwareSerial.h>
#include <arduinoFFT.h> 

arduinoFFT FFT = arduinoFFT();

#define PIN_CLK     0
#define PIN_DATA    34
#define READ_LEN    (2 * 256 * 8)
#define GAIN_FACTOR 0
uint8_t BUFFER[READ_LEN] = {0};

#define SAMPLING_FREQUENCY (44100)

float oldy[20];
int16_t *adcBuffer = NULL;

#define SCREEN_WIDTH 135
#define SCREEN_HEIGHT 240

/*
  https://pages.mtu.edu/~suits/notefreqs.html
*/

struct Note {
  char chr[4];
  double freq;
} notes[] = {
  { "E 4", 329.00 },
  { "B 3", 246.50},
  { "G 3", 195.50},
  { "D 3", 148.00},
  { "A 2", 109.70 },
  { "E 2", 82.41 },

  { "A 3", 220.00},
  { "E 3", 164.81},
  //{ "D 4", 293.66 },
  //{ "D#4", 311.13 },
  //{ "F 4", 349.23 },
  //{ "F#4", 369.99 },
  //{ "G 4", 392.0 },
  //{ "G#4", 415.30 },
  //{ "A 4", 440.0 },
  //{ "A#4", 466.13 },
  //{ "B 4", 493.88 },
  //{ "C 5", 523.25 },
  //{ "C#5", 554.37 },
  //{ "D 5", 587.33 },
  //{ "D#5", 622.25 },
  //{ "E 5", 659.25 },
  //{ "F 5", 698.46 },
  //{ "F#5", 739.99 },
  //{ "G 5", 783.99 },
  //{ "G#5", 830.61 },
  //{ "A 5", 880.00 },
  //{ "A#5", 932.33 },
  //{ "B 5", 987.77 },
  //{ "C 6", 1046.50 },
  //{ "C#6", 1108.73 },
  //{ "D 6", 1174.66 },
  //{ "D#6", 1244.51 },
  //{ "E 6", 1318.81 }
};

void i2sInit(){
      Serial.println("I2sInit");
  i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX |
    I2S_MODE_PDM),  // Set the I2S operating mode.

    .sample_rate = SAMPLING_FREQUENCY,  // Set the I2S sampling rate.
    .bits_per_sample =
    I2S_BITS_PER_SAMPLE_16BIT,  // Fixed 12-bit stereo MSB.

    .channel_format =
    I2S_CHANNEL_FMT_ALL_RIGHT,  // Set the channel format.
#if ESP_IDF_VERSION > ESP_IDF_VERSION_VAL(4, 1, 0)
    .communication_format =
    I2S_COMM_FORMAT_STAND_I2S,  // Set the format of the communication.
#else
    .communication_format = I2S_COMM_FORMAT_I2S,
#endif
    .intr_alloc_flags =
    ESP_INTR_FLAG_LEVEL1,  // Set the interrupt flag.
    .dma_buf_count = 2,        // DMA buffer count.
    .dma_buf_len   = 128,      // DMA buffer length.
  };

  i2s_pin_config_t pin_config;

#if (ESP_IDF_VERSION > ESP_IDF_VERSION_VAL(4, 3, 0))
  pin_config.mck_io_num = I2S_PIN_NO_CHANGE;
#endif

  pin_config.bck_io_num   = I2S_PIN_NO_CHANGE;
  pin_config.ws_io_num    = PIN_CLK;
  pin_config.data_out_num = I2S_PIN_NO_CHANGE;
  pin_config.data_in_num  = PIN_DATA;

  i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
  i2s_set_pin(I2S_NUM_0, &pin_config);
  i2s_set_clk(I2S_NUM_0, SAMPLING_FREQUENCY, I2S_BITS_PER_SAMPLE_16BIT, I2S_CHANNEL_MONO);
}

double vReal[READ_LEN / 2];
double vImag[READ_LEN / 2];

void mic_record_task(void *arg) {
  Serial.println("MIC");
  size_t bytesread;
  while (1) {
    i2s_read(I2S_NUM_0, (char *)BUFFER, READ_LEN, &bytesread, 
             (100 / portTICK_RATE_MS));
    adcBuffer = (int16_t *)BUFFER;

    for (int i = 0; i < READ_LEN / 2; i++) {
      vReal[i] = adcBuffer[i];
      vImag[i] = 0;
    }
    FFT.DCRemoval(vReal, READ_LEN / 2);
    FFT.Windowing(vReal, READ_LEN / 2, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
    FFT.Compute(vReal, vImag, READ_LEN / 2, FFT_FORWARD);
    FFT.ComplexToMagnitude(vReal, vImag, READ_LEN / 2);
    double peakf;
    double peakv;
    //Serial.println(peakf);
    FFT.MajorPeak(vReal, READ_LEN / 2, SAMPLING_FREQUENCY, &peakf, &peakv);
    int i;
    bool found = false;
    for (i = 0; i < sizeof(notes) / sizeof(struct Note); i++) {
      if (abs(peakf - notes[i].freq) < 14) {
        found = true;
        break;
      }
    }

    M5.Lcd.setCursor(8, 30);
    M5.Lcd.setTextSize(8);

    if (found == true) {

      uint16_t color = BLACK;

      if (abs(peakf - notes[i].freq) < 1 ){
          Serial.println("green");
        color = GREEN;
      } else if (peakf < notes[i].freq) {
          Serial.println("orange");
        color = ORANGE;
      } else if (peakf > notes[i].freq) {
          Serial.println("red");
        color = RED;
      }

      M5.Lcd.setTextColor(color, BLACK);
      M5.Lcd.println(
        notes[i].chr
      );
      M5.Lcd.setTextSize(2);
      M5.Lcd.printf("  %5.0fHz\n", peakf);

      for (int j = 0; j < 20; j++) {
        M5.Lcd.fillEllipse(SCREEN_WIDTH / 2 + oldy[j] * 10, SCREEN_HEIGHT / 2 + j * 5, 10 - j / 2, 10 - j / 2, BLACK);
      }

      for (int j = 19; j > 0; j--) {
        oldy[j] = oldy[j - 1];
      }

      oldy[0] = (peakf - notes[i].freq);

      for (int j = 0; j < 20; j++) {
        M5.Lcd.fillEllipse(SCREEN_WIDTH / 2 + oldy[j] * 10, SCREEN_HEIGHT / 2 + j * 5, 10 - j / 2, 10 - j / 2, color);
      }

      M5.Lcd.drawLine(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, SCREEN_WIDTH / 2, SCREEN_HEIGHT, color);

     
    }
    vTaskDelay(10 / portTICK_RATE_MS);
  }
}