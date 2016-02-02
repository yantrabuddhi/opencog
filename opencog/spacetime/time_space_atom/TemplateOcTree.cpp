#include "TemplateOcTree.h"
namespace octomap {
  // tree implementation  --------------------------------------
  TemplateOcTree::TemplateOcTree(double resolution)
  : OccupancyOcTreeBase<TemplateOcTreeNode>(resolution) {
    templateOcTreeMemberInit.ensureLinking();
  };

  TemplateOcTreeNode* TemplateOcTree::setNodeData(const OcTreeKey& key, 
                                             const T& tDat){
    TemplateOcTreeNode* n = search (key);
    if (n != 0) {
      n->setData(tDat);//setColor 
    }
    return n;
  }
/* This may not be required ..  
  void TemplateOcTree::updateInnerOccupancy() {
    this->updateInnerOccupancyRecurs(this->root, 0);
  }

  void TemplateOcTree::updateInnerOccupancyRecurs(TemplateOcTreeNode* node, unsigned int depth) {
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

  std::ostream& operator<<(std::ostream& out, T const& c) {
    return out << '(' << c << ')';
  }
*/

  TemplateOcTree::StaticMemberInitializer TemplateOcTree::colorOcTreeMemberInit;

} // end namespace
