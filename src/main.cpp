#include <DHT.h>
#include <DHT_U.h>
#include <cstdio>
#include <cstring>
#include <hardware/adc.h>
#include <lwip/ip4_addr.h>
#include <lwip/netif.h>
#include <pico/cyw43_arch.h>
#include <pico/stdio.h>
#include <pico/time.h>
#include <src/RtcDS1302.h>

// Pinos módulos

constexpr uint8_t PIN_DHT11_DATA = 2;

constexpr uint8_t PIN_RTC_DAT = PICO_DEFAULT_I2C_SDA_PIN;
constexpr uint8_t PIN_RTC_CLK = PICO_DEFAULT_I2C_SCL_PIN;
constexpr uint8_t PIN_RTC_RST = 6;

constexpr uint8_t PIN_R_LED = 18;
constexpr uint8_t PIN_G_LED = 19;
constexpr uint8_t PIN_B_LED = 20;

constexpr uint8_t PIN_LDR = 26;

constexpr const char* ssid = "YOUR_SSID";
constexpr const char* password = "";

const uint16_t ANALOG_RES = 1U << 12U;

void getNetworkList();
void connectToInternet();
void readIncidentLight();
void readTemperatureAndHumidity();
void printDateTime(const RtcDateTime& dateTime);
void performPrintDateTime();
void scan_worker_fn(async_context_t* context, async_at_time_worker_t* worker);
const char* readStringUntilNewLine();

DHT_Unified dht(PIN_DHT11_DATA, DHT11);
ThreeWire myWire(PIN_RTC_DAT, PIN_RTC_CLK, PIN_RTC_RST); // IO, SCLK, CE
RtcDS1302 RTC(myWire);

int main() {
  uint32_t delayMS = 0;

  stdio_init_all();
  sleep_ms(10000);

  if (cyw43_arch_init() != 0) {
    printf("failed to initialise\n");
    return 1;
  }

  cyw43_arch_enable_sta_mode();

  // Start a scan immediately
  bool scan_started = false;
  async_at_time_worker_t scan_worker = { .do_work = scan_worker_fn, .user_data = &scan_started };
  hard_assert(async_context_add_at_time_worker_in_ms(cyw43_arch_async_context(), &scan_worker, 0));

  adc_init();
  adc_gpio_init(PIN_LDR);

  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  delayMS = sensor.min_delay / 1000;

  printf("Data compilado: %s\n", __DATE__);
  printf("Horário compilado: %s\n", __TIME__);

  RTC.Begin();

  auto compiled = RtcDateTime(__DATE__, __TIME__);
  printDateTime(compiled);
  printf("\n");

  if (!RTC.IsDateTimeValid()) {
    // Common Causes:
    //    1) first time you ran and the device wasn't running yet
    //    2) the battery on the device is low or even missing

    printf("RTC lost confidence in the DateTime!\n");
    RTC.SetDateTime(compiled);
    printf("Set new DateTime\n");
  }

  if (RTC.GetIsWriteProtected()) {
    printf("RTC was write protected, enabling writing now\n");
    RTC.SetIsWriteProtected(false);
  }

  if (!RTC.GetIsRunning()) {
    printf("RTC was not actively running, starting now\n");
    RTC.SetIsRunning(true);
  }

  if (RtcDateTime now = RTC.GetDateTime(); now < compiled) {
    printf("RTC is older than compile time!  (Updating DateTime)\n");
    RTC.SetDateTime(compiled);
  } else if (now > compiled) {
    printf("RTC is newer than compile time. (this is expected)\n");
  } else if (now == compiled) {
    printf("RTC is the same as compile time! (not expected but all is fine)\n");
  }

  while (true) {
    static uint32_t lastTime = 0;
    if (!cyw43_wifi_scan_active(&cyw43_state) && scan_started) {
      // Start a scan in 10s
      scan_started = false;
      hard_assert(async_context_add_at_time_worker_in_ms(cyw43_arch_async_context(), &scan_worker, 10000));
    }
    cyw43_arch_poll();
    cyw43_arch_wait_for_work_until(at_the_end_of_time);
    if (to_ms_since_boot(get_absolute_time()) - lastTime >= delayMS) {
      performPrintDateTime();
      readTemperatureAndHumidity();
      lastTime = to_ms_since_boot(get_absolute_time());
    }
    readIncidentLight();
    sleep_ms(1000);
  }
}

