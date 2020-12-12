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
void listar(int dd,int mm,int aaaa);

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
	printf("\n\n\n\n\t\t\t   PRIMERO INICIE SESI�N");
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
				listar(dd,mm,aaaa);
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
		printf("\n\n\n\t\tA�n no se registraron usuarios en el sistema\n\n\t\t");
		printf("Contacte a administraci�n para ser registrado");
		system("pause ->NUL");
		op=5;
	} else {
		if(!comprobar_hay_asistentes(arch)) {
			Beep(700,300);
			system("color 0e");
			printf("\n\n\n\t\tA�n no se registraron usuarios ASISTENTES\n\n\t\t");
			printf("Contacte a administraci�n para ser registrado");
			system("pause ->NUL");
			op=5;
		} else {
			do {
				i=0;
				rewind(arch);

				system("cls");
				printf("\n\t\tINICIO DE SESI�N\n\t\t---------------");
				printf("\n\n\n\t\tUsuario: ");
				_flushall();
				gets(usuario);
				printf("\n\t\tContrase�a: ");

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
					printf("\n\n\t\tUsuario y/o contrase�a incorrecta");
					printf("\n\n\t\t�Volver a intentar? S/N: ");
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
	}
	system("color 07");
}
void fecha_valida(char nm[],char ape[],float peso,int &d,int &m,int &a,int dd,int mm,int aaaa){
	bool band=false;
	int cont;
	do{
		cont=0;
		system("cls");
		printf("\n\t\t\tREGISTRO DE MASCOTA                  [ %.2d / %.2d / %d ]\n\t\t\t-------------------\n\n",dd,mm,aaaa);
	printf("\t\t     .Nombre de mascota: %s\n",nm);
	printf("\t\t     .Apellido del due�o: %s\n",ape);
	printf("\t\t     .Peso de mascota: %.2f Kg.",peso);
	printf("\n\n\t\t     FECHA DE NACIMIENTO:\n\n\t\t     D�a: ");
	scanf("%d",&d);
	printf("\n\t\t     Mes: ");	scanf("%d",&m);
	printf("\n\t\t     A�o (aaaa): ");   scanf("%d",&a);
	
		if(d<1 or d>31 or m<1 or m>12 or a<2000 or a>aaaa){
			system("color 0e");
			Beep(700,300);
			printf("\n\tAsegurese de ingresar un n�mero correcto para d�as, mes y a�o");
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
	char nombre[40],ape[40],op;
	FILE *arch=fopen("Mascotas.dat","a+b");
	
	printf("\n\t\t\tREGISTRO DE MASCOTA\n\t\t\t-------------------\n\n");
	printf("\t\t     .Nombre de mascota: ");
	_flushall();
	gets(nombre);	strupr(nombre);
	printf("\t\t     .Apellido del due�o: ");
	_flushall();
	gets(ape);		strupr(ape);
	printf("\t\t     .Peso de mascota: ");
	scanf("%f",&peso);
	fecha_valida(nombre,ape,peso,d,m,a,dd,mm,aaaa);
	
	printf("\n\t\t\tREGISTRO DE MASCOTA\n\t\t\t-------------------\n\n");
	printf("\t\t     .Nombre de mascota: %s",nombre);
	printf("\n\t\t     .Apellido del due�o: %s",ape);
	printf("\n\t\t     .Peso de mascota: %.2f Kg.",peso);
	printf("\n\t\t     .Nacimiento: %.2d / %.2d / %.2d",d,m,a);
	printf("\n\n\t\t     .Localidad:  ");
	_flushall();
	gets(pets.localidad);	strupr(pets.localidad);
	printf("\t\t     .Domicilio: ");
	_flushall();
	gets(pets.domicilio);		strupr(pets.domicilio);
	printf("\t\t     .DNI del due�o: ");
	scanf("%d",&pets.dni_de_dueno);
	printf("\t\t     .Tel�fono de contacto: ");
	scanf("%d",&pets.telefono);
	
	printf("\n\t\t�CONFIRMAR REGISTRO DE MASCOTA? S/N: ");
	_flushall();
	scanf("%c",&op);
	
	if(op=='s' or op=='S'){
	strcpy(pets.nombre,nombre);			strcpy(pets.apellido,ape);
	pets.peso=peso;
	pets.Fecha_nacimiento.dia=d;		pets.Fecha_nacimiento.mes=m;
	pets.Fecha_nacimiento.anio=a;
	fwrite(&pets,sizeof(mascotas),1,arch);
	fclose(arch);
	//pets.borrar=false; ---> verificar si haria falta
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
void registrar_turno(int dd,int mm,int aaaa){
	int dni,mat,d,m,a,salir=0;
	char op[2];
	bool pasa=false;
	veterinarios vetes;
	mascotas mascota;
	turno turn;
	
	FILE *vets=fopen("Veterinarios.dat","rb");
	FILE *pets=fopen("Mascotas.dat","rb");
	
	if(pets==NULL){
		Beep(700,300);
			system("color 0e");
			printf("\n\n\n\t\tA�n no hay mascotas registradas en el sistema");
	}
	else if(vets==NULL){
		Beep(700,300);
			system("color 0e");
			printf("\n\n\n\t\tA�n no hay veterinarios registrados en el sistema\n\t\t");
			printf("         no podr� registrar el turno");
	}
	if(pets!=NULL and vets!=NULL){
		
			FILE *turnos=fopen("Turnos.dat","a+b");
		
		while(pasa==false && salir==0){
			system("cls");
			printf("\n\t\t\t  TURNOS\n\t\t\t  ------\n\n");
			printf("\t.DNI Del due�o de la mascota: ");
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
				printf("\n\tEl DNI %d no est� registrado en el sistema de datos de mascotas",dni);
				printf("\n\n\t�Desea ingresar nuevamente? S/N: ");
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
			printf("\n\t.Nro. de matr�cula: ");		scanf("%d",&mat);
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
				printf("\n\tLa m�tricula %d no est� registrada en el sistema de veterinarios",mat);
				printf("\n\n\t�Desea ingresar nuevamente? S/N: ");
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
			printf("\n\t.Nro. de matr�cula: %d\n",mat);
			printf("\t.Apellido y Nombre: %s\n\t.DNI: %d\n\n",vetes.Apellido_y_nombre,vetes.dni);
			printf("\tFECHA DE TURNO:\n\n");
			printf("\t.D�a: ");		scanf("%d",&d);
			printf("\t.Mes: ");		scanf("%d",&m);
			printf("\t.A�o: ");		scanf("%d",&a);
			
			if(d<1 or d>31 or m>12 or m<1 or a<aaaa or a>9999){
			system("color 0e");
			Beep(700,300);
			printf("\n\tAsegurese de ingresar un n�mero correcto para d�as, mes y a�o");
			system("pause ->NUL");
			}else if(a==aaaa){
				if(m==mm){
					if(d<dd){
					system("color 0e");
					Beep(700,300);
					printf("\n\tEl turno no puede ser anterior al d�a de hoy (%.2d/%.2d/%d)",dd,mm,aaaa);
					system("pause ->NUL");
					}
				}
				if(m<mm){
				system("color 0e");
				Beep(700,300);
				printf("\n\tEl turno no puede ser anterior al d�a de hoy (%.2d/%.2d/%d)",dd,mm,aaaa);
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
			printf("\n\t.Nro. de matr�cula: %d\n",mat);
			printf("\t.Apellido y Nombre: %s\n\t.DNI: %d\n\n",vetes.Apellido_y_nombre,vetes.dni);
			printf("\t.Fecha de turno: %.2d/%.2d%d",d,m,a);
			printf("\n\n\t\t�CONFIRMAR TURNO? S/N: ");
			_flushall();	gets(op);	strlwr(op);
			if(strcmp(op,"s")!=0)
			salir++;
		}
		
		if(salir>0){
			Beep(700,300);
				system("color 0e");
				printf("\n\t�TURNO CANCELADO!");
		}
		else{
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
		fclose(pets);
		fclose(turnos);
	}
	system("pause ->NUL");
	system("color 07"); 
}
void listar(int dd,int mm,int aaaa){
	typedef char diagnostico[380];
	typedef char nombre[60];
	char op;
	nombre mascota_nombres[20];
	char veterinario_nombre[60];
	bool veterinario_encontrado;
	diagnostico diagnosticos[20];
	FILE *turnos=fopen("Turnos.dat","rb");
	FILE *vets=fopen("Veterinarios.dat","rb");
	bool pasa;
	
	int d,m,a,i,j,n;
	int matricula;
	
	if(turnos==NULL){
		Beep(700,300);
		system("color 0e");
		printf("\n\n\n\t\t\tA�n no se registraron turnos");
		system("pause ->NUL");
	}else if(vets==NULL){
		Beep(700,300);
		system("color 0e");
		printf("\n\n\n\t\t\tA�n no se registraron Veterinarios");
		system("pause ->NUL");
	}
	if(turnos!=NULL and vets!=NULL){
	FILE *pets=fopen("Mascotas.dat","rb");
		turno turn;
		veterinarios veterinario;
		mascotas mascota;
		
		do{	i=0;
		system("color 07");
		pasa=false;
		printf("\n\t\tFECHA A BUSCAR ATENCIONES\n\n");
		printf("\t\tD�a: ");	scanf("%d",&d);
		printf("\t\tMes: ");	scanf("%d",&m);
		printf("\t\tA�o: ");	scanf("%d",&a);
		if(d<1 or d>31 or m>12 or m<1 or a<aaaa-5 or a>aaaa){
			i++;
			system("color 0e");
			Beep(700,300);
			printf("\n\tAsegurese de ingresar un n�mero correcto para d�as, mes y a�o");
				if(a<aaaa-5)
				printf("\n\tSe guardan solo las atenciones de los ultimos 5 a�os");
				if(a>aaaa)
				printf("\n\tEl a�o no puede ser posterior al a�o en curso (%d)",aaaa);
			system("pause ->NUL");
			}else if(a==aaaa){
				if(m==mm){
					i++;
					if(d>dd){
					system("color 0e");
					Beep(700,300);
					printf("\n\tLas atenciones no pueden ser posteriores al d�a de hoy (%.2d/%.2d/%d)",dd,mm,aaaa);
					system("pause ->NUL");
					}
				}
				if(m>mm){
					i++;
				system("color 0e");
				Beep(700,300);
				printf("\n\tLas atenciones no pueden ser posteriores al d�a de hoy (%.2d/%.2d/%d)",dd,mm,aaaa);
				system("pause ->NUL");
				}
			}
			if(i==0)
				pasa=true;
			system("cls");
		}while(pasa==false);
			
			do{	system("cls");			system("color 07");
				i=0,j=0;
				veterinario_encontrado=false;
				rewind(turnos);	rewind(vets); rewind(pets);
			printf("\n\n\t.Fecha de atenci�n %.2d/%.2d/%d",d,m,a);
			printf("\n\t.Matr�cula de Veterinario: ");
			scanf("%d",&matricula);
			
			fread(&turn,sizeof(turno),1,turnos);
			while(!feof(turnos)){
				if(matricula==turn.matricula && veterinario_encontrado==false){
					fread(&veterinario,sizeof(veterinarios),1,vets);
					while(!feof(vets)){
					if(matricula==veterinario.matricula){
						veterinario_encontrado=true;
					strcpy(veterinario_nombre,veterinario.Apellido_y_nombre);
					}
					fread(&veterinario,sizeof(veterinarios),1,vets);
					}
				}
				if(veterinario_encontrado==false){
				break;
				}
				
			if(turn.matricula==matricula && turn.atendido==true && turn.atencion.dia==d && turn.atencion.dia==m && turn.atencion.dia==a)
			{
				strcpy(diagnosticos[i],turn.detalle_de_atencion);
				i++;
			}
			fread(&turn,sizeof(turno),1,turnos);
			}
		if(veterinario_encontrado==false){
				Beep(700,300);
				system("color 0e");
			printf("\n\t.La matr�cula %d no est� registrada en el sistema",matricula);
			printf("\n\t �DESEA INGRESAR UNA DISTINTA? S/N: ");
			_flushall(); scanf("%c",&op);
		}else{
			if(i==0){
				Beep(700,300);
				system("color 0e");
				printf("\n\t.Veterinario: %s",veterinario_nombre);
				printf("\n\n\tNO REGISTRA ATENCIONES EL D�A INDICADO");
				system("pause ->NUL");
			}
			else{
				n=i-1;
				printf("\n\t.Veterinario: %s",veterinario_nombre);
				printf("\n\n\tREGISTRA %d ATENCIONES EL D�A INDICADO",n);
				printf("\n\n\tPRESIONA UNA TECLA PARA VER LOS DETALLES DE ATENCION REGISTRADOS...");
				system("pause ->NUL");
				for(i=0;i<n;i++){
					system("cls");
					printf("(%d)\n\nFECHA %.2d/%.2d/%d\nVETERINARIO: %s  ��  MATR�CULA %d\n\n",i+1,d,m,a,veterinario_nombre,matricula);
					printf("DETALLE DE ATENCION: %s",diagnosticos[i]);
					system("pause -> NUL");
				}
			}
		}
		}while(op=='s' or op=='S');
	}
	system("color 07");
}
