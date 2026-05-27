/************************************************************************
 * @description: Represents the Edge of the Graph Data Structure.
 * @author: Josephine Esposito
 * @date: 16/04/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"

class GraphNode;

/**
 * Represents the weighted edges of the graph
 */
class DOROTHYSJOB_API GraphEdge
{
public:
#pragma region | Constructor and Destructor Methods
	/**
	 * @brief The default constructor. This will create an empty non-valid edge with zero weight
	 */
	GraphEdge();

	/**
	 * @brief The base constructor. This will create a new edge based on the received nodes
	 * @param _firstNode
	 * @param _secondNode
	 * @param _weight  Default value is 0
	 */
	GraphEdge(TSharedPtr<GraphNode> _firstNode, TSharedPtr<GraphNode> _secondNode, double _weight = 0);

	/**
	 * @brief The base destructor of the edge
	 */
	~GraphEdge();
#pragma endregion

#pragma region | Setters
	/**
	 * @brief To set the weight of the edge
	 * @param _weight, A double representing the weight of the edge. This value can be negative and 0
	 */
	void SetWeight(double& _weight);
#pragma endregion

#pragma region | Getters
	/**
	 * @brief To get the current weight of the edge
	 * @return A double, representing the weight of the edge, this value can be negative and 0
	 */
	double GetWeight() const;
#pragma endregion

	/**
	 * @brief This will compare two edges
	 * @param _edge, The other edge to compare
	 * @return Will return true, if both edges are perfectly equal in connected nodes and weight
	 */
	bool IsEqualTo(TSharedPtr<GraphEdge>& _edge);

	/**
	 * @brief Checks if the given node is present inside the edge
	 * @param _node The node to validate
	 * @return A flag stating the result of the check
	 */
	bool HasNode(TSharedPtr<GraphNode>& _node);

	/**
	 * @brief A flag stating if the edge is of the supertriangle or not
	 */
	bool m_bIsHelper = false;

	/**
	 * @brief The weight of the edge. This will be calculated with set rules and distance
	 */
	double m_dWeight;

	/**
	 * @brief The first node of the edge
	 */
	TSharedPtr<GraphNode> m_pFirstNode;

	/**
	 * @brief The second node of the edge
	 */
	TSharedPtr<GraphNode> m_pSecondNode;
};
//EOF