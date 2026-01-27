#include <WiFi.h>
#include <DHT.h>
#include <DHT_U.h>

// Pinos módulos

#define PIN_DHT11_DATA 2

#define PIN_RTC_DAT 4
#define PIN_RTC_CLK 5
#define PIN_RTC_RST 6

#define PIN_R_LED 18
#define PIN_G_LED 19
#define PIN_B_LED 20

#define PIN_LDR 26

const char* macToString(const uint8_t mac[6]);
const char* encToString(uint8_t enc);
void getNetworkList();
void connectToInternet();
void readIncidentLight();
void readTemperatureAndHumidity();

const String ssid = "Apartamento 502";
const String password = "cobalto01";

DHT_Unified dht(PIN_DHT11_DATA, DHT11);

uint32_t delayMS;

void setup() {
  Serial.begin(115200);
  delay(10000);
  // connectToInternet();
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  delayMS = sensor.min_delay / 1000;
}

void loop() {
  static uint32_t lastTime = 0;
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Você está conectado!");
  }
  if (millis() - lastTime >= delayMS) {
    readTemperatureAndHumidity();
    lastTime = millis();
  }
  readIncidentLight();
  delay(1000);
}

void getNetworkList() {
  delay(2000);
  Serial.printf("Iniciando o escaneamento em %lu\n", millis());
  auto cnt = WiFi.scanNetworks();
  while (!cnt) {
    Serial.printf("Nenhuma rede encontrada\n");
  }

  Serial.printf("Encontrado %d redes\n\n", cnt);
  Serial.printf("%32s %5s %17s %2s %4s\n", "SSID", "ENC", "BSSID        ", "CH", "RSSI");
  for (uint8_t i = 0; i < cnt; i++) {
    uint8_t bssid[6];
    WiFi.BSSID(i, bssid);
    Serial.printf("%32s %5s %17s %2d %4ld\n", WiFi.SSID(i), encToString(WiFi.encryptionType(i)), macToString(bssid), WiFi.channel(i), WiFi.RSSI(i));
  }

  delay(2000);
}

void connectToInternet() {
  getNetworkList();

  Serial.print("Escreva o nome da internet para conectar:");
  while (Serial.available() == 0);
  if (String inputSSID = Serial.readStringUntil('\n'); inputSSID == "") {
    Serial.println("SSID não foi digitada, usando da programação");
  }

  Serial.print("Agora a senha da internet:");
  while (Serial.available() == 0);
  if (String inputPASSWORD = Serial.readStringUntil('\n'); inputPASSWORD == "") {
    Serial.println("Senha não foi digitada, usando da programação");
  }

  Serial.printf("O nome do wifi é %s e sua senha do wifi é %s.\n", ssid.c_str(), password.c_str());

  Serial.print("Conectando");
  WiFi.begin(ssid.c_str(), password.c_str());

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");

    if (WiFi.status() == WL_CONNECT_FAILED) {
      Serial.print("A conexão falhou tente mais tarde");
      return;
    }
  }
  Serial.println();

  Serial.printf("Conectado, IP address: %s\n", WiFi.localIP().toString().c_str());
}

void readTemperatureAndHumidity() {
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println(F("Error reading temperature!"));
  } else {
    Serial.printf("Temperature: %f °C", event.temperature);
  }

  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println(F("Error reading humidity!"));
  }
  else {
    Serial.printf("Humidity: %f \\%", event.relative_humidity);
  }
}

void readIncidentLight() {
  auto adcRead = static_cast<uint16_t>(analogRead(PIN_LDR));
  auto lux = static_cast<uint8_t>(map(adcRead, 100, 1023, 0, 10));
  Serial.printf("A quantidade de luz no ambiente é: %c lm\n", lux);
}

const char* macToString(const uint8_t mac[6]) {
  static char s[20];
  sprintf(s, "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  return s;
}

const char* encToString(uint8_t enc) {
  switch (enc) {
    case ENC_TYPE_NONE: return "NONE";
    case ENC_TYPE_TKIP: return "WPA";
    case ENC_TYPE_CCMP: return "WPA2";
    case ENC_TYPE_AUTO: return "AUTO";
    default: return "UNKN";
  }
}
