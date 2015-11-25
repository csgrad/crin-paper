/*
 * Author: Jason Ernst
 * Date Started: Tuesday December 11th, 2007
 * HyperGraph functions
 * 
 * Note: The pointer to the hypergraph must be set to NULL initially
 * to prevent segfaults since it works the same as LL
 */

#include "ll.c"
#include "hypergraph.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/*
 * Allocates and Initializes a new hypergraph
 */
hypergraph * newHyperGraph()
{
  hypergraph * tmp = NULL;
  
  if ((tmp = (hypergraph*)malloc(sizeof(*tmp))) == NULL)
  {
    printf("Error allocating memory for the hypergraph, program ending.");
    exit(0);
  }
  
  tmp->vertexSets = NULL;
  tmp->edgeSets = NULL;
  
  return tmp;
}

/*
 * This function builds a partially connected graph from a given hypergraph.
 * It is similar to a fully connected graph because it is a naiive algorithm
 * however it is considered better because only each individual hyperedge is
 * connected
 */
hypergraph * buildPartialGraph(hypergraph * H)
{
  hypergraph * G = newHyperGraph();
  G->vertexSets = copyVertexSets(H->vertexSets);
  
  //erase the edges associated with each vertex
  vertexSet * currentVertex = G->vertexSets;
  while(currentVertex != NULL)
  {
    currentVertex->edges = NULL;
    currentVertex = currentVertex->nextVertex;
  }
  
  //traverse each edge in H, making sure to make each fully connected
  //when adding it to G
  edgeSet * currentEdge = H->edgeSets;
  int edges =  numEdges(H->edgeSets) + 1;
  
  //Loop through each edge in H generating the eBar edges for each
  while(currentEdge!=NULL)
  {
    edgeSet * eBars = buildEdgesFromHyperEdge(currentEdge);
    
    //Loop through each of the generated eBar edges and add them to G
    while(eBars!=NULL)
    {
      edgeSet * currentG = G->edgeSets;
      bool found = false;
      while(currentG!=NULL)
      {
        if(equal_list(eBars->vertices, currentG->vertices))
          found = true;
        currentG = currentG->nextEdge;
      }
      
      //only add the edge if it is unique
      if(!found)
      {  
        edgeSet * newEdge = appendEdgeSet(&G->edgeSets, edges);
        newEdge->vertices = copyList(eBars->vertices);
        addEdgetoVertices(newEdge, G->vertexSets);
        edges++;
      }
      
      //advance to the next eBar
      eBars = eBars->nextEdge;
    }    
    currentEdge = currentEdge->nextEdge;
  }
  return G;
}

/*
 * Creates a set of edges generated by every possible combination of edges
 * in the given hyperEdge
 */
edgeSet * buildEdgesFromHyperEdge(edgeSet * hyperEdge)
{
  int numEdges = 0;
  edgeSet * newEdges = NULL;
  
  llnode * u = hyperEdge->vertices;
  while(u!=NULL)
  {
    llnode * v = u->next;
    while(v!=NULL)
    {
      edgeSet * newEdge = appendUniqueEdgeSet(&newEdges, numEdges);
      appendUnique(&newEdge->vertices, u->data);
      appendUnique(&newEdge->vertices, v->data);
      numEdges++;
      v=v->next;
    }
    u=u->next;
  }
  
  return newEdges;
}

/*
 * Copies an existing hypergraph
 */
hypergraph * copyHyperGraph(hypergraph * H)
{
  hypergraph * G = newHyperGraph();
  G->edgeSets = copyEdgeSets(H->edgeSets);
  G->vertexSets = copyVertexSets(H->vertexSets);
  return G;
}

/*
 * Allocates and Initializes a new edgeSet
 */
edgeSet * newEdge(int id)
{
  edgeSet * tmp = NULL;
  
  if ((tmp = (edgeSet*)malloc(sizeof(*tmp))) == NULL)
  {
    printf("Error allocating memory for the hypergraph, program ending.");
    exit(0);
  }
  
  tmp->id = id;
  tmp->nextEdge = NULL;
  tmp->vertices = NULL;
  
  return tmp;
}

