/*
 * test_interconnect_checking.c
 * 
 * Checks if a given graph is a interconnection graph of another graph
 * 
 * Author: Jason Ernst
 * Date started: Monday, March 3rd, 2008
 */

#include "reduction.c"

int main(void)
{
  //geometric graph generation
  hypergraph * H = generateGeometricGraph(4);
  H = sortHyperGraph(H);
  
  //Set testGraph to be the reduction of H for testing purposes
  hypergraph * testGraph = newHyperGraph();
  vertexSet *tmp = H->vertexSets;
  while(tmp != NULL)
  {
    appendUniqueVertexSet(&testGraph->vertexSets, tmp->id);
    tmp = tmp->nextVertex;
  }
  
  testGraph = reduction(H, testGraph);
  
  //removeEdgeFromVertices(testGraph->vertexSets,1);
  //removeEdgeSet(&testGraph->edgeSets, 1);
  
  displayHyperGraph(testGraph, "testGraph");
  
  //regenerate H since it was destroyed
  H = generateGeometricGraph(4);
  H = sortHyperGraph(H);
  displayHyperGraph(H, "H");
  
  //test if testgraph is the interconnection graph of h
  if(isInterConnectionGraphOf(H, testGraph))
    printf("testGraph is interconnection graph of H\n");
  else
    printf("testGraph is not interconnection graph of H\n");
  
  //Free Memory
  freeHyperGraph(H);
  freeHyperGraph(testGraph);
  
  return 0;
}
