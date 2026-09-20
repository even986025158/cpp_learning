/*
 * 本文件只证明：tag 放在结构体第一个字段，才能把派生节点当成 Node* 并先看 tag 再读特有字段。
 * 不证明：完整的 PG Node 体系、List、拷贝/序列化节点。
 */
#include "check.h"
#include <stddef.h>
#include <string.h>

typedef enum NodeTag { NODE_INT = 1, NODE_CSTR = 2 } NodeTag;

typedef struct Node {
    NodeTag tag;
} Node;

typedef struct IntNode {
    NodeTag tag;
    int value;
} IntNode;

typedef struct CStrNode {
    NodeTag tag;
    const char *text;
} CStrNode;

static int node_as_int(const Node *n, int *out)
{
    if (n == NULL || n->tag != NODE_INT || out == NULL) {
        return 0;
    }
    *out = ((const IntNode *)n)->value;
    return 1;
}

int main(void)
{
    IntNode in;
    in.tag = NODE_INT;
    in.value = 42;

    CStrNode sn;
    sn.tag = NODE_CSTR;
    sn.text = "ok";

    Node *a = (Node *)&in;
    Node *b = (Node *)&sn;

    FD_CHECK(a->tag == NODE_INT, "int node tag is visible through Node*");
    FD_CHECK(b->tag == NODE_CSTR, "cstr node tag is visible through Node*");
    FD_CHECK(offsetof(IntNode, tag) == 0, "tag is the first field of IntNode");
    FD_CHECK(offsetof(CStrNode, tag) == 0, "tag is the first field of CStrNode");

    int value = 0;
    FD_CHECK(node_as_int(a, &value) == 1, "tag check allows reading IntNode.value");
    FD_CHECK(value == 42, "downcast value is 42");
    FD_CHECK(node_as_int(b, &value) == 0, "wrong tag refuses the downcast");
    FD_CHECK(strcmp(sn.text, "ok") == 0, "cstr payload stays intact");
    return 0;
}
