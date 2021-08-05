#ifndef RRT_H
#define RRT_H

#include <cstdlib>
#include "obstacles.h"
#include <QPoint>
#include <stdlib.h>
#include <vector>
#include <math.h>
#include "datas.h"
#include "object.h"
#include "datatrans.h"
#include "robot.h"
extern "C" {
#include "extApi.h"
}
using namespace std;
using namespace Eigen;



struct Node {
    vector<Node *> children;
    Node *parent;
    Vector2f position;
};

class RRT
{
public:
    RRT();
    void initialize();
    Node* getRandomNode();
    Node* nearest(Vector2f point);
    int distance(Vector2f& p, Vector2f& q);
    Vector2f newConfig(Node* q, Node* qNearest);
    void add(Node* qNearest, Node* qNew);
    bool reached();
    void setStepSize(int step);
    void setMaxIterations(int iter);
    void deleteNodes(Node* root);
    void genShortPath();
    Obstacles* obstacles;
    vector<Node*> nodes;
    vector<Node*> path;
    vector<datas> guiShortPath;
    vector<datas> vrepShortPath;
    Node* root, * lastNode;
    Vector2f startPos, endPos;
    int max_iter;
    int step_size;
    float random();
};

#endif // RRT_H
