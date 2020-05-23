#pragma once

#ifdef GLFWKEYMAP_INPUTMANAGERPRIVATE

#include "Input/InputKeyEnum.h"
#include "Common/DebugMacros.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <map>

inline static const std::map<unsigned short, EInputKey>& GetGLFWKeyMap()
{
	// This map should be in the same order as the EInputKey enum
	static const std::map<unsigned short, EInputKey> GLFWKeyMap =
	{
		{ GLFW_KEY_A,				EInputKey::A				},
		{ GLFW_KEY_B,				EInputKey::B				},
		{ GLFW_KEY_C,				EInputKey::C				},
		{ GLFW_KEY_D,				EInputKey::D				},
		{ GLFW_KEY_E,				EInputKey::E				},
		{ GLFW_KEY_F,				EInputKey::F				},
		{ GLFW_KEY_G,				EInputKey::G				},
		{ GLFW_KEY_H,				EInputKey::H				},
		{ GLFW_KEY_I,				EInputKey::I				},
		{ GLFW_KEY_J,				EInputKey::J				},
		{ GLFW_KEY_K,				EInputKey::K				},
		{ GLFW_KEY_L,				EInputKey::L				},
		{ GLFW_KEY_M,				EInputKey::M				},
		{ GLFW_KEY_N,				EInputKey::N				},
		{ GLFW_KEY_O,				EInputKey::O				},
		{ GLFW_KEY_P,				EInputKey::P				},
		{ GLFW_KEY_Q,				EInputKey::Q				},
		{ GLFW_KEY_R,				EInputKey::R				},
		{ GLFW_KEY_S,				EInputKey::S				},
		{ GLFW_KEY_T,				EInputKey::T				},
		{ GLFW_KEY_U,				EInputKey::U				},
		{ GLFW_KEY_V,				EInputKey::V				},
		{ GLFW_KEY_W,				EInputKey::W				},
		{ GLFW_KEY_X,				EInputKey::X				},
		{ GLFW_KEY_Y,				EInputKey::Y				},
		{ GLFW_KEY_Z,				EInputKey::Z				},
		{ GLFW_KEY_0,				EInputKey::ZERO				},
		{ GLFW_KEY_1,				EInputKey::ONE				},
		{ GLFW_KEY_2,				EInputKey::TWO				},
		{ GLFW_KEY_3,				EInputKey::THREE			},
		{ GLFW_KEY_4,				EInputKey::FOUR				},
		{ GLFW_KEY_5,				EInputKey::FIVE				},
		{ GLFW_KEY_6,				EInputKey::SIX				},
		{ GLFW_KEY_7,				EInputKey::SEVEN			},
		{ GLFW_KEY_8,				EInputKey::EIGHT			},
		{ GLFW_KEY_9,				EInputKey::NINE				},
		{ GLFW_KEY_SPACE,			EInputKey::SPACEBAR			},
		{ GLFW_KEY_ESCAPE,			EInputKey::ESCAPE			},
		{ GLFW_KEY_0,				EInputKey::NUMPAD_ZERO		},
		{ GLFW_KEY_1,				EInputKey::NUMPAD_ONE		},
		{ GLFW_KEY_2,				EInputKey::NUMPAD_TWO		},
		{ GLFW_KEY_3,				EInputKey::NUMPAD_THREE		},
		{ GLFW_KEY_4,				EInputKey::NUMPAD_FOUR		},
		{ GLFW_KEY_5,				EInputKey::NUMPAD_FIVE		},
		{ GLFW_KEY_6,				EInputKey::NUMPAD_SIX		},
		{ GLFW_KEY_7,				EInputKey::NUMPAD_SEVEN		},
		{ GLFW_KEY_8,				EInputKey::NUMPAD_EIGHT		},
		{ GLFW_KEY_9,				EInputKey::NUMPAD_NINE		},
	#if PLATFORM_WINDOWS
		{ GLFW_KEY_MENU,			EInputKey::WIN_KEY			},
	#endif
		{ GLFW_KEY_LEFT_CONTROL,	EInputKey::LEFT_CTRL		},
		{ GLFW_KEY_LEFT_ALT,		EInputKey::LEFT_ALT			},
		{ GLFW_KEY_LEFT_SHIFT,		EInputKey::LEFT_SHIFT		},
		{ GLFW_KEY_RIGHT_CONTROL,	EInputKey::RIGHT_CTRL		},
		{ GLFW_KEY_RIGHT_ALT,		EInputKey::RIGHT_ALT		},
		{ GLFW_KEY_RIGHT_SHIFT,		EInputKey::RIGHT_SHIFT		},
		{ GLFW_KEY_TAB,				EInputKey::TAB				},
		//{ GLFW_KEY_TILDE,			EInputKey::TILDE			},
		{ GLFW_KEY_ENTER,			EInputKey::ENTER			},
		{ GLFW_KEY_ENTER,			EInputKey::NUMPAD_ENTER		},
		{ GLFW_KEY_BACKSPACE,		EInputKey::BACKSPACE		},
		{ GLFW_KEY_UP,				EInputKey::ARROW_UP			},
		{ GLFW_KEY_RIGHT,			EInputKey::ARROW_RIGHT		},
		{ GLFW_KEY_DOWN,			EInputKey::ARROW_DOWN		},
		{ GLFW_KEY_LEFT,			EInputKey::ARROW_LEFT		}
	};

	return GLFWKeyMap;
}

const EInputKey GetKeyFromGLFWKey(const unsigned short GLFWKey)
{
	ASSERT(GetGLFWKeyMap().find(GLFWKey) != GetGLFWKeyMap().end(), "Key conversion not implemented for GLFW");

	return GetGLFWKeyMap().at(GLFWKey);
}

#endif // GLFWKEYMAP_INPUTMANAGERPRIVATE
