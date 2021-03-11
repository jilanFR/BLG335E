#include <iostream>
#include "hw3.h"

using namespace std;

class Node {
public:
	char color; // 'R' or 'B'
	int key;
	int ordinal;
	Node* left;
	Node* right;
	Node* parent;
	Node() {
		left = right = parent = NULL;
		key = ordinal = -1;
	}
	Node(int k, char c, int o) {
		key = k;
		color = c;
		ordinal = o;
		left = right = parent = NULL;
	}
};

class RedBlackTree {
private:
	Node* root;
	int size = 0;
public:

	void insert(int newKey, int newOrdinal);

	void rbAdjustment(Node*);

	void remove(int);

	RedBlackTree();

	Node** getRoot() { return &root; }

	int getMinKey();
	int countKey(int key);

	int getSize() { return size; }

	int getRedNodeNumber();
};

void countKeyInTree(Node * traverse, int key, int* count) {

	if (traverse == NULL)
		return;

	if (traverse->key == key)
		* count = *count + 1;

	countKeyInTree(traverse->right, key, count);

	countKeyInTree(traverse->left, key, count);
}

int RedBlackTree::countKey(int key) {
	int count = 0;
	countKeyInTree(root, key, &count);
	return count;
}

void countRed(Node* traverse, int* count) {
	if (!traverse)
		return;
	if (traverse->color == 'R')
		(*count)++;
	countRed(traverse->right, count);
	countRed(traverse->left, count);
}

int RedBlackTree::getRedNodeNumber() {
	int count = 0;
	countRed(root, &count);
	return count;
}

int RedBlackTree::getMinKey() {
	if (!root)
		return -1; // means empty

	Node* traverse = root;

	while (traverse->left)
		traverse = traverse->left;

	return traverse->key;
}



void bstInsert(Node * *root, Node * newNode) {
	if (*root == NULL) {
		*root = newNode;
	}
	else {
		Node* traverse = *root;

		while (1) {
			if (newNode->key < traverse->key) {

				if (traverse->left) {
					traverse = traverse->left;
				}
				else {
					traverse->left = newNode;
					newNode->parent = traverse;
					break;
				}
			}
			else {
				if (traverse->right) {
					traverse = traverse->right;
				}
				else {
					traverse->right = newNode;
					newNode->parent = traverse;
					break;
				}
			}
		}
	}
}

Node* getUncle(Node * nodePtr) {
	if (nodePtr->parent && nodePtr->parent->parent) {
		if (nodePtr->parent->parent->left == nodePtr->parent) {
			return nodePtr->parent->parent->right;
		}
		else {
			return nodePtr->parent->parent->left;
		}
	}
	else {
		return NULL;
	}
}

void rightRotate(Node * *root, Node * g) {
	Node* left = g->left;

	g->left = left->right;
	if (left->right) {
		left->right->parent = g;
	}

	if (g->parent) {
		if (g == g->parent->left) {
			g->parent->left = left;
		}
		else {
			g->parent->right = left;
		}
	}
	else {
		*root = left;
	}
	left->parent = g->parent;

	left->right = g;
	g->parent = left;
}

void leftRotate(Node * *root, Node * g) {
	Node* right = g->right;

	g->right = right->left;
	if (right->left) {
		right->left->parent = g;
	}

	if (g->parent) {
		if (g == g->parent->left) {
			g->parent->left = right;
		}
		else {
			g->parent->right = right;
		}

	}
	else {
		*root = right;
	}
	right->parent = g->parent;

	right->left = g;
	g->parent = right;
}

Node* findNode(Node * *root, int key) {
	Node* traverse = *root;

	if (traverse == NULL) {
		return NULL;
	}
	while (traverse) {
		if (traverse->key == key) {
			return traverse;
		}
		else if (traverse->key < key) {
			traverse = traverse->right;
		}
		else {
			traverse = traverse->left;
		}
	}
	return NULL;
}

void iterateTree(RedBlackTree * treePtr, Node * traverse) {

	if (traverse == NULL)
		return;

	treePtr->insert(traverse->key, traverse->ordinal);

	iterateTree(treePtr, traverse->right);

	iterateTree(treePtr, traverse->left);

}

void regenerateRBTree(Node * *root) {
	RedBlackTree tree;

	iterateTree(&tree, *root);

	*root = *tree.getRoot();
}

