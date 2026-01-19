#if defined __linux__
#include "/home/codeleaded/System/Static/Library/WindowEngine1.0.h"
#include "/home/codeleaded/System/Static/Library/Lib3D_Mathlib.h"
#include "/home/codeleaded/System/Static/Library/Lib3D_Cube.h"
#elif defined _WINE
#include "/home/codeleaded/System/Static/Library/WindowEngine1.0.h"
#include "/home/codeleaded/System/Static/Library/Lib3D_Math.h"
#include "/home/codeleaded/System/Static/Library/Lib3D_Cube.h"
#elif defined _WIN32
#include "F:/home/codeleaded/System/Static/Library/WindowEngine1.0.h"
#include "F:/home/codeleaded/System/Static/Library/Lib3D_Math.h"
#include "/home/codeleaded/System/Static/Library/Lib3D_Cube.h"
#elif defined(__APPLE__)
#error "Apple not supported!"
#else
#error "Platform not supported!"
#endif

typedef struct Cube4D{
	Vec3D points[16];
} Cube4D;

void Cube4D_Build(Vector* points,Vec3D p,Vec3D d){
	Cube4D ps = {{
		{	p.x,		p.y,		p.z,		p.w			},
		{	p.x + d.x,	p.y,		p.z,		p.w			},
		{	p.x,		p.y + d.y,	p.z,		p.w			},
		{	p.x,		p.y,		p.z + d.z,	p.w			},
		{	p.x,		p.y,		p.z,		p.w + d.w	},
		{	p.x + d.x,	p.y + d.y,	p.z,		p.w			},
		{	p.x + d.x,	p.y,		p.z + d.z,	p.w			},
		{	p.x + d.x,	p.y,		p.z,		p.w + d.w	},
		{	p.x,		p.y + d.y,	p.z + d.z,	p.w			},
		{	p.x,		p.y + d.y,	p.z,		p.w + d.w	},
		{	p.x,		p.y,		p.z + d.z,	p.w + d.w	},
		{	p.x + d.x,	p.y + d.y,	p.z + d.z,	p.w			},
		{	p.x + d.x,	p.y + d.y,	p.z,		p.w + d.w	},
		{	p.x + d.x,	p.y,		p.z + d.z,	p.w + d.w	},
		{	p.x,		p.y + d.y,	p.z + d.z,	p.w + d.w	},
		{	p.x + d.x,	p.y + d.y,	p.z + d.z,	p.w + d.w	},
	}};
	for(int i = 0;i<sizeof(ps.points) / sizeof(Vec3D);i++){
		Vector_Push(points,ps.points + i);
	}
}

M4x4D Matrix_MakeRotation4XY(float fAngleRad){
	return (M4x4D){{
		{ 1.0f,0.0f,			0.0f,			0.0f },
		{ 0.0f,1.0f,			0.0f,			0.0f },
		{ 0.0f,0.0f, cosf(fAngleRad),sinf(fAngleRad) },
		{ 0.0f,0.0f,-sinf(fAngleRad),cosf(fAngleRad) }
	}};
}
M4x4D Matrix_MakeRotation4XZ(float fAngleRad){
	return (M4x4D){{
		{ 1.0f,0.0f,			0.0f,			0.0f },
		{ 0.0f,cosf(fAngleRad),	0.0f,sinf(fAngleRad) },
		{ 0.0f,0.0f, 			1.0f,			0.0f },
		{ 0.0f,-sinf(fAngleRad),0.0f,cosf(fAngleRad) }
	}};
}
M4x4D Matrix_MakeRotation4YZ(float fAngleRad){
	return (M4x4D){{
		{ cosf(fAngleRad),	0.0f,0.0f,	sinf(fAngleRad) },
		{ 0.0f,				1.0f,0.0f,			   0.0f },
		{ 0.0f,				0.0f,1.0f,			   0.0f },
		{ -sinf(fAngleRad), 0.0f,0.0f,  cosf(fAngleRad) }
	}};
}



Vector points;
float angle;
Vec3D cam;
float ax;
float ay;
float ar;

