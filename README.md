# 🚀 ESP8266 Bitcoin Solo Mining Project  
" 미친 짓은 곧 혁신이다. " : " Crazy is innovation."

## 🔥 개요

이 프로젝트는 **단돈 몇 천 원짜리 ESP8266 마이크로컨트롤러로**  
전 세계 수십억 달러짜리 Bitcoin 네트워크에  
**정면으로 도전하는 실험이자 반란**입니다.

> "우리가 미쳤다는 건 알고 있다.  
> 하지만 단 1번이라도 블록을 찾는다면, 그건 세계가 미친 거다."

## 🎯 목표

- ESP8266 단독으로 `solo.ckpool.org` 풀에 접속
- 실시간 SHA256d 채굴 연산 수행
- 블록 발견 시 자동 전송
- 해시레이트 측정 및 OLED 표시 (옵션)
- 단 1회의 기적, 1블록을 노린다.

## 🛠️ 사양

| 항목         | 내용 |
|--------------|------|
| 디바이스     | NodeMCU ESP8266 (ESP-12E) |
| 풀 주소      | `solo.ckpool.org:3333` |
| 후원 주소    | `bc1qavzc2wcs03m7lyz0gvqzl8yhk4nn79sme0twwu` |
| 평균 해시    | 333~500 H/s |
| 라이브러리   | ESP8266WiFi, WiFiClient, Hash, Crypto, SHA256 |
| 언어         | Arduino C++ |
| 저작권       | CC BY-SA 4.0 |

## 📨 저작권
This work is licensed under Creative Commons Attribution-ShareAlike 4.0 International.
ESP-Miner © 2025 by sihoo67 is licensed under Creative Commons Attribution-ShareAlike 4.0 International. To view a copy of this license, visit https://creativecommons.org/licenses/by-sa/4.0/
<p xmlns:cc="http://creativecommons.org/ns#" xmlns:dct="http://purl.org/dc/terms/"><a property="dct:title" rel="cc:attributionURL" href="https://github.com/sihoo67/ESP-Miner">ESP-Miner</a> by <a rel="cc:attributionURL dct:creator" property="cc:attributionName" href="https://github.com/sihoo67">sihoo67</a> is licensed under <a href="https://creativecommons.org/licenses/by-sa/4.0/?ref=chooser-v1" target="_blank" rel="license noopener noreferrer" style="display:inline-block;">Creative Commons Attribution-ShareAlike 4.0 International<img style="height:22px!important;margin-left:3px;vertical-align:text-bottom;" src="https://mirrors.creativecommons.org/presskit/icons/cc.svg?ref=chooser-v1" alt=""><img style="height:22px!important;margin-left:3px;vertical-align:text-bottom;" src="https://mirrors.creativecommons.org/presskit/icons/by.svg?ref=chooser-v1" alt=""><img style="height:22px!important;margin-left:3px;vertical-align:text-bottom;" src="https://mirrors.creativecommons.org/presskit/icons/sa.svg?ref=chooser-v1" alt=""></a></p>

## 📦 설치

1. [Arduino IDE](https://www.arduino.cc/en/software) 설치
2. 보드 매니저에서 `ESP8266` 보드 추가
2-1. ESP8266 보드는 http://arduino.esp8266.com/stable/package_esp8266com_index.json 입니다.
3. 코드 열기
4. 코드에 본인의 BTC 주소와 와이파이 정보 기입.

## ✒️ 철학

" 미친 짓은 곧 혁신이다. " : " Crazy is innovation. "

" 0이지만, 0은 곧 아니다. " : " 0, but it's not zero. "

" 0에서 새로운 혁신으로 " : " From 0 to new innovations. "
