#include"Graphics/Graphics.h"
#include"ResourceManager.h"

//���f�����\�[�X�ǂݍ���
std::shared_ptr<ModelResource>ReosurceManager::LoadModelResource(const char* filename)
{
    // ���f������
    auto it = models.find(filename);
    if (it != models.end()) {
        // ���f�������������ꍇ�A���̃��f�����擾���ĕԂ�
        if (auto model = it->second.lock()) {
            return model;
        }
    }

    // �V�K���f�����\�[�X�쐬���ǂݍ���
    std::shared_ptr<ModelResource> model = std::make_shared<ModelResource>();
    model->Load(Graphics::Instance().GetDevice(), filename);
    // �}�b�v�ɓo�^
    models[filename] = model;
    return model;
}