/*
 * Append edgeSet at end of the edgeSet list, returns a pointer 
 * to the edge just added
 */
edgeSet * appendEdgeSet(edgeSet **head, int id)
{
  if((*head)==NULL)
  {    
    (*head) = newEdge(id);
    return (*head);
  }
  else
  {
    return appendEdgeSet(&(*head)->nextEdge, id);
  }
}

/*
 * Append unique edgeSet at end of the edgeSet list, returns a pointer
 * to the edge just added
 */
edgeSet * appendUniqueEdgeSet(edgeSet **head, int id)
{
  if(findEdge(*head, id) == NULL)
    return appendEdgeSet(head, id);
  else
    return NULL;
}

/*
 * Copies every edge from head on and returns the head of
 * the new list of edges
 */
edgeSet * copyEdgeSets(edgeSet * head)
{
  edgeSet * newHead = NULL;
  edgeSet * currentEdge = NULL;
  
  while(head != NULL)
  {
    currentEdge = appendUniqueEdgeSet(&newHead, head->id);
    if(currentEdge != NULL)
    {
      currentEdge->vertices = copyList(head->vertices);
      head = head->nextEdge;
    }
    else
    {
      printf("Error copying edge. Program ending.\n");
      exit (0);
    }
  }
  return newHead;
}

/*
 * Returns the number of edges from head on
 */
int numEdges(edgeSet * head)
{
  if(head == NULL)
    return 0;
  else
    return 1 + numEdges(head->nextEdge);
}

/*
 * Recursive function to find an edge in the list and return it
 * returns null if not found
 */
edgeSet * findEdge(edgeSet *head, int id)
{
  //Loop through the list looking for a match
  while (head != NULL)
  {
    if(head->id == id)
    {
      return head;
    }
    else
    {
      return findEdge(head->nextEdge, id);
    }
  }
  return NULL;
}

/*
 * displays a single edge and all the vertices associated with it
 */
void displayEdge(edgeSet *edge)
{
  if(edge!=NULL)
  {
    printf("E%d: ", edge->id);
    displayListAsSet(edge->vertices);
  }
  else
    printf("NULL EDGE\n");
}

/*
 * displays every edge and the associated vertices starting at head
 */
void displayEdges(edgeSet * head)
{
  if(head!=NULL)
  {
    displayEdge(head);
    displayEdges(head->nextEdge);
  }
}

/*
 * frees the memory allocated for an edgeSet
 */
void freeEdgeSets(edgeSet * head)
{
  edgeSet * tmp;
  
  while(head != NULL)
  {
    tmp = head->nextEdge;
    freeList(head->vertices);
    free(head);
    head = tmp;
  }
}

/*
 * Returns a complete listing of every edge in E
 * containing v_id or NULL if not found
 */
edgeSet * edgesContainingVertex(edgeSet * E, int v_id)
{
  edgeSet * edgesToReturn = NULL;
  //Loop through E looking for edges containing v_id
  while(E!=NULL)
  {
    //When found create a new edge so it is returned all by itself
    if(findNode(E->vertices, v_id)!=NULL)
    {
      edgeSet * tmp = appendUniqueEdgeSet(&edgesToReturn, E->id);
      if(tmp != NULL)
      {
        tmp->vertices = copyList(E->vertices);
        tmp->nextEdge = NULL;
      }
      else
        printf("Error searching edges for v_id\n");
    }
    E=E->nextEdge;
  }
  return edgesToReturn;
}

/*
 * Returns a complete listing of every edge in E
 * containing {u_id, v_id} in proper order
 */
