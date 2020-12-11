#include<stdio.h>
#include<string.h>
#include<windows.h>
#include<locale.h>
#include<conio.h>
#include<time.h>

void gotoxy(int x, int y) {
	HANDLE hcon = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD dwPos;
	dwPos.X = x;
	dwPos.Y = y;
	SetConsoleCursorPosition(hcon, dwPos);
}

struct usuarios {
	bool veterinario;
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
	char nombre[40];
	char apellido[40];
	char domicilio[60];
	int dni_de_dueno;
	char localidad[60];
	fecha Fecha_nacimiento;
	float peso;
	int telefono;
};
struct turno{
	bool atendido;
	int matricula;
	fecha atencion;
	int dni_dueno;
	char detalle_de_atencion[380];
};
struct veterinarios{
	char Apellido_y_nombre[60];
	int matricula;
	int dni;
	int telefono;
	int atenciones;
};

void menuasist(int &op);
void opcionesasist(int &op,bool &iniciada,int dd,int mm,int aaaa);
void iniciar_sesion(bool &iniciada,int &op);
void registrar_mascota(int dd,int mm,int aaaa);
void registrar_turno(int dd,int mm,int aaaa);

main() {
	setlocale(LC_CTYPE, "spanish");
	int dia_actual,mes_actual,anio_actual;
	
	time_t t=time(NULL);
	struct tm today = *localtime(&t);
	mes_actual=today.tm_mon+1;
	dia_actual=today.tm_mday;
	anio_actual=(today.tm_year+1900);
	
	int op=0;
	bool sesion_iniciada=false;

	while(op!=5) {
		menuasist(op);
		if(op!=5)
			opcionesasist(op,sesion_iniciada,dia_actual,mes_actual,anio_actual);
	}
}
void menuasist(int &op) {
	system("cls");
	do {
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
	} while(op!=1 && op!=2 && op!=3 && op!=4 && op!=5);
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
			if(iniciada==false)
				iniciar_sesion(iniciada,op);
			else {
				system("color 0a");
				printf("\n\n\n\n\t\t\t   SESION YA INICADA");
				system("pause ->NUL");
				system("color 07");
			}
			break;
		case 2:
			if(iniciada==false)
				mensaje();
			else
				registrar_mascota(dd,mm,aaaa);
			break;
		case 3:
			if(iniciada==false)
				mensaje();
			else
				registrar_turno(dd,mm,aaaa);
			break;
		case 4:
			if(iniciada==false)
				mensaje();
			else
				printf("Listado de atencion\n\n");
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
void iniciar_sesion(bool &iniciada,int &op) {
	usuarios users;
	char usuario[10],pass[32],opc[2],caracter;
	int coinc,i;

	FILE* arch=fopen("Archives//usuarios.dat","rb");
	if(arch==NULL) {
		Beep(700,300);
		system("color 0e");
		printf("\n\n\n\t\tAún no se registraron usuarios en el sistema\n\n\t\t");
		printf("Contacte a administración para ser registrado");
		system("pause ->NUL");
		op=5;
	} else {
		if(!comprobar_hay_asistentes(arch)) {
			Beep(700,300);
			system("color 0e");
			printf("\n\n\n\t\tAún no se registraron usuarios ASISTENTES\n\n\t\t");
			printf("Contacte a administración para ser registrado");
			system("pause ->NUL");
			op=5;
		} else {
			do {
				i=0;
				rewind(arch);

				system("cls");
				printf("\n\t\tINICIO DE SESIÓN\n\t\t---------------");
				printf("\n\n\n\t\tUsuario: ");
				_flushall();
				gets(usuario);
				printf("\n\t\tContraseña: ");

				while(caracter=getch()) {
					if(caracter==13) {
						pass[i]= '\0';
						break;
					} else if(caracter==8) {
						if(i>0) {
							i--;
							printf("\b \b");
						}
					} else {
						printf("*");
						pass[i]=caracter;
						i++;
					}
				}

				fread(&users,sizeof(usuarios),1,arch);
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
				if(iniciada==false) {
					Beep(700,300);
					system("color 0e");
					printf("\n\n\t\tUsuario y/o contraseña incorrecta");
					printf("\n\n\t\t¿Volver a intentar? S/N: ");
					_flushall();
					gets(opc);
					strlwr(opc);
					system("color 07");
				} else {
					system("color 0a");
					printf("\n\n\t\tBievenido, %s",users.Apellido_y_nombre);
					system("pause ->NUL");
				}
			} while(iniciada==false and strcmp(opc,"s")==0);
		}
		fclose(arch);
		system("color 07");
	}
}
void fecha_valida(char nm[],char ape[],float peso,int &d,int &m,int &a,int dd,int mm,int aaaa){
	bool band=false;
	int cont;
	do{
		cont=0;
		system("cls");
		printf("\n\t\t\tREGISTRO DE MASCOTA                  [ %.2d / %.2d / %d ]\n\t\t\t-------------------\n\n",dd,mm,aaaa);
	printf("\t\t     .Nombre de mascota: %s\n",nm);
	printf("\t\t     .Apellido del dueño: %s\n",ape);
	printf("\t\t     .Peso de mascota: %.2f Kg.",peso);
	printf("\n\n\t\t     FECHA DE NACIMIENTO:\n\n\t\t     Día: ");
	scanf("%d",&d);
	printf("\n\t\t     Mes: ");	scanf("%d",&m);
	printf("\n\t\t     Año (aaaa): ");   scanf("%d",&a);
	
		if(d<1 or d>31 or m<1 or m>12 or a<2000 or a>aaaa){
			system("color 0e");
			Beep(700,300);
			printf("\n\tAsegurese de ingresar un número correcto para días, mes y año");
			system("pause ->NUL");
			cont++;
		}else{
			if(a==aaaa){
				if(m==mm){
					if(d>dd){
					system("color 0e");
					Beep(700,300);
					printf("\n\tLa fecha de nacimiento no puede ser posterior a la fecha actual");
					system("pause ->NUL");
					cont++;
					}
				}
				if(m>mm){
				system("color 0e");
				Beep(700,300);
				printf("\n\tLa fecha de nacimiento no puede ser posterior a la fecha actual");
				system("pause ->NUL");
				cont++;
				}
			}	
		}
	if(cont==0)
		band=true;
	system("color 07");
	}while(band==false);
	system("cls");
}
void registrar_mascota(int dd,int mm,int aaaa){
	mascotas pets;
	int d,m,a;
	float peso;
	char nombre[40],ape[40];
	FILE *arch=fopen("Archives//Mascotas.dat","a+b");
	
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
	
	printf("\n\t\t\tREGISTRO DE MASCOTA\n\t\t\t-------------------\n\n");
	printf("\t\t     .Nombre de mascota: %s",nombre);
	printf("\n\t\t     .Apellido del dueño: %s",ape);
	printf("\n\t\t     .Peso de mascota: %.2f Kg.",peso);
	printf("\n\t\t     .Nacimiento: %.2d / %.2d / %.2d",d,m,a);
	printf("\n\n\t\t     .Localidad:  ");
	_flushall();
	gets(pets.localidad);	strupr(pets.localidad);
	printf("\t\t     .Domicilio: ");
	_flushall();
	gets(pets.domicilio);		strupr(pets.domicilio);
	printf("\t\t     .DNI del dueño: ");
	scanf("%d",&pets.dni_de_dueno);
	printf("\t\t     .Teléfono de contacto: ");
	scanf("%d",&pets.telefono);
	
	strcpy(pets.nombre,nombre);			strcpy(pets.apellido,ape);
	pets.peso=peso;
	pets.Fecha_nacimiento.dia=d;		pets.Fecha_nacimiento.mes=m;
	pets.Fecha_nacimiento.anio=a;
	
	fwrite(&pets,sizeof(mascotas),1,arch);
	fclose(arch);
	system("color 0a");
	printf("\n\n\t\t     DATOS REGISTRADOS CORRECTAMENTE");
	system("pause ->NUL");
	system("color 07");
}
void registrar_turno(int dd,int mm,int aaaa){
	int dni,mat,d,m,a,salir=0;
	char op[2];
	bool pasa=false;
	veterinarios vetes;
	mascotas mascota;
	turno turn;
	
	FILE *vets=fopen("Archives//Veterinarios.dat","rb");
	FILE *turnos=fopen("Archives//Turnos.dat","a+b");
	FILE *pets=fopen("Archives//Mascotas.dat","rb");
	
	if(pets==NULL){
		Beep(700,300);
			system("color 0e");
			printf("\n\n\n\t\tAún no hay mascotas registradas en el sistema");
			system("pause ->NUL");
	}
	else if(vets==NULL){
		Beep(700,300);
			system("color 0e");
			printf("\n\n\n\t\tAún no hay veterinarios registrados en el sistema\n\t\t");
			printf("         no podrá registrar el turno");
			system("pause ->NUL");
	}
	if(pets!=NULL and vets!=NULL){
		while(pasa==false && salir==0){
			system("cls");
			printf("\n\t\t\t  TURNOS\n\t\t\t  ------\n\n");
			printf("\t.DNI Del dueño de la mascota: ");
			scanf("%d",&dni);
			rewind(pets);
			fread(&mascota,sizeof(mascotas),1,pets);
			while(!feof(pets) and pasa==false){
				if(dni==mascota.dni_de_dueno){
					pasa= true;
				}
				else{
					fread(&mascota,sizeof(mascotas),1,pets);
				}
			}
			if(pasa==false){
				Beep(700,300);
				system("color 0e");
				printf("\n\tEl DNI %d no está registrado en el sistema de datos de mascotas",dni);
				printf("\n\n\t¿Desea ingresar nuevamente? S/N: ");
				_flushall();	gets(op);	strlwr(op);
				system("color 07");
				if(strcmp(op,"s")!=0)
				salir++;
			}
		}

		pasa=false;
		
		while(pasa==false && salir==0){
			system("cls");
			printf("\n\t\t\t  TURNOS\n\t\t\t  ------\n\n");
			printf("\tDATOS DE MASCOTA:\n\n");
			printf("\t.Nombre y Apellido: %s, %s -- Peso: %.2f Kg.\n",mascota.nombre,mascota.apellido,mascota.peso);
			printf("\n\tDATOS DE VETERINARIO:\n");
			printf("\n\t.Nro. de matrícula: ");		scanf("%d",&mat);
			rewind(vets);
			
			fread(&vetes,sizeof(veterinarios),1,vets);
			while(!feof(vets) and pasa==false){
				if(mat==vetes.matricula){
					pasa= true;
				}
				else{
					fread(&vetes,sizeof(veterinarios),1,vets);
				}
			}
			if(pasa==false){
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
		
		while(pasa==false && salir==0){
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
			printf("\n\n\t\t¿CONFIRMAR TURNO? S/N: ");
			_flushall();	gets(op);	strlwr(op);
			if(strcmp(op,"s")!=0)
			salir++;
		}
		
		if(salir>0){
			Beep(700,300);
				system("color 0e");
				printf("\n\t¡TURNO CANCELADO!");
		}
		else{
			system("color 0a");
				printf("\n\tTURNO REGISTRADO CORRECTAMENTE");
		}
		fclose(vets);
		fclose(pets);
	}	
	system("pause ->NUL");
	system("color 07"); 
	fclose(turnos);
}
