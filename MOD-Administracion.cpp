#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>
#include<windows.h>

struct usuarios{
	char usuario[10];
	char contrasena[32];
	char Apellido_y_nombre[60];
};

void menuadm(int &op);
void opcionesadm(int op,FILE *&arc);
void registro_usuario_y_contra(FILE *&arc);

main(){
	char claveadministracion[]="VETeriAR012";
	char intentoclave[30];
	int intentos=4,opc=0;
	
	FILE *arc;
	
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
		opcionesadm(opc,arc);
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
void opcionesadm(int op,FILE *&arc){
	switch(op){
		case 1:	registro_usuario_y_contra(arc);
			break;
		case 2: printf("Asistentes\n\n");
			break;
		case 3:	printf("Atencion por vet\n\n");
			break;
		case 4: printf("Ranking\n\n");
			break;	
	}
}
bool repetido(char user[10],FILE *&arc){
	usuarios users;
	
	arc=fopen("usuarios.dat","a+b");
	rewind(arc);
	
	fread(&users,sizeof(usuarios),1,arc);
	while(!feof(arc)){
		if(strcmp(user,users.usuario)==0){
		printf("\n");
		system("color 0c");
		printf("X Usuario '%s' ya registrado ingrese uno nuevo\n",user);
		system("pause");
		system("color 07");
		fclose(arc);
		return true;
		}
		fread(&users,sizeof(usuarios),1,arc);
	}
	fclose(arc);
	
	return false;
}
void usuario(char user[10],FILE *&arc){
	int condiciones,may,num;
	
	do{
	do{
		system("cls");
		condiciones=4;
		may=0;
		num=0;
	printf("\nIngrese un nombre de usuario: ");
	_flushall();
	gets(user);
	printf("\n");
	if(strlen(user)<4 or strlen(user)>10){
		printf("X El usuario ingresado debe tener entre cuatro y diez caracteres\n");
		condiciones--;
	}
	if(user[0]<'a' or user[0]>'z'){
		printf("X La primera letra del usuario ingresado debe ser minuscula\n");
		condiciones--;
	}
	for(int i=0;i<strlen(user);i++){
		if(user[i]>='A' and user[i]<='Z')
			may++;
		if(isdigit(user[i]))
			num++;
	}
	if(may<2){
		printf("X El usuario ingresado debe tener al menos dos letras mayusculas\n");
		condiciones--;
	}
	if(num>3){
		printf("X El usuario puede tener como maximo tres numeros\n");
		condiciones--;
	}
	if(condiciones<4){
		printf("\n");
		system("color 0c");
		system("pause");
		system("color 07");
		}
	}while(condiciones<4);
	}while(repetido(user,arc));
	
}
void registro_usuario_y_contra(FILE *&arc){
	char user[10],pass[10],nombre[30];
	
	usuarios users;
	
	printf("\n\t\tCREACION DE USUARIO\n\nCondiciones de usuarios validos:\n\n");
	printf(". No repetidos\n. Comenzar con minusculas\n. Tener al menos dos letras mayusculas\n");
	printf(". Tener como maximo tres numeros\n. Entre cuatro y diez caracteres\n\n");
	system("pause");
	
	usuario(user,arc);
	printf("Usuario valido, Ingrese una contrasena: ");
	_flushall();
	gets(pass);
	printf("Y un nombre");
	_flushall();
	gets(nombre);
	
	arc=fopen("usuarios.dat","a+b");
	strcpy(users.usuario,user);
	strcpy(users.contrasena,pass);
	strcpy(users.Apellido_y_nombre,nombre);
	fwrite(&users,sizeof(usuarios),1,arc);
	printf("REGISTRADo");
	fclose(arc);
}
/*
arc=fopen("usuarios.dat","a+b");
	fwrite(&users,sizeof(usuarios),1,arc);
	system("color 0a");
	printf("\tUsuario '%s' registrado correctamente\n\n",user);
	system("pause");
	system("color 07");
*/
