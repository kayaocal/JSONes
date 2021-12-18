//###############################################################################################################
//***************************************************************************************************************
//                                 _  _____  ____  _   _           
//                                | |/ ____|/ __ \| \ | |          
//                                | | (___ | |  | |  \| | ___  ___ 
//                            _   | |\___ \| |  | | . ` |/ _ \/ __|
//                           | |__| |____) | |__| | |\  |  __/\__ \
//                            \____/|_____/ \____/|_| \_|\___||___/ by Enes Kaya ÖCAL
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
        
        void AddArr(std::string key, JArr* arr);
        void AddObj(std::string key, JObj* obj);
        void AddFloat(std::string key, float value);
        void AddInt(std::string key, int value);
        void AddString(std::string key, std::string value);
        void AddBool(std::string key, bool value);
        
        void AddValue(std::string s, std::string k);
        void AddValue(std::string s, JVal* val);
        
        JVal* operator[](std::string);
        JVal* operator[](const char*);
    };

    struct JArr : public JObj
    {
        std::vector<JVal*> arr;

        /// @brief Constructor
        /// @param parent just keep it null. parentObj is only needed while parsing.
        JArr(JObj* parent);
        ~JArr() override;

        void AddObj(JObj* obj);
        void AddFloat(float value);
        void AddInt(int value);
        void AddString(std::string value);
        void AddBool(bool value);
        
        void PushBack(JVal* val);
        void PushBack(std::string s);
    };

    /// @brief cout's json structure and it's members types.
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


