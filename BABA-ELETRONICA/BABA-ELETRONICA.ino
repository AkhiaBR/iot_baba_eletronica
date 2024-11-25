#include "arduino_secrets.h"
// BABÁ ELETRÔNICA

// BIBLIOTECAS
#include "thingProperties.h"
#include <DHT.h>
#include <DHT_U.h>

// PINOS
#define DHTPIN 35
#define DHTTYPE 11
#define BUZZER_PIN 22
#define SOUND_SENSOR_PIN 34
#define DIGITAL_SOUND_SENSOR_PIN 32

// VARIÁVEIS
DHT dht(DHTPIN, DHTTYPE); 

// FUNÇÃO PARA CONVERTER INPUT EM CAIXA ALTA
String toUpperCase(String str) { 
  for (int i = 0; i < str.length(); i++) { 
    str[i] = toupper(str[i]); 
  } 
  return str;
}

// FUNÇÕES DEFAULT
void setup() {
  Serial.begin(9600);

  initProperties(); // inicia a função properties, chamando o arquivo "thingProperties.h"
  ArduinoCloud.begin(ArduinoIoTPreferredConnection); // inicia a conexão com o Arduino Cloud
  
  setDebugMessageLevel(2); // mensagens para debug do Cloud
  ArduinoCloud.printDebugInfo();

  // DEFINIÇÃO DOS SENSORES E ATUADORES
  dht.begin();
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(SOUND_SENSOR_PIN, INPUT);
  pinMode(DIGITAL_SOUND_SENSOR_PIN, INPUT);

  // INICIALIZAÇÃO DAS VARIÁVEIS
  sensorTemperatura = dht.readTemperature();
  sensorUmidade = dht.readHumidity();
  sensorSom = analogRead(SOUND_SENSOR_PIN);
  sensorSom = map(sensorSom, 0, 4095, 0, 100);
  sensorDigitalSom = digitalRead(DIGITAL_SOUND_SENSOR_PIN);
}

void loop() {
  ArduinoCloud.update();

  sensorSom = analogRead(SOUND_SENSOR_PIN);
  sensorSom = map(sensorSom, 0, 4095, 0, 100);  // cria uma nova forma de medição para os valores do sensor
  sensorDigitalSom = digitalRead(DIGITAL_SOUND_SENSOR_PIN);

  // CHECA ERROS DE LEITURA DOS SENSORES DE UMIDADE E TEMPERATURA
  sensorTemperatura = dht.readTemperature();
  if (isnan(sensorTemperatura)) {
    babaMessager = "ALERTA: Erro na leitura da temperatura!";
  }

  sensorUmidade = dht.readHumidity();
  if (isnan(sensorUmidade)) {
    babaMessager = "ALERTA: Erro na leitura da umidade!";
  }
}

// FUNÇÕES ADICIONADAS
void onSensorUmidadeChange()  {
  if (sensorUmidade <= 30) {
    babaMessager = "ALERTA: A umidade do ambiente está muito baixa!";
  }
  else if (sensorUmidade >= 85) {
    babaMessager = "ALERTA: A umidade do ambiente está muito alta!";
  }
}

void onSensorSomChange()  {
  Serial.println(sensorSom); // verifica se o sensor analógico está funcionando
}

void onSensorTemperaturaChange()  {
  if (sensorTemperatura <= 20) {
    babaMessager = "ALERTA: A temperatura do ambiente está muito baixa!";
  }
  else if (sensorTemperatura >= 30) {
    babaMessager = "ALERTA: A temperatura do ambiente está muito alta!";
  }
}

void onSensorDigitalSomChange()  {
    if (sensorDigitalSom == HIGH) { // se o sensor digital detectar um som alto:
    babaMessager = "ALERTA: Som alto detectado. O bebê pode estar chorando!";

    tocarMusica();
  }
}

