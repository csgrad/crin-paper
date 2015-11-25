/*
 * test_pole_graph.c
 * 
 * Program to test the reduction algorithm on pole graph
 * These are the graphs which have n poles and are connected with hyperedges of size 4
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
  //pole graph generation
  //number = # of poles
  int x = 50;
  
  while(x<=500)
  {
  hypergraph * H = generatePoleGraph(x);
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
  
  //printf("\nReduction complete...\n\n");
  displayHyperGraph(G, "G'");
  
  hypergraph * partial = buildPartialGraph(copyHyperGraph(H));
  displayHyperGraph(partial, "Partial");
  
  //int n = numVertices(H->vertexSets);
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
  
  //Free Memory
  freeHyperGraph(H);
  freeHyperGraph(G);
  freeHyperGraph(partial);
  
  printf("----------------------------------------\n");
  
  x = x+50;
  }
  return 0;
}
