#ifndef __RAYTRACING_RAYTRACE_CONFIG_H__
#define __RAYTRACING_RAYTRACE_CONFIG_H__
namespace RayTrace
{
	const float c_max_dist = 1024.f;
	extern bool IsMaxDist(float d);

	const int c_max_recurDepth = 5;
	extern bool IsMaxRecurDepth(int r);

	enum { c_screen_width = 400 };
	enum { c_screen_height = 300 };
}

#endif 