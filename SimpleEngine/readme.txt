Shader �߰���
Shader���ϰ� ����Ǵ� ShaderŬ�� ����
	ShaderŬ�� �ۼ�
	- Initialize�� ���� ����
	- SetUniform �ۼ�

BaseShader::GetShader�� ���� �߰� / �ش� Ŭ�󽺿� ����

���� ����
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
			
		
