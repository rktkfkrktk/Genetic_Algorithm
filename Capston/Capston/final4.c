#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define GENERATION 50
#define GENERATIONS 500
#define TEST 200
#define MAX_QUEUE 10000
//define one generation's number 20
#define ACCEL 100
//at cross section, how much accel at upper things (1~1000)
#define ACCEL_VALUE 5
//how much upper value need to survive


float KOSPI[220];
int KOSPI_UPDOWN[220];
//2000.02~2017.12
struct member {
	int date[250];
	float fac1[250];
	float fac2[250];
	float fac3[250];
	float fac4[250];
	float fac5[250];
	float fac6[250];
	float fac7[250];
	float fac8[250];
	//need to know all factor.
};
struct member kospi;
//kospi stuct have all data that need to make GA

typedef struct TreeNode {
	char cal;
	float data;
	int flag;
	//flag 0 = calculate, 1~8 = variable, 9 = random number
	struct TreeNode * left;
	struct TreeNode * right;
}TreeNode;
//TreeNode have 3 data. if calculator, or constant or variable(data)

TreeNode * createNode(char cal, float newData, int val) {
	TreeNode * NewNode = (TreeNode*)malloc(sizeof(TreeNode));
	NewNode->left = NULL;
	NewNode->right = NULL;
	NewNode->data = newData;
	NewNode->cal = cal;
	NewNode->flag = val;
	return NewNode;
}
//create new node

struct Queue {
	int front, rear;
	int size;
	struct TreeNode* *array;
};
//I want to make complete binary tree at first. so I use Queue

struct Queue* createQueue() {
	struct Queue* queue = (struct Queue*)malloc(sizeof(struct Queue));
	queue->front = queue->rear = -1;
	queue->size = MAX_QUEUE;
	queue->array = (struct TreeNode**)malloc(queue->size * sizeof(struct TreeNode*));
	for (int i = 0; i < queue->size; i++) {
		queue->array[i] = NULL;
	}
	return queue;
};
//create Queue.

struct KGA {
	//binary tree one, how much correct, flag,
	int acc;
	float accuracy;
	//accuracy = ok/216
	int acc_rank;
	//accuracy ranking
	int flagset[8];
	int factor;
	//i want to use lot of factor.
	int height;
	//Tree's max height = 15.
	TreeNode* Tree;
};

struct KGA GAKOSPI[GENERATION];

int isEmpty(struct Queue* queue) {
	return queue->front == -1;
}
int isFull(struct Queue* queue) {
	return queue->rear == queue->size - 1;
}
int hasOnlyOneItem(struct Queue* queue) {
	return queue->front == queue->rear;
}
void Enqueue(struct TreeNode *root, struct Queue* queue) {
	if (isFull(queue)) {
		return;
	}
	queue->array[++queue->rear] = root;
	if (isEmpty(queue)) {
		++queue->front;
	}
}
struct TreeNode* Dequeue(struct Queue* queue) {
	if (isEmpty(queue))
		return NULL;

	struct TreeNode* temp = queue->array[queue->front];
	if (hasOnlyOneItem(queue)) {
		queue->front = queue->rear = -1;
	}
	else {
		++queue->front;
	}
	return temp;
}
struct TreeNode* getFront(struct Queue* queue) {
	return queue->array[queue->front];
}
int hasBothChild(struct TreeNode* temp) {
	return temp && (temp->left) && (temp->right);
}
void insert(struct TreeNode **root, int flag, char cal, float data, struct Queue* queue) {

	struct TreeNode *temp = createNode(cal, data, flag);

	if (!*root) {
		*root = temp;
	}
	else {
		struct TreeNode* front = getFront(queue);
		if (!front->left) {
			front->left = temp;
		}
		else if (!front->right) {
			front->right = temp;
		}
		if (hasBothChild(front)) {
			Dequeue(queue);
		}
	}
	Enqueue(temp, queue);

}
int MAX(int a, int b) {
	if (a > b) {
		return a;
	}
	else {
		return b;
	}
}
int GetHeight(TreeNode *Node) {
	if (Node == NULL) {
		return 0;
	}
	else
		return MAX(GetHeight(Node->left), GetHeight(Node->right));
}


