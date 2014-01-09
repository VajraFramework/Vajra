#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/Input/Input.h"
#include "Vajra/Engine/Input/ITouchTarget.h"

IGameTouchTarget::IGameTouchTarget() {
	ENGINE->GetInput()->AddGameTouchTarget(this);
}
