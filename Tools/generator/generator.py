#!/usr/bin/python3
import sys, os, time
import ctypes
import sys, os, time
from jinja2 import Template
from field_base import FieldBase
from field_base import FieldUint32
from field_base import FieldInt
from field_base import FieldInt64
from field_base import FieldTime
from field_base import FieldString
from field_base import FieldBool
import xml.dom.minidom as minidom

kernel32 = ctypes.windll.kernel32
kernel32.SetConsoleMode(kernel32.GetStdHandle(-11),7)
#encoding:utf-8

def pwarning(txt):
    print('\033[5;33m' + txt + '\033[0m')

def perror(txt):
    print('\033[1;31m' + txt + '\033[0m')

class Document:
    def __init__(self):
        self.m_strFirstTableName = ''                               #一级表名
        self.m_strSecTableName = ''                                 #二级表名
        self.m_strSecTableType = ''                                 #二级表名类型(object|list)
        self.m_strClassName = ''                                    #生成类名
        self.m_strCacheName = ''                                    #生成操作内存数据的结构体名字
        self.m_strCacheNameValueType = ''                           #生成内存数据结构体名字
        self.m_strSyncIns = ''
        self.m_strFileName = ''                                     #生成的文件名

        self.m_stFieldList = []                                     #字段集合
        self.m_stEndVec = ['.xml']

    def InitFromXml(self, stXmlPath):                               #将xml数据load到内存中
        if not os.path.isfile(stXmlPath):
            print(stXmlPath,'集合配置文件路径不存在:%s'%(stXmlPath))
            sys.exit()
        dom = minidom.parse(stXmlPath)
        docInfo = dom.documentElement                       #获取文档信息节点(根节点)
        self.m_strFirstTableName = docInfo.getAttribute('first_table_name')
        self.m_strSecTableName = docInfo.getAttribute('sec_table_name')
        self.m_strSecTableType = docInfo.getAttribute('sec_table_type')
        self.m_strClassName = docInfo.getAttribute('class_name')
        self.m_strCacheName = docInfo.getAttribute('cache_name')
        self.m_strCacheNameValueType = self.m_strCacheName[0:-2] + '_value_type'
        self.m_strSyncIns = docInfo.getAttribute('sync_ins')
        self.m_strFileName = docInfo.getAttribute('file_name')
        #获取每个字段信息节点
        store = []
        fieldList = docInfo.getElementsByTagName('field_info')
        for fieldInfo in fieldList:
            strName = fieldInfo.getAttribute('name')
            strType = fieldInfo.getAttribute('type')
            strFKey = fieldInfo.getAttribute('first_key')
            strSKey = fieldInfo.getAttribute('sec_key')
            strDft = fieldInfo.getAttribute('default')
            strComment = fieldInfo.getAttribute('comment')
            cFiledBase = FieldBase(strName, strType, strFKey, strSKey, strDft, strComment)
            if strType == 'uint32_t':
                cFiledBase = FieldUint32(strName, strType, strFKey, strSKey, strDft, strComment)
            elif strType == 'int':
                cFiledBase = FieldInt(strName, strType, strFKey, strSKey, strDft, strComment)
            elif strType == 'int64_t':
                cFiledBase = FieldInt64(strName, strType, strFKey, strSKey, strDft, strComment)
            elif strType == 'time_t':
                cFiledBase = FieldTime(strName, strType, strFKey, strSKey, strDft, strComment)
            elif strType == 'string':
                cFiledBase = FieldString(strName, strType, strFKey, strSKey, strDft, strComment)
            elif strType == 'bool':
                 cFiledBase = FieldBool(strName, strType, strFKey, strSKey, strDft, strComment)
            else:
                print('错误的配置字段类型%s不存在' % (strType))
                sys.exit()
            #初始化每个字段信息    
            cFiledBase.GenerateName()
            self.m_stFieldList.append(cFiledBase)

    def  GenUserH(self, strCodePath):
        with open('../../Tools/generator/tpl_user_h.j2', 'r', encoding='utf-8') as includeFile:
            buffer = includeFile.read()
            template = Template(buffer)
    
            data = {
                'file_name' : self.m_strFileName,
                'first_name' : self.m_strFirstTableName,
                'sec_name' : self.m_strSecTableName,
                'sec_type' : self.m_strSecTableType,
                'class_name' : self.m_strClassName,
                'cache_name' : self.m_strCacheName,
                'cache_value_name' : self.m_strCacheNameValueType,
                'field_list' : self.m_stFieldList,
                'sync_ins' : self.m_strSyncIns,
            }  
            wf = open(strCodePath + self.m_strFileName + ".h", 'w') 
            wf.write(template.render(data)) 
            wf.close()

    def GenUserCpp(self, strCodePath):
        with open('../../Tools/generator/tpl_user_cpp.j2', 'r', encoding='utf-8') as cppFile:
            buffer = cppFile.read()
            template = Template(buffer)

            data = {
                'file_name' : self.m_strFileName,
                'first_name' : self.m_strFirstTableName,
                'sec_name' : self.m_strSecTableName,
                'sec_type' : self.m_strSecTableType,
                'class_name' : self.m_strClassName,
                'cache_name' : self.m_strCacheName,
                'cache_value_name' : self.m_strCacheNameValueType,
                'field_list' : self.m_stFieldList,
                'sync_ins' : self.m_strSyncIns,
            }
            wf = open(strCodePath + self.m_strFileName + ".cpp", 'w')
            wf.write(template.render(data)) 
            wf.close()

    def  GenGlobalH(self, strCodePath):
        with open('../../Tools/generator/tpl_global_h.j2', 'r', encoding='utf-8') as includeFile:
            buffer = includeFile.read()
            template = Template(buffer)
    
            data = {
                'file_name' : self.m_strFileName,
                'first_name' : self.m_strFirstTableName,
                'sec_type' : self.m_strSecTableType,
                'class_name' : self.m_strClassName,
                'cache_name' : self.m_strCacheName,
                'cache_value_name' : self.m_strCacheNameValueType,
                'field_list' : self.m_stFieldList,
                'sync_ins' : self.m_strSyncIns,
            }  
            wf = open(strCodePath + self.m_strFileName + ".h", 'w') 
            wf.write(template.render(data)) 
            wf.close()

    def GenGlobalCpp(self, strCodePath):
        with open('../../Tools/generator/tpl_global_cpp.j2', 'r', encoding='utf-8') as cppFile:
            buffer = cppFile.read()
            template = Template(buffer)

            data = {
                'file_name' : self.m_strFileName,
                'first_name' : self.m_strFirstTableName,
                'sec_type' : self.m_strSecTableType,
                'class_name' : self.m_strClassName,
                'cache_name' : self.m_strCacheName,
                'cache_value_name' : self.m_strCacheNameValueType,
                'field_list' : self.m_stFieldList,
                'sync_ins' : self.m_strSyncIns,
            }
            wf = open(strCodePath + self.m_strFileName + ".cpp", 'w')
            wf.write(template.render(data)) 
            wf.close()
    
    def IsExcelFile(self, strFile):
        for end in self.m_stEndVec:
            if strFile.endswith(end) == True:
                return end
        return False

