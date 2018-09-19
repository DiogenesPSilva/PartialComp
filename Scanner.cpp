#include <iostream>
#include <string>
#include <cstdlib>
#include <array>

#define OpSum 8
#define OpSub 9
#define OpMul 10
#define OpDiv 11
#define Ident 12
#define IdNum 13
#define Maior 14
#define MaoI  15
#define Menor 16
#define MeoI  17
#define Equals 18
#define AbP 19
#define FeP 20
#define AbC 21
#define FeC 22
#define Comma 23
#define PoCom 24
#define Equal 25
#define NotE 26
#define Lit 27

using namespace std;

void bloco(FILE *arq);
void decla_var(FILE *arq);
void programa(FILE *arq, token T);
void tipo(FILE *arq);
void Id(FILE *arq);

int line = 1, column = 1;

array <string, 8> word = {"while", "for", "if", "else", "int", "float", "char", "main"};

typedef struct Token{

	short id;
	string lexema;

}token;

token Scanner(FILE *arq){

	static char LookAhead = ' ';
	token T;
	static string Ult;

Again:
	if( LookAhead == '\n' ){
		line += 1;
		column = 1;
	}
	if( LookAhead == '\t' )
		column += 4;

	while(isspace(LookAhead))
		LookAhead = fgetc(arq);

	if(isalpha(LookAhead) || LookAhead == '_'){
		T.lexema.insert(T.lexema.end(), LookAhead);
		LookAhead = fgetc(arq);
		column += 1;
		while(isalnum(LookAhead) || LookAhead == '_'){
			T.lexema.insert(T.lexema.end(), LookAhead);
			LookAhead = fgetc(arq);
			column += 1;
		}
		Ult.replace(Ult.begin(), Ult.end(), T.lexema);
		for(int i = 0; i < 8; i++){
			if(T.lexema.compare(word[i]) == 0){
				T.id = i;
				return T;
			}
		}
		T.id = Ident;
		return T;
	}

	if(isdigit(LookAhead)){
		T.lexema.insert(T.lexema.end(), LookAhead);
		LookAhead = fgetc(arq);
		column += 1;
		while(isdigit(LookAhead)){
			T.lexema.insert(T.lexema.end(), LookAhead);
			LookAhead = fgetc(arq);
			column += 1;
		}
		if(LookAhead == '.'){
			LookAhead = fgetc(arq);
			column += 1;
			if(isdigit(LookAhead)){
				T.lexema.insert(T.lexema.end(), '.');
				T.lexema.insert(T.lexema.end(), LookAhead);
				LookAhead = fgetc(arq);
				column += 1;
				while(isdigit(LookAhead)){
					T.lexema.insert(T.lexema.end(), LookAhead);
					LookAhead = fgetc(arq);
					column += 1;
				}
				Ult.replace(Ult.begin(), Ult.end(), T.lexema);
				T.id = IdNum;
				return T;
			}
			else{
			cout<<"Erro:Float mal formado!"<<endl;
			cout<<"Ultimo token lido: "<<Ult<<endl;
			exit(1);
			}
		}
		else{
			T.id = IdNum;
			return T;
		}
	}
	
	if(LookAhead == '<'){
		T.lexema.insert(T.lexema.end(), LookAhead);
		LookAhead = fgetc(arq);
		column += 1;
		if(LookAhead == '='){
			T.lexema.insert(T.lexema.end(), LookAhead);
			LookAhead = fgetc(arq);
			column += 1;
			T.id = MeoI;
			Ult.replace(Ult.begin(), Ult.end(), T.lexema);
			return T;
			}
		else{
			T.id = Menor;
			Ult.replace(Ult.begin(), Ult.end(), T.lexema);
			return T;
			}	
	}

	if(LookAhead == '>'){
			T.lexema.insert(T.lexema.end(), LookAhead);
			LookAhead = fgetc(arq);
			column += 1;
			if(LookAhead == '='){
				T.lexema.insert(T.lexema.end(), LookAhead);
				LookAhead = fgetc(arq);
				column += 1;
				T.id = MaoI;
				Ult.replace(Ult.begin(), Ult.end(), T.lexema);
				return T;
			}
			else{
				T.id = Maior;
				Ult.replace(Ult.begin(), Ult.end(), T.lexema);
				return T;
			}
	}

	if(LookAhead == '+'){
		T.lexema.insert(T.lexema.end(), LookAhead);
		T.id = OpSum;
		LookAhead = fgetc(arq);
		column += 1;
		Ult.replace(Ult.begin(), Ult.end(), T.lexema);
		return T;
	}

	if(LookAhead == '-'){
		T.lexema.insert(T.lexema.end(), LookAhead);
		T.id = OpSub;
		LookAhead = fgetc(arq);
		column += 1;
		Ult.replace(Ult.begin(), Ult.end(), T.lexema);
		return T;
	}

	if(LookAhead == '*'){
		T.lexema.insert(T.lexema.end(), LookAhead);
		T.id = OpMul;
		LookAhead = fgetc(arq);
		column += 1;
		Ult.replace(Ult.begin(), Ult.end(), T.lexema);
		return T;
	}

	if( LookAhead == '/' ){
		LookAhead = fgetc(arq);
		column += 1;
		if(LookAhead == '/'){
			while(LookAhead != '\n')
				LookAhead = fgetc(arq);
			goto Again;
		}
		if( LookAhead == '*' ){
Repeat:		do{
				LookAhead = fgetc(arq);
				column += 1;
				if( LookAhead == EOF ){
					cout<<"Erro! Linha "<<line<<" coluna "<<column<<" Fim de arquivo encontrado dentro de um comentario '/**/'!"<<endl;
					cout<<"Ultimo token lido: "<<Ult<<endl;
					exit(2);
				}
			}while( LookAhead != '*' );
			LookAhead = fgetc(arq);
			column += 1;
			if( LookAhead == '/' ){
				LookAhead = fgetc(arq);
				column += 1;
				goto Again;
			}
			else
				goto Repeat;  
		}
		else{
		T.lexema.insert(T.lexema.end(), '/');
		T.id = OpDiv;
		Ult.replace(Ult.begin(), Ult.end(), T.lexema);
		return T;
		}
	}
	
	if(LookAhead == '('){
		T.lexema.insert(T.lexema.end(), LookAhead);
		LookAhead = fgetc(arq);
		column += 1;
		T.id = AbP;
		Ult.replace(Ult.begin(), Ult.end(), T.lexema);
		return T;
	}

	if(LookAhead == ')'){
		T.lexema.insert(T.lexema.end(), LookAhead);
		LookAhead = fgetc(arq);
		column += 1;
		T.id = FeP;
		Ult.replace(Ult.begin(), Ult.end(), T.lexema);
		return T;
	}

	if(LookAhead == '{'){
		T.lexema.insert(T.lexema.end(), LookAhead);
		LookAhead = fgetc(arq);
		column += 1;
		T.id = AbC;
		Ult.replace(Ult.begin(), Ult.end(), T.lexema);
		return T;
	}

	if(LookAhead == '}'){
		T.lexema.insert(T.lexema.end(), LookAhead);
		LookAhead = fgetc(arq);
		column += 1;
		T.id = FeC;
		Ult.replace(Ult.begin(), Ult.end(), T.lexema);
		return T;
	}

	if(LookAhead == ','){
		T.lexema.insert(T.lexema.end(), LookAhead);
		LookAhead = fgetc(arq);
		column += 1;
		T.id = Comma;
		Ult.replace(Ult.begin(), Ult.end(), T.lexema);
		return T;
	}

	if(LookAhead == ';'){
		T.lexema.insert(T.lexema.end(), LookAhead);
		LookAhead = fgetc(arq);
		column += 1;
		T.id = PoCom;
		Ult.replace(Ult.begin(), Ult.end(), T.lexema);
		return T;
	}

	if(LookAhead ==	'='){
		T.lexema.insert(T.lexema.end(), LookAhead);
		LookAhead = fgetc(arq);
		column += 1;
		if(LookAhead == '='){
			T.lexema.insert(T.lexema.end(), LookAhead);
			LookAhead = fgetc(arq);
			column += 1;
			T.id = Equals;
			Ult.replace(Ult.begin(), Ult.end(), T.lexema);
			return T;
		}
		else{
			T.id = Equal;
			Ult.replace(Ult.begin(), Ult.end(), T.lexema);
			return T;
		}
	}

	if(LookAhead == '!'){
		T.lexema.insert(T.lexema.end(), LookAhead);
		LookAhead = fgetc(arq);
		column += 1;
		if(LookAhead == '='){
			T.lexema.insert(T.lexema.end(), LookAhead);
			LookAhead = fgetc(arq);
			column += 1;
			T.id = NotE;
			Ult.replace(Ult.begin(), Ult.end(), T.lexema);
			return T;
		}
		else{
			cout<<"Erro na linha: "<<line<<" coluna: "<<column<<" operador nao identificado!"<<endl;
			cout<<"Ultimo token lido: "<<Ult<<endl;
			exit(3);
		}
	}

	if(LookAhead == '\''){
		T.lexema.insert(T.lexema.end(), LookAhead);
		LookAhead = fgetc(arq);
		column += 1;
		if(isalnum(LookAhead)){
			T.lexema.insert(T.lexema.end(), LookAhead);
			LookAhead = fgetc(arq);
			column += 1;
			if(LookAhead == '\''){
				T.lexema.insert(T.lexema.end(), LookAhead);
				LookAhead = fgetc(arq);
				column += 1;
				T.id = Lit;
				Ult.replace(Ult.begin(), Ult.end(), T.lexema);
				return T;
			}
			else{
				cout<<"Erro na linha: "<<line<<" coluna: "<<column<<" literal mal formado!"<<endl;
				cout<<"Ultimo token lido: "<<Ult<<endl;
			}
		}
		else{
			cout<<"Erro na linha: "<<line<<" coluna: "<<column<<" operador nao identificado!"<<endl;
			cout<<"Ultimo token lido: "<<Ult<<endl;
		}
	}
}

