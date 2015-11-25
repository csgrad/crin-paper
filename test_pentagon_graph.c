/*
 * test_pentagon_graph.c
 * 
 * Program to test the reduction algorithm on pentagon graph
 * These are the graphs which are made of a pentagon pattern where
 * the smaller pentagons in middle form a larger pentagon
 * 
 * Author: Jason Ernst
 * Date started: Monday, March 3rd, 2008
 */

#include "reduction.c"

int main(void)
{
  //pentagon graph generation
  //number = # of poles
  hypergraph * H = generatePentagonGraph(7);
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
  
  printf("\nWorking on reduction...\n\n");
  G = reduction(H, G);
  
  printf("\nReduction complete...\n\n");
  displayHyperGraph(G, "G'");
  
  H = generatePoleGraph(7);
  
  if(isInterConnectionGraphOf(H, G))
    printf("SOLUTION CORRECT\n");
  else
    printf("SOLUTION INCORRECT\n");
  
  //Free Memory
  freeHyperGraph(H);
  freeHyperGraph(G);
  
  return 0;
}
