﻿#include "Jsones.h"
#include <iostream>

void CreateJsonTest()
{
    using namespace Jsones;
    JObj* obj = new JObj(nullptr);
    obj->AddBool("boolTest", false);
    obj->AddInt("integerTest", 5);
    obj->AddFloat("floatTest", 3.5f);

    JObj* childObj = new JObj(obj);
    childObj->AddString("child", "childStr");
    childObj->AddBool("childbool", false);
    obj->AddObj("childObj", childObj);

    JArr* intArr = new JArr(obj);
    intArr->AddInt(5);
    intArr->AddInt(2);
    intArr->AddFloat(4.3f);
    intArr->AddInt(44);
    obj->AddArr("intArr", intArr);

    JArr* boolArr = new JArr(obj);
    boolArr->AddBool(false);
    boolArr->AddBool(true);
    boolArr->AddBool(false);
    obj->AddArr("boolArr", boolArr);

    JArr* strArr = new JArr(obj);
    strArr->AddString("str1");
    strArr->AddString("str2");
    strArr->AddString("str3");
    obj->AddArr("strArr", strArr);
    
    JArr* objArr = new JArr(obj);
    JObj* child1 = new JObj(objArr);
    child1->AddInt("child1/int", 3);
    child1->AddFloat("child1/float", 3.2f);
    child1->AddBool("child1/bool", false);
    JObj* child2 = new JObj(objArr);
    child2->AddInt("child2/int", 3);
    child2->AddFloat("child2/float", 3.2f);
    child2->AddBool("child2/bool", false);
    objArr->AddObj(child1);
    objArr->AddObj(child2);
    obj->AddArr("objArr", objArr);

    std::cout<<std::endl << "JSON : " << JWrite(obj, 0, true).rdbuf();
    std::string s = JWrite(obj, 0, true).str();

     std::cout<<std::endl<<"----------------"<<std::endl;
    JObj* parsedObj = JParse(s.c_str());
    PrintJson(parsedObj);
    delete obj;

}



