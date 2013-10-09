#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <curses.h>
/*header pid_t*/
#include <unistd.h>
#include <sys/types.h>

int eax=0,ebx=0,ecx=0,edx=0,pc= 0;
char auxIR[15];	
char nombreArchivo[50];
char auxarchi[15];
int pid=1;

void dispatcher();
int deletePID(int pid);
/*void ejecucion(char archi[],int pid);*/
void imprime();
void imprimePCB(char archi[],int pidpcb);
void insertFinal(char *nameArchivo,int PID);
int kbhit(void);
int moveFinal(int pidmover);
void MuestraArch();
int proceso(char archi[],int pid);
void readArch();
void saveContext();
void status(int edo);

/*estructuras para colas*/
struct Nodolistos{
	/* data */
	int actualPID;
	char nombreArchivo[15];
	char estado[15];
	char nombreArch[15];
	int pc;
	char auxIR[15];
	int eax,ebx,ecx,edx;
	//FILE *arch;
	struct Nodolistos *siguiente;
};
struct Nodolistos *iniciolistos;


/*+++++++++++++++++++++++++++++*/

/*++++FUNCIONES++++*/

void dispatcher(){
	struct Nodolistos *temp;
	temp=iniciolistos;
	if(temp==NULL){
		printf("Lista de listos VACIA\n");
	}else{
		proceso(temp->nombreArchivo,temp->actualPID);
	}
}

int deletePID(int pid){
	struct Nodolistos *temp, *anterior;
	int encontrado=0;

	if(iniciolistos==NULL){
		return encontrado;
	}
	temp=iniciolistos;
	anterior=NULL;
	while(temp!=NULL && encontrado==0){
		if(temp->actualPID==pid){
			encontrado=1;
			if(anterior==NULL){
				iniciolistos=temp->siguiente;
			}else{
				anterior->siguiente=temp->siguiente;
			}
			free(temp);
		}else{
			anterior=temp;
			temp=temp->siguiente;
		}
	}
	return encontrado;
}

void imprime(int pc){
	
	initscr();

	mvprintw(3,2,"------------------------CPU------------------------");
	mvprintw(4,2,"PC\tIR\t\tEAX\tEBX\tECX\tEDX\n");

	refresh();mvprintw(5,2,"%d",pc);refresh();
	refresh();mvprintw(5,8,"%s      ",auxIR);
	refresh();mvprintw(5,24,"%X     ",eax);
	refresh();mvprintw(5,32,"%X     ",ebx);
	refresh();mvprintw(5,40,"%X     ",ecx);
	refresh();mvprintw(5,48,"%X     ",edx);

	usleep(900000);
}

void imprimePCB(char archi[],int pidpcb){
	int renglon=12;
	mvprintw(10,2,"-----------------------PCB------------------------");
	mvprintw(11,2,"PID\tArch\tEstado\tPC\tIR\tEAX\tEBX\tEDX\tECX\n");	
		mvprintw(renglon,1,"                                                                                                                       ");
		refresh();mvprintw(renglon+pidpcb,3,"%d",iniciolistos->actualPID=pidpcb);
		refresh();mvprintw(renglon+pidpcb,8,"%s",strcpy(iniciolistos->nombreArch,archi));
		refresh();mvprintw(renglon+pidpcb,24,"%d",iniciolistos->pc=pc);
		refresh();mvprintw(renglon+pidpcb,27,"%s",strcpy(iniciolistos->auxIR,auxIR));
		refresh();mvprintw(renglon+pidpcb,42,"%X",iniciolistos->eax=eax);
		refresh();mvprintw(renglon+pidpcb,49,"%X",iniciolistos->ebx=ebx);
		refresh();mvprintw(renglon+pidpcb,56,"%X",iniciolistos->ecx=ecx);
		refresh();mvprintw(renglon+pidpcb,64,"%X",iniciolistos->edx=edx);
}

/*funcion que inserta al final de la lista*/
void insertFinal(char *nameArchivo,int PID){
	struct Nodolistos *temp, *temp2;
	//Apartamos ram para nuevo nodo
	temp = (struct Nodolistos *) malloc(sizeof(struct Nodolistos));
	//llenamos campos con los valores recibidos
	temp ->actualPID = PID;
	strcpy(temp->nombreArchivo,nameArchivo);
	temp->siguiente = NULL;

	if (iniciolistos==NULL)//si es el primero, apunta a nuevo nodo
	{
		iniciolistos = temp;
	}else{ //buscamos el ultimo nodo
		temp2=iniciolistos;
		while(temp2->siguiente != NULL){
			temp2=temp2->siguiente;
		}
		temp2->siguiente=temp;
	}
}

