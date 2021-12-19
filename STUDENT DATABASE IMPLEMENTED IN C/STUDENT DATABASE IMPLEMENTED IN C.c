#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "time.h"
#include "ctype.h"

#define NAME_NOT_FOUND -1

typedef struct{
	long unsigned int aem;
	char name[64];
	short unsigned int lessons;	
}student;

/*h sinartish katharizei to buffer*/
void clean_stdin(void){ 
	int c;
	
	do{
		c = getchar();
	}while(c!='\n' && c!=EOF);
}

/*exoume mia sinartish  h opoia metatrepei to onoma se kefalaia*/
void name_to_upps(char name[64]){
	int i=0;
	
	for(i=0;i<strlen(name);i++){
		name[i] = toupper(name[i]);
	}
	
}

/*sinartish pou psaxnei ta onomata, xrhsimopoieitai stis entoles pou xreiazetai kai epistrefei thn thesi toy onomatos an yparxei, enw epistrefei -1 an den yparxei to onoma*/
int find(student **data_base, int number_of_students, char find_student[64],int sorted_until,long unsigned int *comps){
	int i;
	
	name_to_upps(find_student);
	/*an oi mathites einai 0 tote den tha yparxei profanos kai to onoma*/
	if(number_of_students == 0){
		return(NAME_NOT_FOUND);
	}
	/*elegxei mexri ekei pou exoume kanei sort*/
	for(i=0;i<sorted_until;i++){ 
		(*comps)++;
		if(strcmp(data_base[i]->name, find_student) == 0){
			return(i);
		}
		if(strcmp(data_base[i]->name, find_student) > 0){
			break;
		}
	}
	if(sorted_until == 0){
		/*koitazei ta onomata se periptwsh pou den exei ginei sort*/
		for(i=0;i<number_of_students;i++){
			(*comps)++;
			if(strcmp(data_base[i]->name, find_student) == 0){
				return(i);
			}
		}
	}
	else {
		/*sigkrinei ta onomata pou einai unsorted*/
		for(i=sorted_until;i<number_of_students;i++){
			(*comps)++;
			if(strcmp(data_base[i]->name, find_student) == 0){
				return(i);
			}
		}
	}
	return(NAME_NOT_FOUND);
}

/*xrhsimopoioume auth thn sinarthsh gia thn leitourgeia ths add h opoia epistrefei ton deikth sthn arxh tou pinaka twn diktwn*/
student **add(student **data_base, int *number_of_students,int sorted_until,int *size, long unsigned int comps){
	char stdin_name[64];
	int i=0;
	long unsigned int aem=0;
	
	/*an iparxei xwros ston pinaka na prostethei allo onoma sinexizei kanonika thn diergasia xwris na kanei allocate kai allo memory*/
	if((*number_of_students) < (*size)){
		scanf(" %lu", &aem);
		for(i=0;i<(*number_of_students);i++){
			if(data_base[i]->aem == aem){
				printf("\nA-NOK %lu\n", aem); 
				clean_stdin();
				return(data_base);
			}
		}

		scanf(" %63s", stdin_name);
		name_to_upps(stdin_name);
		
		/*elegxei mesw ths find an iparxei to onoma hdh, h opoia an den iparxei epistrefei -1*/
		if(find(data_base,*number_of_students,stdin_name,sorted_until,&comps) < 0){
			data_base[*number_of_students]->aem = aem;
			strcpy(data_base[*number_of_students]->name,stdin_name);
			scanf(" %hu", &data_base[*number_of_students]->lessons);
			(*number_of_students)++;
			printf("\nA-OK\n");
			return(data_base);/*added*/
		}
		else {
			printf("\nA-NOK %s\n",stdin_name);
			clean_stdin();
			return(data_base);
		}
	}
	/*efoson den iparxei allos xwros na prostethoun alla stoixeia enos kainourgiou mathith, desmeuei epipleon xwro, elegxontas prwta na yparxei allh egrafei me idio aem h idio onoma*/
	else{
		scanf(" %lu", &aem);
		for(i=0;i<(*number_of_students);i++){
			if(data_base[i]->aem == aem){
				clean_stdin();
				printf("\nA-NOK %lu\n", aem); 
				return(data_base);
			}
		}
		scanf(" %63s", stdin_name);
		name_to_upps(stdin_name);
		
		if(find(data_base,*number_of_students,stdin_name,sorted_until, &comps) < 0){
			(*size)++;
			data_base =(student**) realloc(data_base,((*size))*sizeof(student*));
			data_base[*size-1] = (student*) malloc(sizeof(student));
			data_base[*number_of_students]->aem = aem;/*to size isoutai me ton arithmo twn mathitwn sin 1*/
			strcpy(data_base[*number_of_students]->name,stdin_name);
			scanf(" %hu", &data_base[*number_of_students]->lessons);
			(*number_of_students)++;
			printf("\nA-OK\n");
			return(data_base);/*added*/
		}
		else {
			clean_stdin();
			printf("\nA-NOK %s\n", stdin_name);
			return(data_base);
		}
	}
}

