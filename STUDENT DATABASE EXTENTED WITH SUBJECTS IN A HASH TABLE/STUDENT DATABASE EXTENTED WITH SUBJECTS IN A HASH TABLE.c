#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "ctype.h"
#include "time.h"

#define LESSON_NOT_FOUND -1

/*h sinarthsh auth katharizei to buffer*/
void clean_stdin(void){ 
	int c;
	
	do{
		c = getchar();
	}while(c!='\n' && c!=EOF);
}

unsigned long hash(char *str){
	
	unsigned long hash = 5381;
    int c;

    while((c = *str++))
        hash = ((hash << 5) + hash) + c;

    return (hash);
}

typedef struct lesson lesson;
/*struct to opoio exei thn lista twn mathimatwn*/
struct lesson{
	unsigned short lesson_code;
	lesson *next_lesson;
};

typedef struct student student;

/*struct tou mathith*/
struct student{
	long unsigned int aem;
	char name[64];
	short unsigned int lessons;	
	lesson *root;
	student *next;
	student *prev;
	unsigned long int bucket_size;
	unsigned long int comps;/*thn metavliti ayth thn xrisimopoioyme sthn find gia na vroyme poses sygkriseis */
};

/*struct pou exei oles tis metavlhtes pou tha xreiastoume sto programma*/
typedef struct{
	student **student_base;
	unsigned long int table_size;
	unsigned long int number_of_students;
}data_base;


/*metatrepei to onoma se kefalaia*/
void name_to_upps(char name[64]){
	int i=0;
	
	for(i=0;i<strlen(name);i++){
		name[i] = toupper(name[i]);
	}
	
}
/*arxikopoiei thn lista mathimatwn */
lesson *list_lessons_init(lesson *root){
	
	root = (lesson *) malloc(sizeof(lesson));
	root->next_lesson = NULL;
	return(root);
}

/*h sinarthsh psaxnei ena mathima , epistrefontas NULL an to vrei, enw epistrefei thn swsth thesi pou prepei na prostethei an den to vrei*/
lesson *find_subject(lesson *root, unsigned short subject_code){
	lesson *curr,*prev;
	
	prev = root;
	curr=root->next_lesson; 
	while(1){
		if(curr == NULL){/*adios pinakas*/
			break;
		}
		if(curr->next_lesson == NULL){/*teleytaio stoixeio*/
			if(subject_code > curr->lesson_code){
				prev = curr;
			}
			break;
		} 
		if(curr->lesson_code == subject_code){
			return(curr);
		}
		if(curr->lesson_code > subject_code){
			break;
		}
		prev = curr;
		curr=curr->next_lesson;
	}
	return(prev);
}
/*eisagei ta mathimata*/
void list_lessons_insert(lesson *root, unsigned short subject_code){
	lesson *prev_pos,*new_lesson;
	
	prev_pos = find_subject(root,subject_code);
	/*elegxei an yparxei to mathima*/
	if(prev_pos->lesson_code != subject_code){
		new_lesson = (lesson *) malloc(sizeof(lesson));
		new_lesson->lesson_code = subject_code;
		new_lesson->next_lesson = prev_pos->next_lesson;
		prev_pos->next_lesson = new_lesson;
	}
	else {
		printf("\nG-NOK %hu\n", root->lesson_code);
	}
}

/*afairh mathimatos*/
void list_lessons_remove(lesson *root, unsigned short code_to_be_removed){
	lesson *curr,*prev;
	/*mia diaforetikh ylopoihsh ths find gia ta mathimata h opoia omws epistrefei to parwn mathima pou theloyme na diagrapsoume kratwntas kai to proigoumeno*/
	for(prev=NULL,curr=root->next_lesson;(curr->next_lesson !=NULL && curr->lesson_code !=code_to_be_removed);prev=curr,curr=curr->next_lesson);
	
	if(curr != NULL && curr->lesson_code == code_to_be_removed){
		if(curr == root->next_lesson){
			root->next_lesson = curr->next_lesson;
		}
		else {
			prev->next_lesson = curr->next_lesson;
		}
		free(curr);
	}
}

/*elenxei an iparxei to mathima sth lista*/
int is_lesson_on_list(lesson *root, unsigned short lesson_code_to_check){
	lesson *curr;
	
	curr = root->next_lesson;
	while(1){
		if(curr != NULL){
			if(curr->lesson_code == lesson_code_to_check){
				return(1);
			}
			if(curr->lesson_code > lesson_code_to_check){
				return(LESSON_NOT_FOUND);
			}
			curr = curr->next_lesson;
		}
		else {
			return(LESSON_NOT_FOUND);
		}
	}
}

