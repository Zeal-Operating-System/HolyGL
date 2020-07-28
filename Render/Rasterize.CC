
// Draws single point to texture, only works with raw textures
U0 DrawPoint(CTex2D *tex, I64 x, I64 y, CBGR24 color)
{
    tex->rawBuf[x + y * tex->w] = color;
}

// Draws a line to texture, only works with raw textures
U0 DrawLine(CTex2D *tex, I64 x0, I64 y0, I64 x1, I64 y1, CBGR24 color)
{
	Bool steep = (Abs(y1 - y0) > Abs(x1 - x0));

	I64 temp;

	if (steep == TRUE)
    {
		temp = x0;
		x0 = y0;
		y0 = temp;
		temp = x1;
		x1 = y1;
		y1 = temp;
    }

	if (x0 > x1)
	{
		temp = x0;
		x0 = x1;
		x1 = temp;
		temp = y0;
		y0 = y1;
		y1 = temp;	
	}

	I64 delta_x = x1 - x0;
	I64 delta_y = Abs(y1 - y0);
	I64 error = delta_x / 2;

	I64 step_y;
	
	if (y0 < y1)
	{
		step_y = 1;
	}
	else
	{
		step_y = -1;
	}

	I64 y = y0;
	

	I64 x;
	for (x = x0; x < (x1 + 1); x++)
	{
		if (steep == TRUE)
		{
			tex->rawBuf[y + x * tex->w] = color;
		}
		else
		{
			tex->rawBuf[x + y * tex->w] = color;
		}

		error -= delta_y;

		if (error < 0)
		{
			y += step_y;
			error += delta_x;
		}
	}
}

// Draws single color triangle to texture, only works on raw textures
U0 DrawTriangle(CTex2D *tex, I64 x0, I64 y0, I64 x1, I64 y1, I64 x2, I64 y2, CBGR24 color)
{
	I64 temp;
	F64 ftemp;
	
	if (y0 > y1)
	{
		temp = x0;
		x0 = x1;
		x1 = temp;
		temp = y0;
		y0 = y1;
		y1 = temp;
	}

	if (y0 > y2)
	{
		temp = x0;
		x0 = x2;
		x2 = temp;
		temp = y0;
		y0 = y2;
		y2 = temp;
	}

	if (y1 > y2)
	{
		temp = x1;
		x1 = x2;
		x2 = temp;
		temp = y1;
		y1 = y2;
		y2 = temp;
	}

	Bool middle_line_drawn = FALSE;
	F64 x_delta;
	F64 y_delta;
	F64 left_delta;
	F64 right_delta;
	F64 left_x;
	F64 right_x;
	I64 y, lX, rX;
	I64 left_index, right_index;
	I64 width;

	// Bottom Half
	if (y0 != y1)
	{
		x_delta = (x1 - x0);
		y_delta = (y1 - y0);
		left_delta = x_delta / y_delta;
		x_delta = (x2 - x0);
		y_delta = (y2 - y0);
		right_delta = x_delta / y_delta;

		if (left_delta > right_delta)
		{
			ftemp = left_delta;
			left_delta = right_delta;
			right_delta = ftemp;
		}

		left_x = x0;
		right_x = x0;

		middle_line_drawn = TRUE;

		for (y = y0; y < (y1 + 1); y++)
		{
			lX = left_x + 0.5;
			rX = right_x + 0.5;
			left_index = y * tex->w + lX;
			right_index = y * tex->w + rX;
			width = right_index - left_index;
			MemSetU32(tex->rawBuf + left_index, color, width);
			left_x += left_delta;
			right_x += right_delta;
		}
	}

	// Top Half
	if (y1 != y2)
	{
		x_delta = -(x1 - x2);
		y_delta = (y1 - y2);
		left_delta = x_delta / y_delta;
		x_delta = -(x0 - x2);
		y_delta = (y0 - y2);
		right_delta = x_delta / y_delta;

		if (left_delta > right_delta)
		{
			ftemp = left_delta;
			left_delta = right_delta;
			right_delta = ftemp;
		}

		left_x = x2;
		right_x = x2;

		if (middle_line_drawn == TRUE)
		{
			y1 += 1;
		}

		for (y = y2; y > (y1 - 1); y--)
		{
			lX = left_x + 0.5;
			rX = right_x + 0.5;
			left_index = y * tex->w + lX;
			right_index = y * tex->w + rX;
			width = right_index - left_index;
			MemSetU32(tex->rawBuf + left_index, color, width);
			left_x += left_delta;
			right_x += right_delta;
		}
	}
}


