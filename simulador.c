#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <curses.h>
#include <panel.h>
/*header pid_t*/
#include <unistd.h>
#include <sys/types.h>

char auxIR[15];
int eax=0,ebx=0,ecx=0,edx=0,pc= 0;
char nombreArchivo[50];
int pid=1;

/*estructuras para colas*/
struct Nodolistos{
	/* data */
	int actualPID;
	char fileName[15];
	int pc;
	char auxIR[15];
	int eax,ebx,ecx,edx;
	struct Nodolistos *siguiente;
	struct nodoHijo *hijo;
};
struct Nodolistos *iniciolistos;

struct Nodoterminados{
		/* data */
	int actualPID;
	char fileName[15];
	int pc;
	char auxIR[15];
	int eax,ebx,ecx,edx;
	struct Nodoterminados *siguiente;	
};
struct Nodoterminados *inicioterminados;

struct nodoHijo{
	/* data */
	int padre;
	int actualPID;
	char fileName[15];
	int pc;
	char auxIR[15];
	int eax,ebx,ecx,edx;
	struct nodoHijo *sig;
};
struct nodoHijo *inicioHijo;
/*+++++++++++++++++++++++++++++*/

/*++++FUNCIONES++++*/
void dispatcher();
int deletePID(int pid);
void ejecucion(char archi[],int pid);
void imprime();
void imprimelist();
void imprimePCB(char archi[],int pidpcb);
void imprimeTerm();
void inserterminados(char itarch[15],int PIDterminados);
void insertFinal(char *nameArchivo,int PID);
void insertHijoF(int father, int PIDhijo,char *nameArchivo);
int kbhit(void);
int moveFinal(int pidmover);
void MostrarArch();
void Pantalla();
int proceso(char archi[],int pidproceso);
void readArch();
void saveContext(char sCarch[],int pidsc);
int search(int pidsc);
int seelist();
void status(int pidsta,int edo);
void returnContext(int pidrc);

void dispatcher(){
	struct Nodolistos *temp;
	temp=iniciolistos;
	if(temp==NULL){
		mvprintw(25,108," 						     ");
		mvprintw(25,108,"Todo los procesos terminados");
		refresh();
	}else{
		ejecucion(temp->fileName,temp->actualPID);
	}
}

int deletePID(int pid){
	struct Nodolistos *temp1, *anterior;
	int encontrado=0;

	if(iniciolistos==NULL){
		return encontrado;
	}
	temp1=iniciolistos;
	anterior=NULL;

	while(temp1!=NULL && encontrado==0){
		if(temp1->actualPID==pid){
			encontrado=1;
			if(anterior==NULL){
				iniciolistos=temp1->siguiente;
			}else{
				anterior->siguiente=temp1->siguiente;
			}
			free(temp1);
		}else{
			anterior=temp1;
			temp1=temp1->siguiente;
		}
	}
	return encontrado;
}

void ejecucion(char archi[],int pidejec){
	int waytoend1=0;
	
	//1.-errores y fin de archivo, 2.- end, 3.- kill, 4.-quantum
	waytoend1=proceso(archi,pidejec);

	switch(waytoend1){
		case 1:
			deletePID(pidejec);
			imprimelist();
			imprimeTerm();
			break;
		case 2:
			inserterminados(archi,pidejec);
			deletePID(pidejec);
			imprimeTerm();
			imprimelist();
			break;
		case 3:
		break;
		case 4:
			imprimelist();
			moveFinal(pidejec);
			imprimelist();
		break;
	}
	refresh();
	dispatcher();
}

void imprime(int pc){
	
	mvprintw(12,1,"                                                                ");
	mvprintw(7,2,"%d",pc);
	mvprintw(7,8,"%s      ",auxIR);
	mvprintw(7,24,"%X     ",eax);
	mvprintw(7,32,"%X     ",ebx);
	mvprintw(7,40,"%X     ",ecx);
	mvprintw(7,48,"%X     ",edx);
	refresh();
	usleep(910000);
}

void imprimelist(){
	
	struct Nodolistos *temp9;
	temp9=iniciolistos;
	int cont=0;
	if(temp9==NULL){
		mvprintw(25,90,"                  ");
		mvprintw(25,90,"Lista vacia");refresh();
	}else{
		//mvprintw(24,90,"listos");
		while(temp9!=NULL){
			//mvprintw(25+cont,90,"                     ");
			mvprintw(25+cont,90,"PID: %d", temp9->actualPID);
			mvprintw(25+cont,100,"Arc: %s", temp9->fileName);
			refresh();
			cont++;
			//mvprintw(12+temp2->actualPID,85,temp2->nombreArchivo );
			temp9=temp9->siguiente;
		}
	}
	
}

