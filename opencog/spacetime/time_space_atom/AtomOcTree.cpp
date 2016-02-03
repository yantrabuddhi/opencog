#include "AtomOcTree.h"
namespace octomap {
  // tree implementation  --------------------------------------
  AtomOcTree::AtomOcTree(double resolution)
  : OccupancyOcTreeBase< AtomOcTreeNode >(resolution) {
    atomOcTreeMemberInit.ensureLinking();
  }

  AtomOcTreeNode* AtomOcTree::setNodeData(const OcTreeKey& key, 
                                             const Handle& r){
    AtomOcTreeNode *n = search(key);
    if (n != 0) {
      n->setData(r);//setColor 
    }
    return n;
  }
/* This may not be required ..  
  template <class T>
  void AtomOcTree<T>::updateInnerOccupancy() {
    this->updateInnerOccupancyRecurs(this->root, 0);
  }

  template <class T>
  void AtomOcTree<T>::updateInnerOccupancyRecurs(AtomOcTreeNode<T>* node, unsigned int depth) {
    // only recurse and update for inner nodes:
    if (node->hasChildren()){
      // return early for last level:
      if (depth < this->tree_depth){
        for (unsigned int i=0; i<8; i++) {
          if (node->childExists(i)) {
            updateInnerOccupancyRecurs(node->getChild(i), depth+1);
          }
        }
      }
      node->updateOccupancyChildren();
      //node->updateTemplateChildren();//
    }
  }

  std::ostream& operator<<(std::ostream& out, Handle const& c) {
    return out << '(' << c << ')';
  }
*/

  //typedef int Handle;
  AtomOcTree::StaticMemberInitializer AtomOcTree::atomOcTreeMemberInit;

} // end namespace
