#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <Hash.h>
#include <Crypto.h>
#include <SHA256.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ====================== [WiFi 설정] ======================
const char* ssid = "here_your_wifi_ssid";
const char* password = "here_your_wifi_pw";

// ====================== [풀 설정] ======================
const char* poolHost = "solo.ckpool.org";
const uint16_t poolPort = 3333;
const char* worker = "here_your_worker_btc_account"; // 본인 주소로 변경
const char* workerPass = "x";

WiFiClient client;

// ====================== [OLED 설정] ======================
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// ====================== [채굴 관련 전역] ======================
String job_id, prevhash, coinb1, coinb2, version, nbits, ntime;
String extranonce1 = "abcdef01";  // 임의
String extranonce2 = "00000002";  // 증가 가능
String merkle_root;

// ====================== [WiFi 연결] ======================
void connectWiFi() {
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\nWiFi connected");
}

// ====================== [풀 연결 및 인증] ======================
void connectPool() {
  if (!client.connect(poolHost, poolPort)) {
    Serial.println("Failed to connect to pool");
    return;
  }

  Serial.println("Connected to mining pool");

  client.println("{\"id\":1, \"method\":\"mining.subscribe\", \"params\":[]}");
  delay(1000);
  String authCmd = String("{\"id\":2, \"method\":\"mining.authorize\", \"params\":[\"") + worker + "\", \"" + workerPass + "\"]}";
  client.println(authCmd);
}

// ====================== [SHA256 Helpers] ======================
String sha256(String input) {
  uint8_t hash[32];
  SHA256 sha256;
  sha256.update((const uint8_t*)input.c_str(), input.length());
  sha256.finalize(hash, sizeof(hash));
  String out = "";
  for (int i = 0; i < 32; i++) {
    char hex[3];
    sprintf(hex, "%02x", hash[i]);
    out += hex;
  }
  return out;
}

String doubleSHA256(String input) {
  return sha256(sha256(input));
}

// ====================== [채굴 메인 로직] ======================
void mine() {
  String coinbase = coinb1 + extranonce1 + extranonce2 + coinb2;
  String coinbase_hash = doubleSHA256(coinbase);
  merkle_root = coinbase_hash;

  String header = version + prevhash + merkle_root + ntime + nbits;
  Serial.println("🔨 채굴 시작");

  unsigned long startTime = millis();
  unsigned long hashesDone = 0;
  float hashrate = 0;

  for (uint32_t nonce = 0; nonce < 0xFFFFFFFF; nonce++) {
    char nonceStr[9];
    sprintf(nonceStr, "%08x", nonce);
    String fullHeader = header + nonceStr;
    String hash = doubleSHA256(fullHeader);

    hashesDone++;

    if (hash.startsWith("000000")) {
      Serial.println("💥 유효한 nonce 발견!");
      Serial.println("Nonce: " + String(nonceStr));
      Serial.println("Hash: " + hash);

      display.clearDisplay();
      display.setCursor(0, 0);
      display.setTextSize(1);
      display.println("🎉 Valid Share Found!");
      display.print("Nonce: ");
      display.println(nonceStr);
      display.display();
      delay(10000);
      break;
    }

    if (nonce % 100000 == 0) {
      unsigned long elapsed = millis() - startTime;
      if (elapsed > 0) {
        hashrate = hashesDone / (elapsed / 1000.0); // H/s

        display.clearDisplay();
        display.setCursor(0, 0);
        display.setTextSize(1);
        display.println("ESP8266 BTC Miner");
        display.print("Rate: ");
        display.print((int)hashrate);
        display.println(" H/s");
        display.print("Nonce: ");
        display.println(nonceStr);
        display.display();
      }
    }
  }
}

// ====================== [풀 메시지 파싱] ======================
void checkPoolMessages() {
  while (client.available()) {
    String line = client.readStringUntil('\n');
    Serial.println("[POOL] " + line);

    if (line.indexOf("\"mining.notify\"") > 0) {
      int start = line.indexOf("[");
      int end = line.lastIndexOf("]");
      String payload = line.substring(start + 1, end);

      job_id = getParam(payload, 0);
      prevhash = getParam(payload, 1);
      coinb1 = getParam(payload, 2);
      coinb2 = getParam(payload, 3);
      version = getParam(payload, 5);
      nbits = getParam(payload, 6);
      ntime = getParam(payload, 7);

      Serial.println("🧱 작업 수신됨 - 채굴 시작");
      mine();
    }
  }
}

String getParam(String src, int index) {
  int count = 0;
  int start = 0;
  for (int i = 0; i < src.length(); i++) {
    if (src[i] == '\"') {
      count++;
      if (count == 2 * index + 1)
        start = i + 1;
      else if (count == 2 * index + 2)
        return src.substring(start, i);
    }
  }
  return "";
}

// ====================== [setup / loop] ======================
void setup() {
  Serial.begin(115200);
  delay(1000);

  // OLED 초기화
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("OLED init failed"));
    for (;;);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("ESP-Miner Booting...");
  display.display();
  delay(1500);

  connectWiFi();
  connectPool();
}

void loop() {
  checkPoolMessages();
}
