/*
	TRABAJO INTEGRADOR 2DO CUATRIMESTRE
	
	MODULO DE ADMINISTRACION DE VETERINARIA
	
	ALUMNO: ROMO J. JONATHAN EMANUEL
	COMISION: 1K05
	DNI: 41374521
*/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>
#include<windows.h>
#include<locale.h> //librearia que me permitira colocar el idioma español

struct usuarios{
	bool veterinario;
	char usuario[10];
	char contrasena[32];
	char Apellido_y_nombre[60];
};
struct veterinarios{
	char Apellido_y_nombre[60];
	int matricula;
	int dni;
	int telefono;
};

void menuadm(int &op);
void opcionesadm(int op,FILE *&arc,FILE *&vet);
void registro_usuario_y_contra(FILE *&arc,int op,FILE *&vet);

main(){
	setlocale(LC_CTYPE, "spanish");
	char claveadministracion[]="VETeriAR012";	//clave
	char intentoclave[30];
	int intentos=4,opc=0;
	
	FILE *arc;
	FILE *vet;
	
	do{								//en este do while valido que la persona que entre al area de administracion
			if(intentos<4){		//tenga permiso para hacerlo, para eso se definio una clave de administracion
			printf("\n\t\tClave incorrecta\n\n\t\t");
			system("pause"); system("cls");
			printf("\n\t\tTiene %d intentos\n",intentos);
			}
			
			printf("\n\t\tIngrese clave de administración: ");
		_flushall();
		gets(intentoclave);
		intentos--;
	}while(strcmp(intentoclave,claveadministracion)!=0 && intentos>0); //se tendra cuatro intentos para ingresar la clave de administracion
	
	if(intentos==0)
		printf("\n\t\tClave incorrecta, contáctese con el administrador\n\n\t\t"); //si la clave no fue ingresada se saldra del modulo de administracion
	else{
		while(opc!=5){ 			//caso contrario se accedera al menu de administracion, (0p=5 saldra del modulo de administracion)
		system("cls");
		menuadm(opc);			//aqui se muestra el menu y devuelve la opcion elegida
		if(opc!=5)				
		opcionesadm(opc,arc,vet);	//si la opcion no es salir se ingresara a esta funcion que registra las distintas opciones
		}
	}
}
/*funcion sin tipo que muestra el menu y devuelve una opcion por referencia*/
void menuadm(int &op){
	do{
		op=0;
		_flushall();
	printf("\n\n\t\t\t   MÓDULO ADMINISTRACIÓN\n");
	printf("\t\t\t===========================\n\n");
	printf("\t\t\t1.- Registrar veterinario\n\n");
	printf("\t\t\t2.- Registrar asistente\n\n");
	printf("\t\t\t3.- Atención por veterinarios\n\n");
	printf("\t\t\t4.- Ranking de Veterinarios por atención\n\n");
	printf("\t\t\t5.- Cerrar módulo\n\n");
	printf("\t\t\tIngrese una opción: ");
	scanf("%d",&op);
	system("cls");
	}while(op!=1 && op!=2 && op!=3 && op!=4 && op!=5);
}
/*funcion que registra el trabajo registrado en cada opcion y modifica los distintos archivos por referencia*/
void opcionesadm(int op,FILE *&arc,FILE *&vet){
	switch(op){
		case 1:	registro_usuario_y_contra(arc,1,vet);
			break;
		case 2: registro_usuario_y_contra(arc,2,vet);
			break;
		case 3:	printf("Atencion por vet\n\n");
			break;
		case 4: printf("Ranking\n\n");
			break;	
	}
}
/*Funcion que devuelve true o false a la funcion "void usuario()" 
aqui se valida que el usuario ingresado y validado no se encuentre
en uso*/
bool repetido(char user[10],FILE *&arc){
	usuarios users;
	
	arc=fopen("usuarios.dat","a+b");	//abro el archivo usuario, si no existe lo creo
	rewind(arc);						//pongo el puntero al principio
	
	fread(&users,sizeof(usuarios),1,arc); //leo el primer registro del archivo
	while(!feof(arc)){								//si no es el fin del archivo
	
	/*se comprueba si el usuario no esta ya registrado en el archivo*/
		if(strcmp(user,users.usuario)==0){
		printf("\n");
		system("color 0c");
		printf("\tX El '%s' usuario no está disponible ingrese uno nuevo\n\t",user);
		system("pause");
		system("color 07");
		fclose(arc);
		return true;		//si esta retorno "true".
		}
		fread(&users,sizeof(usuarios),1,arc);
	}
	fclose(arc);
	
	return false;	//si no esta retorno "false"
}
/*funcion que registra los ususarios, los valida y los devuelve a la funcion
"void registro_usuario_y_contra()". De esta funcion se desprende la funcion
bool repetido()*/
void usuario(char user[],FILE *&arc,char nombre[],char opcion[]){
	int condiciones,may,num,coinc,otros;
	char conjunto[]= "+-*/?¿¡!";
	
	do{
	do{
		system("cls");
		condiciones=5,may=0,num=0,otros=0;
		
	printf("\n\t\t%s\n\n\n\t\tApellido/s y Nombre/s: %s\n\n\t\tINGRESE UN NOMBRE DE USUARIO: ",opcion,nombre);
	_flushall();
	gets(user);
	printf("\n");
	if(strlen(user)<6 or strlen(user)>10){
		printf("\tX Debe tener entre seis y diez caracteres\n");
		condiciones--;
	}
	if(user[0]<'a' or user[0]>'z'){
		printf("\tX La primera letra debe ser minúscula\n");
		condiciones--;
	}
	for(int i=0;i<strlen(user);i++){
		if(user[i]>='A' and user[i]<='Z')
			may++;
		if(isdigit(user[i]))
			num++;
		if(!isdigit(user[i]) && !isalpha(user[i]) && otros==0){
			coinc=0;
			for(int j=0;j<strlen(conjunto);j++){
				if(user[i]==conjunto[j])
				coinc++;
				if(j==strlen(conjunto)-1 and coinc==0)
				otros++;
			}
		}
	}
	if(may<2){
		printf("\tX Debe tener al menos dos letras mayúsculas\n");
		condiciones--;
	}
	if(num>3){
		printf("\tX Debe tener como máximo tres números\n");
		condiciones--;
	}
	if(otros>0){
		printf("\tX Solo se admite los simbolos del conjunto {+,-,*,/,?,¿,¡,!}\n");
		condiciones--;
	}
	if(condiciones<5){
		printf("\n\t");
		system("color 0e");
		system("pause");
		system("color 07");
		}
	}while(condiciones<5);
	}while(repetido(user,arc));	
}
/*Funcion que valida una contraseña ingresada y la devuelve a la funcion
void registro_usuario_y_contra()*/
void contra(char pass[],char user[],char nombre[],char opcion[]){
	char aux[32];
	int i,min,may,num,simbolos,condic,num_consec,letras_consec;
	
	do{
		system("cls");
		condic=5,min=0,may=0,num=0,simbolos=0,num_consec=0,letras_consec=0;
		
		printf("\n\t\t%s\n\n\n\t\tApellido/s y Nombre/s: %s\n\n\t\tUSUARIO: %s\n\n\t\tESTABLEZCA UNA CONTRASEÑA: ",opcion,nombre,user);
		_flushall();
		gets(pass);
		strlwr(strcpy(aux,pass));
		printf("\n\n");
		
	if(strlen(pass)>32 or strlen(pass)<6){							//condicion de tamaño
		printf("\tX Debe tener entre 6 y 32 carácteres\n");
		condic--;
	}
		/*---- decidi agruparlos asi para que el codigo sea mas claro ----*/
		for(i=0;i<strlen(pass);i++){
		if(pass[i]>='a' and pass[i]<='z') //condicion de minusculas, si hay minusculas aumenta contador de minusculas
			min++;
		if(pass[i]>='A' and pass[i]<='Z') //condicion de mayusculas, si hay mayusculas aumenta su contador
			may++;
		if(pass[i]>='0' and pass[i]<='9') //condicion de numeros, si hay numeros aumenta su contador
			num++;
		if(!isdigit(pass[i]) && !isalpha(pass[i])) //condicion de no simbolos, si hay simbolos aumenta su contador
			simbolos++;
		}
	
		if(min==0 or may==0 or num==0){ //En caso de no haber encontrado al menos una minuscula, una mayuscula y un numero
			printf("\tX Debe tener al menos una letra minúscula, una mayúsula y un número\n");
			condic--;
		}
		if(simbolos!=0){ //si encontro algun simbolo
			printf("\tX No debe tener símbolos, puntuaciones ni espacios\n");
			condic--;
		}
		/*---- aqui comprobare las letras y numeros consecutivos usando la cadena auxiliar ---*/
		for(i=0;i<strlen(aux);i++){
			if(isdigit(aux[i]) && num_consec==0){			//si un caracter es numerico y su contador es 0
				if(isdigit(aux[i-1]) and isdigit(aux[i-2]) && isdigit(aux[i-3])) //se preguntara si los tres caracteres anteriores son numericos
					 num_consec++;												//si lo son, se incrementara su contador a 1
			}
			if(isalpha(aux[i]) && letras_consec==0){	//i un caracter es alfabetico y su contador es 0
				if(aux[i]-1==aux[i-1])					//se preguntara si el caracter anterior es igual al caracter actual - 1 (b-1 = a;)
					letras_consec++;					//si lo son entonces se incrementara el contador a 1
			}
			if(letras_consec>0 && num_consec>0)			//si ya se encontraron numeros consecutivos y letras ascendentes
				break;									//saldra del ciclo sin analizar toda la palabra
		}
		if(num_consec>0){				
			printf("\tX No debe tener más de tres carácteres numéricos consecutivos\n");
			condic--;
		}
		if(letras_consec>0){
			printf("\tX No debe tener dos carácteres alfabéticos ascendentes y consecutivos\n");
			condic--;
		}
		
		if(condic<5){
			printf("\n\t");
			system("color 0e");
			system("pause");
			system("color 07");
		}
	}while(condic<5);	
}
bool matricula(FILE *vet,int mat){
	vet=fopen("Veterinarios.dat","rb");
	veterinarios vete;
	
	if(vet==NULL)
		return true;
	else{
		fread(&vete,sizeof(veterinarios),1,vet);
		while(!feof(vet)){
			if(mat==vete.matricula){
				fclose(vet);
				return false;
			}
		fread(&vete,sizeof(veterinarios),1,vet);	
		}
			fclose(vet);
			return true;
	}
}

