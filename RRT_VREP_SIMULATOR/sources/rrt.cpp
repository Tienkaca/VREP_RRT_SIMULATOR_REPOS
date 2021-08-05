#include "rrt.h"

extern int clientID;
extern robot Robots;
extern datas sts;
extern object obs[NUMBER_OBS];

RRT::RRT()
{
	sts = Robots.getData();
    obstacles = new Obstacles;
    startPos.x() = round((sts.x + 2.5)/5*400);
    startPos.y() = round((sts.y + 2.5)/5*400);
    endPos.x() = END_POS_X;
    endPos.y() = END_POS_Y;
    root = new Node;
    root->parent = NULL;
    root->position = startPos;
    lastNode = root;
    nodes.push_back(root);
    step_size = 5;
    max_iter = 30000;
}

/**
 * @brief Initialize root node of RRT.
 */
float RRT::random()
{
    float ran = (float)(rand() % (RAND_MAX + 1)) / RAND_MAX;
    return ran;
}

void RRT::initialize()
{
    root = new Node;
    root->parent = NULL;
    root->position = startPos;
    lastNode = root;
    nodes.push_back(root);
}

/**
 * @brief Generate a random node in the field.
 * @return
 */
Node* RRT::getRandomNode()
{
    Node* ret;
    Vector2f point(random() * WORLD_WIDTH, random() * WORLD_HEIGHT);
    if (point.x() >= 0 && point.x() <= WORLD_WIDTH && point.y() >= 0 && point.y() <= WORLD_HEIGHT) {
        ret = new Node;
        ret->position = point;
        return ret;
    }
    return NULL;
}

/**
 * @brief Helper method to find distance between two positions.
 * @param p
 * @param q
 * @return
 */
int RRT::distance(Vector2f &p, Vector2f &q)
{
    Vector2f v = p - q;
    return sqrt(powf(v.x(), 2) + powf(v.y(), 2));
}

/**
 * @brief Get nearest node from a given configuration/position.
 * @param point
 * @return
 */
Node* RRT::nearest(Vector2f point)
{
    float minDist = 1e9;
    Node *closest = NULL;
    for(int i = 0; i < (int)nodes.size(); i++) {
        float dist = distance(point, nodes[i]->position);
        if (dist < minDist) {
            minDist = dist;
            closest = nodes[i];
        }
    }
    return closest;
}

/**
 * @brief Find a configuration at a distance step_size from nearest node to random node.
 * @param q
 * @param qNearest
 * @return
 */
Vector2f RRT::newConfig(Node *q, Node *qNearest)
{
    Vector2f to = q->position;
    Vector2f from = qNearest->position;
    Vector2f intermediate = to - from;
    intermediate = intermediate / intermediate.norm();
    Vector2f ret = from + step_size * intermediate;
    return ret;
}

/**
 * @brief Add a node to the tree.
 * @param qNearest
 * @param qNew
 */
void RRT::add(Node *qNearest, Node *qNew)
{
    qNew->parent = qNearest;
    qNearest->children.push_back(qNew);
    nodes.push_back(qNew);
    lastNode = qNew;
}

/**
 * @brief Check if the last node is close to the end position.
 * @return
 */
bool RRT::reached()
{
    if (distance(lastNode->position, endPos) < END_DIST_THRESHOLD)
        return true;
    return false;
}

void RRT::setStepSize(int step)
{
    step_size = step;
}

void RRT::setMaxIterations(int iter)
{
    max_iter = iter;
}

/**
 * @brief Delete all nodes using DFS technique.
 * @param root
 */
void RRT::deleteNodes(Node *root)
{
    for(int i = 0; i < (int)root->children.size(); i++) {
        deleteNodes(root->children[i]);
    }
    delete root;
}

void RRT::genShortPath()
{
	QPointF tempStart,tempEnd,lastTempEnd;
	 for(int i = 0; i < (int)path.size() - 1; i++) {
        if(i == 0)
        {
            tempStart.setX(endPos.x());
            tempStart.setY(endPos.y());
        }
        tempEnd.setX(path[i]->position.x());
        tempEnd.setY(path[i]->position.y());
        Vector2f point(tempStart.x(),tempStart.y()) ;
        if(obstacles->isSegmentInObstacle(point,path[i+1]->position) || i == ((int)path.size() - 2) )
        {
            if(i == ((int)path.size() - 2) )
            {
                //painter.drawLine(tempStart, QPointF(rrt->startPos.x(),rrt->startPos.y()));
                
                //GEN BLUE PATH
                guiShortPath.push_back({(float)tempStart.x(),(float)tempStart.y(),0});
                //vrepShortPath.push_back({Qt2Vrep_dataTransform(tempStart.x()),Qt2Vrep_dataTransform(tempStart.y()),0});
                guiShortPath.push_back({(float)startPos.x(),(float)startPos.y(),0});
                //vrepShortPath.push_back({Qt2Vrep_dataTransform(startPos.x()),Qt2Vrep_dataTransform(startPos.y()),0});
            }
            else
            {
                //GEN BLUE PATH
                guiShortPath.push_back({(float)tempStart.x(),(float)tempStart.y(),0});
                //vrepShortPath.push_back({Qt2Vrep_dataTransform(tempStart.x()),Qt2Vrep_dataTransform(tempStart.y()),0});
                //painter.drawLine(tempStart, tempEnd);
                tempStart.setX(tempEnd.x());
                tempStart.setY(tempEnd.y());
            }
            
        }
    }
    int s = guiShortPath.size();
    for (int i = 0; i<s;i++)
    {
        vrepShortPath.push_back({Qt2Vrep_dataTransform(guiShortPath[s-i-1].x),Qt2Vrep_dataTransform(guiShortPath[s-i-1].y),0});
    }
   	
}