void bstDelete(Node * *root, int key) {

	Node* target = findNode(root, key);

	if (target == NULL) {
		cout << "couldnt find key to delete\n";
		return;
	}

	if (target->left == NULL && target->right == NULL) {
		if (target == *root) {
			*root = NULL;
			delete target;
			return;
		}
		if (target == target->parent->left) {
			target->parent->left = NULL;
		}
		else {
			target->parent->right = NULL;
		}

		// temporary solution, i will fix this if i can understand the deletion
		if (target->color == 'B') {
			// target is leaf
			// both u and v are black case
			regenerateRBTree(root);
		}

		delete target;
	}
	else if (target->left == NULL) {
		if (target == *root) {
			target->right->parent = target->parent;
			*root = target->right;
			target->right->color = 'B';
			delete target;
			return;
		}
		if (target == target->parent->left) {
			target->parent->left = target->right;
		}
		else {
			target->parent->right = target->right;
		}
		target->right->parent = target->parent;

		if (target->color == 'R' || target->right->color == 'R') {
			target->right->color = 'B';
		}
		else {
			// both u and v are black case
			regenerateRBTree(root);
		}

		delete target;
	}
	else if (target->right == NULL) {
		if (target == *root) {
			target->left->parent = target->parent;
			*root = target->left;
			target->left->color = 'B';
			delete target;
			return;
		}
		if (target == target->parent->left) {
			target->parent->left = target->left;
		}
		else {
			target->parent->right = target->left;
		}
		target->left->parent = target->parent;

		if (target->color == 'R' || target->left->color == 'R') {
			target->left->color = 'B';
		}
		else {
			// both u and v are black case
			regenerateRBTree(root);
		}

		delete target;
	}
	else {
		Node* inorderSuccessor = target->right;
		while (inorderSuccessor->left) {
			inorderSuccessor = inorderSuccessor->left;
		}

		target->key = inorderSuccessor->key;
		target->ordinal = inorderSuccessor->ordinal;

		if (inorderSuccessor == inorderSuccessor->parent->left) {
			inorderSuccessor->parent->left = inorderSuccessor->right;
		}
		else {
			inorderSuccessor->parent->right = inorderSuccessor->right;
		}
		if (inorderSuccessor->right) {
			inorderSuccessor->right->parent = inorderSuccessor->parent;

			if (inorderSuccessor->color == 'R' || inorderSuccessor->right->color == 'R') {
				inorderSuccessor->right->color = 'B';
			}
			else {
				// both u and v are black case
				regenerateRBTree(root);
			}
		}
		else {
			// inorderSuccessor is leaf
			// both u and v are black case
			regenerateRBTree(root);
		}
		delete inorderSuccessor;
	}
}


void RedBlackTree::rbAdjustment(Node * newNode) {
	if (newNode == root) {
		newNode->color = 'B';
	}
	else if (newNode->parent->color == 'R') {
		if (getUncle(newNode) && getUncle(newNode)->color == 'R') {
			newNode->parent->color = 'B';
			getUncle(newNode)->color = 'B';
			newNode->parent->parent->color = 'R';
			rbAdjustment(newNode->parent->parent);
		}
		else {
			Node* p = newNode->parent;
			Node* g = p->parent;

			if (p == g->left && newNode == p->left) {
				rightRotate(&root, g);
				p->color = 'B';
				g->color = 'R';
			}
			else if (p == g->left && newNode == p->right) {
				leftRotate(&root, p);
				rightRotate(&root, g);
				newNode->color = 'B';
				g->color = 'R';
			}
			else if (p == g->right && newNode == p->right) {
				leftRotate(&root, g);
				p->color = 'B';
				g->color = 'R';
			}
			else if (p == g->right && newNode == p->left) {
				rightRotate(&root, p);
				leftRotate(&root, g);
				newNode->color = 'B';
				g->color = 'R';
			}
		}
	}
}

void RedBlackTree::insert(int newKey, int newOrdinal) {
	Node* newNode = new Node(newKey, 'R', newOrdinal);
	bstInsert(&(this->root), newNode);
	rbAdjustment(newNode);
	size++;
}

void RedBlackTree::remove(int i) {
	bstDelete(&(this->root), i);
	size--;
}

RedBlackTree::RedBlackTree() {
	root = NULL;
}

void populateTree(RedBlackTree * tree, int count,
	int sizes[],
	int ordinals[]) {
	for (int i = 0; i < count; i++) {
		tree->insert(sizes[i], ordinals[i]);
	}
}

int findSmallestOrdinal(Node * packages, int size) {
	int smallestOrdinal = packages[0].ordinal;
	for (int i = 1; i < size; i++) {
		if (packages[i].ordinal < smallestOrdinal)
			smallestOrdinal = packages[i].ordinal;
	}
	return smallestOrdinal;
}


int smallestSizeShipment(RedBlackTree * sourceTree, Node * packages, int* smallestOrdinal) {
	int minSize = sourceTree->getMinKey();
	int countMinSize = sourceTree->countKey(minSize);

	for (int i = 0; i < countMinSize; i++) {
		Node* target = findNode((sourceTree->getRoot()), minSize);
		Node newNode(target->key, target->color, target->ordinal);
		sourceTree->remove(minSize);
		packages[i] = newNode;
	}

	*smallestOrdinal = findSmallestOrdinal(packages, countMinSize);
	return countMinSize;
}

