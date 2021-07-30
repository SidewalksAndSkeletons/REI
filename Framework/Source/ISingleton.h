#pragma once

template <class T>
class ISingleton
{
private:
    static std::weak_ptr<T> Instance;

public:
    static std::shared_ptr<T> CreateInstance()
    {
        std::shared_ptr<T> Singleton = Instance.lock();
        if (!Singleton)
        {
            Singleton.reset(new T());
            Instance = Singleton;
        }

        return Singleton;
    }
};

template <class T>
std::weak_ptr<T> ISingleton<T>::Instance;
