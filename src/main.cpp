#include <WiFi.h>

const char* macToString(uint8_t mac[6]);
const char* encToString(uint8_t enc);
void getNetworkList();
void connectToInternet();

String ssid = "Apartamento 502";
String password = "cobalto01";
int status = WL_IDLE_STATUS;

void setup() {
  Serial.begin(115200);
  delay(10000);
  connectToInternet();
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Você está conectado!");
    delay(1000);
  }
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
  for (auto i = 0; i < cnt; i++) {
    uint8_t bssid[6];
    WiFi.BSSID(i, bssid);
    Serial.printf("%32s %5s %17s %2d %4ld\n", WiFi.SSID(i), encToString(WiFi.encryptionType(i)), macToString(bssid), WiFi.channel(i), WiFi.RSSI(i));
  }

  delay(2000);
}

void connectToInternet() {
  getNetworkList();

  Serial.print("Escreva o nome da internet para conectar:");
  while (Serial.available() == 0) {
  }
  String inputSSID = Serial.readStringUntil('\n');
  if (inputSSID == "") {
    Serial.println("SSID não foi digitada, usando da programação");
  }

  Serial.print("Agora a senha da internet:");
  while (Serial.available() == 0) {
  }
  String inputPASSWORD = Serial.readStringUntil('\n');
  if (inputPASSWORD == "") {
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

const char* macToString(uint8_t mac[6]) {
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
  }
  return "UNKN";
}
