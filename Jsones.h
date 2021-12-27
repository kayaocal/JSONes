//###############################################################################################################
//***************************************************************************************************************
//                                 _  _____  ____  _   _           
//                                | |/ ____|/ __ \| \ | |          
//                                | | (___ | |  | |  \| | ___  ___ 
//                            _   | |\___ \| |  | | . ` |/ _ \/ __|
//                           | |__| |____) | |__| | |\  |  __/\__ \
//                            \____/|_____/ \____/|_| \_|\___||___/ by Enes Kaya OCAL
//                                                                                                       
//***************************************************************************************************************
//###############################################################################################################
//  After spending long time to choose an json library for my game engine project, I decided to make new one.
//  JSONes - just another small json library. it is just a parser and writer. No reflection or fancy specs.
//  JSONes's name is coming from merge of json and enes words.
//  
//  
//
// You may need JSONES_API for importing or exporting in your library
#ifndef JSONES_API
#ifdef OyunEngine_EXPORTS
#define JSONES_API __declspec(dllexport)
#else
#define JSONES_API __declspec(dllimport)
#endif
#endif


#ifndef JSONES_H__
#define JSONES_H__

#include <assert.h>
#include <string>
#include <map>
#include <vector>
#include <sstream>

// Use (void) to silent unused warnings.
#define jassert(exp, msg) assert(((void)msg, exp))

#define jassert_m(cond, msg) do \
{ if (!(cond)) { std::ostringstream str; str << msg; std::cerr << str.str(); std::abort(); } \
} while(0)


namespace Jsones
{
    enum class JSONES_API JType : char
    {
        NUL = 0,
        OBJ = 1,
        NUM = 2,
        STR = 3,
        BOOL = 4,
        ARR = 5
    };

    struct JArr;
    struct JObj;

    struct JSONES_API JVal
    {
        JType type;

        explicit JVal(JType t);

        virtual ~JVal();

        template <typename T>
        void operator =(const T& v)
        {
            jassert(false, "Undefined Type");
        }

        void operator =(const int& v);
        void operator =(const float& v);
        void operator =(const double& v);
        void operator =(const bool v);
        void operator =(const std::string& str);
        void operator =(const char* str);
        void operator =(JObj* o);
        void operator =(const JObj& o);
        void operator =(JObj&& o) noexcept;

        virtual std::string& ToString();
    };
    
    struct JSONES_API JNull : public JVal
    {
        JNull();

        ~JNull() override;

        virtual std::string& ToString() override;
    };
    
    struct JSONES_API JStr : public JVal
    {
        std::string str;
        JStr(const char* str, size_t beg, size_t end) noexcept;
        JStr(const std::string& str) noexcept;
        JStr(std::string&& str) noexcept;
        JStr(const char* ch) noexcept;
        JStr(const JStr& s) noexcept;

        ~JStr() override;

        virtual std::string& ToString() override;
    };

    struct JSONES_API JNumber : public JVal
    {
        std::string str;
        JNumber(const char* s, size_t b, size_t e);
        explicit JNumber(int s);
        explicit JNumber(float s);
        explicit JNumber(double s);
        JNumber(const JNumber& n) noexcept;
        ~JNumber() override;
        bool IsInteger();
        int AsInt() const;
        float AsFloat() const;
        double AsDouble() const;

        virtual std::string& ToString() override;

    };

    struct JSONES_API JBool : public JVal
    {
        bool val;
        JBool(bool b);
        JBool(const JBool& b) noexcept;
        ~JBool() override;

        virtual std::string& ToString() override;
    };

    
    /// @brief JObj is JsonObject. JObj contains map<string, JVal> objects and it's parent object.
    struct JSONES_API JObj : public JVal
    {
        std::map<uint32_t, JVal*> objects;

        /// @brief Constructor
        explicit JObj();
        ~JObj() override;

        JObj(JObj&& objR) noexcept;
        //JObj(std::initializer_list<std::pair<uint32_t, JVal*>> list);
        JObj(const JObj&) noexcept;

        JObj(const char* str);
        JObj(std::initializer_list<std::pair<uint32_t, JVal*>>);

        void Add(std::pair<uint32_t, JVal*> add);
        void Add(std::pair<uint32_t, JArr*> add);
        void Add(std::pair<uint32_t, JObj*> add);
        JVal* Get(uint32_t hash);


        /// @brief This function returns JObj* with given key. *Might return nullptr if given key does not exist or cant cast!* 
        JObj* GetObj(const std::string& key);

        JArr* GetArr(const std::string& key);

        void RemoveKeyValue(const std::string& key);

        template <typename T>
        void Set(const std::string& key, T v)
        {
            JVal* val = Get(key);
            jassert(val != nullptr, "JObj does not contains given key!");
            *val = v;
        }

        JVal& operator[](const std::string&);

        void operator =(const JObj& o);
        void operator =(JObj&& o) noexcept;
    };

    struct JSONES_API JArr : public JVal
    {
        std::vector<JVal*> array;
        
        /// @brief Constructor
        JArr();
        ~JArr() override;
        JArr(JArr&& ar) noexcept;
        JArr(const JArr& ar);
        JArr(const char* str);

