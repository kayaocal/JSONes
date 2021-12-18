//###############################################################################################################
//***************************************************************************************************************
//                                 _  _____  ____  _   _           
//                                | |/ ____|/ __ \| \ | |          
//                                | | (___ | |  | |  \| | ___  ___ 
//                            _   | |\___ \| |  | | . ` |/ _ \/ __|
//                           | |__| |____) | |__| | |\  |  __/\__ \
//                            \____/|_____/ \____/|_| \_|\___||___/ by Enes Kaya �CAL
//                                                                                                       
//***************************************************************************************************************
//###############################################################################################################
//  After spending long time to choose an json library for my game engine project, I decided to make new one.
//  JSONes - just another small json library. it is just a parser and writer. No reflection or fancy specs.
//  JSONes's name is coming from merge of json and enes words.
//  
  


#ifndef JSONES_H__
#define JSONES_H__

#include <string>
#include <map>
#include <vector>
#include <sstream>

namespace Jsones
{
    enum class JType
    {
        OBJ = 1,
        NUM = 2,
        STR = 3,
        BOOL = 4,
        ARR = 5
    };


    struct JVal
    {
        JType type;

        explicit JVal(JType t);

        virtual ~JVal();
    };

    struct JStr : public JVal
    {
      std::string str;

      JStr(std::string s);

      ~JStr() override;
    };

    struct JNumber : public JVal
    {
        std::string str;

        JNumber(std::string s);
        explicit JNumber(int s);
        explicit JNumber(float s);
        explicit JNumber(double s);
        ~JNumber() override;
        bool IsInteger();
        int AsInt();
        float AsFloat();
        float AsDouble();
    };

    struct JBool : public JVal
    {
        bool val;
        JBool(bool b);
        ~JBool() override;
    };
    struct JArr;

    /// @brief JObj is JsonObject. JObj contains map<string, JVal> objects and it's parent object.
    struct JObj : public  JVal
    {
        std::map<std::string, JVal*> objects;
        JObj* parentObj;

        /// @brief Constructor
        /// @param parent just keep it null. parentObj is only needed while parsing.
        JObj(JObj* parent = nullptr);
        ~JObj() override;
        
        JObj(std::initializer_list <std::pair<std::string, JVal*>>);

        void Add(std::pair<std::string, JVal*> add);
        void Add(std::pair<std::string, JArr*> add);
        void Add(std::pair<std::string, JObj*> add);

        void AddArr(std::string key, JArr* arr);
        void AddObj(std::string key, JObj* obj);
        void AddFloat(std::string key, float value);
        void AddInt(std::string key, int value);
        void AddString(std::string key, std::string value);
        void AddBool(std::string key, bool value);
        
        /// @brief Called from JParse.
        /// @param key
        /// @param val string will be detected and converted to JNumber, JBool, JString...
        void AddValue(std::string key, std::string val);
        void AddValue(std::string key, JVal* val);
      
        JVal* Get(const std::string& key);

        
    };
    std::pair<std::string, JVal*> JValue(const std::string& str, const std::string& val);
    std::pair<std::string, JVal*> JValue(const std::string& str, const char* val);
    std::pair<std::string, JVal*> JValue(const std::string& str, double val);
    std::pair<std::string, JVal*> JValue(const std::string& str, float val);
    std::pair<std::string, JVal*> JValue(const std::string& str, bool val);
    std::pair<std::string, JVal*> JValue(const std::string& str, int val);
    std::pair<std::string, JVal*> JValue(const std::string& str, JObj* val);
    std::pair<std::string, JVal*> JValue(const std::string& str, JArr* arr);

    struct JArr : public JObj
    {
        std::vector<JVal*> arr;

        /// @brief Constructor
        /// @param parent just keep it null. parentObj is only needed while parsing.
        JArr(JObj* parent);
        ~JArr() override;

        JArr(std::initializer_list<int>);
        JArr(std::initializer_list<float>);
        JArr(std::initializer_list<double>);
        JArr(std::initializer_list<bool>);
        JArr(std::initializer_list<std::string>);
        JArr(std::initializer_list<JObj* >);
        JArr(std::initializer_list<const char*> list);

        void AddObj(JObj* obj);
        void AddFloat(float value);
        void AddDouble(float value);
        void AddInt(int value);
        void AddString(std::string value);
        void AddBool(bool value);
        


        void PushBack(JVal* val);
        void PushBack(std::string s);
    };

    /// @brief cout json's structure and it's members types.
    /// @param root 
    /// @param tab 
    extern void PrintJson(JObj* root, int tab = 0);
    
    /// @brief Parses json
    /// @return JObj
    extern JObj* JParse(const char*);

    /// @brief Converts JObj to json string
    /// @param root 
    /// @param tab just give 0
    /// @param beautify if true
    /// @return 
    extern std::stringstream JWrite(JObj* root, float tab = 0, bool beautify = false);
}


#endif//JSONES_H__


