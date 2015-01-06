/*
 * Type Definitions */
typedef struct note_struct{
	unsigned int pitch;
	unsigned int tick_len;
	//articulation

} note_struct;

typedef struct Q_node{
	note_struct * note;
	unsigned int trig_tick;
	struct Q_node * next;

} Q_node; 

typedef struct Q_head{
	Q_node * Q_ptr;
	unsigned int len;

} Q_head;



void		enqueue(Q_head* q, unsigned int n, unsigned int dur);
void		dequeue(Q_head* q);
void		clearqueue(Q_head* q);
//Q_node*	pop(Q_head* q);
note_struct* check4note(Q_head* Q, unsigned int tick);
void		print_queue(Q_head* Q);


/*
 *Function Definitions
 */


//Q_node * pop(Q_head * q){
//	Q_node * popped = q->Q_ptr;
//	if(q->Q_ptr != NULL)
//		q->Q_ptr = q->Q_ptr->next;
//
//	q->len -= popped->note->duration;
//	return popped;
//}

void print_queue(Q_head* q){
	printf("Queue len:%u\n", q->len);
	Q_node * tail = q->Q_ptr;
	while(tail != NULL){
		printf("\ttrig_time:%u, pitch:%u, time:%u\n",
			tail->trig_tick, tail->note->pitch, tail->note->tick_len);
		tail = tail->next;
	}	
}
void enqueue(Q_head * q, unsigned int p, unsigned int ticks){
	Q_node** tail = &(q->Q_ptr);
	while(*tail != NULL)
		tail = &((*tail)->next);

	(*tail) = ( Q_node *) malloc(sizeof(Q_node));
	(*tail)->next = ( Q_node *) malloc(sizeof(Q_node));
	(*tail)->next = NULL;
	(*tail)->note = (note_struct *) malloc(sizeof(note_struct));
	(*tail)->note->pitch = p;
	(*tail)->note->tick_len = ticks;

	q->len+=ticks;

	tail = &(q->Q_ptr);
	unsigned int trig_time = 0;
	while(*tail != NULL){
		(*tail)->trig_tick = trig_time;
		trig_time += (*tail)->note->tick_len;	
		tail = &((*tail)->next);
	}
	//print_queue(q);
}

void dequeue(Q_head * Q){
	if(Q->Q_ptr != NULL){
		Q_node ** tail = &(Q->Q_ptr);
		while((*tail)->next != NULL)
			tail = &((*tail)->next);
	
		Q_node * dequeued = *tail;	
		*tail = NULL;
		Q->len -= dequeued->note->tick_len;
		free(dequeued);
	}
}	

void clearqueue(Q_head* Q){
	while(Q->Q_ptr != NULL)
		dequeue(Q);
}

note_struct* check4note(Q_head * Q, unsigned int tick){
	note_struct* ret_note = NULL;

	Q_node* tracer= Q->Q_ptr;
	while(tracer != NULL){
		if(tick == tracer->trig_tick){
			puts("in tick checker");
			print_queue(Q);
			ret_note = malloc(sizeof(note_struct));
			ret_note = tracer->note;
			break;
		}
		tracer = tracer->next;
	}

	return ret_note;
}