        template <typename T>
        JArr(std::initializer_list<T> list)
            : JVal(JType::ARR)
        {
            std::cout<<"Jarr initializer list"<<std::endl;
            auto it = list.begin();
            while (it != list.end())
            {
                Add(*it);
                it++;
            }
        }

        JArr& Add(const JObj& obj);
        JArr& Add(JObj&& obj);
        void Add(JObj* obj);
        void Add(double value);
        void Add(int value);
        void Add(float value);
        void Add(const char* str, int b, int e);
        void Add(bool value);
        void Add(const char* str);
        void Add(const std::string& str);
        void Add(const std::string&& str);

        void RemoveAt(int index);
        JVal& operator[](int index);

        void operator =(const JArr& o);
        void operator =(JArr&& o) noexcept;
        
        void PushBack(JVal* val);
        void PushBack(const char* str, int b, int e);
    };


    uint32_t GetHash(const char* str);
    uint32_t GetHash(const std::string& str);
    uint32_t GetKeyHash(const std::string str);
    uint32_t GetKeyHash(const char* str, size_t b, size_t e);

    //********************************************** ***** **********************************************
    //********************************************** JPair **********************************************
    
    JSONES_API __forceinline std::pair<uint32_t, JVal*> JPair(const std::string& key, const std::string& val)
    {
        return std::pair<uint32_t, JVal*>(GetKeyHash(key), new JStr(val));
    }

    JSONES_API __forceinline std::pair<uint32_t, JVal*> JPair(const std::string& key, int val)
    {
        return std::pair<uint32_t, JVal*>(GetKeyHash(key), new JNumber(val));
    }
    
    JSONES_API __forceinline std::pair<uint32_t, JVal*> JPair(const std::string& key, double val)
    {
        return std::pair<uint32_t, JVal*>(GetKeyHash(key), new JNumber(val));
    }

    JSONES_API __forceinline std::pair<uint32_t, JVal*> JPair(const std::string& key, float val)
    {
        return std::pair<uint32_t, JVal*>(GetKeyHash(key), new JNumber(val));
    }

    JSONES_API __forceinline std::pair<uint32_t, JVal*> JPair(const std::string& key, bool val)
    {
        return std::pair<uint32_t, JVal*>(GetKeyHash(key), new JBool(val));
    }

    JSONES_API __forceinline std::pair<uint32_t, JVal*> JPair(const std::string& key, const char* str)
    {
        return std::pair<uint32_t, JVal*>(GetKeyHash(key), new JStr(str));
    }

    JSONES_API __forceinline std::pair<uint32_t, JVal*> JPair(const std::string& key, std::string&& rStr)
    {
        return std::pair<uint32_t, JVal*>(GetKeyHash(key), new JStr(rStr));
    }

    JSONES_API __forceinline std::pair<uint32_t, JVal*> JPair(const std::string& key, JObj* obj)
    {
        return std::pair<uint32_t, JVal*>(GetKeyHash(key), static_cast<JVal*>(obj));
    }

    JSONES_API __forceinline std::pair<uint32_t, JVal*> JPair(const std::string& key, const JObj& obj)
    {
        return std::pair<uint32_t, JVal*>(GetKeyHash(key), static_cast<JVal*>(new JObj(obj)));
    }

    JSONES_API __forceinline std::pair<uint32_t, JVal*> JPair(const std::string& key, JObj&& obj)
    {
        return std::pair<uint32_t, JVal*>(GetKeyHash(key), static_cast<JVal*>(new JObj(std::move(obj))));
    }

    JSONES_API __forceinline std::pair<uint32_t, JVal*> JPair(const std::string& key, JArr* arr)
    {
        return std::pair<uint32_t, JVal*>(GetKeyHash(key), static_cast<JVal*>(arr));
    }
    
    JSONES_API __forceinline std::pair<uint32_t, JVal*> JPair(const std::string& key, const JArr& arr)
    {
        return std::pair<uint32_t, JVal*>(GetKeyHash(key), static_cast<JVal*>(new JArr(arr)));
    }

    JSONES_API __forceinline std::pair<uint32_t, JVal*> JPair(const std::string& key, JArr&& arr)
    {
        JArr* jarr = new JArr(std::move(arr));
        return std::pair<uint32_t, JVal*>(GetKeyHash(key), static_cast<JVal*>(jarr));
    }
   

    //********************************************** ***** **********************************************


    void PrintArray(JArr* arr, int tab = 0);
    void PrintObj(JObj* obj, int tab = 0);
    
    /// @brief Parses json
    /// @return JObj
    JSONES_API JObj* JObject(const char*);
    
    JSONES_API JArr* JArray(const char*);

    /// @brief Converts JObj to json string
    /// @param root 
    /// @param tab just give 0
    /// @param beautify if true
    /// @return 
    JSONES_API std::stringstream JWrite(JObj* root, bool beautify = false, int tab = 0);

    /// @brief Converts JArr to json string
   /// @param arr 
   /// @param tab just give 0
   /// @param beautify if true
   /// @return 
    std::stringstream JWrite(JArr* arr, bool beautify = false, int tab = 0);
}


#endif//JSONES_H__