int main()
{
    using namespace std;
     CreateJsonTest();
    //const char* js = "  {\"glossary\":{ \"bool\": true   ,\"int\":101,\"float\":100.5,\"title\": \"Bu bir test\",\"GlossDiv\":{\"title\":\"S\",\"GlossList\":{\"GlossEntry\":{\"ID\":\"SGML\",\"SortAs\":\"SGML\",\"GlossTerm\":\"Standard Generalized Markup Language\",\"Acronym\":\"SGML\",\"Abbrev\":\"ISO 8879:1986\",\"GlossDef\":{\"para\":\"A meta-markup language, used to create markup languages such as DocBook.\",\"GlossSeeAlso\": [\"GML\", \"XML\"]},\"GlossSee\":\"markup\"}}}, \"test\":\"Bu bir testtir\"}, \"Iki\":\" uc\"""}";
    //const char* js = "[{\"test\":15,\"bool\":false,\"str\":\"strtest\"},{\"test\":18,\"bool\":true,\"str\":\"strtes2t\"}]";
    //const char* js = "[15,25,158,11,66,12]";
   /* const char* js = "{\"widget\": {\
    \"debug\": \"on\",\
    \"window\": {\
        \"title\": \"Sample Konfabulator Widget\",\
        \"name\": \"main_window\",\
        \"width\": 500,\
        \"height\": 500\
    },\
    \"image\": { \
        \"src\": \"Images/Sun.png\",\
        \"name\": \"sun1\",\
        \"hOffset\": 250,\
        \"vOffset\": 250,\
        \"alignment\": \"center\"\
    },\
    \"text\": {\
        \"data\": \"Click Here\",\
        \"size\": 36,\
        \"style\": \"bold\",\
        \"name\": \"text1\",\
        \"hOffset\": 250,\
        \"vOffset\": 100,\
        \"alignment\": \"center\",\
        \"onMouseUp\": \"sun1.opacity = (sun1.opacity / 100) * 90;\"\
    }\
}} ";*/

    /*const char* js = "{\"web-app\": {\
  \"servlet\": [   \
    {\
      \"servlet-name\": \"cofaxCDS\",\
      \"servlet-class\": \"org.cofax.cds.CDSServlet\",\
      \"init-param\": {\
        \"configGlossary:installationAt\": \"Philadelphia, PA\",\
        \"configGlossary:adminEmail\": \"ksm@pobox.com\",\
        \"configGlossary:poweredBy\": \"Cofax\",\
        \"configGlossary:poweredByIcon\": \"/images/cofax.gif\",\
        \"configGlossary:staticPath\": \"/content/static\",\
        \"templateProcessorClass\": \"org.cofax.WysiwygTemplate\",\
        \"templateLoaderClass\": \"org.cofax.FilesTemplateLoader\",\
        \"templatePath\": \"templates\",\
        \"templateOverridePath\": \"\",\
        \"defaultListTemplate\": \"listTemplate.htm\",\
        \"defaultFileTemplate\": \"articleTemplate.htm\",\
        \"useJSP\": false,\
        \"jspListTemplate\": \"listTemplate.jsp\",\
        \"jspFileTemplate\": \"articleTemplate.jsp\",\
        \"cachePackageTagsTrack\": 200,\
        \"cachePackageTagsStore\": 200,\
        \"cachePackageTagsRefresh\": 60,\
        \"cacheTemplatesTrack\": 100,\
        \"cacheTemplatesStore\": 50,\
        \"cacheTemplatesRefresh\": 15,\
        \"cachePagesTrack\": 200,\
        \"cachePagesStore\": 100,\
        \"cachePagesRefresh\": 10,\
        \"cachePagesDirtyRead\": 10,\
        \"searchEngineListTemplate\": \"forSearchEnginesList.htm\",\
        \"searchEngineFileTemplate\": \"forSearchEngines.htm\",\
        \"searchEngineRobotsDb\": \"WEB-INF/robots.db\",\
        \"useDataStore\": true,\
        \"dataStoreClass\": \"org.cofax.SqlDataStore\",\
        \"redirectionClass\": \"org.cofax.SqlRedirection\",\
        \"dataStoreName\": \"cofax\",\
        \"dataStoreDriver\": \"com.microsoft.jdbc.sqlserver.SQLServerDriver\",\
        \"dataStoreUrl\": \"jdbc:microsoft:sqlserver://LOCALHOST:1433;DatabaseName=goon\",\
        \"dataStoreUser\": \"sa\",\
        \"dataStorePassword\": \"dataStoreTestQuery\",\
        \"dataStoreTestQuery\": \"SET NOCOUNT ON;select test='test';\",\
        \"dataStoreLogFile\": \"/usr/local/tomcat/logs/datastore.log\",\
        \"dataStoreInitConns\": 10,\
        \"dataStoreMaxConns\": 100,\
        \"dataStoreConnUsageLimit\": 100,\
        \"dataStoreLogLevel\": \"debug\",\
        \"maxUrlLength\": 500}},\
    {\
      \"servlet-name\": \"cofaxEmail\",\
      \"servlet-class\": \"org.cofax.cds.EmailServlet\",\
      \"init-param\": {\
      \"mailHost\": \"mail1\",\
      \"mailHostOverride\": \"mail2\"}},\
    {\
      \"servlet-name\": \"cofaxAdmin\",\
      \"servlet-class\": \"org.cofax.cds.AdminServlet\"},\
 \
    {\
      \"servlet-name\": \"fileServlet\",\
      \"servlet-class\": \"org.cofax.cds.FileServlet\"},\
    {\
      \"servlet-name\": \"cofaxTools\",\
      \"servlet-class\": \"org.cofax.cms.CofaxToolsServlet\",\
      \"init-param\": {\
        \"templatePath\": \"toolstemplates/\",\
        \"log\": 1,\
        \"logLocation\": \"/usr/local/tomcat/logs/CofaxTools.log\",\
        \"logMaxSize\": \"\",\
        \"dataLog\": 1,\
        \"dataLogLocation\": \"/usr/local/tomcat/logs/dataLog.log\",\
        \"dataLogMaxSize\": \"\",\
        \"removePageCache\": \"/content/admin/remove?cache=pages&id=\",\
        \"removeTemplateCache\": \"/content/admin/remove?cache=templates&id=\",\
        \"fileTransferFolder\": \"/usr/local/tomcat/webapps/content/fileTransferFolder\",\
        \"lookInContext\": 1,\
        \"adminGroupID\": 4,\
        \"betaServer\": true}}],\
  \"servlet-mapping\": {\
    \"cofaxCDS\": \"/\",\
    \"cofaxEmail\": \"/cofaxutil/aemail/*\",\
    \"cofaxAdmin\": \"/admin/*\",\
    \"fileServlet\": \"/static/*\",\
    \"cofaxTools\": \"/tools/*\"},\
 \
  \"taglib\": {\
    \"taglib-uri\": \"cofax.tld\",\
    \"taglib-location\": \"/WEB-INF/tlds/cofax.tld\"}}}";
    */

  //  JObj* root = JParse(js);
    //delete root;


   
    
}