void bloco(FILE *arq){

	token T;

	T = Scanner(arq);

	if( T.id != AbC ){
		cout<<"Erro na linha: "<<line<<" coluna: "<<column<<endl;
		cout<<"Token esperado: '{'"<<endl;
		exit(9);
	}
	decla_var(arq);
	T = Scanner(arq);
}
void programa(FILE *arq, token T){

	if( T.id == 4 )
		T = Scanner(arq);
	else{
		cout<<"Erro na linha: "<<line<<" coluna: "<<column<<endl;
		cout<<"Token esperado: int"<<endl;
		exit(5);
	}

	if( T.id == 7 )
		T = Scanner(arq);
	else{
		cout<<"Erro na linha: "<<line<<" coluna: "<<column<<endl;
		cout<<"Token esperado: main"<<endl;
		exit(6);
	}

	if( T.id == AbP )
		T = Scanner(arq);
	else{
		cout<<"Erro na linha: "<<line<<" coluna: "<<column<<endl;
		cout<<"Token esperado: '('"<<endl;
		exit(7);
	}

	if( T.id == FeP )
		bloco(arq);
	else{
		cout<<"Erro na linha: "<<line<<" coluna: "<<column<<endl;
		cout<<"Token esperado: ')'"<<endl;
		exit(8);
	}
}

