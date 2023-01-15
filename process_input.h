
#ifdef __cplusplus
extern "C" {
#endif

struct out
{
	struct vec2 {
		float x, y;
	}* points;
	
	unsigned n;
};

struct out process_input();

#ifdef __cplusplus
}
#endif
