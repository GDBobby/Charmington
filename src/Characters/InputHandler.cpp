#include "InputHandler.h"

namespace EWE {
	CharmerKeys CharmerInput::pollInput() {
		CharmerKeys returnBuffer = liveActions;
		liveActions = CharmerKeys{};
	}
}