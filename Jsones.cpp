#include <iostream>
#include <fstream>
#include "Jsones.h"


#include "lookup3.h"



namespace Jsones
{
    //*********************************************** ***** **********************************************
    //******************************************** Tokenizing ********************************************

    enum class TokenType : char
    {
        CURLY_BRACKET_OPEN,
        CURLY_BRACKET_CLOSE,
        ANGLE_BRACKET_OPEN,
        ANGLE_BRACKET_CLOSE,
        COLON,
        COMMA,
        KEY
    };

    struct Token
    {
        TokenType type;

         Token(TokenType ty)
            : type(ty)
        {
        }
        
        std::string ToString() const;
    };

    struct StrToken : public Token
    {
        size_t beginIndex;
        size_t endIndex;

        StrToken(TokenType ty, size_t begin, size_t end)
            : Token(ty), beginIndex(begin), endIndex(end)
        {
        }
    };

    std::string Token::ToString() const
    {
        switch (type)
        {
        case TokenType::CURLY_BRACKET_OPEN: return std::string("CURLY_BRACKET_OPEN");
        case TokenType::CURLY_BRACKET_CLOSE: return std::string("CURLY_BRACKET_CLOSE");
        case TokenType::ANGLE_BRACKET_OPEN: return std::string("ANGLE_BRACKET_OPEN");
        case TokenType::ANGLE_BRACKET_CLOSE: return std::string("ANGLE_BRACKET_CLOSE");
        case TokenType::COLON: return std::string("COLON");
        case TokenType::COMMA: return std::string("COMMA");
        case TokenType::KEY: return std::string("KEY");
        default: return std::string("NONE");
        }
    }

    Token* TokenPtrComma = new Token(TokenType::COMMA);
    Token* TokenPtrColon = new Token(TokenType::COLON);
    Token* TokenPtrCurlyBracketOpen = new Token(TokenType::CURLY_BRACKET_OPEN);
    Token* TokenPtrCurlyBracketClose = new Token(TokenType::CURLY_BRACKET_CLOSE);
    Token* TokenPtrAngleBracketOpenToken = new Token(TokenType::ANGLE_BRACKET_OPEN);
    Token* TokenPtrAngleBracketClose = new Token(TokenType::ANGLE_BRACKET_CLOSE);

    void Tokenize(const char* js, std::vector<Token*>& tokens)
    {
        bool keyStarted = false;
        size_t len = strlen(js);
        bool dittoMarked = false;
        int indexS;
        for (int i = 0; i < len; i++)
        {
            if (keyStarted)
            {
                if (js[i] == '\"' || (js[i] == '\n' && js[i-1] != '\\') || (js[i] == '\t' && js[i-1] != '\\') || js[i] == '\r')
                {
                    tokens.push_back(new StrToken(TokenType::KEY, indexS, i));
                    keyStarted = false;
                    dittoMarked = false;
                }
                else if (js[i] == ',' && !dittoMarked)
                {
                    keyStarted = false;
                    tokens.push_back(new StrToken(TokenType::KEY, indexS, i));
                    tokens.push_back(TokenPtrComma);
                }
                else if (js[i] == '}' && !dittoMarked)
                {
                    keyStarted = false;
                    tokens.push_back(new StrToken(TokenType::KEY, indexS, i));
                    tokens.push_back(TokenPtrCurlyBracketClose);
                }
                else if (js[i] == ']' && !dittoMarked)
                {
                    keyStarted = false;
                    tokens.push_back(new StrToken(TokenType::KEY, indexS, i));
                    tokens.push_back(TokenPtrAngleBracketClose);
                }
             
                continue;
            }

            if (js[i] == ' ')
            {
                continue;
            }
            if (js[i] == '\n' || js[i] == '\t' || js[i] == '\r')
            {
                continue;
            }

            else if (js[i] == '{')
            {
                tokens.push_back(TokenPtrCurlyBracketOpen);
            }
            else if (js[i] == '}')
            {
                tokens.push_back(TokenPtrCurlyBracketClose);
            }
            else if (js[i] == '[')
            {
                tokens.push_back(TokenPtrAngleBracketOpenToken);
            }
            else if (js[i] == ']')
            {
                tokens.push_back(TokenPtrAngleBracketClose);
            }
            else if (js[i] == ',')
            {
                tokens.push_back(TokenPtrComma);
            }
            else if (js[i] == ':')
            {
                tokens.push_back(TokenPtrColon);
            }
            else if (js[i] == '\"')
            {
                dittoMarked = true;
                keyStarted = true;
                indexS = i+1;
            }
            else
            {
                keyStarted = true;
                indexS = i;
            }
        }
    }

