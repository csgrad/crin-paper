/*
 * test_random_graph.c
 * 
 * Program to test the reduction algorithm on random graph with
 * certain parameters specified below
 * 
 * Author: Jason Ernst
 * Date started: Monday, March 3rd, 2008
 */

#include "hypergraph.h"
#include "graph_generation.h"
#include "reduction.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/times.h>

int main(void)
{
  //totals used for averages
  int numRuns = 10;
  float edgesInG = 0;
  float edgesFullyConnected = 0;
  float edgesPartiallyConnected = 0;
  float time = 0;
  
  int x=0;
  while(x<numRuns)
  {    
  //random graph parameters
  int numVertices = 100;
  int maxEdgeSize = 10;
  int maxEdges = numVertices * 5;
  //int maxEdges = pow(numVertices,2);
  
  printf("Generating Random Graph...\n");
  hypergraph * H = generateRandomGraph(numVertices, maxEdgeSize, maxEdges);
  H = sortHyperGraph(H);
  
  displayHyperGraph(H, "H");
  
  //Initialize G with no edges and the same vertices as H
  hypergraph * G = newHyperGraph();
  vertexSet *tmp = H->vertexSets;
  while(tmp != NULL)
  {
    appendUniqueVertexSet(&G->vertexSets, tmp->id);
    tmp = tmp->nextVertex;
  }
  
  struct tms before, after;
  times(&before);
  
  printf("\nWorking on reduction...\n\n");
  G = reduction(copyHyperGraph(H), G);
  
  times(&after);
  
  printf("\nReduction complete...\n\n");
  displayHyperGraph(G, "G'");
  
  hypergraph * partial = buildPartialGraph(copyHyperGraph(H));
  displayHyperGraph(partial, "Partial");
  
  //int n = numVertices;
  //int m = n*(n-1)/2;
  //printf("Number of edges of Completely Connected: %d\n", m);
  
  /*
  if(isInterConnectionGraphOf(H, G))
    printf("G is a solution of H\n");
  else
    printf("G is not a solution of H\n");
  */
  printf("Runtime: %.2f seconds\n", ((float)(after.tms_utime -
        before.tms_utime)/100));
  
  time = time + ((float)(after.tms_utime - before.tms_utime)/100);
  edgesPartiallyConnected = edgesPartiallyConnected + numEdges(partial->edgeSets);
  //edgesFullyConnected = edgesFullyConnected + m;
  edgesInG = edgesInG + numEdges(G->edgeSets);
  
  //Free Memory
  freeHyperGraph(H);
  freeHyperGraph(G);
  freeHyperGraph(partial);
  x++;
  }
  
  time = time / numRuns;
  edgesPartiallyConnected = edgesPartiallyConnected / numRuns;
  edgesFullyConnected = edgesFullyConnected / numRuns;
  edgesInG = edgesInG / numRuns;
  
  printf("AVG TIME: %f\n", time);
  printf("G: %f\n", edgesInG);
  //printf("FULLY: %f\n", edgesFullyConnected);
  printf("PARTIAL: %f", edgesPartiallyConnected);
  
  puts("\a"); //system beep so we can hear when it is done!
  
  return 0;
}
