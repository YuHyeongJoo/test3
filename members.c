#include <stdio.h>
#include<string.h>
#include <stdbool.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/wait.h>
#include<stdlib.h>

#include<signal.h>
typedef struct route_and_distance{
	int distance;
	int route[50];
	char best_route_uptonow[200]; 
}rad;
rad ans = {2000000008};
int n=0,process_count =1, process_limit, parent_pid;;
unsigned long long route_count =0;
int dist[50][50],fd[2];
bool visited[50] ={true,};
int length(rad arr, int k);
rad min(rad a, rad b);
rad pre_clear(rad arr, int k);
rad pre_add(rad arr, int k, int add_num);
rad chld(rad prefix);
int back(rad arr,int k);
void parent(int k,rad arr);
void handler(int sig);
void sigint_handler(int sig);
rad init_arr(rad arr);
int main(int argc, char *argv[] ){
	parent_pid = getpid();
	signal(SIGCHLD,handler);
	signal(SIGINT,sigint_handler);
	rad pref;
	pref = init_arr(pref);
	pref.distance= 0;
	pipe(fd);
	FILE *fp = fopen(argv[1],"r");
	char strTemp[255];
	fgets(strTemp,sizeof(strTemp),fp);
	char *ptr = strtok(strTemp, " ");
	while(ptr!=NULL){
		ptr = strtok(NULL, " ");
		n++;
	}
	
	rewind(fp);

	for(int i=0; i<n; i++){
		 for(int j=0; j<n; j++){
      			fscanf(fp,"%d", &dist[i][j]);
  		 }
	 }
	process_limit = atoi(argv[2]);


	ans=init_arr(ans);
	parent(n-12,pref);
	while( (wait(NULL)) > 0 );
	printf("\nShortest distance: %d  The number of route: %lld   \nShortest route: %s \n",ans.distance,route_count*2*3*4*5*6*7*8*9*10*11*12 ,ans.best_route_uptonow);

}
char* int_to_string(rad arr){
	char *result;
	result = malloc(200);
	for(int i=0; i<length(arr,n);i++){
		char tmp[5];
		int num = arr.route[i];
		sprintf(tmp, "%d", num);
		strcat(result,tmp);
		strcat(result, "->");
	}	
	return result;
}
int length(rad arr, int k){
	int c=1;
	for(int i=1; i<k; i++){
		if(arr.route[i]!=0) c++;
		else break;
	}
	return c;
}
rad init_arr(rad arr){
	for(int i=0; i<50; i++){
		arr.route[i]=0;
	}
	return arr;
}
rad pre_add(rad arr, int k, int add_num){
	for(int i=1; i<k; i++){
		if(arr.route[i]==0){
			arr.route[i] = add_num;
			break;
		}
	}
	return arr;
}
rad pre_clear(rad arr, int k){
	arr.route[length(arr,n)-1] = 0;
	return arr;
}
rad min(rad a, rad b){
	if(a.distance<b.distance) return a;
	else return b;
}
rad chld(rad prefix){
	rad ret;
	ret.distance=1000000007;
	if(length(prefix,n)== n){
		prefix.distance +=dist[back(prefix,n)][0];
		return prefix;
	}
	for(int i=0; i<n; i++){
		if(visited[i]==true) continue;
		int here = back(prefix,n);
		if(dist[here][i]==0) continue ;
		visited[i]=true;
		prefix = pre_add(prefix,n,i);
		prefix.distance +=dist[here][i];
		ret = min(chld(prefix),ret);

		visited[i] = false;
		prefix.distance -= dist[here][i];
		prefix = pre_clear(prefix,n);
	}
	return ret;
}
int back(rad arr,int k){
	int tmp = arr.route[0];
	for(int i=1; i<k; i++){

		if(arr.route[i]==0) break;
		tmp = arr.route[i];
	}
	return tmp;
}
void parent(int k, rad arr){
	int pid;
	if(length(arr,n)==k){
		route_count++;
	
		while(process_count>process_limit){
                        };
                        pid = fork();
			route_count++;
                        process_count++;
		if(pid ==0){
			printf("A child process is executed\n");	
			rad tmp = chld(arr);
			int current_distance = tmp.distance;
			write(fd[1],&current_distance,sizeof(int));
			char route_string[200];
		 	strcpy(route_string,int_to_string(tmp));
              	  	write(fd[1], route_string,200);
			exit(0) ;	
		}
	}
	else{	
		for(int i=0; i<n; i++){
	    		if(visited[i]==true) continue;
			int here = back(arr,n);
			if(dist[here][i]==0) continue ;
	    		visited[i] = true;
	    		arr= pre_add(arr,n,i);
			arr.distance+=dist[here][i];
			parent(k,arr);
		 	visited[i] = false;
			arr.distance -= dist[here][i];
			arr =pre_clear(arr,n);
	  }
	}

}

void handler (int sig){
	
		int tmp2;
		char best_route[200];
		process_count--;
		read(fd[0], &tmp2, sizeof(int));
		read(fd[0], best_route, 200);
		printf("A Child Process is Dead. It returned valuses 1. Distance:%d  2. Path:  %s \n",tmp2,best_route);
		if(ans.distance>tmp2){	
			printf("\nThe shortest route was updated  1.  Distanbe: %d   2. Path: %s\n\n",tmp2,best_route);
			ans.distance = tmp2;
			strcpy(ans.best_route_uptonow,best_route);
		}
		
	
}
void sigint_handler(int sig){
	if(getpid()==parent_pid){
		if(ans.distance == 2000000008)
			printf("No child processes have been terminated. So there is no path found\n");
		printf("\nShortest distance upto now: %d  The number of route upto now : %lld  \nShortest route upto now: %s\n",ans.distance,route_count*2*3*4*5*6*7*8*9*10*11*12 ,ans.best_route_uptonow);}
	exit(0);
}