void onBabaMessagerChange()  {
  String cmdMessager = toUpperCase(babaMessager); // mensagens enviadas para o messager são alteradas para CAIXA ALTA
  
  if (cmdMessager == "TOCAR MUSICA 1") {
    tocarMusica1();
  }
  else if (cmdMessager == "TOCAR MUSICA 2") {
    tocarMusica2();
  }
  else if (cmdMessager == "TOCAR MUSICA 3") {
    tocarMusica3();
  }
  else if (cmdMessager == "TEMPERATURA") {
    babaMessager = "A temperatura do ambiente é igual a: "+String(sensorTemperatura);
  }
  else if (cmdMessager == "UMIDADE") {
    babaMessager = "A umidade do ambiente é igual a: "+String(sensorUmidade);
  }
  else if (cmdMessager == "AJUDA") {
    babaMessager = "Bem-vindo à Babá Eletrônica!\n"
                   "\nComandos disponíveis:\n"
                   "- 'TOCAR MUSICA 1': Toca a primeira canção de ninar.\n"
                   "- 'TOCAR MUSICA 2': Toca a segunda canção de ninar.\n"
                   "- 'TOCAR MUSICA 3': Toca a terceira canção de ninar.\n"
                   "- 'TEMPERATURA': Mostra a temperatura do ambiente.\n"
                   "- 'UMIDADE': Mostra a umidade do ambiente."; 
  }
  else {
    babaMessager = "ALERTA: Comando não reconhecido. Tente novamente...";
  }
}

void tocarMusica() {
  tone(BUZZER_PIN, 262, 500); // nota C4
  delay(500); 
  tone(BUZZER_PIN, 392, 500); // nota G4
  delay(500); 
  tone(BUZZER_PIN, 440, 500); // nota A4
  delay(500); 
  tone(BUZZER_PIN, 392, 500); // nota G4
  delay(500); 
  tone(BUZZER_PIN, 349, 500); // nota F4
  delay(500); 
  tone(BUZZER_PIN, 330, 500); // nota E4
  delay(500); 
  tone(BUZZER_PIN, 294, 500); // nota D4
  delay(500); 
  tone(BUZZER_PIN, 262, 500); // nota C4
  delay(500);
  noTone(BUZZER_PIN);
}

void tocarMusica1() {
  tone(BUZZER_PIN, 262, 500); // C4
  delay(500); 
  tone(BUZZER_PIN, 262, 500); // C4
  delay(500); 
  tone(BUZZER_PIN, 392, 500); // G4
  delay(500); 
  tone(BUZZER_PIN, 392, 500); // G4
  delay(500); 
  tone(BUZZER_PIN, 440, 500); // A4
  delay(500); 
  tone(BUZZER_PIN, 440, 500); // A4
  delay(500); 
  tone(BUZZER_PIN, 392, 1000); // G4
  delay(1000); 
  tone(BUZZER_PIN, 349, 500); // F4
  delay(500); 
  tone(BUZZER_PIN, 349, 500); // F4
  delay(500); 
  tone(BUZZER_PIN, 330, 500); // E4
  delay(500); 
  tone(BUZZER_PIN, 330, 500); // E4
  delay(500); 
  tone(BUZZER_PIN, 294, 500); // D4
  delay(500); 
  tone(BUZZER_PIN, 294, 500); // D4
  delay(500); 
  tone(BUZZER_PIN, 262, 1000); // C4
  delay(1000);
  noTone(BUZZER_PIN);
}

void tocarMusica2() {
  tone(BUZZER_PIN, 392, 500); // G4
  delay(500); 
  tone(BUZZER_PIN, 392, 500); // G4
  delay(500); 
  tone(BUZZER_PIN, 440, 500); // A4
  delay(500); 
  tone(BUZZER_PIN, 440, 500); // A4
  delay(500); 
  tone(BUZZER_PIN, 392, 500); // G4
  delay(500); 
  tone(BUZZER_PIN, 392, 500); // G4
  delay(500); 
  tone(BUZZER_PIN, 330, 500); // E4
  delay(500); 
  tone(BUZZER_PIN, 330, 500); // E4
  delay(500); 
  tone(BUZZER_PIN, 294, 500); // D4
  delay(500); 
  tone(BUZZER_PIN, 294, 500); // D4
  delay(500); 
  tone(BUZZER_PIN, 262, 1000); // C4
  delay(1000);
  noTone(BUZZER_PIN);
}

void tocarMusica3() {
  tone(BUZZER_PIN, 294, 500); // D4
  delay(500); 
  tone(BUZZER_PIN, 262, 500); // C4
  delay(500); 
  tone(BUZZER_PIN, 220, 500); // A3
  delay(500); 
  tone(BUZZER_PIN, 262, 500); // C4
  delay(500); 
  tone(BUZZER_PIN, 294, 500); // D4
  delay(500); 
  tone(BUZZER_PIN, 330, 500); // E4
  delay(500); 
  tone(BUZZER_PIN, 349, 500); // F4
  delay(500); 
  tone(BUZZER_PIN, 392, 500); // G4
  delay(500);
  noTone(BUZZER_PIN);
}
