#include <iostream>
#include <limits.h>
#include <stdlib.h>
using namespace std;

struct ListNode
{
	int dest;
	int weight;
	ListNode* next;

	ListNode(int dest, int weight)
	{
		this->dest = dest;
		this->weight = weight;
		this->next = NULL;
	}
};

struct List
{
	ListNode *head;
};

struct Graph
{
	int V;
	List* array;

	Graph(int V)
	{
		this->V = V;
		this->array = (List*) malloc (V * sizeof(List));
		for(int i = 0; i < V; i++)
		{
			this->array[i].head = NULL;
		}
	}
};

void addEdge(Graph* graph, int src, int dest, int weight)
{
	ListNode* nNode = new ListNode(dest, weight);
	nNode->next = graph->array[src].head;
	graph->array[src].head = nNode;
	nNode = new ListNode(src, weight);
	nNode->next = graph->array[dest].head;
	graph->array[dest].head = nNode;
}

struct GraphNode
{
	int v;
	int key;

	GraphNode(int v, int key)
	{
		this->v = v;
		this->key = key;
	}
};

struct MinHeapGraph
{
	int size;
	int capacity;
	int *pos;
	GraphNode **array;

	MinHeapGraph(int capacity)
	{
		this->pos = (int *)malloc(capacity * sizeof(int));
		this->size = 0;
		this->capacity = capacity;
		this->array = (GraphNode**)malloc(capacity * sizeof(GraphNode*));
	}
};

void swap(GraphNode** x, GraphNode** y)
{
	GraphNode* temp = *x;
	*x = *y;
	*y = temp;
}

void minHeapify(MinHeapGraph* minHeap, int index)
{
	int minNode = index;
	int left;
	int right;
	left = 2 * index + 1;
	right = 2 * index + 2;

	if((right < minHeap->size) && (minHeap->array[right]->key < minHeap->array[minNode]->key))
	{
		minNode = right;
	}
	if((left < minHeap->size) && (minHeap->array[left]->key < minHeap->array[minNode]->key))
	{
		minNode = left;
	}
	if(minNode != index)
	{
		GraphNode *smllNode = minHeap->array[minNode];
		GraphNode *idxNode = minHeap->array[index];
		minHeap->pos[smllNode->v] = index;
		minHeap->pos[idxNode->v] = minNode;
		swap(&minHeap->array[minNode], &minHeap->array[index]);
		minHeapify(minHeap, minNode);
	}
}

struct GraphNode* getMin(MinHeapGraph* minHeap)
{
	if(minHeap->size == 0)
	{
		return NULL;
	}
	GraphNode* root = minHeap->array[0];
	GraphNode* lastNode = minHeap->array[minHeap->size - 1];
	minHeap->array[0] = lastNode;
	minHeap->pos[root->v] = minHeap->size-1;
	minHeap->pos[lastNode->v] = 0;
	--minHeap->size;
	minHeapify(minHeap, 0);
	return root;
}

void DecreaseKey(MinHeapGraph* minHeap, int v, int key)
{
	int i = minHeap->pos[v];
	minHeap->array[i]->key = key;
	while((minHeap->array[(i - 1) / 2]->key) > (minHeap->array[i]->key))
	{
		minHeap->pos[minHeap->array[i]->v] = (i-1)/2;
		minHeap->pos[minHeap->array[(i-1)/2]->v] = i;
		swap(&minHeap->array[i],  &minHeap->array[(i - 1) / 2]);
		i = (i - 1) / 2;
	}
}

void Prims(Graph* graph)
{
	int V = graph->V;
	int parents[V], key[V];
 	MinHeapGraph* minHeap = new MinHeapGraph(V);

	for(int i = 0; i < V; i++)
	{
		parents[i] = -1;
        	key[i] = INT_MAX;
		minHeap->array[i] = new GraphNode(i, key[i]);
		minHeap->pos[i] = i;
	}

	key[0] = 0;
	minHeap->array[0] = new GraphNode(0, key[0]);
	minHeap->pos[0] = 0;
	minHeap->size = V;

	while(!(minHeap->size == 0))
	{
		GraphNode* minHeapNode = getMin(minHeap);
		int u = minHeapNode->v;
		ListNode* tempArray = graph->array[u].head;
		while (tempArray != NULL)
		{
			int v = tempArray->dest;
			if((minHeap->pos[v] < minHeap->size) && (tempArray->weight < key[v]))
			{
				key[v] = tempArray->weight;
				parents[v] = u;
				DecreaseKey(minHeap, v, key[v]);
			}
			tempArray = tempArray->next;
		}
	}

	for(int i = 1; i < V; i++)
	{
		cout << parents[i] << endl;
	}
}

int main() {
	int verts, edges, v1, v2, weight;
	cin >> verts;
	Graph* graph = new Graph(verts);
	cin >> edges;
	for(int i = 0; i < edges; i++)
	{
		cin >> v1;
		cin >> v2;
		cin >> weight;
		addEdge(graph, v1, v2, weight);
	}
	Prims(graph);
	return 0;
}
