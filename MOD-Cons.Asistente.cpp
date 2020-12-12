#include<stdio.h>
#include<string.h>
#include<windows.h>
#include<locale.h>
#include<conio.h>
#include<time.h>

struct usuarios {	// PARA LEER LOS DATOS DE USUARIOS REGISTRADOS
	bool veterinario;	//este es falso para los asistentes
	char usuario[10];
	char contrasena[32];
	char Apellido_y_nombre[60];
};
struct fecha{
	int dia;
	int mes;
	int anio;
};
struct mascotas{
	bool borrar;			//este campo me ayudara a hacer un borrado logico de las mascotas atendidas
	char nombre[40];
	char apellido[40];
	char domicilio[60];
	int dni_de_dueno;
	char localidad[60];
	fecha Fecha_nacimiento;
	float peso;
	int telefono;
};
struct turno{	//estructura para leer y escribir los datos del archivo de turnos
	bool atendido;	//este sera true una vez atendida la mascota y asi desaparecera de la lista del consultorio
	int matricula;
	fecha atencion;
	int dni_dueno;
	char detalle_de_atencion[380];
};
struct veterinarios{	//estructura para leer los datos de veterinarios
	char Apellido_y_nombre[60];
	int matricula;
	int dni;
	int telefono;
	int atenciones;	//aqui se van acumulando las atenciones que tiene el veterinario
};

void menuasist(int &op);		//aparece el menu y registra la opcion elegida
void opcionesasist(int &op,bool &iniciada,int dd,int mm,int aaaa);	//este hace el trabajo de llamar la funcion elegida en la opcion del menu
void iniciar_sesion(bool &iniciada,int &op);	//inicia sesion 
void registrar_mascota(int dd,int mm,int aaaa);	//registra mascotas (lleva parametros de la fecha de hoy)
void registrar_turno(int dd,int mm,int aaaa);	//registra turnos
void listar(int dd,int mm,int aaaa);	//lista atenciones

