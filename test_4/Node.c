#include <stdlib.h>
#include <stdio.h>
#define FIRST 1
/*This program creat a linked list with 'n' size that the client choose.
* The linked list build from the end ('n') to zero by series fibonacci. 
**** Define linked list *****/
typedef struct Node 
{
int data;
struct Node *next;
} Node;
/* Methods */
struct Node * create_linked_list(int);
void print_linked_list(struct Node *);

/* Main */
int main(){
	int n;
	printf("Please insert a positive number: \n");
	scanf( "%d",&n);
	printf("Your nodes number is: %d (not include node 0)\n",n);
	if(n < 0)
		printf("Ileagal number, please insert only positive numbers");
	else{
	Node *head = create_linked_list(n);
	print_linked_list(head);}
	return 0;}

/* Create new linked list 
* if n == 0 this method will return only 0 node.
* else this method create all of the node till 'n',
* when the next pointer will point on the previous node (from the end to begining)
*/
struct Node * create_linked_list(int n){
	int prev = 0;	
	Node *head = NULL;
	Node *temp;
	Node *last;
	Node *first;
	temp = (struct Node*)malloc(sizeof(struct Node));
	/* Only 1 node of 0 */	
	if(n == 0){
		temp-> data = 0;
		temp-> next = temp;
		return temp;}
	/* If n > 0 create node 1*/	
	temp-> data = FIRST;
	temp-> next = temp;
	head = temp;
	last = temp;
	while(n>1){
		temp = (struct Node*)malloc(sizeof(struct Node));
		temp-> data = prev+(last->data);
		prev = last->data;
		temp-> next = last;
		last = temp;
		n -= 1;
	}
	/* create node 0 and make it as the first one*/
	first = (struct Node*)malloc(sizeof(struct Node));
	first-> data = 0;
	first-> next = last;
	head-> next = first;
	head = last;
	return head;

}
/*Printing method from the highest (node 0) to the lowest (node 'n')*/
void print_linked_list(struct Node *head){
	Node *to_print = head;
	int k = 0;	
	while((to_print->data)>0){
		printf("Node num: %d = %d\n",k,to_print->data);
		to_print = to_print-> next;
		k += 1;
		}
		/*Print last node will data = 0*/
		printf("Node num: %d = %d\n",k,to_print->data);
		printf("*********************END**************************\n");
}
