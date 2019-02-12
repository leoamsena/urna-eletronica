#include "candidato.h"

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