void KOSPI_GenerateGA(struct KGA *GAKOSPI) {
	GAKOSPI->acc = 0;
	GAKOSPI->accuracy = 0;
	GAKOSPI->acc_rank = 0;
	for (int i = 0; i < 8; i++) {
		GAKOSPI->flagset[i] = 0;
	}
	GAKOSPI->factor = 0;
	GAKOSPI->height = 0;

	struct Queue* queue = createQueue();

	for (int i = 0; i < 31; i++) {
		//printf("generated...\n");
		int a = (rand() % 4);
		if (a == 0) {
			insert(&GAKOSPI->Tree, 0, '+', 0, queue);
		}
		else if (a == 1) {
			insert(&GAKOSPI->Tree, 0, '-', 0, queue);
		}
		else if (a == 2) {
			insert(&GAKOSPI->Tree, 0, '*', 0, queue);
		}
		else if (a == 3) {
			insert(&GAKOSPI->Tree, 0, '/', 0, queue);
		}
		else {
			printf("??");
		}
	}
	for (int i = 0; i < 32; i++) {
		if (i % 2 == 0) {
			float num = (rand() % 1000 + 1) * 0.01;
			insert(&GAKOSPI->Tree, 9, ' ', num, queue);
		}
		else {
			float a = rand() % 10 + (rand() % 100 / 100);
			int b = (rand() % 8) + 1;
			insert(&GAKOSPI->Tree, b, ' ', 0, queue);
		}
	}

}

float test_data_input(int flag, int i) {
	switch (flag) {
	case 1:
		return kospi.fac1[i];
		break;
	case 2:
		return kospi.fac2[i];
		break;
	case 3:
		return kospi.fac3[i];
		break;
	case 4:
		return kospi.fac4[i];
		break;
	case 5:
		return kospi.fac5[i];
		break;
	case 6:
		return kospi.fac6[i];
		break;
	case 7:
		return kospi.fac7[i];
		break;
	case 8:
		return kospi.fac8[i];
		break;
	dafault:
		break;

	}
}

float Evaluate(TreeNode* Node, int i) {
	if (Node == NULL) {
		return 0;
	}
	else if (Node->left == NULL && Node->right == NULL) {
		if (Node->flag != 9) {
			Node->data = test_data_input(Node->flag, i);
		}
		return Node->data;
	}
	else {
		float op1 = Evaluate(Node->left, i);
		float op2 = Evaluate(Node->right, i);
		if (op1 > 1000 || op1 < -1000) {
			op1 = op1 / 100;
		}
		if (op2 > 1000 || op2 < -1000) {
			op2 = op2 / 100;
		}

		switch (Node->cal) {
		case'+':
			return op1 + op2;
			break;
		case '-':
			return op1 - op2;
			break;
		case '*':
			if (op1 != 0 && op2 != 0) {
				return op1 * op2;
			}
			else {
				return op1 + op2;
			}
			break;
		case '/':
			if (op2<-1 || op2>1) {
				return op1 / op2;
			}
			else {
				return op1 + op2;
			}
			break;
		}
	}
	return 0;
}