U0 RenderTris(CShader *shd, CTex2D *frameBuf, CTex2D *depthBuf, U8 *mdl, U8 *uniforms, I64 nTris)
{
	Bool middle_line_drawn = FALSE;
	F64 x_delta;
	F64 y_delta;
	F64 left_delta;
	F64 right_delta;
	F64 left_x;
	F64 right_x;
	I64 y, lX, rX;
	I64 left_index, right_index;
	I64 width;

	I64 i, j, temp;
	F64 ftemp;
	F64 z; // Interpolated value to depth check

	// Stores baryocentric coordinates for current fragment
	F64 u, v, w;
	F64 bc[3];

	// Vertex coordinates. P is the point to get baryocentric coordinate for
	CVec2 p, a, b, c;

	CBGR24 fragColor;	// Final color to write to fragment
	
	// Calculate dimensions and allocate vertex attributes buffer
	// to pass interpolated per-frag values to the fragment
	// shader from the vertex shader:

	F64 *vOut;		// This would be like Out from GLSL, it is 3x the size of fIn (3 vertices)
	F64 *fIn;		// This would be like In from GLSL

	I64 attrBufSize = 0;
	for (i = 0; i < shd->nVertValues; i++)
	{
		switch (shd->vertValues[i])
		{
			case SHD_F64:
				attrBufSize += 1;
				break;

			case SHD_CVEC2:
				attrBufSize += 2;
				break;

			case SHD_CVEC3:
				attrBufSize += 3;
				break;

			case SHD_CMat4:
				attrBufSize += 16;
				break;
		}
	}
	vOut = MAlloc(attrBufSize * sizeof(F64) * 3);	// 3x for 3 vertexes
	fIn = MAlloc(attrBufSize * sizeof(F64));		// Single set of interpolated values

	CTri cTri;			// Current Triangle in NDC
	CIVec2 cTriPx[3];	// Current Triangle in pixels
	I64 tri;			// Current Triangle Index
	I64 VI[3];			// Vertex Indices

	for (tri = 0; tri < nTris; tri++)
	{
		// Get current triangle in NDC from vertex shader
		shd->VertShd(&cTri, vOut, mdl, uniforms, tri, nTris);
	
		ftemp = 0;
		ftemp += Abs(cTri.p[0].x);
		ftemp += Abs(cTri.p[0].y);
		ftemp += Abs(cTri.p[1].x);
		ftemp += Abs(cTri.p[1].y);
		ftemp += Abs(cTri.p[2].x);
		ftemp += Abs(cTri.p[2].y);

		// Triangle is not outside NDC range, it is on screen
		//if (ftemp =< 6)
		if (1 == 1)
		{
		
			// This triangle may not be in the correct order.
			// y0 should be lowest in texture coordinates, then y1 and y2.
			// This means y0 should be the highest in NDC coordinates.
		
			// In VI (Vertex Indices), the array index is the index of the final point,
			// and it's value is the point from the original tri it get's it's
			// coordinates from.
			VI[0] = 0;
			VI[1] = 1;
			VI[2] = 2;

			if (cTri.p[VI[0]].y < cTri.p[VI[1]].y)
			{
				temp = VI[0];
				VI[0] = VI[1];
				VI[1] = temp;
			}

			if (cTri.p[VI[0]].y < cTri.p[VI[2]].y)
			{
				temp = VI[0];
				VI[0] = VI[2];
				VI[2] = temp;
			}

			if (cTri.p[VI[1]].y < cTri.p[VI[2]].y)
			{
				temp = VI[1];
				VI[1] = VI[2];
				VI[2] = temp;
			}

			// By now, VI[0] points to the point with the lowest
			// y in texture coordinates, and VI[2] points to the highest
		
			// Convert NDC coordinates into tex coordinates for rasterization process
			cTriPx[0].x = ToI64(((cTri.p[VI[0]].x * 0.5) + 0.5) * ToF64(frameBuf->w));
			cTriPx[0].y = ToI64(((-cTri.p[VI[0]].y * 0.5) + 0.5) * ToF64(frameBuf->h));
			cTriPx[1].x = ToI64(((cTri.p[VI[1]].x * 0.5) + 0.5) * ToF64(frameBuf->w));
			cTriPx[1].y = ToI64(((-cTri.p[VI[1]].y * 0.5) + 0.5) * ToF64(frameBuf->h));
			cTriPx[2].x = ToI64(((cTri.p[VI[2]].x * 0.5) + 0.5) * ToF64(frameBuf->w));
			cTriPx[2].y = ToI64(((-cTri.p[VI[2]].y * 0.5) + 0.5) * ToF64(frameBuf->h));

		
			// Prepare F64 triangle point vectors for calculating barycentric coords later
			//Userd to use VI[0]
			a.x = cTriPx[0].x;
			a.y = cTriPx[0].y;
			b.x = cTriPx[1].x;
			b.y = cTriPx[1].y;
			c.x = cTriPx[2].x;
			c.y = cTriPx[2].y;

			// Rasterize Bottom Half (Tex coordinates, not NDC)
			if (cTriPx[0].y != cTriPx[1].y)
			{
				x_delta = (cTriPx[1].x - cTriPx[0].x);
				y_delta = (cTriPx[1].y - cTriPx[0].y);
				left_delta = x_delta / y_delta;
				x_delta = (cTriPx[2].x - cTriPx[0].x);
				y_delta = (cTriPx[2].y - cTriPx[0].y);
				right_delta = x_delta / y_delta;

				if (left_delta > right_delta)
				{
					ftemp = left_delta;
					left_delta = right_delta;
					right_delta = ftemp;
				}

				left_x = cTriPx[0].x;
				right_x = cTriPx[0].x;

				middle_line_drawn = TRUE;

				for (y = cTriPx[0].y; y < (cTriPx[1].y + 1); y++)
				{
					lX = left_x + 0.5;
					rX = right_x + 0.5;
					left_index = y * frameBuf->w + lX;
					right_index = y * frameBuf->w + rX;
					
					// Temporary fix, the y should just be clipped and left_x/right_x adjusted accordingly
					if (TRUE)

					{
						// Check if x coordinate is off screen and clip it.
						if (lX < 0)
						{
							lX = 0;
						}
						if (lX > frameBuf->w)
						{
							lX = frameBuf->w;
						}
						if (rX > frameBuf->w)
						{
							rX = frameBuf->w;
						}
						if (rX < 0)
						{
							rX = 0;
						}
 
						// For every pixel along the line, set it's color
						for (i = lX; i < rX; i++)
						{
							// Get barycentric coordinates (u, v, w)
							p.x = i;
							p.y = y;
							Barycentric2(&p, &a, &b, &c, &bc[VI[0]], &bc[VI[1]], &bc[VI[2]]);

							z = (bc[VI[0]] * cTri.p[VI[0]].z) + (bc[VI[1]] * cTri.p[VI[1]].z) + (bc[VI[2]] * cTri.p[VI[2]].z);

							if (z <= depthBuf->depthBuf[y * frameBuf->w + i])
							{
								// Interpolate all vOut values to fIn values for the fragment shader
								for (j = 0; j < attrBufSize; j++)
								{
									fIn[j] = (vOut[j] * bc[0]) + (vOut[j+attrBufSize] * bc[1]) + (vOut[j+(2*attrBufSize)] * bc[2]);
								}

								depthBuf->depthBuf[y * frameBuf->w + i] = z;
								shd->FragShd(&fragColor, fIn, uniforms);
								left_index = y * frameBuf->w + i;
								MemSetU32(frameBuf->rawBuf + left_index, fragColor, 1);
							}
						}
					} 
					left_x += left_delta;
					right_x += right_delta;
				}
			}

			// Rasterize Top Half (Tex coordinates, not NDC)
			if (cTriPx[1].y != cTriPx[2].y)
			{
				x_delta = -(cTriPx[1].x - cTriPx[2].x);
				y_delta = (cTriPx[1].y - cTriPx[2].y);
				left_delta = x_delta / y_delta;
				x_delta = -(cTriPx[0].x - cTriPx[2].x);
				y_delta = (cTriPx[0].y - cTriPx[2].y);
				right_delta = x_delta / y_delta;

				if (left_delta > right_delta)
				{
					ftemp = left_delta;
					left_delta = right_delta;
					right_delta = ftemp;
				}

				left_x = cTriPx[2].x;
				right_x = cTriPx[2].x;

				if (middle_line_drawn == TRUE)
				{
					cTriPx[1].y += 1;
				}

				a.x = cTriPx[0].x;
				a.y = cTriPx[0].y;
				b.x = cTriPx[1].x;
				b.y = cTriPx[1].y;
				c.x = cTriPx[2].x;
				c.y = cTriPx[2].y;


				for (y = cTriPx[2].y; y > (cTriPx[1].y - 1); y--)
				{
					lX = left_x + 0.5;
					rX = right_x + 0.5;
					left_index = y * frameBuf->w + lX;
					right_index = y * frameBuf->w + rX;

					// Temporary fix, the y should just be clipped and left_x/right_x adjusted accordingly
					if (TRUE)
					{
						if (lX < 0)
						{
							lX = 0;
						}
						if (lX > frameBuf->w)
						{
							lX = frameBuf->w;
						}
						if (rX > frameBuf->w)
						{
							rX = frameBuf->w;
						}
						if (rX < 0)
						{
							rX = 0;
						}

						// For every pixel along the line, set it's color
						for (i = lX; i < rX; i++)
						{
							// Get barycentric coordinates (u, v, w)
							p.x = i;
							p.y = y;
							Barycentric2(&p, &a, &b, &c, &bc[VI[0]], &bc[VI[1]], &bc[VI[2]]);

							z = (bc[VI[0]] * cTri.p[VI[0]].z) + (bc[VI[1]] * cTri.p[VI[1]].z) + (bc[VI[2]] * cTri.p[VI[2]].z);

							if (z <= depthBuf->depthBuf[y * frameBuf->w + i])
							{
								// Interpolate all vOut values to fIn values for the fragment shader
								for (j = 0; j < attrBufSize; j++)
								{
									fIn[j] = (vOut[j] * bc[0]) + (vOut[j+attrBufSize] * bc[1]) + (vOut[j+(2*attrBufSize)] * bc[2]);
								}

								depthBuf->depthBuf[y * frameBuf->w + i] = z;
								shd->FragShd(&fragColor, fIn, uniforms);
								left_index = y * frameBuf->w + i;
								MemSetU32(frameBuf->rawBuf + left_index, fragColor, 1);
							}
						}
					}
					left_x += left_delta;
					right_x += right_delta;
				}
			}
		}
		
	}

	Free(vOut);
	Free(fIn);
}














