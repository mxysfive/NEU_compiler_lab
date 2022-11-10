//
// Created by MengXiangyu on 2022/10/25.
//

#pragma once
#include <string>

using namespace std;

enum ASTNodeKind{
    CONST_ILLUSTRATE,
    CONST_DEFINE,
    U_INT,
    S_INT,
};

class ASTTreeNode {
public:
    ASTTreeNode(ASTNodeKind kind, string &value, ASTTreeNode *left, ASTTreeNode *mid, ASTTreeNode *right);
    friend ostream &operator<<(ostream &out, ASTTreeNode *node);
private:
    ASTNodeKind kind_;
    string value_;
    ASTTreeNode *left_;
    ASTTreeNode *mid_;
    ASTTreeNode *right_;
};

// Make a node that have three child
ASTTreeNode *MkTripleChildNode(ASTNodeKind kind, string &value,
                         ASTTreeNode *left_child, ASTTreeNode *mid_child, ASTTreeNode *right_child);

// Make a node that have two child
ASTTreeNode *MkBinaryChildNode(ASTNodeKind kind, string &value, ASTTreeNode *left_child, ASTTreeNode *right_child);

// Make a node that have one child
ASTTreeNode *MkUnaryChildNode(ASTNodeKind kind, string &value, ASTTreeNode *unary_child);

// Make a leaf node (have no child)
ASTTreeNode *MkUnaryChildNode(ASTNodeKind kind, string &value);