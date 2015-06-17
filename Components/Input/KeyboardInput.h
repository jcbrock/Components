#ifndef KEYBOARDINPUT_H
#define KEYBOARDINPUT_H

// Include GLFW
#include <glfw3.h>
#include <unordered_map>
#include <vector>
//#include "KeyboardObserverInterface.h"

class KeyboardInput //: public KeyboardObserverInterface
{
public:
    KeyboardInput();

    void Loop(GLFWwindow* window);

    //void Register(KeyboardObserverInterface* observer);
private:
    void CheckKeyAction(GLFWwindow* window, int key);
    //void NotifyObservers(int key, int action) const;
    //void AddKeyEvent(std::string eventName, int key, int action);
private:
    std::unordered_map<int, bool> previousKeyValues;
    //<key value <action value, event>>
    //std::unordered_map<int, std::unordered_map<int, unsigned int>> mKeysToDetect;
    std::vector<int> mKeysToDetect;
    //std::vector<KeyboardObserverInterface*> mObservers;
};

#endif