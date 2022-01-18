#include "VariantDB.h"

VariantDB::VariantDB() {
    ResetNext();
}

VariantDB::~VariantDB() {
    DeleteAll();
}

void VariantDB::DeleteAll() {
    {
        dataList::iterator itor = m_data.begin();
        while (itor != m_data.end()) {
            if (itor->second) {
                delete (itor->second);
                itor->second = nullptr;
            }

            itor++;
        }
    }

    m_data.clear();

    { // So I can use "itor" again
        functionList::iterator itor = m_functionData.begin();
        while (itor != m_functionData.end()) {
            delete (itor->second);
            itor++;
        }
    }

    m_functionData.clear();
}


int VariantDB::DeleteVar(std::string keyName) {
    int deleted = 0;

    dataList::iterator itor = m_data.begin();
    while (itor != m_data.end()) {
        if (itor->first == keyName) {
            // Match!
            delete (itor->second);
            dataList::iterator itorTemp = itor;
            itor++;

            m_data.erase(itorTemp);
            deleted++;
            continue;
        }

        itor++;
    }
    return deleted;
}

Variant* VariantDB::GetVarIfExists(const std::string &keyName) {
    dataList::iterator itor = m_data.find(keyName);
    if (itor != m_data.end()) {
        // Bingo!
        return &((*itor->second));
    }
    return NULL;
}

Variant* VariantDB::GetVarWithDefault(const std::string &keyName, const Variant &vDefault) {
    Variant *pData = GetVarIfExists(keyName);
    if (!pData) {
        // Create it
        pData = new Variant(vDefault);
        m_data[keyName] = pData;
    }
    return pData;
}

Variant* VariantDB::GetVar(const std::string &keyName) {
    Variant *pData = GetVarIfExists(keyName);
    if (!pData) {
        // Create it
        pData = new Variant;
        m_data[keyName] = pData;
    }
    return pData;
}

FunctionObject * VariantDB::GetFunctionIfExists(const std::string &keyName) {
    functionList::iterator itor = m_functionData.find(keyName);
    if (itor != m_functionData.end()) {
        // Bingo!
        return &( (*itor->second));
    }
    return NULL; // Doesn't exist
}

FunctionObject* VariantDB::GetFunction(const std::string &keyName) {
    FunctionObject *pData = GetFunctionIfExists(keyName);
    if (!pData) {
        // Create it
        pData = new FunctionObject;
        m_functionData[keyName] = pData;
    }
    return pData;
}

void VariantDB::CallFunctionIfExists(const std::string &keyName, VariantList *pVList) {
    FunctionObject *pFunc = GetFunctionIfExists(keyName);
    if (pFunc) {
        pFunc->sig_function(pVList);
    }
}

bool VariantDB::Save(const std::string &fileName, bool bAddBasePath) {
    // Unused
    return false;
}

bool VariantDB::Load(const std::string &fileName, bool *pFileExistedOut, bool bAddBasePath) {
    // Unused
    return false;
}

void VariantDB::Print() {
    // Unused
}

int VariantDB::DeleteVarsStartingWith(std::string deleteStr) {
    int deleted = 0;

    dataList::iterator itor = m_data.begin();
    while (itor != m_data.end()) {
        if (itor->first.compare(0, deleteStr.size(), deleteStr) == 0) {
            // Match!
            delete (itor->second);
            dataList::iterator itorTemp = itor;
            itor++;

            m_data.erase(itorTemp);
            deleted++;
            continue;
        }
        itor++;
    }
    return deleted;
}

std::string VariantDB::DumpAsString() {
    // Unused
    std::string log = "*********************\r\n";
    return log;
}

void VariantDB::ResetNext() {
    m_nextItor = m_data.begin();
}

Variant* VariantDB::GetNext(std::string &keyOut) {
    Variant *pReturn = NULL;

    if (m_nextItor == m_data.end()) {
        // All done
        ResetNext();
        return NULL;
    }

    keyOut = m_nextItor->first;
    pReturn = m_nextItor->second;
    m_nextItor++;
    return pReturn;
}

bool StringFromStartMatches(const std::string &line, const std::string textToMatch) {
    for (uint32_t i=0; i < textToMatch.size(); i++) {
        if (i >= line.length()) return false;
        if (line[i] != textToMatch[i]) return false;
    }
    return true;
}

int VariantDB::AddVarPointersToVector(std::vector<std::pair<const std::string*, Variant*>>* varListOut, const std::string keyMustStartWithThisText) {
    int count = 0;
    dataList::iterator itor = m_data.begin();

    while (itor != m_data.end()) {
        // Variant *pV = itor->second;
        if (keyMustStartWithThisText.empty() || StringFromStartMatches(itor->first, keyMustStartWithThisText)) {
            varListOut->push_back(std::make_pair(&itor->first, itor->second));
            count++;
        }

        itor++;
    }
    return count;
}

void VariantDB::Clear() {
    m_data.clear();
    m_functionData.clear();
    ResetNext();
}