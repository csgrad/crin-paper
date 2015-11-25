/*
 * Turns BS representation into graph representation
 */
hypergraph * generateHyperGraph(vector <BaseStation>& bs)
{
  hypergraph * H = newHyperGraph();
  
  for(int vertices=0;vertices<bs.size();vertices++)
  { vertexSet * newVertex = appendVertexSet(&H->vertexSets, vertices); }

  int numEdges = 0;  
      
  for(int vertices=0;vertices<bs.size();vertices++)
  {
    edgeSet * newEdge = appendEdgeSet(&H->edgeSets, numEdges);
    for(int edges=0;edges<MAXNEIGHBOURS;edges++)
    {
      if(bs[vertices].getNeighbour(edges)!=-1)
        append(&newEdge->vertices, bs[vertices].getNeighbour(edges));
      addEdgetoVertices(newEdge, H->vertexSets);
      numEdges++;
    }
  }
  return H;
}

//returns true if source exists in the vector of visited nodes
bool beenVisited(int source, vector <int>& visitedNodes)
{
  for(int counter=0;counter<visitedNodes.size(); counter++)
  {
    if(visitedNodes[counter] == source)
      return true;
  }
  return false;
}

int length(int source, int destination, vector <BaseStation>& bs, vector <int> visitedNodes)
{
  //arrived at dest
  if(source == destination)
    return 0;
  else
  {
    //loop
    if(beenVisited(source, visitedNodes))
      return 99999;
    else
    {
      int minLength = 99999;
      visitedNodes.push_back(source);
      //for each neighbour of source, check the length to destination
      for(int currentNeighbour=0; currentNeighbour<MAXNEIGHBOURS; currentNeighbour++)
      {
        int neighbour = bs[source].getNeighbour(currentNeighbour);
        if(neighbour != -1)
        {
          int currentLength = 1 + length(neighbour, destination, bs, visitedNodes);
          if(currentLength < minLength)
            minLength = currentLength;
        }
      }
      visitedNodes.pop_back();//pop currentNode off the visited so that it can be reached again from another direction.
      return minLength;
    }
  }
}

int getCrinNeighbour(int source, int destination, vector <BaseStation>& bs)
{
  int minLength = 99999;
  int minNeighbour = 99999;
  
  //for each neighbour of source, check the length to destination
  for(int currentNeighbour=0; currentNeighbour<MAXNEIGHBOURS; currentNeighbour++)
  {
    int neighbour = bs[source].getNeighbour(currentNeighbour);
    if(neighbour != -1)
    {
      //cout << "Checking neighbour: " << neighbour << "\n";
      vector <int> visitedNodes;
      visitedNodes.push_back(source); //keep track of nodes already visited
      int currentLength = length(neighbour, destination, bs, visitedNodes);
      //cout << "Current Length: " << currentLength << "\n";
      if( currentLength< minLength)
      {
        minLength = currentLength;
        minNeighbour = neighbour;
      }
    }
  }
  return minNeighbour;
}

/*
 * Turns graph representation into BS representation
 */
void * generateCbsNeighbours(hypergraph * G, vector <BaseStation>& cbs, vector <BaseStation>& bs)
{
  //add new neighbours
  edgeSet * currentEdge = G->edgeSets;
  while(currentEdge != NULL)
  {
    llnode * vertex = currentEdge->vertices;
    while(vertex!=NULL)
    {
      llnode * neighbour = currentEdge->vertices;
      while(neighbour!=NULL)
      {
        if(neighbour->data != vertex->data)
        {
          double tmpDist = getDistance(cbs[vertex->data].getX(), cbs[neighbour->data].getX(),cbs[vertex->data].getY(), cbs[neighbour->data].getY());
          if(tmpDist < (double)RANGE)
            cbs[vertex->data].addNeighbour(neighbour->data);
          else
          {
            int nearNeighbour = getNearestNeighbour(vertex->data, neighbour->data, cbs);
            if(nearNeighbour == -1)
            {
              nearNeighbour = getNearestNeighbour(vertex->data, neighbour->data, bs);
            }
            cbs[vertex->data].addNeighbour(nearNeighbour);
            cbs[nearNeighbour].addNeighbour(vertex->data);
          }
        }
        neighbour = neighbour->next;
      }
      vertex = vertex->next;
    }
    currentEdge = currentEdge->nextEdge;
  }
}
