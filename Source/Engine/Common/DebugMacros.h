#pragma once

// ASSERTS
#define ASSERT(Statement) if (!Statement) __debugbreak;
#define ASSERT_FAIL(Statement, Fail) if (!Statement) { __debugbreak; Fail; }

// OTHER
#define PAUSE system("pause")