int nSizeShipment(RedBlackTree * sourceTree, Node * packages, int* smallestOrdinal, int nthSmallest) {
	int countMinSize = sourceTree->countKey(nthSmallest);

	for (int i = 0; i < countMinSize; i++) {
		Node* target = findNode((sourceTree->getRoot()), nthSmallest);
		Node newNode(target->key, target->color, target->ordinal);
		sourceTree->remove(nthSmallest);
		packages[i] = newNode;
	}

	*smallestOrdinal = findSmallestOrdinal(packages, countMinSize);

	return countMinSize;
}

int findNthNode(Node* traverse, int* counter, int n){

	if (!traverse)
		return -1;

	int left = findNthNode(traverse->left, counter, n);
	
	if (left != -1)
		return left;

	(*counter)++;

	if ((*counter) == n)
		return traverse->key;
	
	return findNthNode(traverse->right, counter, n);
}

int findNthSmallestNode(RedBlackTree * tree, int n) {
	int counter = 0;
	return findNthNode(*(tree->getRoot()), &counter, n);
}

void insertPackages(Node * packages, RedBlackTree * destination, int packagesCount) {
	for (int i = 0; i < packagesCount; i++) {
		destination->insert(packages[i].key, packages[i].ordinal);
	}
}

void leadCourier(RedBlackTree * eastTree, RedBlackTree * westTree, int totalPackages) {
	Node* packages = new Node[totalPackages];
	int packagesCount;
	int smallestOrdinal;

	bool firstShipment = true;
	bool lastShipment = false;
	RedBlackTree* source = eastTree;
	RedBlackTree* destination = westTree;

	Node* packages2 = new Node[totalPackages];
	int smallestOrdinal2;
	int packagesCount2;

	while (true) {
		if (firstShipment) {
			packagesCount = smallestSizeShipment(source, packages, &smallestOrdinal);
		}
		else {
			if (smallestOrdinal > source->getSize()) {
				packagesCount = smallestSizeShipment(source, packages, &smallestOrdinal);
			}
			else {
				int nthSmallest = findNthSmallestNode(source, smallestOrdinal);
				packagesCount2 = nSizeShipment(source, packages2, &smallestOrdinal2, nthSmallest);
				insertPackages(packages, source, packagesCount);

				packages = packages2;
				smallestOrdinal = smallestOrdinal2;
				packagesCount = packagesCount2;
			}
		}

		if (source->getSize() == 0) {
			lastShipment = true;
		}

		if (lastShipment) {
			insertPackages(packages, destination, packagesCount);
			break;
		}

		if (destination->getSize() >= smallestOrdinal) {
			int nthSmallest = findNthSmallestNode(destination, smallestOrdinal);
			packagesCount2 = nSizeShipment(destination, packages2, &smallestOrdinal2, nthSmallest);
			insertPackages(packages, destination, packagesCount);

			packages = packages2;
			smallestOrdinal = smallestOrdinal2;
			packagesCount = packagesCount2;

			/*RedBlackTree* temp = source;
			source = destination;
			destination = temp;*/
		}
		else {
			packagesCount = smallestSizeShipment(destination, packages2, &smallestOrdinal2);
		}
		firstShipment = false;

		if (destination->getSize() == 0) {
			lastShipment = true;
		}

		if (lastShipment) {
			insertPackages(packages, source, packagesCount);
			break;
		}
	}

}


HW3_Result hw3(int eastWarehousePackageCount,
	int eastWarehousePackageSizes[],
	int eastWarehousePackageOrdinals[],
	int westWarehousePackageCount,
	int westWarehousePackageSizes[],
	int westWarehousePackageOrdinals[]) {

	HW3_Result result;

	RedBlackTree eastTree;
	RedBlackTree westTree;

	populateTree(&eastTree, eastWarehousePackageCount, eastWarehousePackageSizes, eastWarehousePackageOrdinals);
	populateTree(&westTree, westWarehousePackageCount, westWarehousePackageSizes, westWarehousePackageOrdinals);

	leadCourier(&eastTree, &westTree, eastWarehousePackageCount + westWarehousePackageCount);

	if (eastTree.getSize() == 0) {
		result.packageCount = westTree.getSize();
		result.blackNodeCount = westTree.getSize() - westTree.getRedNodeNumber();
		result.redNodeCount = westTree.getRedNodeNumber();
	} else {
		result.packageCount = eastTree.getSize();
		result.blackNodeCount = eastTree.getSize() - eastTree.getRedNodeNumber();
		result.redNodeCount = eastTree.getRedNodeNumber();
	}

	return result;
}

