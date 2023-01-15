
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <math.h>
#include <stdio.h>

#include <glad/glad.h>
//#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

#include <debug.h>

#include <cmdln/process.h>
#include <cmdln/just_print.h>

#include <process_input.h>

#include <shaders/fragment.h>
#include <shaders/vertex.h>

#include <mat4/struct.h>
#include <mat4/identity.h>
#include <mat4/multiply.h>
#include <mat4/translate.h>
#include <mat4/rotate.h>
#include <mat4/add.h>
#include <mat4/subtract.h>
#include <mat4/invert.h>
#include <mat4/scale.h>

#include <vec4/vec4.h>

#include <misc/map.h>

#define N(array) (sizeof(array) / sizeof(*array))

int main(int argc, char* const* argv)
{
	ENTER;
	
	cmdln_process(argc, argv);
	
	struct out plotme = process_input();
	
	if (!just_print)
	{
		glfwInit();
		
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
		
		glfwWindowHint(GLFW_SAMPLES, 4);
		
		unsigned width = 100, height = 100;
		
		GLFWwindow *window = glfwCreateWindow(width, height, "money", NULL, NULL);
		
		if (window == NULL)
		{
			fprintf(stderr, "Failed to create GLFW window\n");
			glfwTerminate();
			return -1;
		}
		
		glfwMakeContextCurrent(window);
		
		if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
		{
			fprintf(stderr, "Failed to initialize GLAD" "\n");
			return -1;
		}
		
		printf("Renderer: %s\n", glGetString(GL_RENDERER));
		
		printf("OpenGL version supported %s\n", glGetString(GL_VERSION));
		
		struct {
			unsigned vertex;
			unsigned fragment;
		} shaders;
		
		// compile vertex shader:
		{
			shaders.vertex = glCreateShader(GL_VERTEX_SHADER);
			
			glShaderSource(shaders.vertex, 1, &vertex_source, NULL);
			
			glCompileShader(shaders.vertex);
			
			GLint status;
			
			glGetShaderiv(shaders.vertex, GL_COMPILE_STATUS, &status);
			
			if (status == GL_FALSE)
			{
				TODO;
				exit(1);
			}
		}
		
		// compile fragment shader:
		{
			shaders.fragment = glCreateShader(GL_FRAGMENT_SHADER);
			
			glShaderSource(shaders.fragment, 1, &fragment_source, NULL);
			
			glCompileShader(shaders.fragment);
			
			GLint status;
			
			glGetShaderiv(shaders.fragment, GL_COMPILE_STATUS, &status);
			
			if (status == GL_FALSE)
			{
				TODO;
				exit(1);
			}
		}
		
		// link:
		unsigned program_id;
		{
			program_id = glCreateProgram();
			
			dpv(program_id);
			
			glAttachShader(program_id, shaders.vertex);
			
			glAttachShader(program_id, shaders.fragment);
			
			glLinkProgram(program_id);
		}
		
		struct {
			unsigned mvp;
			unsigned position;
			unsigned color;
		} attributes;
		
		// get attributes:
		{
			attributes.mvp = glGetUniformLocation(program_id, "mvp");
			
			attributes.position = glGetAttribLocation(program_id, "position");
			
			attributes.color = glGetAttribLocation(program_id, "color");
			
			dpv(attributes.mvp);
			
			dpv(attributes.position);
			
			dpv(attributes.color);
		}
		
		struct {
			unsigned vertex_array;
			unsigned buffer;
		} plot, grid;
		
		glGenVertexArrays(1, &plot.vertex_array);
		glGenVertexArrays(1, &grid.vertex_array);
		
		glGenBuffers(1, &plot.buffer);
		glGenBuffers(1, &grid.buffer);
		
		glBindVertexArray(plot.vertex_array);
		{
			glBindBuffer(GL_ARRAY_BUFFER, plot.buffer);
			{
				struct bundle {
					float x, y, r, g, b;
				};
				
				unsigned alloc_size = sizeof(struct bundle) * plotme.n;
				
				struct bundle* points = malloc(alloc_size);
				
				for (unsigned i = 0; i < plotme.n; i++)
				{
					points[i].x = plotme.points[i].x / (60 * 60 * 24);
					points[i].y = plotme.points[i].y / 100;
					
					if (plotme.points[i].y < 0)
					{
						points[i].r = 1.0;
						points[i].g = 0.1;
						points[i].b = 0.1;
					}
					else
					{
						points[i].r = 0.0;
						points[i].g = 0.0;
						points[i].b = 0.0;
					}
				}
				
				glBufferData(GL_ARRAY_BUFFER, alloc_size, points, GL_STATIC_DRAW);
				
				glEnableVertexAttribArray(attributes.position);
				glVertexAttribPointer(attributes.position, 2, GL_FLOAT, GL_FALSE, 20, NULL);
				
				glEnableVertexAttribArray(attributes.color);
				glVertexAttribPointer(attributes.color, 3, GL_FLOAT, GL_FALSE, 20, (void*) 8);
				
				free(points);
			}
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
		glBindVertexArray(0);
		
		glBindVertexArray(grid.vertex_array);
		{
			glBindBuffer(GL_ARRAY_BUFFER, grid.buffer);
			{
				struct vector2f {
					float x, y, r, g, b;
				} points[10];
				
				glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
				
				glEnableVertexAttribArray(attributes.position);
				glVertexAttribPointer(attributes.position, 2, GL_FLOAT, GL_FALSE, 20, NULL);
				
				glEnableVertexAttribArray(attributes.color);
				glVertexAttribPointer(attributes.color, 3, GL_FLOAT, GL_FALSE, 20, (void*) 8);
			}
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
		glBindVertexArray(0);
		
		struct mat4 projection = mat4_identity();
		
		struct mat4 view = mat4_identity();
		
		void update_mvp()
		{
			struct mat4 mvp = mat4_multiply(projection, view);
			
			glUseProgram(program_id);
			{
				glUniformMatrix4fv(attributes.mvp, 1, GL_TRUE, (void*) &mvp);
			}
			glUseProgram(0);
		}
		
		void update_grid()
		{
			struct mat4 mvp = mat4_multiply(projection, view);
			
			struct mat4 inverse = mat4_invert(mvp);
			
			struct vec4 tr = mat4_multiply_with_vec4(inverse, vec4( 1,  1, 0, 1));
			struct vec4 tl = mat4_multiply_with_vec4(inverse, vec4( 1, -1, 0, 1));
			struct vec4 br = mat4_multiply_with_vec4(inverse, vec4(-1,  1, 0, 1));
			struct vec4 bl = mat4_multiply_with_vec4(inverse, vec4(-1, -1, 0, 1));
			
			float minx = tr.x, maxx = tr.x;
			
			if (tl.x < minx) { minx = tl.x; } if (tl.x > maxx) { maxx = tl.x; }
			if (br.x < minx) { minx = br.x; } if (br.x > maxx) { maxx = br.x; }
			if (bl.x < minx) { minx = bl.x; } if (bl.x > maxx) { maxx = bl.x; }
			
			float miny = tr.y, maxy = tr.y;
			
			if (tl.y < miny) { miny = tl.y; } if (tl.y > maxy) { maxy = tl.y; }
			if (br.y < miny) { miny = br.y; } if (br.y > maxy) { maxy = br.y; }
			if (bl.y < miny) { miny = bl.y; } if (bl.y > maxy) { maxy = bl.y; }
			
			unsigned i = 0;
			
			struct {
				struct {
					float x, y, r, g, b;
				} to, from;
			} points[30] = {};
			
			for (double   incx = pow(5, round(log(maxx - minx) / log(5)) - 1),
				        startx  = minx - remainder(minx, incx),
				          endx  = maxx - remainder(maxx, incx);
				        startx <= endx;
				        startx += incx)
			{
				points[i].to.x = startx;
				points[i].to.y = maxy;
				points[i].to.r = 0.1;
				points[i].to.g = 0.1;
				points[i].to.b = 0.1;
				
				points[i].from.x = startx;
				points[i].from.y = miny;
				points[i].from.r = 0.1;
				points[i].from.g = 0.1;
				points[i].from.b = 0.1;
				i++;
			}
			
			for (double   incy = pow(5, round(log(maxy - miny) / log(5)) - 1),
				        starty  = miny - remainder(miny, incy),
				          endy  = maxy - remainder(maxy, incy);
				        starty <= endy;
				        starty += incy)
			{
				points[i].to.x = maxx;
				points[i].to.y = starty;
				points[i].to.r = 0.1;
				points[i].to.g = 0.1;
				points[i].to.b = 0.1;
				
				points[i].from.x = minx;
				points[i].from.y = starty;
				points[i].from.r = 0.1;
				points[i].from.g = 0.1;
				points[i].from.b = 0.1;
				i++;
			}
			
			{
				points[i].to.x = maxx;
				points[i].to.y = 0;
				points[i].to.r = 0.75;
				points[i].to.g = 0.1;
				points[i].to.b = 0.1;
				
				points[i].from.x = minx;
				points[i].from.y = 0;
				points[i].from.r = 0.75;
				points[i].from.g = 0.1;
				points[i].from.b = 0.1;
				i++;
			}
			
			{
				points[i].to.x = 0;
				points[i].to.y = maxy;
				points[i].to.r = 0.1;
				points[i].to.g = 0.1;
				points[i].to.b = 0.75;
				
				points[i].from.x = 0;
				points[i].from.y = miny;
				points[i].from.r = 0.1;
				points[i].from.g = 0.1;
				points[i].from.b = 0.75;
				i++;
			}
			
			assert(i <= sizeof(points) / sizeof(*points));
			
			glNamedBufferData(grid.buffer, sizeof(points), points, GL_DYNAMIC_DRAW);
		}
		
		void framebuffer_size_callback(
			GLFWwindow *window,
			int new_width, int new_height)
		{
			width = new_width, height = new_height;
			
			glViewport(0, 0, width, height);
			
			projection = mat4_scale(1.0, (double) width / height, 1.0, 1.0);
			
			update_mvp();
			
			update_grid();
		}
		
		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
		
		void cursor_position_callback(
			GLFWwindow *window,
			double xpos, double ypos)
		{
			static struct {
				bool left_pressed, right_pressed;
				struct mat4 view;
				double xpos, ypos;
			} was;
			
			bool left_pressed = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
			bool right_pressed = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;
			
			if (left_pressed)
			{
				xpos = map(xpos, 0, width, -1,  1);
				ypos = map(ypos, 0, width,  1, -1);
				
				if (was.left_pressed)
				{
					view = mat4_multiply(mat4_translate(xpos - was.xpos, ypos - was.ypos, 0), was.view);
					
					update_mvp();
					
					update_grid();
				}
				else
				{
					was.view = view;
					
					was.xpos = xpos;
					was.ypos = ypos;
				}
			}
			else if (right_pressed)
			{
				if (was.right_pressed)
				{
					double rads;
					
					if (was.xpos < width / 2)
						rads = map(ypos - was.ypos, 0, height, 0, +M_PI);
					else
						rads = map(ypos - was.ypos, 0, height, 0, -M_PI);
					
					view = mat4_multiply(mat4_rotateZ(rads), was.view);
					
					update_mvp();
					
			 		update_grid();
				}
				else
				{
					was.view = view;
					
					was.xpos = xpos;
					was.ypos = ypos;
				}
			}
			
			was.left_pressed = left_pressed;
			was.right_pressed = right_pressed;
		}

		glfwSetCursorPosCallback(window, cursor_position_callback);
		
		void scroll_callback(
			GLFWwindow* window,
			double xoffset, double yoffset)
		{
			double f = (10 + yoffset) / 10;
			
			struct mat4 scale;
			
			if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
				scale = mat4_scale(1, f, 1, 1);
			else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
				scale = mat4_scale(f, 1, 1, 1);
			else
				scale = mat4_scale(f, f, 1, 1);
			
			view = mat4_multiply(scale, view);
			
			update_mvp();
			
			update_grid();
		}
		
		glfwSetScrollCallback(window, scroll_callback);
		
		glPointSize(10);
		glEnable(GL_MULTISAMPLE);
		
		while (!glfwWindowShouldClose(window))
		{
			glfwPollEvents();
			
			if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
				glfwSetWindowShouldClose(window, true);
			
			if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
			{
				#define F (0.05)
				
				view = mat4_identity();
	/*			view = mat4_add(view,*/
	/*				mat4_multiply(*/
	/*					mat4_scale(F, F, F, F),*/
	/*					mat4_subtract(mat4_identity(), view)));*/
				
				update_mvp();
				
				update_grid();
			}
			
			glClearColor(0.65f, 0.7f, 0.7f, 1.0f);
			
			glClear(GL_COLOR_BUFFER_BIT);
			
			glUseProgram(program_id);
			
			glLineWidth(2);
			glBindVertexArray(grid.vertex_array);
			{
				glDrawArrays(GL_LINES, 0, 2 * 30);
			}
			glBindVertexArray(0);
			
			glLineWidth(4);
			glBindVertexArray(plot.vertex_array);
			{
				glDrawArrays(GL_LINE_STRIP, 0, plotme.n);
			}
			glBindVertexArray(0);
			
			
			glfwSwapBuffers(window);
		}
		
		#if 0
		glDeleteVertexArrays(sizeof(va) / sizeof(unsigned), (GLuint*) &va);
		
		glDeleteBuffers(sizeof(buf) / sizeof(unsigned), (GLuint*) &buf);
		#endif
		
		glfwTerminate();
	}
	
	free(plotme.points);
	
	EXIT;
	return 0;
}



















