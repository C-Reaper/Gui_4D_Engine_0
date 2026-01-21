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

typedef struct Connection{
	int i0;
	int i1;
} Connection;

typedef struct Cube4D{
	Vec3D points[16];
} Cube4D;

void Cube4D_Build(Vector* points,Vector* connections,Vec3D p,Vec3D d){
	Cube4D ps = {{
		{	p.x,		p.y,		p.z,		p.w			},
		{	p.x + d.x,	p.y,		p.z,		p.w			},
		{	p.x,		p.y + d.y,	p.z,		p.w			},
		{	p.x,		p.y,		p.z + d.z,	p.w			},
		{	p.x + d.x,	p.y + d.y,	p.z + d.z,	p.w			},
		{	p.x,		p.y + d.y,	p.z + d.z,	p.w			},
		{	p.x + d.x,	p.y,		p.z + d.z,	p.w			},
		{	p.x + d.x,	p.y + d.y,	p.z,		p.w			},

		{	p.x,		p.y,		p.z,		p.w + d.w	},
		{	p.x + d.x,	p.y,		p.z,		p.w + d.w	},
		{	p.x,		p.y + d.y,	p.z,		p.w + d.w	},
		{	p.x,		p.y,		p.z + d.z,	p.w + d.w	},
		{	p.x + d.x,	p.y + d.y,	p.z + d.z,	p.w + d.w	},
		{	p.x,		p.y + d.y,	p.z + d.z,	p.w + d.w	},
		{	p.x + d.x,	p.y,		p.z + d.z,	p.w + d.w	},
		{	p.x + d.x,	p.y + d.y,	p.z,		p.w + d.w	},
	}};
	Connection cs[] = {
		{ 0,1 },{ 0,2 },{ 0,3 },
		{ 4,5 },{ 4,6 },{ 4,7 },
		{ 5,3 },{ 5,2 },
		{ 6,1 },{ 6,3 },
		{ 7,1 },{ 7,2 },

		{ 8,9 },{ 8,10 },{ 8,11 },
		{ 12,13 },{ 12,14 },{ 12,15 },
		{ 13,11 },{ 13,10 },
		{ 14,9 },{ 14,11 },
		{ 15,9 },{ 15,10 },

		{ 0,8 },{ 1,9 },{ 2,10 },
		{ 3,11 },{ 4,12 },{ 5,13 },
		{ 6,14 },{ 7,15 }
	};

	for(int i = 0;i<sizeof(ps.points) / sizeof(Vec3D);i++){
		Vector_Push(points,ps.points + i);
	}
	for(int i = 0;i<sizeof(cs) / sizeof(Connection);i++){
		Vector_Push(connections,cs + i);
	}
}

