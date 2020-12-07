#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void menuasist(int &op);
void opcionesasist(int op);

main(){
	int op=0;

	while(op!=5){
		menuasist(op);
		if(op!=5)
		opcionesasist(op);
	}
}
void menuasist(int &op){
	do{
		op=0;
		_flushall();
		printf("\n\n\t\t\t   MODULO DEL ASISTENTE\n");
		printf("\t\t\t==========================\n\n");
		printf("\t\t\t1.- Iniciar sesion\n\n");
		printf("\t\t\t2.- Registrar mascota\n\n");
		printf("\t\t\t3.- Registrar Turno\n\n");
		printf("\t\t\t4.- Listado de atenciones\n\n");
		printf("\t\t\t5.- Cerrar modulo\n\n");
		printf("\t\t\tIngrese una opcion: ");
		scanf("%d",&op);
		system("cls");
	}while(op!=1 && op!=2 && op!=3 && op!=4 && op!=5);
}
void opcionesasist(int op){
	switch(op){
		case 1:	printf("inicio de sesion\n\n");
			break;
		case 2:	printf("registrar mascota\n\n");
			break;
		case 3: printf("registrar turno\n\n");
			break;
		case 4: printf("Listado de atencion\n\n");
	}
}