main() {
	setlocale(LC_CTYPE, "spanish");		//para usar el alfabeto y simbolos del español
	
/*--estructura que capta el dia de hoy , dia, mes y año--*/
	int dia_actual,mes_actual,anio_actual;
	
	time_t t=time(NULL);
	struct tm today = *localtime(&t);
	mes_actual=today.tm_mon+1;
	dia_actual=today.tm_mday;
	anio_actual=(today.tm_year+1900);
/*---------------hasta aqui-----------------------------*/
	int op=0;					//guarda las opcion elegida esta se retorna desde una funcion por referencia
	bool sesion_iniciada=false;	// este se convierte en true una vez iniciada sesion	

	while(op!=5) {			//mientras la opcion no sea 5=salir 
		menuasist(op);	//menu funcion que devuelve el valor op por referencia
		if(op!=5)	//mientras no se haya elegido salir
			opcionesasist(op,sesion_iniciada,dia_actual,mes_actual,anio_actual);	//aqui se registran todas las operaciones
	}
}
void menuasist(int &op) {
	system("cls");
	do {
		op=0;	//se reinicia por cada opcion incorrecta
		_flushall();												//se limpia el buffer por si la opcion no haya sido numero
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
	} while(op!=1 && op!=2 && op!=3 && op!=4 && op!=5);	//mientras la opcion ingresada no haya sido una de la lista se repite
}
void mensaje() {
	Beep(700,300);
	system("color 0e");
	printf("\n\n\n\n\t\t\t   PRIMERO INICIE SESIÓN");
	system("pause ->NUL");
	system("color 07");
}
void opcionesasist(int &op,bool &iniciada,int dd,int mm,int aaaa) {
	switch(op) {
		case 1:
			if(iniciada==false)						//cuando la sesion no fue iniciada 
				iniciar_sesion(iniciada,op); //se llama a una funcion donde se hace el logueo
			else {
				system("color 0a");
				printf("\n\n\n\n\t\t\t   SESION YA INICADA");	//si la sesion ya fue iniciada y se vuelve elegir iniciar sesion
				system("pause ->NUL");							//se muestra un mensaje al respecto
				system("color 07");
			}
			break;
		case 2:
			if(iniciada==false)			//si la sesion no fue inicada y se eligio una opcion que no sea salir o iniciar
				mensaje();		// se muestra un mensaje de error
			else
				registrar_mascota(dd,mm,aaaa);	//caso contrario, en esta funcion se registran las mascotas
			break;
		case 3:
			if(iniciada==false)
				mensaje();
			else
				registrar_turno(dd,mm,aaaa);	//aqui se registran los turnos y se manda la fecha de hoy para que se ingresen numeros validos
			break;
		case 4:
			if(iniciada==false)
				mensaje();
			else
				listar(dd,mm,aaaa);			//se  muestra los detalles de atencion hechos un dia determinado por un veterinario determinado
	}
}
bool comprobar_hay_asistentes(FILE* arc) {
	usuarios users;

	fread(&users,sizeof(usuarios),1,arc);
	while(!feof(arc)) {
		if(users.veterinario==false)
			return true;
		fread(&users,sizeof(usuarios),1,arc);
	}
	return false;
}
void iniciar_sesion(bool &iniciada,int &op) {	//inicio de sesion
	usuarios users;
	char usuario[10],pass[32],opc[2],caracter;	//caracter nos ayudara para mostrar la contraseña como asteriscos
	int coinc,i;

	FILE* arch=fopen("usuarios.dat","rb");// se abre el archivo de usuarios en modo lectura
	if(arch==NULL) {	//si aun no exite algun usuario registrado se muestra un mensaje al respecto
		Beep(700,300);
		system("color 0e");
		printf("\n\n\n\t\tAún no se registraron usuarios en el sistema\n\n\t\t");
		printf("Contacte a administración para ser registrado");
		system("pause ->NUL");
		op=5;			//se retorna la opcion=5 salir y se cierra el modulo de asistentes
	} else {
		if(!comprobar_hay_asistentes(arch)) { //si hay usuarios registrados ahora se comprueba que hayn asistentes por medio de una funcion
			Beep(700,300);
			system("color 0e");
			printf("\n\n\n\t\tAún no se registraron usuarios ASISTENTES\n\n\t\t");
			printf("Contacte a administración para ser registrado");
			system("pause ->NUL");										//si no hay asistentes se muestra el mensaje
			op=5;												//y cierra el programa
		} else {
			do {			//si hay usuarios asistentes entonces se podra continuar
				i=0;
				rewind(arch);	//se reinicia el puntero del archivo

				system("cls");
				printf("\n\t\tINICIO DE SESIÓN\n\t\t---------------");
				printf("\n\n\n\t\tUsuario: ");
				_flushall();
				gets(usuario);
				printf("\n\t\tContraseña: ");
	/* la contraseña se registra por medio de este ciclo*/
				while(caracter=getch()) {				//se capta un caracter ingresado por medio ddel getch
					if(caracter==13) {	//si el caracter fue enter codigo ascii 13
						pass[i]= '\0';	//se asigna el caracter nulo y se termina el ciclo
						break;
					} else if(caracter==8) {//si el caracter fue la tecla de retroceso codigo ascii 8
						if(i>0) {	//y el contador de caracteres es mayor a cero
							i--;	//se resta el contador
							printf("\b \b");	//se borrara un digito de pantalla
						}
					} else {
						printf("*");	//cuando se ingresan otros caracteres se muestra un asterisco en pantalla
						pass[i]=caracter;	//y el caracter ingresado se asigna a un espacio de la cadena pass que es la contraseña total
						i++;
					}
				}

				fread(&users,sizeof(usuarios),1,arch);	//luego se compara la contraseña y usuario con las contraseñas y usuarios registrados
				while(!feof(arch) and iniciada==false) {
					coinc=0;
					if(strcmp(usuario,users.usuario)==0)
						coinc++;
					if(strcmp(pass,users.contrasena)==0) {
						if(coinc>0)
							iniciada=true;
					}
					if(iniciada==false)
						fread(&users,sizeof(usuarios),1,arch);
				}
				if(iniciada==false) {	//cuando la contraseña y/o el usuario son invalidos
					Beep(700,300);
					system("color 0e");
					printf("\n\n\t\tUsuario y/o contraseña incorrecta");
					printf("\n\n\t\t¿Volver a intentar? S/N: ");
					_flushall();
					gets(opc);
					strlwr(opc);
					system("color 07");
				} else {								//si se ingresaron validos
					system("color 0a");
					printf("\n\n\t\tBievenido, %s",users.Apellido_y_nombre);
					system("pause ->NUL");
				}
			} while(iniciada==false and strcmp(opc,"s")==0); //mientras no haya ingresado correctamente y haya elegido volver a intentar
		}
		fclose(arch); //se cierra el archivo
	}
	system("color 07");
}
/*En esta funcion que se desprende de la de registro de mascotas se validan las fechas ingresadas
comparandola con el dia de hoy*/
void fecha_valida(char nm[],char ape[],float peso,int &d,int &m,int &a,int dd,int mm,int aaaa){
	bool band=false;
	int cont;
	do{/*--DATOS INGRESADOS EN LA FUNCION DE LA QUE SE DERIVA--*/
		cont=0;
		system("cls");
		printf("\n\t\t\tREGISTRO DE MASCOTA                  [ %.2d / %.2d / %d ]\n\t\t\t-------------------\n\n",dd,mm,aaaa);
	printf("\t\t     .Nombre de mascota: %s\n",nm);
	printf("\t\t     .Apellido del dueño: %s\n",ape);
	printf("\t\t     .Peso de mascota: %.2f Kg.",peso);
	/*-----------------------------------------------------*/
	printf("\n\n\t\t     FECHA DE NACIMIENTO:\n\n\t\t     Día: ");
	scanf("%d",&d);
	printf("\n\t\t     Mes: ");	scanf("%d",&m);
	printf("\n\t\t     Año (aaaa): ");   scanf("%d",&a);
	
		if(d<1 or d>31 or m<1 or m>12 or a<2000 or a>aaaa){
			system("color 0e");
			Beep(700,300);
			printf("\n\tAsegurese de ingresar un número correcto para días, mes y año");
			if(a>aaaa)
			printf("\n\tEl año de nacimiento no puede ser posterior al de en curso (%d)",aaaa);
			system("pause ->NUL");
			cont++;					//cuenta los errores en el ingreso de la fecha
		}else{
			if(a==aaaa){	//si el año ingresado es el actual
				if(m==mm){		//veremos si el mes es tambien el mismo
					if(d>dd){	//si el mes es el mismo se preguntara si el dia es posterior al de hoy
					system("color 0e");
					Beep(700,300);
					printf("\n\tLa fecha de nacimiento no puede ser posterior a la fecha actual");
					system("pause ->NUL");
					cont++;			//se cuenta error
					}
				}
				if(m>mm){			//si el año es el mismo y el mes es posterior 
				system("color 0e");
				Beep(700,300);
				printf("\n\tLa fecha de nacimiento no puede ser posterior a la fecha actual");
				system("pause ->NUL");
				cont++; //se cueta otro error
				}
			}	
		}
	if(cont==0)//si la fecha fue validada
		band=true;
	system("color 07");
	}while(band==false); //hasta que la fecha no sea valida (puede haber errores como 30-2 pero este sistema fue lo maximo que pude
	system("cls");
}

