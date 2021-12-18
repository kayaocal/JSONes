# JSONes

* Just another small json library. it is just a parser and writer. No reflection or fancy specs.
* It is tested with examples at json.org.
* *null* type is not supported
* No dependency. Only standart library functions.
* Cpp11

## Use
* Just copy Jsones.h and Jsone.cpp to your project.

##Compile
* Cmake can be used to generate project files


## How to parse Json

```c++
using namespace Jsones;
JObj* parsedObj = JParse(s.c_str());
```

## How to write Json

```c++
using namespace Jsones;
JObj* obj = new JObj();
obj->AddBool("test", false);
obj->AddString("JSONes", "Not best Json parser in the universe");
std::string jsonOutput = JWrite(obj);
```
Result of jsonOutput will be;
```json
{"test":false, "JSONes":"Not best Json parser in the universe"}
```