int kbhit(void){
    struct timeval tv;
    fd_set read_fd;
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    FD_ZERO(&read_fd);
    FD_SET(0, &read_fd);

    if (select(1, &read_fd, NULL, NULL, &tv) == -1)
        return 0;
    if (FD_ISSET(0, &read_fd))
        return 1;
    return 0;
}

int moveFinal(int pidmover){
	struct Nodolistos *temp, *auxMover;//temp: recorre la lista, auxMover: se movera al final.
	int encontrado=0;
	temp = iniciolistos;
	auxMover=iniciolistos;

		while(temp!=NULL && encontrado==0){
			if (temp->actualPID==pidmover && temp->siguiente==NULL)
			{
				break;
			}
			temp=temp->siguiente;
			if(temp->siguiente==NULL){
				encontrado=1;
				if(temp->actualPID==pidmover){
					temp=iniciolistos;
				}else{
					iniciolistos=auxMover->siguiente;
					temp->siguiente=auxMover;
					auxMover->siguiente=NULL;	
				}
			}
		}
		return encontrado;
}


void MuestraArch(){
	struct Nodolistos *temp;
	int i=1;
	temp=iniciolistos;
	while(temp != NULL){
		printf("PID de Archivos: %d\n", temp->actualPID);
		printf("nombreArchivo: %s\n",temp->nombreArchivo );
		i++;
		temp=temp->siguiente;
	}
}



