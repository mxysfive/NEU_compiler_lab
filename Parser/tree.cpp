//
// Created by MengXiangyu on 2022/10/25.
//
#include "tree.h"
#include <iostream>

ASTTreeNode::ASTTreeNode(ASTNodeKind kind, string &value, ASTTreeNode *left, ASTTreeNode *mid, ASTTreeNode *right) {
    kind_ = kind;
    value_ = value;
    left_ = left;
    mid_ = mid;
    right_ = right;
}

ostream &operator<<(ostream &out, ASTTreeNode *node) {
    out<<"{ "<<"NodeKind: "<<node->kind_<<", NodeValue: "<<node->value_<<"}"<<endl;
    return out;
}



// Make a node that have three child
ASTTreeNode *MkTripleChildNode(ASTNodeKind kind, string &value,
                               ASTTreeNode *left_child, ASTTreeNode *mid_child, ASTTreeNode *right_child) {
    return new ASTTreeNode(kind, value, left_child, mid_child, right_child);
}

// Make a node that have two child
ASTTreeNode *MkBinaryChildNode(ASTNodeKind kind, string &value, ASTTreeNode *left_child, ASTTreeNode *right_child) {
    return new ASTTreeNode(kind, value, left_child, nullptr, right_child);
}

// Make a node that have one child
ASTTreeNode *MkUnaryChildNode(ASTNodeKind kind, string &value, ASTTreeNode *unary_child) {
    return new ASTTreeNode(kind, value, unary_child, nullptr, nullptr);
}

// Make a leaf node (have no child)
ASTTreeNode *MkUnaryChildNode(ASTNodeKind kind, string &value) {
    return new ASTTreeNode(kind, value, nullptr, nullptr, nullptr);
}


