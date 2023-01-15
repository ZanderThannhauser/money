const char* vertex_source = ""
	"" "\n"
	"#version 330 core" "\n"
	"" "\n"
	"uniform mat4 mvp;" "\n"
	"" "\n"
	"in vec2 position;" "\n"
	"in vec3 color;" "\n"
	"" "\n"
	"out vec3 Color;" "\n"
	"" "\n"
	"void main()" "\n"
	"{" "\n"
	"\tColor = color;" "\n"
	"" "\n"
	"\tgl_Position = mvp * vec4(position, 0.0, 1.0);" "\n"
	"}" "\n"
	"" "\n"
	"" "\n"
	"" "\n"
	"";