static void
toyrend_clear(toyrend_struct* Renderer, uint32_t Color)
{
	LONG Width = Renderer->BitmapInfo.bmiHeader.biWidth;
	LONG Height = Renderer->BitmapInfo.bmiHeader.biHeight;
	size_t NumPixels = Width * Height;
	for(int Index = 0; Index < NumPixels; Index += 1)
	{
		Renderer->Pixels[Index] = Color;
	}
}

static void
toyrend_pixel(toyrend_struct* Renderer, point_t P, uint32_t Color)
{
	LONG RendererWidth = Renderer->BitmapInfo.bmiHeader.biWidth;
	LONG RendererHeight = Renderer->BitmapInfo.bmiHeader.biHeight;
	size_t PixelIndex = (P.Y * RendererWidth) + P.X;
	size_t NumPixels = RendererWidth * RendererHeight;
	if(PixelIndex < NumPixels)
	{
		Renderer->Pixels[PixelIndex] = Color;
	}
}

static void
toyrend_line(toyrend_struct* Renderer, line_t Line, uint32_t Color)
{
	int32_t DeltaX = Abs(Line.X1 - Line.X0);
	int32_t DeltaY = -Abs(Line.Y1 - Line.Y0);
	int32_t StepX = (Line.X0 < Line.X1) ? 1 : -1;
	int32_t StepY = (Line.Y0 < Line.Y1) ? 1 : -1;
	int32_t Error = DeltaX + DeltaY;
	point_t CurrentPixel = {Line.X0, Line.Y0};
	for(;;)
	{
		toyrend_pixel(Renderer, CurrentPixel, Color);
		if((CurrentPixel.X == Line.X1) || (CurrentPixel.Y == Line.Y1))
		{
			break;
		}
		int32_t Error2 = Error * 2;
		if(DeltaY <= Error2)
		{
			CurrentPixel.X += StepX;
			Error += DeltaY;
		}
		if(DeltaX >= Error2)
		{
			CurrentPixel.Y += StepY;
			Error += DeltaX;
		}
	}
}
