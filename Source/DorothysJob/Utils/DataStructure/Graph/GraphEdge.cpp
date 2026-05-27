/************************************************************************
 * @description: Represents the Edge of the Graph Data Structure.
 * @author: Josephine Esposito
 * @date: 16/04/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/
#include "GraphEdge.h"
#include "GraphNode.h"


#pragma region | Constructor and Destructor Methods
GraphEdge::GraphEdge()
{
}

GraphEdge::GraphEdge(TSharedPtr<GraphNode> _firstNode, TSharedPtr<GraphNode> _secondNode, double _weight)
{
  m_pFirstNode = _firstNode;
  m_pSecondNode = _secondNode;
  m_dWeight = _weight;
}


GraphEdge::~GraphEdge()
{
}
#pragma endregion



#pragma region | Setters
void GraphEdge::SetWeight(double& _weight)
{
  m_dWeight = _weight;
}
#pragma endregion



#pragma region | Getters
double GraphEdge::GetWeight() const
{
  return m_dWeight;
}
#pragma endregion

bool GraphEdge::IsEqualTo(TSharedPtr<GraphEdge>& _edge)
{
  // a flag for if the edge follows the same order as the given
  bool bFlagSameOrder = m_pFirstNode == _edge->m_pFirstNode && m_pSecondNode == _edge->m_pSecondNode;
  // a flag for if the edge follows the reverse order as the given
  bool bFlagReverseOrder = m_pFirstNode == _edge->m_pSecondNode && m_pSecondNode == _edge->m_pFirstNode;

  // either order is fine since the graph is not directed
  return bFlagSameOrder || bFlagReverseOrder;
}

bool GraphEdge::HasNode(TSharedPtr<GraphNode>& _node)
{
  if (m_pFirstNode == _node || m_pSecondNode == _node)
  {
    return true;
  }

  return false;
}


//EOF