void imprimePCB(char archi[],int pidpcb){
	struct Nodolistos *temp10;
	struct nodoHijo *temphijo;
	temp10=iniciolistos;
	int cont= 1;
	//refresh();mvprintw(13,15,"%s","ejec");refresh();
	if (temp10==NULL){
		//mvprintw(12+cont+1,3,"Lista vacia  imprimePCB Nodolistos                                   ");
	}else{
		
		while(temp10!=NULL){
			mvprintw(12+cont,3,"                                                              ");
			//mvprintw(12+cont,0,"                                                                      ");
			//refresh();
			//mvprintw(12+pidpcb,0,"                                                                                     ");
			mvprintw(12+cont,3,"%d",temp10->actualPID);
			mvprintw(12+cont,25,"%s",temp10->fileName);

			mvprintw(12+cont,32,"%s","list");
			mvprintw(12+cont,40,"%d",temp10->pc);
			mvprintw(12+cont,44,"%s",temp10->auxIR);
			mvprintw(12+cont,58,"%X",temp10->eax);
			mvprintw(12+cont,68,"%X",temp10->ebx);
			mvprintw(12+cont,78,"%X",temp10->ecx);
			mvprintw(12+cont,88,"%X",temp10->edx);
			refresh();
			//mvprintw(12+temp2->actualPID,85,temp2->nombreArchivo );
			if(temp10->hijo!=NULL){
				cont++;
				int conthijo=0;
				conthijo=cont;
				temphijo=inicioHijo;
				while(temphijo!=NULL){

						mvprintw(12+conthijo,3,"%d",temphijo->actualPID);
						mvprintw(12+conthijo,8,"%d",temphijo->padre);
						mvprintw(12+conthijo,25,"%s",temphijo->fileName);
						//int pc;
						//char auxIR[15];
						//int eax,ebx,ecx,edx;
						//struct nodoHijo *sig;
						conthijo++;
						temphijo=temphijo->sig;
				}
				cont=conthijo-1;
			}
			cont++;
			temp10=temp10->siguiente;
		}
	
	//struct Nodoterminados *temp2;
	//temp2=inicioterminados;

	/*if(temp2==NULL){
		mvprintw(25,75,"%s","Lista vacia");refresh();
	}else{
		while(temp2!=NULL){
			
			mvprintw(12,1,"                                                                ");
			refresh();mvprintw(12+temp2->actualPID,3,"%d",temp2->actualPID);
			//refresh();mvprintw(12+temp2->actualPID,8,"%s",archi);
			refresh();mvprintw(12+temp2->actualPID,15,"%s","term");
			refresh();mvprintw(12+temp2->actualPID,24,"%d",temp2->pc);
			refresh();mvprintw(12+temp2->actualPID,27,"%s",temp2->auxIR);
			refresh();mvprintw(12+temp2->actualPID,42,"%X",temp2->eax);
			refresh();mvprintw(12+temp2->actualPID,49,"%X",temp2->ebx);
			refresh();mvprintw(12+temp2->actualPID,56,"%X",temp2->ecx);
			refresh();mvprintw(12+temp2->actualPID,64,"%X",temp2->edx);
			
			refresh();mvprintw(24,75,"terminados");
			refresh();mvprintw(25+temp2->actualPID,75,"PID: %d", temp2->actualPID);refresh();
			//mvprintw(12+temp2->actualPID,85,temp2->nombreArchivo );

			temp2=temp2->siguiente;
		}

	}*/


	}
}

void imprimeTerm(){

	struct Nodoterminados *temp2;
	temp2=inicioterminados;
	int reng=0;

	if(temp2==NULL){
		mvprintw(25,75,"               ");
		mvprintw(25,75,"%s","Lista vacia");refresh();
	}else{
		mvprintw(25,75,"               ");
		while(temp2!=NULL){			
			//mvprintw(12,1,"                                                                ");
			/*refresh();mvprintw(12+temp2->actualPID,3,"%d",temp2->actualPID);
			//refresh();mvprintw(12+temp2->actualPID,8,"%s",archi);
			refresh();mvprintw(12+temp2->actualPID,15,"%s","term");
			refresh();mvprintw(12+temp2->actualPID,24,"%d",temp2->pc);
			refresh();mvprintw(12+temp2->actualPID,27,"%s",temp2->auxIR);
			refresh();mvprintw(12+temp2->actualPID,42,"%X",temp2->eax);
			refresh();mvprintw(12+temp2->actualPID,49,"%X",temp2->ebx);
			refresh();mvprintw(12+temp2->actualPID,56,"%X",temp2->ecx);
			refresh();mvprintw(12+temp2->actualPID,64,"%X",temp2->edx);*/
			
			//refresh();mvprintw(24,75,"terminados");
			mvprintw(25+reng,75,"PID: %d", temp2->actualPID);refresh();
			//mvprintw(12+temp2->actualPID,85,temp2->nombreArchivo );
			reng++;
			temp2=temp2->siguiente;
		}

	}
}



