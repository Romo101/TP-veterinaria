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
	int matricula;
	fecha atencion;
	int dni_dueno;
	char detalle_de_atencion[380];
};

void menuasist(int &op);
void opcionesasist(int &op,bool &iniciada);
void iniciar_sesion(bool &iniciada,int &op);
void registrar_mascota();

main() {
	setlocale(LC_CTYPE, "spanish");
	
	int op=0;
	bool sesion_iniciada=false;

	while(op!=5) {
		menuasist(op);
		if(op!=5)
			opcionesasist(op,sesion_iniciada);
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
void opcionesasist(int &op,bool &iniciada) {
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
				registrar_mascota();
			break;
		case 3:
			if(iniciada==false)
				mensaje();
			else
				printf("registrar turno\n\n");
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

	FILE* arch=fopen("usuarios.dat","rb");
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
void fecha_valida(char nm[],char ape[],float peso,int &d,int &m,int &a){
	int dd,mm,aaaa;
	
	time_t t=time(NULL);
	struct tm today = *localtime(&t);
	mm=today.tm_mon+1;
	dd=today.tm_mday;
	aaaa=(today.tm_year+1900);
	
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
void registrar_mascota(){
	mascotas pets;
	int d,m,a;
	float peso;
	char nombre[40],ape[40];
	FILE *arch=fopen("Macotas.dat","a+b");
	
	printf("\n\t\t\tREGISTRO DE MASCOTA\n\t\t\t-------------------\n\n");
	printf("\t\t     .Nombre de mascota: ");
	_flushall();
	gets(nombre);	strupr(nombre);
	printf("\t\t     .Apellido del dueño: ");
	_flushall();
	gets(ape);		strupr(ape);
	printf("\t\t     .Peso de mascota: ");
	scanf("%f",&peso);
	fecha_valida(nombre,ape,peso,d,m,a);
	
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
