#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>
#include<windows.h>
#include<locale.h>

struct usuarios{
	char usuario[10];
	char contrasena[32];
	char Apellido_y_nombre[60];
};

void menuadm(int &op);
void opcionesadm(int op,FILE *&arc);
void registro_usuario_y_contra(FILE *&arc);

main(){
	setlocale(LC_CTYPE, "spanish");
	char claveadministracion[]="VETeriAR012";
	char intentoclave[30];
	int intentos=4,opc=0;
	
	FILE *arc;
	
	do{								//en este do while valido que la persona que entre al area de administracion
			if(intentos<4){		//tenga permiso para hacerlo
			printf("\n\t\tClave incorrecta\n\n\t\t");
			system("pause"); system("cls");
			printf("\n\t\tTiene %d intentos\n",intentos);
			}
			
			printf("\n\t\tIngrese clave de administración: ");
		_flushall();
		gets(intentoclave);
		intentos--;
	}while(strcmp(intentoclave,claveadministracion)!=0 && intentos>0);
	
	if(intentos==0)
		printf("\n\t\tClave incorrecta, contáctese con el administrador\n\n\t\t");
	else{
		while(opc!=5){
		system("cls");
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
		printf("\tX El '%s' usuario no está disponible ingrese uno nuevo\n\t",user);
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
	int condiciones,may,num,coinc,otros;
	char conjunto[]= "+-*/?¿¡!";
	
	do{
	do{
		system("cls");
		condiciones=5,may=0,num=0,otros=0;
		
	printf("\n\n\n\t\tINGRESE UN NOMBRE DE USUARIO: ");
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
void contra(char pas[]){
	char aux[32],pass[32];
	int i,min,may,num,simbolos,condic,num_consec,letras_consec;
	
	do{
		system("cls");
		condic=5,min=0,may=0,num=0,simbolos=0,num_consec=0,letras_consec=0;
		
		printf("\n\n\n\t\tINGRESE UNA CONTRASEÑA: ");
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
	
	system("color 0a");
	printf("\t\tCONTRASEÑA VALIDA\n\n\t\t");
	system("pause");	system("cls");
	system("color 07");	
}
void registro_usuario_y_contra(FILE *&arc){
	char user[10],password[32],nombre[30];
	
	usuarios users;
	
	printf("\n\n\t\t\tCREACIÓN DE USUARIO\n\n\t\tCondiciones de usuarios válidos:\n\n");
	printf("\t\t. Que no esté en uso\n\t\t. Comenzar con minúsculas\n\t\t. Tener al menos dos letras mayúsculas\n");
	printf("\t\t. Tener como máximo tres numeros\n\t\t. Entre seis y diez carácteres\n");
	printf("\t\t. Admitirá los simbolos del conjunto: {+,-,*,/,¿,?,¡,!}\n\n\t\t");
	system("pause");
	
	usuario(user,arc);
	system("color 0a");
	printf("\t\tUsuario válido\n\n\t\t");
	system("pause");	system("cls");
	system("color 07");
	
	printf("\n\t\t\tCREACIÓN DE CONTRASEÑAS\n\n\tCondiciones de contraseñas válidas:\n\n");
	printf("\t. Tener al menos una letra máyuscula, una minúscula y un número\n");
	printf("\t. No contener símbolos, acentos ni espacios\n\t. Tener entre 6 y 32 carácteres\n");
	printf("\t. No tener más de tres números consecutivos\n\t. No tener 2 letras seguidas y en órden alfabético\n\n\t");
	system("pause");
	
	contra(password);
	/*_flushall();
	gets(pass);
	printf("Y un nombre");
	_flushall();
	gets(nombre);
	
	arc=fopen("usuarios.dat","a+b");
	strcpy(users.usuario,user);
	strcpy(users.contrasena,pass);
	strcpy(users.Apellido_y_nombre,nombre);
	fwrite(&users,sizeof(usuarios),1,arc);
	printf("REGISTRADO");
	fclose(arc);*/
}
/*
arc=fopen("usuarios.dat","a+b");
	fwrite(&users,sizeof(usuarios),1,arc);
	system("color 0a");
	printf("\tUsuario '%s' registrado correctamente\n\n",user);
	system("pause");
	system("color 07");
*/
