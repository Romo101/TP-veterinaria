#include<stdio.h>
#include<stdlib.h>

//Veterinaria
main(){
	int op;
	do{
			system("cls");
		op=4;
		_flushall();
	printf("\n\n\t\t    SISTEMA DE GESTION DE VETERINARIA \n\n\n\n");
	printf("\t\t   1.- Ingresar a Modulo de Administracion\n\n");
	printf("\t\t   2.- Ingresar a Modulo de Consultorio\n\n");
	printf("\t\t   3.- Ingresar a Modulo de Asistentes\n\n");
	printf("\t\t   0.- Salir\n\n");
	printf("\t\t   Ingrese una opcion: ");
	scanf("%d",&op);
	}while(op!=1 && op!=2 && op!=3 && op!=0);
	if(op==0)
	return 0;
	
	/*switch(op){
		case 1:	//administracion();
			break;
		case 2: //constultorio();
			break;
		case 3: //asistentes();
			break;
	}*/
		
system("pause");
}
