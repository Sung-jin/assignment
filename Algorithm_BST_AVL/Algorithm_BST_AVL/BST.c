#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define InitNum -1	//init Tree root key value

#pragma comment(lib, "winmm.lib") 

typedef struct TreeNode {
	int key;
	struct TreeNode *PNode;	//Parent Node
	struct TreeNode *LNode;	//Left Node
	struct TreeNode *RNode;	//Right Node
}Tree;

Tree *root;

void inOrder(Tree *root);							//inorder func
void preOrder(Tree *root);							//preorder func
void postOrder(Tree *root);							//postorder func
void init_Tree();									//init tree
struct TreeNode *search(Tree *root, int value);		//search data in tree
void insert_Tree(Tree *insert_P, int value);		//insert data in tree
Tree *minmum(Tree *standard);						//find minmum data in tree
Tree *maximum(Tree *standard);						//find maximum data in tree
Tree *successor(Tree *preValue);					//find successor
void delete_Tree(Tree *source, int delNum);			//delete tree

int main() {
	
	int i = 0;
	FILE *inputF, *controlF;
	Tree *check;
	DWORD dwStartTime, dwEndTime;

	init_Tree();

	int Inputdata[5000], Controldata[5000];

	inputF = fopen("inputData.txt", "r");
	if (inputF == NULL) {
		perror("File read");
	}

	controlF = fopen("controlData.txt", "r");
	if (controlF == NULL) {
		perror("File read");
	}

	while (fscanf(inputF, "%d", &Inputdata[i]) != EOF) {
		i++;
	}
	i = 0;
	while (fscanf(controlF, "%d", &Controldata[i]) != EOF) {
		i++;
	}

	//5000 input data and control data

	//////////////Measure 5000 data insertion times/////////

	dwStartTime = timeGetTime();

	for (i = 0; i < 5000; i++) {
		insert_Tree(root, Inputdata[i]);

	}

	dwEndTime = timeGetTime();

	printf("Total insert time : %d ms \n", dwEndTime - dwStartTime);

	/////////////Saved to 5000 control data variables


	//////////////Measure 5000 Data search times/////////

	dwStartTime = timeGetTime();

	for (i = 0; i < 5000; i++) {
		check = search(root, Controldata[i]);
	}

	dwEndTime = timeGetTime();

	printf("Total search time : %d ms \n", dwEndTime - dwStartTime);


	//////////////Measure 5000 Data Deletion Times/////////

	dwStartTime = timeGetTime();

	for (i = 0; i < 5000; i++) {
		if (root->LNode != NULL && root->RNode != NULL && root != NULL) {
			delete_Tree(root, Controldata[i]);
		}
		else {
			init_Tree();
		}
	}

	dwEndTime = timeGetTime();

	printf("Total delete time : %d ms \n", dwEndTime - dwStartTime);


	if (root->key != -1) {
		inOrder(root);
	}
	else {
		printf("Empty Tree!!\n");
	}

	fclose(inputF);
	fclose(controlF);

	return 0;
	
}

void inOrder(Tree *root) {
	if (root->LNode != NULL) {
		inOrder(root->LNode);
	}
	printf("%d\n", root->key);
	if (root->RNode != NULL) {
		inOrder(root->RNode);
	}
	
}

void preOrder(Tree *root) {
	printf("%d\n", root->key);
	if (root->LNode != NULL) {
		preOrder(root->LNode);
	}
	if (root->RNode != NULL) {
		preOrder(root->RNode);
	}
}

void postOrder(Tree *root) {
	if (root->LNode != NULL) {
		postOrder(root->LNode);
	}
	if (root->RNode != NULL) {
		postOrder(root->RNode);
	}
	printf("%d\n", root->key);
}

void init_Tree() {	//root init
	root = (Tree*)malloc(sizeof(Tree));
	root->PNode = NULL;
	root->LNode = NULL;
	root->RNode = NULL;
	root->key = InitNum;
}

Tree *search(Tree *find, int value) {

	if (find == NULL || find->key == value)
		return find;
	//if the value does not exist, return NULL
	if (find->key < value)
		return search(find->RNode, value);
	else
		return search(find->LNode, value);

}

void insert_Tree(Tree *insert_P, int value) {
	
	Tree *temp = insert_P, *temp_P;
	Tree *Insert = (Tree*)malloc(sizeof(Tree));

	Insert->key = value;
	Insert->PNode = NULL;
	Insert->LNode = NULL;
	Insert->RNode = NULL;
	//insert Tree Node init
	if (insert_P->key == InitNum) {	//first insert this tree
		insert_P->key = value;
		free(Insert);				//temp node is not need, so release
		return;
	}

	while (temp != NULL) {
		if (temp->key == value) {
			printf("already existing value\n");	//Handling duplicate values
			free(Insert);						//temp node is not need, so release
		}
		else if (temp->key < value) {
			temp_P = temp;
			temp = temp->RNode;
		}
		else {
			temp_P = temp;
			temp = temp->LNode;
		}
	}
	
	
	if (temp_P->key < value) {
		temp_P->RNode = Insert;
		Insert->PNode = temp_P;
	}
	else {
		temp_P->LNode = Insert;
		Insert->PNode = temp_P;
	}

}

Tree *minmum(Tree *standard) {
	Tree *temp = standard;

	while (temp->LNode != NULL)
		temp = temp->LNode;
	return temp;
}

Tree *maximum(Tree *standard) {
	Tree *temp = standard;

	while (temp->LNode != NULL)
		temp = temp->RNode;
	return temp;
}

Tree *successor(Tree *preValue) {
	Tree *Ptemp = preValue->PNode, *temp = preValue;
	
	if (temp->RNode != NULL)
		return minmum(temp->RNode);

	while (Ptemp != NULL && temp == Ptemp->RNode) {
		temp = Ptemp;
		Ptemp = Ptemp->PNode;
	}

	return Ptemp;
}

void delete_Tree(Tree *source, int delNum) {

	Tree *temp_S, *temp_D, *temp;

	temp_S = search(source, delNum);	//find node to delete

	if (temp_S == NULL) {
		printf("Non-existent value\n");
	}


	if (temp_S->LNode == NULL || temp_S->RNode == NULL)
		temp_D = temp_S;
	else
		temp_D = successor(temp_S);


	if (temp_D->LNode != NULL)
		temp = temp_D->LNode;
	else
		temp = temp_D->RNode;


	if (temp != NULL)
		temp->PNode = temp_D->PNode;


	if (temp_D->PNode == NULL)
		temp_D = temp;
	else if (temp_D == temp_D->PNode->LNode)
		temp_D->PNode->LNode = temp;
	else
		temp_D->PNode->RNode = temp;


	if (temp_D != temp_S)
		temp_S->key = temp_D->key;
	
	
	free(temp_D);		//delete node, so release
}