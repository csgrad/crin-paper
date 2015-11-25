/*
 * Author: Jason Ernst
 * Date Started: Tuesday December 11th, 2007
 * Hypergraph Implementation in C
 * 
 * Note: The pointer to the hypergraph must be set to NULL initially
 * to prevent segfaults since it works the same as LL
 */

//EdgeSet structure, has an id to identify the edge
//and a list of vertices which make up the edge
//also a list to the next edge in the structure
typedef struct vertex
{
  int id;         
  struct vertex* nextVertex; 
  llnode * edges; 
} vertexSet;

typedef struct edge
{
  int id;         
  struct edge * nextEdge; 
  llnode * vertices; 
} edgeSet;

typedef struct hgraph
{
  vertexSet * vertexSets;
  edgeSet * edgeSets;
} hypergraph;

hypergraph * loadHyperGraph(char * file);
hypergraph * newHyperGraph();
void displayHyperGraph(hypergraph * h, char * name);
bool isConnected(hypergraph * h);
void freeHyperGraph(hypergraph * h);

edgeSet * newEdge(int id);
edgeSet * appendEdgeSet(edgeSet **head, int id);
edgeSet * appendUniqueEdgeSet(edgeSet **head, int id);
int numEdges(edgeSet * head);
edgeSet * findEdge(edgeSet *head, int id);
edgeSet * copyEdgeSets(edgeSet * head);
void displayEdge(edgeSet *edge);
void displayEdges(edgeSet * head);
void freeEdgeSets(edgeSet * head);
bool removeEdgeSet(edgeSet **head, int id);
edgeSet * edgesContainingVertex(edgeSet * E, int v_id);

void removeEdgeFromVertices(vertexSet * head, int id);
void addEdgetoVertices(edgeSet * edge, vertexSet * head);

hypergraph * generateInducedSubgraph(hypergraph *G, edgeSet *N);
hypergraph * buildPartialGraph(hypergraph * H);
edgeSet * buildEdgesFromHyperEdge(edgeSet * hyperEdge);

vertexSet * newVertex(int id);
vertexSet * appendVertexSet(vertexSet **head, int id);
vertexSet * appendUniqueVertexSet(vertexSet **head, int id);
int numVertices(vertexSet * head);
vertexSet * findVertex(vertexSet *head, int id);
vertexSet * copyVertexSets(vertexSet * head);
void displayVertex(vertexSet *vertex);
void displayVertices(vertexSet * head);
void freeVertexSets(vertexSet * head);
bool removeVertexSet(vertexSet **head, int id);
bool equalVertices(vertexSet * v1, vertexSet * v2);