/*------EN ESTA FUNCION SE REGISTRARAN LAS MASCOTAS Y SE VALIDA FECHAS Y QUE EL DOCUMENTO DEL DUEÑO NO SEA REPETIDO
ESTO ULTIMO PARA EVITAR CONFLICTOS CON LAS DEMAS FUNCIONES,,, SE PUEDE MEJORAR CON MAS TIEMPO---- */
void registrar_mascota(int dd,int mm,int aaaa){	
	mascotas pets;
	int d,m,a,dni;
	float peso;
	char nombre[40],ape[40],op;
	bool pasa;
	FILE *arch;
	
	printf("\n\t\t\tREGISTRO DE MASCOTA\n\t\t\t-------------------\n\n");
	printf("\t\t     .Nombre de mascota: ");
	_flushall();
	gets(nombre);	strupr(nombre);
	printf("\t\t     .Apellido del dueño: ");
	_flushall();
	gets(ape);		strupr(ape);
	printf("\t\t     .Peso de mascota: ");
	scanf("%f",&peso);
	fecha_valida(nombre,ape,peso,d,m,a,dd,mm,aaaa);
	
	do{	pasa=true; //por defecto pasa
		op='n';		//y la respuesta automatica es n
		system("cls");
	printf("\n\t\t\tREGISTRO DE MASCOTA\n\t\t\t-------------------\n\n");
	printf("\t\t     .Nombre de mascota: %s",nombre);
	printf("\n\t\t     .Apellido del dueño: %s",ape);
	printf("\n\t\t     .Peso de mascota: %.2f Kg.",peso);
	printf("\n\t\t     .Nacimiento: %.2d / %.2d / %.2d",d,m,a);
	printf("\n\n\t\t     .DNI del dueño: ");
	scanf("%d",&dni);
	/*----------------------------------------------------------*/
	/*---aqui se ve si el documento ingresado no se encuentra registrado ya-*/
			arch=fopen("Mascotas.dat","rb");
			if(arch==NULL)
			pasa=true;
			else{
			fread(&pets,sizeof(mascotas),1,arch);
			while(!feof(arch)){
				if(pets.dni_de_dueno==dni and pets.borrar==false){ //si el documento se registro anteriormente
					pasa=false;									//y la mascota fue atendida se permitira volver a registrar
					break;
				}else{
					pasa=true;
					fread(&pets,sizeof(mascotas),1,arch);
				}
			}
			fclose(arch);
		}
		if(pasa==false){
			printf("\n\n\t\t     DNI %d registrado anteriormente y en espera de atención",dni);
					printf("\n\t\t     ¿INGRESAR UNO DISTINTO? S/N: ");
					_flushall();		scanf("%c",&op);					//aqui se permite volver al principio para ingresar otro doc
		}
	}while(op=='s' or op=='S'); //opciones que reiniciaran el ciclo do
	
	if(pasa==true){			// si se comprobo se registran los otros datos
	printf("\t\t     .Localidad: ");
	_flushall();
	gets(pets.localidad);	strupr(pets.localidad);
	printf("\t\t     .Domicilio: ");
	_flushall();
	gets(pets.domicilio);		strupr(pets.domicilio);
	printf("\t\t     .Teléfono de contacto: ");
	scanf("%d",&pets.telefono);
	
	printf("\n\t\t¿CONFIRMAR REGISTRO DE MASCOTA? S/N: ");
	_flushall();
	scanf("%c",&op);		//una vez ingresado todos los datos se pedira confirmacion de registro
	}
	if(op=='s' or op=='S'){
		arch=fopen("Mascotas.dat","ab");
	strcpy(pets.nombre,nombre);			strcpy(pets.apellido,ape);
	pets.peso=peso;
	pets.Fecha_nacimiento.dia=d;		pets.Fecha_nacimiento.mes=m;
	pets.Fecha_nacimiento.anio=a;
	pets.dni_de_dueno=dni;
	pets.borrar=false;
	fwrite(&pets,sizeof(mascotas),1,arch);
	fclose(arch);
	system("color 0a");
	printf("\n\n\t\t     DATOS REGISTRADOS CORRECTAMENTE");
	}else{
		Beep(700,300);
			system("color 0e");
			printf("\n\n\t\t     REGISTRO CANCELADO!");
	}
	system("pause ->NUL");
	system("color 07");
}