def main(argv):
    if len(sys.argv) < 2:
        print('usage : ', argv[0], ' all')                  #执行全部
        print('usage : ', argv[0], ' generate')             #生成.h和.cpp文件
        print('usage : ', argv[0], ' index')                #生成mongo中索引

    stCmd = argv[1]
    strXmlPath = "../../Config/table"                       #mongo中doc的配置路径
    strConfigPath = "../../Config/db/mongo_config.xml"      #mongo配置路径(ip&port&user等等)  
    strCodePath = "../../GameServer/"                       #生成.h和.cpp文件路径

    xmlPathDict = {}                                       #读取所有doc配置文件路径
    for mainDir, subDir, nameList in os.walk(strXmlPath):
        for name in nameList:
            xmlPathDict[name] = mainDir + "/" + name

    if stCmd == "gen":                                      #生成对应.h和.cpp文件
        stEndVec = ['.xls', '.xlsx']
        for key in xmlPathDict :
            stDocument = Document()
            bEnd = stDocument.IsExcelFile(xmlPathDict[key])
            if (bEnd == False):
                continue
            stDocument.InitFromXml(xmlPathDict[key])
            nameList = key.split('_')
            if nameList[0] == 'user':
                print("begin generate user doc:", xmlPathDict[key])
                stDocument.GenUserH(strCodePath)
                stDocument.GenUserCpp(strCodePath)
            elif nameList[0] == 'global':
                 print("begin generate global doc:", xmlPathDict[key])
                 stDocument.GenGlobalH(strCodePath)
                 stDocument.GenGlobalCpp(strCodePath)
            else :
                print("error xml file:", xmlPathDict[key])


    else:
        print("error cmd, plaese use gen|create|update")


if __name__ == "__main__":
    main(sys.argv)
    time.sleep(5)