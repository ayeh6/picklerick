#include <iostream>
#include <limits.h>
#include <stdlib.h>
using namespace std;

struct AdjListNode
{
	int dest;
	int weight;
	struct AdjListNode* next;

	AdjListNode(int dest, int weight)
	{
		this->dest = dest;
		this->weight = weight;
		this->next = NULL;
	}
};

struct AdjList
{
	struct AdjListNode *head;
};

struct Graph
{
	int V;
	struct AdjList* array;

	Graph(int V)
	{
		this->V = V;
		this->array = (struct AdjList*) malloc (V * sizeof(struct AdjList));
		for(int i = 0; i < V; i++)
		{
			this->array[i].head = NULL;
		}
	}
};

void addEdge(struct Graph* graph, int source, int destination, int weight)
{
	AdjListNode* newNode = new AdjListNode(destination, weight);
	newNode->next = graph->array[source].head;
	graph->array[source].head = newNode;
	newNode = new AdjListNode(source, weight);
	newNode->next = graph->array[destination].head;
	graph->array[destination].head = newNode;
}

struct MinHeapNode
{
	int v;
	int key;

	MinHeapNode(int v, int key)
	{
		this->v = v;
		this->key = key;
	}
};

struct MinHeap
{
	int size;
	int capacity;
	int *position;
	struct MinHeapNode **array;

	MinHeap(int capacity)
	{
		this->position = (int *)malloc(capacity * sizeof(int));
		this->size = 0;
		this->capacity = capacity;
		this->array = (struct MinHeapNode**)malloc(capacity * sizeof(struct MinHeapNode*));
	}
};

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
	MinHeapNode* root = minHeap->array[0]; // Store the root node
	MinHeapNode* lastNode = minHeap->array[minHeap->size - 1];
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
	while((minHeap->array[(i - 1) / 2]->key) > (minHeap->array[i]->key))
	{
		minHeap->position[minHeap->array[i]->v] = (i-1)/2;
		minHeap->position[minHeap->array[(i-1)/2]->v] = i;
		swap(&minHeap->array[i],  &minHeap->array[(i - 1) / 2]);
		i = (i - 1) / 2;
	}
}

void Prim_Algorithm(struct Graph* graph) {
	int i, V = graph->V, parent_arr[V], key[V];
 	MinHeap* minHeap = new MinHeap(V);
	for(i = 1; i < V; i++) {
		parent_arr[i] = -1;
        	key[i] = INT_MAX;
		minHeap->array[i] = new MinHeapNode(i, key[i]);
		minHeap->position[i] = i;
	}
	key[0] = 0;
	minHeap->array[0] = new MinHeapNode(0, key[0]);
	minHeap->position[0] = 0;
	minHeap->size = V;
	while(!(minHeap->size == 0)) {
		MinHeapNode* minHeapNode = extractMin(minHeap);
		int u = minHeapNode->v;
		AdjListNode* temp_arr = graph->array[u].head;
		while (temp_arr != NULL)
		{
			int v = temp_arr->dest;
			if((minHeap->position[v] < minHeap->size) && (temp_arr->weight < key[v]))
			{
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
	Graph* graph = new Graph(numVertices);
	cin >> numEdges;
	for(i = 0; i < numEdges; i++) {
		cin >> u >> v >> weight;
		addEdge(graph, u, v, weight);
	}
	Prim_Algorithm(graph);
	return 0;
}