/*funcion que inserta al final de la lista*/
void insertFinal(char *nameArchivo,int PID){
	struct Nodolistos *temp3, *temp4;
	//Apartamos ram para nuevo nodo
	temp3 = (struct Nodolistos *) malloc(sizeof(struct Nodolistos));
	//llenamos campos con los valores recibidos
	temp3 ->actualPID = PID;
	strcpy(temp3->fileName,nameArchivo);
	temp3->siguiente = NULL;

	if (iniciolistos==NULL)//si es el primero, apunta a nuevo nodo
	{
		iniciolistos = temp3;
	}else{ //buscamos el ultimo nodo
		temp4=iniciolistos;
		while(temp4->siguiente != NULL){
			temp4=temp4->siguiente;
		}
		temp4->siguiente=temp3;
	}
}

void insertHijoF(int father, int PIDhijo,char *nameArchivo){
	struct Nodolistos *temp13;
	struct nodoHijo *temp11, *temp12;
	//Apartamos ram para nuevo nodo
	temp11 = (struct nodoHijo *) malloc(sizeof(struct nodoHijo));
	temp13=iniciolistos;
	//llenamos campos con los valores recibidos
	temp11->padre=father;
	temp11->actualPID = PIDhijo;
	strcpy(temp11->fileName,nameArchivo);
	temp11->sig = NULL;

	while(temp13!=NULL){
		if(temp13->actualPID==father){
			//temp13->hijo=inicioHijo;
			if (inicioHijo==NULL){//si es el primero, apunta a nuevo nodo
				inicioHijo = temp11;
			}else{ //buscamos el ultimo nodo
				temp12=inicioHijo;
				while(temp12->sig != NULL){
					temp12=temp12->sig;
				}
				temp12->sig=temp11;
			}
			temp13->hijo=inicioHijo;
		}
		temp13=temp13->siguiente;
	}

	
}



