#include "Camera2D.h"
#include "Random.hpp"
namespace ZEngine {

    Camera2D::Camera2D() : _position(0.0f, 0.0f),
        _cameraMatrix(1.0f),
        _orthoMatrix(1.0f),
        _scale(1.0f),
        _needsMatrixUpdate(true),
        _screenWidth(500),
        _screenHeight(500)
    {
        m_timer.start();
    }


    Camera2D::~Camera2D()
    {
    }

    void Camera2D::init(int screenWidth, int screenHeight) {
        _screenWidth = screenWidth;
        _screenHeight = screenHeight;
        _orthoMatrix = glm::ortho(0.0f, (float)_screenWidth, 0.0f, (float)_screenHeight);
		setPosition(glm::vec2(_screenWidth / 2, _screenHeight / 2));
    }

    //updates the camera matrix if needed
    void Camera2D::update() {

        //Only update if our position or scale have changed
		if (m_shaking) {
            if (m_timer.elapsed() >= m_shakeTime) {
				m_shaking = false;
				_needsMatrixUpdate = true;
			}
            if (m_shakeTimer.elapsed() >= SHAKE_UPDATE_INTERVAL) {
				float rand1 = randomReal < float >(-20.0f, 20.0f);
				float rand2 = randomReal < float >(-20.0f, 20.0f);
				m_shakingPosition = _position + glm::vec2(m_shakeIntensity * rand1, m_shakeIntensity * rand2);
				m_actualPosition = _position;
                m_shakeTimer.restart();
			}
			const glm::vec2 destVec = m_shakingPosition - _position;
			const glm::vec2 dir = glm::normalize(destVec);
			const float length = glm::length(destVec);
			if (length > m_shakeIntensity) {
				m_actualPosition += dir * m_shakeIntensity;
			}
			glm::vec3 translate(-(int)m_actualPosition.x + _screenWidth / 2, -(int)m_actualPosition.y + _screenHeight / 2, 0.0f);
			_cameraMatrix = glm::translate(_orthoMatrix, translate);

			glm::vec3 scale(_scale, _scale, 0.0f);
			_cameraMatrix = glm::scale(glm::mat4(1.0f), scale) * _cameraMatrix;
		}
        else if (_needsMatrixUpdate) {
            
            //Camera Translation
            glm::vec3 translate(-(int)_position.x + _screenWidth / 2, -(int)_position.y + _screenHeight / 2, 0.0f);
            _cameraMatrix = glm::translate(_orthoMatrix, translate);

            //Camera Scale
            glm::vec3 scale(_scale, _scale, 0.0f);
            _cameraMatrix = glm::scale(glm::mat4(1.0f), scale) * _cameraMatrix;

            _needsMatrixUpdate = false;
        }
    }

    glm::vec2 Camera2D::convertScreenToWorld(glm::vec2 screenCoords) const {
        // Make it so that 0 is the center
        screenCoords -= glm::vec2(_screenWidth / 2, _screenHeight / 2);
        // Scale the coordinates
        screenCoords /= _scale;
        // Translate with the camera position
        screenCoords += _position;
        return screenCoords;
    }

    // Simple AABB test to see if a box is in the camera view
    bool Camera2D::isBoxInView(const glm::vec2& position, const glm::vec2& dimensions) const {

        glm::vec2 scaledScreenDimensions = glm::vec2(_screenWidth, _screenHeight) / (_scale);

        // The minimum distance before a collision occurs
        const float MIN_DISTANCE_X = dimensions.x / 2.0f + scaledScreenDimensions.x / 2.0f;
        const float MIN_DISTANCE_Y = dimensions.y / 2.0f + scaledScreenDimensions.y / 2.0f;

        // Center position of the parameters
        glm::vec2 centerPos = position + dimensions / 2.0f;
        // Center position of the camera
        glm::vec2 centerCameraPos = _position;
        // Vector from the input to the camera
        glm::vec2 distVec = centerPos - centerCameraPos;

        // Get the depth of the collision
        float xDepth = MIN_DISTANCE_X - abs(distVec.x);
        float yDepth = MIN_DISTANCE_Y - abs(distVec.y);

        // If both the depths are > 0, then we collided
        if (xDepth > 0 && yDepth > 0) {
            // There was a collision
            return true;
        }
        return false;
    }
	void Camera2D::shake(float intensity, float time) {
		m_shaking = true;
		m_timer.restart();
		m_shakeIntensity = intensity;
		m_shakeTime = time;
	}
	void Camera2D::setPosition(const glm::vec2& newPosition) {
		_position = newPosition;
		updateBorders();
		_needsMatrixUpdate = true;
	}
	void Camera2D::offsetPosition(const glm::vec2& offset) {
		_position += offset;
		updateBorders();
		_needsMatrixUpdate = true;
	}
	void Camera2D::setBorders(const glm::vec4 &borders) {
		m_borders = borders;
		m_bordred = true;
		updateBorders();
		_needsMatrixUpdate = true;
	}
	void Camera2D::updateBorders() {
		if (m_bordred) {
			if (_position.x < m_borders.x)_position.x = m_borders.x;
			else if (_position.x > m_borders.y)_position.x = m_borders.y;
			if (_position.y < m_borders.z)_position.y = m_borders.z;
			else if (_position.y > m_borders.w)_position.y = m_borders.w;
		}
	}
}
