#include <ArduinoJson.h>
#include <SPI.h>
#include <MFRC522.h>
#include <BluetoothSerial.h>
#include <WiFi.h>
#include <HTTPClient.h>

#define SS_PIN 5
#define RST_PIN 0

MFRC522 mfrc522(SS_PIN, RST_PIN);
const char *ssid = "tplink_ceis";
const char *password = "ceis_tplink";
const char *serverUrl = "localhost:8080/test-room";

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando a WiFi...");
  }
  Serial.println("Conectado a la red WiFi");
  SPI.begin();
  mfrc522.PCD_Init();
}

void loop() {
  /*
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    String jsonStr = fetchDataFromServer();
    configureFromJson(jsonStr);
    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
  }
  */
  if (Serial.available() != 0) {
    char command = Serial.read();
    Serial.println(command);
    if (command == 'o') {
      String jsonStr = fetchDataFromServer();
      //configureFromJson(jsonStr);
    }
  }
}

String fetchDataFromServer() {
  HTTPClient http;
  http.begin(serverUrl);

  int httpCode = http.GET();
  String payload = "{}";

  if (httpCode > 0) {
    Serial.println(httpCode);
    
    if (httpCode == HTTP_CODE_OK){      
      payload = http.getString();
      Serial.println(payload);
    }
   }
 
  else {
    Serial.printf("Error al obtener datos del servidor. Código HTTP: %d\n", httpCode);
  }

  http.end();
  return payload;
}

void configureFromJson(String jsonStr) {
  DynamicJsonDocument doc(256);
  DeserializationError error = deserializeJson(doc, jsonStr);

    Serial.print("Error al analizar el JSON: ");
  if (error) {
    Serial.println(error.c_str());
    return;
  }

  int roomNumber = doc["Numero"];
  bool roomOccupied = doc["Ocupada"];
  String roomCard = doc["TarjetaAsociada"];

  /*
  int clientId = doc["Cliente"]["Identificación"];
  String clientName = doc["Cliente"]["Nombre"];
  String clientPhone = doc["Cliente"]["Teléfono"];
  String clientCard = doc["Cliente"]["Tarjeta Asociada"];
  */
  
  Serial.println("Datos de la habitación:");
  Serial.print("Número: ");
  Serial.println(roomNumber);
  Serial.print("Ocupada: ");
  Serial.println(roomOccupied);
  Serial.print("Tarjeta Asociada: ");
  Serial.println(roomCard);

  /*
  Serial.println("Datos del cliente:");
  Serial.print("Identificación: ");
  Serial.println(clientId);
  Serial.print("Nombre: ");
  Serial.println(clientName);
  Serial.print("Teléfono: ");
  Serial.println(clientPhone);
  Serial.print("Tarjeta Asociada: ");
  Serial.println(clientCard);
  */
}
