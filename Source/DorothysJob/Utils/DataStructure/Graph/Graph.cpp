/************************************************************************
 * @description: Represents the Graph Data Structure.
 * @author: Josephine Esposito
 * @date: 12/04/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/
#include "Graph.h"
#include "GraphNode.h"
#include "GraphEdge.h"
#include "Quadrant.h"
#include "Door.h"
#include "DorothysJob/Actor/Room/BaseRoom.h"

void Graph::ExportToFile(const FString& _fileName) const
{
  FString output;

  output += TEXT("---------- GRAPH EXPORT ----------\n");
  output += FString::Printf(TEXT("Current number of nodes is: %d\n"), m_lNodes.Num());
  output += TEXT("The nodes are:\n");

  for (const TSharedPtr<GraphNode> node : m_lNodes)
  {
    if (node.IsValid()) // we check if valid
    {
      FIntPoint position = node->GetPosition();
      output += FString::Printf(TEXT("ID: <%d>\tRoom ID:  <%d>\t"), node->GetNodeID(), node->GetRoomID());
      output += FString::Printf(TEXT("\tPosition: [%d, %d]\t"), node->GetPosition().X, node->GetPosition().Y);
      output += FString::Printf(TEXT("\tRoomSize: [%d, %d]\t"), node->GetSize().X, node->GetSize().Y);
      output += FString::Printf(TEXT("\tRotation: [%d]\n"), node->GetRotation());

      const TArray<TSharedPtr<Quadrant>>& quadrants = node->GetQuadrants();
      output += FString::Printf(TEXT("\tQuadrants (%d):\n"), quadrants.Num());

      for (int32 i = 0; i < quadrants.Num(); ++i)
      {
        const TSharedPtr<Quadrant>& quad = quadrants[i];
        if (quad.IsValid())
        {
          output += FString::Printf(TEXT("\t\tQuadrant %d: Location [%d, %d], bIsExterior: %s\n"),
            i, quad->Location.X, quad->Location.Y, quad->bIsExterior ? TEXT("true") : TEXT("false"));

          const TArray<TSharedPtr<Door>>& doors = quad->Doors;
          output += FString::Printf(TEXT("\t\t\tDoors (%d):\n"), doors.Num());

          for (int32 j = 0; j < doors.Num(); ++j)
          {
            const TSharedPtr<Door>& door = doors[j];
            if (door.IsValid())
            {
              output += FString::Printf(TEXT("\t\t\t\tDoor %d: Used: %s, Direction: %d, Opposite: %d\n"),
                j,
                door->IsDoorUsed ? TEXT("true") : TEXT("false"),
                door->Direction,
                door->GetOppositeDirection());
            }
          }
        }
      }
    }
  }

  output += TEXT("\nThe edges are:\n");

  for (const TSharedPtr<GraphEdge> edge : m_lEdges)
  {
    if (edge.IsValid() && edge->m_pFirstNode.IsValid() && edge->m_pSecondNode.IsValid()) // we check if valid
    {
      output += FString::Printf(TEXT("Edge: %d <-> %d\n"), edge->m_pFirstNode->GetNodeID(), edge->m_pSecondNode->GetNodeID());
    }
  }

  const FString filePath = FPaths::ProjectDir() + "Content/Debug/TextFiles/" + _fileName;

  if (FFileHelper::SaveStringToFile(output, *filePath))
  {
    UE_LOG(LogTemp, Log, TEXT("Graph exported to: %s"), *filePath);
  }
  else
  {
    UE_LOG(LogTemp, Error, TEXT("Failed to write graph to: %s"), *filePath);
  }
}

#pragma region | Construct and Destruct methods

Graph::Graph(TSharedPtr<GraphNode> _root)
{
  _root->SetNodeID(m_lNodes.Num());
  m_lNodes.Add(_root);
}

Graph::~Graph()
{
  // we clean the edges first
  m_lEdges.Empty();

  // we clean the nodes then
  m_lNodes.Empty();
}

#pragma endregion

#pragma region | Add elements

void Graph::AddEdge(TSharedPtr<GraphNode> _firstNode, TSharedPtr<GraphNode> _secondNode, double _weight)
{
  TSharedPtr<GraphEdge> edge = MakeShared<GraphEdge>(_firstNode, _secondNode, _weight);

  if (HasEdge(edge))
  { // the edge is already in the graph
    return;
  }

  if (_firstNode == _secondNode)
  { // both nodes are equal
    return;
  }

  m_lEdges.Add(edge);
}

void Graph::AddNode(TSharedPtr<GraphNode> _node)
{
  for (TSharedPtr<GraphNode> node : m_lNodes)
  {
    if (node->GetNodeID() == _node->GetNodeID())
    {
      UE_LOG(LogTemp, Error, TEXT("This node already exists."));
      return; // already exists
    }
  }

  _node->SetNodeID(m_lNodes.Num());
  m_lNodes.Add(_node);
}

void Graph::AddNode(TSharedPtr<GraphNode> _node, int32 _ID)
{
  for (TSharedPtr<GraphNode> node : m_lNodes)
  {
    if (node->GetPosition().X == _node->GetPosition().X && node->GetPosition().Y == _node->GetPosition().Y)
    {
      UE_LOG(LogTemp, Error, TEXT("This node already exists."));
      return; // already exists
    }

    if (_ID == node->GetNodeID())
    {
      _ID = m_lNodes.Num();
    }
  }

  _node->SetNodeID(_ID);
  m_lNodes.Add(_node);
}

#pragma endregion

#pragma region | Getters

TArray<TSharedPtr<GraphNode>> Graph::GetNodes() const
{
  return m_lNodes;
}

TArray<TSharedPtr<GraphEdge>> Graph::GetEdges() const
{
  return m_lEdges;
}

TArray<TSharedPtr<GraphNode>> Graph::GetConnectedNodes(TSharedPtr<GraphNode> _main) const
{
  TArray<TSharedPtr<GraphNode>> lConnectedNodes_;

  for (TSharedPtr<GraphEdge> edge : m_lEdges)
  {
    // we check for validity
    if (!edge.IsValid() || !_main.IsValid())
    {
      continue;
    }

    if (_main == edge->m_pFirstNode)
    {
      lConnectedNodes_.Add(edge->m_pSecondNode);
    }
    if (_main == edge->m_pSecondNode)
    {
      lConnectedNodes_.Add(edge->m_pFirstNode);
    }
  }

  return lConnectedNodes_;
}

TArray<TSharedPtr<GraphNode>> Graph::GetUnconnectedNodes(TSharedPtr<GraphNode> _main) const
{
  TArray<TSharedPtr<GraphNode>> lUnconnectedNodes;

  TArray<TSharedPtr<GraphNode>> lAllNodes = m_lNodes;

  TArray<TSharedPtr<GraphNode>> lConnectedNodes = GetConnectedNodes(_main);

  for (TSharedPtr<GraphNode> node : lAllNodes)
  {
    if (node != _main && !lConnectedNodes.Contains(node))
    {
      lUnconnectedNodes.Add(node);
    }
  }

  return lUnconnectedNodes;
}

TArray<TSharedPtr<GraphNode>> Graph::GetQ1Nodes() const
{
  TArray<TSharedPtr<GraphNode>> Q1Nodes;

  for (TSharedPtr<GraphNode> Node : m_lNodes)
  {
    // we check for validity
    if (!Node.IsValid())
    {
      continue;
    }

    if (Node->GetRoomShape() == ERoomShape::Q1)
    {
      Q1Nodes.Add(Node);
    }
  }

  return Q1Nodes;
}

bool Graph::HasEdge(TSharedPtr<GraphEdge> _edge)
{
  for (const TSharedPtr<GraphEdge>& edge : m_lEdges)
  {
    if (
      (edge->m_pFirstNode == _edge->m_pFirstNode && edge->m_pSecondNode == _edge->m_pSecondNode) ||
      (edge->m_pFirstNode == _edge->m_pSecondNode && edge->m_pSecondNode == _edge->m_pFirstNode)
      )
    {
      return true;
    }
  }
  return false;
}

TSharedPtr<GraphNode> Graph::FindNode(TSharedPtr<GraphNode> _node)
{
  for (TSharedPtr<GraphNode>& node : m_lNodes)
  {
    if (node == _node)
    {
      return node;
    }
  }

  return nullptr;
}

TArray<TSharedPtr<GraphEdge>> Graph::GetRelevantEdges(TSharedPtr<GraphNode> _node)
{
  TArray<TSharedPtr<GraphEdge>> relevantEdges_;
  if (!FindNode(_node))
  { // the node was not found inside this graph
    return relevantEdges_;
  }

  for (TSharedPtr<GraphEdge>& edge : m_lEdges)
  {
    TSharedPtr<GraphNode>& nodeA = edge->m_pFirstNode;
    TSharedPtr<GraphNode>& nodeB = edge->m_pSecondNode;
    if (edge.IsValid() && (_node == nodeA || _node == nodeB))
    {
      relevantEdges_.Add(edge);
    }
  }

  return relevantEdges_;
}

int32 Graph::GetTotalQuadrants()
{
  int32 TotalQuadrants = 0;
  for (TSharedPtr<GraphNode> Node : m_lNodes)
  {
    TotalQuadrants += Node->GetQuadrants().Num();
  }
  return TotalQuadrants;
}

bool Graph::AlreadyConnectedWith(const TSharedPtr<GraphNode>& NodeA, const TSharedPtr<GraphNode>& NodeB) const
{
  TArray<TSharedPtr<GraphNode>> ConnectedNodes = GetConnectedNodes(NodeA);
  if (ConnectedNodes.Contains(NodeB))
  {
    return true;
  }
  return false;
}

void Graph::RemoveNode(const TSharedPtr<GraphNode>& NodeToRemove)
{
  TSharedPtr<GraphNode> Node = FindNode(NodeToRemove);
  if (!Node)
  {
    return;
  }

  int32 TargetID = Node->GetNodeID();

  // Recolectamos todos los IDs a eliminar
  TSet<int32> NodeIDsToRemove;
  for (const TSharedPtr<GraphNode>& N : m_lNodes)
  {
    if (N->GetNodeID() >= TargetID)
    {
      NodeIDsToRemove.Add(N->GetNodeID());
    }
  }

  // Eliminamos las aristas asociadas a esos nodos
  m_lEdges.RemoveAll([&NodeIDsToRemove](const TSharedPtr<GraphEdge>& Edge)
    {
      int32 ID1 = Edge->m_pFirstNode->GetNodeID();
      int32 ID2 = Edge->m_pSecondNode->GetNodeID();
      return NodeIDsToRemove.Contains(ID1) || NodeIDsToRemove.Contains(ID2);
    });

  // Eliminamos los nodos
  m_lNodes.RemoveAll([&NodeIDsToRemove](const TSharedPtr<GraphNode>& N)
    {
      return NodeIDsToRemove.Contains(N->GetNodeID());
    });
}
#pragma endregion
//EOF