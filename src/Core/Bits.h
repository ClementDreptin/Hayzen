#pragma once

// Set the Nth bit of X to 1.
#define BIT_SET(X, N) ((X) |= (1 << (N)))

// Set the Nth bit of X to 0.
#define BIT_CLEAR(X, N) ((X) &= ~(1 << (N)))

// Flip the Nth bit of X.
#define BIT_FLIP(X, N) ((X) ^= (1 << (N)))

// Check if the Nth bit of X is set to 1.
#define BIT_CHECK(X, N) (!!((X) & (1 << (N)))) // !! guarantees that the result will either be 0 or 1
