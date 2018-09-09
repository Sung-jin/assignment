#include <stdio.h>
#include <stdlib.h>       //srand
#include <windows.h>

#define InitNum -1	//init Tree root key value

#pragma comment(lib, "winmm.lib") 

typedef struct TreeNode {
	int key;
	int level;
	int hight;
	struct TreeNode *PNode;	//Parent Node
	struct TreeNode *LNode;	//Left Node
	struct TreeNode *RNode;	//Right Node
}AVL;

AVL *root;	

void inOrder(AVL *root);						//inorder func
void preOrder(AVL *root);						//preorder func
void postOrder(AVL *root);						////postorder func
void init_Tree();								//init tree
struct TreeNode *search(AVL *root, int value);	//search data in tree
void insert_Tree(AVL *insert_P, int value);		//insert data in tree
AVL *minmum(AVL *standard);						//find maximum data in tree
AVL *maximum(AVL *standard);					//find minmum data in tree
AVL *successor(AVL *preValue);					//find successor
void delete_Tree(AVL *source, int delNum);		//delete tree
int get_height(AVL *avl_root);					//obtain height of all nodes func
void set_Height(AVL *avl_root, int height);		//Set the height of all nodes
void set_Level(AVL *avl_root);					//Set the level of all nodes
void leftRotation(AVL *rotation);				//leftRotation func
void rightRotation(AVL *rotation);				//RightRotation func