void registro_veterinarios(FILE *&vet,char nombre[],char user[],char pass[],int &opc){
	veterinarios vets;
	char op[3];
	int mat;
	
	regresar:
	system("cls");
	printf("\n\t\tREGISTRO DE VETERINARIOS\n\n\n\t\tUsuario: %s\n\n",user);
	printf("\t\tContraseña: %s",pass);
	printf("\n\n\t\tApellido/s y Nombre/s: %s",nombre);
	printf("\n\n\t\tMatricula: ");
	scanf("%d",&mat);
	if(!matricula(vet,mat)){
			system("color 0e");
		printf("\n\n\t\tLa matrícula %d fue registrada anteriormente\n\n",mat);
		printf("\t\tDebe ingresar una matricula distinta.\n\t\tDe lo contrario se cancelará el registro\n\n");
		printf("\t\t¿Ingresar otra matrícula? S/N: ");
		_flushall();
		gets(op);
		system("color 07");
		if(strcmp(op,"s")==0 or strcmp(op,"S")==0)
			goto regresar;
		else{
			printf("\n\n\t\tREGISTRO CANCELADO\n\n\t\t");
			system("pause");
			opc=0;
		}
	}else{
	printf("\n\t\tDNI: ");
	scanf("%d",&vets.dni);
	printf("\n\t\tTelefono: ");
	scanf("%d",&vets.telefono);
	strcpy(vets.Apellido_y_nombre,nombre);
	vets.matricula=mat;
	
	vet=fopen("Veterinarios.dat","a+b");
	fwrite(&vets,sizeof(veterinarios),1,vet);
	fclose(vet);
	}
}
/*cuando la opcion es registrar usuarios (veterinarios o asistentes)
De esta funcion se desprenden otras tres "void usuario(),bool repetido() y void contra()
Funciones que se encuentran mas arriba"*/
void registro_usuario_y_contra(FILE *&arc,int op,FILE *&vet){
	char user[10],password[32],nombre[60],opcion[30];
	int pasa=0;
	usuarios users;
	veterinarios vets;
	
	if(op==1)								   // cuando la opcion registrada en el menu sea 1 (registro de veterinarios)
	strcpy(opcion,"REGISTRO DE VETERINARIOS"); 
	else									   // cuando la opcion registrada en el menu sea 2 (registro de asistentes)
	strcpy(opcion,"REGISTRO DE ASISTENTES");
	
	printf("\n\t\t%s",opcion);
	printf("\n\n\n\t\tApellido/s y Nombre/s: ");
	_flushall();
	gets(nombre);
	
	usuario(user,arc,nombre,opcion); //se ingresara el usuario, lo validara y lo devolvera por referencia
	 
	contra(password,user,nombre,opcion); //se ingresa la contraseña, se la valida y se la devuelve a esta funcion

	arc=fopen("usuarios.dat","a+b");
	strcpy(users.usuario,user);
	strcpy(users.contrasena,password);
	strcpy(users.Apellido_y_nombre,nombre);
	
	if(op==1){
	users.veterinario=true;
	registro_veterinarios(vet,nombre,user,password,op);
	}
	else{
		users.veterinario=false;
		system("cls");
		printf("\n\t\t%s\n\n\n\t\tUsuario: %s\n\n",opcion,user);
		printf("\t\tContraseña: %s",password);
		printf("\n\n\t\tApellido/s y Nombre/s: %s",nombre);
	}
	
	if(op==1 or op==2){
	fwrite(&users,sizeof(usuarios),1,arc);
	system("color 0a");
	printf("\n\n\t\tREGISTRADO\n\n\t\t");
	system("pause");
	system("color 07");
	fclose(arc);
	}
}
