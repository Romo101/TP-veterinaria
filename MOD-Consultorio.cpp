/*
	TRABAJO INTEGRADOR 2DO CUATRIMESTRE
	
	MODULO CONSULTORIO
	
	ALUMNO: ROMO J. JONATHAN EMANUEL
	COMISION: 1K05
	DNI: 41374521
*/
#include<locale.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<windows.h>
#include<conio.h>

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
struct fecha{
	int dia;
	int mes;
	int anio;
};
struct mascotas{
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

void menuvet(int &op);
void opcionesvet(int &op, bool &iniciada,int d,int m,int a,int &matricula,int &dni_ultimo);
void iniciar_sesion(bool &iniciada,int &op,int &matricula);
void lista_del_dia(int d,int m,int a,int matricula,int &dni_ultimo);

main(){
	setlocale(LC_CTYPE, "spanish");
	int op=0,dni_ultimo=0,matricula;
	bool sesion_iniciada;
	
/*--estructura que capta el dia de hoy , dia, mes y año--*/
	int dia_actual,mes_actual,anio_actual;
	
	time_t t=time(NULL);
	struct tm today = *localtime(&t);
	mes_actual=today.tm_mon+1;
	dia_actual=today.tm_mday;
	anio_actual=(today.tm_year+1900);
/*---------------hasta aqui-----------------------------*/
	
	while(op!=4){
		system("cls");
		menuvet(op);
		if(op==4 && dni_ultimo!=0){
			Beep(700,300);
			system("color 0e");
			printf("\n\n\n\n\n\t\t   POR FAVOR, PRIMERO REGISTRA EL DETALLE DE LA\n\t\t\t        ULTIMA ATENCIÓN");
			system("pause ->NUL");	system("color 07");
			op=0;
		}
		if(op!=4)
		opcionesvet(op,sesion_iniciada,dia_actual,mes_actual,anio_actual,matricula,dni_ultimo);
	}
}
void menuvet(int &op){
	do{
		op=0;
		_flushall();
		printf("\n\n\t\t\tMÓDULO CONSULTORIO VETERINARIO\n");
		printf("\t\t\t==============================\n\n");
		printf("\t\t\t1.- Iniciar sesión\n\n");
		printf("\t\t\t2.- Visualizar lista de espera\n\n");
		printf("\t\t\t3.- Registrar evolución de la mascota\n\n");
		printf("\t\t\t4.- Cerrar módulo\n\n");
		printf("\t\t\tIngrese una opción: ");
		scanf("%d",&op);
		system("cls");
	}while(op!=1 && op!=2 && op!=3 && op!=4);
}
void mensaje(){
	Beep(700,300);
	system("color 0e");
	printf("\n\n\n\n\n\t\t\t     PRIMERO INICIE SESION");
	system("pause ->NUL");
	system("color 07");
}
void opcionesvet(int &op, bool &iniciada,int d,int m,int a,int &matricula,int &dni_ultimo){
	switch(op){
		case 1:	if(iniciada==true){
				Beep(700,300);
				system("color 0a");
				printf("\n\n\n\n\n\t\t\t       SESION YA INICIADA");
				system("pause ->NUL");
				system("color 07");
				}else{
					iniciar_sesion(iniciada,op,matricula);
				}
			break;
		case 2:	if(iniciada==false)
				mensaje();
				else{
					lista_del_dia(d,m,a,matricula,dni_ultimo);
				}
			break;
		case 3: if(iniciada==false)
				mensaje();
				else{
					printf("registrar evolucion\n\n");
					system("pause ->NUL");
				}
			break;
	}
}
bool comprobar_hay_veterinarios(FILE* arc) {
	usuarios users;

	fread(&users,sizeof(usuarios),1,arc);
	while(!feof(arc)) {
		if(users.veterinario==true)
			return true;
		fread(&users,sizeof(usuarios),1,arc);
	}
	return false;
}
void iniciar_sesion(bool &iniciada,int &op,int &matricula) {	//inicio de sesion
	usuarios users;
	veterinarios veterinario;
	char usuario[10],pass[32],opc[2],caracter;	//caracter nos ayudara para mostrar la contraseña como asteriscos
	int coinc,i;

	FILE* arch=fopen("usuarios.dat","rb");// se abre el archivo de usuarios en modo lectura
	if(arch==NULL) {	//si aun no exite algun usuario registrado se muestra un mensaje al respecto
		Beep(700,300);
		system("color 0e");
		printf("\n\n\n\t\tAún no se registraron usuarios en el sistema\n\n\t\t");
		printf("Contacte a administración para ser registrado");
		system("pause ->NUL");
		op=4;			//se retorna la opcion=5 salir y se cierra el modulo de asistentes
	} else {
		if(!comprobar_hay_veterinarios(arch)) { //si hay usuarios registrados ahora se comprueba que hayn asistentes por medio de una funcion
			Beep(700,300);
			system("color 0e");
			printf("\n\n\n\t\tAún no se registraron usuarios VETERINARIOS\n\n\t\t");
			printf("Contacte a administración para ser registrado");
			system("pause ->NUL");										//si no hay asistentes se muestra el mensaje
			op=4;												//y cierra el programa
		} else {
			FILE* vets=fopen("Veterinarios.dat","rb");
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
					_flushall();	gets(opc);	strlwr(opc);
					system("color 07");
				} else {								//si se ingresaron validos
				fread(&veterinario,sizeof(veterinarios),1,vets);
				while(!feof(vets)){
					if(strcmp(users.Apellido_y_nombre,veterinario.Apellido_y_nombre)==0){
						matricula=veterinario.matricula;
						break;
					}
					else{
					fread(&veterinario,sizeof(veterinarios),1,vets);	
					}
				}
					fclose(vets);
					system("color 0a");
					printf("\n\n\t\tBievenido veterinario, %s\n\n\t\tNro. Matrícula: %d",users.Apellido_y_nombre,matricula);
					system("pause ->NUL");
				}
			} while(iniciada==false and strcmp(opc,"s")==0); //mientras no haya ingresado correctamente y haya elegido volver a intentar
		}
		fclose(arch);//se cierran los archivos
	}
	system("color 07");
}
void edad(int d_nac,int m_nac,int a_nac,int d,int m,int a){
		int meses,anios,dias;
		anios=a-a_nac;
		meses=0;
		dias=0;
		
		printf("º .EDAD: ");
			if(m_nac!=m){
			if(m<m_nac){
				anios-=1;
				meses=12-(m_nac-m);
			}
			if(m>m_nac){
				meses=m-m_nac;
			}
			}
			else if(m_nac==m){
				dias=d-d_nac;
			}
		if(anios>0){
			printf("%d años.",anios);
		}
		if(meses==0)
			printf(" %d días.",dias);
		else
			printf(" %d meses",meses);
		
}
void lista_del_dia(int d,int m,int a,int matricula,int &dni_ultimo){
	FILE* turn=fopen("Turnos.dat","rb");		//esta funcion me devolvera el dni del dueño de la mascota atendida
	
	if(turn==NULL){
		Beep(700,300);
		system("color 0e");
		printf("\n\n\n\t\t\tNO HAY TURNOS REGISTRADOS");
		system("pause ->NUL");
	}else if(dni_ultimo!=0){
		Beep(700,300);
		system("color 0e");
		printf("\n\n\n\t\t\tPRIMERO REGISTRE EL DETALLE DE ATENCION\n\t\t\tDE LA ÚLTIMA MASCOTA ATENDIDA");
		system("pause ->NUL");
	}else{ 
		int i=0,n,dni[20];
		bool pasa;
		printf("%.2d/%.2d/%d",d,m,a);
		FILE* pets=fopen("Mascotas.dat","rb");
		turno turnos;
		mascotas mascota;
		
		fread(&turnos,sizeof(turno),1,turn);
		while(!feof(turn)){
			if(turnos.matricula==matricula && turnos.atendido==false && turnos.atencion.dia==d && turnos.atencion.mes==m && turnos.atencion.anio==a){
				dni[i]=turnos.dni_dueno;
				i++;
			}
			fread(&turnos,sizeof(turno),1,turn);
		}
		if(i==0){
			Beep(700,300);
		system("color 0e");
		printf("\n\n\n\t\t  AÚN NO HAY TURNOS REGISTRADOS PARA EL DÍA DE HOY");
		system("pause ->NUL");
		}else{
			n=i,i=0;
			char nombre[40],apellido[40],op;
			bool pasa;
			int di,me,an,dni_atendido;
			do{	pasa=false, op='n', i=0;
				system("cls");
				while(i<n){
				rewind(pets);
				fread(&mascota,sizeof(mascotas),1,pets);
				while(!feof(pets)){
					if(dni[i]==mascota.dni_de_dueno){
						printf("(%d)\nº .Nombre de mascota: %s\n",i+1,mascota.nombre);
						printf("º .Apellido de dueño: %s\nº .Peso de mascota: %.2f Kg.\n",mascota.apellido,mascota.peso);
						di=mascota.Fecha_nacimiento.dia;	me=mascota.Fecha_nacimiento.mes;
						an=mascota.Fecha_nacimiento.anio;
						edad(di,me,an,d,m,a);
						printf("\nº .DNI de dueño: %d",mascota.dni_de_dueno);
						printf("\nº .Localidad: %s\n\n",mascota.localidad);
						break;
					}
					fread(&mascota,sizeof(mascotas),1,pets);
				}
				i++;
				}
				printf("LLAMAR MASCOTA: \n\n");
				printf("Nombre de la mascota: ");
				_flushall();	gets(nombre); 	strupr(nombre);
				printf("Apellido: ");
				_flushall();	gets(apellido);	strupr(apellido);
				printf("DNI de dueño: ");
					scanf("%d",&dni_atendido);
					
				rewind(turn);
				fread(&turnos,sizeof(turno),1,turn);
				while(!feof(turn)){
					if(dni_atendido==turnos.dni_dueno){
						rewind(pets);
						fread(&mascota,sizeof(mascotas),1,pets);
						while(!feof(pets)){
							if(dni_atendido==mascota.dni_de_dueno && strcmp(nombre,mascota.nombre)==0 && strcmp(apellido,mascota.apellido)==0){
								pasa=true;
								break;
							}
							fread(&mascota,sizeof(mascotas),1,pets);
						}
					}
					if(pasa==true){
							fseek(turn,-sizeof(turno),SEEK_CUR);
							break;
					}
					fread(&turnos,sizeof(turno),1,turn);
				}
				if(pasa==true){
					system("color 0a");
					printf("\nMASCOTA LLAMADA");
					turnos.atendido=true;
					fwrite(&turnos,sizeof(turno),1,turn);
					dni_ultimo=dni_atendido;
					system("pause ->NUL");
				}else{
					Beep(700,300);
					system("color 0e");
					printf("\nLA MASCOTA NO ESTÁ EN LA LISTA");
					printf("\n¿Volver a ingresar? S/N: ");
					_flushall();	scanf("%c",&op);
					system("color 07");
				}
			}while(op=='s' or op=='S');
		}
		fclose(pets);	fclose(turn);
	}
	system("color 07");
}
