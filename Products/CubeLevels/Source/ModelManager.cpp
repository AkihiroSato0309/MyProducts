#include "ModelManager.h"
#include "Global.h"

using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;

//*********************************************************************
// �R���X�g���N�^
//*********************************************************************
ModelManager::ModelManager()
{
	
}


//*********************************************************************
// �f�X�g���N�^
//*********************************************************************
ModelManager::~ModelManager()
{

}


//*********************************************************************
// ���f���̓o�^
//*********************************************************************
Model* ModelManager::EntryModel(eModels modelNumber, const wchar_t* modelName)
{
	// ���f���̓ǂݍ���
	Model* model;
	model = Model::CreateFromCMO(g_pd3dDevice, L"Cube.cmo", *g_FXFactory).get();

	// �}�b�v�o�^
	_models.insert( map<eModels, Model*>::value_type( modelNumber, model ));

	return nullptr;
}


//*********************************************************************
// ���f���̃|�C���^�擾
//*********************************************************************
Model* ModelManager::GetModel(eModels modelNumber)
{

	return nullptr;
}
