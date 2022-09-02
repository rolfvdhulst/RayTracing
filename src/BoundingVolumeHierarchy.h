//
// Created by rolf on 18-8-22.
//

#ifndef RAYTRACING_SRC_BOUNDINGVOLUMEHIERARCHY_H_
#define RAYTRACING_SRC_BOUNDINGVOLUMEHIERARCHY_H_
#include <utility>
#include <variant>
#include <vector>
#include <span>

#include "AABB.h"
#include "Random.h"
#include "Sphere.h"
#include "Rectangle.h"

using BVHIndex = u_int32_t;
constexpr BVHIndex INVALID_INDEX = std::numeric_limits<BVHIndex>::max();

enum class BVHNodeType : int{
  Node,
  Leaf
};

class BVHObject{
 public:
  template<typename X> BVHObject(X x) : object{x}{};

  [[nodiscard]] AABB boundingBox(Real offsetTime) const{
	return std::visit(
		overload{[&offsetTime](const SphereData &obj) -> AABB { return obj.boundingBox(offsetTime); },
				 [](const AARectangleData& obj) -> AABB {return obj.boundingBox();}
		}
		,object);
  }
  [[nodiscard]] std::optional<HitRecord> hit(const Ray& ray, Real tMin, Real tMax) const{
	return std::visit(overload{
	  [&](const SphereData& obj) {return obj.hit(ray,tMin,tMax);},
	  [&](const AARectangleData& obj) {return obj.hit(ray,tMin,tMax);}
	  },object);
  }
 private:
  std::variant<SphereData,AARectangleData> object; //TODO: keep data here or just a pointer?
};

class BVHObjectList{
  [[nodiscard]] AABB boundingBox(Real offsetTime) const{
	AABB boundingBox = objects[0].boundingBox(offsetTime);
	for(size_t i = 1; i < objects.size(); ++i){
	  boundingBox = AABB(boundingBox,objects[i].boundingBox(offsetTime));
	}
	return boundingBox;
  }
  [[nodiscard]] std::optional<HitRecord> hit(const Ray& ray, Real tMin, Real tMax) const{
	std::optional<HitRecord> hit;
	for(const auto& object : objects){
	  hit = closestHitRecord(hit,object.hit(ray,tMin,tMax));
	}
	return hit;
  }
 private:
  std::vector<BVHObject> objects;
};

class BVHNode{
 public:

  static BVHIndex populate(std::vector<BVHObject>& objects,
						   long beginIndex,long endIndex,
				std::vector<BVHNode>& nodes, Real offsetTime,
				RandomDevice& device){

	int axis = device.randomUInt() % 3;

	auto objBegin = objects.begin()+beginIndex;
	auto objEnd = objects.begin()+endIndex;

	std::sort(objBegin, objEnd,
			  [axis,offsetTime](const BVHObject& a, const BVHObject& b) -> bool {
	  AABB leftBox = a.boundingBox(offsetTime);
	  AABB rightBox = b.boundingBox(offsetTime);
	  return leftBox.minimum()[axis] < rightBox.minimum()[axis];
	});

	long size = endIndex-beginIndex;

	constexpr long minNodeSpan = 2;

	if(size <= minNodeSpan){
	  BVHNode node;
	  node.nodeType = BVHNodeType::Leaf;
	  node.left = BVHIndex(beginIndex);
	  node.right = BVHIndex(endIndex-beginIndex);
	  {
		auto it = objects.begin()+beginIndex;
		AABB surroundingBoundingBox = it->boundingBox(offsetTime);
		++it;
		while(it != (objects.begin()+endIndex)){
		  surroundingBoundingBox = AABB(surroundingBoundingBox,it->boundingBox(offsetTime));
		  ++it;
		}
		node.aabb = surroundingBoundingBox;
	  }

	  BVHIndex index = static_cast<BVHIndex>(nodes.size());
	  nodes.push_back(node);
	  return index;
	}else{
	  long splitIndex = beginIndex + (endIndex-beginIndex)/2 ;

	  assert(splitIndex != beginIndex && splitIndex != endIndex);

	  BVHNode node;
	  node.nodeType = BVHNodeType::Node;
	  node.left = populate(objects,beginIndex,splitIndex,nodes,offsetTime,device);
	  node.right = populate(objects,splitIndex,endIndex,nodes,offsetTime,device);
	  node.aabb = AABB(nodes[node.left].aabb,nodes[node.right].aabb);

	  BVHIndex index = static_cast<BVHIndex>(nodes.size());
	  nodes[node.left].parentIdx = index;
	  nodes[node.right].parentIdx = index;
	  nodes.push_back(node);
	  return index;
	}
  }

