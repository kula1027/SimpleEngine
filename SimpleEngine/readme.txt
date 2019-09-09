Shader 추가법
Shader파일과 연결되는 Shader클라스 생성
	Shader클라스 작성
	- Initialize에 변수 연결
	- SetUniform 작성

BaseShader::GetShader에 파일 추가 / 해당 클라스와 연결

구동 순서
Engine Initialize
Load Scene to Engine
Engine Begins
	Init Scene
		Init Shaders
		foreach gameobject Init
			foreach Init components(+renderer)
		foreach updatables Init
		foreach updatables OnStart
	RenderLoop Begins
		Timer Tick
		Check Input
		foreach updatables Update
		Render
			
		
