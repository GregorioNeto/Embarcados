/*
 * IRremote: IRsendRawDemo - demonstrates sending IR codes with sendRaw
 * An IR LED must be connected to Arduino PWM pin 3.
 * Version 0.1 July, 2009
 * Copyright 2009 Ken Shirriff
 * http://arcfn.com
 *
 * IRsendRawDemo - added by AnalysIR (via www.AnalysIR.com), 24 August 2015
 *
 * This example shows how to send a RAW signal using the IRremote library.
 * The example signal is actually a 32 bit NEC signal.
 * Remote Control button: LGTV Power On/Off. 
 * Hex Value: 0x20DF10EF, 32 bits
 * 
 * It is more efficient to use the sendNEC function to send NEC signals. 
 * Use of sendRaw here, serves only as an example of using the function.
 * 
 */

/*
 * =====================================================================================
 *
 *       Filename:  cadastro_controle.ino
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  12/02/2017 20:05:55
 *       Revision:  none
 *       Compiler:  arduino IDE
 *
 *         Author:  Gregório Neto, g_neto01@hotmail.com
 *   Organization:  UFC-Quixadá
 *
 * =====================================================================================
 */

#include <IRremote.h>


int RECV_PIN = 11; // PINO DO RECEPTOR
IRsend irsend; // Definindo o infravermelho emissor
IRrecv irrecv(RECV_PIN); // Definindo o infravermelho receptor
decode_results results; // Definindo o codigo decodificador dos sinais em results

unsigned int count_value = 0; // contador para quantos sinais é recebidos.
unsigned int sign_value = 0; // valor do sinal recebido
unsigned int syze_buffer[200];

int khz = 38; // 38kHz carrier frequency for the NEC protocol

/* Função para armazenar e mostrar os sinais enviados pelo infravermelho do controle e recebidos pelo receptor infravermelho*/
void save_irrecv(decode_results *results) {
  int count = results->rawlen;  // quantidade de sinais recebidos
  int count_aux = 0; // contador auxiliar para armazenar no buffer apartir da posição 0
  Serial.println("CODIGO RECEBIDO:");
  Serial.print("RAW_BUFFER");
  Serial.print("[200] = ");
    for (int i = 1; i < count; i++) {
        count_value = i; // para imprimir as quantidades de sinais obtidas 
    if (i & 1) {
      //Serial.print(results->rawbuf[i]*USECPERTICK, DEC);
      sign_value = (results->rawbuf[i]*USECPERTICK); // recebendo os sinais menores para i posição do raw_buffer
      syze_buffer[count_aux] = sign_value; // salvando o sinal obtido no buffer auxiliar
      Serial.print(syze_buffer[count_aux]); 
    }
    else {
      Serial.write('-');
      //Serial.print((unsigned long) results->rawbuf[i]*USECPERTICK, DEC);
      sign_value = ((unsigned long) results->rawbuf[i]*USECPERTICK); // recebendo os sinais longo para i posição do raw_buffer 
      syze_buffer[count_aux] = sign_value; // salvando o sinal obtido no buffer auxiliar
      Serial.print(syze_buffer[count_aux]);
    }
    Serial.print(" ");
    count_aux++; // Conta quantos tem no array 
  }
  Serial.println();
  Serial.print("Quantidade de Sinais: ");
  Serial.print(count_value);
  Serial.println();
}

void recv_cod()
{
      if (irrecv.decode(&results)) { // decodifica os sinais recebidos
    //Serial.println(results.value, HEX); // valor decodificado em hex, mas não é tão preciso
    save_irrecv(&results); // é chamado a função de
    irrecv.resume(); // Recebe o Proximo Valor do Receptor infravermelho 
    } 
}


void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn();
}


void loop() {
  recv_cod();
  switch(Serial.read()){
  case 'a':
  Serial.println(" ");
  Serial.println("Sinal Enviado para Ar-Condicionado");
  irsend.sendRaw(syze_buffer, sizeof(syze_buffer) / sizeof(syze_buffer[0]), khz); //Note the approach used to automatically calculate the size of the array.
    break;
  }

}
