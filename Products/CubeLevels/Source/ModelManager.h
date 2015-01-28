//********************************************************************************
//
// ���f���̊Ǘ��N���X
//
//********************************************************************************
#pragma once

#include "Singleton.h"
#include <map>

// ���f�����̗񋓌^
enum eModels
{
	eModel_Cube,
};

//���f���N���X
class ModelManager : public Singleton<ModelManager>
{
	friend Singleton<ModelManager>;
public:

	// ���f���̓o�^
	DirectX::Model* EntryModel(eModels modelNumber, const wchar_t* modelName);

	// ���f���|�C���^�̎擾
	DirectX::Model* GetModel(eModels modelNumber);

private:
	ModelManager();
	~ModelManager();

	// ���f���ێ��}�b�v
	std::map<eModels, DirectX::Model*> _models;
};