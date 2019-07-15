#include "route_model.h"
#include <iostream>

RouteModel::RouteModel(const std::vector<std::byte> &xml) : Model(xml) {
  int couter = 0;
  for (Model::Node node : this->Nodes()) {
    this->m_Nodes.push_back(this->Node(couter, this, node));
    couter++;
  }
  CreateNodeToRoadHashmap();
}
void RouteModel::CreateNodeToRoadHashmap()
{
  for (const Model::Road &road : this->Roads()) {
    if (road.type != Model::Road::Type::Footway) {
      for (const int node_idx : this->Ways()[road.way].nodes) {
        if (node_to_road[node_idx] == node_to_road.end()) {
          node_to_road[node_idx] = std::vector<const Modal::Road*> ();
        }
        node_to_road[node_idx].push_back(&road);
      }
    }
  }
};