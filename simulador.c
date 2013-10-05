#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <curses.h>
/*header pid_t*/
#include <unistd.h>
#include <sys/types.h>

int eax=0,ebx=0,ecx=0,edx=0,pc= 0;
char auxIR[15];	
int activo;
void imprime();
int kbhit(void);
int dispatcher();
char nombreArchivo[50];
int estado;
int pid=1;

struct pcb{
int pid;
int estado;
int pc;
int eax,ebx,ecx,edx;

struct pcb *siguiente;
}PCB;

struct Nodolistos
{
<<<<<<< HEAD
	/* data */
	int actualPID;
	char nombreArchivo[50];

	struct Nodolistos *siguiente;
=======
	int actual
	struct listos *siguiente;
};
struct listos *list;

struct ejecucion
{
>>>>>>> 38bd80056778b6597828aedb5a9e351ca34179a8
};
struct Nodolistos *iniciolistos;

/*funcion que inserta al final de la lista*/
void insertaFinal(char *nameArchivo,int PID){
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

void moverFinal(){
	struct Nodolistos *temp, *auxMover;//temp: recorre la lista, auxMover: se movera al final.
	temp = iniciolistos;
	auxMover=NULL;

		while(temp!=NULL){

			temp=temp->siguiente;
			if(temp ->siguiente!=NULL && auxMover==NULL){
				auxMover=iniciolistos;
			}
			if(temp->siguiente==NULL && auxMover==NULL){
				free(temp);
			}
			if(temp->siguiente==NULL && auxMover!=NULL){
				iniciolistos=auxMover->siguiente;
				temp->siguiente=auxMover;
				auxMover->siguiente=NULL;
			}
		}
}

void MuestraArch(){
	struct Nodolistos *temp;
	int i=1;
	temp=iniciolistos;
	while(temp != NULL){
		printf("PIB de Archivos: %d\n", temp->actualPID);
		printf("nombreArchivo: %s\n",temp->nombreArchivo );
		i++;
		temp=temp->siguiente;
	}
}

int dispatcher(){
	struct Nodolistos *temp;
	temp=iniciolistos;
	if(temp==NULL){
		printf("Lista de listos VACIA\n");
	}else{
		proceso(temp->nombreArchivo);
	}
}

void saveContext(){
PCB.pc =pc;
PCB.eax=eax;
PCB.ebx= ebx;
PCB.ecx= ecx;
PCB.edx= edx;
}

int proceso(char archi[]){
	eax=0,ebx=0,ecx=0,edx=0;
	int quantum = 0;
	FILE *archivo;

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
	activo = 1;	

	if((archivo = fopen(nombreArchivo,"r"))!=NULL){
		//printf("archivo abierto correctamente\n\n");
		while(!feof(archivo)){
			fgets(IR,15,archivo);
			if(strcmp(IR,"\n")==0){printf("tienes un renglon vacio,pc = %d\n",pc);exit(1);}//verifica si hay un renglon vacio
			else{
				if(feof(archivo)){
					
				}else{
					strcpy(auxIR,IR);
					strtok(auxIR,del2);
					instruccion=strtok(IR,del);
					if(strcmp(instruccion,"MOV")==0){
						//printf("es MOV\n");
						operacion=1;			
					}else if(strcmp(instruccion,"ADD")==0){
						//printf("es ADD\n");
						operacion=2;
					}else if(strcmp(instruccion,"SUB")==0){
						//printf("es SUB\n");
						operacion=3;
					}else if(strcmp(instruccion,"MUL")==0){
						//printf("es MUL\n");
						operacion=4;
					}else if(strcmp(instruccion,"DIV")==0){
						//printf("es DIV\n");
						operacion=5;
					}else if(strcmp(instruccion,"INC")==0){
						//printf("es INC\n");
						operacion=6;
					}else if(strcmp(instruccion,"DEC")==0){
						//printf("es DEC\n");
						operacion=7;
					}else if(strcmp(instruccion,"END")==0){
						//printf("es END\n");
						operacion=8;
					}
					if(operacion==8){
						//printf("adios\n");
						estado= 0;
						//exit(1);

					}else{
			
								registro=strtok(NULL,del);
							
							
								//printf("%s\n",registro);
								if(strcmp(registro,"EAX")==0){
									//printf("es EAX\n");
									operador1=eax;
									bandera1=1;
								}else if(strcmp(registro,"EBX")==0){
									//printf("es EBX\n");
									operador1=ebx;
									bandera1=2;
								}else if(strcmp(registro,"ECX")==0){
									//printf("es ECX\n");
									operador1=ecx;
									bandera1=3;
								}else if(strcmp(registro,"EDX")==0){
									//printf("es EDX\n");
									operador1=edx;
									bandera1=4;
								}

								if(operacion==7 || operacion==6){
									//registroextra= strtok(NULL,del);								
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
										endwin();
										//printf("pc =%d\n",pc);
										return 0;
									}	
									//printf("%s\n",regconst);
									if(strcmp(regconst,"EAX")==0){
										//printf("es EAX\n");
											operador2=eax;		
									}else if(strcmp(regconst,"EBX")==0){
										//printf("es EBX\n");
											operador2=ebx;
									}else if(strcmp(regconst,"ECX")==0){
										//printf("es ECX\n");
											operador2=ecx;
									}else if(strcmp(regconst,"EDX")==0){
										//printf("es EDX\n");
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
					
					imprime(pc);
					
					pc++;
					quantum++;
					if(quantum%3==0){
						//moverFinal();
						saveContext();
					}
					refresh();
				}
			
			}//fin else feof(archivo)
		}//fin else strcmp
	}//fin while
	fclose(archivo);
	}else{
		printf("error al abrir");
	}
return 0;
}


void imprime(int pc){
	
	initscr();
	mvprintw(1,3,"$");

	mvprintw(3,2,"------------------------CPU------------------------");
	mvprintw(4,2,"PC\tIR\t\tEAX\tEBX\tECX\tEDX\n");


	mvprintw(10,2,"-----------------------PCB------------------------");
	mvprintw(11,2,"PID\tEstado\t\tPC\tIR\tEAX\tEBX\tEDX\tECX\n");

	refresh();mvprintw(5,2,"%d",pc);refresh();
	refresh();mvprintw(5,8,"%s      ",auxIR);
	refresh();mvprintw(5,24,"%X     ",eax);
	refresh();mvprintw(5,32,"%X     ",ebx);
	refresh();mvprintw(5,40,"%X     ",ecx);
	refresh();mvprintw(5,48,"%X     ",edx);
	mvprintw(12,24,"%d",PCB.pc);
	mvprintw(12,40,"%X",PCB.eax);
	mvprintw(12,48,"%X",PCB.ebx);
	mvprintw(12,56,"%X",PCB.ecx);
	mvprintw(12,64,"%X",PCB.edx);
	usleep(700000);
	
		if(kbhit()){
			refresh();
			mvscanw(2,5,"%s",nombreArchivo);
			if(strcmp(nombreArchivo,"salir")==0){
				endwin();
				//MuestraArch();
				exit(0);
			}else{
				pid++;
				//saveContext();
				insertaFinal(nombreArchivo,pid);
				moverFinal();
				dispatcher();
			}
			//MuestraArch();
		}

}

int kbhit(void) 
{
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

int  main(void){
	//declaracion cabezera de listos
	iniciolistos=NULL;
	initscr();
	mvprintw(2,3,"$");
	mvscanw(2,5,"%s",nombreArchivo);
	insertaFinal(nombreArchivo,pid);
	refresh();
	dispatcher();

	endwin();
return 0;
}

