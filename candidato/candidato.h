#ifndef CANDIDATO_H
#define CANDIDATO_H
#include <Arduino.h>
class Candidato{
	private:
		unsigned votos;
		unsigned short numero;
		String nome;
	public:
		unsigned getVotos();
		void votar();
		Candidato(String nome,unsigned short numero);
		unsigned short getNumero();
		String getNome();
};

Candidato::Candidato(String nome,unsigned short numero){
	this->nome = nome;
	this->numero = numero;
	this->votos = 0;
}
void Candidato::votar(){
	this->votos++;
}
unsigned Candidato::getVotos(){
	return this->votos;
}
unsigned short Candidato::getNumero(){
	return this->numero;
}
String Candidato::getNome(){
	return this->nome;
}

#endif
