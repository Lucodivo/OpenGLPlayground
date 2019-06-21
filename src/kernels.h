#pragma once

// ===== Image Kernels =====
// 3x3 Kernels
const float32 kernels3x3[5][9] = {
	// normal kernel
	{0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f},

	// sharpen kernel
	{-1.0f, -1.0f, -1.0f,
	-1.0f,  9.0f, -1.0f,
	-1.0f, -1.0f, -1.0f},

	// outline kernel
	{1.0f, 1.0f, 1.0f,
	1.0f, -8.0f, 1.0f,
	1.0f, 1.0f, 1.0f},

	// sketch kernel
	{37.0f, -12.0f, 9.0f,
	-13.0f, 22.0f, -18.0f,
	22.0f, -50.0f, 3.0f},

	// emboss kernel
	{-2.0f, -1.0f, 0.0f,
	-1.0f, 1.0f, 1.0f,
	0.0f, 1.0f, 2.0f},
};

const uint32 normalKernel3x3Index = 0;

const uint32 sharpenKernel3x3Index = 1;

const uint32 outlineKernel3x3Index = 2;

const uint32 sketchKernel3x3Index = 3;

const uint32 embossKernel3x3Index = 4;
// ===== Image Kernels =====