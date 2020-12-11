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
struct veterinarios{
	char Apellido_y_nombre[60];
	int matricula;
	int dni;
	int telefono;
	int atenciones;
};

void menuadm(int &op);
void opcionesadm(int op,FILE *&arc,FILE *&vet);
void registro_usuario_y_contra(FILE *&arc,int op,FILE *&vet);
void lista_veterinarios(FILE *vet,int numero);
void ordenar_ranking(FILE *&vet);

main(){
	setlocale(LC_CTYPE, "spanish"); //me permite usar el alfabeto y los signos del español
	int intentos=4,opc=0;
	/*A los archivos se los declaro de esta manera y se los pasa a las funciones por referencia
	con fin de que el codigo sea un poco mas legible*/
	FILE *arc; //archivo que se usara para los usuarios
	FILE *vet; //para veterinarios

		while(opc!=5){ 		//(0p=5 saldra del modulo de administracion)
		system("cls");
		menuadm(opc);			//aqui se muestra el menu y devuelve la opcion elegida
		if(opc!=5)				
		opcionesadm(opc,arc,vet);	//si la opcion no es salir se ingresara a esta funcion que registra las distintas opciones
		}
}
/*funcion sin tipo que muestra el menu y devuelve una opcion por referencia*/
void menuadm(int &op){
	do{
		op=0;
		_flushall();
	printf("\n\n\n\t\t\t   MÓDULO ADMINISTRACIÓN\n");
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
		case 3:	lista_veterinarios(vet,0);
			break;
		case 4: ordenar_ranking(vet);
		printf("-------------------------------------------------------------------------------\n");
		printf("                          RANKING DE VETERINARIOS                              \n");
		lista_veterinarios(vet,2);
			break;	
	}
}
/*Funcion que devuelve true o false a la funcion "void usuario()" 
aqui se valida que el usuario ingresado y validado no se encuentre
en uso*/
bool repetido(char user[10],FILE *&arc){
	usuarios users;
	arc=fopen("usuarios.dat","rb");	//abro el archivo usuario para leerlo
	
	if(arc==NULL)
	return false;      //si el archivo no existe entonces el usuario no podria repetirse asi que retorno false
	else{
	fread(&users,sizeof(usuarios),1,arc); //leo el primer registro del archivo
	while(!feof(arc)){								//si no es el fin del archivo
	
	/*se comprueba si el usuario esta ya registrado en el archivo*/
		if(strcmp(user,users.usuario)==0){
		printf("\n");
		system("color 0e");
		Beep(700,300);
		printf("\tX El  usuario '%s' no está disponible ingrese uno nuevo\n\n\t",user);
		system("pause ->NUL");
		system("color 07");
		fclose(arc);
		return true;		//si esta retorno "true".
		}
		fread(&users,sizeof(usuarios),1,arc);
	}
	fclose(arc);
	return false;	//si no esta retorno "false"
	}
}
/*funcion que registra los ususarios, los valida y los devuelve a la funcion
"void registro_usuario_y_contra()". De esta funcion se desprende la funcion
bool repetido()*/
void usuario(char user[],FILE *&arc,char nombre[],char opcion[]){
	int condiciones,may,num,coinc,otros;
	char conjunto[]= "+-*/?¿¡!";  //conjunto de simbolos permitidos
	
	do{ //dos ciclos do while
	do{
		system("cls");
		condiciones=5,may=0,num=0,otros=0;  //las condiciones son 5, luego los contadores de las 
											//mayusculas, minusculas y numeros empiezan en 0
	printf("\n\t\t%s\n\n\n\t\tApellido/s y Nombre/s: %s\n\n\t\tINGRESE UN NOMBRE DE USUARIO: ",opcion,nombre);
	_flushall();
	gets(user);
	printf("\n");
	if(strlen(user)<6 or strlen(user)>10){						//aqui se compara el tamaño del usuario con las condiciones
		printf("\tX Debe tener entre seis y diez caracteres\n");
		condiciones--;										//si no se cumplen se resta en uno las condiciones
	}
	if(user[0]<'a' or user[0]>'z'){                        //se comprueba que la primera letra sea minuscula
		printf("\tX La primera letra debe ser minúscula\n");
		condiciones--;
	}
	for(int i=0;i<strlen(user);i++){ 			//aqui se comprueba la cantidad de mayusculas, numeros y si hay simbolos no permitidos
		if(user[i]>='A' and user[i]<='Z')	//cuenta mayusculas
			may++;
		if(isdigit(user[i]))		//cuenta numeros con la funcion que comprueba que un caracter es numero
			num++;
		if(!isdigit(user[i]) && !isalpha(user[i]) && otros==0){ //si el caracter es simbolo se comprobara si es permitido
																//si ya se entro anteriormente en este ciclo y se encontro caracter no valido
																//entonces no entrara (por cada contraseña invalida)
			coinc=0;
			for(int j=0;j<strlen(conjunto);j++){  //se comprueba cada simbolo con el conjunto permitido
				if(user[i]==conjunto[j])			//si el simbolo comparado se encuentra en el conjunto
				coinc++;										//se aumenta un contador de simbolos permitidos
				if(j==strlen(conjunto)-1 and coinc==0)	//si ya se comparo el simbolo con todos los simbolos del conjunto y no coincidio con ninguno permitido
				otros++;								//se aumentara un contador que determina si la palabra tiene simbolos no permitidos
			}											//una vez encontrado uno no permitido ya no se comparara los demas caracterses
		}
	}
	if(may<2){		//si no hay al menos dos mayusculas
		printf("\tX Debe tener al menos dos letras mayúsculas\n");
		condiciones--;
	}
	if(num>3){ //si hay mas de tres numeros
		printf("\tX Debe tener como máximo tres números\n");
		condiciones--;
	}
	if(otros>0){ //si se encontro caracteres simbolos no validos
		printf("\tX Solo se admite los simbolos del conjunto {+,-,*,/,?,¿,¡,!}\n");
		condiciones--;
	}
	if(condiciones<5){
		system("color 0e");
		Beep(700,300);
		printf("\n\t");
		system("pause ->NUL");
		system("color 07");
		}
	}while(condiciones<5);   //cuando no se cumple con los cincos requisitos se repite
	}while(repetido(user,arc));	//una vez cumplido los cinco requisitos se compara el usuario con
				//los usuarios ya registrados para comprobar que no se repitan si lo hacen se vuelve a ingresar otro usuario
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
			Beep(700,300);
			system("pause ->NUL");
			system("color 07");
		}
	}while(condic<5);	
}
/*funcion que valida las matriculas ingresadas en la funcion
"void registro_veterinarios()"*/
bool matricula(FILE *vet,int mat){
	vet=fopen("Veterinarios.dat","rb"); 	//se abre el archivo de veterinarios con permiso de lectura
	veterinarios vete;
	
	if(vet==NULL)						//si el archivo aun no existe entonces la matricula ingresada
		return true;					//se valida
	else{
		fread(&vete,sizeof(veterinarios),1,vet);	//cuando el archivo exite se lee el primer registro
		while(!feof(vet)){
			if(mat==vete.matricula){				//se lo compara con la matricula ingresada si son iguales
				fclose(vet);						//la matricula no sera valida
				return false;
			}
		fread(&vete,sizeof(veterinarios),1,vet);	//si la matricula no se encuentra se lee el siguiente registro del archivo
		}
			fclose(vet);
			return true;		//una vez leido el archivo completo y no encontrada la matricula ingresada entonces se valida
	}
}
/*En esta funcion se registran los datos de los veterinarios luego de validar usuario
y contraseña, tambien se validan las matriculas viendo que no se registren dos matriculas
iguales para eso se desprende una funcion "bool matricula()" en caso de encontrar la matricula
se dara la opcion de cancelar ingreso de datos como la de volver a ingresar otra matricula*/
void registro_veterinarios(FILE *&vet,char nombre[],char user[],char pass[],int &opc){
	veterinarios vets;
	char op[3];			//este guardara la opcion ingresada cuando la matricula se repita
	int mat;
	
	regresar:
	system("cls");
	printf("\n\t\tREGISTRO DE VETERINARIOS\n\n\n\t\tUsuario: %s\n\n",user);
	printf("\t\tContraseña: %s",pass);
	printf("\n\n\t\tApellido/s y Nombre/s: %s",nombre);
	printf("\n\n\t\tNúmero de Matrícula: ");
	scanf("%d",&mat);
	if(!matricula(vet,mat)){		//se llama a la funcion que comprueba que la matricula ingresada no este registrada ya
		system("color 0e");			//si esta funcion retorna false
		Beep(700,300);
		printf("\n\n\t\tLa matrícula %d fue registrada anteriormente\n\n",mat);
		printf("\t\tDebe ingresar una matrícula distinta.\n\t\tDe lo contrario se cancelará el registro\n\n");
		printf("\t\t¿Ingresar otra matrícula? S/N: ");
		_flushall();
		gets(op);
		system("color 07");
		if(strcmp(op,"s")==0 or strcmp(op,"S")==0) //si se elige volver a ingresar la matricula se volvera al principio de
			goto regresar;						  //la sentencia goto
		else{
			printf("\n\n\t\tREGISTRO CANCELADO\n\n\t\t");
			system("pause ->NUL");
			opc=0;								//sino se cambiara la opcion y no se registrara el veterinario
		}
	}else{                 //si la funcion matricula retorna true (la matricula es valida)
	printf("\n\t\tDNI: ");
	scanf("%d",&vets.dni);
	printf("\n\t\tTelefono: ");
	scanf("%d",&vets.telefono);
	strcpy(vets.Apellido_y_nombre,nombre);
	vets.matricula=mat;
	vets.atenciones=0;
	
	vet=fopen("Veterinarios.dat","a+b");		//se abre el archivo con permisos de lectura y escritura y conservacion de datos
	fwrite(&vets,sizeof(veterinarios),1,vet);	//se guardara los datos del veterinario en el archivo de veterinarios
	fclose(vet);
	}
}
/*cuando la opcion es registrar usuarios (veterinarios o asistentes)
De esta funcion se desprenden otras tres "void usuario(), void contra()
y void registro_veterinarios()" Funciones que se encuentran mas arriba"*/
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
	strlwr(nombre);
	
	nombre[0]=toupper(nombre[0]);
	for(int i=0;i<strlen(nombre);i++){		//aqui hago que los nombres y apellidos comienzen con mayusculas
		if(nombre[i-1]==' ')				//y las demas letras minusculas
			nombre[i]=toupper(nombre[i]);
	}
	
	usuario(user,arc,nombre,opcion); //se ingresara el usuario, lo validara y lo devolvera por referencia
	 
	contra(password,user,nombre,opcion); //se ingresa la contraseña, se la valida y se la devuelve a esta funcion
	
	
	/*si la opcion ingresada en el menu fue registrar veterinarios*/
	if(op==1){								//se casigna un campo booleano del registro de usuarios, este campo
	users.veterinario=true;					//define si el usuario es veterinario (true) o asistente(false)
	/*funcion que registra y valida los datos de los veterinarios
	en caso de ingresarse una matricula repetida y querer cancelar
	el ingreso de datos de los veterinarios esta funcion cambiara
	el valor de la opcion a 0 y no se registrara el usuario ni los
	datos del veterinario*/
	registro_veterinarios(vet,nombre,user,password,op);
	}
	else{//si el usuario es asistente --> veterinario=false
		users.veterinario=false;
		system("cls");
		printf("\n\t\t%s\n\n\n\t\tUsuario: %s\n\n",opcion,user);	//se muestran los datos de usuario ordenados
		printf("\t\tContraseña: %s",password);
		printf("\n\n\t\tApellido/s y Nombre/s: %s",nombre);
	}
	
	if(op==1 or op==2){					//si la opcion 1 no cambio en la funcion registro_veterinarios o la opcion
	arc=fopen("usuarios.dat","a+b");	//es ingresar asistentes se registraran los datos de usuario ingresados
	strcpy(users.usuario,user);
	strcpy(users.contrasena,password);
	strcpy(users.Apellido_y_nombre,nombre);
	fwrite(&users,sizeof(usuarios),1,arc);
	system("color 0a");
	printf("\n\n\t\tREGISTRADO\n\n\t\t");
	system("pause ->NUL");
	system("color 07");
	fclose(arc);
	}
}
/*Funcion que muestra la lista de veterinarios con las atenciones que dieron
si no hay veterinarios registrados en el sistema da aviso de ello*/
void lista_veterinarios(FILE *vet,int numero){ //numero xd
	veterinarios vets;
	int i=3+numero; 		//se manda un numero el cual se suma a un entero que es el numero de la linea
							//desde la que se debe empezar a imprimir los datos de los veterinarios.
	vet=fopen("Veterinarios.dat","rb");
	if(vet==NULL){
		system("color 0e");
		Beep(700,300);
		printf("\n\n\n\n\t\tNo hay veterinarios registrados en el sistema\n\n\n\t\t");
		system("pause ->NUL");	system("cls");
		system("color 07");
	}
	else{
		printf("-------------------------------------------------------------------------------\n");
		printf(" Apellido/s y nombres                   Matrícula      DNI          Atenciones\n");
		printf("-------------------------------------------------------------------------------\n");
		fread(&vets,sizeof(veterinarios),1,vet);
		while(!feof(vet)){
		gotoxy(1,i);  printf("%s",vets.Apellido_y_nombre);
		gotoxy(40,i); printf("%d",vets.matricula);
		gotoxy(55,i); printf("%d",vets.dni);
		gotoxy(68,i); printf("%d",vets.atenciones);
		i++;
		fread(&vets,sizeof(veterinarios),1,vet);
		}
		fclose(vet);
		printf("\n\n");
		system("pause ->NUL");
	}
}
void ordenar_ranking(FILE *&vet){
	veterinarios reg,v[50],aux;
	int i,b,n;
	
	vet=fopen("Veterinarios.dat","rb");
	fread(&reg,sizeof(veterinarios),1,vet);
	i=0;
	if(vet!=NULL){
	while(!feof(vet)){
		v[i]=reg;
		i++;
		fread(&reg,sizeof(veterinarios),1,vet);
	}
	n=i;

    do{
        b=0;
        for (i=0;i<n-1;i++){
            if (v[i].atenciones<v[i+1].atenciones){
                aux=v[i];
                v[i]=v[i+1];
                v[i+1]=aux;
                b=1;
            }
        }
    }
    while (b==1);
    
    fclose(vet);
    vet=fopen("Veterinarios.dat","wb");
    for (i=0;i<n;i++){     
       reg=v[i];
       fwrite(&reg,sizeof(veterinarios),1,vet);           
    } 
    
	fclose(vet);
	}
}
