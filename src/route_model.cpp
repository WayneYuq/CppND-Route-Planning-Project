#include "route_model.h"
#include <iostream>

RouteModel::RouteModel(const std::vector<std::byte> &xml) : Model(xml) {
  int couter = 0;
  for (Model::Node node : this->Nodes()) {
    m_Nodes.push_back(Node(couter, this, node));
    couter++;
  }
  CreateNodeToRoadHashmap();
}
void RouteModel::CreateNodeToRoadHashmap()
{
  for (const Model::Road &road : Roads()) {
    if (road.type != Model::Road::Type::Footway) {
      for (int node_idx : Ways()[road.way].nodes) {
        if (node_to_road.find(node_idx) == node_to_road.end()) {
          node_to_road[node_idx] = std::vector<const Model::Road *> ();
        }
        node_to_road[node_idx].push_back(&road);
      }
    }
  }
}

RouteModel::Node *RouteModel::Node::FindNeighbor(std::vector<int> node_indices)
{
  Node *closet_node = nullptr;
  Node node;
  for (int node_index : node_indices) {
    node = parent_model->SNodes()[node_index];
    if (!node.visited && distance(node) != 0) {
      if (closet_node == nullptr || distance(node) < distance(*closet_node)) {
        closet_node = &parent_model->SNodes()[node_index];
      }
    }
  }
  return closet_node;
}

void RouteModel::Node::FindNeighbors()
{
  RouteModel::Node *closet_node = nullptr;
  for (const auto road_ptr : parent_model->node_to_road[this->index]) {
    closet_node = FindNeighbor(parent_model->Ways()[road_ptr->way].nodes);
    if (closet_node) neighbors.push_back(closet_node);
  }
}

RouteModel::Node &RouteModel::FindClosestNode(float x, float y)
{
  Node node;
  node.x = x;
  node.y = y;

  float min_dist = std::numeric_limits<float>::max();
  float dist;
  int closest_idx;

  for (const auto &road : Roads()) {
    if (road.type != Model::Road::Type::Footway) {
      for (int node_idx : Ways()[road.way].nodes) {
        dist = node.distance(SNodes()[node_idx]);
        if (dist < min_dist) {
          min_dist = dist;
          closest_idx = node_idx;
        }
      }
    }
  }
  return SNodes()[closest_idx];
}
