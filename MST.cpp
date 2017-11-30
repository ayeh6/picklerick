#include <iostream>
#include <limits.h>
#include <stdlib.h>
using namespace std;

struct AdjListNode
{
	int destination;
	int weight;
	struct AdjListNode* next;
};

struct AdjList
{
	struct AdjListNode *head;
};

struct AdjListNode* newAdjListNode(int destination, int weight)
{
    struct AdjListNode* newNode = (struct AdjListNode*) malloc (sizeof(struct AdjListNode));
    newNode->destination = destination;
    newNode->weight = weight;
    newNode->next = NULL;
    return newNode;
}

struct Graph
{
	int V;
	struct AdjList* array;
};

struct Graph* newGraph(int V)
{
	struct Graph* graph = (struct Graph*) malloc (sizeof(struct Graph));
	graph->V = V;
	graph->array = (struct AdjList*) malloc (V * sizeof(struct AdjList));
	for(int i = 0; i < V; i++) {
		graph->array[i].head = NULL;
	}
	return graph;
}

void addEdge(struct Graph* graph, int source, int destination, int weight)
{
	struct AdjListNode* newNode = newAdjListNode(destination, weight);
	newNode->next = graph->array[source].head;
	graph->array[source].head = newNode;
	newNode = newAdjListNode(source, weight);
	newNode->next = graph->array[destination].head;
	graph->array[destination].head = newNode;
}

struct MinHeapNode
{
	int v;
	int key;
};

struct MinHeap
{
	int size;
	int capacity;
	int *position;
	struct MinHeapNode **array;
};

struct MinHeapNode* newMinHeapNode(int v, int key)
{
	struct MinHeapNode* minHeapNode = (struct MinHeapNode*)malloc(sizeof(struct MinHeapNode));
	minHeapNode->v = v;
	minHeapNode->key = key;
	return minHeapNode;
}

struct MinHeap* createMinHeap(int capacity)
{
	struct MinHeap* minHeap = (struct MinHeap*)malloc(sizeof(struct MinHeap));
	minHeap->position = (int *)malloc(capacity * sizeof(int));
	minHeap->size = 0;
	minHeap->capacity = capacity;
	minHeap->array = (struct MinHeapNode**)malloc(capacity * sizeof(struct MinHeapNode*));
	return minHeap;
}

void swap(struct MinHeapNode** x, struct MinHeapNode** y)
{
	struct MinHeapNode* temp = *x;
	*x = *y;
	*y = temp;
}

void minHeapify(struct MinHeap* minHeap, int index)
{
	int minNode = index, left = 2 * index + 1, right = 2 * index + 2;
	if((right < minHeap->size) && (minHeap->array[right]->key < minHeap->array[minNode]->key)) {
		minNode = right;
	}
	if((left < minHeap->size) && (minHeap->array[left]->key < minHeap->array[minNode]->key)) {
		minNode = left;
		}
	if(minNode != index) {
		MinHeapNode *smallestNode = minHeap->array[minNode];
		MinHeapNode *idxNode = minHeap->array[index];
		minHeap->position[smallestNode->v] = index;
		minHeap->position[idxNode->v] = minNode;
		swap(&minHeap->array[minNode], &minHeap->array[index]);
		minHeapify(minHeap, minNode);
	}
}

struct MinHeapNode* extractMin(struct MinHeap* minHeap) {
	if(minHeap->size == 0) {
		return NULL;
	}
	struct MinHeapNode* root = minHeap->array[0]; // Store the root node
	struct MinHeapNode* lastNode = minHeap->array[minHeap->size - 1];
	minHeap->array[0] = lastNode;
	minHeap->position[root->v] = minHeap->size-1;
	minHeap->position[lastNode->v] = 0;
	--minHeap->size;
	minHeapify(minHeap, 0);
	return root;
}

void decreaseKey(struct MinHeap* minHeap, int v, int key) {
	int i = minHeap->position[v];
	minHeap->array[i]->key = key;
	while((minHeap->array[(i - 1) / 2]->key) > (minHeap->array[i]->key)) {
		minHeap->position[minHeap->array[i]->v] = (i-1)/2;
		minHeap->position[minHeap->array[(i-1)/2]->v] = i;
		swap(&minHeap->array[i],  &minHeap->array[(i - 1) / 2]);
		i = (i - 1) / 2;
	}
}

void Prim_Algorithm(struct Graph* graph) {
	int i, V = graph->V, parent_arr[V], key[V];
 	struct MinHeap* minHeap = createMinHeap(V);
	for(i = 1; i < V; i++) {
		parent_arr[i] = -1;
        	key[i] = INT_MAX;
		minHeap->array[i] = newMinHeapNode(i, key[i]);
		minHeap->position[i] = i;
	}
	key[0] = 0;
	minHeap->array[0] = newMinHeapNode(0, key[0]);
	minHeap->position[0] = 0;
	minHeap->size = V;
	while(!(minHeap->size == 0)) {
		struct MinHeapNode* minHeapNode = extractMin(minHeap);
		int u = minHeapNode->v;
		struct AdjListNode* temp_arr = graph->array[u].head;
		while (temp_arr != NULL) {
			int v = temp_arr->destination;
			if((minHeap->position[v] < minHeap->size) && (temp_arr->weight < key[v])) {
				key[v] = temp_arr->weight;
				parent_arr[v] = u;
				decreaseKey(minHeap, v, key[v]);
			}
			temp_arr = temp_arr->next;
		}
	}
	for(i = 1; i < V; i++) {
	cout << parent_arr[i] <<endl;
	}
}

int main() {
	int i, numVertices, numEdges, u, v, weight;
	cin >> numVertices;
	struct Graph* graph = newGraph(numVertices);
	cin >> numEdges;
	for(i = 0; i < numEdges; i++) {
		cin >> u >> v >> weight;
		addEdge(graph, u, v, weight);
	}
	Prim_Algorithm(graph);
	return 0;
}
