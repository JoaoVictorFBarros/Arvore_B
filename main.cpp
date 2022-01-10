#include <iostream>
#include "b_tree.hpp"
using namespace std;

class client {
	public:
		string name;
		string CPF;
		string age;
		
		void print (){
			cout<<"Nome: "<<name<<"CPF: "<<CPF<<"Idade: "<<age<<"\n\n";
		}
};

int main (){
	
	setlocale (LC_ALL,"portuguese");	
		
	FILE *f;
	f = fopen("clients.txt","r");
	client clients[1000]; 
	
	for(int i=0;i<1000;i++){
		
		char cpf[11],name[200],age[2],empty[1];
		string aux;
		
		fgets(cpf,13,f);
		fgets(name,200,f);
		fgets(age,40,f);
		
		aux.push_back(age[0]);
		aux.push_back(age[1]);
		
		fgets(empty,3,f);
		clients[i].name = name;
		clients[i].CPF = cpf;
		clients[i].age = aux;
	}
	fclose(f);
	
	page *p = NULL;
	
	for (int i=0;i<1000;i++){
		record r;
		for (int j=0;j<2;j++){
			r.age[j] = clients[i].age[j];
		}
		r.age[2] = '\0';
		
		int cont = 0;
		
		while (clients[i].name[cont] != '\0'){
			r.name[cont] = clients[i].name[cont]; 
			cont++;
		}
		r.name[cont] = '\0';
		
		r.CPF = stod(clients[i].CPF);
		
		insert(&p,r);
	}	
	
	short op = 0;
	
	while (op != 4){
		cout<<"1: Inserir\n2: Pesquisar\n3: remover\n4: Sair\n\nOperação: ";
		cin>>op;
		
		switch (op){
			case 1:{
				record r;
				cout<<"Digite o nome: ";
				cin>>r.name;
				cout<<"Digite a idade: ";
				cin>>r.age;
				cout<<"Digite o CPF: ";
				cin>>r.CPF;
				
				insert(&p,r);
				cout<<"\nCliente inserido.\n\n";
				break;
			}
			case 2:{
				record r;
				cout<<"Digite o CPF: ";
				cin>>r.CPF;
				search(p,&r);
				break;
			}
			case 3:{
				record r;
				bool *underflow;
				bool aux = true;
				underflow = &aux;
				cout<<"Digite o CPF: ";
				cin>>r.CPF;
				remove(&p,r.CPF,underflow);
				cout<<"\nCliente removido. \n\n";
				break;
			}
		}
		system ("pause");
		system ("cls");
	}
	
	return 0;
}