/*h sinarthsh h opoia afairei enan mathith apo ton pinaka deiktwn */
student **remove_student(student **data_base, int *number_of_students,int *sorted_until,int *size,long unsigned int comps){
	char name_to_be_removed[64];
	int name_to_be_removed_pos=0;
	
	scanf(" %63s", name_to_be_removed);
	/*elegxei pali mesw ths find an iparxei to onoma epistrefontas thn timh -1 an den iparxei enw epistrefei thn thesi tou an yparxei*/
	name_to_be_removed_pos = find(data_base, *number_of_students, name_to_be_removed, *sorted_until, &comps);
	
	if(name_to_be_removed_pos < 0){
		clean_stdin();
		printf("\nR-NOK %s\n", name_to_be_removed);
		return(data_base);/*not removed*/
	}
	/*metaferei ton teleutaio mathith sth thesi tou mathith pou afairethike kai kanei realloc meiwnontas thn mnhmh*/
	else {
		(*number_of_students)--;
		data_base[name_to_be_removed_pos] = data_base[(*number_of_students)]; 
		(*size) = (*number_of_students);/*to size tou pinaka ousiastika tha ginei oso einai o neos arithmos mathitwn*/
		data_base = (student**) realloc(data_base, (*number_of_students)*sizeof(student));
	
		/*an exei klithei h synartish sort kai an to onoma poy egine remove einai ena apo ta sorted , tote me tin taktiki poy akoloythisame , oi mathites poy tha parameinoyn sorted tha einai apo tin arxh mexri ena mathiti prin apo auton pou egine remove*/
		if(((*sorted_until) != 0) && (name_to_be_removed_pos <= (*sorted_until))){
			*sorted_until = name_to_be_removed_pos - 1;
		}
		printf("\nR-OK\n");
		return(data_base);/*removed*/
	}
}

/*ektipwnei ta stoixeia twn mathitwn*/
void print(int number_of_students,student **data_base){
	int i;
	
	printf("\n#\n");
	for(i=0;i<number_of_students;i++){
		printf("%lu %s %hu\n", data_base[i]->aem, data_base[i]->name, data_base[i]->lessons);
	}
}

/*einai h sinarthsh pou vriskei ton mathith mesw ths find*/
void find_case(student **data_base, int number_of_students, int current_sorted_pos){
	char find_student[64];
	int find_res=0;
	long unsigned int comps=0;
	clock_t start=0,end=0,difference=0;

	start = clock();
	scanf(" %63s", find_student);
	/*mesw ths find vriskei thn thesi toy onomatos efoson yparxei*/
	find_res = find(data_base, number_of_students,find_student,current_sorted_pos,&comps);
	if(find_res >= 0){
		printf("\n#\nF-OK %lu %hu\n", data_base[find_res]->aem, data_base[find_res]->lessons);
	}
	else{
		printf("\n#\nF-NOK %s\n", find_student);
	}
	end = clock();
	difference = (end - start);
	fprintf(stderr, "\n# %lu,%ju\n", comps,difference);
}

