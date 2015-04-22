#include "Entity.h"
#include "Shader.h"

class Background : public Entity
{


public:
	Background(ID3D11Device* device);

	void Render(ID3D11DeviceContext* deviceContext, Shader* shader);

	~Background();



};