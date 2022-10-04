#include "cBulletDebugRenderer.h"

void cBulletDebugRenderer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{


	float tmp[6] = { from.getX(), from.getY(), from.getZ(),
				to.getX(), to.getY(), to.getZ() };


	glUseProgram(program); //this is hardcoded, WATCH OUT! :)
	//glEnable(GL_DEPTH_TEST);
	glPushMatrix();
	{
		unsigned int buffer; // The ID, kind of a pointer for VRAM
		

		glGenBuffers(1, &buffer);

		glBindBuffer(GL_ARRAY_BUFFER, buffer); // Set the buffer as the active array
		glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), &tmp, GL_STATIC_DRAW); // Fill the buffer with data
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0); // Specify how the buffer is converted to vertices
		glEnableVertexAttribArray(0); // Enable the vertex array

		GLint colour_loc = glGetUniformLocation(program, "colour");
		glUniform4f(colour_loc, color.getX(), color.getY(), color.getZ(), 1.0f);


		glPointSize(5.0f);
		glDrawArrays(GL_POINTS, 0, 2);
		glDrawArrays(GL_LINES, 0, 2);

		glDeleteBuffers(1, &buffer);
	}
	glPopMatrix();
	//glUseProgram(3);

}