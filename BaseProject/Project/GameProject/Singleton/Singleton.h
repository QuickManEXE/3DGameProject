#pragma once

//template<class T>
//class Singleton
//{
//protected:
//
//    static T m_Instance;
//
//public:
//
//    static inline void CreateInstance() {
//
//        m_Instance = (*new T);
//
//    }
//
//    static inline T& GetInstance()
//    {
//         return m_Instance;
//    }
//
//    static inline void ReleaseInstance() {
//        if (m_Instance) {
//            m_Instance = nullptr;
//        }  
//    }
//
//};

//template<class T>
//class Singleton
//{
//public:
//    static inline T& GetInstance()
//    {
//        //static T instance;
//        return instance;
//    }
//
//protected:
//    Singleton() {} // 外部でのインスタンス作成は禁止
//    virtual ~Singleton() {}
//    static T instance;
//
//private:
//    
//    void operator=(const Singleton& obj) {} // 代入演算子禁止
//    Singleton(const Singleton& obj) {} // コピーコンストラクタ禁止
//};

template<class T>
class Singleton
{
protected:
    //  コンストラクタの外部での使用を禁止
    Singleton() {}
    virtual ~Singleton() {}
private:
    // コピーコンストラクタの使用を禁止
    Singleton(const Singleton&);
    Singleton& operator = (const Singleton&);
public:
    static void Build()
    {
        assert(!m_pInstance);
        m_pInstance = new T;
    }
    static void Release()
    {
        assert(m_pInstance);
        m_pInstance = null;
    }
    static T& Instance()
    {
        return *m_pInstance;
    }
private:
    static T* m_pInstance;
};

template<class T>
T* Singleton<T>::m_pInstance = NULL;