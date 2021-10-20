#pragma once

#include "ControllerScheme.h"
#include "EventWithMouse.h"

class KeyboardControllerScheme final : public ControllerScheme {
  public:
    ControllerSchemeState GetInput(const EventWithMouse &) const;
};