/*katharizei th lista mathimatwn*/
void list_lessons_destroy(lesson *root){
	lesson *curr,*prev;
	/*vriskoyme kathe fora to teleutaio stoixeio kai to kanoume free*/
	prev = root;
	curr = root->next_lesson;
	while(curr != NULL){
		if(curr->next_lesson == NULL){
			prev->next_lesson = NULL;
			free(curr);
			prev = root;
			curr = root->next_lesson;
		}
		else {
			prev = curr;
			curr = curr->next_lesson;
		}
	}
}

/*ektipwnei ta mathimata*/
void list_lessons_print(lesson *root){
	lesson *curr;
	
	curr = root->next_lesson;
	printf("\n");
	while(curr != NULL){
		printf("%hu\n", curr->lesson_code);
		curr = curr->next_lesson;
	}
}

/*arxikopoihsh ths listas twn mathitwn*/
student *list_student_init(student *head){
	head = (student *)malloc(sizeof(student));
	head->next = head;
	head->prev = head;
	head->root = (lesson *)malloc(sizeof(lesson));
	head->bucket_size = 0;
	return(head);
}

/*psaxnei na vrei to mathiti epistrefontas to proigoumeno*/
student *find(student *head, char name[64]){
	student *curr;

	strcpy(head->name, name);
	if(head->next == head){
		return(head);
	}
	else {
		for(curr=head->next;strcmp(name,curr->name)>0;curr=curr->next);
		curr = curr->prev;
	}
	return(curr);
}
/*psaxnei na vrei to mathiti epistrefontas to paron*/
student *find_curr(student *head, char name[64]){
	student *curr;
	
	head->comps = 0;
	strcpy(head->name, name);
	if(head->next == head){
		return(head);
	}
	else {
		for(curr=head->next;strcmp(name,curr->name)>0;curr=curr->next){
			head->comps++;
		}
		head->comps++;
		//curr = curr->prev;
	}
	return(curr);
}
/*synartisi prosthikhs poy xrisimopoieite sthn leitourgia rehash*/
void list_student_add_rehash(student *head, student student_to_add){
	student *curr,*prev;
	
	curr = (student *)malloc(sizeof(student));
	curr->root = student_to_add.root;/*krata thn lista mathimatwn toy mathiti*/
	strcpy(curr->name, student_to_add.name);
	curr->aem = student_to_add.aem;
	curr->lessons = student_to_add.lessons;
	prev = find(head, student_to_add.name);
	curr->next = prev->next;
	curr->prev = prev;
	curr->next->prev = curr;
	curr->prev->next = curr;
}

/*katharizei ena bucket mathitwn*/
void list_student_destroy(student *head){
	student *curr;
	/*vriskoyme kathe fora to teleytaio stoixeio toy bucket kai to kanoyme free*/
	while(1){
		for(curr=head->next;curr->next!=head;curr=curr->next);
		if(head->next == head){
			break;
		}
		list_lessons_destroy(curr->root);
		curr->next->prev= curr->prev;
		curr->prev->next= curr->next;
		free(curr);
	}
}

/*ektipwnei ena bucket*/
void list_student_print(student *head){
	student *curr;
	
	if(head != NULL){
		curr = head->next;
		while(curr != head){
			printf(" [%ld %s %hu]", curr->aem, curr->name, curr->lessons);
			curr = curr->next;
		}
	}
	printf("\n");
}

/*elenxei olo ton pinaka an iparxei mathiths me to idio aem*/
int list_student_has_aem(data_base data, int aem){
	unsigned long int i;
	student *curr;
	
	for(i=0;i<data.table_size;i++){
		curr = data.student_base[i]->next;
		while(curr != data.student_base[i]){
			if(curr->aem == aem){
				return(1);
			}
			else {
				curr = curr->next;
			}
		}
	}
	return(-1);
}