Vec3D Vec4D_Add(Vec3D v1, Vec3D v2){
	return (Vec3D){ v1.x + v2.x,v1.y + v2.y,v1.z + v2.z,v1.w + v2.w };
}
Vec3D Vec4D_Sub(Vec3D v1, Vec3D v2){
	return (Vec3D){ v1.x - v2.x,v1.y - v2.y,v1.z - v2.z,v1.w - v2.w };
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
Vector output;
Vector connections;

float angle;
Vec3D cam4;
Vec3D cam3;
float ax;
float ay;
float ar;

char Vec4D_Calc(Vec3D cam4,M4x4D rot4,Vec3D cam3,M4x4D rot3,Vec3D* pt,Vec2* out){
	const Vec3D t4 = Vec4D_Sub(*pt,cam4);
	const Vec3D r4 = Matrix_MultiplyVector(rot4,t4);

	if(r4.w <= 0.0f)
		return 0;

	const float d4 = 2.0f;
	const Vec3D p3 = (Vec3D){
	    r4.x * d4 / r4.w,
	    r4.y * d4 / r4.w,
	    r4.z * d4 / r4.w,
	    r4.w
	};


	const Vec3D t3 = Vec4D_Sub(p3,cam3);
	const Vec3D r3 = Matrix_MultiplyVector(rot3,t3);
	
	if(r3.z <= 0.0f)
		return 0;

	const float d3 = 1.5f;
	const Vec3D ppp = (Vec3D){
	    r3.x * d3 / r3.z,
	    r3.y * d3 / r3.z,
	    r3.z,
	    r3.w
	};
	
	*pt = ppp;
	
	const float w = (float)GetWidth() * 0.5f;
	const float h = (float)GetHeight() * 0.5f;
	*out = (Vec2){ ppp.x * w + w,ppp.y * h * ar + h };
	return 1;
}

void Setup(AlxWindow* w){
	cam4 = (Vec3D){ 0.5f,0.5f,0.0f,-1.0f };
	cam3 = (Vec3D){ 0.0f,0.0f,0.0f,0.0f };

	angle = 0.0f;
	ax = 0.0f;
	ay = 0.0f;
	ar = -16.0f / 9.0f;

	points = Vector_New(sizeof(Vec3D));
	output = Vector_New(sizeof(Vec2));
	connections = Vector_New(sizeof(Connection));
	
	Cube4D_Build(&points,&connections,(Vec3D){0.0f,0.0f,1.0f,1.0f},(Vec3D){1.0f,1.0f,1.0f,1.0f});
}
void Update(AlxWindow* w){
	const M4x4D rotY = Matrix_MakeRotationY(ay);

	const Vec3D left 	= Matrix_MultiplyVector(rotY,(Vec3D){1.0f * w->ElapsedTime,0.0f,0.0f,0.0f});
	const Vec3D front 	= Matrix_MultiplyVector(rotY,(Vec3D){0.0f,0.0f,1.0f * w->ElapsedTime,0.0f});
	const Vec3D front4 	= { 0.0f,0.0f,0.0f,1.0f * w->ElapsedTime };

	if(Stroke(ALX_KEY_T).DOWN) cam4 = Vec4D_Add(cam4,front4);
	if(Stroke(ALX_KEY_G).DOWN) cam4 = Vec4D_Sub(cam4,front4);

	if(Stroke(ALX_KEY_W).DOWN) cam3 = Vec4D_Add(cam3,front);
	if(Stroke(ALX_KEY_S).DOWN) cam3 = Vec4D_Sub(cam3,front);
	if(Stroke(ALX_KEY_A).DOWN) cam3 = Vec4D_Sub(cam3,left);
	if(Stroke(ALX_KEY_D).DOWN) cam3 = Vec4D_Add(cam3,left);
	if(Stroke(ALX_KEY_R).DOWN) cam3.y += 1.0f * w->ElapsedTime;
	if(Stroke(ALX_KEY_F).DOWN) cam3.y -= 1.0f * w->ElapsedTime;

	if(Stroke(ALX_KEY_UP).DOWN) 	ax -= F32_PI * w->ElapsedTime;
	if(Stroke(ALX_KEY_DOWN).DOWN) 	ax += F32_PI * w->ElapsedTime;
	if(Stroke(ALX_KEY_LEFT).DOWN) 	ay += F32_PI * w->ElapsedTime;
	if(Stroke(ALX_KEY_RIGHT).DOWN) 	ay -= F32_PI * w->ElapsedTime;

	Clear(BLACK);

	angle += 0.25f * F32_PI * w->ElapsedTime;

	//const M4x4D rotXY = Matrix_MultiplyMatrix(Matrix_MakeRotation4XZ(angle * 0.33f),Matrix_MakeRotation4YZ(angle));
	const M4x4D rotXY = Matrix_MakeRotationZ(angle);
	const M4x4D rot3 = Matrix_MultiplyMatrix(Matrix_MakeRotationY(-ay),Matrix_MakeRotationX(-ax));

	Vector_Clear(&output);

	for (int i = 0;i<connections.size;i++){
		Connection* c = (Connection*)Vector_Get(&connections,i);
		
		Vec3D rpt0 = *(Vec3D*)Vector_Get(&points,c->i0);
		Vec3D rpt1 = *(Vec3D*)Vector_Get(&points,c->i1);

		Vec2 out0;
		Vec2 out1;
		if(!Vec4D_Calc(cam4,rotXY,cam3,rot3,&rpt0,&out0) || !Vec4D_Calc(cam4,rotXY,cam3,rot3,&rpt1,&out1))
			continue;
		
		const float w = (float)GetWidth() * 0.5f;
		const float h = (float)GetHeight() * 0.5f;

		Circle_RenderX(
			WINDOW_STD_ARGS,
			out0,F32_Clamp(w * 0.01f / rpt0.z,0.0f,w),
			WHITE
		);
		Circle_RenderX(
			WINDOW_STD_ARGS,
			out1,F32_Clamp(w * 0.01f / rpt1.z,0.0f,w),
			WHITE
		);

		Line_RenderX(
			WINDOW_STD_ARGS,
			out0,out1,
			WHITE,1.0f
		);
	}
}
void Delete(AlxWindow* w){
	Vector_Free(&points);
	Vector_Free(&output);
	Vector_Free(&connections);
}

int main(){
    if(Create("4D Engine",2500,1200,1,1,Setup,Update,Delete))
        Start();
    return 0;
}