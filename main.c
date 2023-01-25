
#include <time.h>
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
					points[i].x = plotme.points[i].x;
					points[i].y = plotme.points[i].y;
					
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
		
		time_t initial = time(NULL), now = initial;
		
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
		
		struct {
			struct {
				float x, y, r, g, b;
			} to, from;
		} points[801] = {};
		
		float minx, maxx, miny, maxy;
		
		void update_grid()
		{
			struct mat4 mvp = mat4_multiply(projection, view);
			
			struct mat4 inverse = mat4_invert(mvp);
			
			struct vec4 tr = mat4_multiply_with_vec4(inverse, vec4( 1,  1, 0, 1));
			struct vec4 tl = mat4_multiply_with_vec4(inverse, vec4( 1, -1, 0, 1));
			struct vec4 br = mat4_multiply_with_vec4(inverse, vec4(-1,  1, 0, 1));
			struct vec4 bl = mat4_multiply_with_vec4(inverse, vec4(-1, -1, 0, 1));
			
			minx = tr.x, maxx = tr.x;
			
			if (tl.x < minx) { minx = tl.x; } if (tl.x > maxx) { maxx = tl.x; }
			if (br.x < minx) { minx = br.x; } if (br.x > maxx) { maxx = br.x; }
			if (bl.x < minx) { minx = bl.x; } if (bl.x > maxx) { maxx = bl.x; }
			
			miny = tr.y, maxy = tr.y;
			
			if (tl.y < miny) { miny = tl.y; } if (tl.y > maxy) { maxy = tl.y; }
			if (br.y < miny) { miny = br.y; } if (br.y > maxy) { maxy = br.y; }
			if (bl.y < miny) { miny = bl.y; } if (bl.y > maxy) { maxy = bl.y; }
			
			unsigned i = 0;
			
			// horizontal gridlines:
			{
				time_t mint = floor(minx) + initial;
				struct tm mintm = *localtime(&mint);
				
				time_t maxt = ceil(maxx) + initial;
				struct tm maxtm = *localtime(&maxt);
				
				double span = maxx - minx;
				
				// seconds (2 minutes)
				if (span < 2 * 60)
				{
					for (double startx  = minx - remainder(minx, 1),
								  endx  = maxx - remainder(maxx, 1);
								startx <= endx;
								startx += 1)
					{
						points[i].to.x = startx;
						points[i].to.y = maxy;
						points[i].to.r = 0.6;
						points[i].to.g = 0.1;
						points[i].to.b = 0.1;
						
						points[i].from.x = startx;
						points[i].from.y = miny;
						points[i].from.r = 0.6;
						points[i].from.g = 0.1;
						points[i].from.b = 0.1;
						i++;
					}
				}
				
				// minutes (2 hours)
				if (span < 2 * 60 * 60)
				{
					struct tm tm = mintm;
					tm.tm_sec = 0;
					
					while (i < 800 && (false
						|| tm.tm_min  <= maxtm.tm_min
						|| tm.tm_hour <  maxtm.tm_hour
						|| tm.tm_mday <  maxtm.tm_mday
						|| tm.tm_mon  <  maxtm.tm_mon
						|| tm.tm_year <  maxtm.tm_year))
					{
						time_t moving = mktime(&tm);
						
						points[i].to.x = moving - initial;
						points[i].to.y = maxy;
						points[i].to.r = 0.6;
						points[i].to.g = 0.6;
						points[i].to.b = 0.1;
						
						points[i].from.x = moving - initial;
						points[i].from.y = miny;
						points[i].from.r = 0.6;
						points[i].from.g = 0.6;
						points[i].from.b = 0.1;
						i++, tm.tm_min++;
					}
				}
				
				// hours (3 days)
				if (span < 3 * 24 * 60 * 60)
				{
					struct tm tm = mintm;
					tm.tm_min = 0, tm.tm_sec = 0;
					
					while (i < 800 && (false
						|| tm.tm_hour <= maxtm.tm_hour
						|| tm.tm_mday <  maxtm.tm_mday
						|| tm.tm_mon  <  maxtm.tm_mon
						|| tm.tm_year <  maxtm.tm_year))
					{
						time_t moving = mktime(&tm);
						
						points[i].to.x = moving - initial;
						points[i].to.y = maxy;
						points[i].to.r = 0.1;
						points[i].to.g = 0.6;
						points[i].to.b = 0.1;
						
						points[i].from.x = moving - initial;
						points[i].from.y = miny;
						points[i].from.r = 0.1;
						points[i].from.g = 0.6;
						points[i].from.b = 0.1;
						i++, tm.tm_hour++;
					}
				}
				
				// days (3 years)
				if (span < 3 * 365 * 24 * 60 * 60)
				{
					struct tm tm = mintm;
					tm.tm_hour = 0, tm.tm_min = 0, tm.tm_sec = 0;
					
					while (i < 800 && (false
						|| tm.tm_mday <= maxtm.tm_mday
						|| tm.tm_mon  < maxtm.tm_mon
						|| tm.tm_year < maxtm.tm_year))
					{
						time_t moving = mktime(&tm);
						
						points[i].to.x = moving - initial;
						points[i].to.y = maxy;
						points[i].to.r = 0.1;
						points[i].to.g = 0.6;
						points[i].to.b = 0.6;
						
						points[i].from.x = moving - initial;
						points[i].from.y = miny;
						points[i].from.r = 0.1;
						points[i].from.g = 0.6;
						points[i].from.b = 0.6;
						i++, tm.tm_mday++;
					}
				}
				
				// weeks (3 years)
				if (span < 3 * 365 * 24 * 60 * 60)
				{
					struct tm tm = mintm;
					tm.tm_mday = 0, tm.tm_hour = 0, tm.tm_min = 0, tm.tm_sec = 0;
					
					do tm.tm_mday++, mktime(&tm); while (tm.tm_wday != 0);
					
					tm.tm_hour = 0, tm.tm_min = 0, tm.tm_sec = 0;
					
					while (i < 800 && (false
						|| tm.tm_mon  <= maxtm.tm_mon
						|| tm.tm_year <  maxtm.tm_year))
					{
						time_t moving = mktime(&tm);
						
						points[i].to.x = moving - initial;
						points[i].to.y = maxy;
						points[i].to.r = 0.1;
						points[i].to.g = 0.1;
						points[i].to.b = 0.6;
						
						points[i].from.x = moving - initial;
						points[i].from.y = miny;
						points[i].from.r = 0.1;
						points[i].from.g = 0.1;
						points[i].from.b = 0.6;
						i++, tm.tm_mday += 7;
					}
				}
				
				// months (3 years)
				if (span < 3 * 365 * 24 * 60 * 60)
				{
					struct tm tm = mintm;
					tm.tm_mon = 0;
					
					while (i < 800 && (false
						|| tm.tm_mon < 12
						|| tm.tm_year < maxtm.tm_year))
					{
						tm.tm_mday = 1, tm.tm_hour = 0, tm.tm_min = 0, tm.tm_sec = 0;
						
						time_t moving = mktime(&tm);
						
						points[i].to.x = moving - initial;
						points[i].to.y = maxy;
						points[i].to.r = 0.6;
						points[i].to.g = 0.1;
						points[i].to.b = 0.6;
						
						points[i].from.x = moving - initial;
						points[i].from.y = miny;
						points[i].from.r = 0.6;
						points[i].from.g = 0.1;
						points[i].from.b = 0.6;
						i++, tm.tm_mon++;
					}
				}
			}
			
			// horizontal gridlines:
			{
				double span = maxy - miny;
				
				// cents (3 dollars)
				if (span < 3)
				{
					for (double starty  = miny - remainder(miny, 0.01),
								  endy  = maxy - remainder(maxy, 0.01);
								starty <= endy;
								starty += 0.01)
					{
						points[i].to.x = maxx;
						points[i].to.y = starty;
						points[i].to.r = 0.6;
						points[i].to.g = 0.1;
						points[i].to.b = 0.1;
						
						points[i].from.x = minx;
						points[i].from.y = starty;
						points[i].from.r = 0.6;
						points[i].from.g = 0.1;
						points[i].from.b = 0.1;
						i++;
					}
				}
				
				// dollars (300 dollars)
				if (span < 300)
				{
					for (double starty  = miny - remainder(miny, 1),
								  endy  = maxy - remainder(maxy, 1);
								starty <= endy && i < 800;
								starty += 1)
					{
						points[i].to.x = maxx;
						points[i].to.y = starty;
						points[i].to.r = 0.6;
						points[i].to.g = 0.6;
						points[i].to.b = 0.1;
						
						points[i].from.x = minx;
						points[i].from.y = starty;
						points[i].from.r = 0.6;
						points[i].from.g = 0.6;
						points[i].from.b = 0.1;
						i++;
					}
				}
				
				// 100 dollars (3,000 dollars)
				if (span < 3 * 1000)
				{
					for (double starty  = miny - remainder(miny, 100),
								  endy  = maxy - remainder(maxy, 100);
								starty <= endy && i < 800;
								starty += 100)
					{
						points[i].to.x = maxx;
						points[i].to.y = starty;
						points[i].to.r = 0.1;
						points[i].to.g = 0.6;
						points[i].to.b = 0.1;
						
						points[i].from.x = minx;
						points[i].from.y = starty;
						points[i].from.r = 0.1;
						points[i].from.g = 0.6;
						points[i].from.b = 0.1;
						i++;
					}
				}
				
				// 1000 dollars (50,000 dollars)
				if (span < 50 * 1000)
				{
					for (double starty  = miny - remainder(miny, 1000),
								  endy  = maxy - remainder(maxy, 1000);
								starty <= endy && i < 800;
								starty += 1000)
					{
						points[i].to.x = maxx;
						points[i].to.y = starty;
						points[i].to.r = 0.1;
						points[i].to.g = 0.6;
						points[i].to.b = 0.6;
						
						points[i].from.x = minx;
						points[i].from.y = starty;
						points[i].from.r = 0.1;
						points[i].from.g = 0.6;
						points[i].from.b = 0.6;
						i++;
					}
				}
			}
			
			// horizontal axis:
			if (i < 800)
			{
				points[i].to.x = maxx;
				points[i].to.y = 0;
				points[i].to.r = 0;
				points[i].to.g = 0;
				points[i].to.b = 0;
				
				points[i].from.x = minx;
				points[i].from.y = 0;
				points[i].from.r = 0;
				points[i].from.g = 0;
				points[i].from.b = 0;
				i++;
			}
			
			assert(i <= 800);
			
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
				view = mat4_identity();
				update_mvp();
				update_grid();
			}
			
			glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
			
			glClear(GL_COLOR_BUFFER_BIT);
			
			glUseProgram(program_id);
			
			time(&now);
			
			points[800].to.x = now - initial;
			points[800].to.y = maxy;
			points[800].to.r = 0;
			points[800].to.g = 0;
			points[800].to.b = 0;
			
			points[800].from.x = now - initial;
			points[800].from.y = miny;
			points[800].from.r = 0;
			points[800].from.g = 0;
			points[800].from.b = 0;
			
			glNamedBufferData(grid.buffer, sizeof(points), points, GL_DYNAMIC_DRAW);
			
			glLineWidth(2);
			glBindVertexArray(grid.vertex_array);
			{
				glDrawArrays(GL_LINES, 0, 2 * (800 + 1));
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



