void test(struct KGA *GAKOSPI) {

	//int height = GetHeight(GAKOSPI->Tree);
}
void data_input() {
	FILE *f;
	fopen_s(&f, "Total_UPDOWN.txt", "r");
	for (int i = 0; i < 216; i++) {
		fscanf_s(f, "%d	%f	%d	%f	%f	%f	%f	%f	%f	%f	%f\n", &kospi.date[i], &KOSPI[i], &KOSPI_UPDOWN[i],
			&kospi.fac1[i], &kospi.fac2[i], &kospi.fac3[i], &kospi.fac4[i], &kospi.fac5[i], &kospi.fac6[i], &kospi.fac7[i], &kospi.fac8[i]);
		//	printf("%d\t%f\t%d\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\n", kospi.date[i], KOSPI[i],KOSPI_UPDOWN[i],
		//		kospi.fac1[i], kospi.fac2[i], kospi.fac3[i], kospi.fac4[i], kospi.fac5[i], kospi.fac6[i], kospi.fac7[i], kospi.fac8[i]);
	}
	fclose(f);
}
void levelOrder(struct TreeNode* root) {
	struct Queue* queue = createQueue();
	Enqueue(root, queue);
	while (!isEmpty(queue)) {
		struct TreeNode* temp = Dequeue(queue);
		if (temp->cal != ' ') {
			printf("%c ", temp->cal);
		}
		else
			printf("", temp->data);

		if (temp->left) {
			Enqueue(temp->left, queue);

		}
		if (temp->right) {
			Enqueue(temp->right, queue);
		}
	}
}
void sortingGA() {
	struct KGA temp;
	int i = 0, j = 0;
	for (i = 0; i < GENERATION - 1;i++) {
		int max = i;
		for (j = i + 1; j < GENERATION; j++) {
			if (GAKOSPI[j].acc > GAKOSPI[max].acc) {
				max = j;
			}
		}
		temp = GAKOSPI[i];
		GAKOSPI[i] = GAKOSPI[max];
		GAKOSPI[max] = temp;
	}
}
void sortingGA_NEXT(struct KGA *GAKOSPI_NEXT) {
	struct KGA temp;

	int i = 0, j = 0;
	for (i = 0; i < GENERATION - 1;i++) {
		int max = i;
		for (j = i + 1; j < GENERATION; j++) {
			if (GAKOSPI_NEXT[j].acc > GAKOSPI_NEXT[max].acc) {
				max = j;
			}
		}
		temp = GAKOSPI_NEXT[i];
		GAKOSPI_NEXT[i] = GAKOSPI_NEXT[max];
		GAKOSPI_NEXT[max] = temp;
	}
}


void generate_smallGA(struct KGA *GAKOSPI) {
	GAKOSPI->acc = 0;
	GAKOSPI->accuracy = 0;
	GAKOSPI->acc_rank = 0;
	for (int i = 0; i < 8; i++) {
		GAKOSPI->flagset[i] = 0;
	}
	GAKOSPI->factor = 0;
	GAKOSPI->height = 0;
	GAKOSPI->Tree = NULL;

	struct Queue* queue = createQueue();

	for (int i = 0; i < 11; i++) {
		//printf("generated...\n");
		int a = (rand() % 4);
		if (a == 0) {
			insert(&GAKOSPI->Tree, 0, '+', 0, queue);
		}
		else if (a == 1) {
			insert(&GAKOSPI->Tree, 0, '-', 0, queue);
		}
		else if (a == 2) {
			insert(&GAKOSPI->Tree, 0, '*', 0, queue);
		}
		else if (a == 3) {
			insert(&GAKOSPI->Tree, 0, '/', 0, queue);
		}
		else {
			printf("??");
		}
	}
	for (int i = 0; i < 12; i++) {
		if (i % 2 == 0) {
			float num = (rand() % 1000 + 1) * 0.01;
			insert(&GAKOSPI->Tree, 9, ' ', num, queue);
		}
		else {
			float a = rand() % 10 + (rand() % 100 / 100);
			int b = (rand() % 8) + 1;
			insert(&GAKOSPI->Tree, b, ' ', 0, queue);
		}
	}

}