edgeSet * edgesContainingUV(edgeSet * E, int u_id, int v_id)
{
  edgeSet * edgesToReturn = NULL;
  //Loop through E looking for edges containing v_id
  while(E!=NULL)
  {
    //When found create a new edge so it is returned all by itself
    if(E->vertices->data == u_id && E->vertices->next->data == v_id)
    {
      edgeSet * tmp = appendUniqueEdgeSet(&edgesToReturn, E->id);
      if(tmp != NULL)
        tmp->vertices = copyList(E->vertices);
      else
        printf("Error searching edges for v_id\n");
    }
    E=E->nextEdge;
  }
  return edgesToReturn;
}

/*
 * Removes the edge id if it exists
 */
bool removeEdgeSet(edgeSet **head, int id)
{
  edgeSet *tmp;
  
  if( (*head) != NULL)
  {
    //head to be removed
    if( (*head)->id == id)
    {
      tmp = (*head)->nextEdge;
      freeList((*head)->vertices);
      free(*head);
      *head = tmp;
      return true;
    }
    //move to next item in list
    else
    {
      return removeEdgeSet(&(*head)->nextEdge, id );
    }
  }
  else
  {
    return false;
  }
}

/*
 * updates the vertex list when and edge is removed so that
 * the vertex list contains the proper new edges for the 
 * corresponding vertices
 */
void removeEdgeFromVertices(vertexSet * head, int id)
{
  while(head!=NULL)
  {
    removeNode(&head->edges, id);
    head = head->nextVertex;
  }
}

/*
 * updates the vertex list when a new edge is added so that
 * the vertex list contains the proper new edges for the 
 * corresponding vertices
 */
void addEdgetoVertices(edgeSet * edge, vertexSet * head)
{
  llnode * currentVertex = edge->vertices;
  vertexSet * headOfList= head;
  
  while(currentVertex != NULL)
  {
    vertexSet * updateVertex = findVertex(headOfList, currentVertex->data);
    if(updateVertex != NULL)
    {
      appendUnique(&updateVertex->edges, edge->id);
    }
    else
    {
      printf("Error, vertex V%d not found, cannot add edge\n", currentVertex->data);
    }
    currentVertex = currentVertex->next;
  }
}

/*
 * Allocates and Initializes a new vertexSet
 */
vertexSet * newVertex(int id)
{
  vertexSet * tmp = NULL;
  
  if ((tmp = (vertexSet*)malloc(sizeof(*tmp))) == NULL)
  {
    printf("Error allocating memory for the hypergraph, program ending.");
    exit(0);
  }
  
  tmp->id = id;
  tmp->nextVertex = NULL;
  tmp->edges = NULL;
  
  return tmp;
}

/*
 * Append vertexSet at end of the vertexSet list
 */
vertexSet * appendVertexSet(vertexSet **head, int id)
{
  if((*head)==NULL)
  {    
    (*head) = newVertex(id);
    return (*head);
  }
  else
  {
    return appendVertexSet(&(*head)->nextVertex, id);
  }
}

/*
 * Append unique vertexSet at end of the vertexSet list
 */
vertexSet * appendUniqueVertexSet(vertexSet **head, int id)
{
  if(findVertex(*head, id) == NULL)
    return appendVertexSet(head, id);
  else
    return NULL;
}

/*
 * Returns the number of edges from head on
 */
int numVertices(vertexSet * head)
{
  if(head == NULL)
    return 0;
  else
    return 1 + numVertices(head->nextVertex);
}

/*
 * Recursive function to find a vertex in the list and return it
 * returns null if not found
 */
vertexSet * findVertex(vertexSet *head, int id)
{
  //Loop through the list looking for a match
  while (head != NULL)
  {
    if(head->id == id)
    {
      return head;
    }
    else
    {
      return findVertex(head->nextVertex, id);
    }
  }
  return NULL;
}

/*
 * Copies every edge from head on and returns the head of
 * the new list of edges
 */
vertexSet * copyVertexSets(vertexSet * head)
{
  vertexSet * newHead = NULL;
  vertexSet * currentVertex = NULL;
  
  while(head != NULL)
  {
    currentVertex = appendUniqueVertexSet(&newHead, head->id);
    if(currentVertex != NULL)
    {
      currentVertex->edges = copyList(head->edges);
      head = head->nextVertex;
    }
    else
    {
      printf("Error copying vertex. Program ending.\n");
      exit (0);
    }
  }
  return newHead;
}

