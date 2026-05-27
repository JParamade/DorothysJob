/************************************************************************
 * @description: Represents the Graph Data Structure.
 * @author: Josephine Esposito
 * @date: 12/04/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"

#pragma region | Forward Declaration
class GraphEdge;
class GraphNode;
#pragma endregion

/**
 * 
 */
class DOROTHYSJOB_API Graph
{
public:
	/**
	 * @brief This writes the current graph into a txt file.
	 *				It can be used for testing
	 * @param _fileName The name of the txt file
	 */
	void ExportToFile(const FString& _fileName) const;

#pragma region | Construct and Destruct methods
	/**
	 * @brief The default constructor. This will create an empty non-valid graph.
	 */
	Graph() = default;

	/**
	 * @brief The base constructor. This will create a graph with one node and no edges
	 * @param _root The first node of the graph
	 */
	Graph(TSharedPtr<GraphNode> _root);

	/**
	 * @brief The default destructor of the graph
	 */
	~Graph();
#pragma endregion


#pragma region | Add elements
	/**
	 * @brief This will add a new edge into the graph. If the node is not present it will be added to the graph's Array
	 * @param _firstNode The first room to add to the edge
	 * @param _secondNode The second room to add to the edge
	 * @param _weight The weight of the edge, By default this will be 0
	 */
	void AddEdge(TSharedPtr<GraphNode> _firstNode, TSharedPtr<GraphNode> _secondNode, double _weight = 0);
	
	/**
	 * @brief This will add a new node into the graph
	 * @param _node The Room to add to the graph
	 */
	void AddNode(TSharedPtr<GraphNode> _node);

	/**
	 * @brief This will add a new node into the graph with the given ID
	 * @param _node The node to add
	 * @param _ID The id to set
	 */
	void AddNode(TSharedPtr<GraphNode> _node, int32 _ID);
#pragma endregion

	/**
	 * @brief Returns the array of nodes in the graph
	 * @return A TArray representing the nodes in the graph
	 */
	TArray<TSharedPtr<GraphNode>> GetNodes() const;

	/**
	 * @brief Returns the array of edges in the graph
	 * @return A TArray representing the nodes in the graph
	 */
	TArray<TSharedPtr<GraphEdge>> GetEdges() const;

	/**
	 * @brief Returns the neighbors of the node
	 * @param _main The main node
	 * @return An array of the connected nodes
	 */
	TArray<TSharedPtr<GraphNode>> GetConnectedNodes(TSharedPtr<GraphNode> _main) const;

	TArray<TSharedPtr<GraphNode>> GetUnconnectedNodes(TSharedPtr<GraphNode> _main) const;

	TArray<TSharedPtr<GraphNode>> GetQ1Nodes() const;

	/**
	 * @brief This checks if the edge is already present into the graph
	 * @param _edge The edge to validade
	 * @return A flag stating if the edge is present or not in the graph
	 */
	bool HasEdge(TSharedPtr<GraphEdge> _edge);

	/**
	 * @brief Finds and returns a pointer to the node inside this graph
	 * @param _node The node to find
	 * @return A pointer of the node or a nullptr if not found
	 */
	TSharedPtr<GraphNode> FindNode(TSharedPtr<GraphNode> _node);

	/**
	 * @brief Returns an array of all the node's edges sorted by weight. This assumes the node is present inside the graph
	 * @param _node The node to validate
	 * @return An array with all the edges.
	 */
	TArray<TSharedPtr<GraphEdge>> GetRelevantEdges(TSharedPtr<GraphNode> _node);

	int32 GetTotalQuadrants();

	bool AlreadyConnectedWith(const TSharedPtr<GraphNode>& NodeA, const TSharedPtr<GraphNode>& NodeB)const;

	void RemoveNode(const TSharedPtr<GraphNode>& NodeToRemove);

private:
	/**
	 * @brief The array of nodes present in the graph
	 */
	TArray<TSharedPtr<GraphNode>> m_lNodes;

	/**
	 * @brief The array of edges present in the graph
	 */
	TArray<TSharedPtr<GraphEdge>> m_lEdges;
};
//EOF