    JArr* ParseJArray(JArr* arr, std::vector<Token*>& tokens, int& index, const char* str);
    JObj* ParseJObj(JObj* o, std::vector<Token*>& tokens, int& index, const char* str);
    //********************************************* Tokenizing *******************************************
    //*********************************************** ***** **********************************************
    //********************************************** Helpers *********************************************
    
    std::string SubstrFromCharArray(const char* str, size_t b, size_t e)
    {
        std::string s= "";
        s.append(&str[b], e- b);
        return s;
    }

    bool CStrSubCompare(const char* str, size_t beg, size_t end, const char* comp)
    {
        if(strlen(comp) != end - beg)
        {
            return false;
        }

        int index = 0;
        for(size_t i = beg; i < end; i++)
        {
            if(str[i] != comp[index])
            {
                return false;
            }
            index++;
        }
        return true;
    }
    
    const char* GetJsonTypeCStr(JType t)
    {
        switch (t)
        {
            case JType::ARR:
                return "[ARR]";
            case JType::OBJ:
                return "[OBJ]";
            case JType::NUL:
                return "[NULL]";
            case JType::NUM:
                return "[NUM]";
            case JType::STR:
                return "[STR]";
            case JType::BOOL:
                return "[BOOL]";
            default: return "[NONE]";
        }
    }

    JVal* GetValueTypeBySubCStr(const char* str, size_t beg, size_t end)
    {
        bool isNumber = true;
        if (beg == end)
        {
            isNumber = false;
        }
        for (size_t i = beg; i < end; i++)
        {
            if (str[i] != '.' && (str[i] < '0' || str[i] > '9'))
            {
                isNumber = false;
            }
        }

        if (isNumber)
        {
            return new JNumber(str, beg, end);
        }
        else if(CStrSubCompare(str,beg,end, "null"))
        {
            return new JNull();
        }
       else if(CStrSubCompare(str,beg,end, "false"))
        {
            return new JBool(false);
        }
        else if(CStrSubCompare(str,beg,end, "true"))
        {
            return new JBool(true);
        }
        else
        {
            return new JStr(str, beg, end);
        }
    }

    uint32_t GetHash(const char* str)
    {
        return Oyun::hashlittle(str, strlen(str), 0);
    }

    uint32_t GetHash(const std::string& str)
    {
        return Oyun::hashlittle(str.c_str(), str.length(), 0);
    }
    
    //********************************************** Helpers *********************************************
    //*********************************************** ***** **********************************************
    //******************************************** KeyHashing ********************************************
    
    std::map<uint32_t, std::string> KeyHashes;

    std::string GetHashedString(uint32_t key)
    {
        auto it  = KeyHashes.find(key);
        if(it!=KeyHashes.end())
        {
            return it->second;
        }

        return "";
    }

     uint32_t GetKeyHash(const std::string str)
    {
        uint32_t hash = GetHash(str);
        auto it = KeyHashes.begin();
        auto found = KeyHashes.find(hash);
        if(found == KeyHashes.end())
        {
            KeyHashes.insert(std::pair<uint32_t, std::string>(hash, str));
        }
        return hash;
    }

     uint32_t GetKeyHash(const char* str, size_t b, size_t e)
    {
        uint32_t hash =  Oyun::hashlittle(&str[b], e-b, 0);
        auto it = KeyHashes.begin();
        auto found = KeyHashes.find(hash);
        if(found == KeyHashes.end())
        {
            KeyHashes.insert(std::pair<uint32_t, std::string>(hash, SubstrFromCharArray(str, b, e)));
        }
        return hash;
    }

