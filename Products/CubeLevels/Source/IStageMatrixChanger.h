#pragma once

enum eRotationMatrix
{
	eRotationUp,
	eRotationDown,
	eRotationRight,
	eRotationLeft,
};

class IStageMatrixChanger
{
public:
	virtual ~IStageMatrixChanger() = 0;
	virtual void ChangeMatrix(eRotationMatrix rotationDirect) = 0;
private:

};