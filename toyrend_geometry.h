typedef struct
{
	int32_t X;
	int32_t Y;
} point_t;

typedef struct
{
	int32_t X0;
	int32_t Y0;
	int32_t X1;
	int32_t Y1;
} line_t;

typedef struct
{
	point_t Vertex[3];
} triangle_t;

typedef struct
{
	point_t Center;
	uint32_t Radius;
} circle_t;