    //******************************************** KeyHashing ********************************************
    //*********************************************** ***** **********************************************
    //********************************************* Test Funcs *******************************************
    
    void PrintArray(JArr* arr, int tab);
    void PrintObj(JObj* obj, int tab)
    {
        for (auto m : obj->objects)
        {
            int i = 0;
            std::cout << std::endl;
            while (i < tab)
            {
                std::cout << "\t";
                i++;
            }
            if (m.second->type == JType::ARR)
            {
                std::cout << "[ARR][" << GetHashedString(m.first) << "] : ";

                PrintArray(dynamic_cast<JArr*>(m.second), tab);
                
            }
            else if(m.second->type == JType::OBJ)
            {
                std::cout << "[OBJ][" << GetHashedString(m.first) << "] : ";
                PrintObj(dynamic_cast<JObj*>(m.second), tab + 1);
            }
            else 
            {
                std::cout << GetJsonTypeCStr(m.second->type)<<"[" << GetHashedString(m.first) << "] : " << m.second->ToString();
            }
        }
    }

    void PrintArray(JArr* arr, int tab)
    {
        if (arr == nullptr)
        {
            return;
        }

         for (auto it :arr->array)
        {
            
            if (it->type == JType::OBJ)
            {
                std::cout << "{" << std::endl;
                PrintObj(dynamic_cast<JObj*>(it), tab + 1);
                std::cout << std::endl << "}";
            }
            else if (it->type == JType::STR || it->type == JType::NUM || it->type == JType::BOOL || it->type == JType::NUL)
            {
                std::cout << it->ToString() << ", ";
            }
        }
        
    }
    
    //********************************************* Test Funcs *******************************************
    //*********************************************** ***** **********************************************
    //*********************************************** JVal ***********************************************

    JVal::JVal(JType t)
        : type(t)
    {
    }


    JVal::~JVal()
    {
    }

    void JVal::operator=(const int& v)
    {
        jassert(type == JType::NUM, "integer type can only be assigned to JNum types");
        JNumber* num = dynamic_cast<JNumber*>(this);
        if (num != nullptr)
        {
            // num->str = std::to_string(v);
        }
    }

    void JVal::operator=(const float& v)
    {
        jassert(type == JType::NUM, "float type can only be assigned to JNum types");
        JNumber* num = dynamic_cast<JNumber*>(this);
        if (num != nullptr)
        {
            // num->str = std::to_string(v);
        }
    }

    void JVal::operator=(const double& v)
    {
        jassert(type == JType::NUM, "double type can only be assigned to JNum types");
        JNumber* num = dynamic_cast<JNumber*>(this);
        if (num != nullptr)
        {
            // num->str = std::to_string(v);
        }
    }

    void JVal::operator=(const bool v)
    {
        jassert(type == JType::BOOL, "bool type can only be assigned to JBool types");
        JBool* num = dynamic_cast<JBool*>(this);
        if (num != nullptr)
        {
            num->val = v;
        }
    }

    void JVal::operator=(const std::string& str)
    {
        jassert(type == JType::STR, "std::string type can only be assigned to JStr types");
        JStr* num = dynamic_cast<JStr*>(this);
        if (num != nullptr)
        {
            // num->str = str;
        }
    }

    void JVal::operator=(const char* str)
    {
        jassert(type == JType::STR, "const char* type can only be assigned to JStr types");
        JStr* num = dynamic_cast<JStr*>(this);
        if (num != nullptr)
        {
            num->str = str;
        }
    }
    std::string valueStr{"!value!"};

    void JVal::operator=(const JObj& o)
    {
        jassert(type == JType::OBJ, "const char* type can only be assigned to JObj types");
        JObj* obj = dynamic_cast<JObj*>(this);
        *obj = o;
    }

    void JVal::operator=(JObj&& o)
    {
        jassert(type == JType::OBJ, "const char* type can only be assigned to JObj types");
        JObj* obj = dynamic_cast<JObj*>(this);
        *obj = std::move(o);
    }

    std::string& JVal::ToString()
    {
        return valueStr;
    }
    //*********************************************** JVal ***********************************************
    //*********************************************** ***** **********************************************
    //*********************************************** JNull **********************************************

