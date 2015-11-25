/*
 * Author: Jason Ernst
 * Date Started: Tuesday December 11th, 2007
 * Reduction Algorithm
 */
#include "hypergraph.h"
#include "ll.h"
#include "reduction.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

hypergraph * reduction(hypergraph * H, hypergraph * G)
{
  //Generate Ebar'
  //printf("Ebar'\n");
  //printf("***************\n");
  
  //OLD way of generating EprimeBar, now we use the partial graph as EprimeBar
  //edgeSet * EprimeBar = generateEprimeBar(H);
  hypergraph * tmp = buildPartialGraph(H);
  edgeSet * EprimeBar = copyEdgeSets(tmp->edgeSets);
 
  //freeHyperGraph(tmp);
  
  //displayEdges(EprimeBar);
  //printf("***************\n");
  
  //initialize reduction condition
  bool reduc = true;
  
  //Loop until R' is NULL
  while(H->edgeSets != NULL)
  {
    if(reduc)
    {
      reduction_rules(H, G, &EprimeBar);
      reduc = false;
    }
    else
    {
      //printf("Generating eStar...\n");
      edgeSet * eStar = generateEstar(H, EprimeBar);
      if(eStar != NULL)
      {
        //printf("eStar generated successfully: ");
        //displayEdge(eStar);
        
        //Add eStar to E'
        //printf("Adding E%d to E': ", eStar->id);
        //displayEdge(eStar);
        edgeSet * tmp = appendUniqueEdgeSet(&G->edgeSets, eStar->id);
        tmp->vertices = copyList(eStar->vertices);
        addEdgetoVertices(tmp, G->vertexSets);
        
        //Remove eStar from Ebar'
        edgeSet * reverseEStar = edgesContainingUV(EprimeBar,eStar->vertices->next->data, eStar->vertices->data);        
        //printf("Removing E%d from Ebar': ", eStar->id);
        //displayEdge(eStar);
        removeEdgeSet(&EprimeBar, eStar->id);
        if(reverseEStar != NULL)
        {
          //printf("Removing E%d from Ebar': ", reverseEStar->id);
          //displayEdge(reverseEStar);
          removeEdgeSet(&EprimeBar, reverseEStar->id);
        }
        
        
        
        reduc = true;
      }
      else
      {
        printf("eStar is NULL, there is a problem.\n");
        displayHyperGraph(H, "HTest");
        displayHyperGraph(G, "GTest");
        exit(0);
      }
    }    
  }
  
  freeEdgeSets(EprimeBar); 
  return G;
}

void reduction_rules(hypergraph * H, hypergraph * G, edgeSet ** EprimeBar)
{
  //printf("Starting Reduction Rules:\n");
  
  bool repeat = true;
  bool r1 = true; bool r2 = true; bool r3 = true; bool r4 = true;
  
  while(repeat)
  {
    //Check rule 1
    if(!rule1(H, G, EprimeBar))
    { r1 = false; }
    else
    {while(rule1(H,G,EprimeBar));}
    
    //Check rule 2
    if(rule2(H, G, EprimeBar))
    { r1 = true; }
    else
    { r2 = false; }
    
    //Check rule 3
    if(rule3(H, G, EprimeBar))
    { r1 = true; r2 = true; }
    else
    { r3 = false; }
      
    if(rule4(H, G, EprimeBar))
    { r1 = true; r2 = true; r3 = true; }
    else
    { r4 = false; }
      
    repeat = r1 | r2 | r3 | r4;
    //displayHyperGraph(G, "G for Testing");
    //displayHyperGraph(H, "H for testing");
    //printf("Press enter to continue: ");
    //getchar();
  }
}

/*
 * Generates eStar which is used when no more reduction may be done on a hypergraph
 * eStar is the edge in the hypergraph with the highest degree
 * returns NULL if we cannot generate Estar properly
 */
edgeSet * generateEstar(hypergraph * H, edgeSet * EprimeBar)
{
  //displayHyperGraph(H, "H test");
  int maxDegree = 0;
  int maxId = -1;
  int currentDegree;
  
  edgeSet * currentEprimeBar = EprimeBar;
  
  while(currentEprimeBar != NULL)
  {
    currentDegree = degree(H, currentEprimeBar->vertices->data, currentEprimeBar->vertices->next->data);
    //check if the degree of the current EprimeBar is higher than the max we have
    //found so far, if so replace the old high with the current
    if( currentDegree > maxDegree )
    {
      maxDegree = currentDegree;
      maxId = currentEprimeBar->id;
    }
    currentEprimeBar = currentEprimeBar->nextEdge;
  }
  return findEdge(EprimeBar, maxId);
}

