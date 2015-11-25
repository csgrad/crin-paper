/*
 * graph_generation.c
 * Author: Jason Ernst
 * Date started: Tuesday, February 26th, 2008
 * 
 * These functions are used to generate graphs for testing the reduction rules against
 * the following graphs:
 * 
 * a geometric graph with variable amounts of inner and outer points may be generated
 * ie) triangle, square, pentagon .. etc
 * 
 * a pole graph with a varying number of poles with each pole connected by hyperedges
 * of size 4
 * 
 * a random hypergraph
 */

#include <time.h>

/*
 * Generate a list of n random vertices
 */
llnode * getRandomVertices(int n, int max)
{
  int numVertices=0;
  llnode * vertexList = NULL;
  
  //Generate the list with n unique vertices
  while(numVertices < n)
  {
    int addVertex = max * (rand() / ((double)RAND_MAX + 1)) + 1;
    if(!findNode(vertexList, addVertex))
    {
      append(&vertexList, addVertex);
      numVertices++;
    }
  }
  return vertexList;
}

/*
 * Generates a random hypergraph based on a number of parameters
 */
hypergraph * generateRandomGraph(int numVertices, int maxHyperEdgeSize, int maxHyperEdges)
{
  hypergraph * H = newHyperGraph();
  
  int i = 1;
  int numEdges = 0;
  while(i<=numVertices)
  {
    appendUniqueVertexSet(&H->vertexSets, i);
    i++;
  }
   
  srand (time (0));
  
  while(numEdges<maxHyperEdges)
  {
    edgeSet * newEdge = appendEdgeSet(&H->edgeSets, numEdges);
    
    bool found = true;
    llnode * vertexList=NULL;
   
    while(found)
    { 
      int currentMaxHyperEdgeSize = maxHyperEdgeSize * (rand() / ((double)RAND_MAX + 1)) + 1;
      while(currentMaxHyperEdgeSize < 2)
      {
        currentMaxHyperEdgeSize = maxHyperEdgeSize * (rand() / ((double)RAND_MAX + 1)) + 1;
      }
      vertexList = getRandomVertices(currentMaxHyperEdgeSize, numVertices);  
      //search for vertexlist in the edges we have already added so that we do not 
      //have equal edges being added
      edgeSet * currentEdge = H->edgeSets;
      
      found = false;
      while(currentEdge!=NULL)
      {
        if(equal_list(currentEdge->vertices, vertexList))
        {
          found=true;  
          //printf("Duplicate generated.\n");
        }
        currentEdge=currentEdge->nextEdge;
      }   
    }
    newEdge->vertices = copyList(vertexList);
    addEdgetoVertices(newEdge, H->vertexSets);
    numEdges++;
  }
 
  return H;
}

/*
 * Generates a hypergraph with a given number of pentagon rings
 * **Complete this later**
 */
hypergraph * generatePentagonGraph(int m)
{
  return newHyperGraph();
}

/*
 * Generates a hypergraph with a given number of poles in a star
 * formation. This hypergraph will make use of all four reduction rules
 * when reduction is applied
 */
hypergraph * generatePoleGraph(int m)
{
  hypergraph * H = newHyperGraph();
  
  int i = 0;
  int numEdges = 0;
  
  //Generate the vertices
  while(i<=((2*m)+2)-1)
  {
    appendUniqueVertexSet(&H->vertexSets, i);
    i++;
  }
  
  //Generate edges
  //set group 1
  i = 2;
  while(i<=m+1)
  {
    edgeSet * newEdge = appendUniqueEdgeSet(&H->edgeSets, numEdges);
    appendUnique(&newEdge->vertices, 0);
    appendUnique(&newEdge->vertices, 1);
    appendUnique(&newEdge->vertices, i);
    appendUnique(&newEdge->vertices, i+m);
    addEdgetoVertices(newEdge, H->vertexSets);
    numEdges++;
    i++;
  }
  
  //set group 2
  i = 2;
  while(i<=m)
  {
    edgeSet * newEdge = appendUniqueEdgeSet(&H->edgeSets, numEdges);
    appendUnique(&newEdge->vertices, i);
    appendUnique(&newEdge->vertices, i+1);
    appendUnique(&newEdge->vertices, i+m);
    appendUnique(&newEdge->vertices, i+m+1);
    addEdgetoVertices(newEdge, H->vertexSets);
    numEdges++;
    i++;
  }
  
  //set group 3
  edgeSet * newEdge = appendUniqueEdgeSet(&H->edgeSets, numEdges);
  appendUnique(&newEdge->vertices, 2);
  appendUnique(&newEdge->vertices, m+1);
  appendUnique(&newEdge->vertices, m+2);
  appendUnique(&newEdge->vertices, 2*m+1);
  addEdgetoVertices(newEdge, H->vertexSets);
  
  return H;
}