    JNull::JNull()
        :JVal(JType::NUL)
    {
    }

    JNull::~JNull()
    {
    }

    
    std::string nullStr{"null"};
    std::string& JNull::ToString()
    {
        return nullStr;
    }
    
    //*********************************************** JNull **********************************************
    //*********************************************** ***** **********************************************
    //*********************************************** JStr ***********************************************


    JStr::JStr(const char* s, size_t b, size_t e) noexcept
        :JVal(JType::STR)
    {
        str = "";
        str.append(&s[b], e-b);
    }

    JStr::JStr(const std::string& str) noexcept
        :JVal(JType::STR)
    {
        this->str = str;
    }

    JStr::JStr(const std::string&& s) noexcept
        :JVal(JType::STR), str(std::move(s))
    {
        
    }

    JStr::JStr(const char* ch) noexcept
    :JVal(JType::STR), str(ch)
    {
    }

    JStr::JStr(const JStr& s) noexcept
    :JVal(JType::STR), str(s.str)
    {
        
    }

    JStr::~JStr()
    {
    }

    
    std::string& JStr::ToString()
    {
       return str;
    }
    //*********************************************** JStr **********************************************
    //********************************************** ***** **********************************************
    //********************************************* JNumber *********************************************

    JNumber::JNumber(const char* s, size_t b, size_t e)
        :JVal(JType::NUM)
    {
        str = "";
        str.append(&s[b], e-b);
    }

     JNumber::JNumber(int v)
         : JVal(JType::NUM), str(std::to_string(v))
     {
     }
    
    JNumber::JNumber(float v)
    : JVal(JType::NUM), str(std::to_string(v))
    {
    }
    
    JNumber::JNumber(double d)
    : JVal(JType::NUM), str(std::to_string(d))
    {
    }

    JNumber::JNumber(const JNumber& n) noexcept
    :JVal(JType::NUM), str(n.str)
    {
    }

    JNumber::~JNumber()
    {
    }

    bool JNumber::IsInteger()
    {
        size_t len = str.length();
        for (int i = 0; i < len; i++)
        {
            if (str[i] == '.')
            {
                return false;
            }
        }

        return true;
    }

    int JNumber::AsInt() const
    {
        return std::stoi(str);
    }

    float JNumber::AsFloat() const
    {
        return std::stof(str);
    }

    double JNumber::AsDouble() const
    {
        return std::stod(str);
    }

    std::string& JNumber::ToString()
    {
        return str;
    }

    //********************************************* JNumber *********************************************
    //********************************************** ***** **********************************************
    //********************************************** JBool **********************************************

    JBool::JBool(bool b)
        : JVal(JType::BOOL), val(b)
    {
    }

    JBool::JBool(const JBool& b) noexcept
    :JVal(JType::BOOL), val(b.val)
    {
        
    }

    JBool::~JBool()
    {
    }

    std::string trueStr{"true"};
    std::string falseStr{"false"};
    std::string& JBool::ToString()
    {
        return (val ? trueStr : falseStr);
    }

    //*********************************************** JBool **********************************************
    //*********************************************** ***** **********************************************
    //*********************************************** JObj ***********************************************

    JObj::JObj()
        : JVal(JType::OBJ)
    {
    }

    JObj::~JObj()
    {
        for (auto m : objects)
        {
            delete m.second;
        }
    }

    JObj::JObj(JObj&& objR) noexcept
        : JVal(JType::OBJ), objects(std::move(objR.objects))
    {
        objR.objects.clear();
    }

