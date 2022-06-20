#pragma once
#include <map>

template <typename T>
class ServiceLocator
{
private:

    std::map<std::string, T> salary;
    //サービスの保持・取得
    static T* Instance;

public:

    //サービスの登録
    static void Bind(T* instance)
    {
        Instance = instance;
    }

    //サービスの開放
    static void UnBind()
    {
        Instance = nullptr;
    }
    
    //サービスの取得
    T* GetService<T>() {
        return 

    }

};