int main() {

	int i = 0;
	FILE *inputF, *controlF;
	AVL *check;
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

void inOrder(AVL *root) {
	if (root->LNode != NULL) {
		inOrder(root->LNode);
	}
	printf("key : %d\t\thight : %d\tlevel : %d\n", root->key, root->hight, root->level);
	if (root->RNode != NULL) {
		inOrder(root->RNode);
	}

}

void preOrder(AVL *root) {
	printf("%d\n", root->key);
	if (root->LNode != NULL) {
		preOrder(root->LNode);
	}
	if (root->RNode != NULL) {
		preOrder(root->RNode);
	}
}

void postOrder(AVL *root) {
	if (root->LNode != NULL) {
		postOrder(root->LNode);
	}
	if (root->RNode != NULL) {
		postOrder(root->RNode);
	}
	printf("%d\n", root->key);
}

void init_Tree() {	//root init
	root = (AVL*)malloc(sizeof(AVL));
	root->PNode = NULL;
	root->LNode = NULL;
	root->RNode = NULL;
	root->key = InitNum;
	root->level = 0;
	root->hight = 1;
}

AVL *search(AVL *find, int value) {

	if (find == NULL || find->key == value)
		return find;
	//if the value does not exist, return NULL
	if (find->key < value)
		return search(find->RNode, value);
	else
		return search(find->LNode, value);

}

void insert_Tree(AVL *insert_P, int value) {

	AVL *temp = insert_P, *temp_P;
	AVL *Insert = (AVL*)malloc(sizeof(AVL));

	int height;

	Insert->key = value;
	Insert->PNode = NULL;
	Insert->LNode = NULL;
	Insert->RNode = NULL;
	Insert->level = 0;
	Insert->hight = 1;
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

	root->hight = get_height(root);
	height = get_height(root);

	set_Height(root, height);
	set_Level(root);
	//Updated all height and level since new data is entered


	while (Insert != NULL) {
		if (Insert->level == 2 || Insert->level == -2)
			break;
		Insert = Insert->PNode;
	}
	if(Insert != NULL){
		if (Insert->level == -2 && Insert->LNode->level <= 0) {//SRR
			rightRotation(Insert);
		}

		if (Insert->level == -2 && Insert->LNode->level > 0) {//DRR
			leftRotation(Insert->LNode);
			rightRotation(Insert);
		}

		if (Insert->level == 2 && Insert->RNode->level >= 0) {//SLR
			leftRotation(Insert);
		}

		if (Insert->level == 2 && Insert->RNode->level < 0) {//DLR
			rightRotation(Insert->RNode);
			leftRotation(Insert);
		}
	}
}

void delete_Tree(AVL *source, int delNum) {

	AVL *temp_S, *temp_D, *temp;

	int height;

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


	if (temp_D != temp_S){
		temp_S->key = temp_D->key;
	}

	

	root->hight = get_height(root);
	height = get_height(root);

	set_Height(root, height);
	set_Level(root);
	//data has been deleted, so all heights and levels are updated
	temp = temp_D;

	while (temp != NULL) {
		if (temp->level == 2 || temp->level == -2)
			break;
		temp = temp->PNode;
	}
	if (temp != NULL) {
		if (temp->level == -2 && temp->LNode->level <= 0) {//SRR
			rightRotation(temp);
		}

		if (temp->level == -2 && temp->LNode->level > 0) {//DRR
			leftRotation(temp->LNode);
			rightRotation(temp);
		}

		if (temp->level == 2 && temp->RNode->level >= 0) {//SLR
			leftRotation(temp);
		}

		if (temp->level == 2 && temp->RNode->level < 0) {//DLR
			rightRotation(temp->RNode);
			leftRotation(temp);
		}
	}

	free(temp_D);		//delete node, so release

}


AVL *minmum(AVL *standard) {
	AVL *temp = standard;

	while (temp->LNode != NULL)
		temp = temp->LNode;
	return temp;
}

AVL *maximum(AVL *standard) {
	AVL *temp = standard;

	while (temp->LNode != NULL)
		temp = temp->RNode;
	return temp;
}

AVL *successor(AVL *preValue) {
	AVL *Ptemp = preValue->PNode, *temp = preValue;

	if (temp->RNode != NULL)
		return minmum(temp->RNode);

	while (Ptemp != NULL && temp == Ptemp->RNode) {
		temp = Ptemp;
		Ptemp = Ptemp->PNode;
	}

	return Ptemp;
}

int get_height(AVL *avl_root) { // Find the height of the tree
	int lHight, rHight;

	if (!avl_root)
		return 0;
	else {
		lHight = get_height(avl_root->LNode); // Get the height of the left subtree through a recursive call
		rHight = get_height(avl_root->RNode); // Get the height of the right subtree through a recursive call
		return 1 + (lHight > rHight ? lHight : rHight); // Return 1 with the greater of the two
	}
}

void set_Height(AVL *avl_root, int height) {

	if (avl_root->LNode != NULL) {
		avl_root->LNode->hight = height - 1;
		set_Height(avl_root->LNode, height - 1);
	}
	if (avl_root->RNode != NULL) {
		avl_root->RNode->hight = height - 1;
		set_Height(avl_root->RNode, height - 1);
	}
}

void set_Level(AVL *avl_root) {
	int lHight, rHight;

	lHight = get_height(avl_root->LNode);
	rHight = get_height(avl_root->RNode);

	avl_root->level = rHight - lHight;

	if (avl_root->LNode != NULL) {
		set_Level(avl_root->LNode);
	}
	if (avl_root->RNode != NULL) {
		set_Level(avl_root->RNode);
	}
}

void leftRotation(AVL *rotation) {
	AVL *parent = rotation->RNode;
	AVL *child = parent->LNode;
	int height;

	if (rotation->PNode == NULL) {
		parent->PNode = NULL;
		root = parent;
	}else {
		if (rotation->PNode->RNode == rotation) {
			rotation->PNode->RNode = parent;
			parent->PNode = rotation->PNode;
		}
		else {
			rotation->PNode->LNode = parent;
			parent->PNode = rotation->PNode;
		}
	}

	parent->LNode = rotation;
	rotation->PNode = parent;

	rotation->RNode = child;
	if(child != NULL){
		child->PNode = rotation;
	}

	root->hight = get_height(root);
	height = get_height(root);

	set_Height(root, height);
	set_Level(root);
}

void rightRotation(AVL *rotation) {
	AVL *parent = rotation->LNode;
	AVL *child = parent->RNode;
	int height;

	if (rotation->PNode == NULL) {
		parent->PNode = NULL;
		root = parent;
	}else {
		if (rotation->PNode->LNode == rotation) {
			rotation->PNode->LNode = parent;
			parent->PNode = rotation->PNode;
		}
		else {
			rotation->PNode->RNode = parent;
			parent->PNode = rotation->PNode;
		}
	}

	parent->RNode = rotation;
	rotation->PNode = parent;

	rotation->LNode = child;
	if(child != NULL){
		child->PNode = rotation;
	}

	root->hight = get_height(root);
	height = get_height(root);

	set_Height(root, height);
	set_Level(root);
}