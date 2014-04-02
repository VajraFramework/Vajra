#ifndef OPENGLCOUNTER_H
#define OPENGLCOUNTER_H

class OpenGLCounter {
public:
	~OpenGLCounter();

	void CountGlCall(const char* glFunctionName);

private:
	void init();
	void destroy();

	OpenGLCounter();


	unsigned int countOfAllGlCallsThisFrame;

	void switchFrames();

	friend class OpenGLWrapper;
	friend class Engine;
};

#endif // OPENGLCOUNTER_H
