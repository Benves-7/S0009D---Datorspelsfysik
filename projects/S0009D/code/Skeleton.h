//
// Created by benves-7 on 9/25/19.
//

#ifndef SKELETON_H
#define SKELETON_H

#include "tinyxml.h"
#include <vector>

#ifndef XMLCheckResult
#define XMLCheckResult(a_eResult) if (a_eResult != XML_SUCCESS) { printf("Error: %i\n", a_eResult); }
#endif

using namespace std;

struct Joint
{
    string name;
    int index, parent;
    vector<int> children;
    Matrix4D localTransform, worldspaceTransform;

    //inverseBindPose? 
};

class Skeleton
{
public:
    Skeleton() {}
    ~Skeleton() {}
    inline void worldSpaceConvertion()
    {
        for (int i = 0; i < joints->size(); i++)
        {
            if (joints->at(i).parent!=-1)
            {
                joints->at(i).worldspaceTransform = joints->at(joints->at(i).parent).worldspaceTransform * joints->at(i).localTransform;
            }
        }
    }
    inline void loadSkeleton(const char* filename)
    {
        TiXmlDocument doc(filename);
        if (!doc.LoadFile())
        {
            printf("Failed to load file \"%s\"\n", filename);
            return;
        }

        TiXmlElement *eJoint = doc.FirstChildElement("Nebula3")->FirstChildElement("Model")->FirstChildElement("CharacterNodes")->FirstChildElement("CharacterNode")->FirstChildElement("Joint");
        while (eJoint)
        {
            Joint joint;
            joint.name = eJoint->Attribute("name");
            joint.index = atoi(eJoint->Attribute("index"));
            joint.parent = atoi(eJoint->Attribute("parent"));

            if (joint.parent != -1)
            {
                joints->at(joint.parent).children.push_back(joint.index);
            }

            float temp[4];
            sscanf(eJoint->Attribute("position"), "%f,%f,%f,%f", &temp[0], &temp[1], &temp[2], &temp[3]);
            Vector4D position(temp);
            Matrix4D p = Matrix4D::getPositionMatrix(position);
            sscanf(eJoint->Attribute("rotation"), "%f,%f,%f,%f", &temp[0], &temp[1], &temp[2], &temp[3]);
            Vector4D rotation(temp);
            Matrix4D r = Matrix4D::getRotationFromQuaternian(rotation);
            sscanf(eJoint->Attribute("scale"),    "%f,%f,%f,%f", &temp[0], &temp[1], &temp[2], &temp[3]);
            Vector4D scale(temp);
            Matrix4D s = Matrix4D::getScaleMatrix(scale);

            joint.worldspaceTransform = p * r * s;
            joint.localTransform = joint.worldspaceTransform;

            joints->push_back(joint);
            eJoint = eJoint->NextSiblingElement("Joint");
        }
        worldSpaceConvertion();
    }
    inline void update(int index)
    {
        Joint& joint = joints->at(index);
        //std::cout << "index: " << index << std::endl;
        //joint.worldspaceTransform.printPosition();
        if (joint.parent == -1)
        {
            joint.worldspaceTransform = joint.localTransform;
        }
        else
        {
            joint.worldspaceTransform = joints->at(joint.parent).worldspaceTransform * joint.localTransform;
        }
        for (int i = 0; i < joint.children.size(); i++)
        {
            update(joint.children.at(i));
        }
        return;
    }

    vector<Joint>* joints = new vector<Joint>();
};


#endif //SKELETON_H