void KOSPI_NextGA(struct KGA * TEMP,struct KGA *GAKOSPI_NEXT) {
	int random, random2;
	int leftright = 0;
	int leftright2 = 0;
	GAKOSPI_NEXT->acc = 0;
	GAKOSPI_NEXT->accuracy = 0;
	GAKOSPI_NEXT->acc_rank = 0;
	for (int i = 0; i < 8; i++) {
		GAKOSPI_NEXT->flagset[i] = 0;
	}
	GAKOSPI_NEXT->factor = 0;
	GAKOSPI_NEXT->height = 0;

	TreeNode * first = createNode(' ', 0, 0);
	TreeNode * second = createNode(' ', 0, 0);
	TreeNode * first_temp = createNode(' ', 0, 0);
	TreeNode * second_temp = createNode(' ', 0, 0);
	TreeNode * second_temp_temp = createNode(' ', 0, 0);

	//choice first input gakospi. rullet wheel with rand algo
	random = rand() % 1000 + 1;
	if (random < ACCEL) {
		random2 = rand() % ACCEL_VALUE;
		first = TEMP[random2].Tree;
	}
	else {
		random2 = rand() % GENERATION;
		first = TEMP[random2].Tree;

	}
	random = rand() % 1000 + 1;
	if (random < ACCEL) {
		random2 = rand() % ACCEL_VALUE;
		second = TEMP[random2].Tree;
	}
	else {
		random2 = rand() % GENERATION;
		second = TEMP[random2].Tree;
	}

	first_temp = first;
	second_temp = second;
	second_temp_temp = second;

	//choice is over.


	random = rand() % 100;
	if (random == 0) {
		first_temp->cal = '+';
	}
	else if (random == 1) {
		first_temp->cal = '-';
	}
	else if (random == 2) {
		first_temp->cal = '*';
	}
	else if (random == 3) {
		first_temp->cal = '/';
	}
	//first calculator change.

	random = rand() % 2;
	if (random == 0&&first_temp->left!=NULL) {
		first_temp = first_temp->left;
	}
	if(random == 1 && first_temp->right != NULL) {
	first_temp = first_temp->right;
	}
	random = 1;
	while (first_temp->cal != ' ' && random != 0 && first_temp->left != NULL && first_temp->right != NULL) {
		random = rand() % 5;
		if (random == 0)break;
		if (first_temp == NULL)break;
		else if (random <= 2) {
			first_temp = first_temp->left;
			leftright = 0;
		}
		else {
			first_temp = first_temp->right;
			leftright = 1;
		}
	}
	//first tree's cross point is first_temp.
	random = rand() % 2;
	if (random == 0 && second_temp->left != NULL) {
		second_temp = second_temp->left;
	}
	if (random == 1 && second_temp->right != NULL) {
		second_temp = second_temp->right;
	}
	random = 1;
	while (second_temp->cal != ' ' && random != 0 && second_temp->left != NULL && second_temp->right != NULL) {
		random = rand() % 5;
		if (random == 0)break;
		if (second_temp == NULL)break;
		else if (random <= 2) {
			second_temp_temp = second_temp;
			second_temp = second_temp->left;
			leftright2 = 0;
		}
		else {
			second_temp_temp = second_temp;
			second_temp = second_temp->right;
			leftright2 = 1;
		}
	}
	//second tree's cross point is second_temp.
	struct KGA *smallGA = (struct KGA*)malloc(sizeof(struct KGA));
	generate_smallGA(&smallGA[0]);
	//levelOrder(smallGA[0].Tree);
	
	if (leftright == 0) {
		first_temp->left = second_temp;
		second_temp->left = NULL;
		second_temp->right = NULL;
		if (leftright2 == 0) {
			second_temp_temp->left = smallGA->Tree;
		}
		else {
			second_temp_temp->right = smallGA->Tree;
		}
	}
	else {
		first_temp->right = second_temp;
		second_temp->left = NULL;
		second_temp->right = NULL;
		if (leftright2 == 0) {
			second_temp_temp->left = smallGA->Tree;
		}
		else {
			second_temp_temp->right = smallGA->Tree;
		}
	}

	//cross.
	GAKOSPI_NEXT->Tree = first;


}
TreeNode* copy_tree(TreeNode * original) {
	TreeNode * temp;
	if (original) {
		temp = (TreeNode*)malloc(sizeof(TreeNode));
		temp->left = copy_tree(original->left);
		temp->right = copy_tree(original->right);
		temp->cal = original->cal;
		temp->data = original->data;
		temp->flag = original->flag;
		return temp;
	}

	return NULL;
}
void copy(struct KGA * temp, int i) {
	temp->acc = GAKOSPI[i].acc;
	temp->accuracy = GAKOSPI[i].accuracy;
	temp->acc_rank = GAKOSPI[i].acc_rank;
	for (int j = 0; j < 8; j++) {
		temp->flagset[j] = GAKOSPI[i].flagset[j];
	}
	temp->factor = GAKOSPI[i].factor;
	temp->height = GAKOSPI[i].height;
	temp->Tree = copy_tree(GAKOSPI[i].Tree);
}