/*
 * displays a single vertex and all the edges associated with it
 */
void displayVertex(vertexSet *vertex)
{
  if(vertex!=NULL)
  {
    printf("V%d: ", vertex->id);
    displayListAsSet(vertex->edges);
  }
  else
    printf("NULL EDGE\n");
}

/*
 * displays every vertex and the associated edges starting at head
 */
void displayVertices(vertexSet * head)
{
  if(head!=NULL)
  {
    displayVertex(head);
    displayVertices(head->nextVertex);
  }
}

/*
 * frees the memory allocated for an vertexSet
 */
void freeVertexSets(vertexSet * head)
{
  vertexSet * tmp;
  
  while(head != NULL)
  {
    tmp = head->nextVertex;
    freeList(head->edges);
    free(head);
    head = tmp;
  }
}

/*
 * Removes the edge id if it exists
 */
bool removeVertexSet(vertexSet **head, int id)
{
  vertexSet *tmp;
  
  if( (*head) != NULL)
  {
    //head to be removed
    if( (*head)->id == id)
    {
      tmp = (*head)->nextVertex;
      freeList((*head)->edges);
      free(*head);
      *head = tmp;
      return true;
    }
    //move to next item in list
    else
    {
      return removeVertexSet(&(*head)->nextVertex, id );
    }
  }
  else
  {
    return false;
  }
}

/*
 * Returns true if v1 and v2 have the same edge sets
 */
bool equalVertices(vertexSet * v1, vertexSet * v2)
{
  llnode * v1_list = v1->edges;
  while(v1_list!=NULL)
  {
    if(findNode(v2->edges, v1_list->data)==NULL)
      return false;
    v1_list = v1_list->next;
  }
  
  llnode * v2_list = v2->edges;
  while(v2_list!=NULL)
  {
    if(findNode(v1->edges, v2_list->data)==NULL)
      return false;
    v2_list = v2_list->next;
  }
  
  return true;
}

/*
 * Loads a hypergraph from an input file
 * Format of the file:
 * - A list of the vertices in each edge separated by commas with
 *   each edge on a new line
 */
hypergraph * loadHyperGraph(char * file)
{
  //Initialize the hypergraph
  hypergraph * tmp = newHyperGraph();
  edgeSet * currentEdgeSet=NULL;
  vertexSet * currentVertexSet=NULL;
  int numEdges=0;
  
  //Open the file
  FILE * input_file = fopen(file, "r");
  
  char line[1024];//buffer for input
  int comma_position, v_id, placeholder, newlen, counter;
  
  if(input_file == NULL)
  {
    printf("Error opening input file, closing program");
    exit(0);
  }
  
  //Read in one line at a time
  while(fscanf(input_file, "%s", line) != EOF)
  { 
    //add a new edge to the graph
    numEdges++;
    currentEdgeSet = appendUniqueEdgeSet(&tmp->edgeSets, numEdges);
       
    //Grab each vertex off line the line one at a time separated by commas
    while(strlen(line) > 0)
    {      
      comma_position = 0; //reset comma position for new line
      //Determine location of comma in the string
      for(counter=0;counter<strlen(line);counter++)
      {
        if(line[counter] == ',')
        {
          comma_position = counter;
          break;
        }
      }
      //At end of line we have no comma
      if (comma_position == 0)
        comma_position = strlen(line);
      //copy the vertex number into a temp string
      char temp[1024];
      strncpy(temp, line, comma_position);
      temp[comma_position] = '\0';
      
      //adjust for v_id > 9  
      placeholder = 0;
      v_id = 0;    
      for(counter=strlen(temp)-1; counter>=0; counter--)
      {
        v_id = v_id + ( pow(10, placeholder) * ( (int)temp[counter] - 48) );
        placeholder ++;
      }
      
      //modify the line to remove the last vertex removed from it
      newlen = strlen(line) - comma_position;
      strncpy(line, &line[comma_position+1], newlen);
      line[newlen] = '\0';
      
      //Add the vertex to the current edge
      appendUnique(&currentEdgeSet->vertices, v_id);
      
      //Add the current edge to the vertex
      currentVertexSet = appendUniqueVertexSet(&tmp->vertexSets, v_id);
      //if the vertex is new this will not be null
      if(currentVertexSet != NULL)
        appendUnique(&currentVertexSet->edges, currentEdgeSet->id);
      //if the vertex has already been added to the hypergrah then we
      //must find it to add the edge to the vertex
      else
      {
        currentVertexSet = findVertex(tmp->vertexSets, v_id);
        if(currentVertexSet != NULL)
          appendUnique(&currentVertexSet->edges, currentEdgeSet->id);
      }
    }
  }
  //Close the file
  fclose(input_file);
  
  return tmp;
}

