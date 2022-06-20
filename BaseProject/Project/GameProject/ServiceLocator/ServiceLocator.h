#pragma once
#include <map>

template <typename T>
class ServiceLocator
{
private:

    std::map<std::string, T> salary;
    //�T�[�r�X�̕ێ��E�擾
    static T* Instance;

public:

    //�T�[�r�X�̓o�^
    static void Bind(T* instance)
    {
        Instance = instance;
    }

    //�T�[�r�X�̊J��
    static void UnBind()
    {
        Instance = nullptr;
    }
    
    //�T�[�r�X�̎擾
    T* GetService<T>() {
        return 

    }

};