int main(void) {
	int i = 0, j = 0;
	srand(time(NULL));
	data_input();
	//use for to make 100 trees.
	for (i = 0; i < GENERATION;i++) {
		KOSPI_GenerateGA(&GAKOSPI[i]);
	}

	for (i = 0; i < GENERATION; i++) {
		for (j = 0; j < TEST; j++) {
			float evaluateResult = Evaluate(GAKOSPI[i].Tree, j);
			if (evaluateResult >= 0 && KOSPI_UPDOWN[j + 1] == 1) {
				GAKOSPI[i].acc++;
			}
			else if (evaluateResult < 0 && KOSPI_UPDOWN[j + 1] == 0) {
				GAKOSPI[i].acc++;
			}
		}
	}

	sortingGA();
	/*
	for (i = 0; i < GENERATION;i++) {
		printf("%3d ", GAKOSPI[i].acc);
	}
	*/
	for (int h = 0; h < GENERATIONS; h++) {



		struct KGA *GAKOSPI_NEXT;
		GAKOSPI_NEXT = (struct KGA*)malloc(sizeof(struct KGA)*GENERATION);

		for (i = 0; i < GENERATION; i++) {
			struct KGA *TEMP_GA;
			TEMP_GA = (struct KGA*)malloc(sizeof(struct KGA)*GENERATION);
			for (j = 0; j < GENERATION; j++) {
				//copy
				copy(&TEMP_GA[j], j);

			}


			KOSPI_NextGA(TEMP_GA,&GAKOSPI_NEXT[i]);
			//	levelOrder(GAKOSPI_NEXT[i].Tree);
			//	printf("\n");
			free(TEMP_GA);
		}



		for (i = 0; i < GENERATION; i++) {
			for (j = 0; j < TEST; j++) {
				float evaluateResult = Evaluate(GAKOSPI_NEXT[i].Tree, j);

				//if (i ==29)printf("acc = %d result = %f\n", GAKOSPI_NEXT[i].acc, evaluateResult);
				if (evaluateResult >= 0 && KOSPI_UPDOWN[j + 1] == 1) {
					GAKOSPI_NEXT[i].acc+=1;
				}
				else if (evaluateResult < 0 && KOSPI_UPDOWN[j + 1] == 0) {
					GAKOSPI_NEXT[i].acc+=1;
				}

			}
		}

		
		sortingGA_NEXT(GAKOSPI_NEXT);
		for (i = ACCEL_VALUE; i < GENERATION; i++) {
			GAKOSPI[i] = GAKOSPI_NEXT[i - ACCEL_VALUE];
		}

		
		for (i = 0; i < GENERATION; i++) {
			printf("%3d ",GAKOSPI[i].acc);
			//levelOrder(GAKOSPI_NEXT[i].Tree);
			//printf("\n");
		}
		printf("\n");

		sortingGA();

		if (GAKOSPI[0].acc > 140) {
			break;
		}
		free(GAKOSPI_NEXT);
	}




	getchar();
	return 0;
}