    JObj::JObj(const JObj& o) noexcept
    :JVal(JType::OBJ)
    {
        for(auto it : o.objects)
        {
            JVal* ptr = nullptr;
            
            if(it.second->type == JType::NUL)
            {
                ptr = new JNull();
            }
            else if(it.second->type == JType::OBJ)
            {
                JObj* obj = static_cast<JObj*>(it.second);
                ptr = new JObj(*obj);
            }
            else if(it.second->type == JType::NUM)
            {
                JNumber* num = static_cast<JNumber*>(it.second);
                ptr = new JNumber(*num);
            }
            else if(it.second->type == JType::STR)
            {
                JStr* s = static_cast<JStr*>(it.second);
                ptr = new JStr(*s);
            }
             else if(it.second->type == JType::BOOL)
            {
                JBool* b = static_cast<JBool*>(it.second);
                ptr = new JBool(*b);
            }
             else if(it.second->type == JType::ARR)
            {
                 JArr* a = static_cast<JArr*>(it.second);
                ptr = new JArr(*a);
            }
            else
            {
                 jassert(false, "Undefined Copy!");
            }
          
            objects.insert(std::pair<uint32_t, JVal*>(it.first, ptr));
        }
    }

    JObj::JObj(const char* str)
        :JVal(JType::OBJ)
    {
        std::vector<Token*> tokens;
        Tokenize(str, tokens);
        int index = 0;
        ParseJObj(this, tokens, index, str);

        for (auto token : tokens)
        {
            if(token->type == TokenType::KEY)
            {
                delete token;
            }
        }
    }


    JObj::JObj(std::initializer_list<std::pair<uint32_t, JVal*>> list)
        : JVal(JType::OBJ)
    {
        auto it = list.begin();
        while (it != list.end())
        {
            Add(*it);
            it++;
        }
    }

    void JObj::Add(std::pair<uint32_t, JVal*> add)
    {
        objects.insert(add);
    }

    void JObj::Add(std::pair<uint32_t, JArr*> add)
    {
        objects.insert(add);
    }

    void JObj::Add(std::pair<uint32_t, JObj*> add)
    {
        objects.insert(add);
    }

    JVal* JObj::Get(uint32_t hash)
    {
        auto found = objects.find(hash);
        if (found != objects.end())
        {
            std::cout << "Succesfully Found!" << std::endl;
            return found->second;
        }


        return nullptr;
    }

    JObj* JObj::GetObj(const std::string& key)
    {
        JVal* val = Get(Oyun::hashlittle(key.c_str(), key.length(), 0));
        assert(val != nullptr, "Given key is exist");
        assert(val->type == JType::OBJ, "Given key is not an JObj*");

        return dynamic_cast<JObj*>(val);
    }

    JArr* JObj::GetArr(const std::string& key)
    {
        JVal* val = Get(Oyun::hashlittle(key.c_str(), key.length(), 0));
        assert(val != nullptr, "Given key is exist");
        assert(val->type == JType::ARR, "Given key is not an JArr*");

        return dynamic_cast<JArr*>(val);
    }

    void JObj::RemoveKeyValue(const std::string& key)
    {
        auto it = objects.find(GetHash(key));
        if(it!=objects.end())
        {
            delete it->second;
            objects.erase(it);
        }
    }

    JVal& JObj::operator[](const std::string& str)
    {
        JVal* val = Get(Oyun::hashlittle(str.c_str(), str.length(), 0));
        jassert(val!=nullptr, "given key is not exist");
        return *val;
    }

    void JObj::operator=(const JObj& o)
    {
        for(auto it : objects)
        {
            delete it.second;
        }

        objects.clear();
        
        for(auto it : o.objects)
        {
            JVal* ptr = nullptr;
            
            if(it.second->type == JType::NUL)
            {
                ptr = new JNull();
            }
            else if(it.second->type == JType::OBJ)
            {
                JObj* obj = static_cast<JObj*>(it.second);
                ptr = new JObj(*obj);
            }
            else if(it.second->type == JType::NUM)
            {
                JNumber* num = static_cast<JNumber*>(it.second);
                ptr = new JNumber(*num);
            }
            else if(it.second->type == JType::STR)
            {
                JStr* s = static_cast<JStr*>(it.second);
                ptr = new JStr(*s);
            }
             else if(it.second->type == JType::BOOL)
            {
                JBool* b = static_cast<JBool*>(it.second);
                ptr = new JBool(*b);
            }
             else if(it.second->type == JType::ARR)
            {
                 JArr* a = static_cast<JArr*>(it.second);
                ptr = new JArr(*a);
            }
            else
            {
                 jassert(false, "Undefined Copy!");
            }
          
            objects.insert(std::pair<uint32_t, JVal*>(it.first, ptr));
        }
    }

