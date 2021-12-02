#ifndef _CAMERA_H_
#define _CAMERA_H_

// #include "Frustum.h"
#include "vmath.h"

class Camera {
public:
	// pos: camera position in cartesian coordinates
	// dir: camera DIRECTION in cartesian coordinates
	Vector3f	pos, dir, up;
	float		theta, phi;               // camera DIRECTION in spherical coordinates

	float		nearP, farP, angle, ratio;

	// Frustum		frustum;
	bool		frustumOn;
	bool		accelerating;

	// because the direction vector is unit length, and we probably don't want
	// to move one full unit every time a button is pressed, just create a constant
	// to keep track of how far we want to move at each step. you could make
	// this change w.r.t. the amount of time the button's held down for
	// simple scale-sensitive movement!
	float		movementConstant;
	float		velocityChange;

	Camera() : up(0.0f, 1.0f, 0.0f) {
		movementConstant = 0.1f;
		velocityChange = 0.01f;
		nearP = 0.1f; farP = 600.0f;
		angle = 60.0f; ratio = 1.0f;
		frustumOn = true;
		accelerating = true;
	}

	void	setPosition(float x, float y, float z) {
		pos.x = x; pos.y = y; pos.z = z;
	}
	void	setDirectionSpherical(float _theta, float _phi) {
		theta = _theta; phi = _phi;
		recomputeOrientation();
	}

	void	recomputeOrientation();
	void	updateFrustum() {
		// frustum.setCamDef(pos, pos + dir, up);
	}
	void	updateCameraInternals() {
		// frustum.setCamInternals(angle, ratio, nearP, farP);
	}
	void	decreaseCameraVelocity();
	void	increaseCameraVelocity();

	void	MoveSide(float angleMov);
	void	Accelerate();
};

/*
 * Updates the camera's position in cartesian coordinates based
 *  on its position in spherical coordinates. Should be called every time
 *  cameraTheta, cameraPhi, or cameraRadius is updated.
 */
void Camera::recomputeOrientation() {
    dir.x =  sinf(theta) * sinf(phi);
    dir.z = -cosf(theta) * sinf(phi);
    dir.y = -cosf(phi);

    // And normalize this directional vector!
	dir.normalize();
}

void Camera::increaseCameraVelocity() {
    movementConstant += velocityChange;
    if (movementConstant > 10.0f)
        movementConstant = 10.0f;
}

void Camera::decreaseCameraVelocity() {
    movementConstant -= velocityChange;
    if (movementConstant < 0.01f)
        movementConstant = 0.01f;
}

void Camera::MoveSide(float angleMov) {
	Vector3f _dir;
	_dir.x = sinf(theta + (float)M_PI / angleMov) * sinf(phi);
	_dir.z = -cosf(theta + (float)M_PI / angleMov) * sinf(phi);
	_dir.y = -cosf(phi);
	//and normalize this directional vector!
	_dir.normalize();
	//just updating X and Z camera positions
	pos.x += _dir.x * movementConstant;
	pos.z += _dir.z * movementConstant;
}

void Camera::Accelerate() {
	
}

#endif
