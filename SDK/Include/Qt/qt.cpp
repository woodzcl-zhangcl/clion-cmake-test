// Copyright (c) 2012-2018 The woodzcl Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

//
// Created by woodzcl on 2018/8/10.
//

#include "Qt/qt.h"

#include <stdio.h>
#include <qstring.h>

#include <qfile.h>
#include <qwindowdefs.h>

bool StructureParser::startDocument() {
    indent = "";
    return true;
}

bool StructureParser::startElement(const QString &, const QString &,
                                   const QString &qName,
                                   const QXmlAttributes &) {
    printf("%s%s\n", indent.toStdString().c_str(), qName.toStdString().c_str());
    indent += "    ";
    return true;
}

bool StructureParser::endElement(const QString &, const QString &, const QString &) {
    indent.remove((uint) 0, 4);
    return true;
}

void sax2_parse(const char *xml_file) {
    StructureParser handler;
    QXmlSimpleReader reader;
    reader.setContentHandler(&handler);

    QFile xmlFile(xml_file);
    QXmlInputSource source(&xmlFile);
    reader.parse(source);
}