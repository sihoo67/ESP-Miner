#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <Hash.h>
#include <Crypto.h>
#include <SHA256.h>

// ====================== [설정] ======================
const char* ssid = "here_your_wifi_ssid";
const char* password = "here_your_wifi_password";

const char* poolHost = "solo.ckpool.org";
const uint16_t poolPort = 3333;
const char* worker = "here_your_btc_account";  // 본인 비트코인 주소
const char* workerPass = "x";

WiFiClient client;

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

// ====================== [Stratum 연결 및 인증] ======================
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

// ====================== [채굴 시도] ======================
void mine() {
  String coinbase = coinb1 + extranonce1 + extranonce2 + coinb2;
  String coinbase_hash = doubleSHA256(coinbase);
  merkle_root = coinbase_hash; // 단순화. 실제는 merkle_branch 포함해야 함.

  String header = version + prevhash + merkle_root + ntime + nbits;
  Serial.println("🔨 시작: SHA256 채굴");

  for (uint32_t nonce = 0; nonce < 0xFFFFFFFF; nonce++) {
    char nonceStr[9];
    sprintf(nonceStr, "%08x", nonce);
    String fullHeader = header + nonceStr;
    String hash = doubleSHA256(fullHeader);

    if (hash.startsWith("000000")) {
      Serial.println("💥 유효한 nonce 발견!");
      Serial.println("Nonce: " + String(nonceStr));
      Serial.println("Hash: " + hash);
      // submit 로직 생략 (Stratum 방식 submit 필요)
      break;
    }

    if (nonce % 1000000 == 0) {
      Serial.print(".");
    }
  }
}

// ====================== [Job 수신 및 파싱] ======================
void checkPoolMessages() {
  while (client.available()) {
    String line = client.readStringUntil('\n');
    Serial.println("[POOL] " + line);

    if (line.indexOf("\"mining.notify\"") > 0) {
      // 아주 단순한 파싱
      int start = line.indexOf("[");
      int end = line.lastIndexOf("]");
      String payload = line.substring(start + 1, end);

      int idx = 0;
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

// ====================== [메인 루프] ======================
void setup() {
  Serial.begin(115200);
  delay(1000);
  connectWiFi();
  connectPool();
}

void loop() {
  checkPoolMessages();
}
