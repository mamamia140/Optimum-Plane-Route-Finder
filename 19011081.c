#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

typedef struct flight{// uçuþ bilgilerini tuttuðum struct
    int time,price;
    char destinationCityName[20];
    struct flight* next;
}FLIGHT;

typedef struct city{ // þehirleri tuttuðum struct
	char cityName[20];
	int visited; // 1 --> visited, 0 --> unvisited
}CITY;

typedef struct graph{ // graph struct'ý
    FLIGHT** nodes;
}GRAPH;

typedef struct result{ // uçuþ sonuçlarýný listelemek için kullandýðým struct
	CITY* source;
	CITY* desination;
	CITY** stops;
	int time;
	int price;
}RESULTS;

void merge(RESULTS* Array,int low,int med,int high,char* choice){ // merge sort fonksiyonunun merge iþlemi için yazdýðým fonksiyon
	RESULTS* temp=(RESULTS*)malloc((high - low + 1)* sizeof(RESULTS));
	int i = low, j = med+1, k = 0;
    if(strcmp(choice,"price")==0){
    	while(i <= med && j <= high) {
			if(Array[i].price <= Array[j].price) {
				temp[k] = Array[i];
				k += 1; i += 1;
			}
			else {
				temp[k] = Array[j];
				k += 1; j += 1;
			}
		}
		
		while(i <= med) {
			temp[k] = Array[i];
			k += 1; i += 1;
		}
	
		while(j <= high) {
			temp[k] = Array[j];
			k += 1; j += 1;
		}
		
		for(i = low; i <= high; i += 1) {
			Array[i] = temp[i - low];
		}
	}
	else{

		while(i <= med && j <= high) {
			if(Array[i].time <= Array[j].time) {
				temp[k] = Array[i];
				k += 1; i += 1;
			}
			else {
				temp[k] = Array[j];
				k += 1; j += 1;
			}
		}
		 
		while(i <= med) {
			temp[k] = Array[i];
			k += 1; i += 1;
		}

		while(j <= high) {
			temp[k] = Array[j];
			k += 1; j += 1;
		}

		for(i = low; i <= high; i += 1) {
			Array[i] = temp[i - low];
		}
	}
	
}

void mergeSort(RESULTS* Array,int low, int high,char* choice){ //merge sort algoritmasýný kullanan fonksiyonum
	int med;
    if(low < high){
        med = (low + high)/2;
        mergeSort(Array,low,med,choice);
        mergeSort(Array,med+1,high,choice);
        merge(Array,low,med,high,choice);
    }
    
}

bool isAdded(CITY* cities,char* cityName,int numberOfCities){
    int i=0;
    while(strcmp(cities[i].cityName,cityName)!=0 && i<numberOfCities){
        i++;
    }
    if(strcmp(cities[i].cityName,cityName)==0){
        return true;
    }
    return false;
}

int findCityNumber(CITY* cities,char* cityName,int numberOfCities){
    int i=0;
    while(strcmp(cities[i].cityName,cityName) !=0 && i<numberOfCities){
        i++;
    }
    if(strcmp(cities[i].cityName,cityName) ==0){
        return i;
    }
    else{
        printf("city couldn't be found");
        return -1;
    }
}
bool isVisited(CITY* cities,char* cityName,int numberOfCities){
    int num;
    num = findCityNumber(cities,cityName,numberOfCities);   

    if(cities[num].visited == 1){
		return true;
    }
    else if(cities[num].visited == 0){
        return false;
    }
    else{
    	printf("bir hata var");
    	return false;
	}
    
}
void printFlight(GRAPH* head,CITY* cities,int numberOfCities){
    FLIGHT* temp;
    int i;
    for(i=0;i<numberOfCities;i++){
        temp = head->nodes[i];
        printf("%s--->",cities[i].cityName);
        while(temp != NULL){
            printf("%s--->",temp->destinationCityName);
            temp = temp->next;
        }
        printf("\n");
    }
    printf("\n");
}
FLIGHT* createNewFlight(char* destination,int hour,int minute,int price){
	FLIGHT* newNode = (FLIGHT*)malloc(sizeof(FLIGHT));
    newNode->time = hour*60 + minute;
    newNode->price = price;
    strcpy(newNode->destinationCityName,destination);
    newNode->next = NULL;
    return newNode;
}

