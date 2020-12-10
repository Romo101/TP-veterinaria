#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<windows.h>
#include<locale.h>

void gotoxy(int x, int y){
	HANDLE hcon = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD dwPos;
	dwPos.X = x;
	dwPos.Y = y;
	SetConsoleCursorPosition(hcon, dwPos);
}

struct usuarios{
	bool veterinario;
	char usuario[10];
	char contrasena[32];
	char Apellido_y_nombre[60];
};

void menuasist(int &op);
void opcionesasist(int &op,bool &iniciada);
void iniciar_sesion(bool &iniciada,int &op);

main(){
	setlocale(LC_CTYPE, "spanish");
	int op=0;
	bool sesion_iniciada=false;
	
	while(op!=5){
		menuasist(op);
		if(op!=5)
		opcionesasist(op,sesion_iniciada);
	}
}
void menuasist(int &op){
		system("cls");
	do{
		op=0;
		_flushall();
		printf("\n\n\t\t\t   MODULO DEL ASISTENTE\n");
		printf("\t\t\t==========================\n\n");
		printf("\t\t\t  1.- Iniciar sesion\n\n");
		printf("\t\t\t  2.- Registrar mascota\n\n");
		printf("\t\t\t  3.- Registrar Turno\n\n");
		printf("\t\t\t  4.- Listado de atenciones\n\n");
		printf("\t\t\t  5.- Cerrar modulo\n\n");
		printf("\t\t\t  Ingrese una opcion: ");
		scanf("%d",&op);
		system("cls");
	}while(op!=1 && op!=2 && op!=3 && op!=4 && op!=5);
}
void opcionesasist(int &op,bool &iniciada){
	switch(op){
		case 1:	if(iniciada==false)
				iniciar_sesion(iniciada,op);
				else{
					system("color 0a");
					printf("\n\n\n\n\t\t\t   SESION YA INICADA");
					system("pause ->NUL");
					system("color 07");
				}
			break;
		case 2:	printf("registrar mascota\n\n");
			break;
		case 3: printf("registrar turno\n\n");
			break;
		case 4: printf("Listado de atencion\n\n");
	}
}
bool comprobar_hay_asistentes(FILE* arc){
	usuarios users;
	
		fread(&users,sizeof(usuarios),1,arc);
		while(!feof(arc)){
			if(users.veterinario==false)
				return true;
		fread(&users,sizeof(usuarios),1,arc);
		}
		return false;
}
void iniciar_sesion(bool &iniciada,int &op){
	usuarios users;
	char usuario[10],pass[32],opc[2];
	int coinc;
	
	FILE* arch=fopen("usuarios.dat","rb");
	if(arch==NULL){
		Beep(700,300);
		system("color 0e"); 
		printf("\n\n\n\t\tAún no se registraron usuarios en el sistema\n\n\t\t");
		printf("Contacte a administración para ser registrado");
		system("pause ->NUL");
		op=5;
	}
	else{
		if(!comprobar_hay_asistentes(arch)){
			Beep(700,300);
			system("color 0e"); 
		printf("\n\n\n\t\tAún no se registraron usuarios ASISTENTES\n\n\t\t");
		printf("Contacte a administración para ser registrado");
		system("pause ->NUL");
		op=5;
		}
		else{
			do{
			rewind(arch);
			
			system("cls");
			printf("\n\n\n\t\tUsuario: ");
			_flushall();	gets(usuario);
			printf("\n\t\tContraseña: ");
			_flushall();	gets(pass);
			
			fread(&users,sizeof(usuarios),1,arch);
			while(!feof(arch) and iniciada==false){
				coinc=0;
				if(strcmp(usuario,users.usuario)==0)
					coinc++;
				if(strcmp(pass,users.contrasena)==0){
					if(coinc>0)
						iniciada=true;
				}
				if(iniciada==false)
					fread(&users,sizeof(usuarios),1,arch);
			}
			if(iniciada==false){
				Beep(700,300);
				system("color 0e");
				printf("\n\n\t\tUsuario y/o contraseña incorrecta");
				printf("\n\n\t\t¿Volver a ingresar? S/N: ");
				_flushall();	gets(opc);	strlwr(opc);
				system("color 07");
			}else{
				system("color 0a");
				printf("\n\n\t\tBIENVENIDO: %s",users.Apellido_y_nombre);
				system("pause ->NUL");
			}
			}while(iniciada==false and strcmp(opc,"s")==0);
		}
		fclose(arch);
		system("color 07");	
	}
}