/*allazei ton arithmo twn xrwsotumenwn mathimatwn*/
void mod(student **data_base,int number_of_students,int sorted_until,long unsigned int comps){
	char name[64];
	short unsigned int courses=0;
	int pos=0;
	
	scanf(" %63s", name);
	name_to_upps(name);
	/*mesw tis synartishs sort vriskei thn thesi poy einai to onoma , apo to opoio theloyme na allaksoyme ton arithmo ton xrwsotumenwn mathimatwn*/
	pos = find(data_base, number_of_students, name, sorted_until, &comps);
	if(pos >= 0){
		scanf(" %hu", &courses);
		data_base[pos]->lessons = courses;
		printf("\nM-OK\n");
	}
	else {
		clean_stdin();
		printf("\nM-NOK %s\n", name);
	}
}

int insertionSort(student *data_base[], int number_of_students){
	int i, j;
	long unsigned int comps=0;
	student *elem;
	clock_t end=0, start=0,difference=0;

	start = clock();
	
	for(i=1;i<number_of_students;i++){
		
		elem = data_base[i];
		j=i-1;
		/*krata ena stoixeio , kai elexei ta aristera tou*/
		while(j>=0 && memcmp(data_base[j]->name, elem->name ,sizeof(student))>0){
			comps++;
			data_base[j+1] = data_base[j];
			j--;
		}
		/*an vgei apo thn while kai to j einai >= 0 tote simainei oti egine mia parapanw sigkrish */
		if(j>=0){
			comps++;
		}
		data_base[j+1] = elem;
	}
	end = clock();
	difference =(end - start);
	printf("\nS-OK\n");
	fprintf(stderr, "\n# %lu,%ju\n", comps, difference);
	return(number_of_students);
}
		
void clear(int *number_of_students, student **data_base,int *size,int *sorted_until){
	int i=0;
	
	for(i=0;i<(*number_of_students);i++){
		free(data_base[i]);
	}

	(*size) = 0;
	(*number_of_students) = 0;
	(*sorted_until) = 0;
}
int main (int argc, char *argv[]){
	int size=0,i,number_of_students=0,current_sorted_pos=0;
	long unsigned int comparisons=0;
	char option;
	student **data_base=NULL;
	
	/*elegxei an elave ena orisma*/
	if(argc == 2 ){
		size = atoi(argv[1]);
	}
	else {
		size = 0;		
	}
	
 	data_base =(student**) malloc(size*sizeof(student*));
	
	if(data_base == NULL){
		printf("\nMalloc Error\n");
		return(0);
	}
	
	for(i=0;i<size;i++){
		data_base[i] = (student*) malloc(sizeof(student));
	}
	
	while(1){
		scanf(" %c", &option);
		switch(option){
			case('a') : {
				data_base =(student**) add(data_base,&number_of_students, current_sorted_pos, &size, comparisons);
				break;
			}
			case('r') : {
				data_base = (student**)remove_student(data_base,&number_of_students, &current_sorted_pos, &size, comparisons);
				break;
			}
			case('m') : {
				mod(data_base, number_of_students, current_sorted_pos, comparisons);
				break;
			}
			case ('s') : {
				current_sorted_pos = insertionSort(data_base, number_of_students);
				break;
			}
			case ('f') : {
				find_case(data_base, number_of_students, current_sorted_pos);
				break;
			}
			case('p') : {
				print(number_of_students, data_base);
				break;
			}
			case('c') : {
				clear(&number_of_students, data_base, &size, &current_sorted_pos);
				printf("\nC-OK\n");
				break;
			}
			case('q'): {
				clear(&number_of_students, data_base, &size, &current_sorted_pos);
				return(0);
			}
			default : {
				return(0);
			}
		}
	}
	return(0);
}
