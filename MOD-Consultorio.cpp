#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void menuvet(int &op);
void opcionesvet(int op);

main(){
	int op=0;

	while(op!=4){
		menuvet(op);
		if(op!=4)
		opcionesvet(op);
	}
}
void menuvet(int &op){
	do{
		op=0;
		_flushall();
		printf("\n\n\t\t\tMODULO CONSULTORIO VETERINARIO\n");
		printf("\t\t\t==============================\n\n");
		printf("\t\t\t1.- Iniciar sesion\n\n");
		printf("\t\t\t2.- Visualizar lista de espera (informe)\n\n");
		printf("\t\t\t3.- Registrar evolucion de la mascota\n\n");
		printf("\t\t\t4.- Cerrar modulo\n\n");
		printf("\t\t\tIngrese una opcion: ");
		scanf("%d",&op);
		system("cls");
	}while(op!=1 && op!=2 && op!=3 && op!=4);
}
void opcionesvet(int op){
	switch(op){
		case 1:	printf("inicio de sesion\n\n");
			break;
		case 2:	printf("lista de espera\n\n");
			break;
		case 3: printf("registrar evolucion\n\n");
			break;
	}
}
