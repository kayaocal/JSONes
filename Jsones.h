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
#define JSONES_API
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

namespace Jsones
{
    enum class JSONES_API JType
    {
        NUL = 0,
        OBJ = 1,
        NUM = 2,
        STR = 3,
        BOOL = 4,
        ARR = 5
    };


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

        virtual std::string ToString();
    };
    
    struct JSONES_API JNull : public JVal
    {
        JNull();

        ~JNull() override;

        virtual std::string ToString() override;
    };
    
    struct JSONES_API JStr : public JVal
    {
        const char* str;
        int begin;
        int end;
        JStr(const char* str, int beg, int end);

        ~JStr() override;

        virtual std::string ToString() override;
    };

    struct JSONES_API JNumber : public JVal
    {
        const char * str;
        int begin;
        int end;
        JNumber(const char* s, int b, int e);
        // explicit JNumber(int s);
        // explicit JNumber(float s);
        // explicit JNumber(double s);
        ~JNumber() override;
        bool IsInteger();
        int AsInt() const;
        float AsFloat() const;
        double AsDouble() const;

        virtual std::string ToString() override;

    };

    struct JSONES_API JBool : public JVal
    {
        bool val;
        JBool(bool b);
        ~JBool() override;

        virtual std::string ToString() override;
    };

    struct JArr;
    
    /// @brief JObj is JsonObject. JObj contains map<string, JVal> objects and it's parent object.
    struct JSONES_API JObj : public JVal
    {
        std::map<std::string, JVal*> objects;
        JVal* parentObj;

        /// @brief Constructor
        /// @param parent just keep it null. parentObj is only needed while parsing.
        explicit JObj(JVal* parent = nullptr);
        ~JObj() override;

        JObj(JObj&& objR) noexcept;
        JObj(const JObj&) = delete;

        JObj(std::initializer_list<std::pair<std::string, JVal*>>);

        void Add(std::pair<std::string, JVal*> add);
        void Add(std::pair<std::string, JArr*> add);
        void Add(std::pair<std::string, JObj*> add);


        /// @brief This function returns JVal* with given key. *Might return nullptr if given key does not exist!* 
        JVal* Get(const std::string& key);
        /// @brief This function returns JObj* with given key. *Might return nullptr if given key does not exist or cant cast!* 
        JObj* GetObj(const std::string& key);

        JArr* GetArr(const std::string& key);

        template <typename T>
        void Set(const std::string& key, T v)
        {
            JVal* val = Get(key);
            jassert(val != nullptr, "JObj does not contains given key!");
            *val = v;
        }

        JVal& operator[](const std::string&);
    };

    /// @brief It's derrived from JObj because it's the easiest way to parse json. You may see it on JParse function.
    struct JSONES_API JArr : public JVal
    {
        std::vector<JVal*> arr;
        JVal* parentObj;
        /// @brief Constructor
        /// @param parent just keep it null. parentObj is only needed while parsing.
        JArr(JVal* parent);
        ~JArr() override;
        JArr(JArr&& ar) noexcept;

        template <typename T>
        JArr(std::initializer_list<T> list)
            : JVal(JType::ARR)
        {
            auto it = list.begin();
            while (it != list.end())
            {
                Add(*it);
                it++;
            }
        }

        void Add(JObj* obj);
        void Add(double value);
        void Add(int value);
        void Add(float value);
        void Add(const char* str, int b, int e);
        void Add(bool value);

        JVal& operator[](int index);

        void PushBack(JVal* val);
        void PushBack(const char* str, int b, int e);
    };

    //********************************************** ***** **********************************************
    //********************************************** JPair **********************************************
    //
    // /// @brief Quick way to create json object pair.
    // JSONES_API std::pair<std::string, JVal*> JPair(const std::string& key, const std::string& val);
    //
    // /// @brief Quick way to create json object pair.
    // JSONES_API std::pair<std::string, JVal*> JPair(const std::string& key, const char* val);
    //
    // /// @brief Quick way to create json object pair.
    // JSONES_API std::pair<std::string, JVal*> JPair(const std::string& key, double val);
    //
    // /// @brief Quick way to create json object pair.
    // JSONES_API std::pair<std::string, JVal*> JPair(const std::string& key, float val);
    //
    // /// @brief Quick way to create json object pair.
    // JSONES_API std::pair<std::string, JVal*> JPair(const std::string& key, bool val);
    //
    // /// @brief Quick way to create json object pair.
    // JSONES_API std::pair<std::string, JVal*> JPair(const std::string& key, int val);
    //
    // /// @brief Quick way to create json object pair.
    // JSONES_API std::pair<std::string, JVal*> JPair(const std::string& key, JObj* val);
    //
    // /// @brief Quick way to create json object pair.
    // JSONES_API std::pair<std::string, JVal*> JPair(const std::string& key, JObj&& val);
    //
    // /// @brief Quick way to create json object pair.
    // JSONES_API std::pair<std::string, JVal*> JPair(const std::string& key, JArr* arr);
    //
    // JSONES_API std::pair<std::string, JVal*> JPair(const std::string& key, JArr&& arr);
    //
    //
    // /// @brief Quick way to create json object pair.
    // JSONES_API std::pair<std::string, JVal*> JPair(const std::string& key, JArr& arr);
    //
    // /// @brief Quick way to create json object pair.
    // JSONES_API std::pair<std::string, JVal*> JPair(const std::string& key, JVal* jVal);

    //********************************************** ***** **********************************************


    /// @brief cout json's structure and it's members types.
    /// @param root 
    /// @param tab 
    JSONES_API void PrintJson(JObj* root, int tab = 0);

    /// @brief Parses json
    /// @return JObj
    JSONES_API JObj* JParse(const char*);

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
