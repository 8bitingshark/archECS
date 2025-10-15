#pragma once

namespace core::key {

	enum KeyCode {

		LeftMouseButton   = 1,
		RightMouseButton  = 2,
		Cancel            = 3,
		MiddleMouseButton = 4,
		Escape            = 27,
		Space             = 32,
		
	};
}

// Same of WinUser.h

#define KDEF_LBUTTON        0x01
#define KDEF_RBUTTON        0x02
#define KDEF_CANCEL         0x03
#define KDEF_MBUTTON        0x04    /* NOT contiguous with L & RBUTTON */
#define KDEF_ESCAPE         0x1B
