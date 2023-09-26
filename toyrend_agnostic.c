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

static void
toyrend_circle(toyrend_struct* Renderer, circle_t Circle, uint32_t Color)
{
	int32_t X0 = Circle.Center.X;
	int32_t Y0 = Circle.Center.Y;
	int32_t F = 1 - Circle.Radius;
	int32_t ddF_x = 0;
	int32_t ddF_y = -2 * Circle.Radius;
	int32_t X = 0;
	int32_t Y = Circle.Radius;

	toyrend_pixel(Renderer, (point_t){X0 + Circle.Radius, Y0}, Color);
	toyrend_pixel(Renderer, (point_t){X0 - Circle.Radius, Y0}, Color);
	toyrend_pixel(Renderer, (point_t){X0, Y0 + Circle.Radius}, Color);
	toyrend_pixel(Renderer, (point_t){X0, Y0 - Circle.Radius}, Color);

	while(X < Y)
	{
		if(F >= 0)
		{
			Y -= 1;
			ddF_y += 2;
			F += ddF_y;
		}
		X += 1;
		ddF_x += 2;
		F += ddF_x + 1;
		toyrend_pixel(Renderer, (point_t){X0 + X, Y0 + Y}, Color);
		toyrend_pixel(Renderer, (point_t){X0 - X, Y0 + Y}, Color);
		toyrend_pixel(Renderer, (point_t){X0 + X, Y0 - Y}, Color);
		toyrend_pixel(Renderer, (point_t){X0 - X, Y0 - Y}, Color);
		toyrend_pixel(Renderer, (point_t){X0 + Y, Y0 + X}, Color);
		toyrend_pixel(Renderer, (point_t){X0 - Y, Y0 + X}, Color);
		toyrend_pixel(Renderer, (point_t){X0 + Y, Y0 - X}, Color);
		toyrend_pixel(Renderer, (point_t){X0 - Y, Y0 - X}, Color);
	}
}
