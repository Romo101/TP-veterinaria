#include<stdio.h>
#include<string.h>
#include<stdlib.h>


struct usuarios{
	char usuario[10];
	char contrasena[10];
	char Apellido_y_nombre[60];
};

void menuadm(int &op);
void opcionesadm(int op);

main(){
	char claveadministracion[]="VETeriAR012";
	char intentoclave[30];
	int intentos=4,opc=0;
	
	do{								//en este do while valido que la persona que entre al area de administracion
			if(intentos<4)			//tenga permiso para hacerlo
			printf("Tiene %d intentos\n\n",intentos);
		printf("Ingrese clave de administracion: ");
		_flushall();
		gets(intentoclave);
		intentos--;
		system("cls");
	}while(strcmp(intentoclave,claveadministracion)!=0 && intentos>0);
	
	if(intentos==0)
		printf("Contactese con el administrador\n\n");
	else{
		while(opc!=5){
		menuadm(opc);
		if(opc!=5)
		opcionesadm(opc);
		}
	}
	
	system("pause");
}
void menuadm(int &op){
	do{
		op=0;
		_flushall();
	printf("\n\n\t\t\t   MODULO ADMINISTRACION\n");
	printf("\t\t\t===========================\n\n");
	printf("\t\t\t1.- Registrar veterinario\n\n");
	printf("\t\t\t2.- Registrar asistente\n\n");
	printf("\t\t\t3.- Atencion por veterinarios\n\n");
	printf("\t\t\t4.- Ranking de Veterinarios por atencion\n\n");
	printf("\t\t\t5.- Cerrar modulo\n\n");
	printf("\t\t\tIngrese una opcion: ");
	scanf("%d",&op);
	system("cls");
	}while(op!=1 && op!=2 && op!=3 && op!=4 && op!=5);
}
void opcionesadm(int op){
	switch(op){
		case 1:	printf("Veterinarios\n\n");
			break;
		case 2: printf("Asistentes\n\n");
			break;
		case 3:	printf("Atencion por vet\n\n");
			break;
		case 4: printf("Ranking\n\n");
			break;	
	}
}
