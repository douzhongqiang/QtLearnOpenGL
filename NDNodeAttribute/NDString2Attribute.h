#ifndef NDSTRING2ATTRIUTE_H
#define NDSTRING2ATTRIUTE_H

#include "NDAttributeBase.h"
#include "ndnodeattribute_global.h"

class NDNODEATTRIBUTESHARED_EXPORT NDString2Attribute : public NDAttributeBase
{
    Q_OBJECT

public:
    NDString2Attribute(NDNodeBase* parentNode = nullptr);
    ~NDString2Attribute();

    AttributeType Type(void) const override{
        return t_string2;
    }

    void setStringList(const QStringList& strList);
    void setStringList(const QList<QPair<QString, QVariant>>& strList);
    const QList<QPair<QString, QVariant>>& getStringList(void);

    QString getCurrentSelectedString(void);
    QVariant getCurrentSelectedData(void);

    // 设置/获取当前选择的索引
    void setCurrentSelectedIndex(int index);
    int getCurrentSelectedIndex(void);

private:
    QList<QPair<QString, QVariant>> m_currentList;
    int m_currentSelectedIndex = -1;

signals:
    void currentSelectedIndexChanged(int index);
};

#endif