bool rule1(hypergraph * H, hypergraph * G, edgeSet ** EprimeBar)
{
  edgeSet * N = H->edgeSets;
  hypergraph *inducedSub = NULL;
  
  //Create a bunch of indused subgraphs of G using a the vertices from edges in H
  while(N!=NULL)
  {
    //printf("Checking induced subgraph on N: ");
    //displayListAsSet(N->vertices);
    inducedSub = generateInducedSubgraph(G, N);
    //displayHyperGraph(inducedSub, "Induced Sub");
    
    //check if the subgraph is connected (just have to make sure each vertex in the list has an edge associated with it
    bool bad_graph = false;
    vertexSet * currentVertex = inducedSub->vertexSets;
    
    while(currentVertex != NULL)
    {
      if(numNodes(currentVertex->edges) == 0)
        bad_graph = true;
      currentVertex = currentVertex->nextVertex;
    }
    
    //if we didnt get a bad_graph then we have a induced sub that is connected
    if(!bad_graph)
    {
      //printf("Connected. Removing E%d from R'\n", N->id);
      //displayEdge(N);
      removeEdgeFromVertices(H->vertexSets, N->id);
      removeEdgeSet(&H->edgeSets, N->id);
      return true;
    }
    
    N = N->nextEdge;
  }
  //printf("No connected induced subgraphs. R1 = false\n");
  return false;
}

bool rule2(hypergraph * H, hypergraph * G, edgeSet ** EprimeBar)
{
  vertexSet * u=H->vertexSets;
  
  //loop through all the vertices looking for equals that can
  //be combined together
  while(u!=NULL)
  {
    vertexSet * v = u->nextVertex;
    while(v!=NULL)
    {
      //check for equal vertices that have non empty edge sets
      if(equal_list(u->edges, v->edges) && u->edges!=NULL && v->edges!=NULL)
      {
        //printf("Equal: U%d V%d\n", u->id, v->id);
        //Add uv from Ebar' to E'
        edgeSet * uv = edgesContainingUV(*EprimeBar, u->id, v->id);
        if(uv != NULL)
        {
          //printf("Adding E%d to E': ", uv->id);
          //displayEdge(uv);
          edgeSet * newEdge = appendUniqueEdgeSet(&G->edgeSets, uv->id);
          newEdge->vertices = copyList(uv->vertices);
          addEdgetoVertices(newEdge, G->vertexSets);
          
          //check rule1 since we have added a new edge
          while(rule1(H,G,EprimeBar));
        }
        else
        {
          //printf("Error getting uv from EprimeBar in rule 2, may already be added.\n");
          //exit(0);
        }
        
        //remove edges in Ebar' with v
        edgeSet * currentEprimeBar = *EprimeBar;
        while(currentEprimeBar != NULL)
        {
          //if we find v in the vertex list of currentEprimeBar
          //remove it from EprimeBar
          if(findNode(currentEprimeBar->vertices, v->id)!=NULL)
          {
            //printf("Removing E%d from Ebar': ", currentEprimeBar->id);
            //displayEdge(currentEprimeBar);
            removeEdgeSet(EprimeBar, currentEprimeBar->id);
          }
          currentEprimeBar = currentEprimeBar->nextEdge;
        }
        
        //remove hyperedges in R containing v
        edgeSet * R = H->edgeSets;
        while(R!=NULL)
        {
          //if we have a hyperedge with v in it remove it from R
          if(findNode(R->vertices, v->id)!=NULL && numNodes(R->vertices) > 2)
          {
            //printf("Removing v%d from all hyperedges which contain it: ", v->id);
            //displayEdge(R);
            removeNode(&R->vertices, v->id);
          }
          R = R->nextEdge;
        }
        
        //remove v from V'
        //printf("Removing V%d from V':", v->id);
        //displayVertex(v);
        removeVertexSet(&H->vertexSets, v->id);
        
        //printf("EQUAL LIST\n");
        //getchar();
        return true;
      }
      v=v->nextVertex;
    }    
    u=u->nextVertex;
  }
  
  //printf("No equal vertices. R2 = False.\n");
  return false;
}