int proceso(char archi[],int pidproceso){
	eax=0,ebx=0,ecx=0,edx=0;
	int quantum = 0;
	FILE *archivo;
	struct Nodolistos *pcbprocess;
	char *instruccion;
	char *registroextra;
	char *registro;
	char *regconst;
	char *resto;
	char IR[15];
	
	pc=0;
	/*Var Aux*/
	int operacion=0,operador1=0,operador2=0;
	int bandera1;
	/*delimitadores*/
	char del[4]=", \n";
	char del2[2]="\n";
	int estado = 1;
	strcpy(auxarchi,archi);

	if((archivo = fopen(archi,"r"))!=NULL){
		while(!feof(archivo)){
			fgets(IR,15,archivo);
			if(strcmp(IR,"\n")==0){printf("tienes un renglon vacio,pc = %d\n",pc);continue;}//verifica si hay un renglon vacio
			else{
				if(feof(archivo)){
					moveFinal(pidproceso);
				}else{
					strcpy(auxIR,IR);
					strtok(auxIR,del2);
					instruccion=strtok(IR,del);
					if(strcmp(instruccion,"MOV")==0){
						operacion=1;			
					}else if(strcmp(instruccion,"ADD")==0){
						operacion=2;
					}else if(strcmp(instruccion,"SUB")==0){
						operacion=3;
					}else if(strcmp(instruccion,"MUL")==0){
						operacion=4;
					}else if(strcmp(instruccion,"DIV")==0){
						operacion=5;
					}else if(strcmp(instruccion,"INC")==0){
						operacion=6;
					}else if(strcmp(instruccion,"DEC")==0){
						operacion=7;
					}else if(strcmp(instruccion,"END")==0){
						operacion=8;
					}
					if(operacion==8){
						estado=3;
						//status(estado);
					}else{
								registro=strtok(NULL,del);	
								
								if(strcmp(registro,"EAX")==0){
									operador1=eax;
									bandera1=1;
								}else if(strcmp(registro,"EBX")==0){
									operador1=ebx;
									bandera1=2;
								}else if(strcmp(registro,"ECX")==0){
									operador1=ecx;
									bandera1=3;
								}else if(strcmp(registro,"EDX")==0){
									operador1=edx;
									bandera1=4;
								}

								if(operacion==7 || operacion==6){								
									if (strtok(NULL,del)){
										printf("tienes un registro extra, pc =%d\n",pc);
										return 0;
									}
									if(operacion==7){
										operador1--;
										if(bandera1==1){
											eax=operador1;
										}else if(bandera1==2){
											ebx=operador1;
										}else if(bandera1==3){
											ecx=operador1;
										}else{
											edx=operador1;
										}
									}

									if(operacion==6){
										operador1++;
										if(bandera1==1){
											eax=operador1;
										}else if(bandera1==2){
											ebx=operador1;
										}else if(bandera1==3){
											ecx=operador1;
										}else{
											edx=operador1;
										}
									}
								}else{
									regconst=strtok(NULL,del);
									registroextra = strtok(NULL,del);
									if (registroextra){
										//printf("\n");
										printf("tienes un error en la sintaxis, pc = %d\n",pc);
										//endwin();
										//printf("pc =%d\n",pc);
										break;	
										return 0;
									}	
									//printf("%s\n",regconst);
									if(strcmp(regconst,"EAX")==0){
											operador2=eax;		
									}else if(strcmp(regconst,"EBX")==0){
											operador2=ebx;
									}else if(strcmp(regconst,"ECX")==0){
											operador2=ecx;
									}else if(strcmp(regconst,"EDX")==0){
											operador2=edx;
									}else{
										//operador2=atoi(regconst);
										operador2 = strtol(regconst,&resto,16);
										if(operador2<0){printf("\nel valor es menor que cero\n");break;}	
									}

									/*Realizamos Operaciones*/
									/*MOV*/
									if(operacion==1){
										operador1=operador2;
									if(bandera1==1){
										eax=operador1;
									}else if(bandera1==2){
										ebx=operador1;
									}else if(bandera1==3){
										ecx=operador1;
									}else{
										edx=operador1;
									}	
									/*ADD*/
									}else if(operacion==2){
										operador1+=operador2;
										if(bandera1==1){
											eax=operador1;
										}else if(bandera1==2){
											ebx=operador1;
										}else if(bandera1==3){
											ecx=operador1;
										}else{
											edx=operador1;
										}
										/*SUB*/
									}else if(operacion==3){
										operador1-=operador2;
										if(bandera1==1){
											eax=operador1;
										}else if(bandera1==2){
											ebx=operador1;
										}else if(bandera1==3){
											ecx=operador1;
										}else{
											edx=operador1;
										}
										/*MUL*/
									}else if(operacion==4){
										operador1*=operador2;
										if(bandera1==1){
											eax=operador1;
										}else if(bandera1==2){
											ebx=operador1;
										}else if(bandera1==3){
											ecx=operador1;
										}else{
											edx=operador1;
										}
										/*DIV*/
									}else if(operacion==5){//si la operacion es la division y el dividendo es 0 termina la ejecucion
										if(operador2==0){mvprintw(20,3,"tienes una division entre cero,pc= %d",pc);pause();}
										operador1/=operador2;
										if(bandera1==1){
											eax=operador1;
										}else if(bandera1==2){
											ebx=operador1;
										}else if(bandera1==3){
											ecx=operador1;
										}else{
											edx=operador1;
										}
									}
								}							
					
					}
					pc++;
					quantum++;
						/*****/
							if(kbhit()){
								pid++;
								refresh();
								mvscanw(2,5,"%s",nombreArchivo);
								if(strcmp(nombreArchivo,"salir")==0){endwin();MuestraArch();exit(0);}
								else insertFinal(nombreArchivo,pid);
							}
						/*****/

					imprime(pc);

					if (quantum%3==0){
						//estado=2;
						//saveContext();
						imprimePCB(archi,pidproceso);
						//status(estado);
						//fclose(archivo);
						goto etiqueta_1;
						
					}
					//status(estado);
				}//fin else feof(archivo)

			}//fin else strcmp
		}//fin while

		fclose(archivo);
		printf("archivorecrrado\n");
		
	}else{
		printf("error al abrir");
	}
	//moveFinal(pidproceso);
	etiqueta_1:
	moveFinal(pidproceso);
	dispatcher();
return 0;	
}

void readArch(){
		refresh();	
		mvprintw(2,3,"$");
		mvscanw(2,5,"%s",nombreArchivo);
		insertFinal(nombreArchivo,pid);
		refresh();
		dispatcher();	
}

/*++++FIN FUNCIONES++++*/

int  main(void){
	//declaracion cabezera de listos
	iniciolistos=NULL;
	initscr();
	refresh();
	readArch();
	endwin();
return 0;
}