/*anakatanomh mathitwn(rehashing) kai auksish tou hash table*/
student **rehashing_add(data_base *data){
	unsigned long int pos,new_pos,old_pos;
	student *curr,*add;
	
	data->table_size = (2 * data->table_size);
	data->student_base = (student **)realloc(data->student_base, data->table_size * sizeof(student*));
	/*arxikopoiw tis nees thesis*/
	for(pos=(data->table_size/2);pos<data->table_size;pos++){
		data->student_base[pos] = list_student_init(data->student_base[pos]);
	}
	/*pername ta stoixeia ston neo pinaka, xekinontas apo to telos toy kathe bucket*/
	for(pos=0;pos<(data->table_size/2);pos++){
		for(curr=data->student_base[pos]->next;curr->next!=data->student_base[pos];curr=curr->next);
		while(curr != data->student_base[pos]){
			curr = curr->prev;
			add = curr->next;
			old_pos = hash(add->name) % (data->table_size/2);
			new_pos = hash(add->name) % data->table_size;
			if(old_pos != new_pos){
				list_student_add_rehash(data->student_base[new_pos], (*add));
				data->student_base[new_pos]->bucket_size++;
				add->next->prev = add->prev;
				add->prev->next = add->next;
				/*den kanoyme free thn lista ton mathimatwn giati tha prepei na ta xanaperasoyme*/
				free(add);
				if(data->student_base[pos]->bucket_size > 0){
					data->student_base[pos]->bucket_size--;
				}
			}
		}
	}
	return(data->student_base);
}

/*prosthikh mathith ston pinaka*/
student **add(data_base *data){
	student student_new;
	unsigned long int pos;
	long unsigned int list_has_aem;
	double load_factor;
	student *curr,*prev;
	
	if(data->table_size == 0){
		data->table_size++;
		data->student_base = (student **) realloc(data->student_base, (data->table_size) * sizeof(student*));
		data->student_base[0] = list_student_init(data->student_base[0]);
		
	}
	scanf(" %ld",&student_new.aem);
	list_has_aem = list_student_has_aem(*data, student_new.aem);
	if(list_has_aem == -1){
		scanf(" %s", student_new.name);
		name_to_upps(student_new.name);
		pos = hash(student_new.name) % data->table_size;
		scanf(" %hu", &student_new.lessons);
		curr = (student *)malloc(sizeof(student));
		curr->root = list_lessons_init(curr->root);
		strcpy(curr->name, student_new.name);
		curr->aem = student_new.aem;
		curr->lessons = student_new.lessons;
		prev = find(data->student_base[pos], student_new.name);
		/*elegxei an yparxei to onoma idi stin lista, efoson den yparxei to prosthetei sthn swsth thesi*/
		if(strcmp(prev->next->name, student_new.name) == 0 && prev->next != data->student_base[pos]){
			printf("\nA-NOK %s\n",student_new.name);
			return(data->student_base);
		}
		else {
			curr->next = prev->next;
			curr->prev = prev;
			curr->next->prev = curr;
			curr->prev->next = curr;
			printf("\nA-OK\n");
			data->student_base[pos]->bucket_size++;
			data->number_of_students++;
			load_factor = (double)data->number_of_students/data->table_size;
			if(load_factor >= 4){
				data->student_base = rehashing_add(data);
			}
			return(data->student_base);
		}
	}
	else {
		printf("\nA-NOK %ld\n", student_new.aem);
	}
	return(data->student_base);
}

/*anakatanomh kai meiwsh tou size*/
student **rehashing_remove(data_base *data){
	unsigned long int pos,new_pos,old_pos;
	student *curr,*add;
	
	if(data->table_size != 1){
		data->table_size = (data->table_size / 2);
	}

	for(pos=0;pos<(2*data->table_size);pos++){
		/*vriskei ton teleutaio mathiti ths thesis*/
		for(curr=data->student_base[pos]->next;curr->next!=data->student_base[pos];curr=curr->next);
		/*h while trexei mexri na xanaperastoun oloi oi mathites toy bucket list , xekinontas apo to telos tou*/
		while(curr != data->student_base[pos]){
			curr = curr->prev;
			add = curr->next;
			new_pos = hash(add->name) % data->table_size;
			old_pos = hash(add->name) % (data->table_size*2);
			if(new_pos != old_pos){
				list_student_add_rehash(data->student_base[new_pos], (*add));
				data->student_base[new_pos]->bucket_size++;
				add->next->prev = add->prev;
				add->prev->next = add->next;
				/*den kanoyme free thn lista ton mathimatwn giati tha prepei na ta xanaperasoyme*/
				free(add);
				if(data->student_base[pos]->bucket_size > 0){
					data->student_base[pos]->bucket_size--;
				}
			}
		}
	}
	data->student_base = (student **)realloc(data->student_base, data->table_size * sizeof(student*));
	return(data->student_base);
}