/*
 * displays the edges and vertices of a hyper graph
 */
void displayHyperGraph(hypergraph * h, char * name)
{
  if(h!=NULL)
  {
    printf("EdgeSets of %s:\n", name);
    displayEdges(h->edgeSets);
    printf("-----------------------------------\n");
    printf("VertexSets of %s:\n", name);
    displayVertices(h->vertexSets);
    printf("-----------------------------------\n");
    printf("Number of edges of %s: %d Number of vertices of %s: %d\n", name, numEdges(h->edgeSets), name, numVertices(h->vertexSets));
    printf("===================================\n");
  }
  else
    printf("NULL GRAPH\n");
}

/*
 * frees the memory allocated for a hypergraph
 */
void freeHyperGraph(hypergraph * h)
{
  freeEdgeSets(h->edgeSets);
  freeVertexSets(h->vertexSets);
}

/* 
 * Returns true if the given graph is connected
 */
bool isConnected(hypergraph * h)
{
  vertexSet * container = copyVertexSets(h->vertexSets);
  
  edgeSet * head = h->edgeSets;
  
  while(head != NULL)
  {
    llnode * vertices = head->vertices;
    
    while(vertices != NULL)
    {
      vertexSet * v = findVertex(container, vertices->data);
      if(v!=NULL)
        removeVertexSet(&container, v->id);
      vertices = vertices->next;
    }
    head = head->nextEdge;
  }
  
  if(container == NULL)
    return true;
  else
    return false;
}

/*
 * Returns the amount of times the edge u and v are covered in
 * a given hypergraph
 */
int degree(hypergraph *h, int u, int v)
{
  int count = 0;
  edgeSet * N = h->edgeSets;
  
  while(N!=NULL)
  {
    if(findNode(N->vertices, u)!=NULL)
      if(findNode(N->vertices, v)!=NULL)
        count++;
        
    N = N->nextEdge;
  }
  return count;
}

/*
 * Returns a new sorted hypergraph based on the non-sorted hypergraph provided by H
 * (improve runtime on this)
 */
hypergraph * sortHyperGraph(hypergraph * H)
{
  hypergraph * sortedGraph = newHyperGraph();
  
  //copy the vertices from the old graph to new graph
  vertexSet * currentVertex = H->vertexSets;
  while(currentVertex != NULL)
  {
    appendUniqueVertexSet(&sortedGraph->vertexSets, currentVertex->id);
    currentVertex = currentVertex->nextVertex;
  }
  
  int numEdges = 0;
  
  //Add the lowest edge from H to the sortedGraph,
  //each time removing the same edge from H until
  //there are no more edges in H
  while(H->edgeSets != NULL)
  {
    edgeSet * currentEdge = H->edgeSets;
    edgeSet * lowestEdge = currentEdge;
    
    //Go through each edge looking for the lowest
    while(currentEdge != NULL)
    {
      //if the current edge is lower, it is the new lowest
      if(currentEdge->id < lowestEdge->id)
      {
        lowestEdge = currentEdge;
      }
      currentEdge = currentEdge->nextEdge;
    }
    
    //After we have found the lowest edge we must add
    //it to sortedGraph and remove it from H
    
    //add new edge
    numEdges++;
    edgeSet * newEdge = appendUniqueEdgeSet(&sortedGraph->edgeSets, numEdges);
    newEdge->vertices = copyList(lowestEdge->vertices);
    addEdgetoVertices(newEdge, sortedGraph->vertexSets);
    
    //remove edge from H
    removeEdgeFromVertices(H->vertexSets, lowestEdge->id);
    removeEdgeSet(&H->edgeSets, lowestEdge->id);
  }
  
  
  return sortedGraph;
}

