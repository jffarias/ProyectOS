#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <curses.h>

int eax=0,ebx=0,ecx=0,edx=0,pc= 0;
char auxIR[15];	
int activo;
void imprime();
int kbhit(void);
int dispatcher();
char nombreArchivo[50];

struct pcb{
int pid;
int estado;
int pc;
int eax,ebx,ecx,edx;
}PCB;

struct listos
{
	/* data */
	int actual
	struct listos *siguiente;
};
struct listos *list;

struct ejecucion
{
	/* data */
};

struct terminados
{
	/* data */
};
void saveContext(){
PCB.pc =pc;
PCB.eax=eax;
PCB.ebx= ebx;
PCB.ecx= ecx;
PCB.edx= edx;

}

int dispatcher(){
	
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
	initscr();
	mvprintw(1,3,"$");

	mvprintw(3,2,"------------------------CPU------------------------");
	mvprintw(4,2,"PC\tIR\t\tEAX\tEBX\tECX\tEDX\n");


	mvprintw(10,2,"-----------------------PCB------------------------");
	mvprintw(11,2,"PID\tEstado\t\tPC\tIR\tEAX\tEBX\tEDX\tECX\n");






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
						printf("adios\n");
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
	//refresh();
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
	saveContext();
		pc= 0;
		mvscanw(1,5,"%s",nombreArchivo);
		if (strcmp(nombreArchivo,"salir")==0){
			mvprintw(12,3,"salir");
			exit(0);
		}
		proceso(nombreArchivo);	
		endwin();	
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
	//refresh();
	//proceso(cad);
	initscr();
	mvprintw(2,3,"$");
	mvscanw(2,5,"%s",nombreArchivo);
	refresh();//usleep(10000000);
		if(kbhit()){
			mvscanw(1,5,"%s",nombreArchivo);
			
		}
		proceso(nombreArchivo);
	endwin();

	//endwin();
return 0;
}
