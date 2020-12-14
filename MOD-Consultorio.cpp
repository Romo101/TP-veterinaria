/*
	TRABAJO INTEGRADOR 2DO CUATRIMESTRE
	
	MODULO CONSULTORIO
	
	ALUMNO: ROMO J. JONATHAN EMANUEL
	COMISION: 1K05
	DNI: 41374521
*/
#include<locale.h>	//libreria para cambiar el idioma
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
	int atenciones;	//registrara las atenciones del veterinario cada vez que complete el diagnostico de una mascota
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

void menuvet(int &op);			//menu del veterinario
void opcionesvet(int &op, bool &iniciada,int d,int m,int a,int &matricula,int &dni_ultimo); 		//capta las opciones
void iniciar_sesion(bool &iniciada,int &op,int &matricula);									//opcion 1
void lista_del_dia(int d,int m,int a,int matricula,int &dni_ultimo);						//opcion 2
void detalle_atencion(int &dni_ultimo,int matricula,int d,int m,int a);						//opcion 3       

main(){
	setlocale(LC_CTYPE, "spanish");		//para usar el idioma español
	/* dni_ultimo capta el dni del dueño de la ultima mascota atendida
	y matricula guardara la matricula de el veterinario que inicio sesion*/
	int op=0,dni_ultimo=0,matricula;
	bool sesion_iniciada;	//sera true cuando la sesion se haya inicada y nos dara permiso a las opciones 2 y 3
						//de lo contrario solo se podra acceder a la opcion 1 y 4 (salir)
	
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
		if(op==4 && dni_ultimo!=0){	/*si se atendio a una mascota y no se registro aun el detalle de atencion no nos
		permitira salir*/
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
/*muestra el menu del modulo y devuelve la opcion elegida por referencia*/
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
	}while(op!=1 && op!=2 && op!=3 && op!=4);	//mientras la opcion no sea una de las opciones de la lista se repite
}
/*mensaje que se mostrara cuando no hayamos inicado sesion y se quiera acceder a la opcion 2 o 3*/
void mensaje(){
	Beep(700,300);
	system("color 0e");
	printf("\n\n\n\n\n\t\t\t     PRIMERO INICIE SESION");
	system("pause ->NUL");
	system("color 07");
}
/*fimcopm que registra el trabajo hecho en cada opcion por medio de funciones devuelve parametro por referencia
la opcion elegida, la variable booleana que indica cuando la sesion esta iniciada, la matricula del veterinario
que inicio sesion y el dni del dueño de la ultima mascota atendida*/
void opcionesvet(int &op, bool &iniciada,int d,int m,int a,int &matricula,int &dni_ultimo){
	switch(op){
		case 1:	if(iniciada==true){//cuando ya hayamos iniciado sesion y se elija iniciar sesion nuevamente
				Beep(700,300);
				system("color 0a");
				printf("\n\n\n\n\n\t\t\t       SESION YA INICIADA");
				system("pause ->NUL");
				system("color 07");
				}else{
					iniciar_sesion(iniciada,op,matricula); //funcion donde se inicia sesion y nos devuelve la matricula
				}
			break;
		case 2:	if(iniciada==false)
				mensaje();
				else{
					lista_del_dia(d,m,a,matricula,dni_ultimo);//nos mostrara si hay mascotas para atender ese dia
				}
			break;
		case 3: if(iniciada==false)
				mensaje();
				else{
					detalle_atencion(dni_ultimo,matricula,d,m,a);//nos permitira guardar el detalle de atencion de la
				}				                      //ultima mascota atendida
			break;
	}
}
/*funcion que verifica si hay veterinarios registrados cuando elegimos iniciar sesion*/
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
/*para iniciar sesion ademas se desprende una funion que verifica la existencia de veterinarios*/
 void iniciar_sesion(bool &iniciada,int &op,int &matricula) {
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
		if(!comprobar_hay_veterinarios(arch)) { //si hay usuarios registrados ahora se comprueba que haya veterinarios
			Beep(700,300);
			system("color 0e");
			printf("\n\n\n\t\tAún no se registraron usuarios VETERINARIOS\n\n\t\t");//si no hay veterinarios
			printf("Contacte a administración para ser registrado");
			system("pause ->NUL");										
			op=4;												//devuelve la opcion 4 para cerrar el modulo
		} else {
			FILE* vets=fopen("Veterinarios.dat","rb");/*archivo veterinarios en modo lectura*/
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
				while(caracter=getch()) {				//se capta un caracter ingresado por medio del getch
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
					if(strcmp(usuario,users.usuario)==0 && users.veterinario==true)
						coinc++;
					if(strcmp(pass,users.contrasena)==0 && users.veterinario==true) {
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
/*funcion donde se calcula la edad de una mascota*/
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
/*muestra la lista de mascotas en la sala de espera una vez llamada a una mascota se devuelve por referencia
el dni del dueño y se cambia el valor de una variable para indicar que la mascota ue atendida, de esa manera
desaparecera de la lista, una vez se sale de esta funcion no se podra salir del modulo ni volver a llamar a
otra mascota hasta que registremos el detalle de atencion de la ultima mascota llamada*/
void lista_del_dia(int d,int m,int a,int matricula,int &dni_ultimo){
	FILE* turn=fopen("Turnos.dat","r+b");
	
	if(turn==NULL){
		Beep(700,300);
		system("color 0e");
		printf("\n\n\n\t\t\tNO HAY TURNOS REGISTRADOS");
		system("pause ->NUL");
	}else if(dni_ultimo!=0){ //si una mascota fue atendida anteriormente y no se registro la atencion el dni sera dif a 0
		Beep(700,300);
		system("color 0e");
		printf("\n\n\n\t\t\tPRIMERO REGISTRE EL DETALLE DE ATENCION\n\t\t\tDE LA ÚLTIMA MASCOTA ATENDIDA");
		system("pause ->NUL");
	}else{ 
		int i=0,n,dni[20];
		bool pasa;
		printf("%.2d/%.2d/%d",d,m,a);//fecha de hoy
		FILE* pets=fopen("Mascotas.dat","rb");
		turno turnos;
		mascotas mascota;
		
		fread(&turnos,sizeof(turno),1,turn);
		while(!feof(turn)){
			if(turnos.matricula==matricula && turnos.atendido==false && turnos.atencion.dia==d && turnos.atencion.mes==m && turnos.atencion.anio==a){
/*si la matricula del doctor del turno es el mismo que el del doctor que inicio sesion y el turno aun no fue atendido
y la fecha del turno sea la fecha de hoy*/	
					dni[i]=turnos.dni_dueno;//se guarda el dni del turno en un vector que nos ayudara a obtener los datos de las mascotas
					i++;
			}
			fread(&turnos,sizeof(turno),1,turn);
		}
		if(i==0){	//si el contador de dnis guardados sigue siendo cero entonces no hay turnos para hoy
			Beep(700,300);
		system("color 0e");
		printf("\n\n\n\t\t  AÚN NO HAY TURNOS REGISTRADOS PARA EL DÍA DE HOY");
		system("pause ->NUL");
		}else{
			n=i,i=0;						//sino se procede a obtener los datos de las mascotas
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
				while(!feof(turn) && pasa==false){
					if(dni_atendido==turnos.dni_dueno){
						rewind(pets);
						fread(&mascota,sizeof(mascotas),1,pets);
						while(!feof(pets) && pasa==false){
							if(dni_atendido==mascota.dni_de_dueno && strcmp(nombre,mascota.nombre)==0 && strcmp(apellido,mascota.apellido)==0){
								pasa=true;
							}
							fread(&mascota,sizeof(mascotas),1,pets);
						}
					}
					if(pasa==true){
							fseek(turn,-sizeof(turno),SEEK_CUR);
					}else{
					fread(&turnos,sizeof(turno),1,turn);
					}
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
/*funcion donde se registrara el detalle de atencion de la anterior mascota atendida
una vez registrado el detalle de atencion el valor de dni ultimo cambiara a 0 permitiendo volver a llamar otra mascota*/
void detalle_atencion(int &dni_ultimo,int matricula,int d,int m,int a){
	
	if(dni_ultimo==0){
		Beep(700,300);
		system("color 0e");
		printf("\n\n\n\n\t\tPRIMERO DEBERÁ ATENDER A UNA MASCOTA");
		system("pause ->NUL");
		system("color 07");
	}else{
	char diagnostico[380];	//se guarda el diaqnostico  mientras ingresamos caracteres
	char caracter; //para ir ingresando
	int maximo=380;	//maximo de caracteres
	char nombre_de_mascota[40];
	int i=0;	//iterador para ir asignando caracteres a la cadena
	FILE *turn=fopen("Turnos.dat","r+b");
	FILE *pets=fopen("Mascotas.dat","rb");
	FILE *vets=fopen("Veterinarios.dat","r+b");
	
	turno turnos;
	mascotas mascota;
	veterinarios veterinario;
	
	/*obtenemos el nombre de la mascota*/
	fread(&turnos,sizeof(turno),1,turn);
	while(!feof(turn)){
		if(turnos.dni_dueno==dni_ultimo && turnos.atendido==true && turnos.atencion.dia==d && turnos.atencion.mes==m && turnos.atencion.anio==a){
			fread(&mascota,sizeof(mascotas),1,pets);
			while(!feof(pets)){
				if(dni_ultimo==mascota.dni_de_dueno){
					strcpy(nombre_de_mascota,mascota.nombre);
					break;
				}
				fread(&mascota,sizeof(mascotas),1,pets);
			}
			fseek(turn,-sizeof(turno),SEEK_CUR);
			break;
		}
		fread(&turnos,sizeof(turno),1,turn);
	}
	fclose(pets);
	
	Beep(700,300);
	system("color 0a");
	printf("\n\n\n\n\t    TENDRÁ UN MÁXIMO DE 380 CARÁCTERES PARA REDACTAR EL DIAGNOSTICO");
	system("pause ->NUL");
	system("color 07");	system("cls");
	
	printf("\n.Nombre de mascota: %s\n.DNI de dueño: %d\n\n",nombre_de_mascota,dni_ultimo);
	printf("									(%d)\n",maximo);	 
	printf("Diagnóstico: "); 
	/*luego en este ciclo se hace un trabajo similar al de la contraseña, se hizo asi a fin de mostrar un contador
	de las letras restantes en la redaccion*/
while(caracter=getch()) {
	if(maximo>0){
		if(caracter==13) {
			diagnostico[i]= '\0';	
			break;	
		} else if(caracter==8) {	
		if(i>0) {
		i--; 
		maximo++;
		system("cls");
	printf("\n.Nombre de mascota: %s\n.DNI de dueño: %d\n\n",nombre_de_mascota,dni_ultimo);
	printf("									(%d)\n",maximo);	 
	printf("Diagnóstico: "); 
		printf("%s",diagnostico);
		diagnostico[i]='\0';
		printf("\b \b");
		}
		} else {
		maximo--;
		system("cls");
	printf("\n.Nombre de mascota: %s\n.DNI de dueño: %d\n\n",nombre_de_mascota,dni_ultimo);
	printf("									(%d)\n",maximo);	 
	printf("Diagnóstico: "); 
		diagnostico[i]=caracter;
		diagnostico[i+1]='\0';
		printf("%s",diagnostico);
		i++;
		}
	}
	else{
		diagnostico[i]='\0';
		break;
	}
		}
		strcpy(turnos.detalle_de_atencion,diagnostico);	//luego se guarda el diagnostico en el archivo de turnos
		fwrite(&turnos,sizeof(turno),1,turn);
		printf("\n\nDIAGNOSTICO GUARDADO!");
		system("pause ->NUL");
		fclose(turn);
	dni_ultimo=0;	
	
	fread(&veterinario,sizeof(veterinarios),1,vets);
	while(!feof(vets)){
		if(matricula==veterinario.matricula){
			fseek(vets,-sizeof(veterinarios),SEEK_CUR);
			break;
		}
		fread(&veterinario,sizeof(veterinarios),1,vets);
	}
	
	veterinario.atenciones+=1;  // y se aumenta en uno las mascotas atendidas por el veterinario
	fwrite(&veterinario,sizeof(veterinarios),1,vets);
	fclose(vets);
	}
}
