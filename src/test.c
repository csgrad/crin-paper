/*
 * test.c
 * Tests a sample network graph and tries to optimize the connections
 */

#include "reduction.c"
#include <sys/types.h>
#include <sys/times.h>

int main(void)
{
  hypergraph * H = loadHyperGraph("data/test.dat");
  displayHyperGraph(H, "H");
  
  //Initialize G with no edges and the same vertices as H
  hypergraph * G = newHyperGraph();
  vertexSet *tmp = H->vertexSets;
  while(tmp != NULL)
  {
    appendUniqueVertexSet(&G->vertexSets, tmp->id);
    tmp = tmp->nextVertex;
  }
  
  G = reduction(copyHyperGraph(H), G);
  
  //printf("\nReduction complete...\n\n");
  displayHyperGraph(G, "G'");
  return 0;
}