void addFlight(FLIGHT* newNode,GRAPH* head,CITY* cities,char* source,int numberOfCities){
	int cityNo;
	FLIGHT* temp;
    cityNo = findCityNumber(cities,source,numberOfCities);
    if(head->nodes[cityNo] == NULL){
        head->nodes[cityNo] = newNode;
    }
	else{
		temp = (head->nodes[cityNo]);
        while(temp->next !=NULL){
            temp=temp->next;
        }
        temp->next = newNode;
	}

}

void depthFirstSearch(GRAPH* head,char* source,char* destination,CITY* cities,int numberOfCities,int* path,int* pathIndex,int* cost,int* minute,RESULTS* results,int* resultsIndex){
	int num = findCityNumber(cities,source,numberOfCities);
    int i=0,j=0,counter=0;
    FLIGHT* temp;
    cities[num].visited = 1;
    path[*pathIndex] = num;
    *pathIndex = *pathIndex + 1;
    
    if(strcmp(source,destination)==0){
    	results[*resultsIndex].source = &(cities[path[0]]);
    	results[*resultsIndex].desination = &(cities[path[*pathIndex - 1]]);
        for(i=1;i<(*pathIndex) - 1;i++){
			results[*resultsIndex].stops[i-1] = &(cities[path[i]]);
			counter++;
        }
        results[*resultsIndex].price = *cost;
        results[*resultsIndex].time = *minute + counter * 60;
        *resultsIndex = *resultsIndex + 1;
    }
    else{
        temp = head->nodes[num];
        while(temp != NULL){
            if(!isVisited(cities,temp->destinationCityName,numberOfCities)){
            	*cost = *cost + temp->price;
            	*minute = *minute + temp->time;
                depthFirstSearch(head,temp->destinationCityName,destination,cities,numberOfCities,path,pathIndex,cost,minute,results,resultsIndex);
                *cost = *cost - temp->price;
            	*minute = *minute - temp->time;
            }
            temp = temp->next;
        }
    }
    *pathIndex = *pathIndex - 1;
    cities[num].visited = 0;
    
}
void sortResults(RESULTS* results,char* choice,int resultIndex){
	
	if(strcmp(choice,"price")==0){
		mergeSort(results,0,resultIndex-1,choice);
	}
	else if(strcmp(choice,"time")==0){
		mergeSort(results,0,resultIndex-1,choice);
	}
	else{
		printf("wrong input");
	}
}
bool isThereAFlight(RESULTS* results,int resultIndex,int transfer){
	int i,count;
	for(i=0;i<resultIndex;i++){
		count=0;
		while(results[i].stops[count] != NULL){
			count++;	
		}
		if(count <= transfer){
			return true;
		}
	}
	return false;
}
void printResults(RESULTS* results,int resultIndex,int transfer){
	int i,j,count;
	printf("\n");
	for(i=0;i<resultIndex;i++){
		count=0;
		j=0;
		
		while(results[i].stops[count] != NULL){
			count++;
		}
		if(count <= transfer){
			printf("source: %s  destination: %s stops: ",results[i].source->cityName,results[i].desination->cityName);
			for(j=0;j<count;j++){
				printf("%s,",results[i].stops[j]->cityName);
			}
			printf(" hour: %d  minute: %d  price: %d\n",(results[i].time)/60,(results[i].time)%60,results[i].price);
		}
	}
}
void reset(int* pathIndex,int* price,int* minute,RESULTS* results,int* resultIndex,int numberOfCities){
	*pathIndex = 0;
	*price = 0;
	*minute = 0;
	int i,j;
	for(i=0;i<*resultIndex;i++){
    	for(j=0;j<numberOfCities;j++){
    		results[i].stops[j] = NULL;
		}
	}
	*resultIndex=0;
}

