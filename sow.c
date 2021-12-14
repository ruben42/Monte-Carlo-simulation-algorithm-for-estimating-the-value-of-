#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <math.h>

//===============( Simulação de monte carlo para a estimativa de π)==============
#define PI 3.141592654
int Points[4] = {20000, 100000, 1000000, 10000000}, N_Threads[4] = {2, 4, 6, 8};
double PI_value = 0, P_IN = 0, P_out = 0;
clock_t start_time; 

//Gera coordenadas para os pontos x e y.
double __generates_Points()
{

    double aux = 100.0*((double)(rand())/RAND_MAX);
    aux = aux - (int)(aux);
	return aux;

}

//Calcula o valor aproximado de pi.
void *cal_PI(void*arg)
{
    int *value = (int*)(arg);
    int z;
    double x = 0, y = 0;
    for(z=0; z < *value; z++)
    {
		x = __generates_Points(); 
		y = __generates_Points();
		if((x*x + y*y) <= 1) 
			P_IN ++; 

		else
			P_out ++;	  				
	
//printf("P(%d) x=%.15f  y=%.15f \n",z,x,y);  		
	}		
return 0;
}


int main()
{
    double error;
    int j, h,  i, N_Points;
    pthread_t thread[8];
  
    for (j = 0; j < sizeof(*Points); j++)
    {
        for (h = 0; h< sizeof(*N_Threads); h++)
        {
            P_IN = 0;
            for (i = 0; i < N_Threads[h]; i++ )
            {
                N_Points = Points[j] / N_Threads[h];

                start_time = clock();
                pthread_create(&thread[h], NULL, cal_PI, (void *) (&N_Points));
                pthread_join(thread[h], NULL);
                start_time = clock() - start_time;

            }

            PI_value = 4.0*(((double)P_IN)/((double)Points[j]));
            error = PI - PI_value;
            float _time = (float)start_time/CLOCKS_PER_SEC;
  
            printf("[O valor de pi para %d pontos em %d Threads]: %lf.\n[Tempo de execução]: %fs \n[Qualidade da Estimativa]: %f \n\n", Points[j], N_Threads[h], PI_value, _time, error);
    
            printf("================================================================\n");
        }
        printf("\n");  
    }

    printf("\n");
}
