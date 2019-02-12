/**
 *
 * Código para urna eletrônica em Arduino Uno
 * 
 *Este códgo utiliza:
 *Auto-falante conectado ao pino 9
 *Display LCD 16x2 conectado segundo imagem (lcd_display.png)
 *Display Serial Touch 2.4 320x240 Ihm Nextion (RX e TX)
 *Teclado Matricial 4x3 12 teclas conectado aos pinos {[COLUNAS -> A4, 7, 8] e [LINHAS ->  A0, A1, A2, A3] }
 *
 *ATENÇÃO:
 *O código foi criado e moldado para realização de eleição em uma unidade da SSVP (http://www.ssvpbrasil.org.br/ || https://www.ssvpglobal.org/) e o 
 *uso de logomarcas foi previamente autorizado por responsáveis da instituição. Para se adequar ao sua situação É INDISPENSÁVEL que o plano de fundo da "page0" 
 *do display Nextion seja alterado. O uso não autorizado de simbolos da instituição em questão podem ocasionar ações legais cabíveis. 
 *
 *
 * @author  Leonardo Amorim de Sena (email:<leonardo123.amorim@gmail.com>)
 * @date    2019/02/12
 * @copyright 
 * Copyright (C) Leonardo Amorim de Sena \n
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 */
#include <Nextion.h>
#include "candidato/candidato.h"
#include <LiquidCrystal.h>
#include <Keypad.h>
NexPicture p0 = NexPicture(1,3,"p0");
NexText passwordInput = NexText(0, 4, "passwordInput");
NexPage page0    = NexPage(0, 0, "page0");
NexPage page1    = NexPage(1, 0, "page1");
NexPage page2    = NexPage(2, 0, "page2");
NexText candNumber = NexText(1,2,"candNumber");
NexText waitingTxt = NexText(0,2,"waitingTxt");
NexText result = NexText(2,2,"result");


const short unsigned pino_falante = 9; // Conectar falante ao pino 9
const short unsigned tone1 = 2132;
const short unsigned tone2 = 2032;

const String SENHA_LIBERACAO = "1212";

const byte nCandidatos = 2;
Candidato candidatos[nCandidatos] = {
	Candidato("Andre de Sena",10),
	Candidato("Donizete Geraldo",23)
};
uint32_t picNumbers[nCandidatos] = {
	1,
	2
};
const uint32_t defaultPic = 4;
const uint32_t picNulo = 3;
unsigned short numero = 0;
unsigned votosNulos = 0;

/* TECLADO  */
const byte LINHAS = 4;
const byte COLUNAS = 3;
char matriz_teclas[LINHAS][COLUNAS] = {
	{'1', '2', '3'},
	{'4', '5', '6'},
	{'7', '8', '9'},
	{'*', '0', '#'}
};
//Definicao dos pinos das linhas
byte PinosLinhas[LINHAS] = {A0, A1, A2, A3};
//Definicao dos pinos das colunas
byte PinosColunas[COLUNAS] = {A4, 7, 8};
//Inicializa o teclado
Keypad meuteclado = Keypad(makeKeymap(matriz_teclas), PinosLinhas, PinosColunas, LINHAS, COLUNAS);
/* TECLADO  */

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);


