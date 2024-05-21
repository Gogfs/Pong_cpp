typedef unsigned int u32;

#define variavel_global static;
#define interno static;

inline int clamp(int min, int val, int max) {
	if (val < min) return min;
	if (val > max) return max;
	return val;
}