/*REGISTRA TURNOS, VALIDA DOCUMENTOS Y MATRICULAS
LLEVA COMO REFERENCIA EL DIA DE HOY*/
void registrar_turno(int dd,int mm,int aaaa){
	int dni,mat,d,m,a,salir=0;
	char op[2];
	bool pasa=false;  //NOS AYUDARA PARA UNA PASAR CICLOS O SALTARLOS
	veterinarios vetes;
	mascotas mascota;
	turno turn;
	/*--PRIMERO ABRIMOS LOS ARCHIVOS EN MODO LECTURA PARA COMPROBAR QUE HAYAN VETERINARIOS Y MASCOTAS REGISTRADAS--*/
	FILE *vets=fopen("Veterinarios.dat","rb"); 
	FILE *pets=fopen("Mascotas.dat","rb");
	
	if(pets==NULL){
		Beep(700,300);
			system("color 0e");
			printf("\n\n\n\t\tAún no hay mascotas registradas en el sistema");
	}
	else if(vets==NULL){
		Beep(700,300);
			system("color 0e");
			printf("\n\n\n\t\tAún no hay veterinarios registrados en el sistema\n\t\t");
			printf("         no podrá registrar el turno");
	}
	if(pets!=NULL and vets!=NULL){ //SI HAY TANTO MASCOTAS COMO VETERINARIOS NOS DEJARA INGRESAR LOS DATOS
		
			FILE *turnos=fopen("Turnos.dat","a+b");	//SE ABRE O SE CREA EL ARCHIVO
		
		while(pasa==false && salir==0){
			system("cls");
			printf("\n\t\t\t  TURNOS\n\t\t\t  ------\n\n");
			printf("\t.DNI Del dueño de la mascota: ");
			scanf("%d",&dni);
			rewind(pets);
			fread(&mascota,sizeof(mascotas),1,pets);
			while(!feof(pets) and pasa==false){
				if(dni==mascota.dni_de_dueno){
					pasa= true;			//si el dni del dueño se encuentra en el archivos de mascotas
				}
				else{
					fread(&mascota,sizeof(mascotas),1,pets);
				}
			}
			if(pasa==false){		//si no se encontro se preguntara si se quiere volver a ingresar
				Beep(700,300);
				system("color 0e");
				printf("\n\tEl DNI %d no está registrado en el sistema de datos de mascotas",dni);
				printf("\n\n\t¿Desea ingresar nuevamente? S/N: ");
				_flushall();	gets(op);	strlwr(op);
				system("color 07");
				if(strcmp(op,"s")!=0)
				salir++;  //si no se elige volver a ingresar se saldra del ciclo
			}
		}

		pasa=false;
		
		while(pasa==false && salir==0){		//ahora mientraras salir aun sea 0
			system("cls");
			printf("\n\t\t\t  TURNOS\n\t\t\t  ------\n\n");
			printf("\tDATOS DE MASCOTA:\n\n");
			printf("\t.Nombre y Apellido: %s, %s -- Peso: %.2f Kg.\n",mascota.nombre,mascota.apellido,mascota.peso);
			printf("\n\tDATOS DE VETERINARIO:\n");
			printf("\n\t.Nro. de matrícula: ");		scanf("%d",&mat);
			rewind(vets);
			
			fread(&vetes,sizeof(veterinarios),1,vets);	//se ingresara la matricula y se la compara con la de veterinarios
			while(!feof(vets) and pasa==false){
				if(mat==vetes.matricula){
					pasa= true;
				}
				else{
					fread(&vetes,sizeof(veterinarios),1,vets);
				}
			}
			if(pasa==false){	//si la matricula no fue encontrada se dara la oportunidad de volver a ingresar
				Beep(700,300);
				system("color 0e");
				printf("\n\tLa mátricula %d no está registrada en el sistema de veterinarios",mat);
				printf("\n\n\t¿Desea ingresar nuevamente? S/N: ");
				_flushall();	gets(op);	strlwr(op);
				system("color 07");
				if(strcmp(op,"s")!=0)	
				salir++;
			}
		}
		
		pasa=false;
		
		while(pasa==false && salir==0){ //si no se ligio salir se ingresara la fecha del turno programado
			system("cls");
			printf("\n\t\t\t  TURNOS\n\t\t\t  ------\n\n");
			printf("\tDATOS DE MASCOTA:\n\n");
			printf("\t.Nombre y Apellido: %s, %s -- Peso: %.2f Kg.\n",mascota.nombre,mascota.apellido,mascota.peso);
			printf("\n\tDATOS DE VETERINARIO:\n");
			printf("\n\t.Nro. de matrícula: %d\n",mat);
			printf("\t.Apellido y Nombre: %s\n\t.DNI: %d\n\n",vetes.Apellido_y_nombre,vetes.dni);
			printf("\tFECHA DE TURNO:\n\n");
			printf("\t.Día: ");		scanf("%d",&d);
			printf("\t.Mes: ");		scanf("%d",&m);
			printf("\t.Año: ");		scanf("%d",&a);
			/*--SE VALIDARA UNA FECHA CORRECTA EN EL FUTURO--*/
			if(d<1 or d>31 or m>12 or m<1 or a<aaaa or a>9999){
			system("color 0e");
			Beep(700,300);
			printf("\n\tAsegurese de ingresar un número correcto para días, mes y año");
			system("pause ->NUL");
			}else if(a==aaaa){
				if(m==mm){
					if(d<dd){
					system("color 0e");
					Beep(700,300);
					printf("\n\tEl turno no puede ser anterior al día de hoy (%.2d/%.2d/%d)",dd,mm,aaaa);
					system("pause ->NUL");
					}
				}
				if(m<mm){
				system("color 0e");
				Beep(700,300);
				printf("\n\tEl turno no puede ser anterior al día de hoy (%.2d/%.2d/%d)",dd,mm,aaaa);
				system("pause ->NUL");
				}
			}else{
				pasa=true;
			}	
			system("color 07");
		}
		
		if(pasa==true){
			system("cls");
			printf("\n\t\t\t  TURNOS\n\t\t\t  ------\n\n");
			printf("\tDATOS DE MASCOTA:\n\n");
			printf("\t.Nombre y Apellido: %s, %s -- Peso: %.2f Kg.\n",mascota.nombre,mascota.apellido,mascota.peso);
			printf("\n\tDATOS DE VETERINARIO:\n");
			printf("\n\t.Nro. de matrícula: %d\n",mat);
			printf("\t.Apellido y Nombre: %s\n\t.DNI: %d\n\n",vetes.Apellido_y_nombre,vetes.dni);
			printf("\t.Fecha de turno: %.2d/%.2d%d",d,m,a);
			printf("\n\n\t\t¿CONFIRMAR TURNO? S/N: ");   //una vez ingresada una fecha correcta se pedira confirmar el turno
			_flushall();	gets(op);	strlwr(op);
			if(strcmp(op,"s")!=0)
			salir++;
		}
		
		if(salir>0){		//en cualquier instancia si se sale se muestra un mensaje de turno cancelado
			Beep(700,300);
				system("color 0e");
				printf("\n\t¡TURNO CANCELADO!");
		}
		else{	//si nunca se eligio salir y se validaron todos los datos se mostrara un mensaje y se procede a guardar los datos en un archivo 
			system("color 0a");
			printf("\n\tTURNO REGISTRADO");
			turn.dni_dueno=dni;
			turn.matricula=mat;
			turn.atencion.dia=d;	turn.atencion.mes=m;	turn.atencion.anio=a;
			turn.atendido=false;
			turn.detalle_de_atencion[0]='\0';
			fwrite(&turn,sizeof(turno),1,turnos);
		}
		fclose(vets);
		fclose(pets); //CERRAMOS LOS ARCHIVOS EN USO
		fclose(turnos);
	}
	system("pause ->NUL");
	system("color 07"); 
}