void setup() {
	//Serial.begin(9600);
	nexInit();
	lcd.begin(16, 2);
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print("Urna Eletronica 1.0");
	lcd.setCursor(0, 1);
	lcd.print(" Carregando...");
	//passwordInput.setText("");
	//p0.setPic(3);
	aguardandoLiberacao();
}
void loop() {
  p0.setPic(8);
  char tecla_pressionada = meuteclado.getKey();
	if(tecla_pressionada) {
		tecla();
		if(tecla_pressionada!='*' && tecla_pressionada!='#' ) { 
			if(numero<10) { //0 ou 1
				if(numero==0) { //nenhuma
					numero = tecla_pressionada - '0';
					char auxCharNumber[1] = {tecla_pressionada};
					candNumber.setText(auxCharNumber);
				} else { //uma tecla
					numero =(numero*10)+ ((tecla_pressionada - '0'));
					char auxCharNumber[2] = {'\0','\0'};
					itoa(numero,auxCharNumber,10);
					candNumber.setText(auxCharNumber);
					//NUMERO CONTEM O NUMERO QUE QUER VOTAR
					unsigned short i=0;

					while(i<nCandidatos && numero != candidatos[i].getNumero()) {
						i++;
					}




          bool votou = false;
					if(i==nCandidatos) { //votar nulo
						votou = telaConfirmacao("NULO",numero,nCandidatos+1);
					} else { //votar em candidatos[i] 
						votou = telaConfirmacao(candidatos[i].getNome(),candidatos[i].getNumero(),i);
					}
          if(votou){
            aguardandoLiberacao();
          }
           //aqui chega dibas
					numero = 0;
				}
			}
		}
	}
 p0.setPic(9);
}
void aguardandoLiberacao() {
	page0.show();
	lcd.clear();
	delay(200);
	lcd.setCursor(0,0);
	lcd.print("Aguardando");
	delay(200);
	lcd.setCursor(0,1);
	lcd.print("Liberacao...");
	delay(200);
	String senha = "";
	passwordInput.setText(senha.c_str());
	bool resultado = false;

	while(senha!=SENHA_LIBERACAO) {
		char tecla_pressionada2 = meuteclado.getKey();

		if(tecla_pressionada2) {
			tecla();

			if(tecla_pressionada2 == '#') {
				resultado = !resultado;
				waitingTxt.setText((resultado)?"Mostrar resultado":"Aguardando liberação...");
			} else {
				if(senha.length()<4) {
					senha = senha + tecla_pressionada2;
				} else {
					senha = "";
				}
				passwordInput.setText(senha.c_str());
			}
		}
	}

	if(!resultado) { //liberar urna
		tecla();
		delay(200);
		tecla();
		lcd.clear();
		delay(200);
		page1.show();
	} else { //mostrar resultado
		fim();
		lcd.clear();
		delay(200);
		lcd.print("Resultado...");
		page2.show();
		String auxStr = "";
    String linha = "";;
		for(unsigned i=0; i<nCandidatos; i++) {
      linha = candidatos[i].getNome() + " - " + candidatos[i].getVotos()+" voto(s)";
			
     for(unsigned j=linha.length();j<40;j++){
      linha = linha + " ";
     }
     auxStr =  auxStr + linha;
		}

		auxStr = auxStr + "Votos nulos - " + votosNulos;
		char auxChar[auxStr.length()+1];
		auxStr.toCharArray(auxChar,auxStr.length()+1);
		result.setText(auxChar);

		while(true) {}
	}
  return;
}
bool telaConfirmacao(String nome,unsigned short numero,unsigned short i) {

	if(i!=nCandidatos+1) {
		p0.setPic(picNumbers[i]);
	} else {
		p0.setPic(picNulo);
	}

	lcd.clear();
	delay(200);
	lcd.setCursor(0,0);
	lcd.print("Confirmar voto:");
	delay(200);
	lcd.setCursor(0,1);
	lcd.print(nome);
	delay(200);
	char teclaPre = false;

	while(!teclaPre || (teclaPre!='*' && teclaPre!='#')) {
		teclaPre = meuteclado.getKey();
	}

	if(teclaPre=='*') { //cancela
		numero = 0;
		String zeroStr = "";
		candNumber.setText(zeroStr.c_str());
		p0.setPic(defaultPic);
		tecla();
    return false;
	} else { //confirma
		if(i==nCandidatos+1) { //voto nulo
			votosNulos++;
			numero = 0;
		} else { //votar em candidato (i)
			candidatos[i].votar();
			numero=0;
		}

		fim();
		candNumber.setText({'\0'});
		return true;
	}
}
void fim() {
	for(short unsigned i = 0; i < 5; i++) {
		tone(pino_falante, tone2, 90);
		delay(90);
		tone(pino_falante, tone1, 90);
		delay(90);
	}

	tone(pino_falante, tone2, 120);
	delay(120);
	noTone(pino_falante);
 return;
}
void tecla() {
	tone(pino_falante, tone2, 200);
 return;
}