/*
 * Generate an induced subgraph of G
 * - Starts with the edge specified by N
 * - Selects all the vertices which are associated with the edge
 * - Determines which edges should be part of the graph based on 
 * - which vertices have been selected and which edges connect them
 * - returns the new graph which is an induced subgraph of G (hopefully)
 */
hypergraph * generateInducedSubgraph(hypergraph *G, edgeSet *N)
{
  hypergraph * tmp = newHyperGraph();
  vertexSet * tmp_vertex = NULL;
  edgeSet * tmp_edge=NULL;
  
  if(N!=NULL)
  {
    //add the vertices to the new graph
    llnode * vertices = N->vertices;
    while(vertices != NULL)
    {
      //Add the vertex into the vertexSet of the new graph
      tmp_vertex = appendUniqueVertexSet(&tmp->vertexSets, vertices->data);
      //advance to next vertex
      vertices = vertices->next;
    }
    
    edgeSet * E = G->edgeSets;
    
    while(E!=NULL)
    {
      bool bad_edge = false;
      llnode * tmp_list = E->vertices;
      //traverse the vertice list of the edge looking for vertices
      //which are not in the induced subgraph
      while(tmp_list != NULL)
      {
        //found a bad egdge
        if(findVertex(tmp->vertexSets, tmp_list->data)==NULL)
        {
          bad_edge = true;
        }
        tmp_list = tmp_list->next;
      }
      //if no bad edges are found we are safe to add the edge to
      //the induced subgraph
      if(!bad_edge)
      {
        tmp_edge = appendUniqueEdgeSet(&tmp->edgeSets, E->id);
        tmp_edge->vertices = copyList(E->vertices);
        addEdgetoVertices(tmp_edge, tmp->vertexSets);
      }
      E = E->nextEdge;
    }
  }
  else
    printf("Error creating induced graph because N is NULL\n");
  return tmp;
}

/*
 * testing function
 *
int main()
{
  hypergraph * H = loadHyperGraph("hypergraph.dat");
  hypergraph * G = newHyperGraph();
  displayHyperGraph(H, "H");
  
  
  removeEdgeSet(&H->edgeSets, 1);
  removeEdgeFromVertices(H->vertexSets, 1);
  removeEdgeSet(&H->edgeSets, 2);
  removeEdgeFromVertices(H->vertexSets, 2);
  removeEdgeSet(&H->edgeSets, 3);
  removeEdgeFromVertices(H->vertexSets, 3);
  removeEdgeSet(&H->edgeSets, 9);
  removeEdgeFromVertices(H->vertexSets, 9);
  removeEdgeSet(&H->edgeSets, 10);
  removeEdgeFromVertices(H->vertexSets, 10);
  removeEdgeSet(&H->edgeSets, 11);
  removeEdgeFromVertices(H->vertexSets, 11);
  
  
  printf("%d\n", isConnected(H));
  
  llnode * vertices=NULL;
  appendUnique(&vertices, 5);
  appendUnique(&vertices, 8);
  
  edgeSet * E = edgeWithEqualVertices(H->edgeSets, vertices);
  displayEdges(E);
  
  G->edgeSets = copyEdgeSets(H->edgeSets);
  G->vertexSets = copyVertexSets(H->vertexSets);
  displayHyperGraph(G, "G");
  
  freeHyperGraph(H);
  freeHyperGraph(G);
  
  return (0);
}
*/