void decla_var(FILE *arq){

	token T;

	tipo(arq);
	do{
		Id(arq);
		T = Scanner(arq);
	}while(T.id == Comma);
	if(T.id != PoCom){
		cout<<"Erro na linha: "<<line<<" coluna: "<<column<<endl;
		cout<<"Esperado ';'!"<<endl;
		exit(12);
	}
}

void tipo(FILE *arq){

	token T;

	T = Scanner(arq);

	if(T.id != 4 || T.id != 5 || T.id != 6){
		cout<<"Erro na linha: "<<line<<" coluna: "<<column<<endl;
		cout<<"Declaracao de tipo de variavel nao encontrado!"<<endl;
		exit(10);
	}
}

void Id(FILE *arq){

	token T;

	T = Scanner(arq);

	if(T.id != Ident){
		cout<<"Erro na linha: "<<line<<" coluna: "<<column<<endl;
		cout<<"Identificador de variavel nao encontrado!"<<endl;
		exit(11);
	}
}


int main(int argc, char *argv[]){

	token t;
	FILE *arq;

	arq = fopen(argv[1], "rt");

	if( arq == NULL ){
		cout<<"Erro ao abrir arquivo!"<<endl;
		exit(4);
	}

	t = Scanner(arq);

	programa(arq, t);

	fclose(arq);

	return 0;

}