#include "Jsones.h"
#include <fstream>
#include <iostream>

void ParseJsonGlossary()
{
    using namespace Jsones;
    std::basic_ifstream<char> stream;
    stream.open("..\\jsonExampleGlossary.json", std::ifstream::in | std::ifstream::binary);
    if (stream.is_open())
    {
        std::string text, line;
        while (std::getline(stream, line))
        {
            text += line + "\n";
        }
        const char* data = text.c_str();

        JObj obj(data);
        PrintObj(&obj);
    }
}

void ParseJsonWidget()
{
    using namespace Jsones;
    std::basic_ifstream<char> stream;
    stream.open("..\\jsonExampleWidget.json", std::ifstream::in | std::ifstream::binary);
    if (stream.is_open())
    {
        std::string text, line;
        while (std::getline(stream, line))
        {
            text += line + "\n";
        }
        const char* data = text.c_str();

        JObj obj(data);
        PrintObj(&obj);
    }
}

void ParseJsonWebApp()
{
    using namespace Jsones;
    std::basic_ifstream<char> stream;
    stream.open("..\\jsonExampleWebApp.json", std::ifstream::in | std::ifstream::binary);
    if (stream.is_open())
    {
        std::string text, line;
        while (std::getline(stream, line))
        {
            text += line + "\n";
        }
        const char* data = text.c_str();

        JObj obj(data);
        PrintObj(&obj);
    }
}

void ParseJsonMenu()
{
    using namespace Jsones;
    std::basic_ifstream<char> stream;
    stream.open("..\\jsonExampleMenu.json", std::ifstream::in | std::ifstream::binary);
    if (stream.is_open())
    {
        std::string text, line;
        while (std::getline(stream, line))
        {
            text += line + "\n";
        }
        const char* data = text.c_str();

        JObj obj(data);
        PrintObj(&obj);
    }
}

void CreateAndWriteJsonObj()
{
    using namespace Jsones;
    JObj obj{
        JPair("name" , "Jason"),
        JPair("birthyear" , 1999),
        JPair("longName" , "Javascript Object Notation"),
        JPair("humanReadable" , true),
        JPair("isXml" , false)
    };

    std::ofstream  stream;
    stream.open("..\\CreateAndWriteJsonObj.json", std::ifstream::out | std::ifstream::binary);
    stream<<JWrite(&obj).rdbuf();
    stream.close();
}

void CreateAndWriteJsonArray()
{
    using namespace Jsones;
    JArr arr{
        JObj
        {
            JPair("name" , "Jason"),
            JPair("birthyear" , 1999),
            JPair("longName" , "Javascript Object Notation"),
            JPair("humanReadable" , true),
            JPair("isXml" , false)
        },
         JObj
        {
            JPair("name" , "xml"),
            JPair("birthyear" , 1996),
            JPair("longName" , "Extensible Markup Language"),
            JPair("humanReadable" , true),
            JPair("isXml" , true)
        }
        
    };

    std::ofstream  stream;
    stream.open("..\\CreateAndWriteJsonArray.json", std::ifstream::out | std::ifstream::binary);
    stream << JWrite(&arr, true).rdbuf();
    stream.close();
}


int main()
{
    //This json is from json.org/example.html
    ParseJsonGlossary();
    //This json is from json.org/example.html
    ParseJsonWidget();
    //This json is from json.org/example.html
    ParseJsonWebApp();
    //This json is from json.org/example.html
    ParseJsonMenu();

    CreateAndWriteJsonObj();
    
    CreateAndWriteJsonArray();
}