  [[nodiscard]] const AABB& box() const {return aabb;}
  [[nodiscard]] BVHIndex parent() const {return parentIdx;}
  [[nodiscard]] BVHIndex leftChild() const {return left;}
  [[nodiscard]] BVHIndex rightChild() const {return right;}
  [[nodiscard]] BVHNodeType type() const {return nodeType;}
 private:
  AABB aabb;
  BVHNodeType nodeType;
  BVHIndex left;
  BVHIndex right;
  BVHIndex parentIdx;
};


class BVH{
 public:
  BVH() = default;
  explicit BVH(std::vector<BVHObject> inObjects, Real offsetTime,RandomDevice& device) : objects(std::move(inObjects)){
	root = BVHNode::populate(objects,0,long(objects.size()),nodes,offsetTime,device);
  };

  [[nodiscard]] std::optional<HitRecord> hit(const Ray& ray, Real tMin, Real tMax) const{
	BVHIndex index = nodes[root].leftChild();
	enum class State{
	  parent,
	  sibling,
	  child
	};
	State state = State::parent;
	std::optional<HitRecord> hit = std::nullopt;
	for(;;){
	  switch(state){
		case State::parent:{
		  const BVHNode& current = nodes[index];
		  if(current.box().hit(ray,tMin,tMax)){
			if(current.type() == BVHNodeType::Leaf){
			  //current.leftChild() or index?
			  size_t objectIndex = current.leftChild();
			  uint32_t numInList = current.rightChild();
			  for (uint32_t i = 0; i < numInList ; ++i) {
				hit = closestHitRecord(hit,objects[objectIndex].hit(ray,tMin,tMax));
				//TODO: update ray search interval tMin tMax based on collision
				objectIndex++;
			  }


			  index = nodes[current.parent()].rightChild();
			  state = State::sibling;
			}else{
			  //Traverse left subtree
			  index = current.leftChild();
			  state = State::parent;
			}
		  }else{
			//Go to right subtree
			index = nodes[current.parent()].rightChild();
			state = State::sibling;
		  }
		} break;
		//visit right nodes from left node
		case State::sibling:{
		  const BVHNode& current = nodes[index];
		  if(current.box().hit(ray,tMin,tMax)){
			if(current.type() == BVHNodeType::Leaf){
			  //current.leftChild() or index?
			  size_t objectIndex = current.leftChild();
			  uint32_t numInList = current.rightChild();
			  for (uint32_t i = 0; i < numInList ; ++i) {
				hit = closestHitRecord(hit,objects[objectIndex].hit(ray,tMin,tMax));
				//TODO: update ray search interval tMin tMax based on collision
				objectIndex++;
			  }

			  index = current.parent();
			  state = State::child;
			}else{
			  //traverse left subtree
			  index = current.leftChild();
			  state = State::parent;
			}
		  }else{
			index = current.parent();
			state = State::child;
		  }
		} break;
		//From right child back to parent, traverse up the tree
		case State::child: {
		  	if(index == root) return hit;
			BVHIndex parentIndex = nodes[index].parent();
			const BVHNode& parent = nodes[parentIndex];
			//traverse right subtree
			if(index == parent.leftChild()){
			  index = parent.rightChild();
			  state = State::sibling;
			}else{
			  //Finished, go up again
			  index = parentIndex;
			  state = State::child;
			}
			break;
		}
	  }
	}
  }
 private:
  BVHIndex root;
  std::vector<BVHObject> objects;

  std::vector<BVHNode> nodes;
};

#endif //RAYTRACING_SRC_BOUNDINGVOLUMEHIERARCHY_H_