void inserterminados(char itarch[15],int PIDterminados){

	struct Nodoterminados *temp5, *temp6;
	//Apartamos ram para nuevo nodo
	temp5 = (struct Nodoterminados *) malloc(sizeof(struct Nodoterminados));
	//llenamos campos con los valores recibidos
	temp5->actualPID = PIDterminados;
	strcpy(temp5->fileName,itarch);
	strcpy(temp5->auxIR,auxIR);
		temp5->eax=eax;
		temp5->ebx=ebx;
		temp5->ecx=ecx;
		temp5->edx=edx;
	temp5->siguiente = NULL;

	if (inicioterminados==NULL)//si es el primero, apunta a nuevo nodo
	{
		inicioterminados = temp5;
	}else{ //buscamos el ultimo nodo
		temp6=inicioterminados;
		while(temp6->siguiente != NULL){
			temp6=temp6->siguiente;
		}
		temp6->siguiente=temp5;
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
	struct Nodolistos *temp7, *auxMover;//temp: recorre la lista, auxMover: se movera al final.
	int encontrado=0;
	temp7 = iniciolistos;
	auxMover=iniciolistos;

	if(temp7==NULL){
		return encontrado;
	}else{
		while(temp7!=NULL && encontrado==0){
			if (temp7->actualPID==pidmover && temp7->siguiente==NULL)
			{
				break;
			}
			temp7=temp7->siguiente;
			if(temp7->siguiente==NULL){
				encontrado=1;
				if(temp7->actualPID==pidmover){
					temp7=iniciolistos;
				}else{
					iniciolistos=auxMover->siguiente;
					temp7->siguiente=auxMover;
					auxMover->siguiente=NULL;	
				}
			}
		}
	}
return encontrado;
}


void MostrarArch(){
	struct Nodolistos *temp8;
	temp8=iniciolistos;

	while(temp8 != NULL){
		printf("PID: %d\n", temp8->actualPID);
		printf("Archivo: %s\n",temp8->fileName);
		printf("pc: %d\n",temp8->pc );
		printf("eax: %x\n",temp8->eax );
		printf("ebc: %x\n",temp8->ebx );
		printf("edx: %x\n",temp8->edx );
		printf("ecx: %x\n",temp8->ecx );

		temp8=temp8->siguiente;
	}
}

int proceso(char archi[],int pidproceso){

	returnContext(pidproceso);
	//1.-errores y fin de archivo, 2.- end, 3.- kill, 4.-quantum
	int waytoend=0;
	/*+++++*/
	int quantum = 0;
	FILE *archivo;
	char *instruccion;
	char *registroextra;
	char *registro;
	char *regconst;
	char *resto;
	char IR[15];
	char auxarchi[15];

	int operacion=0,operador1=0,operador2=0;
	int bandera1;
	int salir2=1;
	/*delimitadores*/
	char del[4]=", \n";
	char del2[2]="\n";
	int estado = 1;
	strcpy(auxarchi,archi);

	int countline=0;

	imprimelist();
	imprimeTerm();

	if((archivo = fopen(archi,"r"))!=NULL){
		while(!feof(archivo)){
			fgets(IR,15,archivo);

			if(countline>=pc){
				if(strcmp(IR,"\n")==0){printf("tienes un renglon vacio,pc = %d\n",pc);continue;}//verifica si hay un renglon vacio
				else{
					if(feof(archivo)){
						waytoend=1;
						break;
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
							//1.-errores y fin de archivo, 2.- end, 3.- kill, 4.-quantum
							saveContext(auxarchi,pidproceso);
							imprimePCB(archi,pidproceso);
							waytoend=2;
							//endwin();
							break;
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
											printf("tienes un error en la sintaxis, pc = %d\n",pc);
											//1.-errores y fin de archivo, 2.- end, 3.- kill, 4.-quantum
											waytoend=1;
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
											if(operador2<0){
												printf("\nel valor es menor que cero\n");
												//1.-errores y fin de archivo, 2.- end, 3.- kill, 4.-quantum
												waytoend=1;
												break;
											}	
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
									mvscanw(2,3,"%s",nombreArchivo);
									if(strcmp(nombreArchivo,"exit")==0){endwin();MostrarArch();exit(0);}

									if(strcmp(nombreArchivo,"kill")==0){

										char cad[50];
										int pidkill;
										getstr(cad);
										pidkill=atoi(cad);
										if(pidkill==pidproceso){
											inserterminados(auxarchi,pidkill);
											imprimeTerm();
											deletePID(pidkill);
											imprimelist();
											//1.-errores y fin de archivo, 2.- end, 3.- kill, 4.-quantum
											waytoend=3;
											break;
										}else{
											//copyNodo(pidkill);
											saveContext(auxarchi,pidproceso);
											imprimePCB(archi,pidproceso);
											inserterminados(auxarchi,pidkill);
											imprimeTerm();
											deletePID(pidkill);
											imprimelist();
											//1.-errores y fin de archivo, 2.- end, 3.- kill, 4.-quantum
											//waytoend=2;
											break;	
										}
										
									}else if(strcmp(nombreArchivo,"fork")==0){
										char p[5],l[5],ar[5];
										int padre,line;
										mvscanw(3,0,"%s %s %s",p,l,ar);
										padre=atoi(p);
										line=atoi(l);
										insertHijoF(padre,pid,ar);
										mvprintw(26,0,"%d %d %s",padre,line,ar);
										refresh();
									}else{
										 insertFinal(nombreArchivo,pid);
										 imprimelist();
									}
								}
							/*****/

						imprime(pc);
						imprimePCB(archi,pidproceso);
						if (quantum%3==0){
							waytoend=4;
							imprimelist();
							saveContext(auxarchi,pidproceso);
							imprimePCB(archi,pidproceso);
							imprimelist();
							//1.-errores y fin de archivo, 2.- end, 3.- kill, 4.-quantum
							break;
						}
						status(pidproceso,estado);
					}//fin else feof(archivo)

				}//fin else strcmp
			}
			countline++;
			
		}//fin while
	//fclose(archivo);
	//mvprintw(30,5,"%s cerrado",archi);
	}else{
		//getch();
		mvprintw(3,0,"error al abrir");
		switch(salir2){
			case 1:
				break;
		}
	}
	//moveFinal(pidproceso);
	fclose(archivo);
	return waytoend;	
}