bool rule3(hypergraph * H, hypergraph * G, edgeSet ** EprimeBar)
{
  bool found = false;
  edgeSet * N = H->edgeSets;
  
  //Loop through all of R' looking for non-hyperedges
  while(N != NULL)
  {
    //Check for non-hyperedge
    if(numNodes(N->vertices) == 2)
    {
      found = true;
      
      //add uv to E'
      //printf("Adding E%d to E': ", N->id);
      //displayEdge(N);
      edgeSet * newEdge = appendUniqueEdgeSet(&G->edgeSets, N->id);
      if(newEdge != NULL)
      {
        //copy the vertices list from the edge in H to the edge in G
        newEdge->vertices = copyList(N->vertices);
        //update vertexSets so they contain the edge we just added
        addEdgetoVertices(newEdge, G->vertexSets);
      }
      else
        printf("Error adding newEdge E%d to G \n", N->id); 
      
      //remove uv from Ebar'
      edgeSet * tmp = edgesContainingVertex(*EprimeBar, N->vertices->data);
      tmp = edgesContainingVertex(tmp, N->vertices->next->data);
      
      while(tmp != NULL)
      {
        edgeSet * nextTmp = tmp->nextEdge;
        //printf("Removing E%d from Ebar': ", tmp->id);
        //displayEdge(tmp);
        removeEdgeSet(EprimeBar, tmp->id);
        tmp = nextTmp;
      }
      
      //remove N from R
      //printf("Removing E%d from R': ", N->id);
      //displayEdge(N);
      removeEdgeFromVertices(H->vertexSets, N->id);
      removeEdgeSet(&H->edgeSets, N->id);
    }
    N = N->nextEdge;
  }
  
  //if(found == false)
    //printf("No non-hyperedges in R'. R3 = False.\n");
  
  return found;
}

//may need to fix this for when one of the edges has already been added, in this
//case it will not be found in eprimebar but we should still add
//the remaining edge
bool rule4(hypergraph * H, hypergraph * G, edgeSet ** EprimeBar)
{
  edgeSet * N = H->edgeSets;
  
  while(N!=NULL)
  {
    //check for {u,v,w} in R'
    if(numNodes(N->vertices)==3)
    {
      //check for uv in E'
      int u = N->vertices->data;
      int v = N->vertices->next->data;
      int w = N->vertices->next->next->data;
      //printf("Testing u:%d v:%d w:%d hyperedge: ", u, v, w);
      //displayEdge(N);
      
      edgeSet * uv = edgesContainingUV(*EprimeBar, u, v);
      edgeSet * uw = edgesContainingUV(*EprimeBar, u, w);
      edgeSet * vw = edgesContainingUV(*EprimeBar, v, w);
      edgeSet * tmp = NULL;
      
      //First try uv and see if that has dH=1
      if(degree(H, u, v) == 1 && uv!=NULL)
      {          
        //add the opposite two edges in E' (1 may not exist because it may already be added)
        //uw
        if(uw!=NULL)
        {
          //printf("Adding E%d to E': ", uw->id);
          //displayEdge(uw);
          tmp = appendEdgeSet(&G->edgeSets, uw->id);
          tmp->vertices = copyList(uw->vertices);
          addEdgetoVertices(tmp, G->vertexSets);
        }
        //vw
        if(vw!=NULL)
        {
          //printf("Adding E%d to E': ", vw->id);
          //displayEdge(vw);
          tmp = appendEdgeSet(&G->edgeSets, vw->id);
          tmp->vertices = copyList(vw->vertices);
          addEdgetoVertices(tmp, G->vertexSets);
        }
          
        //remove all components of hyperedge from Ebar'
        //(1 may not exist because it may already be removed)
        //printf("Removing E%d from Ebar': ",uv->id);
        //displayEdge(uv);
        removeEdgeSet(EprimeBar, uv->id);
        if(uw!=NULL)
        {
          //printf("Removing E%d from Ebar': ",uw->id);
          //displayEdge(uw);
          removeEdgeSet(EprimeBar, uw->id);
        }
        if(vw!=NULL)
        {
          //printf("Removing E%d from Ebar': ",vw->id);
          //displayEdge(vw);
          removeEdgeSet(EprimeBar, vw->id);
        }
        return true;
      }
        
      //Next try uw for dH=1
      //printf("Testing Degree of UW: ");
      //displayEdges(uw);
      if(degree(H, u, w) == 1 && uw !=NULL)
      {
        //add the opposite two edges in E' (1 may not exist because it may already be added)
        //uv
        if(uv!=NULL)
        {
          //printf("Adding E%d to E': ", uv->id);
          //displayEdge(uv);
          tmp = appendEdgeSet(&G->edgeSets, uv->id);
          tmp->vertices = copyList(uv->vertices);
          addEdgetoVertices(tmp, G->vertexSets);
        }
        //vw
        if(vw!=NULL)
        {
          //printf("Adding E%d to E': ", vw->id);
          //displayEdge(vw);
          tmp = appendEdgeSet(&G->edgeSets, vw->id);
          tmp->vertices = copyList(vw->vertices);
          addEdgetoVertices(tmp, G->vertexSets);
        }
          
        //remove all components of hyperedge from Ebar'
        //(1 may not exist because it may already be removed)
        if(uv!=NULL)
        {
          //printf("Removing E%d from Ebar': ",uv->id);
          //displayEdge(uv);
          removeEdgeSet(EprimeBar, uv->id);
        }
        //printf("Removing E%d from Ebar': ",uw->id);
        //displayEdge(uw);
        removeEdgeSet(EprimeBar, uw->id);
        if(vw!=NULL)
        {
          //printf("Removing E%d from Ebar': ",vw->id);
          //displayEdge(vw);
          removeEdgeSet(EprimeBar, vw->id);
        }
        return true;
      }
      //Last try vw for dH=1    
      //printf("Testing Degree of VW: ");
      //displayEdges(vw);
      if(degree(H, v, w) == 1 && vw !=NULL)
      {
        //add the opposite two edges in E' (1 may not exist because it may already be added)
        //uv
        if(uv!=NULL)
        {
          //printf("Adding E%d to E': ", uv->id);
          //displayEdge(uv);
          tmp = appendEdgeSet(&G->edgeSets, uv->id);
          tmp->vertices = copyList(uv->vertices);
          addEdgetoVertices(tmp, G->vertexSets);
        }
        //uw
        if(uw!=NULL)
        {
          //printf("Adding E%d to E': ", uw->id);
          //displayEdge(uw);
          tmp = appendEdgeSet(&G->edgeSets, uw->id);
          tmp->vertices = copyList(uw->vertices);
          addEdgetoVertices(tmp, G->vertexSets);
        }
          
        //remove all components of hyperedge from Ebar'
        //(1 may not exist because it may already be removed)
        if(uv!=NULL)
        {
          //printf("Removing E%d from Ebar': ",uv->id);
          //displayEdge(uv);
          removeEdgeSet(EprimeBar, uv->id);
        }
        if(uw!=NULL)
        {
          //printf("Removing E%d from Ebar': ",uw->id);
          //displayEdge(uw);
          removeEdgeSet(EprimeBar, uw->id);
        }
        //printf("Removing E%d from Ebar': ",vw->id);
        //displayEdge(vw);
        removeEdgeSet(EprimeBar, vw->id);
        return true;
      }          
    }
    N = N->nextEdge;
  }
 // printf("No hyperedges with a with covering of 1 in H'. Rule 4 false.\n");
  return false;
}

