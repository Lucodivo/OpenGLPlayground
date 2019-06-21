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
	0.0f, 1.0f, 2.0f}
};

const uint32 normalKernel3x3Index = 0;

const uint32 sharpenKernel3x3Index = 1;

const uint32 outlineKernel3x3Index = 2;

const uint32 sketchKernel3x3Index = 3;

const uint32 embossKernel3x3Index = 4;


// 5x5 Kernels
const float32 kernels5x5[8][25] = {
	// normal kernel
	{0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 1, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0},

	// sharpen 1 kernel
	{-1, -1, -1, -1, -1,
	-1, 1, 1, 1, -1,
	-1, 1, 9, 1, -1,
	-1, 1, 1, 1, -1,
	-1, -1, -1, -1, -1},

	// sharpen 2 kernel
	{-1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1,
	-1, -1, 25, -1, -1,
	-1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1},

	// outline kernel
	{1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, -24.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f, 1.0f},

	// emboss kernel
	{-4.0f, -3.0f, -2.0f -1.0f, 0.0f
	-3.0f, -2.0f, -1.0f, 0.0f, 1.0f,
	-2.0f, -1.0f, 1.0f, 1.0f, 2.0f,
	-1.0f, 0.0f, 1.0f, 2.0f, 3.0f,
	0.0f, 1.0f, 2.0f, 3.0f, 4.0f},

	// edge kernel 1
	{(2.0f / 18.0f), (2.0f / 18.0f), (4.0f / 18.0f), (2.0f / 18.0f), (2.0f / 18.0f),
	(1.0f / 18.0f), (1.0f / 18.0f), (2.0f / 18.0f), (1.0f / 18.0f), (1.0f / 18.0f),
	0, 0, 0, 0, 0,
	(-2.0f / 18.0f), (-2.0f / 18.0f), (-4.0f / 18.0f), (-2.0f / 18.0f), (-2.0f / 18.0f),
	(-1.0f / 18.0f), (-1.0f / 18.0f), (-2.0f / 18.0f), (-1.0f / 18.0f), (-1.0f / 18.0f)},

	// edge kernel 2
	{(2.0f / 18.0f), (1.0f / 18.0f), 0, (-1.0f / 18.0f), (-2.0f / 18.0f),
	(2.0f / 18.0f), (1.0f / 18.0f), 0, (-1.0f / 18.0f), (-2.0f / 18.0f),
	(4.0f / 18.0f), (2.0f / 18.0f), 0, (-2.0f / 18.0f), (-4.0f / 18.0f),
	(2.0f / 18.0f), (1.0f / 18.0f), 0, (-1.0f / 18.0f), (-2.0f / 18.0f),
	(2.0f / 18.0f), (1.0f / 18.0f), 0, (-1.0f / 18.0f), (-2.0f / 18.0f)},

	// blur kernel
	{(1.0f / 40.0f), (1.0f / 40.0f), (2.0f / 40.0f), (1.0f / 40.0f), (1.0f / 40.0f),
	(1.0f / 40.0f), (2.0f / 40.0f), (2.0f / 40.0f), (2.0f / 40.0f), (1.0f / 40.0f),
	(2.0f / 40.0f), (2.0f / 40.0f), (4.0f / 40.0f), (2.0f / 40.0f), (2.0f / 40.0f),
	(1.0f / 40.0f), (2.0f / 40.0f), (2.0f / 40.0f), (2.0f / 40.0f), (1.0f / 40.0f),
	(1.0f / 40.0f), (1.0f / 40.0f), (2.0f / 40.0f), (1.0f / 40.0f), (1.0f / 40.0f)}
};

const uint32 normalKernel5x5Index = 0;

const uint32 sharpen1Kernel5x5Index = 1;

const uint32 sharpen2Kernel5x5Index = 2;

const uint32 outlineKernal5x5Index = 3;

const uint32 embossKernel5x5Index = 4;

const uint32 edge1Kernel5x5Index = 5;

const uint32 edge2Kernel5x5Index = 6;

const uint32 blurKernel5x5Index = 7;
// ===== Image Kernels =====