//Generates a hypergraph with a inner / outer ring of a given size
//ex a square inside = 4, pentagon = 5, hexagon = 6
hypergraph * generateGeometricGraph(int sides)
{
  hypergraph * H = newHyperGraph();
  
  int counter = 1;
  
  //Generate the vertices of the hypergraph
  while(counter <= sides*2)
  {
    appendUniqueVertexSet(&H->vertexSets, counter);
    counter++;
  }

  //Generate the edge sets of the hypergraph
  int currentVertex = 1;
  int numEdges = 1;
  
  while(currentVertex <= sides)
  {    
    //edges with 2 vertices, outer ring
    int u = currentVertex;
    int v = currentVertex+1;
    if(v>sides)
    {  u = v-sides;
       v = currentVertex;
    }
    //printf("%d, %d \n", u, v);
    //set id of new edge to be the current number of edges
    edgeSet * newEdge = appendUniqueEdgeSet(&H->edgeSets, numEdges);
    if(newEdge != NULL)
    {
      //add the vertices to the set
      appendUnique(&newEdge->vertices, u);
      appendUnique(&newEdge->vertices, v);
      addEdgetoVertices(newEdge, H->vertexSets);
    }
    else
    {
      printf("Error generating the graph.n");
      exit(0);
    }
   
    
    numEdges++;
    
    //edges with 2 vertices, inner ring
    u = currentVertex + sides;
    v = currentVertex + sides + 1;
    if(v>sides*2)
    {
      u = sides+1;
      v = currentVertex+sides;
    }
    //printf("%d, %d \n", u, v);
    //set id of new edge to be the current number of edges
    newEdge = appendUniqueEdgeSet(&H->edgeSets, numEdges);
    //add the vertices to the set
    if(newEdge != NULL)
    {
      //add the vertices to the set
      appendUnique(&newEdge->vertices, u);
      appendUnique(&newEdge->vertices, v);
      addEdgetoVertices(newEdge, H->vertexSets);
    }
    else
    {
      printf("Error generating the graph.n");
      exit(0);
    }
    numEdges++;
    
    //H1
    u = currentVertex;
    v = currentVertex + sides + 1;
    int w = currentVertex + sides + 2;
    if(w>sides*2)
    {
      w = w - sides;
    }
    if(v>sides*2)
    {
      v = v - sides;
    }
    if(v > w)
    {
      int tmp = v;
      v = w;
      w = tmp;
    }
    //printf("%d, %d, %d\n", u, v, w);
    //set id of new edge to be the current number of edges
    newEdge = appendUniqueEdgeSet(&H->edgeSets, numEdges);
    //add the vertices to the set
    if(newEdge != NULL)
    {
      //add the vertices to the set
      appendUnique(&newEdge->vertices, u);
      appendUnique(&newEdge->vertices, v);
      appendUnique(&newEdge->vertices, w);
      addEdgetoVertices(newEdge, H->vertexSets);
    }
    else
    {
      printf("Error generating the graph.n");
      exit(0);
    }
    
    numEdges++;
    
    //H2
    u = currentVertex;
    v = currentVertex+1;
    w = currentVertex+sides+2;
    if(w>sides*2)
    {
      w = w - sides;
    }
    if(v>sides)
    {
      u = v - sides;
      v = currentVertex;
    }
    //printf("%d, %d, %d\n", u, v, w);
    //set id of new edge to be the current number of edges
    newEdge = appendUniqueEdgeSet(&H->edgeSets, numEdges);
    //add the vertices to the set
    if(newEdge != NULL)
    {
      //add the vertices to the set
      appendUnique(&newEdge->vertices, u);
      appendUnique(&newEdge->vertices, v);
      appendUnique(&newEdge->vertices, w);
      addEdgetoVertices(newEdge, H->vertexSets);
    }
    else
    {
      printf("Error generating the graph.n");
      exit(0);
    }
    numEdges++;
     
    currentVertex++;
  }  
  return H;
}