void Pantalla(){
	char mesg[]="***Simulador de procesos***";
	//WINDOW *CPU;
	//PANEL *PCPU;

	//CPU=newwin(5,60,5,2);
	//box(CPU,0,0);
	//PCPU=new_panel(CPU);

	mvprintw(1,(COLS-strlen(mesg))/2,"%s",mesg);
	mvprintw(24,75,"terminados");
	mvprintw(24,90,"listos");
	mvprintw(24,108,"Avisos");

	if(has_colors() == FALSE){ 
    	endwin();
    	printf("Su terminal no permite usar colores!\n");
    	exit(1);
  	}
  	start_color();
  	init_pair(1, COLOR_CYAN, COLOR_BLACK);
  	attron(COLOR_PAIR(1));
	attron(A_BOLD);/* Activamos la negrita (bold) */         
	
	/*mvwprintw(CPU,1,1,"--------------------------CPU--------------------------");
	mvwprintw(CPU,2,1,"PC\tIR\t\tEAX\tEBX\tECX\tEDX");*/
	mvprintw(5,2,"------------------------CPU------------------------");
	mvprintw(6,2,"PC\tIR\t\tEAX\tEBX\tECX\tEDX\n");
	attroff(A_BOLD);
	attroff(COLOR_PAIR(1));
	/* Update the stacking order. 2nd panel will be on top */
	//update_panels();
	/* Show it on the screen */
	//doupdate();
	init_pair(2,COLOR_GREEN,COLOR_BLACK);
	attron(COLOR_PAIR(2));
	attron(A_BOLD);
	mvprintw(10,2,"-------------------------------------------PCB------------------------------------------");
	mvprintw(11,2,"PID\tPPID\tGPID\tfile\tstat\tPC\tIR\t  EAX\t    EBX\t      ECX\tEDX");
	attroff(A_BOLD);
	attroff(COLOR_PAIR(2));
	refresh();
}

void readArch(){
		refresh();	
		mvprintw(2,3,"$");
		mvscanw(2,5,"%s",nombreArchivo);
		insertFinal(nombreArchivo,pid);
		refresh();
		dispatcher();	
}

void saveContext(char sCarch[],int pidsc){
	//que guarde los datos y el pid del nodo leido
	struct Nodolistos *s;
	s=iniciolistos;
	while(s!=NULL){
		if(s->actualPID==pidsc){
			strcpy(s->fileName,sCarch);
			s->pc=pc;
			strcpy(s->auxIR,auxIR);
			s->eax=eax;
			s->ebx=ebx;
			s->ecx=ecx;
			s->edx=edx;
		}
	s=s->siguiente;
	}
	
}

int search(int pidsc){
	//que guarde los datos y el pid del nodo leido
	struct Nodolistos *see;
	see=iniciolistos;
	int encontre=0;
	if(see==NULL){
		return encontre;
	}else{
			while(see!=NULL && encontre==0){

			if(see->actualPID==pidsc){
				encontre=1;
			}
			see=see->siguiente;
		}	
	}
	return encontre;
}

int seelist(){

	struct Nodolistos *enc;
	enc=iniciolistos;
	int vacio=0;
	if(enc==NULL){
		return vacio;
	}else{
		vacio=1;
	}
	return vacio;
}

void status(int pidsta,int edo){
	if(edo==1){
		mvprintw(13,32,"%s","ejec");
		refresh();
	}
}

void returnContext(int pidrc){
	//que guarde los datos y el pid del nodo leido
	struct Nodolistos *revisa;//*regresa,
	//revisa = (struct Nodolistos *) malloc(sizeof(struct Nodolistos));
	//q = (struct Nodolistos *) malloc(sizeof(struct Nodolistos));
	revisa=iniciolistos;
	int encontrado2=0;
	//regresa=NULL;
	if(revisa==NULL){
		eax=0,ebx=0,ecx=0,edx=0,pc= 0;
	}else{
		while(revisa!=NULL && encontrado2==0){
			if(revisa->actualPID==pidrc){
				
				encontrado2=1;
				pc=revisa->pc;
				strcpy(auxIR,revisa->auxIR);
				eax=revisa->eax;
				ebx=revisa->ebx;
				ecx=revisa->ecx;
				edx=revisa->edx;
			}
			revisa=revisa->siguiente;
		}	
	}
}

/*++++FIN FUNCIONES++++*/

int  main(void){

	int enc1=0;
	//declaracion cabezera de listos
	iniciolistos=NULL;
	inicioterminados=NULL;

	FILE *archivitos;
	//stdscr();
	initscr();
	Pantalla();
	while(1){
		mvprintw(2,0,"$");
		mvscanw(2,3,"%s",nombreArchivo);

		if(strcmp(nombreArchivo,"exit")==0){
			endwin();
			exit(0);
		}

		if((archivitos = fopen(nombreArchivo,"r"))!=NULL){
			insertFinal(nombreArchivo,pid);
			//pid++;
		}else{
			mvprintw(25,108,"                                                ");
			mvprintw(25,108,"No existe archivo");
			refresh();
		}

		enc1=seelist();
		if(enc1==1){
			dispatcher();	
		}
		refresh();
	}
return 0;
}