#include "NDString2Attribute.h"

NDString2Attribute::NDString2Attribute(NDNodeBase* parentNode)
    :NDAttributeBase(parentNode)
{

}

NDString2Attribute::~NDString2Attribute()
{

}

void NDString2Attribute::setStringList(const QStringList& strList)
{
    m_currentList.clear();
    for (auto iter = strList.begin(); iter != strList.end(); ++iter)
    {
        QPair<QString, QVariant> pair(*iter, QVariant());
        m_currentList.push_back(pair);
    }

    if (m_currentList.size() > 0)
    {
        setCurrentSelectedIndex(0);
    }
    else
        m_currentSelectedIndex = -1;
}

void NDString2Attribute::setStringList(const QList<QPair<QString, QVariant>>& strList)
{
    m_currentList.clear();
    for (auto iter = strList.begin(); iter != strList.end(); ++iter)
    {
        m_currentList.push_back(*iter);
    }

    if (m_currentList.size() > 0)
    {
        setCurrentSelectedIndex(0);
    }
    else
        m_currentSelectedIndex = -1;
}

const QList<QPair<QString, QVariant>>& NDString2Attribute::getStringList(void)
{
    return m_currentList;
}

QString NDString2Attribute::getCurrentSelectedString(void)
{
    if (m_currentSelectedIndex < 0 || m_currentSelectedIndex >= m_currentList.size())
        return "";

    return m_currentList[m_currentSelectedIndex].first;
}

QVariant NDString2Attribute::getCurrentSelectedData(void)
{
    if (m_currentSelectedIndex < 0 || m_currentSelectedIndex >= m_currentList.size())
        return QVariant();

    return m_currentList[m_currentSelectedIndex].second;
}

void NDString2Attribute::setCurrentSelectedIndex(int index)
{
    if (m_currentSelectedIndex == index)
        return;

    m_currentSelectedIndex = index;
    this->setValue(getCurrentSelectedString());
    emit currentSelectedIndexChanged(m_currentSelectedIndex);
}

int NDString2Attribute::getCurrentSelectedIndex(void)
{
    return m_currentSelectedIndex;
}
