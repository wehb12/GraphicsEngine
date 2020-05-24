#pragma once

#define MAX(A, B) (A > B) ? A : B
#define MIN(A, B) (A > B) ? B : A
#define INRANGE(A, MinValue, MaxValue) MAX(MIN(A, MaxValue), MinValue)
