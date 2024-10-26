// https://www.blogdarobotica.com/2021/05/12/como-utilizar-o-shield-display-16x02-lcd-com-teclado-no-arduino/
//
/*
https://www.makerhero.com/blog/aprenda-a-construir-uma-balanca-com-arduino-e-modulo-hx711/

Quando você ligar (ou reiniciar) o Arduino, o programa considera que a balança está vazia, coloca um aviso no display e chama o método tare() da biblioteca. Esta rotina lê 50 vezes o valor retornado pelo HX711 e considera que a média é o offset a ser descontado de todas as leituras.

Em seguidam o programa verifica na EEProm se já foi feita a calibração. Se não, será feito o procedimento de calibração:

Coloca uma mensagem no display pedindo para colocar um peso de 1Kg;
Aguarda o pressionar e soltar a tecla Select;
Chama o método read_average() da biblioteca para obter a média de 50 leituras do HX711;
Calcula a escala e salva na EEProm.
Como parte final da iniciação, o valor da escala é passado para a biblioteca através do método set_scale().

Concluída a iniciação, o programa passa a chamar periodicamente o método get_units() e a apresentar o valor retornado. Este método obtém uma leitura do HX711 (usamos uma média de 4 leituras), subtrai o offset e divide pela escala, resultando no valor em quilos.

O programa testa também se foi apertada e solta uma das seguintes teclas:

Right: faz novamente o processo de tara;
Left: refaz a calibração.
Estas teclas devem ser apertadas com a balança vazia.
*/
// Baudrate
int baudrate = 9600;  //115200

// Linhas
#include <LiquidCrystal.h>
#include <EEPROM.h>
#include <HX711.h>

// Define as conexões da balança e cria o objeto para acesso
const int CELULA_DADO = 2;
const int CELULA_CLOCK = 3;
HX711 celulaCarga;

// Define as conexões do display e cria o objeto para acesso
const int rs = 8, en = 9, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
const int backLight = 10;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Limites para detecção das teclas, em ordem crescente
int limiteTecla[] = { 50, 150, 300, 500, 750, 1024 };

// Indices para as teclas
const int TEC_DIREITA = 0;
const int TEC_CIMA = 1;
const int TEC_BAIXO = 2;
const int TEC_ESQUERDA = 3;
const int TEC_SELECT = 4;
const int TEC_NENHUMA = 5;

// Endereços na EEProm
const int ENDER_FLAG = 0;
const int ENDER_ESCALA = 1;

// Indicação de balança calibrada
const byte FLAG_CALIBRADA = 0x55;

// Leitura das rotações
int rotacoes;
volatile byte pulsos;
unsigned long tempo;
const byte interruptPin = 18;  //2,3 //chipKIT: https://embedded-lab.com/blog/exploring-the-chipkit-uno32-board/#google_vignette
unsigned int pulsos_por_volta = 2;


// Iniciação
void setup() {
  // Iniciação do Monitor Serial
  //Serial.begin(baudrate);
  Serial.begin(9600);
  // Iniciação do Display
  pinMode(backLight, OUTPUT);
  lcd.begin(16, 2);
  mostraVersao();
  digitalWrite(backLight, HIGH);  // acende

  // Configuração do pino e das variáveis de rotação
  pinMode(interruptPin, INPUT_PULLUP);
  //essa funcao soma o contador a cada vez que tem um pulso
  attachInterrupt(digitalPinToInterrupt(interruptPin), contador, RISING);
  pulsos = 0;
  rotacoes = 0;
  tempo = 0;

  // Iniciação do HX711
  celulaCarga.begin(CELULA_DADO, CELULA_CLOCK);
  ajustaTara();
  if (EEPROM.read(ENDER_FLAG) != FLAG_CALIBRADA) {
    calibra();
  } else {
    // Usa escala salva na EEPROM
    float escala;
    EEPROM.get(ENDER_ESCALA, escala);
    celulaCarga.set_scale(escala);
  }
  mostraVersao();
}

// Laço principal
void loop() {
  // Lê e mostra o peso
  float peso = celulaCarga.get_units();
  char medida[17];
  //  float gc = 9.80665;
  dtostrf(peso, 7, 3, medida);
  //  float medidaN = medida*gc;
  strcat(medida, ", kgf");  //    strcat (medida, ", N");
  lcd.setCursor(0, 1);
  lcd.print(medida);
  // Lê e mostra a rotação
  // verifica o contador de 1 em 1 segundo (1000)ms
  if (millis() - tempo >= 1000) {
    //desliga a funçao enquanto faz a soma
    detachInterrupt(digitalPinToInterrupt(interruptPin));
    rotacoes = (60 * 1000 / pulsos_por_volta) / (millis() - tempo) * pulsos;
    tempo = millis();
    pulsos = 0;
    //volta ao normal apos o calculo
    attachInterrupt(digitalPinToInterrupt(interruptPin), contador, RISING);
  }
  // Mostra o RPM e o Peso no monitor
  Serial.print("RPM, ");
  Serial.print(rotacoes);
  Serial.print(", rpm,");
  //  Serial.print(medida, "kg");
  Serial.print(" Forca,");
  Serial.println(medida);
  //Serial.println(", kgf");

  // Dá um tempo entre as leituras
  delay(250);

  // Trata as teclas
  int tecla = leTecla();
  if (tecla != TEC_NENHUMA) {
    lcd.setCursor(0, 1);
    lcd.print("Solte a tecla");
    while (leTecla() != TEC_NENHUMA) {
      delay(100);
    }
    if (tecla == TEC_DIREITA) {
      ajustaTara();
    } else if (tecla == TEC_ESQUERDA) {
      ajustaTara();
      calibra();
    }
    mostraVersao();  // recompõe a tela
  }
}

// Apresenta a identificação e versão do programa
void mostraVersao() {
  lcd.clear();
  lcd.print("BALANCA v1.50");
}

// Ajusta o offset para o valor da balança vazia
void ajustaTara() {
  lcd.setCursor(0, 1);
  lcd.print("Registrando TARA");
  delay(500);
  celulaCarga.tare(50);
}

// Efetua a calibração da balança
void calibra() {
  // Mostra as instruções
  lcd.clear();
  lcd.print("CALIBRACAO");
  delay(2000);
  lcd.clear();
  lcd.print("Coloque 1 kg");
  lcd.setCursor(0, 1);
  lcd.print("Aperte SELECT");
  while (leTecla() != TEC_SELECT) {
    delay(100);
  }
  lcd.setCursor(0, 1);
  lcd.print("Solte  SELECT");
  while (leTecla() != TEC_NENHUMA) {
    delay(100);
  }
  lcd.print("Aguarde......");
  delay(2000);

  // Faz a leitura e calcula a escala
  long leitura = celulaCarga.read_average(50);
  float escala = (leitura - celulaCarga.get_offset()) / 1.00f;

  // Salva na EEProm
  EEPROM.put(ENDER_ESCALA, escala);
  EEPROM.write(ENDER_FLAG, FLAG_CALIBRADA);

  // Usa a escala calculada
  celulaCarga.set_scale(escala);
}

// Le uma tecla
// Cada tecla do shield resulta em uma tensão diferente em A0
int leTecla() {
  int leitura = analogRead(A0);
  int tecla;
  for (tecla = 0;; tecla++) {
    if (leitura < limiteTecla[tecla]) {
      return tecla;
    }
  }
  return TEC_NENHUMA;  // Não deve acontecer!
}

void contador() {
  //soma 1 pulso a cada vez que a "marca" passa
  pulsos++;
}
