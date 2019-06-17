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

const uint32 normalKernelIndex = 0;

const uint32 sharpenKernelIndex = 1;

const uint32 blurKernelIndex = 2;

const uint32 outlineKernel1Index = 3;

const uint32 outlineKernel2Index = 4;

const uint32 sketchKernelIndex = 5;

const uint32 embossKernelIndex = 6;
// ===== Image Kernels =====