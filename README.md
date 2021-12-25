# JSONes

* Just another small json parser and writer. It has no reflection or fancy specs.
* It is tested with examples at json.org
* Only standart library. No any other 3rd party library required.
* Written in cpp 11.

## Use
* Just copy Jsones.h and Jsones.cpp and lookup3.h to your project.
* **lookup3.h** is written by **Bob Jenkins**

## Compile
* Cmake can be used to generate project files

## How to parse Json

```c++
using namespace Jsones;

const char* jsonArray = "[...]";
JArr arr (jsonArray);

const char* jsonObject = "{...}";
JObj obj(jsonObject);

```

## How to create Json

```c++
using namespace Jsones;

JObj obj{
    JPair("StrKey", "StrValue"),
    JPair("NumberKey", 333),
    JPair("BoolKey", true),
    JPair("IntArrayKey", JArr{1,2,3,4,5}),
    JPair("StrArrayKey", JArr{"str1","str2", "str3"}),
    JPair("SubObject", JObj{JPair("SubObjStrKey", "SubObjStrValue"), JPair("SubObjInt", 956)})
    };
    
JArr arr{"str1","str2","str3"}

```
## How to edit Json
```c++
using namespace Jsones;

JObj obj{
    JPair("StrKey", "StrValue"),
    JPair("NumberKey", 333)
    };

//Sets value of "NumberKey"    
obj["NumberKey"] = 123;

//Removes and deletes object
obj.RemoveKeyValue("StrKey");

JArr arr{"str1","str2","str3"}

arr[0] = "newStr1";

arr.RemoveAt(1);

arr.Add("str4");

```
## How to write Json

```c++
using namespace Jsones;
//Arrays =>
const char* jsonArray = "[...]";
JArr arr (jsonArray);

//true for beautify
JWrite(&arr, true);

//Objects =>
const char* jsonObject = "{...}";
JObj obj(jsonObject);

//true for beautify
JWrite(&obj, true);
```
## Structure
```c++
enum class JSONES_API JType : char
    {
        NUL = 0,
        OBJ = 1,
        NUM = 2,
        STR = 3,
        BOOL = 4,
        ARR = 5
    };
    
    struct JSONES_API JVal { JType type; ... }
    
    struct JSONES_API JNull : public JVal { ... }
    
    struct JSONES_API JStr : public JVal { std::string str; ... }
    
    struct JSONES_API JNumber : public JVal {  std::string str; ... }
    
    struct JSONES_API JBool : public JVal { bool val; ... }
       
    struct JSONES_API JObj : public JVal { std::map<uint32_t, JVal*> objects; ... }
    
    struct JSONES_API JArr : public JVal { std::vector<JVal*> array; ... }

```