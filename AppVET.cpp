#include<stdio.h>
#include<stdlib.h>
#include<locale.h>

//Veterinaria
main(){
	setlocale(LC_CTYPE, "spanish");
	int op;
	
	do{
	do{
		system("cls");
		op=4;
		_flushall();
	printf("\n\n\t\t    SISTEMA DE GESTI�N DE VETERINARIA \n\n\n\n");
	printf("\t\t   1.- Ingresar al M�dulo de Administraci�n\n\n");
	printf("\t\t   2.- Ingresar al M�dulo de Consultorio\n\n");
	printf("\t\t   3.- Ingresar al M�dulo de Asistentes\n\n");
	printf("\t\t   0.- Salir\n\n");
	printf("\t\t   Ingrese una opci�n: ");
	scanf("%d",&op);
	}while(op!=1 && op!=2 && op!=3 && op!=0);
	
	switch(op){
		case 1: system("MOD-Administracion.exe");
			break;
		case 2: system("MOD-Consultorio.exe");
			break;
		case 3: system("MOD-Cons.Asistente.exe");
			break;
	}
	}while(op!=0);
	
}