/*afairesh mathith apo ton pinaka*/
student **remove_student(data_base *data){
	student student,*curr;
	unsigned long int pos;
	double load_factor;
	
	scanf(" %s", student.name);
	name_to_upps(student.name);
	if(data->table_size != 0){
		pos = hash(student.name) % data->table_size;
	}
	else {
		pos = 0;
	}
	strcpy(data->student_base[pos]->name,student.name);
	curr = find_curr(data->student_base[pos], student.name);
	if(curr != data->student_base[pos] && strcmp(curr->name,student.name) == 0){
		curr->next->prev = curr->prev;
		curr->prev->next = curr->next;
		list_lessons_destroy(curr->root);
		free(curr);
		data->student_base[pos]->bucket_size--;
		data->number_of_students--;
		load_factor = (double)data->number_of_students/data->table_size;
		if(load_factor <= 1 ){
			data->student_base = rehashing_remove(data);
		}
		printf("\nR-OK\n");
	}
	else {
		printf("\nR-NOK %s\n", student.name);
	}
	return(data->student_base);
}

/*allagh xrwstoumenwn mathimatwn*/
void mod(data_base *data){
	short unsigned int new_lessons;
	char name[64];
	student *curr;
	unsigned long int pos;
	
	scanf(" %s", name);
	name_to_upps(name);
	pos = hash(name) % data->table_size;
	
	curr = find_curr(data->student_base[pos], name);
	if(strcmp(curr->name, name) != 0){
		printf("\nM-NOK %s\n", name);
	}
	else if(curr == data->student_base[pos]){
		printf("\nM-NOK %s\n", name);
	}
	else {
		scanf(" %hu", &new_lessons);
		curr->lessons = new_lessons;
		printf("\nM-OK\n");
	}
}
/*vriskei ton mathith*/
void find_stu(data_base *data){
	char name[64];
	student *curr;
	unsigned long int pos;
	clock_t end=0, start=0,difference=0;
	
	start = clock();
	scanf(" %63s", name);
	name_to_upps(name);
	if(data->table_size != 0){
		pos = hash(name) % data->table_size;
		curr = find_curr(data->student_base[pos], name);
		
		if(strcmp(curr->name, name) != 0){
			printf("\nF-NOK %s\n", name);
		}
		else if(curr == data->student_base[pos]){
			printf("\nF-NOK %s\n", name);
		}
		else {
			printf("\n#");
			printf("\nF-OK %ld %hu\n", curr->aem, curr->lessons);
		}
	}
	else {
		printf("\nF-NOK %s\n", name);
	}
	end = clock();
	difference = (end - start);
	fprintf(stderr, "\n# %lu, %ju\n",data->student_base[pos]->comps, difference);
}
/*vriskei kai epistrefei to megalitero bucket*/
unsigned long int find_largest_bucket(data_base data){
	unsigned long int largest_bucket=0;
	unsigned long int pos;
	
	if(data.number_of_students == 1){
		largest_bucket = 1;
		return(largest_bucket);
	}
	largest_bucket = data.student_base[0]->bucket_size;
	for(pos=1;pos<data.table_size;pos++){
		if(largest_bucket < data.student_base[pos]->bucket_size){
			largest_bucket = data.student_base[pos]->bucket_size;
		}
	}
	return(largest_bucket);
}
/*kanei print olo to hash table*/
void print_base(data_base data){
	unsigned long int largest_bucket=0,pos;
	double load_factor=0;
	
	if(data.number_of_students > 0){
		load_factor = (double)data.number_of_students/data.table_size;
		largest_bucket = find_largest_bucket(data);
		printf("\n#\n");
		printf("%lu %lu %.2lf %lu", data.table_size, data.number_of_students, load_factor, largest_bucket);
		for(pos=0;pos<data.table_size;pos++){
			printf("\n%lu %lu", pos, data.student_base[pos]->bucket_size);
			list_student_print(data.student_base[pos]);
		}
		printf("\n");
	}
	else {
		printf("\n#\n");
		printf("%lu %lu 0.00 0", data.table_size, data.number_of_students);
		for(pos=0;pos<data.table_size;pos++){
			printf("\n%lu 0\n",pos);
		}
		printf("\n");
	}
}
/*kanei eggrafh sto mathima*/
void register_lesson(data_base *data){
	char name[64];
	unsigned short code;
	student *curr;
	unsigned long int pos;
	
	scanf(" %63s", name);
	name_to_upps(name);
	scanf(" %hu", &code);
	pos = hash(name) % data->table_size;
	curr = find_curr(data->student_base[pos], name);
	
	if(strcmp(curr->name, name) != 0){
		printf("\nG-NOK %s\n", name);
	}
	else if(curr == data->student_base[pos]){
		printf("\nG-NOK %s\n", name);
	}
	else {
		if(is_lesson_on_list(curr->root, code) == -1){
				list_lessons_insert(curr->root, code);
				printf("\nG-OK\n");
		}
		else {
			printf("\nG-NOK %hu\n", code);
		}
	}
}

