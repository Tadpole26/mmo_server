#!/usr/bin/python3

class FieldBase:
    def __init__(self, strName, strType, bFKey, bSKey, default, strComment):
        self.m_strType = strType         #字段类型(int|bool|uint64_t|std::string|userdata)
        self.m_strOriName = strName      #xml原始名字(aa_bb_cc)
        self.m_strFKey = True if bFKey == 'true' else False
        self.m_strSKey = True if bSKey == 'true' else False
        self.m_strDft = default
        self.m_strComment = strComment

        nameList = self.m_strOriName.split('_')
        TmpName = ''
        for name in nameList:
            TmpName = TmpName + name[:1].upper() + name[1:]
        self.m_strBaseName = TmpName                    #基础名字(AaBbCc)
        self.m_strPartName = ''                         #局部变量名字(iAaBbCc)
        self.m_strMemName = ''                          #成员变量名字(m_iAaBbCc)
        self.m_strConstName = 's_str' + TmpName         #字段内存名字(s_strAaBbCc)
    
    def GenerateName(self):
        self.m_strPartName = self.m_strBaseName
        self.m_strMemName = self.m_strBaseName

######################################uint32#####################################
class FieldUint32(FieldBase):
    def GenerateName(self):
        self.m_strPartName = 'ui' + self.m_strBaseName
        self.m_strMemName = 'm_ui' + self.m_strBaseName

######################################int32######################################
class FieldInt(FieldBase):
    def GenerateName(self):
        self.m_strPartName = 'i' + self.m_strBaseName
        self.m_strMemName = 'm_i' + self.m_strBaseName

######################################int64######################################
class FieldInt64(FieldBase):
    def GenerateName(self):
        self.m_strPartName = 'll' + self.m_strBaseName
        self.m_strMemName = 'm_ll' + self.m_strBaseName

######################################time_t######################################
class FieldTime(FieldBase):
    def GenerateName(self):
        self.m_strPartName = 'll' + self.m_strBaseName
        self.m_strMemName = 'm_ll' + self.m_strBaseName

######################################string######################################
class FieldString(FieldBase):
    def GenerateName(self):
        self.m_strPartName = 'str' + self.m_strBaseName
        self.m_strMemName = 'm_str' + self.m_strBaseName

#######################################bool########################################
class FieldBool(FieldBase):
    def GenerateName(self):
        self.m_strPartName = 'b' + self.m_strBaseName
        self.m_strMemName = 'm_b' + self.m_strBaseName