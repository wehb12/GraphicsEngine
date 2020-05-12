#pragma once

#include <vector>
#include <memory>

class GWindow;
class GraphicsShader;

class GRenderer
{
public:
	GRenderer();

	~GRenderer();

	void Init();

	void Run();

private:
	void InitGraphics();

	void InitWindow();

	void MakeWindow();

	int PostWindowInit();

	void CompileShaders();

	bool CompileShader(const char* ShaderSource, GraphicsShader& Shader);

	bool LinkShader(GraphicsShader& ShaderProgram, std::vector<GraphicsShader> ShaderArray);

	void RenderScene();

	void Terminate();

private:
	std::unique_ptr<GWindow> WindowPtr;

	// Hello Triangle
	std::unique_ptr<GraphicsShader> VertexShaderPtr;
	std::unique_ptr<GraphicsShader> FragmentShaderPtr;
	std::unique_ptr<GraphicsShader> ShaderProgramPtr;

	unsigned int VAO = 0;
	unsigned int VBO = 0;
};