    void JObj::operator=(JObj&& o)
    {
        for(auto it : objects)
        {
            delete it.second;
        }

        objects.clear();
        objects = std::move(o.objects);
    }

    //*********************************************** JObj ***********************************************
    //*********************************************** ***** **********************************************
    //*********************************************** JArr ***********************************************


    void JArr::PushBack(JVal* val)
    {
        array.push_back(val);
    }

    void JArr::PushBack(const char* str, int b, int e)
    {
        array.push_back(new JStr(str, b, e));
    }

    JArr::JArr()
        : JVal(JType::ARR)
    {
    }

    JArr::~JArr()
    {
        for (auto m : array)
        {
            delete m;
        }
    }

    JArr::JArr(JArr&& ar): JVal(JType::ARR),  array(std::move(ar.array)) 
    {
    }

    JArr::JArr(const JArr& ar)
        :JVal(JType::ARR)
    {
        for(auto it : ar.array)
        {
            JVal* ptr = nullptr;
            
            if(it->type == JType::NUL)
            {
                ptr = new JNull();
            }
            else if(it->type == JType::OBJ)
            {
                JObj* obj = static_cast<JObj*>(it);
                ptr = new JObj(*obj);
            }
            else if(it->type == JType::NUM)
            {
                JNumber* num = static_cast<JNumber*>(it);
                ptr = new JNumber(*num);
            }
            else if(it->type == JType::STR)
            {
                JStr* s = static_cast<JStr*>(it);
                ptr = new JStr(*s);
            }
             else if(it->type == JType::BOOL)
            {
                JBool* b = static_cast<JBool*>(it);
                ptr = new JBool(*b);
            }
             else if(it->type == JType::ARR)
            {
                 JArr* a = static_cast<JArr*>(it);
                ptr = new JArr(*a);
            }
            else
            {
                 jassert(false, "Undefined Copy!");
            }

            array.push_back(ptr);
        }
    }

    JArr::JArr(const char* str)
        :JVal(JType::ARR)
    {
        std::vector<Token*> tokens;
        Tokenize(str, tokens);
        int index = 0;
        ParseJArray(this, tokens, index, str);
        //PrintArray(arr, 0);

        for (auto token : tokens)
        {
            if(token->type == TokenType::KEY)
            {
                delete token;
            }
        }
    }


    JArr& JArr::Add(const JObj& o)
    {
        JObj* obj = new JObj(o);
        array.push_back(obj);
        return *this;
    }

    JArr& JArr::Add(JObj&& o)
    {
        JObj* obj = new JObj(std::move(o));
        array.push_back(obj);
        return *this;
    }

    void JArr::Add(JObj* obj)
    {
        array.push_back(obj);
    }

    void JArr::Add(double value)
    {
        array.push_back(new JNumber(value));
    }

    void JArr::Add(int value)
    {
        array.push_back(new JNumber(value));
    }
    
    void JArr::Add(float value)
    {
        array.push_back(new JNumber(value));
    }

    void JArr::Add(const char* str, int b, int e)
    {
        array.push_back(new JStr(str, b, e));
    }

    void JArr::Add(bool value)
    {
        array.push_back(new JBool(value));
    }

    void JArr::Add(const char* str)
    {
        
        array.push_back(new JStr(std::move(str)));
    }

    void JArr::Add(const std::string& str)
    {
        array.push_back(new JStr(str));
    }

    void JArr::Add(const std::string&& str)
    {
        array.push_back(new JStr(str));
    }

    void JArr::RemoveAt(int index)
    {
        if(index < 0 || index > array.size())
        {
            return;
        }
        
        delete array[index];
        array.erase(array.begin() + index);
    }

    JVal& JArr::operator[](int index)
    {
        jassert(index < array.size(), "JArr index exceeds vector size!");
        return *array[index];
    }