void writeResults(RESULTS* results,int resultIndex,int transfer){
	int i,j,count;
	FILE* file = fopen("results.txt","w");
	fprintf(file,"%s","source // destination // stops // hour // minute // price\n");
	for(i=0;i<resultIndex;i++){
		count=0;
		j=0;
		
		while(results[i].stops[count] != NULL){
			count++;
		}
		
		if(count <= transfer){
			fprintf(file,"%s // %s //",results[i].source->cityName,results[i].desination->cityName);
			
			for(j=0;j<count;j++){
				fprintf(file,"%s,",results[i].stops[j]->cityName);
			}
			fprintf(file,"// %d // %d // %d\n",(results[i].time)/60,(results[i].time)%60,results[i].price);
		}
	}
	fclose(file);
	
}

int main(){
    int hour,minute,price,i,j,numberOfCities=0,legcount,caseNum,pathIndex=0,resultIndex=0,leg;
    int* path = (int*)malloc(numberOfCities*sizeof(int));
	char* yesOrNo = (char*)malloc(3*sizeof(char)); 
	char* choice = (char*)malloc(5*sizeof(char));
    char* source = (char*)malloc(30 * sizeof(char));
    char* destination = (char*)malloc(30 * sizeof(char));
    GRAPH head;
    RESULTS* results = (RESULTS*)malloc(10 * sizeof(RESULTS));
    CITY* cities = (CITY*)malloc(50 * sizeof(CITY));
    FILE* file = fopen("sample.txt","r");

    while(fscanf(file,"%s %s %*d %*d %*d",source,destination) != EOF){
        if(!isAdded(cities,source,numberOfCities)){
            strcpy(cities[numberOfCities].cityName,source);
			cities[numberOfCities].visited=0; 
            numberOfCities++;
        }
        if(!isAdded(cities,destination,numberOfCities)){
            strcpy(cities[numberOfCities].cityName,destination);
            cities[numberOfCities].visited=0;
            numberOfCities++;
        }

    }
    for(i=0;i<10;i++){
    	results[i].stops = (CITY**)malloc(numberOfCities * sizeof(CITY*));
    	for(j=0;j<numberOfCities;j++){
    		results[i].stops[j] = NULL;
		}
	}
	
    head.nodes = (FLIGHT**)malloc(numberOfCities * sizeof(FLIGHT*));
    
    for(i=0;i<numberOfCities;i++){
        head.nodes[i] = NULL;
    }

    rewind(file);
	
    while(fscanf(file,"%s %s %d %d %d",source,destination,&hour,&minute,&price) != EOF){
        addFlight(createNewFlight(destination,hour,minute,price),&head,cities,source,numberOfCities);
        addFlight(createNewFlight(source,hour,minute,price),&head,cities,destination,numberOfCities);

    }
    fclose(file); 
    
    price=0;
    minute=0;
	while(1){
    	printf("\nwhat do you want to do?\n");
        printf("1-search for a flight\n");
        printf("2-quit\n");
        scanf("%d",&caseNum);

        switch (caseNum){
            case 1:
                printf("from?\n");
	 	        scanf("%s",source);
	 	        printf("to?\n");
	 	        scanf("%s",destination);
                printf("maximum number of transfers?\n");
                scanf("%d",&leg);
                printf("minimum price or minimum time?(type price or time)\n");
                scanf("%s",choice);
                depthFirstSearch(&head,source,destination,cities,numberOfCities,path,&pathIndex,&price,&minute,results,&resultIndex);
            	if(isThereAFlight(results,resultIndex,leg)){
            		sortResults(results,choice,resultIndex);
                	printResults(results,resultIndex,leg);
                	
                	printf("\ndo you want to export it?(y/n)\n");
                	scanf("%s",yesOrNo);
                	if(strcmp(yesOrNo,"y") == 0){
                		writeResults(results,resultIndex,leg);
					}	
				}
				else{
					printf("there is no flight with these conditions");
				}
				reset(&pathIndex,&price,&minute,results,&resultIndex,numberOfCities);
                break;
            case 2:
                break;
        }
        if(caseNum == 2){
            break;
        }
	}
		
    return 0;
}
