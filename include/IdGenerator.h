#ifndef ID_GENERATOR_H
#define ID_GENERATOR_H

#include <type_traits>
#include <limits>
#include <string>
#include <unordered_map>
#include <mutex>
#include <fstream>
#include <memory>
#include <exception>
#include <iostream>
#include <limits>

using namespace std;

namespace IdGenerator
{
    
    struct Settings
    {
        bool loop;
        
        // For the future
        bool operator== (const Settings& settings) const
        {
            return settings.loop == loop;
        }
    };
    
    class StorageConnectException: 
        public exception
    {
        public:
            virtual char const* what() const throw()
            {
                return "Cannot open Generator Index Storage!";
            }
    };        
    
    class OutOfLimitException:
        public exception
    {
        public:
            virtual char const* what() const throw()
            {
                return "Index generator reached the max value!";
            }        
    };
    
    template<typename T> class GeneratorBuilder;
    

    template<typename T>
    class Generator
    {
        friend class GeneratorBuilder<T>;
        static_assert (std::is_arithmetic<T>::value && std::numeric_limits<T>::is_integer, "IdGenerator does not support this type!");
        private:
            Generator (Settings&& settings)
            {
                _lastIndex = 0;
                _limit = numeric_limits<decltype (_lastIndex)>::max ();
                _settings = forward<Settings> (settings);
                try
                {
                    ifstream stream (this->storageFileName ());
                    if (stream.is_open ())
                    {
                        if (!(stream >> _lastIndex))
                        {
                            _lastIndex = 0;
                        }
                    }                       
                }
                catch (const exception& e)
                {
                    throw StorageConnectException ();
                }
            }
            
            const Settings& settings () const
            {
                return _settings;
            }
            
            Generator () = delete;
            Generator (Settings& settings) = delete;
            Generator (const Generator<T>& generator) = delete;
            
            string storageFileName ()
            {
                string name = ".generator_" + string (typeid(T).name ());
                name += (_settings.loop) ? "_l":"";
                name += ".txt";
                return name;
            }
        public:
            T generate ()
            {
                lock_guard<mutex> locker (_mutex);
                if (_lastIndex == (_limit - 1))
                {
                    if (_settings.loop)
                    {
                        _lastIndex = 0;
                    }
                    else
                    {
                        throw OutOfLimitException ();
                    }
                }
                _lastIndex++;
                return _lastIndex;
            }
            
            ~Generator ()
            {
                lock_guard<mutex> locker (_mutex);
                try
                {
                    ofstream stream (this->storageFileName ());
                    if (stream.is_open ())
                    {
                        stream << _lastIndex;
                    }
                    else
                    {
                        throw StorageConnectException ();
                    }
                }
                catch (const exception& e)
                {
                    throw StorageConnectException ();
                }
            }
        private:
            T _lastIndex;
            Settings _settings;
            mutex _mutex;
            fstream storageStream;
            T _limit;
    };

    template<typename T>
    class GeneratorBuilder
    {
        public:
            static shared_ptr<Generator<T>> build (bool loop = false)
            {
                lock_guard<mutex> locker (_creatorMutex);
                Settings settings = {loop};
                auto itr = find_if (_generatorList.begin (), _generatorList.end (), 
                                    [&settings] (auto generator){  return (generator->settings () == settings); });
                
                if (itr == _generatorList.end ())
                {
                    shared_ptr<Generator<T>> generator (new Generator<T> (move (settings)));
                    _generatorList.push_back (generator);
                    return generator;                    
                }
                else
                {
                    return *itr;
                }
                
            }
        private:
            static vector<shared_ptr<Generator<T>>> _generatorList;
            static mutex _creatorMutex;
    };
    
    template<typename T> 
        vector<shared_ptr<Generator<T>>> GeneratorBuilder<T>::_generatorList;
    template<typename T>
        mutex GeneratorBuilder<T>::_creatorMutex;

    
}




#endif