/*
 * Generates uv: {u,v} c= V
 */
edgeSet * generateEprimeBar(hypergraph * H)
{
  //Generate EprimeBar uv: {u,v} proper subset of V
  edgeSet * EprimeBar = NULL;
  
  //Get the maximum edge id so that we can use unique indices
  //for adding the new candidate edges
  int maxEdges = numEdges(H->edgeSets);
  
  //Loop through V and generate edges with all combinations of u and v
  vertexSet * u = H->vertexSets;
  while(u != NULL)
  {
    vertexSet * v = H->vertexSets;
    while(v != NULL)
    {
      if(u->id != v->id)
      {
        maxEdges++;
        //Add the generated edge to EprimeBar
        edgeSet * currentEprime = appendUniqueEdgeSet(&EprimeBar, maxEdges);
        if(currentEprime != NULL)
        {
          appendUnique(&currentEprime->vertices, u->id);
          appendUnique(&currentEprime->vertices, v->id);
        }
        //if we get two edges with the same id we cannot continue
        else
        {
          printf("Error generating EprimeBar, program closing.");
          exit(0);
        }
      }
      v = v->nextVertex;
    }
    u = u->nextVertex;
  }
  
  return EprimeBar;
}

/*
 * Returns true if testGraph is an InterConnectionGraph of H
 */
bool isInterConnectionGraphOf(hypergraph * H, hypergraph * testGraph)
{
  edgeSet * EprimeBar = generateEprimeBar(H);
  
  while(rule1(H, testGraph, &EprimeBar))
  {}
  
  if(numEdges(H->edgeSets) == 0)
    return true;
  else
    return false;
}

/*
 * Main Reduction Algorithm Testing Program
 *
int main()
{  
  //Load HyperGraph Definition from file
  hypergraph * H = loadHyperGraph("pentagon_graph.dat");
  displayHyperGraph(H, "H");
  
  //Initialize G with no edges and the same vertices as H
  hypergraph * G = newHyperGraph();
  vertexSet *tmp = H->vertexSets;
  while(tmp != NULL)
  {
    appendUniqueVertexSet(&G->vertexSets, tmp->id);
    tmp = tmp->nextVertex;
  }
  
  printf("\nWorking on reduction...\n\n");
  G = reduction(H, G);
  
  printf("\nReduction complete...\n\n");
  displayHyperGraph(G, "G'");
  
  //Free Memory
  freeHyperGraph(H);
  freeHyperGraph(G);
  return (0);
}
*/