    void JArr::operator=(const JArr& ar)
    {
        for(auto it : array)
        {
            delete it;
        }

        array.clear();
        for(auto it : ar.array)
        {
            JVal* ptr = nullptr;
            
            if(it->type == JType::NUL)
            {
                ptr = new JNull();
            }
            else if(it->type == JType::OBJ)
            {
                JObj* obj = static_cast<JObj*>(it);
                ptr = new JObj(*obj);
            }
            else if(it->type == JType::NUM)
            {
                JNumber* num = static_cast<JNumber*>(it);
                ptr = new JNumber(*num);
            }
            else if(it->type == JType::STR)
            {
                JStr* s = static_cast<JStr*>(it);
                ptr = new JStr(*s);
            }
             else if(it->type == JType::BOOL)
            {
                JBool* b = static_cast<JBool*>(it);
                ptr = new JBool(*b);
            }
             else if(it->type == JType::ARR)
            {
                 JArr* a = static_cast<JArr*>(it);
                ptr = new JArr(*a);
            }
            else
            {
                 jassert(false, "Undefined Copy!");
            }

            array.push_back(ptr);
        }
    }

    void JArr::operator=(JArr&& ar)
    {
        for(auto it : array)
        {
            delete it;
        }

        array.clear();
        array = std::move(ar.array);
    }

    //*********************************************** JArr ***********************************************
    //*********************************************** ***** **********************************************
    //********************************************** Parsing *********************************************

    JObj* ParseJObj(JObj* obj, std::vector<Token*>& tokens, int& index, const char* str);
    
    JArr* ParseJArray(JArr* arr, std::vector<Token*>& tokens, int& index, const char* str)
    {
        
        JArr* array = arr;
        if(arr == nullptr)
        {
            array = new JArr();
        }
        
        bool valueSetted = false;

        size_t valueBeg = 0;
        size_t valueEnd = 0;
        for(int i = index; i < tokens.size(); i++)
        {
            if(tokens[i]->type == TokenType::ANGLE_BRACKET_CLOSE)
            {
                if(valueSetted)
                {
                    array->PushBack(GetValueTypeBySubCStr(str, valueBeg, valueEnd));
                    valueSetted = false;
                }
                index = i;
                return array;
            }

            if(tokens[i]->type == TokenType::CURLY_BRACKET_OPEN)
            {
                array->PushBack(ParseJObj(nullptr, tokens, i, str));
            }
            else if(tokens[i]->type == TokenType::COMMA)
            {
                 if(valueSetted)
                 {
                    array->PushBack(GetValueTypeBySubCStr(str, valueBeg, valueEnd));
                    valueSetted = false;
                 }
            }
            else if(tokens[i]->type == TokenType::KEY)
            {
                StrToken* tkn = static_cast<StrToken*>(tokens[i]);
                valueBeg = tkn->beginIndex;
                valueEnd = tkn->endIndex;
                valueSetted = true;
            }
        }
        
        index;
        return array;
    }

   
    JObj* ParseJObj(JObj* o, std::vector<Token*>& tokens, int& index, const char* str)
    {
        JObj* obj = o;
        if(obj==nullptr)
        {
            obj = new JObj();
        }

        size_t keyStart = 0;
        size_t keyEnd = 0;
        size_t valueStart = 0;
        size_t valueEnd = 0;
        bool setKey = true;
        
        for(int i = index; i < tokens.size(); i++)
        {
            if(tokens[i]->type == TokenType::KEY)
            {
                StrToken* tkn = static_cast<StrToken*>(tokens[i]);
                if(setKey)
                {
                    keyStart = tkn->beginIndex;
                    keyEnd = tkn->endIndex;
                }
                else
                {
                    valueStart = tkn->beginIndex;
                    valueEnd = tkn->endIndex;
                }
            }
            else if(tokens[i]->type == TokenType::COLON)
            {
                setKey = !setKey;
            }
            else if(tokens[i]->type == TokenType::COMMA)
            {
                obj->Add(std::pair<uint32_t, JVal*>(GetKeyHash(str, keyStart, keyEnd), GetValueTypeBySubCStr(str, valueStart, valueEnd)));
                setKey = true;
            }
            else if(tokens[i]->type == TokenType::CURLY_BRACKET_OPEN)
            {
                if(!setKey)
                {
                    obj->Add(std::pair<uint32_t, JVal*>(GetKeyHash(str, keyStart, keyEnd), ParseJObj(nullptr, tokens, i, str)));
                }
            }
            else if(tokens[i]->type == TokenType::CURLY_BRACKET_CLOSE)
            {
                if(!setKey)
                {
                    obj->Add(std::pair<uint32_t, JVal*>(GetKeyHash(str, keyStart, keyEnd), GetValueTypeBySubCStr(str, valueStart, valueEnd)));
                }
                index = i;
                return obj;
            }
            else if(tokens[i]->type == TokenType::ANGLE_BRACKET_OPEN)
            {
                if(!setKey)
                {
                    obj->Add(std::pair<uint32_t, JVal*>(GetKeyHash(str, keyStart, keyEnd), ParseJArray(nullptr, tokens, i, str)));
                }
            }
            
        }
        
        index;
        return obj;
    }

