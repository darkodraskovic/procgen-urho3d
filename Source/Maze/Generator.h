#pragma once

#include <Urho3D/Urho3DAll.h>

namespace Maze {

enum CellType {
  CELL_EMPTY = 0,
  CELL_WALL,
};

class Generator {
  void Start();

 public:
  explicit Generator();

  void SetNode(Node* node);

  void InitializeMap(unsigned int width, unsigned int height);
  virtual void Generate();
  void Draw();
  int CountSquareNeighbors(int x, int z);
  int CountDiagonalNeighbors(int x, int z);
  int CountAllNeighbors(int x, int z);

 protected:
  WeakPtr<Node> node_;
  WeakPtr<Node> parent_;
  Vector<Vector<unsigned>> map_;
  unsigned int width_;
  unsigned int height_;
};
}  // namespace Maze
