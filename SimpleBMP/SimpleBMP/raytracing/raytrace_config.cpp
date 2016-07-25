#include "raytrace_config.h"

namespace RayTrace
{
	/*const float c_max_dist = 1024;*/
	bool IsMaxDist(float d){return d > c_max_dist - 0.1;}

	/*const int c_max_recurDepth = 5;*/
	bool IsMaxRecurDepth(int r){return r >= c_max_recurDepth;}
}