/*------------FUNCION PARA LISTAR LOS DIAGNOSTICOS DE LAS MASCOTAS INGRESADAS
TENIENDO COMO REFERENCIA LA MATRICULA DE UN VETERINARIO Y UNA FECHA DETERMINADA ----*/
void listar(int dd,int mm,int aaaa){
	typedef char diagnostico[380];
	typedef char nombre[60];
	nombre mascota_nombres[20];		//ARRAY DE CADENAS DE CARACTERES QUE GUARDARA NOMBRES DE LAS MASCOTAS ATENDIDAS
	char veterinario_nombre[60];	//CADENA QUE GUARDARA EL NOMBRE DEL VETERINARIO INGRESADO
	char op;
	bool pasa;
	diagnostico diagnosticos[20]; //ARRAY DE CADENAS DE CARACTERES QUE GUARDARA LOS DISTINTOS DIAGNOSTICOS
	FILE *turnos=fopen("Turnos.dat","rb");	//abrimos el archivo turno para comprobar 
	
	int d,m,a,i,j,n,matricula;
	int dni_de_dueno[20]; //guardara el dni del dueño de la mascota con fin de luego conseguir el nombre de la mascota
	
	if(turnos==NULL){
		Beep(700,300);
		system("color 0e");
		printf("\n\n\n\t\t\tAún no se registraron turnos");
		system("pause ->NUL");
	}else{
	FILE *pets=fopen("Mascotas.dat","rb");
	FILE *vets=fopen("Veterinarios.dat","rb");
		turno turn;
		veterinarios veterinario;
		mascotas mascota;
		
		do{	i=0;
		system("color 07");
		pasa=false;
		printf("\n\t\tFECHA A BUSCAR ATENCIONES\n\n");
		printf("\t\tDía: ");	scanf("%d",&d);
		printf("\t\tMes: ");	scanf("%d",&m);
		printf("\t\tAño: ");	scanf("%d",&a);
		
		/*---SE VALIDARÁ LA FECHA DE ATENCION (que sea una fecha valida dentro de lo posible)---*/
		if(d<1 or d>31 or m>12 or m<1 or a<aaaa-5 or a>aaaa){
			i++;
			system("color 0e");
			Beep(700,300);
			printf("\n\tAsegurese de ingresar un número correcto para días, mes y año");
				if(a<aaaa-5)
				printf("\n\tSe guardan solo las atenciones de los ultimos 5 años");
				if(a>aaaa)
				printf("\n\tEl año no puede ser posterior al año en curso (%d)",aaaa);
			system("pause ->NUL");
			}else if(a==aaaa){
				if(m==mm){
					i++;
					if(d>dd){
					system("color 0e");
					Beep(700,300);
					printf("\n\tLas atenciones no pueden ser posteriores al día de hoy (%.2d/%.2d/%d)",dd,mm,aaaa);
					system("pause ->NUL");
					}
				}
				if(m>mm){
					i++;
				system("color 0e");
				Beep(700,300);
				printf("\n\tLas atenciones no pueden ser posteriores al día de hoy (%.2d/%.2d/%d)",dd,mm,aaaa);
				system("pause ->NUL");
				}
			}
				
			if(i==0)
				pasa=true;
			system("cls");
		}while(pasa==false);//mientras la fecha no sea valida (el valor de "pasa" se conseva en true) y se repite el ciclo de validacion

/*---VALIDADA LA FECHA SE INGRESARA LA MATRICULA SE VALIDARA QUE EXISTA, DE EXISTIR
SE COPIARAN LOS DATOS UTILES EN DISTINTOS ARREGLOS Y LUEGO SE PROCEDERA A MOSTRARLOS---*/
			do{	system("cls");			system("color 07");
				i=0,j=0;
				pasa=false;
				rewind(turnos);	rewind(vets); rewind(pets);
			printf("\n\n\t.Fecha de atención %.2d/%.2d/%d",d,m,a);
			printf("\n\t.Matrícula de Veterinario: ");
			scanf("%d",&matricula);
			
			fread(&turn,sizeof(turno),1,turnos);
			while(!feof(turnos)){								//MIENTRAS NO SEA EL FIN DE TURNOS
				if(matricula==turn.matricula && pasa==false){			//se verifica de una vez si la matricula ingresada esta registrada
					fread(&veterinario,sizeof(veterinarios),1,vets);
					while(!feof(vets)){
					if(matricula==veterinario.matricula){
					pasa=true;
					strcpy(veterinario_nombre,veterinario.Apellido_y_nombre); //si la matricula se encontro se copia el nombre asociado a la cadena nombre de veterina
					}
					fread(&veterinario,sizeof(veterinarios),1,vets);
					}
				}
				if(pasa==false){			//si no se encontro la matricula se sale del ciclo de comprobacion
				break;
				}
/*SE COMPRUEBA SI LA MATRICULA SE ENCUENTRA EN EL ARCHIVO TURNO Y A LA VEZ SI EL TURNO FUE ATENDIDO Y SI FUE EN EL DIA
INGRESADO*/		
			if(turn.matricula==matricula && turn.atendido==true && turn.atencion.dia==d && turn.atencion.dia==m && turn.atencion.dia==a)
			{	
				dni_de_dueno[i]=turn.dni_dueno;		//esto lo usaremos en otro ciclo para obtener los nombres de mascotas
				strcpy(diagnosticos[i],turn.detalle_de_atencion); 
				i++;   //CUANDO LOS DATOS CUMPEN LA CONDICION SE COPIA ELDIAGNOSTICO A UN ARREGLO DE CADENAS, TAMBIEN EL DNI DEL DUEÑO DE LA MASCOTA
			}			//se comparara estos datos luego con el archivo de mascotas para obtenet sus nombres
			fread(&turn,sizeof(turno),1,turnos);
			}
			n=i-1,j=0;
			fread(&mascota,sizeof(mascotas),1,pets);	//se obtine el nombre de las mascotas
			while(!feof(pets) && j<n){
				if(dni_de_dueno[j]==mascota.dni_de_dueno){
					strcpy(mascota_nombres[j],mascota.nombre);
					j++;
				}
				fread(&mascota,sizeof(mascotas),1,pets);
			}
			
		if(pasa==false){	//si no se encontro la matricula del veterinario
				Beep(700,300);
				system("color 0e");
			printf("\n\t.La matrícula %d no está registrada en el sistema",matricula);
			printf("\n\t ¿DESEA INGRESAR UNA DISTINTA? S/N: ");
			_flushall(); scanf("%c",&op);			//se da la oportunidad de volver a ingresar
		}else{
			if(i==0){	//si no hay detalles de atencion copiados entonces el veterinario no registro atenciones
				Beep(700,300);
				system("color 0e");
				printf("\n\t.Veterinario: %s",veterinario_nombre);
				printf("\n\n\tNO REGISTRA ATENCIONES EL DÍA INDICADO");
				system("pause ->NUL");
			}
			else{													// de lo contrario se procede a mostrar los datos obtenidos
				printf("\n\t.Veterinario: %s",veterinario_nombre);
				printf("\n\n\tREGISTRA %d ATENCIONES EL DÍA INDICADO",n);
				printf("\n\n\tPRESIONA UNA TECLA PARA VER LOS DETALLES DE ATENCION REGISTRADOS...");
				system("pause ->NUL");
				for(i=0;i<n;i++){
					system("cls");
					printf("(%d)FECHA %.2d/%.2d/%d\n\nVETERINARIO: %s  ºº  MATRÍCULA %d\n",i+1,d,m,a,veterinario_nombre,matricula);
					printf("NOMBRE DE MASCOTA: %s\n\n",mascota_nombres[i]);
					printf("DETALLE DE ATENCION: %s",diagnosticos[i]);
					system("pause -> NUL");
				}
			}
		}
		}while(op=='s' or op=='S');
		fclose(pets);	fclose(turnos);		fclose(vets);	//cerramos archivos
	}
	system("color 07");
}
