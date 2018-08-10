// Copyright (c) 2012-2018 The woodzcl Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

//
// Created by woodzcl on 2018/8/10.
//

#include <qxml.h>

#ifndef SDK_QT_H
#define SDK_QT_H

class QString;

class StructureParser : public QXmlDefaultHandler
{
public:
    bool startDocument();
    bool startElement( const QString&, const QString&, const QString& ,
                       const QXmlAttributes& );
    bool endElement( const QString&, const QString&, const QString& );

private:
    QString indent;
};

#ifdef __cplusplus
extern "C" {
#endif

void sax2_parse(const char *xml_file);

#ifdef __cplusplus
}
#endif

#endif //SDK_QT_H
