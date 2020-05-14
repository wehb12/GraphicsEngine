#pragma once

// ASSERTS
#define ASSERT(Statement) if (!Statement) __debugbreak();
#define ASSERT_FAIL(Statement, Fail) if (!Statement) { Fail; __debugbreak(); }

// OTHER
#define PAUSE system("pause")