int scan_result(void* /*env*/, const cyw43_ev_scan_result_t* result) {
  if (result != nullptr) {
    printf("ssid: %-32s rssi: %4d chan: %3d mac: %02x:%02x:%02x:%02x:%02x:%02x sec: %u\n", result->ssid, result->rssi, result->channel,
           result->bssid[0], result->bssid[1], result->bssid[2], result->bssid[3], result->bssid[4], result->bssid[5], result->auth_mode);
  }
  return 0;
}

// Start a wifi scan
void scan_worker_fn(async_context_t* /*context*/, async_at_time_worker_t* worker) {
  cyw43_wifi_scan_options_t scan_options = { .version = 0 };
  int err = cyw43_wifi_scan(&cyw43_state, &scan_options, nullptr, scan_result);
  if (err == 0) {
    auto* scan_started = static_cast<bool*>(worker->user_data);
    *scan_started = true;
    printf("Iniciando o escaneamento em %lu\n", to_ms_since_boot(get_absolute_time()));
  } else {
    printf("Failed to start scan: %d\n", err);
  }
}

void connectToInternet() {
  getNetworkList();

  printf("Escreva o nome da internet para conectar:");
  if (strcmp(readStringUntilNewLine(), "") != 0) {
    printf("SSID não foi digitada, usando da programação\n");
  }

  printf("Agora a senha da internet:");
  if (strcmp(readStringUntilNewLine(), "") != 0) {
    printf("Senha não foi digitada, usando da programação\n");
  }

  printf("O nome do wifi é %s e sua senha do wifi é %s.\n", ssid, password);

  printf("Conectando");
  cyw43_arch_enable_sta_mode();
  printf("Connecting to Wi-Fi... (press 'd' to disconnect)\n");
  if (cyw43_arch_wifi_connect_timeout_ms(ssid, password, CYW43_AUTH_WPA2_AES_PSK, 30000) != 0) {
    printf("failed to connect.\n");
    return;
  }

  putchar('\n');

  if (netif_default != nullptr) {
    printf("Conectado, IP address: %s\n", ip4addr_ntoa(&netif_default->ip_addr));
  }
}

const char* readStringUntilNewLine() {

  static char buffer[32];
  static uint8_t index = 0;
  const char* string = "";

  int chr = getchar_timeout_us(0);

  if (chr == PICO_ERROR_TIMEOUT) {
    return string;
  }

  if (chr == '\r' || chr == '\n') {

    buffer[index] = '\0';

    string = buffer;

    index = 0;
    (void)fflush(stdout);

    return string;
  }

  if (index < sizeof(buffer) - 1) {
    buffer[index++] = static_cast<char>(chr);
  }
  return string;
}

void printDateTime(const RtcDateTime& dateTime) {
  printf("%02u/%02u/%04u %02u:%02u:%02u", dateTime.Month(), dateTime.Day(), dateTime.Year(), dateTime.Hour(), dateTime.Minute(), dateTime.Second());
}

void performPrintDateTime() {
  RtcDateTime now = RTC.GetDateTime();

  printDateTime(now);
  putchar('\n');

  if (!now.IsValid()) {
    // Common Causes:
    //    1) the battery on the device is low or even missing and the power line was disconnected
    printf("RTC lost confidence in the DateTime!\n");
  }
}

void readTemperatureAndHumidity() {
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnanf(event.temperature) != 0) {
    printf("Error reading temperature!\n");
  } else {
    printf("Temperature: %.2f °C\n", event.temperature);
  }

  dht.humidity().getEvent(&event);
  if (isnanf(event.relative_humidity) != 0) {
    printf("Error reading humidity!\n");
  } else {
    printf("Humidity: %.2f %%\n", event.relative_humidity);
  }
}

template <typename T, typename U> U map(T value, T in_min, T in_max, U out_min, U out_max) {
  return static_cast<U>(((value - in_min) * (out_max - out_min) / (in_max - in_min)) + out_min);
}

void readIncidentLight() {
  const uint16_t OFFSET_VAL = 100;
  const uint8_t MAX_PERCENT = 100;

  adc_select_input(PIN_LDR - ADC_BASE_PIN);
  uint16_t adcRead = adc_read();
  uint32_t lux = map(adcRead, OFFSET_VAL, static_cast<uint16_t>(ANALOG_RES - 1), static_cast<uint8_t>(0), MAX_PERCENT);
  printf("A quantidade de luz no ambiente é: %lu lm\n", lux);
}