    //********************************************** Parsing *********************************************
    //*********************************************** ***** **********************************************
    //********************************************** Writing *********************************************

    std::string beautifiedTab("    ");
    std::string beautifiedNewLine("\n");
    std::string beautifiedSpace(" ");
    std::string emptyString("");

    std::string& NewLine(bool beautify)
    {
        if (beautify)
        {
            return beautifiedNewLine;
        }
        return emptyString;
    }

    std::string& Tab(bool beautify)
    {
        if (beautify)
        {
            return beautifiedTab;
        }
        return emptyString;
    }

    std::string& Space(bool beautify)
    {
        if (beautify)
        {
            return beautifiedSpace;
        }
        return emptyString;
    }

    std::string Tab(bool beautify, int count)
    {
        if (beautify)
        {
            std::string str;
            for (int i = 0; i < count; i++)
            {
                str += beautifiedTab;
            }
            return str;
        }
        return emptyString;
    }

    std::stringstream JWrite(JObj* obj, bool beautify, int tab);
    std::stringstream JWrite(JArr* arr, bool beautify, int tab)
    {
         std::stringstream ss;
        ss << NewLine(beautify)<< Tab(beautify, tab);
        ss << "[" << NewLine(beautify);

        bool subFirst = true;
        for (auto it : arr->array)
        {
            if (!subFirst)
            {
                ss << "," << NewLine(beautify);
            }
            subFirst = false;
            if (it->type == JType::OBJ)
            {
                ss << JWrite(dynamic_cast<JObj*>(it), beautify, tab + 1).rdbuf();
            }
            else if(it->type == JType::ARR)
            {
                ss<<JWrite(dynamic_cast<JArr*>(it), true, tab+1).rdbuf();
            }
            else
            {
                if(it->type == JType::STR)
                {
                    ss<<"\""<<it->ToString()<<"\"";
                }
                else
                {
                    ss<<it->ToString();
                }
            }
                      
        }
        ss << NewLine(beautify) << Tab(beautify, tab) << "]";
        return ss;
    }
    
    std::stringstream JWrite(JObj* obj, bool beautify, int tab)
    {
         std::stringstream ss;
        ss <<  Tab(beautify, tab);
        ss << "{" << NewLine(beautify);

        bool subFirst = true;
        for (auto it : obj->objects)
        {
            if (!subFirst)
            {
                ss << "," << NewLine(beautify);
            }
            ss<<Tab(beautify, tab+1);

            subFirst = false;
            ss<<"\""<< GetHashedString(it.first) <<"\"" << Space(beautify) << ":" << Space(beautify);

            if (it.second->type == JType::OBJ)
            {
                ss << JWrite(dynamic_cast<JObj*>(it.second), beautify, tab + 1).rdbuf();
            }
            else if(it.second->type == JType::ARR)
            {
                ss<<JWrite(dynamic_cast<JArr*>(it.second), true, tab+1).rdbuf();
            }
            else
            {
                if(it.second->type == JType::STR)
                {
                    ss<<"\""<<it.second->ToString()<<"\"";
                }
                else
                {
                    ss<<it.second->ToString();
                }
            }
                      
        }
        ss << NewLine(beautify) << Tab(beautify, tab) << "}";
        return ss;
    }
    
    //********************************************** Writing *********************************************
    //*********************************************** ***** **********************************************
}