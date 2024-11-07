#include"Graphics/Graphics.h"
#include"Effect.h"
#include"EffectManager.h"

//�R���X�g���N�^
Effect::Effect(const char* filename)
{
	//�ʃX���b�h���Ƀf�o�C�X�R���e�L�X�g���g��ꂽ�ꍇ��
	//�����A�N�Z�X���Ȃ��悤�ɔr�����䂷��
	std::lock_guard<std::mutex>lock(Graphics::Instance().GetMutex());

	//Effekseer�̃��\�[�X��ǂݍ���
	//Effekseer��UTF-16�̃t�@�C���p�X�ȊO�͑Ή����ĂȂ����ߕ����R�[�h�ϊ����K�v
	char16_t utf16FIlename[256];
	Effekseer::ConvertUtf8ToUtf16(utf16FIlename, 256, filename);

	//Effekseer::Manager���擾
	Effekseer::ManagerRef effekseerManager = EffectManager::Instace().GetEffekseerManager();

	//Effekseer�G�t�F�N�g��ǂݍ���
	effekseerEffect = Effekseer::Effect::Create(effekseerManager, (EFK_CHAR*)utf16FIlename);
}

//�Đ�
Effekseer::Handle Effect::Play(const DirectX::XMFLOAT3& position, float scale)
{
	Effekseer::ManagerRef effekseerManager = EffectManager::Instace().GetEffekseerManager();
	Effekseer::Handle handle = effekseerManager->Play(effekseerEffect, position.x, position.y, position.z);
	effekseerManager->SetScale(handle, scale, scale, scale);
	return handle;
}

//��~
void Effect::Stop(Effekseer::Handle handle)
{
	Effekseer::ManagerRef effekseerManager = EffectManager::Instace().GetEffekseerManager();
	effekseerManager->StopEffect(handle);
}

//���W�ݒ�
void Effect::SetPosition(Effekseer::Handle handle, const DirectX::XMFLOAT3& position)
{
	Effekseer::ManagerRef effekseerManager = EffectManager::Instace().GetEffekseerManager();
	effekseerManager->SetLocation(handle, position.x, position.y, position.z);
}

//�X�P�[���ݒ�
void Effect::SetScale(Effekseer::Handle handle, const DirectX::XMFLOAT3& scale)
{
	Effekseer::ManagerRef effekseerManager = EffectManager::Instace().GetEffekseerManager();
	effekseerManager->SetScale(handle, scale.x, scale.y, scale.z);
}