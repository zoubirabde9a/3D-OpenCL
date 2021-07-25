#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <unordered_map>
#include <glm/glm.hpp>

namespace ZEngine {

// Input manager stores a key map that maps Keys to booleans.
// If the value in the key map is true, then the key is pressed.
// Otherwise, it is released.

class InputManager
{
public:
    InputManager();
    ~InputManager();

    void update();

    void pressKey(unsigned int keyID);
    void releaseKey(unsigned int keyID);

    void setMouseCoords(float x, float y);
    void setMouseRel(const glm::vec2 &mouseRel) {m_mouseRel = mouseRel; }
    void setMouseMotion(bool mouseMotion) { m_mouseMotion = mouseMotion; }

    /// Returns true if the key is held down
    bool isKeyDown(unsigned int keyID) const;

    /// Returns true if the key was just pressed
    bool isKeyPressed(unsigned int keyID) const;
    bool isKeyReleased(unsigned int keyID) const;

    //getters
    glm::vec2 getMouseCoords() const { return _mouseCoords; }
    const glm::vec2 &getMouseRel() const { return m_mouseRel; }
    bool isMouseMotion() const { return m_mouseMotion; }
private:
    /// Returns true if the key is held down
    bool wasKeyDown(unsigned int keyID) const;
    std::unordered_map<unsigned int, bool> _keyMap;
    std::unordered_map<unsigned int, bool> _previousKeyMap;
    glm::vec2 _mouseCoords;
    glm::vec2 m_mouseRel;
    bool m_mouseMotion = false;
};

}


#endif // INPUTMANAGER_H
