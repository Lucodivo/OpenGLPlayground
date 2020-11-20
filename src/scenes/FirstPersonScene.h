#pragma once

#include "Scene.h"
#include "../Camera.h"

class FirstPersonScene : public Scene
{
public:
  FirstPersonScene();

  virtual void init(Extent2D windowExtent);
  virtual void deinit();
  virtual void inputStatesUpdated();

  void enableDefaultMouseCameraMovement(bool enable);
  void enableDefaultKeyboardCameraMovement(bool enable);
  void enableDefaultControllerCameraMovement(bool enable);

protected:
  Camera camera = Camera();
  float32 cameraMovementSpeed = 1.0f;
  bool mouseCameraMovementEnabled = true;
  bool keyboardCameraMovementEnabled = true;
  bool controllerCameraMovementEnabled = true;
};