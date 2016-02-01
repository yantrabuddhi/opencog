/*
 * OctoMap - An Efficient Probabilistic 3D Mapping Framework Based on Octrees
 * http://octomap.github.com/
 *
 * Copyright (c) 2009-2013, K.M. Wurm and A. Hornung, University of Freiburg
 * All rights reserved.
 * License: New BSD
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the University of Freiburg nor the names of its
 *       contributors may be used to endorse or promote products derived from
 *       this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <octomap/TemplateOcTree.h>

namespace octomap {


  // node implementation  --------------------------------------
  std::ostream& TemplateOcTreeNode::writeValue (std::ostream &s) const {
    // 1 bit for each children; 0: empty, 1: allocated
    std::bitset<8> children;
    for (unsigned int i=0; i<8; i++) {
      if (childExists(i)) children[i] = 1;
      else                children[i] = 0;
    }
    char children_char = (char) children.to_ulong();
    
    // write node data
    s.write((const char*) &value, sizeof(value)); // occupancy
    s.write((const char*) &dat, sizeof(T)); // dat
    s.write((char*)&children_char, sizeof(char)); // child existence

    // write existing children
    for (unsigned int i=0; i<8; ++i) 
      if (children[i] == 1) this->getChild(i)->writeValue(s);    
    return s;
  }

  std::istream& TemplateOcTreeNode::readValue (std::istream &s) {
    // read node data
    char children_char;
    s.read((char*) &value, sizeof(value)); // occupancy
    s.read((char*) &dat, sizeof(T)); // dat
    s.read((char*)&children_char, sizeof(char)); // child existence

    // read existing children
    std::bitset<8> children ((unsigned long long) children_char);
    for (unsigned int i=0; i<8; i++) {
      if (children[i] == 1){
        createChild(i);
        getChild(i)->readValue(s);
      }
    }
    return s;
  }
/*
  TemplateOcTreeNode::T TemplateOcTreeNode::getAverageChildColor() const {
    int mr(0), mg(0), mb(0);
    int c(0);
    for (int i=0; i<8; i++) {
      if (childExists(i) && getChild(i)->isColorSet()) {
        mr += getChild(i)->getColor().r;
        mg += getChild(i)->getColor().g;
        mb += getChild(i)->getColor().b;
        ++c;
      }
    }
    if (c) {
      mr /= c;
      mg /= c;
      mb /= c;
      return T((unsigned char) mr, (unsigned char) mg, (unsigned char) mb);
    }
    else { // no child had a dat other than white
      return T(255, 255, 255);
    }
  }


  void TemplateOcTreeNode::updateColorChildren() {      //not needed mandeep
    dat = getAverageChildColor();
  }
*/
  // pruning =============

  bool TemplateOcTreeNode::pruneNode() {
    // checks for equal occupancy only, dat ignored
    if (!this->collapsible()) return false;
    // set occupancy value 
    setLogOdds(getChild(0)->getLogOdds());
    // set dat to average dat
    ////if (isColorSet()) dat = getAverageChildColor();//commented by mandeep
    // delete children
    for (unsigned int i=0;i<8;i++) {
      delete children[i];
    }
    delete[] children;
    children = NULL;
    return true;
  }

  void TemplateOcTreeNode::expandNode() {
    assert(!hasChildren());
    for (unsigned int k=0; k<8; k++) {
      createChild(k);
      children[k]->setValue(value);
      getChild(k)->setData(dat);
    }
  }

  // tree implementation  --------------------------------------
  TemplateOcTree::TemplateOcTree(double resolution)
  : OccupancyOcTreeBase<TemplateOcTreeNode>(resolution) {
    colorOcTreeMemberInit.ensureLinking();
  };

  TemplateOcTreeNode* TemplateOcTree::setNodeData(const OcTreeKey& key, 
                                             const T& tDat){
    TemplateOcTreeNode* n = search (key);
    if (n != 0) {
      n->setData(tDat);//setColor 
    }
    return n;
  }
/* Need this for value search
  TemplateOcTreeNode* TemplateOcTree::averageNodeColor(const OcTreeKey& key, 
                                                 const unsigned char& r, 
                                                 const unsigned char& g, 
                                                 const unsigned char& b) {
    TemplateOcTreeNode* n = search (key);
    if (n != 0) {
      if (n->isColorSet()) {
        TemplateOcTreeNode::T prev_color = n->getColor();
        n->setColor((prev_color.r + r)/2, (prev_color.g + g)/2, (prev_color.b + b)/2); 
      }
      else {
        n->setColor(r, g, b);
      }
    }
    return n;
  }
*/
 
  
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
      //node->updateColorChildren();
    }
  }

  std::ostream& operator<<(std::ostream& out, T const& c) {
    return out << '(' << c << ')';
  }


  TemplateOcTree::StaticMemberInitializer TemplateOcTree::colorOcTreeMemberInit;

} // end namespace