void Setup(AlxWindow* w){
	cam = Vec3D_New(0.0f,0.0f,0.0f);
	angle = 0.0f;
	ax = 0.0f;
	ay = 0.0f;
	ar = 16.0f / 9.0f;

	points = Vector_New(sizeof(Vec3D));
	Cube4D_Build(&points,(Vec3D){0.0f,0.0f,0.0f,3.0f},(Vec3D){1.0f,1.0f,1.0f,1.0f});
}
void Update(AlxWindow* w){
	const M4x4D rotY = Matrix_MakeRotationY(ay);

	const Vec3D left 	= Matrix_MultiplyVector(rotY,(Vec3D){1.0f * w->ElapsedTime,0.0f,0.0f,0.0f});
	const Vec3D front 	= Matrix_MultiplyVector(rotY,(Vec3D){0.0f,0.0f,1.0f * w->ElapsedTime,0.0f});
	const Vec3D front4 	= Matrix_MultiplyVector(rotY,(Vec3D){0.0f,0.0f,0.0f,1.0f * w->ElapsedTime});

	if(Stroke(ALX_KEY_T).DOWN) cam = Vec3D_Add(cam,front4);
	if(Stroke(ALX_KEY_G).DOWN) cam = Vec3D_Sub(cam,front4);

	if(Stroke(ALX_KEY_W).DOWN) cam = Vec3D_Add(cam,front);
	if(Stroke(ALX_KEY_S).DOWN) cam = Vec3D_Sub(cam,front);
	if(Stroke(ALX_KEY_A).DOWN) cam = Vec3D_Sub(cam,left);
	if(Stroke(ALX_KEY_D).DOWN) cam = Vec3D_Add(cam,left);
	if(Stroke(ALX_KEY_R).DOWN) cam.y -= 1.0f * w->ElapsedTime;
	if(Stroke(ALX_KEY_F).DOWN) cam.y += 1.0f * w->ElapsedTime;

	if(Stroke(ALX_KEY_UP).DOWN) 	ax += F32_PI * w->ElapsedTime;
	if(Stroke(ALX_KEY_DOWN).DOWN) 	ax -= F32_PI * w->ElapsedTime;
	if(Stroke(ALX_KEY_LEFT).DOWN) 	ay += F32_PI * w->ElapsedTime;
	if(Stroke(ALX_KEY_RIGHT).DOWN) 	ay -= F32_PI * w->ElapsedTime;

	Clear(BLACK);

	angle += F32_PI * w->ElapsedTime;

	const M4x4D rotXY = Matrix_MultiplyMatrix(Matrix_MakeRotation4XZ(angle * 0.33f),Matrix_MakeRotation4YZ(angle));
	const M4x4D rot = Matrix_MultiplyMatrix(Matrix_MakeRotationY(-ay),Matrix_MakeRotationX(-ax));

	for (int i = 0;i<points.size;i++){
		Vec3D pt = *(Vec3D*)Vector_Get(&points,i);
		
		const Vec3D rpt = Matrix_MultiplyVector(rotXY,pt);

		const Vec3D t = Vec3D_Sub(rpt,cam);
		const Vec3D p = Matrix_MultiplyVector(rot,t);

		if(p.w <= 0.0f)
			continue;

		const Vec3D pp = (Vec3D){ p.x / p.w,p.y / p.w,p.z / p.w,p.w };
		
		if(p.z <= 0.0f)
			continue;

		const Vec3D ppp = (Vec3D){ p.x / p.z,p.y / p.z,p.z,p.w };
		
		const float w = (float)GetWidth() * 0.5f;
		const float h = (float)GetHeight() * 0.5f;
		const Vec2 op = { ppp.x * w + w,ppp.y * h * ar + h };

		Circle_RenderX(
			WINDOW_STD_ARGS,
			op,F32_Clamp(w * 0.01f / ppp.z,0.0f,w),
			WHITE
		);
	}
}
void Delete(AlxWindow* w){
	Vector_Free(&points);
}

int main(){
    if(Create("3D Engine",2500,1200,1,1,Setup,Update,Delete))
        Start();
    return 0;
}