/*svhnei ton mathith apo to epilegmeno mathima*/
void unreg(data_base *data){
	char name[64];
	unsigned short code;
	student *curr;
	unsigned long int pos;
	
	scanf(" %63s", name);
	name_to_upps(name);
	scanf(" %hu", &code);
	
	pos = hash(name) % data->table_size;
	curr = find_curr(data->student_base[pos], name);

	if(strcmp(curr->name, name) != 0){
		printf("\nU-NOK %s\n", name);
	}
	else if(curr == data->student_base[pos]){
		printf("\nU-NOK %s\n", name);
	}
	else {
		if(is_lesson_on_list(curr->root, code) == 1){
			list_lessons_remove(curr->root, code);
			printf("\nU-OK\n");
		}
		else {
			printf("\nU-NOK %hu\n", code);
		}
	}
}
/*elegxei an einai eggegrammenos sto mathima*/
void isreg(data_base *data){
	char name[64];
	unsigned short code;
	student *curr;
	unsigned long int pos;
	
	scanf(" %63s", name);
	name_to_upps(name);
	scanf(" %hu", &code);
	
	pos = hash(name) % data->table_size;
	curr = find_curr(data->student_base[pos], name);
	if(strcmp(curr->name, name) != 0){
		printf("\nI-NOK %s\n", name);
	}
	else if(curr == data->student_base[pos]){
		printf("\nI-NOK %s\n", name);
	}
	else {
		if(is_lesson_on_list(curr->root, code) == 1){
			printf("\nYES\n");
		}
		else {
			printf("\nNO\n");
		}
	}
}
/*ektipwnei olh thn lista twn mathimatwn*/
void print_list(data_base data){
	char name[64];
	student *curr;
	unsigned long int pos;
	
	scanf(" %63s", name);
	name_to_upps(name);
	
	pos = hash(name) % data.table_size;
	curr = find_curr(data.student_base[pos], name);

	if(strcmp(curr->name, name) != 0){
		printf("\nL-NOK %s\n", name);
	}
	else if(curr == data.student_base[pos]){
		printf("\nL-NOK %s\n", name);
	}
	else {
		printf("\n#\n");
		printf("%s", name);
		list_lessons_print(curr->root);
	}
}
/*kanei clear olo to hash table, vazontas sto arxikw megethos toy*/
void clear(data_base *data, unsigned long int size){
	unsigned long int pos;
	
	for(pos=0;pos<data->table_size;pos++){
		data->student_base[pos]->bucket_size = 0;
		list_student_destroy(data->student_base[pos]);
	}
	data->number_of_students=0;
	data->table_size = size;	
}

int main(int argc, char *argv[]){
	char option;
	data_base data;
	int i;
	unsigned long int size;
	
	data.number_of_students = 0;
	
	if(argc == 2 ){
		data.table_size = atoi(argv[1]);
	}
	else {
		data.table_size = 8;		
	}
	size = data.table_size;
	data.student_base = (student **) malloc((data.table_size)*sizeof(student*));
	if(data.student_base == NULL){
		printf("\n!Malloc Error!\n");
	}
	if(data.table_size == 0){
		data.table_size++;
	}
	
	for(i=0;i<data.table_size;i++){
		data.student_base[i] = list_student_init(data.student_base[i]);
	}
		
	while(1){
		scanf(" %c", &option);
		switch(option){
			case('a') : {
				data.student_base = (student **) add(&data);
				clean_stdin();
				break;
			}
			case('r') : {
				data.student_base = (student **) remove_student(&data);
				clean_stdin();
				break;
			}
			case('m') : {
				mod(&data);
				clean_stdin();
				break;
			}
			case ('f') : {
				find_stu(&data);
				clean_stdin();
				break;
			}
			case('p') : {
				print_base(data);
				clean_stdin();
				break;
			}
			case('g') : {
				register_lesson(&data);
				clean_stdin();
				break;
			}
			case('u') : {
				unreg(&data);
				clean_stdin();
				break;
			}
			case('i') : {
				isreg(&data);
				clean_stdin();
				break;
			}
			case('l') : {
				print_list(data);
				clean_stdin();
				break;
			}
			case('c') : {
				clear(&data,size);
				clean_stdin();
				printf("\nC-OK\n");
				break;
			}
			case('q'): {
				clear(&data,size);
				return(0);
			}
			default : {
				return(0);
			}
		}
	}
	return(0);
}
