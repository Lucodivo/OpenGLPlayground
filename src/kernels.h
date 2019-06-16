#pragma once

// ===== Image Kernels =====
const float32 kernels[7][9] = {
	// normal kernel
	{0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f},

	// blur kernel
	{1.0f / 16, 2.0f / 16, 1.0f / 16,
	2.0f / 16, 4.0f / 16, 2.0f / 16,
	1.0f / 16, 2.0f / 16, 1.0f / 16},

	// sharpen kernel
	{-1.0f, -1.0f, -1.0f,
	-1.0f,  9.0f, -1.0f,
	-1.0f, -1.0f, -1.0f},

	// outline 1 kernel
	{1.0f, 1.0f, 1.0f,
	1.0f, -8.0f, 1.0f,
	1.0f, 1.0f, 1.0f},

	// outline 2 kernel
	{-1.0f, -1.0f, -1.0f,
	-1.0f, 8.0f, -1.0f,
	-1.0f, -1.0f, -1.0f},

	// sketch kernel
	{37.0f, -12.0f, 9.0f,
	-13.0f, 22.0f, -18.0f,
	22.0f, -50.0f, 3.0f},

	// emboss kernel
	{-2.0f, -1.0f, 0.0f,
	-1.0f, 1.0f, 1.0f,
	0.0f, 1.0f, 2.0f},

};

const float32* normalKernel = kernels[0];

const float32* sharpenKernel = kernels[1];

const float32* blurKernel = kernels[2];

const float32* edgeKernel = kernels[3];
// ===== Image Kernels =====