#include "KeyboardInput.h"
#include <iostream>
#include <chrono>
#include <thread>
//#include "EventMapping.h"
#include <memory>
#include "../EventSystem/EventData.h"
#include "../EventSystem/Event.h"

extern std::vector<Event*> eventQueue;
KeyboardInput::KeyboardInput()
{
   /* AddKeyEvent("WPressed", GLFW_KEY_W, GLFW_PRESS);
    AddKeyEvent("APressed", GLFW_KEY_A, GLFW_PRESS);
    AddKeyEvent("SPressed", GLFW_KEY_S, GLFW_PRESS);
    AddKeyEvent("DPressed", GLFW_KEY_D, GLFW_PRESS);
    AddKeyEvent("RPressed", GLFW_KEY_R, GLFW_PRESS);
    AddKeyEvent("TPressed", GLFW_KEY_T, GLFW_PRESS);
    AddKeyEvent("UPPressed", GLFW_KEY_UP, GLFW_PRESS);
    AddKeyEvent("DOWNPressed", GLFW_KEY_DOWN, GLFW_PRESS);
    AddKeyEvent("WReleased", GLFW_KEY_W, GLFW_RELEASE);
    AddKeyEvent("AReleased", GLFW_KEY_A, GLFW_RELEASE);
    AddKeyEvent("SReleased", GLFW_KEY_S, GLFW_RELEASE);
    AddKeyEvent("DReleased", GLFW_KEY_D, GLFW_RELEASE);
    AddKeyEvent("RReleased", GLFW_KEY_R, GLFW_RELEASE);
    AddKeyEvent("TReleased", GLFW_KEY_T, GLFW_RELEASE);
    AddKeyEvent("UPReleased", GLFW_KEY_UP, GLFW_RELEASE);
    AddKeyEvent("DOWNReleased", GLFW_KEY_DOWN, GLFW_RELEASE);
    AddKeyEvent("WReleased", GLFW_KEY_W, GLFW_RELEASE);

    for (const auto& key : mKeysToDetect)
    {
        previousKeyValues[key.first] = false;
    }*/

    //dunno if this is the best place to map keyboard event to IN GAME event... but w/e
    //yeah...its not, but w/e
    //TODO - add observer pattern


    mKeysToDetect.push_back(GLFW_KEY_W);
    mKeysToDetect.push_back(GLFW_KEY_A);
    mKeysToDetect.push_back(GLFW_KEY_S);
    mKeysToDetect.push_back(GLFW_KEY_D);
    mKeysToDetect.push_back(GLFW_KEY_R);
    mKeysToDetect.push_back(GLFW_KEY_T);
    mKeysToDetect.push_back(GLFW_KEY_UP);
    mKeysToDetect.push_back(GLFW_KEY_DOWN);
}
/*
void KeyboardInput::AddKeyEvent(std::string eventName, int key, int action)
{
std::unique_ptr<EventId> eventId(new EventId()); //control gets past to EventMapping instance
EventMapping::Instance().AddEventMapping(eventId, eventName);

mKeysToDetect[key][action] = EventMapping::Instance().LookupEventId(eventName);
}
void KeyboardInput::Register(KeyboardObserverInterface* observer)
{
    mObservers.push_back(observer);
}

void KeyboardInput::NotifyObservers(int key, int action) const
{
    for (auto thing : mObservers)
    {
        thing->Notify((*(*mKeysToDetect.find(key)).second.find(action)).second, action);
    }
}
*/
void KeyboardInput::CheckKeyAction(GLFWwindow* window, int key)
{
    if (!previousKeyValues[key] && glfwGetKey(window, key) == GLFW_PRESS)
    {
        std::cout << "Key: " << key << " pressed!" << std::endl;
        previousKeyValues[key] = !previousKeyValues[key];

        if (key == GLFW_KEY_W)
        {
            Event* dummyEvt2 = new Event();
            dummyEvt2->type = EventType::MOVE_PADDLE;
            MovePaddleData* mvData = new MovePaddleData();
            mvData->priority = EventPriority::IMMEDIATE;
            // mvData.obj1 = &leftPaddle;
            //todo - hardcoding to left paddle
            mvData->destX = 0;
            mvData->destY = 0.1f;
            dummyEvt2->data = dynamic_cast<EventData*>(mvData);
            eventQueue.push_back(dummyEvt2); //need this to get copied.........
        }

        //NotifyObservers(key, GLFW_PRESS);
    }
    else if (previousKeyValues[key] && glfwGetKey(window, key) == GLFW_RELEASE)
    {
        std::cout << "Key: " << key << " released!" << std::endl;
        previousKeyValues[key] = !previousKeyValues[key];
        //NotifyObservers(key, GLFW_RELEASE);
    }
}

//Damn, side note: how the fuck do I handle keys held down. I suppose I look for
//up and down calls. Better hope my looping doesn't slow down and is responsive...

void KeyboardInput::Loop(GLFWwindow* window)
{
    //ok, how to make this more efficient? it was taking up like 12% cpu
    //cuz continuous loop

    //well, maybe only poll like 60 times a second (or maybe once a frame)
    //I mean, yeah... even those this is on another thread, doesn't mean it
    //can't fuck me.

    //ok, I figure this will poll 60 times a second, and notify MessageDispatcher
    //ok, so what is a good way to detect what keys have been pressed without
    //some massive if statement section. I mean, I don't have to check every
    //key, but some automation would be nice...
    //I DONT think you can really control the callback situation (frequency)
    //well, maybe that is fine, can I ensure it is on this thread?
    //meh, le fuck it. more of a convience thing if I can indeed use it...

    //its around 0.06 CPU when doing 60 fps. Will replacing the sleep with a
    //condition_variable lower that? Probably... but going to hold off for now
    //std::cout << "Looping" << std::endl;
    //previousKeyValues[GLFW_KEY_W] = false;

    //bool pressed = false;

    int counter = 0;

    //while (true)
    //{
        std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
        //auto foo = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start);
        for (const auto& key : mKeysToDetect)
        {
            CheckKeyAction(window, key);
        }
        //std::this_thread::sleep_for(std::chrono::milliseconds(1));
        
        auto foo = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start);
        
        //if it goes over 17, that is less than 60 FPS, but I'm going to set the
        //warning threshold higher
        //if (foo.count() > 10)
        if (foo.count() > 17)
        {
            std::cout << "WARNING: Input checking took longer than 10 ms. Actual time: " << foo.count() << std::endl;
            //std::cout << "sleeping for: " << std::chrono::duration_cast<std::chrono::milliseconds>(foo).count() << std::endl;
        }
       
        //std::this_thread::sleep_for(std::chrono::milliseconds(17) - (std::chrono::steady_clock::now() - start));

        ++counter;
        if (counter > 100)
        {
            return;
        }
    //}
}